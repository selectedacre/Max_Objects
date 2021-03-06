/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vtake_Anything.cpp                                                      */
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
/*  Created:    2000/07/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vtake.h"

/*------------------------------------ cmd_Anything ---*/
ANYTHING_HEADER(VObjectData)
{
    if (xx)
    {
        t_atom * newArg = NULL;
        long     elementCount = 0;

        clearPrevious(xx);
        /* Determine the size of the output: */
        if (0 < xx->fHowMany)
        {
            elementCount = xx->fHowMany;
        }
        else if (0 > xx->fHowMany)
        {
            elementCount = -xx->fHowMany;
        }
        if ((argc + 1) < elementCount)
        {
            elementCount = argc + 1;
        }
        /* Collect the pieces that we need: */
        newArg = GET_BYTES(elementCount, t_atom);
        if (newArg)
        {
            if (0 < xx->fHowMany)
            {
                atom_setsym(newArg, message);
                memcpy(newArg + 1, argv, (elementCount - 1) * sizeof(t_atom));
            }
            else if ((argc + 1) > (-xx->fHowMany))
            {
                memcpy(newArg, argv + argc - elementCount, elementCount * sizeof(t_atom));
            }
            else
            {
                atom_setsym(newArg, message);
                memcpy(newArg + 1, argv, (elementCount - 1) * sizeof(t_atom));
            }
        }
        else
        {
            elementCount = 0;
        }
        xx->fPreviousList = newArg;
        xx->fPreviousLength = elementCount;
        if (xx->fPreviousList)
        {
            genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
        }
    }
} // cmd_Anything
