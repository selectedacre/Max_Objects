/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vreduce_Int.cpp                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_Int().                                                  */
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
/*  Created:    2000/11/29                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vreduce.h"

/*------------------------------------ cmd_Int ---*/
INT_HEADER(VreduceData)
{
    if (xx)
    {
        bool okSoFar = true;

        if (IR_NONZERO == (xx->fCheck & IR_NONZERO))
        {
            if (! msg)
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "zero in input list")
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            xx->fResultIsFloat = false;
            switch (xx->fOperation)
            {
                case OP_ADD:
                case OP_BITAND:
                case OP_BITOR:
                case OP_DIVIDE:
                case OP_EXOR:
                case OP_MAX:
                case OP_MIN:
                case OP_MODULUS:
                case OP_MULTIPLY:
                case OP_SUBTRACT:
                    xx->fPreviousLong = msg;
                    break;

                case OP_AND:
                case OP_OR:
                    xx->fPreviousLong = (msg ? 1 : 0);
                    break;

                default:
                    break;
                    
            }
            outlet_int(xx->fResultOut, xx->fPreviousLong);
        }
    }
} // cmd_Int
