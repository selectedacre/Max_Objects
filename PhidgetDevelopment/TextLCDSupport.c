/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       TextLCDSupport.c                                                        */
/*                                                                                      */
/*  Contains:   The support routines for LCD access by phidgets.                        */
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
// File: TextLCDSupport.c

#include "TextLCDSupport.h"
#include "HD44780.h"

/*------------------------------------ doTextLCDBacklightOff ---*/
ReturnType
doTextLCDBacklightOff(Pchar                  name,
                      HIDDeviceDataStruct &  thisDevice,    
                      HIDElementDataStruct & outputElement)
{
    ReturnType    result;
    Atom                buffer[DATA_BUFFER_SIZE];
    
    SETLONG(buffer, HD44780_Backlight_Off_0);
    for (int ii = 1; ii < (DATA_BUFFER_SIZE - 1); ++ii)
        SETLONG(buffer + ii, 0);
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, HD44780_Backlight_Off_n);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);    
    return result;
} /* doTextLCDBacklightOff */
    
/*------------------------------------ doTextLCDBacklightOn ---*/
ReturnType
doTextLCDBacklightOn(Pchar                  name,
                     HIDDeviceDataStruct &  thisDevice,    
                     HIDElementDataStruct & outputElement)
{
    ReturnType    result;
    Atom                buffer[DATA_BUFFER_SIZE];
    
    SETLONG(buffer, HD44780_Backlight_On_0);
    for (int ii = 1; ii < (DATA_BUFFER_SIZE - 1); ++ii)
        SETLONG(buffer + ii, 0);
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, HD44780_Backlight_On_n);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);    
    return result;
} /* doTextLCDBacklightOn */
    
/*------------------------------------ doTextLCDClear ---*/
ReturnType
doTextLCDClear(Pchar                  name,
               HIDDeviceDataStruct &  thisDevice,    
               HIDElementDataStruct & outputElement)
{
    ReturnType    result = doTextLCDClearLine1(name, thisDevice, outputElement);
    
#if defined(COMPILE_FOR_OSX_4)
    if (result == KERN_SUCCESS)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    if (result == noErr)
#endif /* COMPILE_FOR_OS9_4 */
        result = doTextLCDClearLine2(name, thisDevice, outputElement);
    return result;
} /* doTextLCDClear */
    
/*------------------------------------ doTextLCDClearLine1 ---*/
ReturnType
doTextLCDClearLine1(Pchar                  name,
                    HIDDeviceDataStruct &  thisDevice,    
                    HIDElementDataStruct & outputElement)
{
    return doTextLCDWriteLine1(name, thisDevice, outputElement, NULL_PTR);
} /* doTextLCDClearLine1 */
    
/*------------------------------------ doTextLCDClearLine2 ---*/
ReturnType
doTextLCDClearLine2(Pchar                  name,
                    HIDDeviceDataStruct &  thisDevice,    
                    HIDElementDataStruct & outputElement)
{
    return doTextLCDWriteLine2(name, thisDevice, outputElement, NULL_PTR);
} /* doTextLCDClearLine2 */
    
/*------------------------------------ doTextLCDEntryMode ---*/
ReturnType
doTextLCDEntryMode(Pchar                  name,
                   HIDDeviceDataStruct &  thisDevice,    
                   HIDElementDataStruct & outputElement,
                   const bool             reversed,
                   const bool             shifted)
{
    ReturnType    result;
    Atom                buffer[DATA_BUFFER_SIZE];
    
    SETLONG(buffer, HD44780_EntryModeSet +
                    (reversed ? HD44780_EntryMode_Decrement : HD44780_EntryMode_Increment) +
                    (shifted ? HD44780_EntryMode_WithShift : HD44780_EntryMode_NoShift));
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, 1);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
    return result;
} /* doTextLCDEntryMode */

/*------------------------------------ doTextLCDGo ---*/
ReturnType
doTextLCDGo(Pchar                  name,
            HIDDeviceDataStruct &  thisDevice,    
            HIDElementDataStruct & outputElement,
            const int              row,
            const int              column)
{
#if defined(COMPILE_FOR_OSX_4)
    ReturnType    result = KERN_SUCCESS;
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    ReturnType    result = noErr;
#endif /* COMPILE_FOR_OS9_4 */
    Atom                buffer[DATA_BUFFER_SIZE];
    int                    startColumn;
    
    if (column >= CHARS_PER_LINE)
        startColumn = CHARS_PER_LINE;
    else if (column < 1)
        startColumn = 1;
    else
        startColumn = column;
    // Set up the initial contents of the buffer.
    SETLONG(buffer, HD44780_DDRamSet + ((1 - (row % 2)) * HD44780_DDRam_Line2) + startColumn - 1);
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, 1);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
    return result;
} /* doTextLCDGo */

/*------------------------------------ doTextLCDOff ---*/
ReturnType
doTextLCDOff(Pchar                  name,
             HIDDeviceDataStruct &  thisDevice,    
             HIDElementDataStruct & outputElement)
{
    ReturnType    result;
    Atom                buffer[DATA_BUFFER_SIZE];
    
    SETLONG(buffer, HD44780_DisplayOnOff + HD44780_Display_Off);
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, 1);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
    return result;
} /* doTextLCDOff */
    
/*------------------------------------ doTextLCDOn ---*/
ReturnType
doTextLCDOn(Pchar                  name,
            HIDDeviceDataStruct &  thisDevice,    
            HIDElementDataStruct & outputElement,
            const bool             cursorState,
            const bool             blinkState)
{
    ReturnType    result;
    Atom                buffer[DATA_BUFFER_SIZE];
    
    SETLONG(buffer, HD44780_FunctionSet + HD44780_Function_8Bit + HD44780_Function_2x5x7);
    SETLONG(buffer + 1, TextLCD_Escape);
    SETLONG(buffer + 2, HD44780_ClearDisplay);
    SETLONG(buffer + 3, HD44780_DisplayOnOff + HD44780_Display_On +
                    (cursorState ? HD44780_Display_WithCursor : HD44780_Display_NoCursor) +
                    (blinkState ? HD44780_Display_WithBlink : HD44780_Display_NoBlink));
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, 4);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
    return result;
} /* doTextLCDOn */

/*------------------------------------ doTextLCDShiftCursor ---*/
ReturnType
doTextLCDShiftCursor(Pchar                  name,
                     HIDDeviceDataStruct &  thisDevice,    
                     HIDElementDataStruct & outputElement,
                     const bool             toRight)
{
    ReturnType    result;
    Atom                buffer[DATA_BUFFER_SIZE];
    
    SETLONG(buffer, HD44780_CursorDisplayShift + HD44780_CursorDisplay_ChangeCursor +
                    (toRight ? HD44780_CursorDisplay_GoRight : HD44780_CursorDisplay_GoLeft));
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, 1);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
    return result;
} /* doTextLCDShiftCursor */

/*------------------------------------ doTextLCDShiftDisplay ---*/
ReturnType
doTextLCDShiftDisplay(Pchar                  name,
                      HIDDeviceDataStruct &  thisDevice,    
                      HIDElementDataStruct & outputElement,
                      const bool             toRight)
{
    ReturnType    result;
    Atom                buffer[DATA_BUFFER_SIZE];
    
    SETLONG(buffer, HD44780_CursorDisplayShift + HD44780_CursorDisplay_ChangeDisplay +
                    (toRight ? HD44780_CursorDisplay_GoRight : HD44780_CursorDisplay_GoLeft));
    SETLONG(buffer + DATA_BUFFER_SIZE - 1, 1);
    setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
    return result;
} /* doTextLCDShiftDisplay */    
    
/*------------------------------------ doTextLCDWrite ---*/
ReturnType
doTextLCDWrite(Pchar                  name,
               HIDDeviceDataStruct &  thisDevice,    
               HIDElementDataStruct & outputElement,
               Qchar                  input)
{
#if defined(COMPILE_FOR_OSX_4)
    ReturnType    result = KERN_SUCCESS;
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    ReturnType    result = noErr;
#endif /* COMPILE_FOR_OS9_4 */
    char                frame[CHARS_PER_LINE + 3];
    Atom                buffer[DATA_BUFFER_SIZE];
    
    // Set up the initial contents of the buffer.
    *frame = TextLCD_DataMode;
    size_t    inLen = (input ? strlen(input) : 0);
    
    if (inLen > CHARS_PER_LINE)
        inLen = CHARS_PER_LINE;
    if (inLen)
        memcpy(frame + 1, input, inLen);
    else if (! input)
    {
        memset(frame + 1, ' ', CHARS_PER_LINE);
        inLen = CHARS_PER_LINE;
    }
    *(frame + inLen + 1) = TextLCD_ControlMode;
    size_t    out = 0;
    
    for (size_t in = 0; in <= (inLen + 1); ++in)
    {
        SETLONG(buffer + out, long(*(frame + in)));
        if (++out >= (DATA_BUFFER_SIZE - 1))
        {
            SETLONG(buffer + DATA_BUFFER_SIZE - 1, long(out));
            setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
#if defined(COMPILE_FOR_OSX_4)
            if (result != KERN_SUCCESS)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
            if (result != noErr)
#endif /* COMPILE_FOR_OS9_4 */
                break;
                
            out = 0;
        }    
    }
#if defined(COMPILE_FOR_OSX_4)
    if (out && (result == KERN_SUCCESS))
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    if (out && (result == noErr))
#endif /* COMPILE_FOR_OS9_4 */
    {
        SETLONG(buffer + DATA_BUFFER_SIZE - 1, long(out));
        setHIDElementValue(name, thisDevice, outputElement, DATA_BUFFER_SIZE, buffer, 0, result);
    }
    return result;
} /* doTextLCDWrite */
    
/*------------------------------------ doTextLCDWriteLine1 ---*/
ReturnType
doTextLCDWriteLine1(Pchar                  name,
                    HIDDeviceDataStruct &  thisDevice,    
                    HIDElementDataStruct & outputElement,
                    Qchar                  input)
{
    ReturnType    result = doTextLCDGo(name, thisDevice, outputElement, 1, 1);
    
#if defined(COMPILE_FOR_OSX_4)
    if (result == KERN_SUCCESS)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    if (result == noErr)
#endif /* COMPILE_FOR_OS9_4 */
        result = doTextLCDWrite(name, thisDevice, outputElement, input);
    return result;
} /* doTextLCDWriteLine1 */
    
/*------------------------------------ doTextLCDWriteLine2 ---*/
ReturnType
doTextLCDWriteLine2(Pchar                  name,
                    HIDDeviceDataStruct &  thisDevice,    
                    HIDElementDataStruct & outputElement,
                    Qchar                  input)
{
    ReturnType    result = doTextLCDGo(name, thisDevice, outputElement, 2, 1);
    
#if defined(COMPILE_FOR_OSX_4)
    if (result == KERN_SUCCESS)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    if (result == noErr)
#endif /* COMPILE_FOR_OS9_4 */
        result = doTextLCDWrite(name, thisDevice, outputElement, input);
    return result;
} /* doTextLCDWriteLine2 */
