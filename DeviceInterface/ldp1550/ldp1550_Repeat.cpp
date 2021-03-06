/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550_Repeat.cpp                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_Repeat().                                               */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "ldp1550.h"

/*------------------------------------ cmd_Repeat ---*/
REPEAT_HEADER(LdpData)
{
    if (xx)
    {
        long           maxPosition;
        long           workCount;
        long           workFactor;
        LdpCommandCode aCommand = kLdpNoCommand;
        LdpCommandCode speed = kLdpCommandPlayForward;

        if (xx->fFirst)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send 'repeat' command while busy")
            outlet_bang(xx->fErrorBangOut);
        }
        else
        {
            maxPosition = ((kLdpModeChapter == xx->fMode) ? MAX_CHAPTER_NUMBER : MAX_FRAME_NUMBER);
            if ((0 >= position) || (position >= maxPosition) || (0 > repeatCount) ||
                (MAX_REPETITIONS < repeatCount) || (0 > stepFactor) ||
                (MAX_STEP_FACTOR < stepFactor))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "bad argument to command 'repeat'")
                outlet_bang(xx->fErrorBangOut);
            }
            else if ((mode != gEmptySymbol) && (mode != gFwdSymbol) && (mode != gFastSymbol) &&
                     (mode != gStepSymbol) && (mode != gSlowSymbol))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "bad mode for command 'repeat'")
                outlet_bang(xx->fErrorBangOut);
            }
            else
            {
                aCommand = kLdpCommandRepeat;
                if (repeatCount)
                {
                    workCount = repeatCount;
                }
                {
                    workCount = 1;
                }
                if (stepFactor)
                {
                    workFactor = stepFactor;
                }
                else
                {
                    workFactor = 1;
                }
                if (mode == gFastSymbol)
                {
                    speed = kLdpCommandPlayFastForward;
                }
                else if (mode == gSlowSymbol)
                {
                    speed = kLdpCommandPlaySlowForward;
                }
                else if (mode == gStepSymbol)
                {
                    speed = kLdpCommandStepForward;
                }
            }
            if (kLdpNoCommand != aCommand)
            {
                short prevLock = lockout_set(1);
                short numCommands = 12;

                if (kLdpModeFrame == xx->fMode)
                {
                    numCommands += 3;
                }
                if (mode == gStepSymbol)
                {
                    numCommands += 4;
                }
                if (ldpCheckPoolSpace(xx, numCommands))
                {
                    ldpInitCommands(xx);
                    ldpAddCommand(xx, aCommand, kLdpStateAwaitingAck);
                    /* Set the target frame */
                    if (kLdpModeFrame == xx->fMode)
                    {
                        ldpAddCommand(xx,
                                      static_cast<LdpCommandCode>(((position / 10000) % 10) + '0'),
                                      kLdpStateAwaitingAck);
                        ldpAddCommand(xx,
                                      static_cast<LdpCommandCode>(((position / 1000) % 10) + '0'),
                                      kLdpStateAwaitingAck);
                        ldpAddCommand(xx,
                                      static_cast<LdpCommandCode>(((position / 100) % 10) + '0'),
                                      kLdpStateAwaitingAck);
                    }
                    ldpAddCommand(xx, static_cast<LdpCommandCode>(((position / 10) % 10) + '0'),
                                  kLdpStateAwaitingAck);
                    ldpAddCommand(xx, static_cast<LdpCommandCode>((position % 10) + '0'),
                                  kLdpStateAwaitingAck);
                    /* Set the play mode */
                    ldpAddCommand(xx, speed, kLdpStateAwaitingAck);
                    ldpAddCommand(xx, kLdpCommandEnter, kLdpStateAwaitingAck);
                    /* Set the repeat count */
                    ldpAddCommand(xx, static_cast<LdpCommandCode>(((workCount / 10) % 10) + '0'),
                                  kLdpStateAwaitingAck);
                    ldpAddCommand(xx, static_cast<LdpCommandCode>((workCount % 10) + '0'),
                                  kLdpStateAwaitingAck);
                    ldpAddCommand(xx, kLdpCommandEnter, kLdpStateAwaitingAck);
                    /* If step mode, send the step factor */
                    if (mode == gStepSymbol)
                    {
                        ldpAddCommand(xx,
                                      static_cast<LdpCommandCode>(((workFactor / 100) % 10) + '0'),
                                      kLdpStateAwaitingAck);
                        ldpAddCommand(xx,
                                      static_cast<LdpCommandCode>(((workFactor / 10) % 10) + '0'),
                                      kLdpStateAwaitingAck);
                        ldpAddCommand(xx, static_cast<LdpCommandCode>((workFactor % 10) + '0'),
                                      kLdpStateAwaitingAck);
                        ldpAddCommand(xx, kLdpCommandEnter, kLdpStateAwaitingAck);
                    }
                    ldpAddCommand(xx, kLdpCommandSignalAccepted, kLdpStateNotWaiting);
                    ldpAddCommand(xx, kLdpCommandCanInterrupt, kLdpStateAwaitingComplete);
                    ldpAddCommand(xx, kLdpCommandSignalDone, kLdpStateNotWaiting);
                    ldpSendCommand(xx);
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "no room for 'repeat' command")
                    outlet_bang(xx->fErrorBangOut);
                }
                lockout_set(prevLock);
            }
        }
    }
} // cmd_Repeat
