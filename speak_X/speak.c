/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       speak.c                                                                 */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the speak module.         */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2001 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2001/04/03                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "speak.h"
#include "speakBuffers.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
bool
isSpeechManagerAvailable(void);

Pvoid
speakCreate(void);

Pvoid
speakFree(SpeakPtr xx);

Pvoid
speakProcessClock(SpeakPtr xx);

Pvoid
speakProcessQueue(SpeakPtr xx);

/*------------------------------------ main ---*/
void
main(Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  if (isSpeechManagerAvailable())
  {
    /* Allocate class memory and set up class. */
    setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(speakCreate),
          reinterpret_cast<method>(speakFree), short(sizeof(SpeakData)),
          reinterpret_cast<method>(0L), A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Continue), "continue", A_NOTHING);
    addfloat(reinterpret_cast<method>(cmd_Float));
    addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
    addint(reinterpret_cast<method>(cmd_Int));
    addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Pause), "pause", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Pitch), "pitch", A_FLOAT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Pitchq), "pitch?", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Rate), "rate", A_FLOAT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Rateq), "rate?", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Say), "say", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Spell), "spell", A_DEFSYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Stop), "stop", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Voice), "voice", A_LONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Voiceq), "voice?", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_VoiceMax), "voicemax?", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Volume), "volume", A_FLOAT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Volumeq), "volume?", A_NOTHING);

    /* Add object class to New Object List */
    finder_addclass("Devices", OUR_NAME);
    /* Make sure that our string list resource is present */
    rescopy('STR#', OUR_RES_NUMB);
    loadAboutBoxData();
    gCommaNameSymbol = gensym("comma");
    gDollarNameSymbol = gensym("dollar");
    gDollarSymbol = gensym("$");
    gEmptySymbol = gensym("");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gPitchSymbol = gensym("pitch");
    gRateSymbol = gensym("rate");
    gSemicolonNameSymbol = gensym("semicolon");
    gVoiceSymbol = gensym("voice");
    gVoiceMaxSymbol = gensym("voicemax");
    gVolumeSymbol = gensym("volume");
    gVersionNumber = reportVersion(OUR_NAME);
    loadOtherSegments();
  }
  ExitCodeResource();
} /* main */

/*------------------------------------ speakCreate ---*/
Pvoid
speakCreate(void)
{
  SpeakPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<SpeakPtr>(newobject(gClass));
  if (xx)
  {
    xx->fSpeaker = NULL_PTR;
    xx->fVoiceNumber = INITIAL_VOICE;
    xx->fDoneQueue = NULL_PTR;
    xx->fCompletionRoutine = NULL_PTR;
    xx->fBuffer = NULL_HDL;
    if ((GetIndVoice(xx->fVoiceNumber, &xx->fVoice) == noErr) &&
          (NewSpeechChannel(&xx->fVoice, &xx->fSpeaker) == noErr))
    {
      xx->fActive = xx->fPaused = xx->fSpelling = false;
      xx->fDoneQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(speakProcessQueue)));
      xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
      xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
      xx->fPausedOut = static_cast<POutlet>(bangout(xx));
      xx->fDoneOut = static_cast<POutlet>(bangout(xx));
      xx->fBuffer = reinterpret_cast<DataBufferHdl>(newhandle(BUFF_MEMORY_TO_ALLOC));
      if (xx->fBuffer)
      {
        HLock(reinterpret_cast<Handle>(xx->fBuffer));
        clearDataBuffer(*xx->fBuffer);
      }
      if (xx->fDoneQueue && xx->fErrorBangOut && xx->fDoneOut && xx->fPausedOut &&
            xx->fResultOut && xx->fBuffer)
      {
        xx->fCompletionRoutine = NewSpeechDoneUPP(speechComplete);
        if ((SetSpeechInfo(xx->fSpeaker, soRefCon, xx) != noErr) ||
              (SetSpeechInfo(xx->fSpeaker, soSpeechDoneCallBack,
                              xx->fCompletionRoutine) != noErr))
        {
          LOG_ERROR_1(OUTPUT_PREFIX "unable to set speech parameters")
          freeobject(reinterpret_cast<PObject>(xx));
          xx = NULL_PTR;
        }
      }
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to create ports for device")
        freeobject(reinterpret_cast<PObject>(xx));
        xx = NULL_PTR;
      }
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create speech channel for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* speakCreate */

/*------------------------------------ speakFree ---*/
Pvoid
speakFree(SpeakPtr xx)
{
  EnterCallback();
  if (xx)
  {
    if (xx->fDoneQueue)
    {
      qelem_unset(xx->fDoneQueue);
      qelem_free(xx->fDoneQueue);
      xx->fDoneQueue = NULL_PTR;
    }
    if (xx->fSpeaker)
    {
      if (SpeechBusy())
        StopSpeechAt(xx->fSpeaker, kImmediate);
      SetSpeechInfo(xx->fSpeaker, soSpeechDoneCallBack, NULL_PTR);
      DisposeSpeechChannel(xx->fSpeaker);
      xx->fSpeaker = NULL_PTR;
    }
    if (xx->fBuffer)
    {
      HUnlock(reinterpret_cast<Handle>(xx->fBuffer));
      disposhandle(reinterpret_cast<Handle>(xx->fBuffer));
      xx->fBuffer = NULL_HDL;
    }
    if (xx->fCompletionRoutine)
    {
      DisposeSpeechDoneUPP(xx->fCompletionRoutine);
      xx->fCompletionRoutine = NULL_PTR;
    }
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* speakFree */

/*------------------------------------ isSpeechManagerAvailable ---*/
bool
isSpeechManagerAvailable(void)
{
  UInt32   speechAttributes;
  OSStatus err = Gestalt(gestaltSpeechAttr, reinterpret_cast<Plong>(&speechAttributes));
  bool     okSoFar = true;

  if ((err != kOTNoError) || (! (speechAttributes & (1 << gestaltSpeechMgrPresent))))
  {
    LOG_ERROR_2(OUR_NAME ": Speech Manager not available (%ld)", err)
    okSoFar = false;
  }
  return okSoFar;
} /* isSpeechManagerAvailable */

/*------------------------------------ speakProcessQueue ---*/
Pvoid
speakProcessQueue(SpeakPtr xx)
{
  EnterCallback();
  if (xx)
  {
    short prevLock = lockout_set(1);

    xx->fActive = false;
    outlet_bang(xx->fDoneOut);
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* speakProcessQueue */

/*------------------------------------ speechComplete ---*/
pascal void
speechComplete(SpeechChannel    chan,
               long                        refCon)
{
#pragma unused(chan)
  SpeakPtr xx = reinterpret_cast<SpeakPtr>(refCon);

  EnterCallback();
  if (xx)
    qelem_set(xx->fDoneQueue);
  ExitCodeResource();
} /* speechComplete */

/*------------------------------------ transmitBuffer ---*/
void
transmitBuffer(SpeakPtr xx)
{
  if (xx && xx->fSpeaker)
  {
    DataBufferPtr aBuffer = *xx->fBuffer;

    if (aBuffer && aBuffer->fNumBytesInUse)
    {
            xx->fActive = true;
            SpeakText(xx->fSpeaker, &aBuffer->fElements, aBuffer->fNumBytesInUse);
    }
  }
} /* transmitBuffer */

StandardInfoRoutine(SpeakPtr)
