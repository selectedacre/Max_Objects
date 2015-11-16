/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vunspell.cpp                                                            */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vunspell module.      */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/06/17                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vunspell.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
void * VunspellCreate(long separator1,
                      long separator2,
                      long separator3,
                      long separator4,
                      long separator5);

void VunspellFree(VObjectData * xx);

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(VunspellCreate),
                               reinterpret_cast<method>(VunspellFree), sizeof(VObjectData),
                               reinterpret_cast<method>(0L), A_LONG, A_DEFLONG, A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    reportVersion(OUR_NAME);
    return 0;
} // main
/*------------------------------------ VunspellCreate ---*/
void * VunspellCreate(long separator1,
                      long separator2,
                      long separator3,
                      long separator4,
                      long separator5)
{
    VObjectData * xx = static_cast<VObjectData *>(object_alloc(gClass));

    if (xx)
    {
        if (setupSeparators(xx, separator1, separator2, separator3, separator4, separator5))
        {
            xx->fPreviousList = NULL_PTR;
            xx->fPreviousLength = 0;
            xx->fChunkList = xx->fLastChunk = NULL_PTR;
            xx->fAssembled = false;
            xx->fBangOut = static_cast<t_outlet *>(bangout(xx));
            xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL_PTR));
            if (! (xx->fBangOut && xx->fResultOut))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
                freeobject(reinterpret_cast<t_object *>(xx));
                xx = NULL_PTR;
            }
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "bad separator list")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL_PTR;
        }
    }
    return xx;
} // VunspellCreate
/*------------------------------------ VunspellFree ---*/
void VunspellFree(VObjectData * xx)
{
    if (xx)
    {
        clearPrevious(xx);
    }
} // VunspellFree
/*------------------------------------ processInput ---*/
void processInput(VObjectData * xx,
                  const long    value)
{
    ChunkData * workChunk;

    if (xx->fAssembled)
    {
        clearPrevious(xx);
        xx->fAssembled = false;
    }
    if (isSeparator(xx, value))
    {
        if (xx->fPreviousLength)
        {
            t_atombuf * accumulator = static_cast<t_atombuf *>(atombuf_new(0, 0));
            t_handle    rawData = sysmem_newhandle(xx->fPreviousLength);

            if (accumulator && rawData)
            {
                short  offset = 0;
                char * temp = *rawData;

                for (ChunkData * iWalker = xx->fChunkList; iWalker; iWalker = iWalker->fNext)
                {
                    for (short index = 0; index < iWalker->fLastEntry; ++index, ++offset)
                    {
                        *(temp + offset) = static_cast<char>(iWalker->fData[index]);
                    }
                }
                atombuf_text(&accumulator, rawData, xx->fPreviousLength);
                if (accumulator->a_argc)
                {
                    genericListOutput(xx->fResultOut, static_cast<short>(accumulator->a_argc), accumulator->a_argv);
                }
            }
            if (rawData)
            {
                sysmem_freehandle(rawData);
            }
            if (accumulator)
            {
                atombuf_free(accumulator);
            }
            xx->fPreviousLength = 0;
            xx->fAssembled = true;
        }
        else
        {
            outlet_bang(xx->fBangOut);
        }
    }
    else if (xx->fLastChunk)
    {
        workChunk = xx->fLastChunk;
        short index = workChunk->fLastEntry;

        if (CHUNK_SIZE == index)
        {
            ChunkData * newChunk = GETBYTES(1, ChunkData);

            if (newChunk)
            {
                workChunk->fNext = xx->fLastChunk = newChunk;
                newChunk->fNext = NULL_PTR;
                newChunk->fLastEntry = 1;
                newChunk->fData[0] = value;
            }
        }
        else
        {
            workChunk->fData[index] = value;
            workChunk->fLastEntry = static_cast<short>(index + 1);
        }
        ++xx->fPreviousLength;
    }
    else
    {
        workChunk = GETBYTES(1, ChunkData);
        if (workChunk)
        {
            xx->fChunkList = xx->fLastChunk = workChunk;
            workChunk->fNext = NULL_PTR;
            workChunk->fLastEntry = xx->fPreviousLength = 1;
            workChunk->fData[0] = value;
        }
    }
} // processInput

StandardAnythingRoutine(VObjectData *)