/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vrtrim_List.cpp                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
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
/*  Created:    2003/06/16                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vrtrim.h"

/*------------------------------------ cmd_List ---*/
void cmd_List(VObjectData * xx,
              t_symbol *    message,
              short         argc,
              t_atom *      argv)
{
#pragma unused(message)
    if (xx)
    {
        clearPrevious(xx);
        if (checkInput(xx, OUTPUT_PREFIX, argc, argv))
        {
            short right = static_cast<short>(argc - 1);

            for ( ; right >= 0; right--)
            {
                if (! isSeparator(xx, argv[right].a_w.w_long))
                {
                    break;
                }
            }
            if (right < 0)
            {
                outlet_bang(xx->fBangOut);
            }
            else
            {
                t_atom * result = GETBYTES(right + 1, t_atom);

                if (result)
                {
                    for (short index = 0; index <= right; ++index)
                    {
                        SETLONG(result + index, argv[index].a_w.w_long);
                    }
                    xx->fPreviousList = result;
                    xx->fPreviousLength = static_cast<short>(right + 1);
                    genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
                }
            }
        }
    }
} // cmd_List
