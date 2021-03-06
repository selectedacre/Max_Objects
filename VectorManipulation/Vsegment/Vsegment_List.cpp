/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vsegment_List.cpp                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
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
/*  Created:    2000/07/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vsegment.h"

/*------------------------------------ cmd_List ---*/
LIST_HEADER(VsegmentData)
{
#pragma unused(message)
    if (xx)
    {
        long elementCount = xx->fHowMany;

        clearPrevious(xx);
        if (elementCount)
        {
            long start = xx->fStart;
            long end;

            if (0 > start)
            {
                start = argc + start;
            }
            if (0 < elementCount)
            {
                end = start + elementCount - 1;
            }
            else
            {
                long temp = start + elementCount + 1;

                end = start;
                start = temp;
            }
            if (0 > start)
            {
                start = 0;
            }
            if (end >= argc)
            {
                end = argc - 1;
            }
            if ((0 <= end) && (start < argc))
            {
                t_atom * newList = GET_BYTES(end + 1 - start, t_atom);

                if (newList)
                {
                    memcpy(newList, argv + start, (end + 1 - start) * sizeof(t_atom));
                    xx->fPreviousList = newList;
                    xx->fPreviousLength = end + 1 - start;
                    genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
                }
            }
        }
    }
} // cmd_List
