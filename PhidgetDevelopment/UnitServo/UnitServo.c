/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       UnitServo.c                                                             */
/*                                                                                      */
/*  Contains:   The main entry points for this phidget.                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/12/20                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
//#define USE_DEFAULT /* */

#include "UnitServo.h"
#include "genericListOutput.h"

static const int    SERVO_REQUEST_SIZE = 6;
static const int    EXPECTED_INPUT_SIZE = 16;
static const int    EXPECTED_OUTPUT_SIZE = 48;

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
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
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
 #pragma unused(sharedStorage,argv)
    PrivatePtr    privateData = reinterpret_cast<PrivatePtr>(privateStorage);

    if (argc)
        LOG_ERROR_3("%s: extra arguments for '%s:get'", name, deviceType->s_name)
    else
    {
        Atom    reportList[3];
        
        SETSYM(reportList, deviceType);
        SETSYM(reportList + 1, thisDevice->fSerialNumber);
        SETFLOAT(reportList + 2, privateData->fServo);
        genericListOutput(outlet, 3, reportList);
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
        float    percentage;
        
        if (argv[2].a_type == A_LONG)
            percentage = argv[2].a_w.w_long;
        else if (argv[2].a_type == A_FLOAT)
            percentage = argv[2].a_w.w_float;
        else
        {
            LOG_ERROR_3("%s: percentage is not a number for '%s:get'", name, deviceType->s_name)
            okSoFar = false;
        }
        if (percentage < 0)
            percentage = 0;
        else if (percentage > 100)
            percentage = 100;
        if (okSoFar)
        {
            int        pulse1 = int((percentage * 180 * 10.6f / 100) + 230); 
            Atom    servoVector[SERVO_REQUEST_SIZE];
            
            SETLONG(servoVector, pulse1 % 256);
            SETLONG(servoVector + 1, pulse1 / 256);
            SETLONG(servoVector + 2, 0);
            SETLONG(servoVector + 3, 0);
            SETLONG(servoVector + 4, 0);
            SETLONG(servoVector + 5, 0);
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

                    setHIDElementValue(name, *thisDevice, *anElement, SERVO_REQUEST_SIZE, servoVector,
                                                            0, result2);
                    privateData->fServo = percentage;
                    *result = static_cast<OSErr>(result2);
              }
              else
                  LOG_ERROR_3("%s: element not found for '%s:put'", name, deviceType->s_name)
            }
        }
    }
    else
        LOG_ERROR_3("%s: missing or extra arguments for '%s:get'", name, deviceType->s_name)
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
    *productID = 0x039;
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
 #pragma unused(name,sharedStorage,deviceType)
  PrivatePtr    privateData = reinterpret_cast<PrivatePtr>(privateStorage);
    SharedPtr        sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
    
    if (privateData)
        privateData->fServo = 0;
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
