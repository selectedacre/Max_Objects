/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       pfsm_Status.cpp                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_Status().                                               */
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
/*  Created:    2000/06/12                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "pfsm.h"

/*------------------------------------ cmd_Status ---*/
STATUS_HEADER(PfsmData)
{
    if (xx)
    {
        if (xx->fRunning)
        {
            const char * autoRestart = (xx->fAutoRestart ? "on" : "off");

            if (xx->fCurrentState)
            {
                LOG_POST_4(xx, OUTPUT_PREFIX "running, state: %s, start state: %s, autorestart: %s",
                           xx->fCurrentState->fSymbol->s_name, xx->fStartState->fSymbol->s_name,
                           autoRestart)
            }
            else if (xx->fStartState)
            {
                LOG_POST_3(xx, OUTPUT_PREFIX "running, unknown state, start: %s, autorestart: %s",
                           xx->fStartState->fSymbol->s_name, autoRestart)
            }
            else
            {
                LOG_POST_2(xx, OUTPUT_PREFIX "running, unknown state, no data loaded, autorestart: "
                           "%s", autoRestart)
            }
        }
        else
        {
            LOG_POST_1(xx, OUTPUT_PREFIX "not running")
        }
    }
} // cmd_Status

