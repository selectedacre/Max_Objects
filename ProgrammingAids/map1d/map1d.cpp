/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1d.cpp                                                               */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the One-Dimensional       */
/*              mapping module.                                                         */
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
/*  Created:    2000/11/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "map1d.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ map1dCreate ---*/
static void *
map1dCreate(t_symbol * initialFile)
{
    Map1dData * xx = static_cast<Map1dData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fVerbose = false;
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        xx->fFirstRange = xx->fLastRange = xx->fPreviousResult = NULL;
        setFOI2Integer(xx->fPreviousInput, 0);
        xx->fRangeCount = 0;
        xx->fBuffer = static_cast<t_binbuf *>(binbuf_new());
        if (! (xx->fResultOut && xx->fBuffer))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
        else if (initialFile == gEmptySymbol)
        {
            LOG_POST_1(xx, OUTPUT_PREFIX "no initial map file")
        }
        else
        {
            LOG_POST_2(xx, OUTPUT_PREFIX "initial file: %s", initialFile->s_name)
            map1dLoadRangeList(xx, initialFile);
        }
    }
    return xx;
} // map1dCreate

/*------------------------------------ map1dFree ---*/
static void
map1dFree(Map1dData * xx)
{
    if (xx)
    {
        if (xx->fBuffer)
        {
            freeobject(reinterpret_cast<t_object *>(xx->fBuffer));
            xx->fBuffer = NULL;
        }
        map1dClearRangeList(xx);
    }
} // map1dFree

/*------------------------------------ main ---*/
int C74_EXPORT
main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(map1dCreate),
                               reinterpret_cast<method>(map1dFree),
                               static_cast<long>(sizeof(Map1dData)), reinterpret_cast<method>(NULL),
                               A_DEFSYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Add), "add", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_After), "after", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Before), "before", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Clear), "clear", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Count), "count", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Delete), "delete", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Dump), "dump", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Get), "get", A_LONG, A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Load), "load", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Replace), "replace", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Set), "set", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Show), "show", A_LONG, 0);
#if defined(BE_VERBOSE)
        class_addmethod(temp, reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, 0);
#endif /* BE_VERBOSE */
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gAsteriskSymbol = gensym("*");
    gCloseRoundSymbol = gensym(")");
    gCloseSquareSymbol = gensym("]");
    gCountSymbol = gensym("count");
    gDollarSymbol = gensym("$");
    gDollarXSymbol = gensym("$x");
    gDollarYSymbol = gensym("$y");
    gDollarZSymbol = gensym("$z");
    gDoubleDollarSymbol = gensym("$$");
    gDoubleDollarXSymbol = gensym("$$x");
    gDoubleDollarYSymbol = gensym("$$y");
    gDoubleDollarZSymbol = gensym("$$z");
    gEmptySymbol = gensym("");
    gLowerSymbol = gensym("lower");
    gNegInfSymbol1 = gensym("-inf");
    gNegInfSymbol2 = gensym("-∞");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gOpenRoundSymbol = gensym("(");
    gOpenSquareSymbol = gensym("[");
    gPosInfSymbol1 = gensym("inf");
    gPosInfSymbol2 = gensym("+inf");
    gPosInfSymbol3 = gensym("∞");
    gPosInfSymbol4 = gensym("+∞");
    gRangeSymbol = gensym("range");
    gResultSymbol = gensym("result");
    gUpperSymbol = gensym("upper");
    gValueSymbol = gensym("value");
    reportVersion();
    return 0;
} // main

StandardAnythingRoutine(Map1dData)
