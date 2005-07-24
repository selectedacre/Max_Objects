/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpLocateLowLevel.c                                                     */
/*                                                                                      */
/*  Contains:   The common utility routines.                                            */
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
/*  Created:    2000/11/06                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpLocate.h"

/*------------------------------------ doNameLookup ---*/
void doNameLookup
  (TcpLocatePtr xx,
   Ptr          name)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(name)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus     err;
    InetHostInfo info;
    InetHost     first_ip;

    WRAP_OT_CALL(xx, err, "OTInetStringToAddress", OTInetStringToAddress(xx->fService,
                                                                          name, &info))
    if (err == kOTNoError)
    {
      first_ip = info.addrs[0];
      if (first_ip)
      {
        char temp_name[32];

        OTInetHostToString(first_ip, temp_name);
        outlet_anything(xx->fResultOut, gensym(temp_name), 0, NULL_PTR);
      }
      else
        outlet_bang(xx->fErrorBangOut);
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
} /* doNameLookup */

/*------------------------------------ initObject ---*/
bool initObject
  (TcpLocatePtr xx)
{
  bool okSoFar = true;

  if (xx)
  {
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering initObject");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
    xx->fAccessControl = NULL_PTR;
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
#if defined(BE_VERBOSE)
    xx->fVerbose = false;
#endif /* BE_VERBOSE */
    if (! (xx->fResultOut && xx->fErrorBangOut))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or buffer for object")
      okSoFar = false;
    }
  }
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
  Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting initObject");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  return okSoFar;
} /* initObject */
