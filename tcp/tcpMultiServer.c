/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer.c                                                        */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the tcpMultiServer        */
/*              module.                                                                 */
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

#define IS_MAIN /* */
#include "tcpMultiServer.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid tcpMultiServerCreate
  (long port,
   long clients,
   long numBuffers);

Pvoid tcpMultiServerFree
  (TcpMultiServerPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  if (isOpenTransportAvailable(OUR_NAME))
  {
    /* Allocate class memory and set up class. */
    setup(reinterpret_cast<t_messlist**>(&gClass),
          reinterpret_cast<method>(tcpMultiServerCreate),
          reinterpret_cast<method>(tcpMultiServerFree), short(sizeof(TcpMultiServerData)),
          reinterpret_cast<method>(0L), A_DEFLONG, A_DEFLONG, A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addbang(reinterpret_cast<method>(cmd_Bang));
    addmess(reinterpret_cast<method>(cmd_Disconnect), "disconnect", A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Info), "info", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Listen), "listen", A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Mode), "mode", A_LONG, A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Port), "port", A_LONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Self), "self", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Send), "send", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Status), "status", A_DEFLONG, A_NOTHING);
#if defined(BE_VERBOSE)
    addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
#endif /* BE_VERBOSE */
    /* Add object class to New Object List */
    finder_addclass("Devices", OUR_NAME);
    /* Make sure that our string list resource is present */
    rescopy('STR#', OUR_RES_NUMB);
   	loadAboutBoxData();
    gDollarSymbol = gensym("$");
    gEmptySymbol = gensym("");
    gMaxSymbol = gensym("max");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gRawSymbol = gensym("raw");
    gReplySymbol = gensym("reply");
    gSelfSymbol = gensym("self");
    gStatusSymbol = gensym("status");
    setUpStateSymbols();
    gVersionNumber = reportVersion(OUR_NAME);
    loadOtherSegments();
  }
#if (! OPEN_TRANSPORT_SUPPORTED)
  LOG_ERROR_1(OUTPUT_PREFIX "not supported on Max/68K")
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  ExitCodeResource();
} /* main */

/*------------------------------------ tcpMultiServerConstructConnections ---*/
static bool tcpMultiServerConstructConnections
  (TcpMultiServerPtr xx)
{
  TcpConnectionHdl   ptr_walker = xx->fConnections;
  TcpConnectionPtr   a_connection;
  DataBufferPtr      next_buffer = *xx->fBufferBase;
  bool               okSoFar = true;
#if OPEN_TRANSPORT_SUPPORTED
  TEndpointInfo      info;
  OSStatus           result;
  OTConfigurationRef data_config = NULL_PTR, a_config;
#endif /* OPEN_TRANSPORT_SUPPORTED */

#if OPEN_TRANSPORT_SUPPORTED
 #if SYSLOG_AVAILABLE
  Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering tcpMultiServerConstructConnections");
 #endif /* SYSLOG_AVAILABLE */
  data_config = OTCreateConfiguration(kTCPName);
  if ((data_config == kOTInvalidConfigurationPtr) ||
      (data_config == kOTNoMemoryConfigurationPtr))
  {
    LOG_ERROR_1(OUTPUT_PREFIX "unable to obtain an OT data configuration")
    okSoFar = false;
  }
#endif /* OPEN_TRANSPORT_SUPPORTED */
  HLock(reinterpret_cast<Handle>(xx->fConnectionBase));
  a_connection = *xx->fConnectionBase;
  memset(xx->fConnections, 0, xx->fMaximumConnections * sizeof(TcpConnectionPtr));
  memset(*xx->fConnectionBase, 0, sizeof(TcpConnectionData) * xx->fMaximumConnections);
  for (ushort ii = 0; okSoFar && (ii < xx->fMaximumConnections); ++ii)
  {
    *ptr_walker++ = a_connection;
    a_connection->fDataEndpoint = kOTInvalidEndpointRef;
    a_connection->fOwner = xx;
    a_connection->fIdentifier = static_cast<ushort>(ii + 1);
    a_connection->fActive = false;  
    a_connection->fSendBuffer = next_buffer;
    a_connection->fClientAddress = 0L;
    a_connection->fClientPort = 0;
    a_connection->fPartnerName = NULL_PTR;
    setConnectionState(a_connection, TCP_OBJECT_UNBOUND);
#if OPEN_TRANSPORT_SUPPORTED
    a_config = OTCloneConfiguration(data_config);
    if (! a_config)
      okSoFar = false;
    if (okSoFar)
    {
 #if defined(COMPILE_FOR_OSX_4)
      a_connection->fDataEndpoint = OTOpenEndpointInContext(a_config, 0, &info, &result,
                                                            xx->fAccessControl->fContext);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      a_connection->fDataEndpoint = OTOpenEndpoint(a_config, 0, &info, &result);
 #endif /* COMPILE_FOR_OS9_4 */
      if (result == kOTNoError)
        a_connection->fServiceType = info.servtype;
      else
      {
        REPORT_ERROR(OUTPUT_PREFIX "unable to obtain an endpoint (%ld = %s)", result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTSetAsynchronous",
                    OTSetAsynchronous(a_connection->fDataEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTSetAsynchronous failed (%ld = %s)", result)
        reportEndpointState(xx, a_connection->fDataEndpoint);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTSetBlocking",
                    OTSetBlocking(a_connection->fDataEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
        reportEndpointState(xx, a_connection->fDataEndpoint);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
 #if defined(COMPILE_FOR_OSX_4)
      WRAP_OT_CALL(xx, result, "OTInstallNotifier",
                    OTInstallNotifier(a_connection->fDataEndpoint,
                                      xx->fDataNotifier, a_connection))
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      WRAP_OT_CALL(xx, result, "OTInstallNotifier",
                    OTInstallNotifier(a_connection->fDataEndpoint,
                                      tcpMultiServerDataNotifier, a_connection))
 #endif /* COMPILE_FOR_OS9_4 */
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
        reportEndpointState(xx, a_connection->fDataEndpoint);
        okSoFar = false;
      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
    a_connection->fReceiveBuffer =
                    reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(a_connection->fSendBuffer,
                                                              BUFF_MEMORY_TO_ALLOC));
    next_buffer = reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(a_connection->fReceiveBuffer,
                                                            BUFF_MEMORY_TO_ALLOC));
    a_connection = reinterpret_cast<TcpConnectionPtr>(ADD_TO_ADDRESS(a_connection,
                                                              sizeof(TcpConnectionData)));   
  }
#if OPEN_TRANSPORT_SUPPORTED
  if ((data_config != kOTInvalidConfigurationPtr) &&
      (data_config != kOTNoMemoryConfigurationPtr))
    OTDestroyConfiguration(data_config);
 #if SYSLOG_AVAILABLE
  Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting tcpMultiServerConstructConnections");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  return okSoFar;
} /* tcpMultiServerConstructConnections */

/*------------------------------------ tcpMultiServerCreate ---*/
Pvoid tcpMultiServerCreate
  (long port,
   long clients,
   long numBuffers)
{
  TcpMultiServerPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<TcpMultiServerPtr>(newobject(gClass));
  if (xx)
  {
    bool               okSoFar = true;
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus           result;
    OTConfigurationRef this_config;
#endif /* OPEN_TRANSPORT_SUPPORTED */

#if defined(BE_VERBOSE)
    xx->fVerbose = false;
#endif /* BE_VERBOSE */
    presetObjectPointers(xx);
    if ((port < 0) || (port > MAX_PORT) || (clients < 0) || (clients > MAX_CLIENTS) ||
        (numBuffers < 0))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "invalid parameters for device")
      okSoFar = false;
    }
    if (okSoFar)
    {
      xx->fServerAddress = 0L;
      xx->fMaximumConnections = static_cast<ushort>(clients ? clients : 5);
      if (numBuffers == 0)
        numBuffers = (NUM_RX_BUFFERS * clients);
      xx->fActiveConnections = 0;
      okSoFar = initObject(xx, port, numBuffers);
      if (okSoFar)
      {
        xx->fConnections = GETBYTES(xx->fMaximumConnections, TcpConnectionPtr);
        xx->fConnectionBase =
                reinterpret_cast<TcpConnectionHdl>(newhandle(long(sizeof(TcpConnectionData) *
                                                            xx->fMaximumConnections)));
        if (! (xx->fConnections || xx->fConnectionBase))
          okSoFar = false;
      }
    }
#if OPEN_TRANSPORT_SUPPORTED
 #if defined(COMPILE_FOR_OSX_4)
    xx->fDataNotifier = NewOTNotifyUPP(tcpMultiServerDataNotifier);
    xx->fListenNotifier = NewOTNotifyUPP(tcpMultiServerListenNotifier);
 #endif /* COMPILE_FOR_OSX_4 */
    if (okSoFar)
    {
      xx->fAccessControl = acquireOpenTransport(OUR_NAME, static_cast<ushort>(port), true);
      if (! xx->fAccessControl)
        okSoFar = false;
    }
    /* Get the desired configurations */
    if (okSoFar)
    {
      this_config = OTCreateConfiguration("tilisten,tcp");
      if ((this_config == kOTInvalidConfigurationPtr) ||
          (this_config == kOTNoMemoryConfigurationPtr))
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to obtain an OT listener configuration")
        okSoFar = false;
      }
    }
    /* Set up the endpoint */
    if (okSoFar)
    {
      TEndpointInfo info;

 #if defined(COMPILE_FOR_OSX_4)
      xx->fListenEndpoint = OTOpenEndpointInContext(this_config, 0, &info, &result,
                                                      xx->fAccessControl->fContext);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      xx->fListenEndpoint = OTOpenEndpoint(this_config, 0, &info, &result);
 #endif /* COMPILE_FOR_OS9_4 */
      if (result == kOTNoError)
      {
        xx->fServiceType = info.servtype;
        xx->fAddressSize = short(info.addr);
      }
      else
      {
        REPORT_ERROR(OUTPUT_PREFIX "unable to obtain an endpoint (%ld = %s)", result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      InetInterfaceInfo interfaceInfo;

      WRAP_OT_CALL(xx, result, "OTInetGetInterfaceInfo",
                    OTInetGetInterfaceInfo(&interfaceInfo, kDefaultInetInterface)); 
      if (result == kOTNoError)
        xx->fServerAddress = interfaceInfo.fAddress;
      else
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTInetGetInterfaceInfo failed (%ld = %s)", result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTSetAsynchronous",
                    OTSetAsynchronous(xx->fListenEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTSetAsynchronous failed (%ld = %s)", result)
        reportEndpointState(xx, xx->fListenEndpoint);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTSetBlocking", OTSetBlocking(xx->fListenEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
        reportEndpointState(xx, xx->fListenEndpoint);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
 #if defined(COMPILE_FOR_OSX_4)
      WRAP_OT_CALL(xx, result, "OTInstallNotifier",
                    OTInstallNotifier(xx->fListenEndpoint, xx->fListenNotifier,
                                      xx))
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      WRAP_OT_CALL(xx, result, "OTInstallNotifier",
                    OTInstallNotifier(xx->fListenEndpoint, tcpMultiServerListenNotifier,
                                      xx))
 #endif /* COMPILE_FOR_OS9_4 */
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
        reportEndpointState(xx, xx->fListenEndpoint);
        okSoFar = false;
      }
    }
    if (okSoFar)
      okSoFar = tcpMultiServerSetPort(xx, false);
    if (okSoFar)
      okSoFar = tcpMultiServerConstructConnections(xx);
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* tcpMultiServerCreate */

/*------------------------------------ tcpMultiServerFree ---*/
Pvoid tcpMultiServerFree
  (TcpMultiServerPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus         result;
    TcpConnectionPtr connection;

    xx->fClosing = true;
    // Close off all other connections:
    for (ushort ii = 0; ii < xx->fMaximumConnections; ++ii)
    {
      connection = *(xx->fConnections + ii);
      if (connection && (connection->fDataEndpoint != kOTInvalidEndpointRef) &&
          connection->fActive)
        tcpMultiServerDisconnect(xx, connection, true);
    }
    if (xx->fListenEndpoint != kOTInvalidEndpointRef)
    {
      switch (xx->fState)
      {
        case TCP_OBJECT_CONNECTED:
          tcpMultiServerDisconnect(xx, NULL_PTR, true); /*!!*/
          /* Fall through */

        case TCP_OBJECT_LISTENING:
          /* Fall through */

        case TCP_OBJECT_BOUND:
          WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fListenEndpoint))
          if (result != kOTNoError)
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
		        reportEndpointState(xx, xx->fListenEndpoint);
          }
          /* Fall through */

        case TCP_OBJECT_UNBOUND:
          WRAP_OT_CALL(xx, result, "OTCloseProvider",
                        OTCloseProvider(xx->fListenEndpoint))
          if (result == kOTNoError)
            xx->fListenEndpoint = kOTInvalidEndpointRef;
          else
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
		        reportEndpointState(xx, xx->fListenEndpoint);
          }
          break;

      }
    }
 #if defined(COMPILE_FOR_OSX_4)
    DisposeOTNotifyUPP(xx->fDataNotifier);
    DisposeOTNotifyUPP(xx->fListenNotifier);
 #endif /* COMPILE_FOR_OSX_4 */
#endif /* OPEN_TRANSPORT_SUPPORTED */
    releaseObjectMemory(xx);
#if OPEN_TRANSPORT_SUPPORTED
    relinquishOpenTransport(OUR_NAME, &xx->fAccessControl);
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* tcpMultiServerFree */

/*------------------------------------ tcpMultiServerSetPort ---*/
bool tcpMultiServerSetPort
  (TcpMultiServerPtr	xx,
   const bool					bangOnError)
{
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus result;

 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering tcpMultiServerSetPort");
 #endif /* SYSLOG_AVAILABLE */
    switch (xx->fState)
    {
      case TCP_OBJECT_LISTENING:
        LOG_ERROR_1(OUTPUT_PREFIX "actively listening")
        okSoFar = false;
        break;
        /* Fall through */

      case TCP_OBJECT_BOUND:
        WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fListenEndpoint))
        if (result != kOTNoError)
        {
          REPORT_ERROR(OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
	        reportEndpointState(xx, xx->fListenEndpoint);
          okSoFar = false;
        }
        setObjectState(xx, TCP_OBJECT_UNBOUND);
        /* Fall through */

      case TCP_OBJECT_UNBOUND:
        break;

      default:
        LOG_ERROR_3(OUTPUT_PREFIX "unexpected state (%ld = %s)", long(xx->fState),
                    mapStateToSymbol(xx->fState)->s_name)
        okSoFar = false;
        break;

    }
    if (okSoFar)
    {
      TBind       bind_request;
      InetAddress in_address;

      OTInitInetAddress(&in_address, xx->fServerPort, xx->fServerAddress);       
      bind_request.addr.len = sizeof(in_address);
      bind_request.addr.buf = reinterpret_cast<Puchar>(&in_address);
      bind_request.qlen = 1;
      WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fListenEndpoint, &bind_request,
                                                NULL_PTR))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
        reportEndpointState(xx, xx->fListenEndpoint);
        okSoFar = false;
      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if ((! okSoFar) && bangOnError)
      signalError(xx);
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting tcpMultiServerSetPort");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  }
  return okSoFar;
} /* tcpMultiServerSetPort */

/*------------------------------------ tcpMultiServerDisconnect ---*/
bool tcpMultiServerDisconnect
  (TcpMultiServerPtr	xx,
   TcpConnectionPtr		connection,
   const bool					forced)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(connection, forced)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  bool okSoFar = true;

  if (xx && connection)
  {
#if OPEN_TRANSPORT_SUPPORTED
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering tcpMultiServerDisconnect (%d)",
            connection->fIdentifier);
 #endif /* SYSLOG_AVAILABLE */
    switch (connection->fState)
    {
      case TCP_OBJECT_UNBOUND:
        if (! xx->fClosing)
        {
          LOG_ERROR_1(OUTPUT_PREFIX "not bound")
          okSoFar = false;
        }
        break;

      case TCP_OBJECT_BOUND:
        if (! xx->fClosing)
        {
          LOG_ERROR_1(OUTPUT_PREFIX "not connected")
          okSoFar = false;
        }
        break;

      case TCP_OBJECT_CONNECTED:
        break;

    }
    if (okSoFar || forced)
    {
      OSStatus result;

      if ((! forced) && ((connection->fServiceType == T_COTS_ORD) ||
          (connection->fServiceType == T_TRANS_ORD)))
      {
        WRAP_OT_CALL(xx, result, "OTSndOrderlyDisconnect",
                      OTSndOrderlyDisconnect(connection->fDataEndpoint))
        if (result == kOTNoDataErr)
          setConnectionState(connection, TCP_OBJECT_DISCONNECTING);
        else
        {
          REPORT_ERROR(OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)", result)
	        reportEndpointState(xx, connection->fDataEndpoint);
          okSoFar = false;
        }
      }
      else
      {
        TCall       snd_call;
        InetAddress in_addr;

        OTInitInetAddress(&in_addr, xx->fServerPort, xx->fServerAddress);
        OTMemzero(&snd_call, sizeof(snd_call));
        snd_call.addr.len = sizeof(in_addr);              
        snd_call.addr.buf = reinterpret_cast<Puchar>(&in_addr);
        WRAP_OT_CALL(xx, result, "OTSndDisconnect",
                      OTSndDisconnect(connection->fDataEndpoint, &snd_call))
        if (result == kOTNoError)
          setConnectionState(connection, TCP_OBJECT_BOUND);
        else
        {
          REPORT_ERROR(OUTPUT_PREFIX "OTSndDisconnect failed (%ld = %s)", result)
	        reportEndpointState(xx, connection->fDataEndpoint);
          okSoFar = false;
        }
      }
    }
    if (! okSoFar)
      signalError(xx);
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting tcpMultiServerDisconnect");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  return okSoFar;
} /* tcpMultiServerDisconnect */

/*------------------------------------ tcpMultiServerValidateClient ---*/
TcpConnectionPtr tcpMultiServerValidateClient
  (TcpMultiServerPtr	xx,
   const long					client)
{
  TcpConnectionPtr candidate = NULL_PTR;

  if ((client > 0) && (client <= xx->fMaximumConnections))
    candidate = *(xx->fConnections + client - 1);
  return candidate;
} /* tcpMultiServerValidateClient */

StandardInfoRoutine(TcpMultiServerPtr)