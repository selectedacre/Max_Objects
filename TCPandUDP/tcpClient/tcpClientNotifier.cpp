/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpClientNotifier.cpp                                                   */
/*                                                                                      */
/*  Contains:   The routine tcpServerNotifier().                                        */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/08/26                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpClient.h"

#if 0
/*------------------------------------ tcpClientNotifier ---*/
pascal void
tcpClientNotifier(void *      context,
                  OTEventCode code,
                  OTResult    result,
                  void *      cookie)
{
#pragma unused(cookie)
    TcpObjectData * xx = static_cast<TcpObjectData *>(context);

    EnterCallback();
    if (xx)
    {
        OSStatus err;
        // OTResult    endpointState;
        TCall       call;
        TDiscon     discon;
        InetAddress caddr;
        bool        do_error_bang = false;

#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            LOG_POST_3(xx, OUTPUT_PREFIX "notifier code: 0x%lx = %s", long(code),
                       mapEventToString(code))
        }
#endif /* BE_VERBOSE */
        switch (code)
        {
            case T_ACCEPTCOMPLETE:
                break;

            case T_BINDCOMPLETE:
                setObjectState(xx, kTcpStateBound);
                break;

            case T_CONNECT:
                if (kOTNoError == result)
                {
                    call.addr.maxlen = sizeof(caddr);
                    call.addr.buf =  reinterpret_cast<unsigned char *>(&caddr);
                    call.opt.maxlen = 0;
                    call.opt.buf = NULL;
                    call.udata.maxlen = 0;
                    call.udata.buf = NULL;
                    WRAP_OT_CALL(xx, err, "OTRcvConnect", OTRcvConnect(xx->fSocket, &call))
                    if (kOTNoError == err)
                    {
                        setObjectState(xx, kTcpStateConnected);
                    }
                    else
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcvConnect failed (%ld = %s)", err)
                        reportEndpointState(OUR_NAME, xx);
                        do_error_bang = true;
                    }
                }
                break;

            case T_DATA:
                do_error_bang = (! makeReceiveBufferAvailable(xx));
                break;

            case T_DISCONNECT:
                WRAP_OT_CALL(xx, err, "OTRcvDisconnect", OTRcvDisconnect(xx->fSocket, &discon))
                if (kOTNoError == err)
                {
#if defined(BE_VERBOSE)
                    if (xx->fVerbose)
                    {
                        LOG_POST_2(xx, OUTPUT_PREFIX "disconnect reason (%ld)",
                                   static_cast<long>(E2OSStatus(discon.reason)))
                    }
#endif /* BE_VERBOSE */
                }
                else
                {
                    if (kOTNoDisconnectErr == err)
                    {
                        break;
                    }

                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcvDisconnect failed (%ld = %s)", err)
                    reportEndpointState(OUR_NAME, xx);
                    do_error_bang = true;
                }
                setObjectState(xx, kTcpStateBound);
                break;

            case T_DISCONNECTCOMPLETE:
                setObjectState(xx, kTcpStateBound);
                break;

            case T_EXDATA:
                break;

            case T_GODATA:
                break;

            case T_GOEXDATA:
                break;

            case T_LISTEN:
                break;

            case T_MEMORYRELEASED:
                break;

            case T_ORDREL:
                WRAP_OT_CALL(xx, err, "OTRcvOrderlyDisconnect",
                             OTRcvOrderlyDisconnect(xx->fSocket))
                if (kOTNoError != result)
                {
                    if (kOTNoDisconnectErr == err)
                    {
                        break;
                    }

                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcvOrderlyDisconnect failed (%ld = %s)", err)
                    reportEndpointState(OUR_NAME, xx);
                    do_error_bang = true;
                }
                if (kTcpStateDisconnecting != xx->fState)
                {
                    WRAP_OT_CALL(xx, err, "OTSndOrderlyDisconnect",
                                 OTSndOrderlyDisconnect(xx->fSocket))
                    if (kOTNoError != result)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)",
                                     err)
                        reportEndpointState(OUR_NAME, xx);
                        do_error_bang = true;
                    }
                }
                setObjectState(xx, kTcpStateBound);
                break;

            case T_PASSCON:
                break;

            case T_UNBINDCOMPLETE:
                setObjectState(xx, kTcpStateUnbound);
                break;

            default:
                break;
                
        }
        if (do_error_bang)
        {
            signalError(xx);
        }
    }
} // tcpClientNotifier
#endif//0
