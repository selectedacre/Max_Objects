/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       caseShift.c                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the caseShift module.     */
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
/*  Created:    2000/10/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "caseShift.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid caseShiftCreate
  (PSymbol direction);

Pvoid caseShiftFree
  (CaseShiftPtr xx);


/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(caseShiftCreate),
        reinterpret_cast<method>(caseShiftFree), short(sizeof(CaseShiftData)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_NOTHING);
  addfloat(reinterpret_cast<method>(cmd_Float));
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Messages", OUR_NAME);
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gDownSymbol = gensym("down");
  gEmptySymbol = gensym("");
  gUpSymbol = gensym("up");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ caseShiftCreate ---*/
Pvoid caseShiftCreate
  (PSymbol direction)
{
  CaseShiftPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<CaseShiftPtr>(newobject(gClass));
  if (xx)
  {
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fDown = false;
    xx->fPreviousKind = A_NOTHING;
    xx->fPreviousList = NULL_PTR;
    if (xx->fResultOut)
    {
      if (direction == gDownSymbol)
        xx->fDown = true;
      else if ((direction != gUpSymbol) && (direction != gEmptySymbol))
        LOG_ERROR_2(OUTPUT_PREFIX "unknown direction (%s)", direction->s_name)
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* caseShiftCreate */

/*------------------------------------ caseShiftFree ---*/
Pvoid caseShiftFree
  (CaseShiftPtr xx)
{
  EnterCallback();
  if (xx)
    clearPrevious(xx);
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* caseShiftFree */

/*------------------------------------ clearPrevious ---*/
void clearPrevious
  (CaseShiftPtr xx)
{
  FREEBYTES(xx->fPreviousList, xx->fPreviousLength)
} /* clearPrevious */

/*------------------------------------ shiftAString ---*/
void shiftAString
  (CaseShiftPtr xx,
   Pchar        newWord,
   Qchar        oldWord)
{
  Pchar outWalker = newWord;
  Qchar	inWalker = oldWord;
  int   aChar;

  if (xx->fDown)
  {
    while (*inWalker)
    {
      aChar = *inWalker++;
      if ((aChar >= 'A') && (aChar <= 'Z'))
        aChar -= ('A' - 'a');
      *outWalker++ = char(aChar);
    }
  }
  else
  {
    while (*inWalker)
    {
      aChar = *inWalker++;
      if ((aChar >= 'a') && (aChar <= 'z'))
        aChar += ('A' - 'a');
      *outWalker++ = char(aChar);
    }
  }
  *outWalker = 0;
} /* shiftAString */

StandardInfoRoutine(CaseShiftPtr)