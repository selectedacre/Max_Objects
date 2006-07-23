/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer_Disconnect.c                                             */
/*                                                                                      */
/*  Contains:   The routine cmd_Disconnect().                                           */
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
/*  Created:    2000/10/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpMultiServer.h"

/*------------------------------------ cmd_Disconnect ---*/
Pvoid cmd_Disconnect
  (TcpMultiServerPtr xx,
   long              client)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(client)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  EnterCallback();
  REPORT_MAX_MESSAGE("disconnect")
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    TcpConnectionPtr connection;

    if (client)
    {
      connection = tcpMultiServerValidateClient(xx, client);
      if (connection)
      {
        if (tcpMultiServerDisconnect(xx, connection, USE_FORCED_DISCONNECTS))
        {
 #if defined(BE_VERBOSE)
          if (xx->fVerbose)
            LOG_POST_1(OUTPUT_PREFIX "disconnecting from client")
 #endif /* BE_VERBOSE */
        }
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "invalid client (%ld)", client)
        signalError(xx);
      }
    }
    else
    {
      for (short ii = 0; ii < xx->fMaximumConnections; ++ii)
      {
        connection = *(xx->fConnections + ii);
        if (connection && (connection->fDataEndpoint != kOTInvalidEndpointRef) &&
              connection->fActive)
          tcpMultiServerDisconnect(xx, connection, true);
      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* cmd_Disconnect */
