/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vscan_List.cpp                                                          */
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
/*  Created:    2000/11/29                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vscan.h"

/*------------------------------------ cmd_List ---*/
LIST_HEADER(VscanData)
{
#pragma unused(message)
    if (xx)
    {
        t_atom * newArg = NULL;
        t_atom * argWalk;
        short    newLength = argc;

        clearPrevious(xx);
        if (checkInput(xx, OUTPUT_PREFIX, argc, argv, xx->fCheck))
        {
            double leftFloat;
            double rightFloat;
            long   leftLong;
            long   rightLong;
            bool   wasFloat = false;
            bool   isFloat = false;

            if (! argc)
            {
                newLength = 1;
            }
            argWalk = newArg = GET_BYTES(newLength, t_atom);
            switch (xx->fOperation)
            {
                case OP_AND:
                case OP_BITAND:
                case OP_MULTIPLY:
                    leftFloat = 1;
                    leftLong = 1;
                    break;

                default:
                    leftFloat = 0;
                    leftLong = 0;
                    break;
                    
            }
            for (long ii = 0; newArg && (ii < argc); ++ii)
            {
                switch (argv[ii].a_type)
                {
                    case A_LONG:
                        if ((OP_AND == xx->fOperation) || (OP_OR == xx->fOperation))
                        {
                            if (ii)
                            {
                                isFloat = false;
                                rightLong = (argv[ii].a_w.w_long ? 1 : 0);
                            }
                            else
                            {
                                wasFloat = false;
                                leftLong = (argv[ii].a_w.w_long ? 1 : 0);
                            }
                        }
                        else if (ii)
                        {
                            isFloat = false;
                            rightLong = argv[ii].a_w.w_long;
                        }
                        else
                        {
                            wasFloat = false;
                            leftLong = argv[ii].a_w.w_long;
                        }
                        break;

                    case A_FLOAT:
                        if ((OP_AND == xx->fOperation) || (OP_OR == xx->fOperation))
                        {
                            if (ii)
                            {
                                isFloat = false;
                                rightLong = (argv[ii].a_w.w_float ? 1 : 0);
                            }
                            else
                            {
                                wasFloat = false;
                                leftLong = (argv[ii].a_w.w_float ? 1 : 0);
                            }
                        }
                        else if (ii)
                        {
                            isFloat = true;
                            rightFloat = argv[ii].a_w.w_float;
                        }
                        else
                        {
                            wasFloat = true;
                            leftFloat = argv[ii].a_w.w_float;
                        }
                        break;
                        
                    default:
                        break;
                        
                }
                if (ii)
                {
                    /* Store the previous element */
                    if (wasFloat)
                    {
                        atom_setfloat(argWalk, TO_DBL(leftFloat));
                    }
                    else
                    {
                        atom_setlong(argWalk, leftLong);
                    }
                    ++argWalk;
                    switch (xx->fOperation)
                    {
                        case OP_ADD:
                            if (wasFloat)
                            {
                                if (isFloat)
                                {
                                    leftFloat += rightFloat;
                                }
                                else
                                {
                                    leftFloat += rightLong;
                                }
                            }
                            else if (isFloat)
                            {
                                leftFloat = leftLong + rightFloat;
                                wasFloat = true;
                            }
                            else
                            {
                                leftLong += rightLong;
                            }
                            break;

                        case OP_AND:
                            leftLong &= rightLong;
                            break;

                        case OP_BITAND:
                            leftLong &= rightLong;
                            break;

                        case OP_BITOR:
                            leftLong |= rightLong;
                            break;

                        case OP_DIVIDE:
                            if (wasFloat)
                            {
                                if (isFloat)
                                {
                                    leftFloat /= rightFloat;
                                }
                                else
                                {
                                    leftFloat /= rightLong;
                                }
                            }
                            else if (isFloat)
                            {
                                leftFloat = (leftLong / rightFloat);
                                wasFloat = true;
                            }
                            else
                            {
                                leftLong /= rightLong;
                            }
                            break;

                        case OP_EXOR:
                            leftLong ^= rightLong;
                            break;

                        case OP_MAX:
                            if (wasFloat)
                            {
                                if (isFloat)
                                {
                                    if (rightFloat > leftFloat)
                                    {
                                        leftFloat = rightFloat;
                                    }
                                }
                                else if (rightLong > leftFloat)
                                {
                                    leftLong = rightLong;
                                    wasFloat = false;
                                }
                            }
                            else if (isFloat)
                            {
                                if (rightFloat > leftLong)
                                {
                                    leftFloat = rightFloat;
                                    wasFloat = true;
                                }
                            }
                            else if (rightLong > leftLong)
                            {
                                leftLong = rightLong;
                            }
                            break;

                        case OP_MIN:
                            if (wasFloat)
                            {
                                if (isFloat)
                                {
                                    if (rightFloat < leftFloat)
                                    {
                                        leftFloat = rightFloat;
                                    }
                                }
                                else if (rightLong < leftFloat)
                                {
                                    leftLong = rightLong;
                                    wasFloat = false;
                                }
                            }
                            else if (isFloat)
                            {
                                if (rightFloat < leftLong)
                                {
                                    leftFloat = rightFloat;
                                    wasFloat = true;
                                }
                            }
                            else if (rightLong < leftLong)
                            {
                                leftLong = rightLong;
                            }
                            break;

                        case OP_MODULUS:
                            leftLong %= rightLong;
                            break;

                        case OP_MULTIPLY:
                            if (wasFloat)
                            {
                                if (isFloat)
                                {
                                    leftFloat *= rightFloat;
                                }
                                else
                                {
                                    leftFloat *= rightLong;
                                }
                            }
                            else if (isFloat)
                            {
                                leftFloat = (leftLong * rightFloat);
                                wasFloat = true;
                            }
                            else
                            {
                                leftLong *= rightLong;
                            }
                            break;

                        case OP_OR:
                            leftLong |= rightLong;
                            break;

                        case OP_SUBTRACT:
                            if (wasFloat)
                            {
                                if (isFloat)
                                {
                                    leftFloat -= rightFloat;
                                }
                                else
                                {
                                    leftFloat -= rightLong;
                                }
                            }
                            else if (isFloat)
                            {
                                leftFloat = leftLong - rightFloat;
                                wasFloat = true;
                            }
                            else
                            {
                                leftLong -= rightLong;
                            }
                            break;

                        default:
                            break;
                            
                    }
                }
            }
            if (argWalk)
            {
                /* Store the previous element */
                if (wasFloat)
                {
                    atom_setfloat(argWalk, TO_DBL(leftFloat));
                }
                else
                {
                    atom_setlong(argWalk, leftLong);
                }
            }
            xx->fPreviousList = newArg;
            xx->fPreviousLength = newLength;
            if (xx->fPreviousList)
            {
                genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
            }
        }
    }
} // cmd_List
