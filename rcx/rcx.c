/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx.c                                                                   */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the rcx module.           */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/03/31                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "rcx.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid rcxCreate
  (PSymbol portClass,
   PSymbol whichPort);

Pvoid rcxFree
  (RcxControlPtr xx);

uint32 rcxNotify
  (GHNOTIFYCODE notifyCode,
   PBKRESULT    resultCode,
   GHSTACK      ghostStack,
   GHQUEUE      queue,
   GHCOMMAND    command,
   Pvoid        context);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
#if __powerc
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(rcxCreate), 
        reinterpret_cast<method>(rcxFree), short(sizeof(RcxControl)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ClearMemory), "clearmemory", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ClearSensor), "clearsensor", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ClearSound), "clearsound", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ContinueTask), "continuetask", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_FloatOutput), "floatoutput", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetAllSensors), "getallsensors", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetAllVariables), "getallvariables", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetBattery), "getbattery", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSensorMode), "getsensormode", A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSensorType), "getsensortype", A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSensorValue), "getsensorvalue", A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSlot), "getslot", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetVariable), "getvariable", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetVersion), "getversion", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Mute), "mute", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_PlaySound), "playsound", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_PlayTone), "playtone", A_LONG, A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Run), "run", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetMessage), "setmessage", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetOutputDirection), "setoutputdirection", A_LONG,
          A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetOutputPower), "setoutputpower", A_LONG, A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetSensorMode), "setsensormode", A_LONG, A_SYM,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetSensorType), "setsensortype", A_LONG, A_SYM,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetSlot), "setslot", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetVariable), "setvariable", A_LONG, A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetWatch), "setwatch", A_LONG, A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StartOutput), "startoutput", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StartTask), "starttask", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StopAllTasks), "stopalltasks", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StopOutput), "stopoutput", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StopTask), "stoptask", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_TurnOff), "turnoff", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Unmute), "unmute", A_NOTHING);
 #if defined(BE_VERBOSE)
  addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
 #endif /* BE_VERBOSE */
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gAllSensorsSymbol = gensym("allsensors");
  gAllVariablesSymbol = gensym("allvariables");
  gAngleSymbol = gensym("angle");
  gAngleStepSymbol = gensym("anglestep");
  gBackwardSymbol = gensym("backward");
  gBatterySymbol = gensym("battery");
  gBeepSymbol = gensym("beep");
  gBooleanSymbol = gensym("boolean");
  gCelsiusSymbol = gensym("celsius");
  gEmptySymbol = gensym("");
  gErrorSymbol = gensym("error");
  gFahrenheitSymbol = gensym("fahrenheit");
  gFastSweepSymbol = gensym("fastsweep");
  gForwardSymbol = gensym("forward");
  gKeyClickSymbol = gensym("keyclick");
  gNoSensorSymbol = gensym("nosensor");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gPercentSymbol = gensym("percent");
  gPeriodCountSymbol = gensym("periodcount");
  gRawSymbol = gensym("raw");
  gReflectionSymbol = gensym("reflection");
  gReverseSymbol = gensym("reverse");
  gSensorModeSymbol = gensym("sensormode");
  gSensorSymbol = gensym("sensor");
  gSensorTypeSymbol = gensym("sensortype");
  gSerialSymbol = gensym("serial");
  gSlotSymbol = gensym("slot");
  gSweepDownSymbol = gensym("sweepdown");
  gSweepUpSymbol = gensym("sweepup");
  gSwitchSymbol = gensym("switch");
  gTemperatureSymbol = gensym("temperature");
  gTransitionSymbol = gensym("transition");
  gUnknownSymbol = gensym("unknown");
  gUsbSymbol = gensym("usb");
  gVariableSymbol = gensym("variable");
  gVersionSymbol = gensym("version");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
#else /* not __powerc */
  error(OUTPUT_PREFIX "not supported on Max/68K");
#endif /* not __powerc */
  ExitCodeResource();
} /* main */

/*------------------------------------ rcxCopyReply ---*/
uint32 rcxCopyReply
  (RcxControlPtr	xx,
   Puchar       	replyBuffer,
   const uint32		replySize)
{
  if (xx)
  {
    uint32        length = xx->fReplyLength;
    const uchar * source = xx->fReplyBuffer + 1;

    if (length > replySize)
      length = replySize;
    for (uint32 ii = 0; ii < length; ii++)
      *replyBuffer++ = *source++;
    return length;

  }
  return 0;
} /* rcxCopyReply */

/*------------------------------------ rcxCreate ---*/
Pvoid rcxCreate
  (PSymbol portClass,
   PSymbol whichPort)
{
#if (! __powerc)
 #pragma unused(portClass, whichPort)
#endif /* not __powerc */
  RcxControlPtr xx = NULL_PTR;

  EnterCallback();
  LOG_ENTER()
#if __powerc
  xx = static_cast<RcxControlPtr>(newobject(gClass));
  if (xx)
  {
    PBKRESULT ghostResult;
    bool      okSoFar = true;

    xx->fGhostStack = NULL_PTR;
    xx->fDeviceOpen = xx->fSynchronized = false;
    xx->fUseUSB = true;
    if (portClass == gSerialSymbol)
      xx->fUseUSB = false;
    else if ((portClass == gEmptySymbol) || (portClass == gUsbSymbol))
      xx->fUseUSB = true;
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid port class (%s) for device", portClass->s_name)
      okSoFar = false;
    }
    if (okSoFar)
    {
      xx->fReplyBuffer = GETBYTES(MAX_REPLY_BUFFER, uchar);
      xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
      xx->fCommandComplete = static_cast<POutlet>(bangout(xx));
      xx->fDataOut = static_cast<POutlet>(outlet_new(xx, 0L));   /* normally just a list */
      if (xx->fReplyBuffer && xx->fErrorBangOut && xx->fCommandComplete && xx->fDataOut)
      {
        // Create a Ghost stack for communication...
        ghostResult = GhCreateStack(xx->fUseUSB ? "LEGO.Pbk.CommStack.Port.USB" :
                                                  "LEGO.Pbk.CommStack.Port.RS232",
                                    "LEGO.Pbk.CommStack.Protocol.IR",
                                    "LEGO.Pbk.CommStack.Session", &xx->fGhostStack);
        if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
        {
          LOG_ERROR_2(OUTPUT_PREFIX "unable to create Ghost stack for device --> 0x%lX",
                      long(ghostResult))
          okSoFar = false;
        }
        if (okSoFar)
        {
          static char deviceName[256], portName[256];

          // Locate the first available device...
          ghostResult = GhSelectFirstDevice(xx->fGhostStack, portName, sizeof(portName));
          if (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult))
          {
            // If we're being picky, make sure that it matches!
            if (whichPort == gEmptySymbol)
            {
              GhGetDeviceName(xx->fGhostStack, deviceName, sizeof(deviceName));
              LOG_POST_3(OUTPUT_PREFIX "port='%s' device='%s'", portName, deviceName)
            }
            else
            {
              PSymbol deviceSymbol;

              okSoFar = false;
              do
              {
                GhGetDeviceName(xx->fGhostStack, deviceName, sizeof(deviceName));
                LOG_POST_3(OUTPUT_PREFIX "port='%s' device='%s'", portName, deviceName)
                deviceSymbol = gensym(deviceName);
                if (deviceSymbol == whichPort)
                {
                  okSoFar = true;
                  break;

                }
                ghostResult = GhSelectNextDevice(xx->fGhostStack, deviceName,
                                                  sizeof(deviceName));
              }
              while (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult));
              if (! okSoFar)
                LOG_ERROR_3(OUTPUT_PREFIX "unable to locate matching device (%s) for "
                            "given port class (%s)", whichPort->s_name,
                            portClass->s_name)
            }
          }
          else
          {
            LOG_ERROR_3(OUTPUT_PREFIX "unable to locate device for given port class "
                        "(%s) --> 0x%lX", portClass->s_name, long(ghostResult))
            okSoFar = false;
          } 
        }
        if (okSoFar)
        {
          // We've selected the device, so connect to it...
          ghostResult = GhOpen(xx->fGhostStack);
          if (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult))
            xx->fDeviceOpen = true;
          else
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to open connection to device --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
        }
        if (okSoFar)
        {
          ghostResult = GhGetRetries(xx->fGhostStack, &xx->fExecuteRetries,
                                      &xx->fDownloadRetries);
          if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to get retry counts --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
        }
        if (okSoFar)
        {
          // Set the notification mode...
 #if defined(USE_NOWAIT)
          ghostResult = GhSetNoWaitMode(xx->fGhostStack, rcxNotify);
          if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to set device to no-wait mode --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
 #else /* not USE_NOWAIT */
          //ghostResult = GhSetWaitMode(xx->fGhostStack, rcxNotify);
          //if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          //{
          //  LOG_ERROR_2(OUTPUT_PREFIX "unable to set device to wait mode --> 0x%lX",
          //              long(ghostResult))
          //  okSoFar = false;
          //}
 #endif /* not USE_NOWAIT */
        }
      }
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or clock for device")
        okSoFar = false;
      }
    }
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
#endif /* __powerc */
  ExitCodeResource();
  return xx;
} /* rcxCreate */

/*------------------------------------ rcxFree ---*/
Pvoid rcxFree
  (RcxControlPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if __powerc
    if (xx->fGhostStack)
    {
      PBKRESULT ghostResult;

      if (xx->fDeviceOpen)
      {
        ghostResult = GhClose(xx->fGhostStack);
        if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          LOG_ERROR_2(OUTPUT_PREFIX "unable to close port --> 0x%lX", long(ghostResult))
      }
      ghostResult = GhDestroyStack(xx->fGhostStack);
      if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
        LOG_ERROR_2(OUTPUT_PREFIX "unable to free Ghost stack --> 0x%lX",
                    long(ghostResult))
      xx->fGhostStack = NULL_PTR;
    }
    FREEBYTES(xx->fReplyBuffer, MAX_REPLY_BUFFER)
#endif /* __powerc */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* rcxFree */

/*------------------------------------ rcxGetReplyByte ---*/
uchar rcxGetReplyByte
  (RcxControlPtr	xx,
   const ushort		index)
{
  if (xx)
    return *(xx->fReplyBuffer + index + 1);

  return 0;
} /* rcxGetReplyByte */

/*------------------------------------ rcxGetValue ---*/
bool rcxGetValue
  (RcxControlPtr	xx,
   const ulong		descriptor,
   long &					result)
{
  uchar valueCommand[] = { RCX_POLL_CMD, 0, 0 };

  valueCommand[1] = GET_RCX_VALUE_TYPE(descriptor);
  valueCommand[2] = static_cast<uchar>(GET_RCX_VALUE_DATA(descriptor));
  if (rcxSendCommand(xx, valueCommand, sizeof(valueCommand), RCX_POLL_REPLY, true))
  {
    uchar val1 = rcxGetReplyByte(xx, 0);
    uchar val2 = rcxGetReplyByte(xx, 1);

    result = ((val2 << 8) + val1);
    return true;

  }	
  return false;
} /* rcxGetValue */

/*------------------------------------ rcxNotify ---*/
uint32 rcxNotify
  (GHNOTIFYCODE notifyCode,
   PBKRESULT    resultCode,
   GHSTACK      ghostStack,
   GHQUEUE      queue,
   GHCOMMAND    command,
   Pvoid        context)
{
#if __powerc
 #pragma unused(ghostStack,queue)
#else /* not __powerc */
 #pragma unused(notifyCode, resultCode, ghostStack, queue, command, context)
#endif /* not __powerc */
  RcxControlPtr xx = static_cast<RcxControlPtr>(context);
  uint32        retCode = 1;

#if __powerc
  LOG_POST_3(OUTPUT_PREFIX "notify triggered notifyCode=%d, resultCode=0x%lX",
              notifyCode, resultCode)
  if (notifyCode == NotifyCommand)
  {
    uchar     cmd[256], reply[256];
    uint32    cmdLen, replyLen;
    PBKRESULT ghostResult;
    bool      okSoFar = true;

    ghostResult = GhGetCommandDataLen(command, &cmdLen);
    if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandDataLen failed --> 0x%lX", long(ghostResult))
      okSoFar = false;
    }
    if (okSoFar)
    {
      ghostResult = GhGetCommandData(command, cmd, sizeof(cmd)); 
      if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandData failed --> 0x%lX", long(ghostResult))
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      ghostResult = GhGetCommandReplyLen(command, &replyLen); 
      if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandReplyLen failed --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      if (replyLen > 0) 
      { 
        ghostResult = GhGetCommandReply(command, reply, sizeof(reply)); 
        if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
        {
          LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandReply failed --> 0x%lX",
                      long(ghostResult))
          okSoFar = false;
        }
      }
    }
    if (okSoFar)
    {
      if (cmdLen > 0)
        LOG_POST_2("Command code: 0x%lX", long(*cmd))
      if (PBK_IS_SUCCESS(resultCode) || PBK_IS_INFO(resultCode)) 
      { 
        if (replyLen > 0)
          LOG_POST_3("Succeeded Reply: 0x%lX (length=%ld)", long(*reply), long(replyLen))
        else
          LOG_POST_1("Succeeded")
      } 
      else 
      { 
        LOG_ERROR_2(OUTPUT_PREFIX "Failed --> 0x%lX", resultCode)
        // Cause the next commands to be aborted
        retCode = 0;
      }
    }
    else
      retCode = 0;
  }
  return retCode;
#else /* not __powerc */
  return 0;
#endif /* not __powerc */
} /* rcxNotify */

/*------------------------------------ rcxSendCommand ---*/
bool rcxSendCommand
  (RcxControlPtr	xx,
   const uchar *	sendData,
   const uint32   sendLength,
   const uint32   expected,
   const bool     doRetry)
{
#if (! __powerc)
 #pragma unused(sendData, sendLength, expected, doRetry)
#endif /* not __powerc */
  bool okSoFar = true, actualRetry = doRetry;

  if (xx)
  {
#if __powerc
    PBKRESULT ghostResult = PBKOK;
    GHQUEUE   aQueue = NULL_PTR;

    if (! expected)
      actualRetry = false;
    if (xx->fVerbose)
      LOG_POST_5("0x%lx %ld %ld %d", static_cast<ulong>(*sendData), sendLength,
                  expected, actualRetry)//!!
    ghostResult = GhCreateCommandQueue(&aQueue);
    if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "unable to create command queue --> 0x%lX",
                  long(ghostResult))
      okSoFar = false;
    }
    if (okSoFar)
    {
      ghostResult = GhSetQueueContext(aQueue, xx);
      if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to set queue context --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      ghostResult = GhAppendCommand(aQueue, const_cast<Puchar>(sendData), sendLength, expected);
      if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to add to command queue --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }			
    }
    if (okSoFar)
    {
      if (actualRetry)
        ghostResult = GhExecute(xx->fGhostStack, aQueue);
      else
      {
        GhSetRetries(xx->fGhostStack, 0, 0);
        ghostResult = GhExecute(xx->fGhostStack, aQueue);
        GhSetRetries(xx->fGhostStack, xx->fExecuteRetries, xx->fDownloadRetries);
      }
      if (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult))
      {
 #if (! defined(USE_NOWAIT))
        if (expected)
        {
          GHCOMMAND aCommand;

          ghostResult = GhGetFirstCommand(aQueue, &aCommand);
          if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to get first command in queue --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
          if (okSoFar)
          {
            ghostResult = GhGetCommandReplyLen(aCommand, &xx->fReplyLength);
            if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
            {
              LOG_ERROR_2(OUTPUT_PREFIX "unable to get reply length --> 0x%lX",
                          long(ghostResult))
              okSoFar = false;
            }
          }
          if (okSoFar)
          {
            if (xx->fReplyLength > MAX_REPLY_BUFFER)
              xx->fReplyLength = MAX_REPLY_BUFFER;
            ghostResult = GhGetCommandReply(aCommand, xx->fReplyBuffer, xx->fReplyLength);
            if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
            {
              LOG_ERROR_2(OUTPUT_PREFIX "unable to get reply --> 0x%lX",
                          long(ghostResult))
              okSoFar = false;
            }
          }	
        }
 #endif /* not USE_NOWAIT */
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "problem executing commands --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }
    }
 #if (! defined(USE_NOWAIT))
    if (aQueue)
      GhDestroyCommandQueue(aQueue);
 #endif /* not USE_NOWAIT */
#endif /* __powerc */
  }
  return okSoFar;
} /* rcxSendCommand */

/*------------------------------------ rcxSynchronize ---*/
bool rcxSynchronize
  (RcxControlPtr xx)
{
  bool didIt = false;

  if (xx)
  {
#if __powerc
    if (xx->fSynchronized)
      didIt = true;
    else
    {
      static uchar pingCommand[] = { RCX_PING_CMD };

      // Start with a ping
      didIt = rcxSendCommand(xx, pingCommand, sizeof(pingCommand), RCX_PING_REPLY, true);
      if (! didIt)
        didIt = rcxSendCommand(xx, pingCommand, sizeof(pingCommand), RCX_PING_REPLY,
                                true);
      xx->fSynchronized = didIt;
    }
#endif /* __powerc */
  }
  return didIt;
} /* rcxSynchronize */

StandardInfoRoutine(RcxControlPtr)