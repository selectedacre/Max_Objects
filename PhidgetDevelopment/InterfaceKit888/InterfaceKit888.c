/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       InterfaceKit888.c                                                       */
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
/*  Created:    2003/12/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
//#define USE_DEFAULT /* */

#include "InterfaceKit888.h"
#include "genericListOutput.h"

static const int    MAX_INDEX = 7;
static const int    DIGITAL_REPORT_SIZE = MAX_INDEX + 5;
static const int    EXPECTED_INPUT_SIZE = 64;
static const int    EXPECTED_OUTPUT_SIZE = 32;

static PSymbol    lAiSymbol = NULL_PTR;
static PSymbol    lDiSymbol = NULL_PTR;
static PSymbol    lDoSymbol = NULL_PTR;
static PSymbol    lFlipSymbol = NULL_PTR;
static PSymbol    lOffSymbol = NULL_PTR;
static PSymbol    lOnSymbol = NULL_PTR;
static PSymbol    lTriggerSymbol = NULL_PTR;

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ inputCallback ---*/
static void
inputCallback(STANDARD_HID_ARGS_INPUTEVENTHANDLER)
{
 #pragma unused(elementCookie,longValue,longValueSize)
    PhidgRefConPtr    data = reinterpret_cast<PhidgRefConPtr>(refCon);
    PrivatePtr            privateData = reinterpret_cast<PrivatePtr>(data->fPrivateStorage);
    
    if (privateData->fDigitalInputTriggerEnabled)
    {
LOG_POST_1("triggered")//!!
#if 0
        Atom    reportList[DIGITAL_REPORT_SIZE];

        // Extract the output bit vector and concatenate with the raw bits
        SETSYM(reportList, data->fDeviceType);
        SETSYM(reportList + 1, data->fThisDevice->fSerialNumber);
        SETSYM(reportList + 2, lDiSymbol);
        SETLONG(reportList + 3, value);
        for (UInt32 ii = 0; ii <= MAX_INDEX; ++ii)
            SETLONG(reportList + ii + 4, (value & (1 << ii)) ? 1 : 0);
        genericListOutput(data->fOutlet, DIGITAL_REPORT_SIZE, reportList);
#endif //0
    }        
} /* inputCallback */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ defineCallback ---*/
E_PhidgResult
defineCallback(STANDARD_PHID_ARGS_DEFINECALLBACK)
{
 #pragma unused(name,privateStorage,thisDevice)
    SharedPtr    sharedData = reinterpret_cast<SharedPtr>(sharedStorage);

    *aFun = inputCallback;
    *cookies = &sharedData->fInputCookie;
    *numCookies = 1;
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
        bool    okSoFar = true, doFlip = false, doOff = false, doTrigger = false;
        long    index;
        
        if (argv[0].a_type == A_SYM)
        {
            PSymbol    checker = argv[0].a_w.w_sym;
            
            if (checker == lTriggerSymbol)
                doTrigger = true;
            else if (checker == lFlipSymbol)
                doFlip = true;
            else if (checker == lOffSymbol)
                doOff = true;    
            else if (checker != lOnSymbol)
            {
                okSoFar = false;
                LOG_ERROR_4("%s: unknown operation ('%s') for '%s:do'", name, checker->s_name,
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
            if (doTrigger)
            {
                if (argv[1].a_type == A_SYM)
                {
                    PSymbol    checker = argv[1].a_w.w_sym;
                    
                    if (checker == lOffSymbol)
                        privateData->fDigitalInputTriggerEnabled = false;                        
                    else if (checker == lOnSymbol)
                        privateData->fDigitalInputTriggerEnabled = true;
                    else
                    {
                        okSoFar = false;
                        LOG_ERROR_4("%s: unknown trigger mode ('%s') for '%s:do'", name, checker->s_name,
                                                deviceType->s_name)
                    }
                }
                else
                {
                    okSoFar = false;
                    LOG_ERROR_3("%s: trigger mode is not a symbol for '%s:do'", name, deviceType->s_name)
                }
            }
            else if (argv[1].a_type == A_LONG)
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
        if (okSoFar && (! doTrigger))
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
    PrivatePtr    privateData = reinterpret_cast<PrivatePtr>(privateStorage);
    SharedPtr        sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
 
    if (argc == 1)
    {
        bool    okSoFar = true, doAnalog = false, doInput = false;
        
        if (argv[0].a_type == A_SYM)
        {
            if (argv[0].a_w.w_sym == lAiSymbol)
                doAnalog = true;
            else if (argv[0].a_w.w_sym == lDiSymbol)
                doInput = true;    
            else if (argv[0].a_w.w_sym != lDoSymbol)
            {
                okSoFar = false;
                LOG_ERROR_4("%s: unknown type ('%s') for '%s:get'", name, argv[0].a_w.w_sym->s_name,
                                        deviceType->s_name)
            }
        }
        else
        {
            okSoFar = false;
            LOG_ERROR_3("%s: type is not a symbol for '%s:get'", name, deviceType->s_name)
        }
        if (okSoFar)
        {
            if (doInput || doAnalog)
            {
 #if defined(COMPILE_FOR_OSX_4)
                // Get the raw data
              HIDElementDataPtr    anElement = NULL_PTR;
              
              // Find the matching element:
                 for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
                 {
                     if (anElement->fCookie == sharedData->fInputCookie)
                         break;
                         
                 }
              if (anElement)
              {
                  bool    canLeave = false;
                  int        counter = 0;
                  
                  for ( ; ! canLeave; )
                  {
                        UInt32        extendedLength;
                        Pvoid            extendedValue;
                        IOReturn    result2;
                        long            value = getHIDElementValue(name, *thisDevice, *anElement, extendedLength,
                                                                                                 extendedValue, result2);
                        
                        if (result2 == KERN_SUCCESS)
                        {
                            if (extendedLength == DATA_PACKET_SIZE)
                            {
                                Pchar    asChar = reinterpret_cast<Pchar>(extendedValue);
                                
                                if (doAnalog)
                                {
                                    // Store and remember the data:
                                    if (*asChar & 0x01)
                                    {
                                        privateData->fHaveOddData = true;
                                        for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
                                            privateData->fOddData[ii] = *(asChar + ii);
                                    }
                                    else
                                    {
                                        privateData->fHaveEvenData = true;
                                        for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
                                            privateData->fEvenData[ii] = *(asChar + ii);
                                    }
                                    if ((privateData->fHaveEvenData && privateData->fHaveOddData) ||
                                            (counter++ > 10))
                                    {
                                        // We've got all that we need or we've taken too long - assemble the results:
                                        Atom    newData[DATA_PACKET_SIZE + 3];
                                        
                                        SETSYM(newData, deviceType);
                                        SETSYM(newData + 1, thisDevice->fSerialNumber);
                                        SETSYM(newData + 2, lAiSymbol);
                                        SETFLOAT(newData + 3, ((privateData->fEvenData[2] & 0x00FF) + ((privateData->fEvenData[3] & 0x000F) * 256)) / 4095.0f);
                                        SETFLOAT(newData + 4, ((privateData->fEvenData[4] & 0x00FF) + ((privateData->fEvenData[3] & 0x00F0) * 16)) / 4095.0f);
                                        SETFLOAT(newData + 5, ((privateData->fEvenData[5] & 0x00FF) + ((privateData->fEvenData[6] & 0x000F) * 256)) / 4095.0f);
                                        SETFLOAT(newData + 6, ((privateData->fEvenData[7] & 0x00FF) + ((privateData->fEvenData[6] & 0x00F0) * 16)) / 4095.0f);
                                        SETFLOAT(newData + 7, ((privateData->fOddData[2] & 0x00FF) + ((privateData->fOddData[3] & 0x000F) * 256)) / 4095.0f);
                                        SETFLOAT(newData + 8, ((privateData->fOddData[4] & 0x00FF) + ((privateData->fOddData[3] & 0x00F0) * 16)) / 4095.0f);
                                        SETFLOAT(newData + 9, ((privateData->fOddData[5] & 0x00FF) + ((privateData->fOddData[6] & 0x000F) * 256)) / 4095.0f);
                                        SETFLOAT(newData + 10, ((privateData->fOddData[7] & 0x00FF) + ((privateData->fOddData[6] & 0x00F0) * 16)) / 4095.0f);
                                    genericListOutput(outlet, short(DATA_PACKET_SIZE + 3), newData);
                                        privateData->fHaveEvenData = privateData->fHaveOddData = false;
                                        canLeave = true;
                                    }
                                }
                                else
                                {
                                    // Extract the output bit vector and concatenate with the raw bits
                                    long    bitValues = ((*(asChar + 1) & 0x00FF) ^ 0x00FF);
                                    Atom    reportList[DIGITAL_REPORT_SIZE];

                                    SETSYM(reportList, deviceType);
                                    SETSYM(reportList + 1, thisDevice->fSerialNumber);
                                    SETSYM(reportList + 2, lDiSymbol);
                                    SETLONG(reportList + 3, bitValues);
                                    for (UInt32 ii = 0; ii <= MAX_INDEX; ++ii)
                                        SETLONG(reportList + ii + 4, (bitValues & (1 << ii)) ? 1 : 0);
                                    genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
                                    // Store and remember the data:
                                    if (*asChar & 0x01)
                                    {
                                        privateData->fHaveOddData = true;
                                        for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
                                            privateData->fOddData[ii] = *(asChar + ii);
                                    }
                                    else
                                    {
                                        privateData->fHaveEvenData = true;
                                        for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
                                            privateData->fEvenData[ii] = *(asChar + ii);
                                    }
                                    canLeave = true;
                                }
                            }
                            else
                            {
                                LOG_ERROR_4("%s: bad value length (%d) for '%s:get'", name, extendedLength,
                                                        deviceType->s_name)
                                break;
                                
                            }
                        }
                        else
                        {
                            *result = static_cast<OSErr>(result2);
                            break;
                            
                        }
                  }                            
              }
              else
                  LOG_ERROR_3("%s: element not found for '%s:get'", name, deviceType->s_name)
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
                if (doAnalog)
                {
                    Atom    newData[DATA_PACKET_SIZE + 3];
                    
                    SETSYM(newData, deviceType);
                    SETSYM(newData + 1, thisDevice->fSerialNumber);
                    SETSYM(newData + 2, lAiSymbol);
                    for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
                        SETFLOAT(newData + ii + 3, privateData->fAnalogInput[ii]);
                genericListOutput(outlet, short(DATA_PACKET_SIZE + 3), newData);
                }
                else
                {
                    // Extract the output bit vector and concatenate with the raw bits
                    Atom    reportList[DIGITAL_REPORT_SIZE];

                    SETSYM(reportList, deviceType);
                    SETSYM(reportList + 1, thisDevice->fSerialNumber);
                    SETSYM(reportList + 2, lDiSymbol);
                    SETLONG(reportList + 3, privateData->fDigitalInput);
                    for (UInt32 ii = 0; ii <= MAX_INDEX; ++ii)
                        SETLONG(reportList + ii + 4, (privateData->fDigitalInput & (1 << ii)) ? 1 : 0);
                    genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
                }
                *result = noErr;
 #endif /* COMPILE_FOR_OS9_4 */
            }
            else
            {
                // report the state of the digital output
                Atom    reportList[DIGITAL_REPORT_SIZE];

                SETSYM(reportList, deviceType);
                SETSYM(reportList + 1, thisDevice->fSerialNumber);
                SETSYM(reportList + 2, lDoSymbol);
                SETLONG(reportList + 3, privateData->fDigitalOutputs);
                for (UInt32 ii = 0; ii <= MAX_INDEX; ++ii)
                    SETLONG(reportList + ii + 4, (privateData->fDigitalOutputs & (1 << ii)) ? 1 : 0);
                genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
            }
        }
    }
    else if (argc)
        LOG_ERROR_3("%s: extra arguments for '%s:get'", name, deviceType->s_name)
    else
        LOG_ERROR_3("%s: missing type (ai, di or do) for '%s:get'", name, deviceType->s_name)
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
            else if (singleValue > 0x00FF)
                singleValue = 0x00FF;
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
#pragma unused(name)
    *productID = 0x045;
    *privateSize = sizeof(PrivateData);
    *sharedSize = sizeof(SharedData);
#if defined(COMPILE_FOR_OSX_4)
    *isAsynchronous = true;
#endif /* COMPILE_FOR_OSX_4 */
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
    lAiSymbol = gensym("ai");
    lDiSymbol = gensym("di");
    lDoSymbol = gensym("do");
    lFlipSymbol = gensym("flip");
    lOffSymbol = gensym("off");
    lOnSymbol = gensym("on");
    lTriggerSymbol = gensym("trigger");
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
    {
        privateData->fDigitalOutputs = 0;
        privateData->fHaveEvenData = privateData->fHaveOddData = false;
        for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
            privateData->fEvenData[ii] = privateData->fOddData[ii] = 0;
 #if defined(COMPILE_FOR_OS9_4)
        privateData->fDigitalInput = 0;
        for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
            privateData->fAnalogInput[ii] = 0;
 #endif /* COMPILE_FOR_OS9_4 */
    }
    if (sharedData)
    {
        if (! sharedData->fCookiesValid)
        {
            bool    foundIn = false, foundOut = false;
            
            // Locate the input and output elements
            for (HIDElementDataPtr walker = thisDevice->fFirstElement; walker;
                        walker = walker->fNext)
            {
                switch (walker->fType)
                {
                    case kIOHIDElementTypeInput_Misc:
                    case kIOHIDElementTypeInput_Button:
                    case kIOHIDElementTypeInput_Axis:
                    case kIOHIDElementTypeInput_ScanCodes:
                        if (walker->fSize == EXPECTED_INPUT_SIZE)
                        {
                            foundIn = true;
                            sharedData->fInputCookie = walker->fCookie;
                        }
                        break;

                    case kIOHIDElementTypeOutput:
                        if (walker->fSize == EXPECTED_OUTPUT_SIZE)
                        {
                            foundOut = true;
                            sharedData->fOutputCookie = walker->fCookie;
                        }
                        break;

                }        
            }
            sharedData->fCookiesValid = (foundIn && foundOut);
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
 #if defined(USE_DEFAULT)
  #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice,outlet,inHIDReport,inHIDReportLength)
 #else /* not USE_DEFAULT */
//  #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice,inHIDReport,inHIDReportLength)
    // Find the element descriptor that we want:
    PrivatePtr                privateData = reinterpret_cast<PrivatePtr>(privateStorage);
    SharedPtr                    sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
    HIDElementDataPtr    anElement = NULL_PTR;
    
    // Find the matching element:
    for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
    {
        if (anElement->fCookie == sharedData->fInputCookie)
            break;
            
    }
  if (anElement)
  {
        HIDReportType    kind;

        switch (anElement->fType)
        {
            case kIOHIDElementTypeOutput:
                kind = kHIDOutputReport;
                break;
                
            case kIOHIDElementTypeFeature:
                kind = kHIDFeatureReport;
                break;
                
            default:
                kind = kHIDInputReport;
                break;
                
        }
        Byte            valueBuffer[DATA_PACKET_SIZE];
        OSStatus    result = HIDGetUsageValueArray(kind, static_cast<UInt32>(anElement->fUsagePage),
                                                                                            0, static_cast<UInt32>(anElement->fUsage),
                                                                                            valueBuffer, sizeof(valueBuffer),
                                                                                            thisDevice->fPrepReport, inHIDReport, inHIDReportLength);  
      
      if (result == noErr)
      {
          Ptr        asChar = reinterpret_cast<Ptr>(valueBuffer);
          long    oldValue = privateData->fDigitalInput;
          
                // Update the digital input in all cases.
            privateData->fDigitalInput = ((*(asChar + 1) & 0x00FF) ^ 0x00FF);
            if (*asChar & 0x01)
            {
                privateData->fHaveOddData = true;
                for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
                    privateData->fOddData[ii] = *(asChar + ii);
            }
            else
            {
                privateData->fHaveEvenData = true;
                for (UInt32 ii = 0; ii < DATA_PACKET_SIZE; ++ii)
                    privateData->fEvenData[ii] = *(asChar + ii);
            }
            if (privateData->fHaveOddData && privateData->fHaveEvenData)
            {
                privateData->fAnalogInput[0] = ((privateData->fEvenData[2] & 0x00FF) + ((privateData->fEvenData[3] & 0x000F) * 256)) / 4095.0f;
                privateData->fAnalogInput[1] = ((privateData->fEvenData[4] & 0x00FF) + ((privateData->fEvenData[3] & 0x00F0) * 16)) / 4095.0f;
                privateData->fAnalogInput[2] = ((privateData->fEvenData[5] & 0x00FF) + ((privateData->fEvenData[6] & 0x000F) * 256)) / 4095.0f;
                privateData->fAnalogInput[3] = ((privateData->fEvenData[7] & 0x00FF) + ((privateData->fEvenData[6] & 0x00F0) * 16)) / 4095.0f;
                privateData->fAnalogInput[4] = ((privateData->fOddData[2] & 0x00FF) + ((privateData->fOddData[3] & 0x000F) * 256)) / 4095.0f;
                privateData->fAnalogInput[5] = ((privateData->fOddData[4] & 0x00FF) + ((privateData->fOddData[3] & 0x00F0) * 16)) / 4095.0f;
                privateData->fAnalogInput[6] = ((privateData->fOddData[5] & 0x00FF) + ((privateData->fOddData[6] & 0x000F) * 256)) / 4095.0f;
                privateData->fAnalogInput[7] = ((privateData->fOddData[7] & 0x00FF) + ((privateData->fOddData[6] & 0x00F0) * 16)) / 4095.0f;
                privateData->fHaveOddData = privateData->fHaveEvenData = false;
            }
            if (privateData->fDigitalInputTriggerEnabled && (oldValue != privateData->fDigitalInput))
            {
                Atom    reportList[DIGITAL_REPORT_SIZE];

                // Extract the output bit vector and concatenate with the raw bits
                SETSYM(reportList, deviceType);
                SETSYM(reportList + 1, thisDevice->fSerialNumber);
                SETSYM(reportList + 2, lDiSymbol);
                SETLONG(reportList + 3, privateData->fDigitalInput);
                for (UInt32 ii = 0; ii <= MAX_INDEX; ++ii)
                    SETLONG(reportList + ii + 4, (privateData->fDigitalInput & (1 << ii)) ? 1 : 0);
                genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
            }
      }
      else
          LOG_ERROR_4("%s: error getting usage value (%d/0x%x)", name, result, result) 
    }
 #endif /* not USE_DEFAULT */
} /* reportHandler */
#endif /* COMPILE_FOR_OS9_4 */
