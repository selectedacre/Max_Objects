/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vcollect_Bang.cpp                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_Bang().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/08/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vcollect.h"

/*------------------------------------ cmd_Bang ---*/
BANG_HEADER(VcollectData)
{
    if (xx)
    {
        switch (xx->fInletNumber)
        {
            case 0:
            case 1:
                /* Left  and right inlet */
                if (0 < xx->fTotalAtoms)
                {
                    /* Build up a new list */
                    t_atom * pile = GET_BYTES(xx->fTotalAtoms, t_atom);

                    if (pile)
                    {
                        t_atom * outWalker = pile;
                        short    toGo = xx->fTotalAtoms;
                        short    thisMove = CHUNK_SIZE;

                        for (CollectChunk * inWalker = xx->fFirstChunk; inWalker && (0 < toGo);
                             inWalker = inWalker->fNext)
                        {
                            if (CHUNK_SIZE > toGo)
                            {
                                thisMove = toGo;
                            }
                            memcpy(outWalker, &inWalker->fAtoms, thisMove * sizeof(t_atom));
                            outWalker += CHUNK_SIZE;
                            toGo -= CHUNK_SIZE;
                        }
                        genericListOutput(xx->fResultOut, xx->fTotalAtoms, pile);
                        FREE_BYTES(pile);
                    }
                }
                break;

            default:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected port (%ld) seen", xx->fInletNumber)
                break;
                
        }
    }
} // cmd_Bang
