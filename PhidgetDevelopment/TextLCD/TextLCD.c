/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       TextLCD.c                                                               */
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
/*  Created:    2003/12/23                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
//#define USE_DEFAULT /* */

#include "TextLCD.h"
#include "genericListOutput.h"
#include "TextLCDSupport.h"
#include "HD44780.h"

static const int    DIGITAL_REPORT_SIZE = 12;
static const int    EXPECTED_OUTPUT_SIZE = 64;

static PSymbol    lBacklightSymbol = NULL_PTR;
static PSymbol    lBlinkSymbol = NULL_PTR;
static PSymbol    lClearSymbol = NULL_PTR;
static PSymbol    lCursorSymbol = NULL_PTR;
static PSymbol    lDisplaySymbol = NULL_PTR;
static PSymbol    lEntryModeSymbol = NULL_PTR;
static PSymbol    lGoSymbol = NULL_PTR;
static PSymbol    lLeftSymbol = NULL_PTR;
static PSymbol    lOffSymbol = NULL_PTR;
static PSymbol    lOnSymbol = NULL_PTR;
static PSymbol    lReverseSymbol = NULL_PTR;
static PSymbol    lRightSymbol = NULL_PTR;
static PSymbol    lShiftSymbol = NULL_PTR;
static PSymbol    lWriteSymbol = NULL_PTR;

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
 #pragma unused(outlet,privateStorage)
 #if defined(COMPILE_FOR_OSX_4)
    IOReturn    result2 = KERN_SUCCESS;
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    OSStatus    result2 = noErr;
 #endif /* COMPILE_FOR_OS9_4 */
    SharedPtr    sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
    
    if (argc >= 1)
    {
        bool                            okSoFar = true;
      HIDElementDataPtr    anElement = NULL_PTR;
      
      // Find the matching element:
         for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
         {
             if (anElement->fCookie == sharedData->fOutputCookie)
                 break;
                 
         }
        if (! anElement)
        {
            LOG_ERROR_3("%s: element not found for '%s:do'", name, deviceType->s_name)
            okSoFar = false;
        }
        if (okSoFar)
        {
            // Check text operations for validity and perform them:
            if (argv[0].a_type == A_SYM)
            {
                bool        overCount = false, notNumber = false, notText = false;
                bool        unknownSymbol = false, badNumber = false;
                PSymbol    check = argv[0].a_w.w_sym;
                
                if (check == lOffSymbol)
                {
                    if (argc == 1)
                        result2 = doTextLCDOff(name, *thisDevice, *anElement);
                    else
                        overCount = true;
                }
                else if (check == lOnSymbol)
                {
                        PSymbol    param;
                        bool        sawCursor = false, sawBlink = false;
                        
                        if (argc >= 2)
                        {
                            if (argv[1].a_type == A_SYM)
                            {
                                param = argv[1].a_w.w_sym;
                                if (param == lCursorSymbol)
                                    sawCursor = true;
                                else if (param == lBlinkSymbol)
                                    sawBlink = true;
                                else
                                    unknownSymbol = true;
                            }
                            else
                                notText = true;
                        }
                        if (argc == 3)
                        {
                            if (argv[2].a_type == A_SYM)
                            {
                                param = argv[2].a_w.w_sym;
                                if (param == lCursorSymbol)
                                    sawCursor = true;
                                else if (param == lBlinkSymbol)
                                    sawBlink = true;
                                else
                                    unknownSymbol = true;
                            }
                            else
                                notText = true;
                        }
                        if ((! notText) && (! unknownSymbol))
                            result2 = doTextLCDOn(name, *thisDevice, *anElement, sawCursor, sawBlink);
                    }
                else if (check == lBacklightSymbol)
                {
                    if (argc == 2)
                    {
                        if (argv[1].a_type == A_SYM)
                        {
                            PSymbol    param = argv[1].a_w.w_sym;
                            
                            if (param == lOffSymbol)
                                result2 = doTextLCDBacklightOff(name, *thisDevice, *anElement);                                
                            else if (param == lOnSymbol)
                                result2 = doTextLCDBacklightOn(name, *thisDevice, *anElement);
                            else
                                unknownSymbol = true;
                        }
                        else
                            notText = true;    
                    }
                    else
                        overCount = true;
                }
                else if (check == lEntryModeSymbol)
                {
                    if (argc > 3)
                        overCount = true;
                    else
                    {
                        PSymbol    param;
                        bool        sawReverse = false, sawShift = false;
                        
                        if (argc >= 2)
                        {
                            if (argv[1].a_type == A_SYM)
                            {
                                param = argv[1].a_w.w_sym;
                                if (param == lReverseSymbol)
                                    sawReverse = true;
                                else if (param == lShiftSymbol)
                                    sawShift = true;
                                else
                                    unknownSymbol = true;
                            }
                            else
                                notText = true;
                        }
                        if (argc == 3)
                        {
                            if (argv[2].a_type == A_SYM)
                            {
                                param = argv[2].a_w.w_sym;
                                if (param == lReverseSymbol)
                                    sawReverse = true;
                                else if (param == lShiftSymbol)
                                    sawShift = true;
                                else
                                    unknownSymbol = true;
                            }
                            else
                                notText = true;
                        }
                        if ((! notText) && (! unknownSymbol))
                            result2 = doTextLCDEntryMode(name, *thisDevice, *anElement,
                                                                                        sawReverse, sawShift);
                    }
                }
                else if (check == lDisplaySymbol)
                {
                    if (argc == 2)
                    {
                        if (argv[1].a_type == A_SYM)
                        {
                            PSymbol    param = argv[1].a_w.w_sym;
                            
                            if (param == lLeftSymbol)
                                result2 = doTextLCDShiftDisplay(name, *thisDevice, *anElement, false);
                            else if (param == lRightSymbol)
                                result2 = doTextLCDShiftDisplay(name, *thisDevice, *anElement, true);
                            else
                                unknownSymbol = true;                            
                        }
                        else
                            notText = true;    
                    }
                    else
                        overCount = true;
                }
                else if (check == lCursorSymbol)
                {
                    if (argc == 2)
                    {
                        if (argv[1].a_type == A_SYM)
                        {
                            PSymbol    param = argv[1].a_w.w_sym;
                            
                            if (param == lLeftSymbol)
                                result2 = doTextLCDShiftCursor(name, *thisDevice, *anElement, false);
                            else if (param == lRightSymbol)
                                result2 = doTextLCDShiftCursor(name, *thisDevice, *anElement, true);
                            else
                                unknownSymbol = true;                            
                        }
                        else
                            notText = true;    
                    }
                    else
                        overCount = true;
                }
                else if (check == lClearSymbol)
                {
                    if (argc == 2)
                    {
                        if (argv[1].a_type == A_LONG)
                        {
                            long    row = argv[1].a_w.w_long;
                            
                            switch (row)
                            {
                                case 1:
                                    result2 = doTextLCDClearLine1(name, *thisDevice, *anElement);                                    
                                    break;
                                
                                case 2:
                                    result2 = doTextLCDClearLine2(name, *thisDevice, *anElement);                                    
                                    break;
                                
                                default:
                                    badNumber = true;
                                    break;
                                    
                            }
                        }
                        else
                            notNumber = true;    
                    }
                    else if (argc == 1)
                        result2 = doTextLCDClear(name, *thisDevice, *anElement);
                    else
                        overCount = true;
                }
                else if (check == lGoSymbol)
                {
                    long    row = 1;
                    long    column = 1;
                        
                    if (argc == 3)
                    {
                        if (argv[1].a_type == A_LONG)
                            row = argv[1].a_w.w_long;
                        else
                            notNumber = true;
                        if (argv[2].a_type == A_LONG)
                            column = argv[2].a_w.w_long;
                        else
                            notNumber = true;
                    }
                    else if (argc == 2)
                    {
                        if (argv[1].a_type == A_LONG)
                            row = argv[1].a_w.w_long;
                        else
                            notNumber = true;
                    }
                    else if (argc != 1)
                        overCount = true;
                    if ((! overCount) && (! notNumber))
                        result2 = doTextLCDGo(name, *thisDevice, *anElement, row, column);
                }
                else if (check == lWriteSymbol)
                {
                    if (argc < 2)
                        LOG_ERROR_3("%s: missing text parameters for '%s:do'", name, deviceType->s_name)
                    else
                    {
                        // Convert the atoms into a text buffer... Note that the most that we can
                        // write is CHARS_PER_LINE in any case.
                        char        buffer[CHARS_PER_LINE + 1];
                        char        numBuffer[10];
                        size_t    lastChar = 0;
                        Ptr            toAdd = NULL_PTR;
                        
                        memset(buffer, 0, sizeof(buffer));
                        for (short ii = 1; ii < argc; ++ii)
                        {
                            switch (argv[ii].a_type)
                            {
                    case A_LONG:
                      sprintf(numBuffer, "%ld", argv[ii].a_w.w_long);
                      toAdd = numBuffer;
                      break;

                    case A_SYM:
                      toAdd = argv[ii].a_w.w_sym->s_name;
                      break;

                    case A_FLOAT:
                      sprintf(numBuffer, "%g", double(argv[ii].a_w.w_float));
                      toAdd = numBuffer;
                      break;

                    case A_SEMI:
                      toAdd = ";";
                      break;

                    case A_COMMA:
                      toAdd = ",";
                      break;

                    case A_DOLLAR:
                      toAdd = "$";
                      break;

                  }
                  if (toAdd)
                  {
                      size_t    len = strlen(toAdd);
                      
                      if ((len + lastChar) > CHARS_PER_LINE)
                          len = CHARS_PER_LINE - lastChar;
                      memcpy(buffer + lastChar, toAdd, len);
                      lastChar += len;
                      if (lastChar >= CHARS_PER_LINE)
                          break;
                      
                      if (ii < (argc - 1))
                      {
                          *(buffer + lastChar) = ' ';
                          ++lastChar;
                          if (lastChar >= CHARS_PER_LINE)
                              break;
                              
                      }
                  }
                        }
                        if (lastChar)
                            result2 = doTextLCDWrite(name, *thisDevice, *anElement, buffer);    
                    }
                }
                else
                    LOG_ERROR_4("%s: unknown text operation ('%s') for '%s:do'", name,
                                            check->s_name, deviceType->s_name)
                if (overCount)
                    LOG_ERROR_3("%s: extra arguments for '%s:do'", name, deviceType->s_name)
                else if (notText)
                    LOG_ERROR_3("%s: text parameter is not a symbol for '%s:do'", name, deviceType->s_name)                        
                else if (notNumber)
                    LOG_ERROR_3("%s: text parameter is not a number for '%s:do'", name, deviceType->s_name)                        
                else if (unknownSymbol)
                    LOG_ERROR_3("%s: text parameter is not recognized for '%s:do'", name, deviceType->s_name)                        
                else if (badNumber)
                    LOG_ERROR_3("%s: text parameter is out of range for '%s:do'", name, deviceType->s_name)                        
            }
            else
                LOG_ERROR_3("%s: text operation is not a symbol for '%s:do'", name, deviceType->s_name)
        }
    }
    else
        LOG_ERROR_3("%s: missing operation and value for '%s:do'", name, deviceType->s_name)
    *result = static_cast<OSErr>(result2);
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
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
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
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doPut */

/*------------------------------------ identify ---*/
OSErr
identify(STANDARD_PHID_ARGS_IDENTIFY)
{
#if defined(COMPILE_FOR_OSX_4)
 #pragma unused(name,isAsynchronous,privateSize)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
 #pragma unused(name,privateSize)
#endif /* COMPILE_FOR_OS9_4 */
    *productID = 0x52;
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
    lBacklightSymbol = gensym("backlight");
    lBlinkSymbol = gensym("blink");
    lClearSymbol = gensym("clear");
    lCursorSymbol = gensym("cursor");
    lDisplaySymbol = gensym("display");
    lEntryModeSymbol = gensym("entrymode");
    lGoSymbol = gensym("go");
    lLeftSymbol = gensym("left");
    lOffSymbol = gensym("off");
    lOnSymbol = gensym("on");
    lReverseSymbol = gensym("reverse");
    lRightSymbol = gensym("right");
    lShiftSymbol = gensym("shift");
    lWriteSymbol = gensym("write");
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
 #pragma unused(name,deviceType,privateStorage)
    SharedPtr    sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
    
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
