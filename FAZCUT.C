/*****************************************************************************
 *                                                                           *
 *   FAZCUT.C    (CMDUTL.C)                                                  *
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
#include "elwfaz.h"

extern CHAR *fazatr;

static VOID updctr(VOID);
static VOID tinla(VOID);
static VOID sptgrp(INT objcls,INT count,INT oclsar[]);
static VOID zapthe(VOID);

CHAR_BUFFER(CANTGOTHATWAY, 80, "...You can't go that way.\r");
CHAR_BUFFER(LINEFEEDCR, 80, "\r");
CHAR_BUFFER(HELPPROMPT, 80, "...Type \"help\" for help.\r");
CHAR_BUFFER(CANTNOTHOLDINGONE, 80, "...You can't, you aren't holding one.\r");
CHAR_BUFFER(FORCEUNSEEN, 80, "...The force is unseen!\r");

VOID
game(VOID)                    /* main <CR>-input handler when in game      */
{
     if (urgc == 0) {
          btuxmt(pyrctr,fazatr);
          if (usrptr->flags&ABOIP) {
               btuxmt(pyrctr,LINEFEEDCR);
          }
          else {
               btuxmt(pyrctr,HELPPROMPT);
          }
     }
     else {
          zapthe();
          sttptr=pyrptr->sttptr;
          if (hasmkey(FAZKEY)) {
               if (!(*(sttptr->cmhdlr))()) {
                    updctr();
                    return;
               }
          }
          if (!stdcmd()) {
               updctr();
               return;
          }
          if (urgc == 1) {
               *urgv[0]=(CHAR)toupper((UCHAR)*urgv[0]);
               mspstg("...%s what?\r",urgv[0]);
          }
          else if (stgcmp(urgv[0],"because") == 0) {
               addmbf("...That's a lousy reason!\r");
          }
          else if (*urgv[0] == 'i') {
               addmbf("...Why explain your problems to a machine?\r");
          }
          else if (urgc == 2) {
               recomb();
               mspstg("...What good does it do to %s?\r",urgv[0]);
          }
          else if (urgc == 3) {
               recomb();
               mspstg("...Why would you want to %s?\r",urgv[0]);
          }
          else if (urgc == 4) {
               recomb();
               mspstg("...How do you expect to %s?\r",urgv[0]);
          }
          else {
               addmbf("...Oh, baloney!\r");
          }
          if (pyrptr->newflg&FLOBIT) {
               addmbf("   (Type \"help\" for help.)\r");
               pyrptr->newflg&=~FLOBIT;
          }
          gibber();
          infoth((pyrptr->logon&SEXBIT) ? "mumbling incoherently to himself"
                                        : "mumbling incoherently to herself");
     }
}

static VOID
updctr(VOID)                  /* update object counters for delayed effects */
{
     for (objidx=0 ; objidx < pyrptr->nobjs ; objidx++) {
          if (pyrptr->pobjct[objidx] != 0) {
               if (--(pyrptr->pobjct[objidx]) == 0) {
                    (*(pyrptr->pyrobj[objidx]->objtvc))();
               }
          }
     }
}

INT
move(                         /* movement utility w/msgs to others         */
struct state *dstspt,
const char *lvmsg,
const char *entmsg)
{
     if (urgc > 1) {
          return(1);
     }
     if (dstspt == NULL) {
          btuxmt(pyrctr,CANTGOTHATWAY);
          infoth("crashing around blindly");
     }
     else {
          lvstt(lvmsg);
          ntrstt(dstspt,entmsg);
     }
     return(0);
}

INT
shove(                        /* shove utility w/msgs to others            */
struct state *dstspt,
const char *lvdir,
const char *entmsg)
{
     struct player **tmpypp;
     struct player *prpsav;
     INT prcsav;

     if (dstspt == NULL) {
          mspstg("...%s crashes against the ",othpyr->auxhdl);
          mspstg("%s wall!\r",lvdir);
          gibber();
          mspstg("***\r%s has just shoved you against the ",pyrptr->auxhdl);
          mspstg("%s wall!\r",lvdir);
          outpyr(othpyr);
          mspstg("***\r%s has just shoved ",pyrptr->auxhdl);
          addmbf(othpyr->auxhdl);
          mspstg(" against the %s wall!\r",lvdir);
          outexc(othpyr);
     }
     else {
          mspstg("...You caught %s by surprise!\r",othpyr->auxhdl);
          gibber();
          mspstg("***\r%s has just shoved you away to the ",pyrptr->auxhdl);
          mspstg("%s!\r",lvdir);
          outpyr(othpyr);
          mspstg("***\r%s has just shoved ",pyrptr->auxhdl);
          addmbf(othpyr->auxhdl);
          mspstg(" away to the %s!\r",lvdir);
          outexc(othpyr);
          tmpypp=&(othpyr->sttptr->plhead);
          while (*tmpypp != othpyr) {
               tmpypp=&((**tmpypp).nxtpyr);
          }
          *tmpypp=othpyr->nxtpyr;
          prpsav=pyrptr;
          prcsav=pyrctr;
          pyrptr=othpyr;
          pyrctr=(INT)(othpyr-pyrarr);
          ntrstt(dstspt,entmsg);
          pyrptr=prpsav;
          pyrctr=prcsav;
     }
     return(0);
}

VOID
ntrstt(                       /* enter-a-state utility, with msg to others */
struct state *svcptr,
const char *txtptr)
{
     INT i,j,k=0; // RH 6/20/2024 added to avoid C4701 potentially uninitialized variable - but what should the default be?

     pyrptr->logon|=INGAME;
     pyrptr->nxtpyr=svcptr->plhead;
     svcptr->plhead=pyrptr;
     pyrptr->sttptr=svcptr;
     if (!invis(pyrptr)) {
          mspstg("***\r%s has just ",pyrptr->auxhdl);
          addmbf(txtptr);
          addmbf("!\r");
          fazoth();
     }
     if (pyrptr->logon&BRFBIT) {
          mspstg("...You're %s.",svcptr->sdscrp);
          whther();
          whoher();
          gibber();
          return;
     }
     for (i=0 ; i < NVSTMN ; ) {
          if (pyrptr->vsttes[i++] == svcptr->sttmsg) {
               mspstg("...You're %s.",svcptr->sdscrp);
               whther();
               whoher();
               gibber();
               return;
          }
     }
     for (i=0,j=0 ; i < NVSTMN ; i++) {
          if (--(pyrptr->vages[i]) < j) {
               j=pyrptr->vages[i];
               k=i;
          }
     }
     pyrptr->vages[k]=0;
     pyrptr->vsttes[k]=(SHORT)(svcptr->sttmsg);
     prfmsg(svcptr->sttmsg);
     whther();
     whoher();
     gibber();
}

VOID
whoher(VOID)                  /* "who's here?" - id of others to player    */
{
     INT ctr;
     INT frcflg;

     frcflg=hold(&objcts[42]);
     ctr=0;
     othpyr=pyrptr->sttptr->plhead;
     while (1) {
          if (othpyr != pyrptr) {
               if (!invis(othpyr) || frcflg) {
                    ctr+=1;
               }
          }
          if ((othpyr=othpyr->nxtpyr) == NULL) {
               break;
          }
     }
     if (ctr == 0) {
          return;
     }
     if (ctr == 1) {
          --ctr;
     }
     othpyr=pyrptr->sttptr->plhead;
     while (1) {
          if (othpyr != pyrptr) {
               if (!invis(othpyr) || frcflg) {
                    if (invis(othpyr)) {
                         mspstg("%s (your fracas hums for a moment)",
                                othpyr->plyrid);
                    }
                    else {
                         addmbf(othpyr->auxhdl);
                    }
                    if (--ctr > 1) {
                         addmbf(", ");
                    }
                    else if (ctr == 1) {
                         addmbf(" and ");
                    }
                    else if (ctr == 0) {
                         addmbf(" are here.\r");
                         break;
                    }
                    else if (ctr < 0) {
                         addmbf(" is here.\r");
                         break;
                    }
               }
          }
          if (othpyr->nxtpyr != NULL) othpyr=othpyr->nxtpyr; // Compiler warning 4/18/2023 R. Hadsall added if statement
     }
}

VOID
whther(VOID)                  /* "what's here" - id of objects to player   */
{
     INT i,j,k,l;
     INT oclsar[NOBJST] = { 0 }, oclcnt[NOBJST + 1] = { 0 }, oclrep[NOBJST + 1] = { 0 }; // RH 6/28/2024 iniitalize with zeroes

     if ((j=pyrptr->sttptr->nobjs) != 0) {
          for (i=0 ; i < j ; i++) {
               oclsar[i]=0;
          }
          k=0;
          for (i=0 ; i < j ; i++) {
               if (oclsar[i] == 0
                && (pyrptr->sttptr->sttobj[i]->flags&OBJNVS) == 0) {
                    oclsar[i]=++k;
                    oclrep[k]=i;
                    oclcnt[k]=1;
                    for (l=i+1 ; l < j ; l++) {
                         if (pyrptr->sttptr->sttobj[l]->positn
                          == pyrptr->sttptr->sttobj[i]->positn) {
                              oclsar[l]=k;
                              oclcnt[k]+=1;
                         }
                    }
               }
          }
          if (k != 0) {
               addmbf("  There is a ");
               for (i=1 ;; i++) {
                    sptgrp(i,oclcnt[i],oclsar);
                    mobjps(pyrptr->sttptr->sttobj[oclrep[i]]);
                    if (i == k) {
                         addmbf(".\r");
                         break;
                    }
                    else if (i == k-1) {
                         addmbf(" and a ");
                    }
                    else {
                         addmbf(", a ");
                    }
               }
          }
          else {
               tinla();
          }
     }
     else {
          tinla();
     }
     noistt=0;
}

static VOID
tinla(VOID)                   /* "there is nothing lying around" utility   */
{
     if (noistt == -1) {
          addmbf("There is nothing lying around.");
     }
     addmbf("\r");
}

static VOID
sptgrp(                       /* spit out a group of objects to player     */
INT objcls,
INT count,
INT oclsar[])
{
     INT i,ndun;

     for (i=0,ndun=0 ;; i++) {
          if (oclsar[i] == objcls) {
               addmbf(pyrptr->sttptr->sttobj[i]->name);
               if (++ndun == count) {
                    break;
               }
               else if (ndun == count-1) {
                    addmbf(" and a ");
               }
               else {
                    addmbf(", a ");
               }
          }
     }
}

VOID
mobjps(                       /* make object position relative to state    */
struct object *local_zbjptr)
{
     mspstg(local_zbjptr->positn,pyrptr->sttptr->sobjlc);
}

VOID
lvstt(                        /* leave-state utility, with msg to others   */
const char *txtptr)
{
     struct player **tmpypp;

     pyrptr->logon&=~INGAME;
     tmpypp=&(pyrptr->sttptr->plhead);
     while (*tmpypp != pyrptr) {
          tmpypp=&((**tmpypp).nxtpyr);
     }
     *tmpypp=pyrptr->nxtpyr;
     if (pyrptr->sttptr->plhead != NULL) {
          if (!invis(pyrptr)) {
               mspstg("***\r%s has just ",pyrptr->auxhdl);
               addmbf(txtptr);
               addmbf("!\r");
               fazoth();
          }
     }
}

static VOID
zapthe(VOID)                  /* remove "the" and "a" from input line      */
{
     zapwrd("the");
     zapwrd("a");
}

VOID
zapwrd(                       /* remove a specified word from input line   */
CHAR *word)
{
     INT i,j;

     for (i=1 ; i < urgc-1 ; i++) {
          if (stgcmp(urgv[i],word) == 0) {
               *arge[i+1]=' ';
               urgc-=1;
               for (j=i ; j < urgc ; j++) {
                    urgv[j]=urgv[j+1];
                    arge[j+1]=arge[j+2];
               }
          }
     }
}

VOID
donthv(VOID)                  /* you-don't-have-one input rejection        */
{
     btuxmt(pyrctr,CANTNOTHOLDINGONE);
     infoth("hallucinating a little");
}

VOID
ainthr(                       /* "he ain't there" input rejection          */
INT argno)
{
     if (stgcmp(urgv[argno],"Some") == 0
      || stgcmp(urgv[argno],"Unseen") == 0
      || stgcmp(urgv[argno],"Force") == 0) {
          btuxmt(pyrctr,FORCEUNSEEN);
     }
     else {
          mspstg("...You can't, %s doesn't appear to be here.\r",urgv[argno]);
          gibber();
     }
     infoth("clutching at phantoms");
}

INT
holdng(                       /* return true if player is holding object   */
struct object *obj)
{
     for (objidx=0 ; objidx < pyrptr->nobjs ; objidx++) {
          if (pyrptr->pyrobj[objidx] == obj) {
               return(1);
          }
     }
     return(0);
}

INT
hold(                         /* same as above but with local objidx       */
struct object *obj)
{
     INT local_objidx;

     for (local_objidx =0 ; local_objidx < pyrptr->nobjs ; local_objidx++) {
          if (pyrptr->pyrobj[local_objidx] == obj) {
               return(1);
          }
     }
     return(0);
}

INT
hldoth(                       /* see if the "other" guy is holding a object */
struct object *obj)
{
     INT local_objidx;

     for (local_objidx =0 ; local_objidx < othpyr->nobjs ; local_objidx++) {
          if (othpyr->pyrobj[local_objidx] == obj) {
               return(1);
          }
     }
     return(0);
}

INT
invis(                        /* return true if player is invisible        */
struct player *pyrp)
{
     return(pyrp->auxhdl[4] == 32);
}

INT
iszleen(                      /* return true if player is a zleen          */
struct player *pyrp)
{
     return(strlen(pyrp->auxhdl) > 9 && !invis(pyrp));
}
