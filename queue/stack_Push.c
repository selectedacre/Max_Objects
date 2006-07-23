/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       stack_Push.c                                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_Push().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/06/22                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "stack.h"

/*------------------------------------ cmd_Push ---*/
Pvoid cmd_Push
  (StackPtr xx,
   PSymbol  message,
   short    argc,
   PAtom    argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    if (xx->fVerbose)
      LOG_POST_1(OUTPUT_PREFIX "value being pushed on stack")
    if (argc)
    {
      StackDescriptorPtr descriptor = xx->fStack;

      if (descriptor)
      {
        StackEntryPtr top = descriptor->fTopOfStack, newTop = GETBYTES(1, StackEntry);

        if (newTop)
        {
          PAtom temp = GETBYTES(argc, Atom);

          newTop->fNext = top;
          newTop->fOutputCount = argc;
          if (temp)
          {                       
            newTop->fOutput = temp;
            for (short ii = 0; ii < argc; ++ii, ++temp)
              *temp = argv[ii];
          }
          else
          {
            newTop->fOutput = NULL_PTR;
            LOG_ERROR_1(OUTPUT_PREFIX "problem allocating memory for stack element")
          }
          ++descriptor->fDepth;
          descriptor->fTopOfStack = newTop;
        }
        else
          LOG_ERROR_1(OUTPUT_PREFIX "problem allocating space for stack element")
      }
    }
    else
      LOG_ERROR_1(OUTPUT_PREFIX "missing data for push")
  }
  ExitMaxMessageHandler()
} /* cmd_Push */
