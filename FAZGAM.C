/*****************************************************************************
 *                                                                           *
 *   FAZGAM.C    (GAMES.C)                                                   *
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
#include "elwfaz.h"
#include "brkthu.h"

static INT dofoon(VOID);
static VOID nwfoon(VOID);
static VOID kcfoon(INT count);
static VOID qtfoon(VOID);
static VOID givprz(struct object *zbjptr,struct state *sptr,CHAR *txt);
static INT isvowel(CHAR chr);
static INT doumfl(VOID);
static VOID ckumdn(INT count);
static VOID qtumfl(VOID);
static INT doosmo(VOID);
static VOID osboom(VOID);
static VOID qtosmo(VOID);
static VOID gogwirg(struct state *gwidst);

static CHAR foonhs[4]={0,0,0,0};
static INT foonon;

CHAR_BUFFER(BALLGOESFLYING, 80, "...POW!  The foon-ball goes flying!\r");

INT
foon(VOID)                    /* special cmd hdlr for Foonball Emporium    */
{
     switch (sltchk(&objcts[0],&objcts[0],&objcts[55],"slot")) {
     case -1:
          if (zbjptr == &objcts[55]) {
               if ((getrnd()&0x7fff)%5 > 1) {
                    jabber(FAZ1507);
                    prfmsg(FAZ1508);
                    mspstg("%s's hands!\r",pyrptr->auxhdl);
                    fazoth();
                    pyrptr->crackl+=3;
               }
               else {
                    prfmsg(FAZ1509);
                    outare(&states[298]);
               }
          }
          else {
               prfmsg(FAZ1500);
               outare(&states[298]);
               states[298].cmhdlr=dofoon;
               states[298].sttmsg=FAZ1506;
               foonhs[0]=foonhs[1]=foonhs[2]=0;
               foonon=1;
               kcfoon(0);
          }
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }

}

static INT
dofoon(VOID)                  /* do foonball game-play command handling    */
{
#define FOONOK 16
#define FOONBD 8
     static CHAR *gfoons[]={"\1\2\3","\2\3\4","\3\4\1","\4\1\5",
                            "\1\5\6","\5\6\7","\6\7\1","\7\1\2",
                            "\3\2\1","\4\3\2","\1\4\3","\5\1\4",
                            "\6\5\1","\7\6\5","\1\7\6","\2\1\7"};
     static CHAR *bfoons[]={"\5\7","\2\4","\1\3","\1\6",
                            "\7\5","\4\2","\3\1","\6\1"};
     INT i;

     if (urgc >= 2 && strcmp(urgv[0],"slap") == 0
      && nmatch(urgv[1],"foon-ball")) {
          btuxmt(pyrctr,BALLGOESFLYING);
          mspstg("***\r%s just slapped the foon-ball!\r",pyrptr->auxhdl);
          fazoth();
          for (i=0 ; i < FOONOK ; i++) {
               if (strcmp(foonhs,gfoons[i]) == 0) {
                    givprz(&objcts[55],&states[298],
                           "materializes in the middle of the floor");
                    prfmsg(FAZ1501);
                    addmbf(pyrptr->auxhdl);
                    prfmsg(FAZ1502);
                    qtfoon();
                    return(0);
               }
          }
          for (i=0 ; i < FOONBD ; i++) {
               if (strcmp(foonhs+1,bfoons[i]) == 0) {
                    prfmsg(FAZ1503);
                    addmbf(pyrptr->auxhdl);
                    prfmsg(FAZ1504);
                    qtfoon();
                    return(0);
               }
          }
          nwfoon();
          return(0);
     }
     return(1);
}

static VOID
nwfoon(VOID)                  /* new foonball location utility             */
{
     INT nwp;
     static CHAR *posnam[]={"","fong","pleebor","oomwazzitz",
                            "neblin","noodiddle","kleng","ormush"};

     foonhs[0]=foonhs[1];
     foonhs[1]=foonhs[2];
     while ((nwp=((getrnd()&0x7fff)%7)+1) == foonhs[0] || nwp == foonhs[1]) {
     }
     foonhs[2]=(CHAR)nwp;
     mspstg("***\rThe foon-ball moves to %s!\r",posnam[nwp]);
     outare(&states[298]);
}

static VOID
kcfoon(                       /* kick foonball around and eventually timeout */
INT count)
{
     if (foonon) {
          if (count < 42) {
               nwfoon();
               zrtkick(7,kcfoon,count+1);
          }
          else {
               prfmsg(FAZ1505);
               qtfoon();
          }
     }
}

static VOID
qtfoon(VOID)                  /* quit foonball                             */
{
     outare(&states[298]);
     states[298].cmhdlr=foon;
     states[298].sttmsg=FAZ298;
     foonon=0;
}

static VOID
givprz(                       /* give foonprize, announce winner           */
struct object * zbjptr_local,
struct state *sptr,
CHAR *txt)
{
     if (sptr->nobjs < NOBJST) {
          mspstg("***\rA %s suddenly ", zbjptr_local->name);
          mspstg("%s!\r",txt);
          outare(sptr);
          insobs(zbjptr_local);
     }
}

#define STUMFL 291
#define STUMFLMSG FAZ291
#define MAXUML 37
#define UMFCCT (sizeof(umfcst)/sizeof(CHAR *))
#define UMFVCT (sizeof(umfvst)/sizeof(CHAR *))
static CHAR *umfcst[]={
     "FLESK","SNAR","NODULE","SHEEN","BEE","NONGA","TROO","WORB",
     "ZATT","FOO","RIBB","SNAG","BOBBLE","NORKER","FLEX","DREA",
     "GOON","BLAST","FREE","NISK","NAPPLE","FISH","SKOO","BIPP",
     "MOSS","SH","TH","DEEDLE","ST","POO","FLISK","NAZZ","BORB"};
static CHAR *umfvst[]={
     "OBBLE","ISPA","EEN","OOM","ADDLE","IZZ","OON","OO","EE",
     "ONGLE","I","IBBI","AGGA","OOTA","IZA","OOTO","AZZU","EEB",
     "ANG","INGA","ORBI","IDD","ULE","UZZ","I","OO","EE","IKKI"};
static CHAR umflwd[MAXUML];
static INT umflon;
static CHAR umflbd[MAXUML];

INT
umflun(VOID)                  /* special cmd hdlr for Hall of Umflungoo    */
{
     INT doumfl();
     INT i,sylcnt;

     switch (sltchk(&objcts[2],&objcts[2],&objcts[2],"hole")) {
     case -1:
          prfmsg(FAZ1510);
          outare(&states[STUMFL]);
          states[STUMFL].cmhdlr=doumfl;
          states[STUMFL].sttmsg=FAZ1511;
          sylcnt=(getrnd()&3)+2;
          strcpy(umflwd,((sylcnt&1) ? umfvst[(getrnd()&0x7fff)%UMFVCT]
                                    : umfcst[(getrnd()&0x7fff)%UMFCCT]));
          for (i=1 ; i < sylcnt ; i++) {
               if (isvowel(umflwd[strlen(umflwd)-1])) {
                    strcat(umflwd,umfcst[(getrnd()&0x7fff)%UMFCCT]);
               }
               else {
                    strcat(umflwd,umfvst[(getrnd()&0x7fff)%UMFVCT]);
               }
          }
          setmem(umflbd,MAXUML,32);
          umflbd[strlen(umflwd)]='\0';
          umflon=1;
          ckumdn(0);
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

static INT
isvowel(                      /* return true if chr is a vowel             */
CHAR chr)
{
     return(chr == 'A' || chr == 'E' || chr == 'I' || chr == 'O' || chr == 'U');
}

static INT
doumfl(VOID)                  /* do umflungoo game play if active          */
{
     INT umflec,i;

     if (urgc >= 2 && strcmp(urgv[0],"yell") == 0) {
          yelrtn();
          if (strcmp(urgv[1],umflwd) == 0) {
               givprz(&objcts[59],&states[STUMFL],
                      "pops out of the hole in the floor");
               prfmsg(FAZ1514);
               mspstg("%s ",umflwd);
               mspstg("%s ",umflwd);
               mspstg("%s",umflwd);
               prfmsg(FAZ1515);
               strcpy(umflbd,pyrptr->auxhdl);
               ucify(umflbd);
               mspstg("%s ",umflbd);
               mspstg("%s ",umflbd);
               mspstg("%s",umflbd);
               prfmsg(FAZ1516);
               qtumfl();
          }
          else {
               for (i=umflec=0 ; urgv[1][i] != 0 && umflwd[i] != 0 ; i++) {
                    if (urgv[1][i] == umflwd[i]) {
                         if (umflbd[i] == 32) {
                              umflbd[i]=umflwd[i];
                              umflec=1;
                         }
                    }
               }
               if (umflec) {
                    prfmsg(FAZ1517);
                    mspstg("%s\r",umflbd);
                    outare(&states[STUMFL]);
               }
          }
          return(0);
     }
     return(1);
}

static VOID
ckumdn(                       /* check if umflungoo is done                */
INT count)
{
     if (umflon) {
          if (count < 120) {
               zrtkick(5,ckumdn,count+1);
          }
          else {
               prfmsg(FAZ1512);
               addmbf(umflwd);
               prfmsg(FAZ1513);
               qtumfl();
          }
     }
}

static VOID
qtumfl(VOID)                  /* quit umflungoo                            */
{
     outare(&states[STUMFL]);
     states[STUMFL].cmhdlr=umflun;
     states[STUMFL].sttmsg=STUMFLMSG;
     umflon=0;
}

static INT osmoon=0;
static struct object *osmobj;
static INT osmcnt=0;

INT
osmo(VOID)                    /* special cmd hdlr for osmotic              */
{
     INT doosmo(VOID),i,j;
     CHAR *mbftmp;

     switch (sltchk(NULL,NULL,NULL,"osmotic")) {
     case -1:
          if ((i=(INT)(zbjptr-objcts)) < 5) {
               j=25;
          }
          else if (i < 15) {
               j=35;
          }
          else if (i < 25) {
               j=44;
          }
          else {
               j=52;
          }
          osmobj=&objcts[(getrnd()&0x7fff)%j];
          prfmsg(FAZ1525);
          mbftmp=prfptr;
          mspstg("%s!\"\r",osmobj->name);
          ucify(mbftmp);
          osboom();
          states[299].cmhdlr=doosmo;
          states[287].sttmsg=FAZ1526;
          osmoon=1;
          osmcnt=0;
     case 0:
          return(0);
     case 1:
     default:
          if (urgc > 1 && strcmp(urgv[1],"how") == 0) {
               prfmsg(FAZ1536);
               osboom();
               osmcnt=0;
          }
          return(1);
     }
}

static INT
doosmo(VOID)                  /* do osmotic game play when active          */
{
     CHAR *mbftmp;

     switch (sltchk(NULL,NULL,NULL,"osmotic")) {
     case -1:
          if (zbjptr == osmobj) {
               prfmsg(FAZ1529);
               mbftmp=prfptr;
               addmbf(pyrptr->auxhdl);
               ucify(mbftmp);
               prfmsg(FAZ1530);
               osboom();
               givprz(&objcts[57],&states[299],
                      "wiggles out from under the osmotic");
               qtosmo();
          }
          else {
               prfmsg(FAZ1531);
               addmbf(pyrptr->auxhdl);
               prfmsg(FAZ1532);
               outare(&states[299]);
               qtosmo();
          }
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

static VOID
osboom(VOID)                  /* boom the osmotic's voice around           */
{
     outare(&states[299]);
     prfmsg(FAZ1527);
     outare(&states[287]);
     prfmsg(FAZ1528);
     outare(&states[304]);
}

VOID
osmwat(VOID)                  /* make osmotic announcement every 45 secs   */
{
     CHAR *mbftmp;

     if ((++osmcnt&15) == 15) {
          if (osmoon) {
               if (osmcnt < 100) {
                    prfmsg(FAZ1533);
                    mbftmp=prfptr;
                    mspstg("%s!\"\r",osmobj->name);
                    ucify(mbftmp);
                    osboom();
               }
               else {
                    prfmsg(FAZ1534);
                    outare(&states[299]);
                    qtosmo();
               }
          }
          else {
               prfmsg(FAZ1535);
               osboom();
          }
     }
     zrtkick(3,(VOID (*)(INT))osmwat,0);
}

static VOID
qtosmo(VOID)                  /* quit osmotic waiting for target object    */
{
     states[299].cmhdlr=osmo;
     states[287].sttmsg=FAZ287;
     osmoon=osmcnt=0;
}

#define nor 0
#define sou 1
#define eas 2
#define wes 3
static CHAR gwirgd[]={
     wes,nor,wes,nor,nor,wes,sou,eas,eas,nor,eas,eas,eas,eas,
     sou,sou,sou,sou,sou,eas,sou,eas,eas,eas,eas,eas,eas,eas,
     eas,nor,nor,nor,nor,nor,nor,eas,nor,nor,wes,wes,wes,wes,
     wes,wes,nor,wes,sou,wes,sou,wes,wes,wes,wes,wes,wes,wes,
     wes,wes,wes,wes,wes,wes,wes,sou,sou,wes,sou,wes,sou,wes,
     wes,wes,wes,wes,wes,wes,nor,nor,nor,nor,eas,sou,eas,sou,
     sou,eas,nor,nor,nor
};
static INT gwirdp=0;
static struct state *gwirgs=&states[286];
static CHAR *dirnms[]={"north","south","east","west"};

VOID
gwirg(VOID)                   /* move gwirg every 5 seconds                */
{
     INT gwidir=0;
     struct state *gwidst;

     (VOID)gwidir;
     if (gwirgs != NULL && sfnobjs("gwirg",gwirgs) != NULL) {
          srmvobs(objidx,gwirgs);
          switch (gwidir=gwirgd[gwirdp]) {
          case nor:
               gwidst=gwirgs->north;
               break;
          case sou:
               gwidst=gwirgs->south;
               break;
          case eas:
               gwidst=gwirgs->east;
               break;
          default: /*case wes:    I made west default because otherwise technically there could be a NULL condition for gwidst - R. Hadsall 4/18/2023 */
               gwidst=gwirgs->west;
               break;
          }
          
          gwirdp=(gwirdp+1)%sizeof(gwirgd);
          prfmsg(FAZ1540);
          mspstg("%s!\r",dirnms[gwidir]);
          outare(gwirgs);
          prfmsg(FAZ1541);
          mspstg("%s!\r",dirnms[gwidir^1]);
          outare(gwidst);
          gogwirg(gwidst);
     }
     zrtkick(5,(VOID (*)(INT))gwirg,0);
}

static VOID
gogwirg(                      /* gwirg movement utility                    */
struct state *gwidst)
{
     if (gwidst->nobjs == NOBJST) {
          zbjptr=gwidst->sttobj[NOBJST-1];
          mspstg("***\rThe gwirg just ate the %s that was",zbjptr->name);
          mspstg(zbjptr->positn,gwidst->sobjlc);
          addmbf("!\r");
          outare(gwidst);
          gwidst->nobjs-=1;
     }
     gwidst->sttobj[gwidst->nobjs++]=&objcts[116];
     gwirgs=gwidst;
}
