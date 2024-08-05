/*****************************************************************************
 *                                                                           *
 *   FAZGUT.C    (GUTILS.C)                                                  *
 *                                                                           *
 *   Worldgroup 3.2 conversion                     - R. Hadsall 4/24/2021    *
 *   The Major BBS v10 conversion                  - R. Hadsall 4/18/2023    *
 *   Minor edits to allow WG32/V10 from same kit   - R. Hadsall 8/5/2024     *
 *                                                                           *
 * Copyright (C) 2005-2024 Rick Hadsall.  All Rights Reserved.               *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU Affero General Public License as published  *
 * by the Free Software Foundation, either version 3 of the License, or      *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU Affero General Public License for more details.                       *
 *                                                                           *
 * You should have received a copy of the GNU Affero General Public License  *
 * along with this program. If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                           *
 * Additional Terms for Contributors:                                        *
 * 1. By contributing to this project, you agree to assign all right, title, *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies.                              *
 * 2. You grant Rick Hadsall and Elwynor Technologies a non-exclusive,       *
 *    royalty-free, worldwide license to use, reproduce, prepare derivative  *
 *    works of, publicly display, publicly perform, sublicense, and          *
 *    distribute your contributions                                          *
 * 3. You represent that you have the legal right to make your contributions *
 *    and that the contributions do not infringe any third-party rights.     *
 * 4. Rick Hadsall and Elwynor Technologies are not obligated to incorporate *
 *    any contributions into the project.                                    *
 * 5. This project is licensed under the AGPL v3, and any derivative works   *
 *    must also be licensed under the AGPL v3.                               *
 * 6. If you create an entirely new project (a fork) based on this work, it  *
 *    must also be licensed under the AGPL v3, you assign all right, title,  *
 *    and interest, including all copyrights, in and to your contributions   *
 *    to Rick Hadsall and Elwynor Technologies, and you must include these   *
 *    additional terms in your project's LICENSE file(s).                    *
 *                                                                           *
 * By contributing to this project, you agree to these terms.                *
 *                                                                           *
 *****************************************************************************/

#include "gcomm.h"
#include "fazdat.h"

static VOID srchrs(VOID);

VOID
ucify(                        /* upper-case-ify target string              */
CHAR *stgptr)
{
     while ((*stgptr=(CHAR)toupper(*stgptr)) != '\0') {
          stgptr++;
     }
}

SHORT
getrnd(VOID)                  /* get a pseudorandom integer                */
{                             /* alternate form: x=(x*25173L+13849L)%0xFFFF*/
     static SHORT rnum=31785;

     return(rnum=(SHORT)((rnum*15483L+7169L)&0x7FFF));
}

VOID
addmbf(                       /* "add to message buffer" (archaic)         */
const char *stgptr)
{
     prf("%s",stgptr);
}

VOID
mspstg(                       /* "make special string" (archaic)           */
const char *stgptr,
...)
{
     va_list ap;
     static CHAR result[200];

     va_start(ap,stgptr);
     vsprintf(result,stgptr,ap);
     va_end(ap);
     prf("%s",result);
}

VOID
prscmd(VOID)                  /* parse command line                        */
{
     urgc=0;
     fzcptr=pyrcmd;
     while (*fzcptr == ' ') {
          fzcptr++;
     }
     if (*fzcptr == '"') {
          fzcptr++;
          while (*fzcptr == ' ') {
               fzcptr++;
          }
          srchrs();
          if (urgc != 0 && *(arge[urgc]-1) == '"') {
               arge[urgc]-=1;
               *arge[urgc]='\0';
               if (urgv[urgc-1] == arge[urgc]) {
                    urgc--;
               }
          }
     }
     else {
          srchrs();
     }
}

static VOID
srchrs(VOID)                  /* search for space characters               */
{
     while (*fzcptr != '\0') {
          urgv[urgc++]=fzcptr;
          while (*fzcptr > ' ') {
               fzcptr++;
          }
          arge[urgc]=fzcptr;
          if (*fzcptr != '\0') {
               *fzcptr++='\0';
          }
          while (*fzcptr == ' ') {
               fzcptr++;
          }
     }
}

VOID
recomb(VOID)                  /* re-combine input word array               */
{
     INT urgctr;

     for (urgctr=1 ; urgctr < urgc ; ) {
          *arge[urgctr++]=' ';
     }
}

INT
nmatch(                       /* "near" match -- similar to sameto()       */
CHAR *trmstg,
CHAR *untstg)
{
     while (*trmstg != '\0') {
          if (*trmstg++ != *untstg++) {
               return(0);
          }
     }
     return(1);
}

INT
stgcmp(                       /* string compare -- similar to strcmp()     */
CHAR *cmdstg,
CHAR *posmat)
{
     for ( ; (*cmdstg) == (*posmat) ; cmdstg++,posmat++) {
          if (*cmdstg == '\0') {
               return(0);
          }
     }
     return((*cmdstg)-(*posmat));
}
