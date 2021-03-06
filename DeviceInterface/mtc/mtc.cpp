/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtc.cpp                                                                 */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the mtc module.           */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1999 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1999/12/05                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "mtc.h"
#include "reportVersion.h"

/*------------------------------------ mtcChangeState ---*/
static void
mtcChangeState(MtcData *      xx,
               const MtcState newState)
{
#if defined(REPORT_STATE_CHANGES)
    const char * oldString;
    static char  numBuffer[NUM_BUFF_SIZE];
#endif /* REPORT_STATE_CHANGES */
    
    if (xx)
    {
#if defined(REPORT_STATE_CHANGES)
        if (xx->fVerbose)
        {
            switch (xx->fState)
            {
                case kMtcStateAwaitingEndOfCommand:
                    oldString = "kMtcStateAwaitingEndOfCommand";
                    break;
                    
                case kMtcStateAwaitingEndOfComp:
                    oldString = "kMtcStateAwaitingEndOfComp";
                    break;
                    
                case kMtcStateAwaitingEndOfData:
                    oldString = "kMtcStateAwaitingEndOfData";
                    break;
                    
                case kMtcStateAwaitingEndOfDesc:
                    oldString = "kMtcStateAwaitingEndOfDesc";
                    break;
                    
                case kMtcStateAwaitingNumBytes:
                    oldString = "kMtcStateAwaitingNumBytes";
                    break;
                    
                case kMtcStateAwaitingNumPackets:
                    oldString = "kMtcStateAwaitingNumPackets";
                    break;
                    
                case kMtcStateAwaitingSampleNumber:
                    oldString = "kMtcStateAwaitingSampleNumber";
                    break;
                    
                case kMtcStateIdle:
                    oldString = "kMtcStateIdle";
                    break;
                    
                case kMtcStateLookingForEnd:
                    oldString = "kMtcStateLookingForEnd";
                    break;
                    
                case kMtcStateSkipToAck:
                    oldString = "kMtcStateSkipToAck";
                    break;
                    
                case kMtcStateSkipToEnd:
                    oldString = "kMtcStateSkipToEnd";
                    break;
                    
                default:
                    snprintf(numBuffer, sizeof(numBuffer), "unrecognized state %ld",
                             static_cast<long>(xx->fState));
                    oldString = numBuffer;
                    break;
                    
            }
        }
#endif /* REPORT_STATE_CHANGES */
        xx->fState = newState;
#if defined(REPORT_STATE_CHANGES)
        if (xx->fVerbose)
        {
            switch (xx->fState)
            {
                case kMtcStateAwaitingEndOfComp:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfComp", oldString)
                    break;
                    
                case kMtcStateAwaitingEndOfData:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfData", oldString)
                    break;
                    
                case kMtcStateAwaitingEndOfDesc:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfDesc", oldString)
                    break;
                    
                case kMtcStateAwaitingEndOfCommand:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfCommand", oldString)
                    break;
                    
                case kMtcStateAwaitingNumBytes:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingNumBytes", oldString)
                    break;
                    
                case kMtcStateAwaitingNumPackets:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingNumPackets", oldString)
                    break;
                    
                case kMtcStateAwaitingSampleNumber:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingSampleNumber", oldString)
                    break;
                    
                case kMtcStateIdle:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateIdle", oldString)
                    break;
                    
                case kMtcStateLookingForEnd:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateLookingForEnd", oldString)
                    break;
                    
                case kMtcStateSkipToAck:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateSkipToAck", oldString)
                    break;
                    
                case kMtcStateSkipToEnd:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateSkipToEnd", oldString);
                    break;
                    
                default:
                    LOG_POST_3(xx, OUTPUT_PREFIX "%s -> unrecognized state (%ld)", oldString,
                               xx->fState)
                    break;
                    
            }
        }
#endif /* REPORT_STATE_CHANGES */
    }
} // mtcChangeState

/*------------------------------------ mtcProcessClock ---*/
static void
mtcProcessClock(MtcData * xx)
{
    if (xx && (! xx->fStopping))
    {
        qelem_set(xx->fPollQueue);
    }
} // mtcProcessClock

/*------------------------------------ mtcProcessQueue ---*/
static void
mtcProcessQueue(MtcData * xx)
{
    if (xx && (! xx->fStopping))
    {
        short prevLock = lockout_set(1);
        
        outlet_bang(xx->fSampleBangOut);
        clock_delay(xx->fPollClock, xx->fPollRate);
        lockout_set(prevLock);
#if USE_EVNUM
        evnum_incr();
#endif /* USE_EVNUM */
    }
} // mtcProcessQueue

/*------------------------------------ mtcCreate ---*/
static void *
mtcCreate(t_symbol *  ss,
          const short argc,
          t_atom *    argv)
{
#pragma unused(ss)
    MtcData * xx = static_cast<MtcData *>(object_alloc(gClass));
    
    if (xx)
    {
        bool okSoFar = true;
        
        xx->fProxy = NULL;
        xx->fPollClock = NULL;
        xx->fPollQueue = NULL;
        xx->fMapFileName = NULL;
        xx->fNormalFileName = NULL;
        xx->fSpots = NULL;
        xx->fRawRow = NULL;
        xx->fDataRecovery = NULL;
        xx->fDataBuffer = NULL;
        xx->fSentinelTaxel = NULL;
        xx->fTaxelMapHandle = xx->fTaxelMatrix = NULL;
        xx->fChunkPulseSent = xx->fStarted = xx->fStopped = xx->fNormalizing = xx->fModeRaw = false;
        xx->fDataCompressed = xx->fIsPacketHeader = xx->fUseCompression = xx->fStopping = false;
        xx->fDescriptorLength = xx->fNumTaxels = xx->fExpectedPackets = 0;
        xx->fThreshold = MTC_DEFAULT_THRESHOLD;
        xx->fLastSampleNumber = xx->fSampleNumber = xx->fSampleNumberBase = 0;
        xx->fSortOrder = kMtcOrderUnordered;
        if (3 > argc)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "missing argments")
            okSoFar = false;
        }
        else
        {
            t_atom_long numSpots = DEFAULT_SPOTS;
            
            /* Check the number of spots */
            if (A_LONG == argv[0].a_type)
            {
                numSpots = argv[0].a_w.w_long;
            }
            else if (A_FLOAT == argv[0].a_type)
            {
                numSpots = TO_INT(argv[0].a_w.w_float);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "number of spots is not numeric")
            }
            if (okSoFar)
            {
                if ((0 >= numSpots) || (MAX_SPOTS < numSpots))
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid number of spots (" LONG_FORMAT
                                ") for device", numSpots)
                    numSpots = DEFAULT_SPOTS;
                }
                xx->fNumSpots = numSpots;
            }
            if (okSoFar)
            {
                /* Check the mapping file */
                if (A_SYM == argv[1].a_type)
                {
                    xx->fMapFileName = GET_BYTES(strlen(argv[1].a_w.w_sym->s_name) + 1, char);
                    if (xx->fMapFileName)
                    {
                        strcpy(xx->fMapFileName, argv[1].a_w.w_sym->s_name);
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "mapping file name is invalid")
                    okSoFar = false;
                }
            }
            if (okSoFar)
            {
                /* Check the normalization file */
                if (A_SYM == argv[2].a_type)
                {
                    xx->fNormalFileName = GET_BYTES(strlen(argv[2].a_w.w_sym->s_name) + 1, char);
                    if (xx->fNormalFileName)
                    {
                        strcpy(xx->fNormalFileName, argv[2].a_w.w_sym->s_name);
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "normalization file name is invalid")
                    okSoFar = false;
                }
            }
        }
        if (okSoFar && (3 < argc))
        {
            /* Check the mode */
            if (A_SYM == argv[3].a_type)
            {
                okSoFar = mtcSetMode(xx, argv[3].a_w.w_sym);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "mode is invalid")
                okSoFar = false;
            }
        }
        if (okSoFar && (4 < argc))
        {
            /* Check the sort order */
            if (A_SYM == argv[4].a_type)
            {
                okSoFar = mtcSetOrder(xx, argv[4].a_w.w_sym);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "sort order is invalid")
                okSoFar = false;
            }
        }
        if (okSoFar && (5 < argc))
        {
            /* Check the polling rate */
            t_atom_long pollRate = SER_SAMPLE_RATE;
            
            if (A_LONG == argv[5].a_type)
            {
                pollRate = argv[5].a_w.w_long;
            }
            else if (A_FLOAT == argv[5].a_type)
            {
                pollRate = TO_INT(argv[5].a_w.w_float);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "polling rate is not numeric")
                okSoFar = false;
            }
            if (okSoFar)
            {
                if ((0 >= pollRate) || (MAX_POLL_RATE < pollRate))
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid polling rate (" LONG_FORMAT
                                ") for device", pollRate)
                    okSoFar = false;
                }
                else
                {
                    xx->fPollRate = pollRate;
                }
            }
        }
        if (okSoFar)
        {
            xx->fSpots = GET_BYTES(xx->fNumSpots, MtcSpot);
            xx->fDataRecovery = GET_BYTES(256, short);
            xx->fDataBuffer = GET_BYTES(MAX_MESSAGE_BYTES, unsigned char);
            /* Set up our connections and private data */
            xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
            xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
            xx->fChunkSendOut = static_cast<t_outlet *>(bangout(xx));
            xx->fDataSendOut = static_cast<t_outlet *>(outlet_new(xx, 0L));  /* list, int */
            xx->fSampleBangOut = static_cast<t_outlet *>(bangout(xx));
            xx->fCommandComplete = static_cast<t_outlet *>(bangout(xx));
            xx->fDataStartStopOut = static_cast<t_outlet *>(intout(xx));
            xx->fDataOut = static_cast<t_outlet *>(outlet_new(xx, 0L));   /* normally just a list */
            xx->fPollClock = MAKE_CLOCK(xx, mtcProcessClock);
            xx->fPollQueue = MAKE_QELEM(xx, mtcProcessQueue);
            if (xx->fProxy && xx->fErrorBangOut && xx->fDataSendOut && xx->fChunkSendOut &&
                xx->fSampleBangOut && xx->fDataOut && xx->fDataStartStopOut &&
                xx->fCommandComplete && xx->fPollClock && xx->fPollQueue && xx->fMapFileName &&
                xx->fNormalFileName && xx->fSpots && xx->fDataRecovery)
            {
                if (mtcReadMapFile(xx))
                {
                    mtcSetupIndices(xx);
                    mtcResetNormalization(xx);
                    mtcReadNormalizationFile(xx);
                    xx->fRawRow = GET_BYTES(xx->fMaxCol * xx->fMaxRow, t_atom);
                    if (xx->fRawRow)
                    {
                        clock_delay(xx->fPollClock, xx->fPollRate);
#if defined(BE_VERBOSE)
                        xx->fVerbose = false;
#endif /* BE_VERBOSE */
                        mtcChangeState(xx, kMtcStateIdle);
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create raw buffer for device")
                        okSoFar = false;
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to open map file for device")
                    okSoFar = false;
                }
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or clock for device")
                okSoFar = false;
            }
        }
        if (! okSoFar)
        {
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // mtcCreate

/*------------------------------------ mtcFree ---*/
void
mtcFree(MtcData * xx)
{
    if (xx)
    {
        xx->fStopping = true;
        if (xx->fPollClock)
        {
            clock_unset(xx->fPollClock);
            clock_free(reinterpret_cast<t_object *>(xx->fPollClock));
            xx->fPollClock = NULL;
        }
        if (xx->fPollQueue)
        {
            qelem_unset(xx->fPollQueue);
            qelem_free(xx->fPollQueue);
            xx->fPollQueue = NULL;
        }
        if (xx->fProxy)
        {
            freeobject(reinterpret_cast<t_object *>(xx->fProxy));
            xx->fProxy = NULL;
        }
        FREE_BYTES(xx->fMapFileName);
        FREE_BYTES(xx->fNormalFileName);
        FREE_BYTES(xx->fSpots);
        FREE_BYTES(xx->fDataRecovery);
        FREE_BYTES(xx->fDataBuffer);
        FREE_BYTES(xx->fRawRow);
        FREE_BYTES(xx->fTaxelMatrix);
        if (xx->fTaxelMapHandle)
        {
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fTaxelMapHandle), 0);
            sysmem_freehandle(reinterpret_cast<t_handle>(xx->fTaxelMapHandle));
            xx->fTaxelMapHandle = NULL;
        }
    }
} // mtcFree

/*------------------------------------ main ---*/
int C74_EXPORT
main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(mtcCreate),
                               reinterpret_cast<method>(mtcFree),
                               static_cast<long>(sizeof(MtcData)), reinterpret_cast<method>(NULL),
                               A_GIMME, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Describe), "describe", 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Order), "order", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Ping), "ping", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Start), "start", A_DEFSYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Stop), "stop", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Threshold), "threshold", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Train), "train", A_DEFSYM, 0);
#if defined(BE_VERBOSE)
        class_addmethod(temp, reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, 0);
#endif /* BE_VERBOSE */
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gCompressedSymbol = gensym("compressed");
    gCookedSymbol = gensym("cooked");
    gEmptySymbol = gensym("");
    gNormalSymbol = gensym("normal");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gPSymbol = gensym("p");
    gRawSymbol = gensym("raw");
    gStartSymbol = gensym("start");
    gStopSymbol = gensym("stop");
    gXSymbol = gensym("x");
    gYSymbol = gensym("y");
    reportVersion();
    return 0;
} // main

/*------------------------------------ mtcDoStart ---*/
void
mtcDoStart(MtcData * xx)
{
    unsigned char compress = 't';
    unsigned char no_compress = 'n';

    if (! xx->fStarted)
    {
        mtcPerformWriteCommand(xx, kMtcCommandBeginSendData, 1, xx->fUseCompression ? &compress :
                               &no_compress);
        xx->fStarted = true;
    }
} // mtcDoStart

/*------------------------------------ mtcDoStop ---*/
void
mtcDoStop(MtcData * xx)
{
    mtcPerformWriteCommand(xx, kMtcCommandStopSendData, 0, NULL);
    xx->fStarted = false;
    xx->fStopped = true;
} // mtcDoStop

/*------------------------------------ mtcPerformWriteCommand ---*/
void
mtcPerformWriteCommand(MtcData *            xx,
                       const MtcCommandCode commandCode,
                       const unsigned char  numBytesToFollow,
                       unsigned char *      bytesToFollow)
{
    unsigned char dataValue;
    static t_atom dataList[MAX_MESSAGE_BYTES + 3];

    if (xx)
    {
        short prevLock = lockout_set(1);

        /* Make sure serialX is in 'chunk' mode. */
        if (! xx->fChunkPulseSent)
        {
            outlet_bang(xx->fChunkSendOut);
            xx->fChunkPulseSent = true;
        }
        /* Send data. */
        atom_setlong(dataList, commandCode);
        for (long ii = 0; ii < numBytesToFollow; ++ii)
        {
            dataValue = *bytesToFollow++;
            atom_setlong(dataList + ii + 1, dataValue);
        }
        atom_setlong(dataList + numBytesToFollow + 1, kMtcCommandEnd);
        outlet_list(xx->fDataSendOut, 0L, numBytesToFollow + 2, dataList);
        lockout_set(prevLock);
    }
} // mtcPerformWriteCommand

/*------------------------------------ mtcProcessResponse ---*/
void
mtcProcessResponse(MtcData * xx,
                   long      rr)
{
    /* We've received a byte. Check if it matches what we are expecting. */
    if (xx)
    {
        short prevLock = lockout_set(1);
        long  incoming = (rr & 0x00ff);

#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            if ((kMtcStateAwaitingEndOfComp != xx->fState) &&
                (kMtcStateAwaitingEndOfData != xx->fState) &&
                (kMtcStateAwaitingEndOfDesc != xx->fState))
            {
                LOG_POST_2(xx, OUTPUT_PREFIX "saw byte: 0x%lx", incoming)
            }
        }
#endif /* BE_VERBOSE */
        switch (xx->fState)
        {
            case kMtcStateAwaitingEndOfCommand:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd :
                                   kMtcStateSkipToAck);
                }
                else if (kMtcResponseEndCommand == incoming)
                {
                    mtcChangeState(xx, kMtcStateIdle);
                    if (kMtcResponseAck == xx->fResponse)
                    {
                        outlet_bang(xx->fCommandComplete);
                    }
                    else if ((kMtcResponseSendData == xx->fResponse) ||
                             (kMtcResponseSendComp == xx->fResponse))
                    {
                        mtcProcessTaxels(xx);
                    }
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "EOC/unexpected input character (0x%lx) seen",
                                incoming)
                    outlet_bang(xx->fErrorBangOut);
                    mtcChangeState(xx, kMtcStateSkipToEnd);
                }
                break;

            case kMtcStateAwaitingEndOfComp:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd :
                                   kMtcStateSkipToAck);
                }
                else
                {
                    /* Copy incoming into the buffer. */
                    *(xx->fDataBuffer + xx->fNextByte) = static_cast<unsigned char>(incoming);
                    ++xx->fNextByte;
                    if (xx->fIsPacketHeader)
                    {
                        /* Analyze the map byte */
                        long count = BITCOUNT(incoming);

                        if (3 < count)
                        {
                            count = 3;
                        }
                        xx->fNumBytes = count;
                        if (count)
                        {
                            xx->fIsPacketHeader = false;
                        }
                        else
                        {
                            /* The packet is empty, so count it off */
                            if (! --xx->fNumPackets)
                            {
                                xx->fLastByte = xx->fNextByte;
                                mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                            }
                        }
                    }
                    else if (! --xx->fNumBytes)
                    {
                        /* We've received the last byte of the packet */
                        if (--xx->fNumPackets)
                        {
                            xx->fIsPacketHeader = true;
                        }
                        else
                        {
                            xx->fLastByte = xx->fNextByte;
                            mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                        }
                    }
                }
                break;

            case kMtcStateAwaitingEndOfData:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd :
                                   kMtcStateSkipToAck);
                }
                else
                {
                    /* Copy incoming into the buffer. */
                    *(xx->fDataBuffer + xx->fNextTaxel) = static_cast<unsigned char>(incoming);
                    if (++xx->fNextTaxel >= xx->fNumBytes)
                    {
                        mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                    }
                }
                break;

            case kMtcStateAwaitingEndOfDesc:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd :
                                   kMtcStateSkipToAck);
                }
                else if (kMtcResponseEndCommand == incoming)
                {
                    xx->fDescriptor[xx->fDescriptorLength] = '\0';
                    LOG_POST_2(xx, OUTPUT_PREFIX "descriptor: '%s'", xx->fDescriptor)
                    mtcChangeState(xx, kMtcStateIdle);
                    outlet_bang(xx->fCommandComplete);
                }
                else
                {
                    xx->fDescriptor[xx->fDescriptorLength] = static_cast<char>(incoming);
                    if (MAX_DESCRIPTOR_LENGTH > xx->fDescriptorLength)
                    {
                        ++xx->fDescriptorLength;
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "overlong descriptor truncated")
                        outlet_bang(xx->fErrorBangOut);
                        mtcChangeState(xx, kMtcStateSkipToEnd);
                    }
                }
                break;

            case kMtcStateAwaitingNumBytes:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd :
                                   kMtcStateSkipToAck);
                }
                else
                {
                    xx->fNumBytes = incoming;
                    xx->fNextTaxel = 0;
                    mtcChangeState(xx, kMtcStateAwaitingEndOfData);
                }
                break;

            case kMtcStateAwaitingNumPackets:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd :
                                   kMtcStateSkipToAck);
                }
                else if (xx->fExpectedPackets == incoming)
                {
                    xx->fNumPackets = incoming;
                    xx->fNextByte = xx->fLastByte = 0;
                    xx->fIsPacketHeader = true;
                    mtcChangeState(xx, kMtcStateAwaitingEndOfComp);
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "Bad number of packets (%ld) seen", incoming)
                    outlet_bang(xx->fErrorBangOut);
                    mtcChangeState(xx, kMtcStateSkipToEnd);
                }
                break;

            case kMtcStateAwaitingSampleNumber:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd :
                                   kMtcStateSkipToAck);
                }
                else
                {
                    if ((incoming + xx->fSampleNumberBase) < xx->fSampleNumber)
                    {
                        xx->fSampleNumberBase += 256;   /* wraparound? */
                    }
                    xx->fSampleNumber = incoming + xx->fSampleNumberBase;
                    mtcChangeState(xx, xx->fDataCompressed ? kMtcStateAwaitingNumPackets :
                                   kMtcStateAwaitingNumBytes);
                }
                break;

            case kMtcStateIdle:
                /* Process received data here. */
                xx->fStopped = false;
                xx->fResponse = static_cast<MtcResponseCode>(incoming);
                if (kMtcResponseSendData == incoming)
                {
                    if (xx->fUseCompression)
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "IDLE/unexpected uncompressed data seen")
                        outlet_bang(xx->fErrorBangOut);
                        mtcChangeState(xx, kMtcStateSkipToEnd);
                    }
                    else
                    {
                        mtcChangeState(xx, kMtcStateAwaitingSampleNumber);
                        xx->fDataCompressed = false;
                    }
                }
                else if (kMtcResponseSendComp == incoming)
                {
                    if (xx->fUseCompression)
                    {
                        mtcChangeState(xx, kMtcStateAwaitingSampleNumber);
                        xx->fDataCompressed = true;
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "IDLE/unexpected compressed data seen")
                        outlet_bang(xx->fErrorBangOut);
                        mtcChangeState(xx, kMtcStateSkipToEnd);
                    }
                }
                else if (kMtcResponseAck == incoming)
                {
                    mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                }
                else if (kMtcResponseUnitDesc == incoming)
                {
                    xx->fDescriptorLength = 0;
                    mtcChangeState(xx, kMtcStateAwaitingEndOfDesc);
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "IDLE/unexpected input character (0x%lx) seen",
                                incoming)
                    outlet_bang(xx->fErrorBangOut);
                    mtcChangeState(xx, kMtcStateSkipToEnd);
                }
                break;

            case kMtcStateLookingForEnd:
                if (kMtcResponseEndCommand == incoming)
                {
                    outlet_bang(xx->fCommandComplete);
                    mtcChangeState(xx, kMtcStateIdle);
                }
                else if (kMtcResponseAck != incoming)
                {
                    mtcChangeState(xx, kMtcStateSkipToAck);
                }
                break;

            case kMtcStateSkipToAck:
                xx->fStopped = false;
                if (kMtcResponseAck == incoming)
                {
                    mtcChangeState(xx, kMtcStateLookingForEnd);
                }
                break;

            case kMtcStateSkipToEnd:
                if (kMtcResponseEndCommand == incoming)
                {
                    mtcChangeState(xx, kMtcStateIdle);
                }
                break;

            default:
                break;
                
        }
        lockout_set(prevLock);
    }
} // mtcProcessResponse

/*------------------------------------ mtcSetKind ---*/
bool
mtcSetKind(MtcData *  xx,
           t_symbol * kind)
{
    bool okSoFar = true;

    if (kind == gCompressedSymbol)
    {
        xx->fUseCompression = true;
    }
    else if ((kind == gNormalSymbol) || (kind == gEmptySymbol))
    {
        xx->fUseCompression = false;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized compression (%s)", kind->s_name)
        okSoFar = false;
    }
    return okSoFar;
} // mtcSetKind

/*------------------------------------ mtcSetMode ---*/
bool
mtcSetMode(MtcData *  xx,
           t_symbol * rawOrCooked)
{
    bool okSoFar = true;

    if (rawOrCooked == gCookedSymbol)
    {
        xx->fModeRaw = false;
    }
    else if (rawOrCooked == gRawSymbol)
    {
        xx->fModeRaw = true;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized processing mode (%s)", rawOrCooked->s_name)
        okSoFar = false;
    }
    return okSoFar;
} // mtcSetMode

/*------------------------------------ mtcSetOrder ---*/
bool
mtcSetOrder(MtcData *  xx,
            t_symbol * order)
{
    bool okSoFar = true;

    if (order == gPSymbol)
    {
        xx->fSortOrder = kMtcOrderByPressure;
    }
    else if (order == gXSymbol)
    {
        xx->fSortOrder = kMtcOrderByX;
    }
    else if (order == gYSymbol)
    {
        xx->fSortOrder = kMtcOrderByY;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized sort order (%s)", order->s_name)
        okSoFar = false;
    }
    return okSoFar;
} // mtcSetOrder
