/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer.h                                                        */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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

#if (! defined(TCPMULTISERVER_H_))
 #define TCPMULTISERVER_H_ /* */
 
 /*#define USE_SYSLOG /* */
 
 #include "MissingAndExtra.h"
 #include "tcpMapping.h"
 
 #define OUR_NAME      "tcpMultiServer"
 #define OUR_RES_NUMB  17144
 #define OUTPUT_PREFIX "tcpMultiServer: "
 
 #define MAX_CLIENTS 100

typedef struct TcpMultiServerData * TcpMultiServerPtr;

struct TcpConnectionData
{
  TcpMultiServerPtr fOwner;
  DataBufferPtr     fReceiveBuffer;
  DataBufferPtr     fSendBuffer;
  EndpointRef       fDataEndpoint;
  PSymbol           fPartnerName;
  ulong             fClientAddress;
  ulong             fClientPort;
  UInt32            fServiceType;
  TcpObjectState    fState;
  ushort            fIdentifier;
  bool              fActive;
  bool              fRawMode;
}; /* TcpConnectionData */

typedef TcpConnectionData * TcpConnectionPtr;

typedef TcpConnectionPtr * TcpConnectionHdl;

struct TcpMultiServerData
{
  Object             fObject;
  POutlet            fErrorBangOut;
  POutlet            fResultOut;
  PQelem             fErrorQueue;
  PQelem             fRebindQueue;
  PQelem             fReceiveQueue;
  AccessOTControlPtr fAccessControl;
  TcpBufferLinkHdl   fLinkBase;
  TcpBufferLinkPtr   fPoolHead;
  TcpBufferLinkPtr   fPoolTail;
  DataBufferHdl      fBufferBase;
  TcpBufferLinkPtr   fReceiveHead;
  TcpBufferLinkPtr   fReceiveTail;
  EndpointRef        fListenEndpoint;
  PSymbol            fSelfName;
  ulong              fServerAddress;
  ushort             fServerPort;
  UInt32             fServiceType;
  short              fAddressSize;
  TcpObjectState     fState;
  bool               fClosing;
  bool               fVerbose;
  ushort             fActiveConnections;
  ushort             fMaximumConnections;
  TcpConnectionHdl   fConnectionBase;
  TcpConnectionHdl   fConnections;
 #if defined(COMPILE_FOR_CATS)
  OTNotifyUPP        fDataNotifier;
  OTNotifyUPP        fListenNotifier;
 #endif /* COMPILE_FOR_CATS */
}; /* TcpMultiServerData */

Pvoid cmd_Disconnect
  (TcpMultiServerPtr xx,
   long              client);

Pvoid cmd_Listen
  (TcpMultiServerPtr xx,
   PSymbol           onOff);

Pvoid cmd_Mode
  (TcpMultiServerPtr xx,
   long              client,
   PSymbol           rawOrCooked);

Pvoid cmd_Port
  (TcpMultiServerPtr xx,
   long              number);

Pvoid cmd_Self
  (TcpMultiServerPtr xx);

Pvoid cmd_Send
  (TcpMultiServerPtr xx,
   PSymbol           message,
   short             argc,
   PAtom             argv);

Pvoid cmd_Status
  (TcpMultiServerPtr xx,
   long              client);

Pvoid cmd_Verbose
  (TcpMultiServerPtr xx,
   PSymbol           onOff);

bool initObject
  (TcpMultiServerPtr xx,
   long              port,
   long              numBuffers);

bool makeReceiveBufferAvailable
  (TcpConnectionPtr connection);

void presetObjectPointers
  (TcpMultiServerPtr xx);

void releaseObjectMemory
  (TcpMultiServerPtr xx);

void setConnectionState
  (TcpConnectionPtr			connection,
   const TcpObjectState	newState);

void setObjectState
  (TcpMultiServerPtr		xx,
   const TcpObjectState	newState);

bool tcpMultiServerDisconnect
  (TcpMultiServerPtr	xx,
   TcpConnectionPtr		connection,
   const bool					forced);

pascal void tcpMultiServerDataNotifier
  (Pvoid       context,
   OTEventCode code,
   OTResult    result,
   Pvoid       cookie);

pascal void tcpMultiServerListenNotifier
  (Pvoid       context,
   OTEventCode code,
   OTResult    result,
   Pvoid       cookie);

bool tcpMultiServerSetPort
  (TcpMultiServerPtr	xx,
   const bool					bangOnError);

TcpConnectionPtr tcpMultiServerValidateClient
  (TcpMultiServerPtr	xx,
   const long					client);

void transmitBuffer
  (TcpMultiServerPtr xx,
   EndpointRef       out,
   DataBufferPtr     aBuffer,
   const bool        rawMode);

StandardRoutineDeclarations(TcpMultiServerPtr)

#endif /* not TCPMULTISERVER_H_ */
