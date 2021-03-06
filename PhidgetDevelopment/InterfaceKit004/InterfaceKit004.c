/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       InterfaceKit004.c                                                       */
/*                                                                                      */
/*  Contains:   The main entry points for this phidget.                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2004 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
/*                                                                                      */
/*              All rights reserved. Redistribution and use in source and binary forms, */
/*              with or without modification, are permitted provided that the following */
/*              conditions are met:                                                     */
/*                * Redistributions of source code must retain the above copyright      */
/*                  notice, this list of conditions and the following disclaimer.       */
/*                * Redistributions in binary form must reproduce the above copyright   */
/*                  notice, this list of conditions and the following disclaimer in the */
/*                  documentation and/or other materials provided with the              */
/*                  distribution.                                                       */
/*                * Neither the name of the copyright holders nor the names of its      */
/*                  contributors may be used to endorse or promote products derived     */
/*                  from this software without specific prior written permission.       */
/*                                                                                      */
/*              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     */
/*              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       */
/*              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A */
/*              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      */
/*              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   */
/*              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        */
/*              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   */
/*              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   */
/*              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     */
/*              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   */
/*              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    */
/*                                                                                      */
/*  Created:    2004/01/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
//#define USE_DEFAULT /* */

#include "InterfaceKit004.h"
#include "genericListOutput.h"

static const int    MAX_INDEX = 3;
static const int    DIGITAL_REPORT_SIZE = MAX_INDEX + 4;
static const int    EXPECTED_OUTPUT_SIZE = 32;

static PSymbol    lFlipSymbol = NULL_PTR;
static PSymbol    lOffSymbol = NULL_PTR;
static PSymbol    lOnSymbol = NULL_PTR;

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ defineCallback ---*/
E_PhidgResult
defineCallback(STANDARD_PHID_ARGS_DEFINECALLBACK)
{
 #pragma unused(name,sharedStorage,privateStorage,thisDevice,aFun,cookies,numCookies)
    *result = noErr;
    return kPhidgSuccess;
} /* defineCallback */
#endif /* COMPILE_FOR_OSX_4 */
    
/*------------------------------------ doCustom ---*/
E_PhidgResult
doCustom(STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(outlet)
    PrivatePtr    privateData = reinterpret_cast<PrivatePtr>(privateStorage);
    SharedPtr        sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
    
    *result = noErr;
    if (argc == 2)
    {
        bool    okSoFar = true, doFlip = false, doOff = false;
        long    index;
        
        if (argv[0].a_type == A_SYM)
        {
            if (argv[0].a_w.w_sym == lFlipSymbol)
                doFlip = true;
            else if (argv[0].a_w.w_sym == lOffSymbol)
                doOff = true;    
            else if (argv[0].a_w.w_sym != lOnSymbol)
            {
                okSoFar = false;
                LOG_ERROR_4("%s: unknown operation ('%s') for '%s:do'", name, argv[0].a_w.w_sym->s_name,
                                        deviceType->s_name)
            }
        }
        else
        {
            okSoFar = false;
            LOG_ERROR_3("%s: operation is not a symbol for '%s:do'", name, deviceType->s_name)
        }
        if (okSoFar)
        {
            if (argv[1].a_type == A_LONG)
            {
                index = argv[1].a_w.w_long - 1;
                if (index < 0)
                    index = 0;
                else if (index > MAX_INDEX)
                    index = MAX_INDEX;
            }
            else
            {
                okSoFar = false;
                LOG_ERROR_3("%s: non-integer value for '%s:do'", name, deviceType->s_name)
            }
        }
        if (okSoFar)
        {
          HIDElementDataPtr    anElement = NULL_PTR;
          
          // Find the matching element:
             for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
             {
                 if (anElement->fCookie == sharedData->fOutputCookie)
                     break;
                     
             }
          if (anElement)
          {
              long            singleValue = privateData->fDigitalOutputs;
 #if defined(COMPILE_FOR_OSX_4)
                IOReturn    result2;
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
                OSStatus    result2;
 #endif /* COMPILE_FOR_OS9_4 */
                
                if (doFlip)
                    singleValue ^= (1 << index);
                else if (doOff)
                    singleValue &= ~ (1 << index);
                else
                    singleValue |= (1 << index);
                setHIDElementValue(name, *thisDevice, *anElement, 0, NULL_PTR, singleValue, result2);
                privateData->fDigitalOutputs = singleValue;
                *result = static_cast<OSErr>(result2);
          }
          else
              LOG_ERROR_3("%s: element not found for '%s:do'", name, deviceType->s_name)
        }
    }
    else if (argc == 1)
        LOG_ERROR_3("%s: missing value for '%s:do'", name, deviceType->s_name)
    else if (argc)
        LOG_ERROR_3("%s: extra values for '%s:do'", name, deviceType->s_name)
    else
        LOG_ERROR_3("%s: missing operation and value for '%s:do'", name, deviceType->s_name)
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doCustom */

/*------------------------------------ doGet ---*/
E_PhidgResult
doGet(STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(argv)
    PrivatePtr    privateData = reinterpret_cast<PrivatePtr>(privateStorage);
    SharedPtr        sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
 
    if (argc)
        LOG_ERROR_3("%s: extra arguments for '%s:get'", name, deviceType->s_name)
    else
    {
        // report the state of the digital output
        Atom    reportList[DIGITAL_REPORT_SIZE];

        SETSYM(reportList, deviceType);
        SETSYM(reportList + 1, thisDevice->fSerialNumber);
        SETLONG(reportList + 2, privateData->fDigitalOutputs);
        for (UInt32 ii = 0; ii <= MAX_INDEX; ++ii)
            SETLONG(reportList + ii + 3, (privateData->fDigitalOutputs & (1 << ii)) ? 1 : 0);
        genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
    }
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doGet */

/*------------------------------------ doPut ---*/
E_PhidgResult
doPut(STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(outlet)
    PrivatePtr    privateData = reinterpret_cast<PrivatePtr>(privateStorage);
    SharedPtr        sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
    
    *result = noErr;
    if (argc == 1)
    {
        bool    okSoFar = true;
      long    singleValue = 0;

        if (argv[0].a_type == A_LONG)
        {
            singleValue = argv[0].a_w.w_long;
            if (singleValue < 0)
                singleValue = 0;
            else if (singleValue > 0x000F)
                singleValue = 0x000F;
        }
        else
        {
            okSoFar = false;
            LOG_ERROR_3("%s: non-integer value for '%s:put'", name, deviceType->s_name)
        }
        if (okSoFar)
        {
          HIDElementDataPtr    anElement = NULL_PTR;
          
          // Find the matching element:
             for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
             {
                 if (anElement->fCookie == sharedData->fOutputCookie)
                     break;
                     
             }
          if (anElement)
          {
 #if defined(COMPILE_FOR_OSX_4)
                IOReturn    result2;
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
                OSStatus    result2;
 #endif /* COMPILE_FOR_OS9_4 */
                
                setHIDElementValue(name, *thisDevice, *anElement, 0, NULL_PTR, singleValue, result2);
                privateData->fDigitalOutputs = singleValue;
                *result = static_cast<OSErr>(result2);
          }
          else
              LOG_ERROR_3("%s: element not found for '%s:put'", name, deviceType->s_name)
        }
    }
    else if (argc)
        LOG_ERROR_3("%s: extra values for '%s:put'", name, deviceType->s_name)
    else
        LOG_ERROR_3("%s: missing value for '%s:put'", name, deviceType->s_name)
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doPut */

/*------------------------------------ identify ---*/
OSErr
identify(STANDARD_PHID_ARGS_IDENTIFY)
{
#if defined(COMPILE_FOR_OSX_4)
 #pragma unused(name,isAsynchronous)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
 #pragma unused(name)
#endif /* COMPILE_FOR_OS9_4 */
    *productID = 0x040;
    *privateSize = sizeof(PrivateData);
    *sharedSize = sizeof(SharedData);
  return noErr;
} /* identify */

/*------------------------------------ main ---*/
OSErr
main(STANDARD_PHID_ARGS_MAIN)
{
#if defined(COMPILE_FOR_OSX_4)
 #pragma unused(name)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
 #pragma unused(name,environment)
#endif /* COMPILE_FOR_OS9_4 */
    STANDARD_MAIN_CODE;
    SharedPtr    sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
    
    if (sharedData)
        sharedData->fCookiesValid = false;
    lFlipSymbol = gensym("flip");
    lOffSymbol = gensym("off");
    lOnSymbol = gensym("on");
  return noErr;
} /* main */

/*------------------------------------ niam ---*/
OSErr
niam(STANDARD_PHID_ARGS_NIAM)
{
#pragma unused(name,sharedStorage)
  return noErr;
} /* niam */

/*------------------------------------ onAttach ---*/
E_PhidgResult
onAttach(STANDARD_PHID_ARGS_ATTACH)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType)
    PrivatePtr    privateData = reinterpret_cast<PrivatePtr>(privateStorage);
    SharedPtr        sharedData = reinterpret_cast<SharedPtr>(sharedStorage);

    if (privateData)
        privateData->fDigitalOutputs = 0;
    if (sharedData)
    {
        if (! sharedData->fCookiesValid)
        {
            bool    foundOut = false;
            
            // Locate the input and output elements
            for (HIDElementDataPtr walker = thisDevice->fFirstElement; walker;
                        walker = walker->fNext)
            {
                switch (walker->fType)
                {
                    case kIOHIDElementTypeOutput:
                        if (walker->fSize == EXPECTED_OUTPUT_SIZE)
                        {
                            foundOut = true;
                            sharedData->fOutputCookie = walker->fCookie;
                        }
                        break;

                }        
            }
            sharedData->fCookiesValid = foundOut;
        }
    }
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* onAttach */

/*------------------------------------ onDetach ---*/
E_PhidgResult
onDetach(STANDARD_PHID_ARGS_ATTACH)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* onDetach */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ reportHandler ---*/
void
reportHandler(STANDARD_PHID_ARGS_REPORTHANDLER)
{
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice,outlet,inHIDReport,inHIDReportLength)
} /* reportHandler */
#endif /* COMPILE_FOR_OS9_4 */
