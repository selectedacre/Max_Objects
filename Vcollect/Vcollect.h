/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vcollect.h                                                              */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2002/08/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(VCOLLECT_H_))
# define VCOLLECT_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_NAME      "Vcollect"
// # define OUR_RES_NUMB  17192
# define OUTPUT_PREFIX "Vcollect: "

# define CHUNK_SIZE 30

struct CollectChunk
{
    CollectChunk * fNext;
    t_atom         fAtoms[CHUNK_SIZE];
}; // CollectChunk

struct VcollectData
{
    t_object       fObject;
    t_outlet *     fLengthOut;
    t_outlet *     fResultOut;
    void *         fProxy;
    CollectChunk * fFirstChunk;
    CollectChunk * fFreeChunks;
    CollectChunk * fLastChunk;
    long           fInletNumber;
    short          fAtomsInLastChunk;
    short          fTotalAtoms;
    bool           fCollecting;
}; // VcollectData

void cmd_Clear(VcollectData * xx);

void cmd_Count(VcollectData * xx);

void cmd_Start(VcollectData * xx);

void cmd_Stop(VcollectData * xx);

void collectAddAtoms(VcollectData * xx,
                     t_atom *       atoms,
                     const short    count);

StandardRoutineDeclarations(VcollectData *);

mextern(t_symbol *)  gClearSymbol; /* Pointer to unique symbol for 'clear' */
mextern(t_symbol *)  gCountSymbol; /* Pointer to unique symbol for 'count' */
mextern(t_symbol *)  gStartSymbol; /* Pointer to unique symbol for 'start' */
mextern(t_symbol *)  gStopSymbol;  /* Pointer to unique symbol for 'stop' */

#endif /* not VCOLLECT_H_ */