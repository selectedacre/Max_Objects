/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       caseShift_Anything.cpp                                                  */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
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
/*  Created:    2000/10/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "caseShift.h"

/*------------------------------------ cmd_Anything ---*/
void cmd_Anything(CaseShiftData * xx,
                  t_symbol *      message,
                  short           argc,
                  t_atom *        argv)
{
    if (xx)
    {
        t_atom * newArg = GETBYTES(argc + 1, t_atom);

        clearPrevious(xx);
        xx->fPreviousKind = A_GIMME;
        if (newArg)
        {
            short  len;
            char * oldWord;
            char * newWord;

            SETSYM(newArg, message);
            memcpy(newArg + 1, argv, argc * sizeof(t_atom));
            for (short ii = 0; ii <= argc; ++ii)
            {
                if (newArg[ii].a_type == A_SYM)
                {
                    oldWord = newArg[ii].a_w.w_sym->s_name;
                    len = static_cast<short>(strlen(oldWord));
                    newWord = GETBYTES(len + 1, char);
                    if (newWord)
                    {
                        shiftAString(xx, newWord, oldWord);
                        newArg[ii].a_w.w_sym = gensym(newWord);
                        FREEBYTES(newWord, len + 1);
                    }
                }
            }
            xx->fPreviousList = newArg;
            xx->fPreviousLength = static_cast<short>(argc + 1);
            genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
        }
    }
} // cmd_Anything
