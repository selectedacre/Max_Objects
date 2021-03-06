/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       queue.h                                                                 */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2001/04/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(QUEUE_H_))
# define QUEUE_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_DESC    "First-in-first-out queue for arbitrary data"
# define OUR_NAME    "queue"
# define OUR_VERSION "1.0.2"
# define OUR_YEAR    "2001"

struct QueueEntry
{
    QueueEntry * fNext;
    t_atom *     fOutput;
    long         fOutputCount;
}; /* QueueEntry */

struct QueueData
{
    t_object     fObject;
    t_outlet *   fResultOut;
    t_outlet *   fDepthOut;
    t_outlet *   fErrorBangOut;
    QueueEntry * fFirstInQueue;
    QueueEntry * fLastInQueue;
    long         fDepth;
    long         fMaxDepth;
    bool         fVerbose;
}; // QueueData

# define ADD_HEADER(type) \
    void\
    cmd_Add(type *     xx,\
            t_symbol * message,\
            const long argc,\
            t_atom *   argv)

# define DEPTH_HEADER(type) \
    void\
    cmd_Depth(type * xx)

# define FETCH_HEADER(type) \
    void\
    cmd_Fetch(type * xx)

# define PULL_HEADER(type) \
    void\
    cmd_Pull(type * xx)

# define SETDEPTH_HEADER(type) \
    void\
    cmd_SetDepth(type *     xx,\
                 const long number)

ADD_HEADER(QueueData);
CLEAR_HEADER(QueueData);
DEPTH_HEADER(QueueData);
FETCH_HEADER(QueueData);
PULL_HEADER(QueueData);
SETDEPTH_HEADER(QueueData);
TRACE_HEADER(QueueData);

void
queueClear(QueueData * xx);

StandardRoutineDeclarations(QueueData);

mextern(t_symbol *) gEmptySymbol; /* Pointer to unique symbol for '' */
mextern(t_symbol *) gOffSymbol;   /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;    /* Pointer to unique symbol for 'on' */

#endif /* not QUEUE_H_ */
