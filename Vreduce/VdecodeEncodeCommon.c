/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       VdecodeEncodeCommon.c                                                   */
/*                                                                                      */
/*  Contains:   Shared routines.                                                        */
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

#include "VdecodeEncodeCommon.h"

/*------------------------------------ CheckBases ---*/
bool CheckBases
  (BaseData * info,
   const long	whichBase1,
   const long	whichBase2,
   const long	whichBase3,
   const long	whichBase4,
   const long	whichBase5)
  {
    bool  wasNegative = false;
    short numBases = -1, index = 0;
    long	workingBase1 = whichBase1;
    
    if (workingBase1 < 0)
    {
      workingBase1 = - workingBase1;
      wasNegative = true;
    }
    if (whichBase5 > 1)
      numBases = 5;
    else if (whichBase4 > 1)
      numBases = 4;
    else if (whichBase3 > 1)
      numBases = 3;
    else if (whichBase2 > 1)
      numBases = 2;
    else if (workingBase1 > 1)
      numBases = 1;
    switch (numBases)
    {
      case 5:
        info->fBases[index++] = whichBase5;
        // fall through
        
      case 4:
        info->fBases[index++] = whichBase4;
        // fall through
        
      case 3:
        info->fBases[index++] = whichBase3;
        // fall through
        
      case 2:
        info->fBases[index++] = whichBase2;
        // fall through
        
      case 1:
        info->fBases[index] = workingBase1;
        break;
        
      default:
        return false;
        
    }
    for (index = 0; index < numBases; ++index)
    {
      if (info->fBases[index] < 2)
        return false;

    }
    info->fNumBases = numBases;
    info->fFirstBaseIsNegative = (wasNegative || (numBases == 1));
    return true;
  } /* CheckBases */

/*------------------------------------ CheckInput ---*/
bool CheckInput
  (Qchar				name,
   const short  argc,
   PAtom  			argv)
{
  bool  okSoFar = true;

  for (short ii = 0; okSoFar && (ii < argc); ++ii)
  {
    switch (argv[ii].a_type)
    {
      case A_LONG:
        if (argv[ii].a_w.w_long < 0)
        {
          LOG_ERROR_2("%snegative value in input list", name)
          okSoFar = false;
        }
        break;

      case A_SYM:
        LOG_ERROR_3("%ssymbol (%s) in input list", name, argv[ii].a_w.w_sym->s_name)
        okSoFar = false;
        break;

      case A_FLOAT:
        LOG_ERROR_3("%sfloating point value (%g) in input list", name,
                    double(argv[ii].a_w.w_float))
        okSoFar = false;
        break;

      default:
        LOG_ERROR_3("%sinput of an unknown type (%hd) seen", name, argv[ii].a_type)
        okSoFar = false;
        break;

    }
  }
  return okSoFar;
} /* CheckInput */
