/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtc_Stop.cpp                                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_Stop().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1999 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1999/12/05                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "mtc.h"

/*------------------------------------ cmd_Stop ---*/
STOP_HEADER(MtcData)
{
    if (xx)
    {
        switch (xx->fInletNumber)
        {
            case 0:
                /* Normal command, normal inlet */
                REPORT_MAX_MESSAGE("STOP")
                mtcDoStop(xx);
                break;

            case 1:
                /* Normal command, wrong inlet */
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected command 'stop' in the right inlet.")
                outlet_bang(xx->fErrorBangOut);
                break;

            default:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected port (%ld) seen", xx->fInletNumber)
                break;
                
        }
    }
} // cmd_Stop