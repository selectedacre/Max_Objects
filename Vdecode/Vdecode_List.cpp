/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vdecode_List.cpp                                                        */
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
/*  Created:    2003/06/15                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vdecode.h"

/*------------------------------------ cmd_List ---*/
void cmd_List(VdecodeData * xx,
              t_symbol *    message,
              short         argc,
              t_atom *      argv)
{
#pragma unused(message)
    if (xx && checkInput(xx, OUTPUT_PREFIX, argc, argv))
    {
        bool  wasNegative = xx->fInfo.fFirstBaseIsNegative;
        short numBases = xx->fInfo.fNumBases;

        if ((! wasNegative) && (argc > numBases))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "input list is too long")
        }
        else
        {
            long base = 1;
            long sum = 0;

            for (int ii = argc - 1, jj = 0; ii >= 0; --ii)
            {
                sum += (argv[ii].a_w.w_long * base);
                base *= xx->fInfo.fBases[jj];
                if (jj < (numBases - 1))
                {
                    ++jj;
                }
            }
            xx->fPreviousLong = sum;
            outlet_int(xx->fResultOut, xx->fPreviousLong);
        }
    }
} // cmd_List
