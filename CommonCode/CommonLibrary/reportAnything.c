/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       reportAnything.c                                                        */
/*                                                                                      */
/*  Contains:   The routine reportAnything().                                           */
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
/*  Created:    2002/04/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "reportAnything.h"

/*------------------------------------ reportAnything ---*/
void reportAnything
  (Qchar				routineName,
   PSymbol      message,
   const short	argc,
   PAtom        argv)
{
#if defined(COMPILE_FOR_STUB)
 #pragma unused(routineName,message,argc,argv)
#else /* not COMPILE_FOR_STUB */
  error("%s unknown message '%s' seen", routineName, message->s_name);
  for (short ii = 0; ii < argc; ii++)
  {
    switch (argv[ii].a_type)
    {
      case A_LONG:
        post("  argument %hd is a long (%ld)", ii, argv[ii].a_w.w_long);
        break;

      case A_SYM:
        post("  argument %hd is a symbol (%s)", ii, argv[ii].a_w.w_sym->s_name);
        break;

      case A_FLOAT:
        post("  argument %hd is a float (%g)", ii, double(argv[ii].a_w.w_float));
        break;

      case A_SEMI:
        post("  argument %hd is a semicolon", ii);
        break;

      case A_COMMA:
        post("  argument %hd is a comma", ii);
        break;

      case A_DOLLAR:
        post("  argument %hd is a dollar sign", ii);
        break;

      default:
        post("  argument %hd is an unknown type (%hd)", ii, argv[ii].a_type);
        break;

    }
  }
#endif /* not COMPILE_FOR_STUB */
} /* reportAnything */

#if defined(COMPILE_FOR_STUB)
 #pragma export list reportAnything
#endif /* COMPILE_FOR_STUB */