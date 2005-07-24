/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort_Int.c                                                           */
/*                                                                                      */
/*  Contains:   The routine cmd_Int().                                                  */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2005 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2005/07/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "udpPort.h"

/*------------------------------------ cmd_Int ---*/
Pvoid cmd_Int
  (UdpObjectPtr xx,
   long         msg)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(msg)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  EnterCallback();
  REPORT_MAX_MESSAGE("int")
  if (xx)
  {
  	if (xx->fPartnerKnown)
  	{
#if OPEN_TRANSPORT_SUPPORTED
	    clearDataBuffer(xx->fSendBuffer);
	    xx->fSendBuffer->fDataType = A_LONG;
	    addLongToBuffer(xx->fSendBuffer, xx->fTransferSize, msg, xx->fRawMode);
	    transmitBuffer(xx, xx->fSendBuffer);
#endif /* OPEN_TRANSPORT_SUPPORTED */
		}
		else
			LOG_ERROR_1(OUTPUT_PREFIX "partner not set up")
  }
  ExitMaxMessageHandler()
} /* cmd_Int */
