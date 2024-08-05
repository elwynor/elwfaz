/*****************************************************************************
 *                                                                           *
 *   FAZMST.C    (MSGSTF.C)                                                  *
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
#include "majorbbs.h"
#include "fazdat.h"

static VOID genpos(struct state *where);
static VOID zing(struct state *zingrg);

static
struct state *poss[17];

VOID
jabber(                       /* output specified msg num to player        */
INT msgnum)
{
     prfmsg(msgnum);
     outprf(pyrctr);
}

VOID
gibber(VOID)                  /* output prfbuf contents to player          */
{
     outprf(pyrctr);
}

VOID
outnis(VOID)                  /* output to everyone not in player's state  */
{
     INT i;

     for (i=0 ; i < nterms ; i++) {
          if (pyrarr[i].sttptr != pyrptr->sttptr && (pyrarr[i].logon&INGAME)) {
               outprf(i);
          }
     }
     clrprf();
}

VOID
fazall(VOID)                  /* output to everyone except player          */
{
     INT i;

     for (i=0 ; i < nterms ; i++) {
          if (i != pyrctr && (pyrarr[i].logon&INGAME)) {
               outprf(i);
          }
     }
     clrprf();
}

VOID
fazoth(VOID)                  /* output to others in player's state        */
{
     struct player *tmpypt;

     tmpypt=pyrptr->sttptr->plhead;
     while (tmpypt != NULL) { //while (tmpypt != 0) {
          if (tmpypt != pyrptr) {
               outprf(tmpypt->portno);
          }
          tmpypt=tmpypt->nxtpyr;
     }
     clrprf();
}

VOID
outpyr(                       /* output to specified player only           */
struct player *othpyr_local)
{
     outprf(othpyr_local->portno);
}

VOID
outexc(                       /* output to 3rd-party viewers               */
struct player *ptrpyr)
{
     struct player *tmpypt;

     tmpypt=pyrptr->sttptr->plhead;
     while (1) {
          if (tmpypt != pyrptr && tmpypt != ptrpyr) {
               outprf(tmpypt->portno);
          }
          if ((tmpypt=tmpypt->nxtpyr) == 0) {
               break;
          }
     }
     clrprf();
}

VOID
outner(VOID)                  /* output to "nearby" states (rooms)         */
{
     poss[0]=pyrptr->sttptr;
     intmp1=1;
     genpos(poss[0]);
     for (intmp2=1 ; intmp2 < intmp1 ; intmp2++) {
          outare(poss[intmp2]);
     }
     clrprf();
}

VOID
fazfar(VOID)                  /* output to "far" (once removed) states     */
{
     INT norgp;

     poss[0]=pyrptr->sttptr;
     intmp1=1;
     genpos(poss[0]);
     norgp=intmp1;
     for (intmp2=1 ; intmp2 < norgp ; intmp2++) {
          genpos(poss[intmp2]);
     }
     for (intmp2=norgp ; intmp2 < intmp1 ; intmp2++) {
          outare(poss[intmp2]);
     }
     clrprf();
}

static VOID
genpos(                       /* generate possibilities to output to       */
struct state *where)
{
     zing(where->north);
     zing(where->south);
     zing(where->east);
     zing(where->west);
}

static VOID
zing(                         /* accumulate possibilities without duplicates */
struct state *zingrg)
{
     INT i;

     if (zingrg != 0) {
          for (i=0 ; i < intmp1 ; i++) {
               if (zingrg == poss[i]) {
                    return;
               }
          }
          poss[intmp1++]=zingrg;
     }
}

VOID
outare(                       /* output prfbuf to a given state (area)     */
struct state *spoint)
{
     struct player *tmpypt;

     tmpypt=spoint->plhead;
     while (tmpypt != 0) {
          outprf(tmpypt->portno);
          tmpypt=tmpypt->nxtpyr;
     }
     clrprf();
}
