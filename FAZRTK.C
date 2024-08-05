/*****************************************************************************
 *                                                                           *
 *   FAZRTK.C    (ZRTKICK.C)                                                 *
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

#define NRTKNT 20             /* number of rtks poss at one time           */

static struct {               /* 'zuul flavor of real-time kicktable data  */
     INT timer;                    /* seconds counter                      */
     VOID (*xfvec)(INT);           /* transfer vector (addr to invoke)     */
     INT param;                    /* parameter to pass when invoking      */
} rtktbl[NRTKNT],*rtkptr;

VOID
zinirtk(VOID)                 /* initialize rtkick stuff                   */
{
     for (rtkptr=rtktbl ; rtkptr != rtktbl+NRTKNT ; rtkptr++) {
          rtkptr->timer=0;
     }
}

VOID
zrtkick(                      /* arrange for an action to take place later */
INT time,
VOID (*rouptr)(INT),
INT parm)
{
     for (rtkptr=rtktbl ; rtkptr != rtktbl+NRTKNT ; rtkptr++) {
          if (rtkptr->timer == 0) {
               rtkptr->timer=time;
               rtkptr->xfvec=rouptr;
               rtkptr->param=parm;
               break;
          }
     }
}

VOID
zrtksvc(VOID)                 /* service any requests that have come due   */
{
     for (rtkptr=rtktbl ; rtkptr != rtktbl+NRTKNT ; rtkptr++) {
          if (rtkptr->timer != 0 && --(rtkptr->timer) == 0) {
               (*(rtkptr->xfvec))(rtkptr->param);
          }
     }
}
