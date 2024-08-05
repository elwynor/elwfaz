/*****************************************************************************
 *                                                                           *
 *   FAZUUL.C                                                                *
 *                                                                           *
 *   This is the mainline of the FAZUUL game.      - T. Stryker 2/15/84      *
 *   Modified to act as a module of The Major BBS  - T. Stryker 7/5/86       *
 *   Modified to be a shippable add-on for version 5.2+ of The Major BBS     *
 *                                                 - T. Stryker 1/24/91      * 
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

static VOID zrtkdo(VOID);
static GBOOL fazinp(VOID);
static GBOOL fazinj(VOID);
static VOID zuuhup(VOID);
static VOID zuuenc(VOID);
static VOID encwrt(SHORT intqty,SHORT *dstbuf);
static VOID zuudcd(VOID);
static SHORT dcdrd(SHORT *srcbuf);
static VOID delfaz(CHAR *userid);
static VOID finzuu(VOID);

INT zuustt;                   /* Fazuul state number                       */
struct module fazuul={        /* module interface block                    */
     "",                      /*    name used to refer to this module      */
     NULL,                    /*    user log-on suplement                  */
     fazinp,                  /*    input routine if selected         */
     dfsthn,                  /*    status-input routine                   */
     fazinj,                  /*    "injoth" routine for this module       */
     NULL,                    /*    user logoff supplemental routine       */
     zuuhup,                  /*    hangup (lost carrier) routine     */
     NULL,                    /*    midnight clean-up                      */
     delfaz,                  /*    delete-account routine            */
     finzuu                   /*    finish-up (sys shutdown) routine  */
};

static DFAFILE *zuulbb;       /* Fazuul Btrieve user data file block ptr   */
static HMCVFILE zuulmb;       /* Fazuul message file block ptr             */

struct player *pyrarr,        /* dynam alloc'd player data structure array */
              *pyrptr,        /* ptr to current user's pyrarr element      */
              *othpyr;        /* "other" player ptr -- for actions etc.    */

struct state  *sttptr;        /* global scratch vbl for room structure ptr */
struct object *zbjptr;        /* global scratch vbl for object (item) ptr  */

INT pyrctr,                   /* "player counter" - same as usrnum in svr  */
    urgc;                     /* argument count - same as margc in server  */

CHAR pyrcmd[256],             /* "player command" - same as input in server*/
     *fzcptr,                 /* global scratch vbl for scanning pyrcmd    */
     *urgv[64],               /* argument values - same as margv in server */
     *arge[65];               /* argument ends - same as marge in server   */

INT intmp1,                   /* scratch temporary vbl 1                   */
    intmp2,                   /* scratch temporary vbl 2                   */
    objidx,                   /* global scratch vbl for object indexes     */
    noistt;                   /* flag to whther() for uncond output        */

CHAR *fazatr;                 /* attribute for displayed FAZUUL text       */
CHAR *fazcant;                /* not allowed to play FAZUUL fully (msg)    */

CHAR_BUFFER(LINEFEED, 80, "\r");

#define VERSION "6.1" // best guess after all the (c) statements from Stryker from 84-92, plus 95's conversion

VOID EXPORT
init__elwfaz(VOID)            /* initialize the Fazuul game                */
{
 
     stzcpy(fazuul.descrp,gmdnam("ELWFAZ.MDF"),MNMSIZ);
     zuustt=register_module(&fazuul);
     zinirtk();
     zuulmb=opnmsg("elwfaz.mcv");

     shocst(spr("ELW Fazuul v%s",VERSION),"(C) Copyright 2005-2024 Elwynor Technologies - www.elwynor.com");

     fazatr=stgopt(FAZATR);
     fazcant=stgopt(FAZCANT);
     inists();
     zuulbb=dfaOpen("elwfaz.dat",sizeof(struct player),NULL);
     rtkick(1,zrtkdo);
     pyrarr=(struct player *)alczer(nterms*sizeof(struct player));
}

static VOID
zrtkdo(VOID)                  /* 'zuul "real-time kicktable" servicer      */
{
     setmbk(zuulmb);
     zrtksvc();
     rtkick(1,zrtkdo);
}

static GBOOL
fazinp(VOID)                  /* main fazuul game <CR>-input entry point   */
{
    CHAR* inpptr, * pycmdp; // , whatever[256] = "";
     INT i,len,bad=0;

     setmbk(zuulmb);
     pyrctr=usrnum;
     pyrptr=&(pyrarr[pyrctr]);
     if (usrptr->substt != 0 && margc > 0
      && (sameas(margv[0],"x") || sameas(margv[0],"exit"))) {
          if (pyrptr->logon&INGAME) {
               lvstt("dematerialized");
          }
          btupmt(pyrctr,0);
          prf("...Exiting FAZUUL, returning to main menu...\r");
          usrptr->pfnacc=0;
          return(0);
     }
     switch (usrptr->substt) {
     case 0:
          dfaSetBlk(zuulbb);
          btuxmt(pyrctr,fazatr);
          btuxmt(pyrctr, LINEFEED);
          if (pyrptr->logon&SGOBIT) {
               ntrstt(pyrptr->sttptr,"materialized next to you");
               btupmt(pyrctr,'>');
               usrptr->substt=6;
          }
          else if (dfaAcqEQ(pyrptr,usaptr->userid,0)) {
               pyrptr->portno=(SHORT)pyrctr;
               zuudcd();
               prfmsg(FAZ47);
               fazall();
               memset(pyrptr->vsttes,0,sizeof(pyrptr->vsttes));
               memset(pyrptr->vages,0,sizeof(pyrptr->vages));
               invrtn();
               ntrstt(pyrptr->sttptr,"popped into existence next to you");
               btupmt(pyrctr,'>');
               usrptr->substt=6;
          }
          else {
               btumil(usrnum,ALSSIZ-1);
               usrptr->substt++;
               prfmsg(GETALS);
               outprf(usrnum);
          }
          break;
     case 1:
          setmbk(zuulmb);
          dfaSetBlk(zuulbb);
          len = strlen(margv[0]);
          if (len > 0) {
              strlwr(margv[0]);
              margv[0][0] = (CHAR)toupper(margv[0][0]);
          }
          else bad = 1;
          if (pfnlvl > 0) {
               prfmsg(FOULID);
               outprf(usrnum);
               break;
          }
          if (!bad && margc != 1) {
               bad=1;
          }
          else if (!bad) {
               for (i=0 ; i < len ; i++) {
                    if (digalw ? !isalnum(margv[0][i])
                               : !isalpha(margv[0][i])) {
                         bad=1;
                         break;
                    }
               }
          }
          if (bad || len < 3 || len > (ALSSIZ-1)) {
               prfmsg(BADPID);
               prfmsg(B4PLA2);
          }
          else if ((sameas(margv[0],"Sysop") && !sameas(usaptr->userid,"Sysop"))
                || dfaQueryEQ(margv[0],1)) {
               prfmsg(NTGOOD);
               prfmsg(B4PLA2);
          }
          else {
               stlcpy(pyrptr->plyrid,margv[0],ALSSIZ);
               pyrptr->logon=(usaptr->sex == 'M' ? SEXBIT+SGOBIT : SGOBIT);
               stlcpy(pyrptr->handle,usaptr->userid,USERSZ);
               stlcpy(pyrptr->auxhdl,pyrptr->plyrid,AUXSIZ);
               pyrptr->newflg=FLOBIT;
               pyrptr->sttptr=&states[1];
               dfaInsert(pyrptr);
               pyrptr->portno=(SHORT)pyrctr;
               prfmsg(GOODPD,margv[0]);
               btumil(usrnum,DFTIMX);
               usrptr->substt++;
          }
          outprf(usrnum);
          break;
     case 2:
          jabber(FAZ26);
          usrptr->substt++;
          break;
     case 3:
          jabber(FAZ27);
          usrptr->substt++;
          break;
     case 4:
          jabber(FAZ28);
          usrptr->substt++;
          break;
     case 5:
          prfmsg(FAZ49);
          fazall();
          ntrstt(&states[1],"beamed in from off-planet");
          btupmt(pyrctr,'>');
          usrptr->substt++;
          break;
     case 6:
         /*for (i = 0; i < margc; i++) {
             strcat(whatever, margv[0]);
             strcat(whatever, " ");
         }*/
         rstrin();
         for (inpptr=input,pycmdp=pyrcmd ; *inpptr != '\0' ; ) {
               *pycmdp++=(CHAR)tolower(*inpptr);
               inpptr+=1;
          }
          *pycmdp='\0';
          prscmd();
          game();
          break;
     }
     return(1);
}

static GBOOL
fazinj(VOID)                  /* 'zuul injoth() routine handler            */
{
     dftinj();
     btuxmn(othusn,fazatr);
     return(1);
}

static VOID
zuuhup(VOID)                  /* 'zuul loss-of-carrier entry point         */
{
     pyrctr=usrnum;
     pyrptr=&(pyrarr[pyrctr]);
     if (pyrptr->logon&SGOBIT) {
          if (pyrptr->logon&INGAME) {
               lvstt("vanished");
          }
          zuuenc();
          dfaSetBlk(zuulbb);
          dfaGetEQ(NULL,pyrptr->handle,0);
          dfaUpdate(pyrptr);
     }
     setmem(pyrptr,sizeof(struct player),0);
}

static VOID
zuuenc(VOID)                  /* "encode" player state and object ptrs     */
{
     INT i;

     encwrt((SHORT)((pyrptr->sttptr)-states),(SHORT *)(&(pyrptr->sttptr)));
     for (i=0 ; i < NOBJHD ; i++) {
          encwrt((SHORT)((pyrptr->pyrobj[i])-objcts),
                 (SHORT *)(&(pyrptr->pyrobj[i])));
     }
}

static VOID
encwrt(                       /* utility routine to avoid compile warnings */
SHORT intqty,
SHORT *dstbuf)
{
     *dstbuf=intqty;
}

static VOID
zuudcd(VOID)                  /* "decode" player state and object ptrs     */
{
     INT i;

     pyrptr->sttptr=&states[dcdrd((SHORT *)(&(pyrptr->sttptr)))];
     for (i=0 ; i < NOBJHD ; i++) {
          pyrptr->pyrobj[i]=&objcts[dcdrd((SHORT *)(&(pyrptr->pyrobj[i])))];
     }
}

static SHORT
dcdrd(                        /* utility routine to avoid compile warnings */
SHORT *srcbuf)
{
     return(*srcbuf);
}

static VOID
delfaz(                       /* 'zuul delete-account entry point          */
CHAR *userid)
{
     dfaSetBlk(zuulbb);
     if (dfaAcqEQ(NULL,userid,0)) {
          dfaDelete();
     }
}

static VOID
finzuu(VOID)                  /* finish-up 'zuul entry point for shutdown  */
{
     clsmsg(zuulmb);
     dfaClose(zuulbb);
}
