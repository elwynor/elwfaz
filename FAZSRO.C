/*****************************************************************************
 *                                                                           *
 *   FAZSRO.C      (STTROU.C)                                                *
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
#include "brkthu.h"
#include "fazdat.h"
#include "elwfaz.h"

static VOID elvdwn(VOID);
static VOID elvup(VOID);
static VOID infelv(INT msgbgn);
static VOID freon(VOID);
static VOID freoff(VOID);
static VOID hallu1(INT pyrc);
static VOID hallu2(INT pyrc);
static VOID hallu3(INT pyrc);
static VOID hallu4(INT pyrc);
static VOID faznul(INT nothing);
static VOID hallu(INT msgno,VOID (*nxtrtp)(INT),INT pyrc);
static INT accspt(struct object *inobj,struct object *outobj,CHAR *devnam);
static INT statue(INT msgno);
static INT cm346(VOID);
static INT xptscm(INT sumstt);
static INT xptrid(INT xptrm);
static VOID xptlvc(VOID);
static VOID xptarr(INT xptdst);
static VOID decgrn(VOID);
static VOID updgbt(VOID);
static INT grin(VOID);
static INT nogrin(VOID);
static VOID doorsu(VOID);
static INT actsw(CHAR *plcstg);
static VOID sprkcc(INT nroom);
static INT saying(VOID);
static VOID norebo(VOID);
static VOID ysrebo(VOID);
static VOID popout(struct object *zbjptr);
static VOID smovie(INT nmovie);
static VOID movie1(INT nmovie);
static INT ltb(INT argno);
static VOID crttlk(INT argno);
static VOID dnytlk(CHAR *dnystg);
static VOID rotcoi(VOID);
static VOID rccoim(VOID);
static VOID bxfull(VOID);
static VOID popbox(struct object *zbjptr);

static INT xptcur=346;
static INT thinkc=0;
static INT stage[3]={0,0,0};
static INT globct=0;
static INT reboon=0;
static INT moviip=0;
static INT chop=0;
static INT bxempt=0;
static INT coicct=0;
static INT lstrbo;
static INT gfrtk=0;
static INT freflw=0;
static INT makbuf=0;
static INT mscmsk=0;

CHAR_BUFFER(POPRIGHTIN, 80, "...It seems to pop right in...\r");
CHAR_BUFFER(POPBACKOUT, 80, "...It pops right back out into your hand!\r");
CHAR_BUFFER(AROUNDAROUND, 80, "...Around and around it goes...\r");
CHAR_BUFFER(HEARCLICK, 80, "...You hear a distinct \"click\".\r");
CHAR_BUFFER(NOTHINGHAPPEN, 80, "...Nothing happens.\r");

VOID
inists(VOID)                  /* initialize state-specific handlers        */
{
     zrtkick(15,(VOID (*)(INT))elvdwn,0);
     zrtkick(1800,(VOID (*)(INT))freon,0);
     osmwat();
     gwirg();
}

static VOID
elvdwn(VOID)                  /* move elevator down                        */
{
     infelv(0);
     states[293].east=&states[335];
     states[293].north=&states[340];
     states[335].west=&states[293];
     states[340].south=&states[293];
     states[292].west=NULL;
     states[295].south=NULL;
     zrtkick(20,(VOID (*)(INT))elvup,0);
}

static VOID
elvup(VOID)                   /* move elevator up                          */
{
     infelv(5);
     states[293].east=&states[292];
     states[293].north=&states[295];
     states[335].west=NULL;
     states[340].south=NULL;
     states[292].west=&states[293];
     states[295].south=&states[293];
     zrtkick(20,(VOID (*)(INT))elvdwn,0);
}

static VOID
infelv(                       /* inform appropriate parties of elevator    */
INT msgbgn)
{
     INT i;
     static INT elvsno[]={293,292,295,335,340,293,292,295,335,340};
     static INT elvstm[]={FAZ293,FAZ450,FAZ451,FAZ452,FAZ453,FAZ293,
                          FAZ292,FAZ295,FAZ335,FAZ340};

     for (i=msgbgn; i < msgbgn+5 ; i++) {
          prfmsg(FAZ900+i);
          outare(&states[elvsno[i]]);
          states[elvsno[i]].sttmsg=elvstm[i];
     }
}

static VOID
freon(VOID)                   /* turn freenish juice gusher on             */
{
     objcts[131].objmsg=FAZ1132;
     freflw=1;
     prfmsg(FAZ461);
     outare(&states[108]);
     zrtkick(60,(VOID (*)(INT))freoff,0);
}

static VOID
freoff(VOID)                  /* turn freenish juice gusher off            */
{
     if (freflw) {
          objcts[131].objmsg=FAZ1131;
          freflw=0;
          prfmsg(FAZ462);
          outare(&states[108]);
          zrtkick(900,(VOID (*)(INT))freon,0);
     }
}

INT
nocmd(VOID)                   /* no command                                */
{
     return(1);
}

INT
cm108(VOID)                   /* special command handler for freenish      */
{
     if (stgcmp(urgv[0],"drink") == 0) {
          if (freflw) {
               jabber(FAZ460);
               infoth("drinking some of the blue fluid");
               zrtkick(60,hallu1,pyrctr);
               freoff();
               return(0);
          }
     }
     return(1);
}

static VOID
hallu1(                       /* hallucination phase 1 after juice drink   */
INT pyrc)
{
     hallu(FAZ1351,hallu2,pyrc);
}

static VOID
hallu2(                       /* hallucination phase 2 after juice drink   */
INT pyrc)
{
     hallu(FAZ1352,hallu3,pyrc);
}

static VOID
hallu3(                       /* hallucination phase 3 after juice drink   */
INT pyrc)
{
     hallu(FAZ1353,hallu4,pyrc);
}

static VOID
hallu4(                       /* hallucination phase 4 after juice drink   */
INT pyrc)
{
     hallu(FAZ1354,faznul,pyrc);
}

static VOID
faznul(INT nothing)                  /* null routine for end of hallu sequence    */
{
    (VOID)nothing;
}

static VOID
hallu(                        /* hallucination handler                     */
INT msgno,
VOID (*nxtrtp)(INT),
INT pyrc)
{
     pyrctr=pyrc;
     pyrptr=&pyrarr[pyrc];
     if (pyrptr->logon&INGAME) {
          jabber(msgno);
          infoth("suddenly staring wide-eyed into space, as though thunderstruck");
          zrtkick(120,nxtrtp,pyrc);
     }
}

INT
cm152(VOID)                   /* no command for mongoosk at the moment     */
{
     return(1);
}

INT
cm77(VOID)                    /* special command handler for platform      */
{
     if (stgcmp(urgv[0],"stand") == 0) {
          jabber(FAZ465);
          infoth("standing transfixed by a vision");
          return(0);
     }
     return(1);
}

INT
cm251(VOID)                   /* oddlewokkus turns zarkon to slosh         */
{
     return(accspt(&objcts[38],&objcts[129],"oddlewokkus"));
}

INT
cm284(VOID)                   /* blub turns slosh to tuskaloosa            */
{
     return(accspt(&objcts[129],&objcts[130],"blub"));
}

INT
cm137(VOID)                   /* nofzinger turns tuskaloosa to gwamp       */
{
     return(accspt(&objcts[130],&objcts[128],"nofzinger"));
}

static INT
accspt(                       /* accept inobj, spit out outobj from devname */
struct object *inobj,
struct object *outobj,
CHAR *devnam)
{
     switch (sltchk(inobj,inobj,inobj,devnam)) {
     case -1:
          mspstg("***\rA %s suddenly materializes ",outobj->name);
          mspstg("inside the %s, and drops out onto the ground!\r",devnam);
          outare(pyrptr->sttptr);
          if (pyrptr->sttptr->nobjs >= NOBJST) {
               pyrptr->sttptr->sttobj[0]=outobj;
          }
          else {
               insobs(outobj);
          }
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

INT
cm104(VOID)                   /* statue gives hint on missing coordinates  */
{
     return(statue(FAZ470));
}

INT
cm109(VOID)                   /* statue gives hint on missing coordinates  */
{
     return(statue(FAZ471));
}

INT
cm113(VOID)                   /* statue gives hint on missing coordinates  */
{
     return(statue(FAZ472));
}

INT
cm116(VOID)                   /* statue gives hint on missing coordinates  */
{
     return(statue(FAZ473));
}

static INT
statue(                       /* statues give hints only when talked around */
INT msgno)
{
     if (saying()) {
          clrprf();
          if ((getrnd()&15) == 0) {
               prfmsg(msgno);
               outare(pyrptr->sttptr);
          }
     }
     return(1);
}

INT
cm193(VOID)                   /* stroke the Orb That Talks to make rebozos */
{
     if (urgc >= 2 && stgcmp(urgv[1],"orb") == 0
      && (stgcmp(urgv[0],"touch") == 0 || stgcmp(urgv[0],"stroke") == 0)) {
          makbuf=1;
          jabber(FAZ1350);
          infoth("acting hypnotized");
          return(0);
     }
     return(1);
}

INT
cm290(VOID)                   /* call transporter via wigglesnort          */
{
     return(xptscm(290));
}

INT
cm330(VOID)                   /* call transporter via wigglesnort          */
{
     return(xptscm(330));
}

static INT
cm346(VOID)                   /* call transporter via wigglesnort          */
{
     return(xptscm(346));
}

INT
cm351(VOID)                   /* transporter itself: eggs control dest     */
{
     switch(sltchk(&objcts[6],&objcts[9],&objcts[29],"slot")) {
     case -1:
          xptlvc();
          if (zbjptr == &objcts[6]) {
               xptarr(346);
          }
          else if (zbjptr == &objcts[9]) {
               xptarr(290);
          }
          else {
               xptarr(330);
          }
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

static INT
xptscm(                       /* summon-transporter utility                */
INT sumstt)
{
     switch (sltchk(&objcts[4],&objcts[4],&objcts[4],"slot")) {
     case -1:
          xptlvc();
          xptarr(sumstt);
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

INT
sltchk(                                 /* drop-xyz-in-slot utility        */
struct object *objps1,
struct object *objps2,
struct object *objps3,
CHAR *apstg)
{
     static struct kwtab drpset[]={
          {"drop",NULL},
          {"feed",NULL},
          {"insert",NULL},
          {"put",NULL},
          {"slip",NULL},
          {"stick",NULL}
     };

     if (urgc >= 4 && fazbin(urgv[0],drpset,6) != NULL
      && (nmatch("in",urgv[2]) || stgcmp("to",urgv[2]) == 0)
      && nmatch(urgv[3],apstg)) {
          if (stgcmp(urgv[1],"hand") == 0 || stgcmp(urgv[1],"dick") == 0) {
               jabber(FAZ719);
               infoth("fooling around a little");
          }
          else if ((zbjptr=fnobjp(urgv[1],pyrptr)) == NULL) {
               donthv();
          }
          else if (objps1 == NULL || zbjptr == objps1
                || zbjptr == objps2 || zbjptr == objps3) {
               btuxmt(pyrctr,POPRIGHTIN);
               mspstg("***\r%s just dropped a ",pyrptr->auxhdl);
               mspstg("%s into the ",zbjptr->name);
               mspstg("%s!\r",apstg);
               fazoth();
               rmvobp(pyrptr,objidx);
               return(-1);
          }
          else {
               btuxmt(pyrctr,POPBACKOUT);
               mspstg("***\r%s just dropped a ",pyrptr->auxhdl);
               mspstg("%s into the ",zbjptr->name);
               mspstg("%s, and it popped back out!\r",apstg);
               fazoth();
          }
          return(0);
     }
     return(1);
}

static INT
xptrid(                       /* return 0, 1, or 2 based on transporter loc */
INT xptrm)
{
     if (xptrm == 290) {
          return(0);
     }
     if (xptrm == 330) {
          return(1);
     }
     return(2);
}

static VOID
xptlvc(VOID)                  /* transporter leaving, let everyone know    */
{
     static INT xptsma[3]={FAZ290,FAZ330,FAZ346};
     INT roomid;

     roomid=xptrid(xptcur);
     prfmsg(FAZ700+roomid);
     outare(&states[xptcur]);
     prfmsg(FAZ710+roomid);
     outare(&states[351]);
     sttptr=&states[xptcur];
     sttptr->sttmsg=xptsma[roomid];
     switch (roomid) {
     case 0:
          sttptr->cmhdlr=cm290;
          sttptr->west=NULL;
          states[351].east=NULL;
          break;
     case 1:
          sttptr->cmhdlr=cm330;
          sttptr->east=NULL;
          states[351].west=NULL;
          break;
     case 2:
          sttptr->cmhdlr=cm346;
          sttptr->north=NULL;
          states[351].south=NULL;
          break;
     }
}

static VOID
xptarr(                       /* transporter arriving, let everyone know   */
INT xptdst)
{
     INT roomid;

     xptcur=xptdst;
     roomid=xptrid(xptcur);
     prfmsg(FAZ705+roomid);
     outare(&states[xptcur]);
     prfmsg(FAZ715+roomid);
     outare(&states[351]);
     sttptr=&states[xptcur];
     sttptr->sttmsg=FAZ400+roomid;
     sttptr->cmhdlr=nocmd;
     switch (roomid) {
     case 0:
          sttptr->west=&states[351];
          states[351].east=sttptr;
          states[351].sttmsg=FAZ405;
          break;
     case 1:
          sttptr->east=&states[351];
          states[351].west=sttptr;
          states[351].sttmsg=FAZ406;
          break;
     case 2:
          sttptr->north=&states[351];
          states[351].south=sttptr;
          states[351].sttmsg=FAZ351;
          break;
     }
}

static VOID
decgrn(VOID)                  /* decrement green of wall with lights if any */
{
     if (thinkc > 0) {
          if (--thinkc < 2) {
               prfmsg(thinkc+FAZ735);
               outare(&states[302]);
               states[302].sttmsg=(thinkc?FAZ437:FAZ302);
               if (thinkc == 1) {
                    prfmsg(FAZ733);
                    outare(&states[318]);
               }
          }
          updgbt();
          if (thinkc > 0) {
               zrtkick(10,(VOID (*)(INT))decgrn,0);
          }
     }
}

INT
cm302(VOID)                   /* special command handler for green lights  */
{
     static INT st302m[4]={FAZ302,FAZ437,FAZ438,FAZ438};

     if (stgcmp(urgv[0],"think") == 0) {
          infoth("concentrating on something ..");
          if (urgc >= 2 && nmatch("green",urgv[1])) {
               jabber(FAZ720+thinkc);
               prfmsg(FAZ725+thinkc);
               fazoth();
               if (thinkc++ == 0) {
                    zrtkick(10,(VOID (*)(INT))decgrn,0);
               }
               if (thinkc > 3) {
                    thinkc=3;
               }
               else if (thinkc == 2) {
                    prfmsg(FAZ732);
                    outare(&states[318]);
               }
               states[302].sttmsg=st302m[thinkc];
          }
          else {
               jabber(FAZ730);
               prfmsg(FAZ731);
               fazoth();
          }
          updgbt();
          return(0);
     }
     return(1);
}

static VOID
updgbt(VOID)                  /* update green-light bit in MSCC mask       */
{
     states[318].sttmsg=FAZ768+(mscmsk=((mscmsk&~1)|(thinkc>1)));
}

INT
cm303(VOID)                   /* special command handler for Hall of Voices */
{
     if (stgcmp(urgv[0],"listen") == 0 || stgcmp(urgv[0],"hear") == 0) {
          infoth("listening for something");
          jabber(FAZ890+((getrnd()&0x7fff)%10));
          return(0);
     }
     return(1);
}

INT
cm306(VOID)                   /* special command handler for roulette wheel */
{
     if (urgc >= 2 && stgcmp(urgv[1],"wheel") == 0
      && (stgcmp(urgv[0],"spin") == 0 || stgcmp(urgv[0],"turn") == 0)) {
          btuxmt(pyrctr,AROUNDAROUND);
          infoth("spinning the wheel");
          prfmsg(FAZ740);
          mspstg("%d!\r",((getrnd()&0x7fff)%36)+1);
          outare(&states[306]);
          return(0);
     }
     return(1);
}

INT
cm307(VOID)                   /* special command handler for grinfrazzitz  */
{
     if (actsw("grinfrazzitz")) {
          btuxmt(pyrctr,HEARCLICK);
          infoth("pressing the button on the grinfrazzitz");
          prfmsg(FAZ745);
          outare(&states[307]);
          prfmsg(FAZ746);
          outare(&states[305]);
          prfmsg(FAZ747);
          outare(&states[308]);
          states[307].sttmsg=FAZ410;
          states[305].sttmsg=FAZ411;
          states[308].sttmsg=FAZ412;
          states[307].south=NULL;
          states[307].west=NULL;
          states[305].east=NULL;
          states[308].north=NULL;
          states[307].cmhdlr=grin;
          if (gfrtk == 0) {
               gfrtk=1;
               zrtkick(120,(VOID (*)(INT))doorsu,0);
          }
          return(0);
     }
     return(1);
}

static INT
grin(VOID)                    /* stage 2 of grinfr. operation (drop stuff) */
{
     INT nomrfl;

     if (actsw("grinfrazzitz")) {
          btuxmt(pyrctr,HEARCLICK);
          infoth("pressing the button on the grinfrazzitz again");
          prfmsg(FAZ748);
          outare(&states[307]);
          prfmsg(FAZ749);
          outare(&states[305]);
          outare(&states[308]);
          othpyr=pyrptr;
          pyrptr=othpyr->sttptr->plhead;
          nomrfl=1;
          while (sttptr->nobjs < NOBJST) {
               if (pyrptr->nobjs != 0) {
                    mspstg("***\rYour %s drops out of your hands!\r",
                           pyrptr->pyrobj[0]->name);
                    outprf(pyrptr->portno);
                    mspstg("***\r%s has just dropped a ",pyrptr->auxhdl);
                    mspstg("%s!\r",pyrptr->pyrobj[0]->name);
                    fazoth();
                    insobs(pyrptr->pyrobj[0]);
                    rmvobp(pyrptr,0);
                    nomrfl=0;
               }
               if ((pyrptr=pyrptr->nxtpyr) == NULL) {
                    if (nomrfl != 0) {
                         break;
                    }
                    else {
                         pyrptr=othpyr->sttptr->plhead;
                         nomrfl=1;
                    }
               }
          }
          pyrptr=othpyr;
          states[307].cmhdlr=nogrin;
          return(0);
     }
     return(1);
}

static INT
nogrin(VOID)                  /* stage 3 of grinfr. operation (all clear)  */
{
     if (actsw("grinfrazzitz")) {
          btuxmt(pyrctr,HEARCLICK);
          infoth("pressing the button on the grinfrazzitz");
          doorsu();
          return(0);
     }
     return(1);
}

static VOID
doorsu(VOID)                  /* grinfrazzitz doors-up utility             */
{
     if (gfrtk != 0) {
          gfrtk=0;
          prfmsg(FAZ750);
          outare(&states[307]);
          prfmsg(FAZ751);
          outare(&states[305]);
          prfmsg(FAZ752);
          outare(&states[308]);
          states[307].sttmsg=FAZ307;
          states[305].sttmsg=FAZ305;
          states[308].sttmsg=FAZ308;
          states[307].south=&states[308];
          states[307].west=&states[305];
          states[305].east=&states[307];
          states[308].north=&states[307];
          states[307].cmhdlr=cm307;
     }
}

static INT
actsw(                        /* activate-switch checker                   */
CHAR *plcstg)
{
     static struct kwtab pshset[]={
          {"flip",NULL},
          {"hit",NULL},
          {"press",NULL},
          {"push",NULL},
          {"turn",NULL}};

     if (urgc >= 2 && fazbin(urgv[0],pshset,5) != NULL
      && (nmatch(urgv[1],"button") || nmatch(urgv[1],"switch"))) {
          if (urgc == 2
           || (urgc == 4 && stgcmp(urgv[2],"on") == 0
            && nmatch(urgv[3],plcstg))) {
               return(1);
          }
     }
     return(0);
}

INT
cm310(VOID)                   /* special command handler for spark room 1  */
{
     sprkcc(0);
     return(1);
}

INT
cm311(VOID)                   /* special command handler for spark room 2  */
{
     sprkcc(1);
     return(1);
}

INT
cm312(VOID)                   /* special command handler for spark room 3  */
{
     sprkcc(2);
     return(1);
}

static VOID
sprkcc(                       /* dance to advance spark colors             */
INT nroom)
{
     INT fizz;

     if (urgc > 0 && stgcmp(urgv[0],"dance") == 0) {
          prfmsg(FAZ755+(stage[nroom]=(stage[nroom]+1)&3));
          outare(&states[310+nroom]);
     }
     if (stage[0]+stage[1]+stage[2] == 9
      && states[310].plhead != 0
      && states[311].plhead != 0
      && states[312].plhead != 0) {
          fizz=2;
     }
     else {
          fizz=0;
     }
     if (((fizz^mscmsk)&2) != 0) {
          states[318].sttmsg=FAZ768+(mscmsk=((mscmsk&~2)|fizz));
          prfmsg(fizz?FAZ760:FAZ761);
          outare(&states[318]);
     }
}

INT
cm318(VOID)                   /* special command handler for MSCC          */
{
     CHAR savbyt;

     if (saying()) {
          savbyt=prfbuf[0];             /* outare calls damaging clrprf    */
          outare(&states[321]);
          prfbuf[0]=savbyt;             /* restore damage from clrprf      */
          outare(&states[345]);
     }
     else if (actsw("panel")) {
          infoth("pressing the button on the panel");
          if (mscmsk == 0x0F) {
               if ((othpyr=states[319].plhead) != NULL
                && othpyr->nxtpyr == NULL && othpyr->nobjs == 1
                && hldoth(&objcts[117])) {
                    jabber(FAZ792);
                    prfmsg(FAZ791);
                    outare(&states[318]);
                    btupmt(states[319].plhead->portno,0);
                    prfmsg(FAZ790);
                    outare(&states[319]);
                    states[319].plhead->sttptr=&states[352];
                    states[319].plhead=NULL;
               }
               else {
                    jabber(FAZ785);
                    prfmsg(FAZ786);
                    outare(&states[318]);
                    prfmsg(FAZ787);
                    outare(&states[319]);
               }
               rotcoi();
               norebo();
          }
          else {
               btuxmt(pyrctr,NOTHINGHAPPEN);
          }
          return(0);
     }
     return(1);
}

INT
cm352(VOID)                   /* game win test after MSTC activation       */
{
     INT coor1,coor2,coor3;

     btupmt(pyrctr,'>');
     coor1=coor2=coor3=0;
     if (urgc == 3 && lstrbo == 0) {
          (VOID)sscanf(urgv[0],"%d",&coor1);
          (VOID)sscanf(urgv[1],"%d",&coor2);
          (VOID)sscanf(urgv[2],"%d",&coor3);
          if (coor1 == 4428 && coor2 == 109 && coor3 == 6257) {
               jabber(FAZ793);
               shocst("ELW Fazuul Winner!","FAZUUL WINNER: %s",usaptr->userid);
          }
          else {
               jabber(FAZ794);
               pyrptr->nobjs=0;
          }
     }
     else {
          jabber(FAZ794);
          pyrptr->nobjs=0;
     }
     ntrstt(&states[getrnd()&3],"appeared, with a weird looking smile");
     return(0);
}

static INT
saying(VOID)                  /* idle talk transmit utility                */
{
     if (stgcmp(urgv[0],"say") == 0 || stgcmp(urgv[0],"yell") == 0) {
          recomb();
          addmbf("***\rA voice fills the room:");
          addmbf(arge[1]);
          addmbf("\r");
          *arge[1]=' ';
          prscmd();
          return(1);
     }
     return(0);
}

INT
cm321(VOID)                   /* special cmd handler for globe/hopper panel */
{
     if (saying()) {
          outare(&states[318]);
     }
     else if (actsw("console")) {
          infoth("pressing the button on the console");
          if (reboon && globct == 5) {
               if (mscmsk&4) {
                    norebo();
               }
               else {
                    jabber(FAZ800);
                    prfmsg(FAZ801);
                    outare(&states[321]);
                    prfmsg(FAZ802);
                    outare(&states[320]);
                    prfmsg(FAZ803);
                    outare(&states[322]);
                    states[321].sttmsg=FAZ415;
                    states[320].sttmsg=FAZ416;
                    states[322].sttmsg=FAZ417;
                    states[318].sttmsg=FAZ768+(mscmsk|=4);
                    prfmsg(FAZ810);
                    outare(&states[318]);
               }
          }
          else {
               jabber(FAZ805);
          }
          return(0);
     }
     return(1);
}

INT
cm320(VOID)                   /* special command handler for golden hopper */
{
     switch (sltchk(&objcts[128],&objcts[129],&objcts[130],"hopper")) {
     case -1:
          if (zbjptr == &objcts[128]) {
               lstrbo=0;
          }
          else if (zbjptr == &objcts[129]) {
               lstrbo=1;
          }
          else {
               lstrbo=2;
          }
          ysrebo();
          return(0);
     case 0:
          norebo();
          return(0);
     case 1:
     default:
          return(1);
     }
}

static VOID
norebo(VOID)                  /* turn off active status of golden hopper   */
{
     if (reboon) {
          if (mscmsk&4) {
               states[318].sttmsg=FAZ768+(mscmsk&=~4);
               prfmsg(FAZ811);
               outare(&states[318]);
               prfmsg(FAZ807);
               outare(&states[321]);
               prfmsg(FAZ808);
               outare(&states[320]);
               prfmsg(FAZ809);
               outare(&states[322]);
          }
          reboon=0;
          prfmsg(FAZ813);
          outare(&states[321]);
          states[321].sttmsg=((globct==5)?FAZ421:FAZ321);
          states[320].sttmsg=FAZ320;
          states[322].sttmsg=FAZ322;
     }
}

static VOID
ysrebo(VOID)                  /* turn on active status of golden hopper    */
{
     if (reboon == 0) {
          reboon=1;
          prfmsg(FAZ812);
          outare(&states[321]);
          states[321].sttmsg=((globct==5)?FAZ422:FAZ420);
     }
}

INT
cm322(VOID)                   /* special command handler for globe buttons */
{
     INT butnum=0;

     if (urgc == 3 && nmatch(urgv[2],"button")
      && (stgcmp(urgv[0],"push") == 0 || stgcmp(urgv[0],"press") == 0
       || stgcmp(urgv[0],"hit") == 0)) {
          if (stgcmp(urgv[1],"blue") == 0
           || (++butnum && stgcmp(urgv[1],"green") == 0)
           || (++butnum && stgcmp(urgv[1],"purple") == 0)
           || (++butnum && stgcmp(urgv[1],"red") == 0)
           || (++butnum && stgcmp(urgv[1],"yellow") == 0)) {
               infoth("pushing one of the buttons");
               jabber(FAZ815);
               if (butnum == globct) {
                    prfmsg(FAZ816);
                    outare(&states[322]);
                    prfmsg(FAZ820+globct);
                    outare(&states[323]);
                    states[323].sttmsg=FAZ425+globct;
                    globct+=1;
                    if (globct == 5) {
                         prfmsg(FAZ826);
                         outare(&states[321]);
                         states[321].sttmsg=(reboon?FAZ422:FAZ421);
                    }
               }
               else if (globct != 0) {
                    prfmsg(FAZ816);
                    outare(&states[322]);
                    prfmsg(FAZ825);
                    outare(&states[323]);
                    states[323].sttmsg=FAZ323;
                    if (globct == 5) {
                         prfmsg(FAZ827);
                         outare(&states[321]);
                         states[321].sttmsg=(reboon?FAZ420:FAZ321);
                    }
                    globct=0;
                    norebo();
               }
          }
          else {
               jabber(FAZ817);
               infoth("seeing things");
          }
          return(0);
     }
     return(1);
}

INT
cm331(VOID)                   /* special command handler for granfalloon   */
{
     switch(sltchk(&objcts[2],&objcts[12],&objcts[23],"slot")) {
     case -1:
          if (zbjptr == &objcts[2]) {
               popout(&objcts[18]);
          }
          else if (zbjptr == &objcts[12]) {
               popout(&objcts[31]);
          }
          else {
               popout(&objcts[52]);
          }
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

static VOID
popout(                       /* granfalloon ticket-maker utility          */
struct object *zbjptr_local)
{
     mspstg("***\rA %s suddenly materializes in front of the granfalloon!\r",
         zbjptr_local->name);
     outare(&states[331]);
     if (states[331].nobjs >= NOBJST) {
          states[331].sttobj[0]= zbjptr_local;
     }
     else {
          insobs(zbjptr_local);
     }
}

INT
cm333(VOID)                   /* special command handler for ribbenfratz   */
{
     if (moviip) {
          return(sltchk(&objcts[TOTOBJ-1],&objcts[TOTOBJ-1],
                        &objcts[TOTOBJ-1],"slot"));
     }
     switch(sltchk(&objcts[18],&objcts[31],&objcts[52],"slot")) {
     case -1:
          if (zbjptr == &objcts[18]) {
               smovie(1);
          }
          else if (zbjptr == &objcts[31]) {
               smovie(2);
          }
          else {
               smovie(3);
          }
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

static VOID
smovie(                       /* show-movie utility                        */
INT nmovie)
{
     moviip=nmovie;
     prfmsg(FAZ830);
     outare(&states[332]);
     prfmsg(FAZ831);
     outare(&states[331]);
     prfmsg(FAZ832);
     outare(&states[330]);
     prfmsg(FAZ833);
     outare(&states[333]);
     zrtkick(10,movie1,0);
}

static VOID
movie1(                       /* ongoing movie manager and terminator      */
INT nmovie)
{
     static INT whmvst[4]={0,FAZ835,FAZ840,FAZ845};

     if (nmovie == 4) {
          prfmsg(FAZ850);
          outare(&states[332]);
          prfmsg(FAZ851);
          outare(&states[331]);
          prfmsg(FAZ852);
          outare(&states[330]);
          prfmsg(FAZ853);
          outare(&states[333]);
          moviip=0;
     }
     else {
          prfmsg(whmvst[moviip]+nmovie);
          outare(&states[332]);
          zrtkick(20,movie1,++nmovie);
     }
}

INT
cm334(VOID)                   /* special command handler for limbo         */
{
     zapwrd("an");
/*
     if (urgc >= 2 && stgcmp(urgv[0],"create") == 0) {
          return(ltb(1));
     }
*/
     if (urgc >= 4 && stgcmp(urgv[0],"let") == 0
       && stgcmp(urgv[1],"there") == 0 && stgcmp(urgv[2],"be") == 0) {
          return(ltb(3));
     }
     return(1);
}

static INT
ltb(                          /* "let there be xyz" utility                */
INT argno)
{
     INT i;

     if (pyrptr->sttptr->nobjs < NOBJST) {
          for (i=0,zbjptr=objcts ; i < 69 ; i++,zbjptr++) {
               if (stgcmp(urgv[argno],zbjptr->name) == 0) {
                    crttlk(argno);
                    insobs(zbjptr);
                    return(0);
               }
          }
     }
     crttlk(argno);
     dnytlk(urgv[argno]);
     return(0);
}

static VOID
crttlk(                       /* create an arbitrary string in limbo       */
INT argno)
{
     recomb();
     mspstg("...And there was%s!\r",arge[argno]);
     gibber();
     mspstg("***\r%s has just created",pyrptr->auxhdl);
     mspstg("%s!\r",arge[argno]);
     fazoth();
}

static VOID
dnytlk(                       /* deny a non-object string in limbo         */
CHAR *dnystg)
{
     CHAR tmpstg[256];

     strcpy(tmpstg,dnystg);
     ucify(tmpstg);
     addmbf("***\rYou suddenly hear the booming voice of a higher authority:\r");
     mspstg("   \"WHERE'S MY %s?\"\r",tmpstg);
     mspstg("***\rThe %s just vanished again!\r",dnystg);
     gibber();
     fazoth();
}

INT
cm336(VOID)                   /* special cmd hdlr for user-to-zleen converter */
{
     if (actsw("wall")) {
          if (states[336].sttmsg == FAZ336) {
               prfmsg(FAZ855);
               outare(&states[336]);
               states[336].sttmsg=FAZ435;
          }
          else {
               prfmsg(FAZ856);
               outare(&states[336]);
               states[336].sttmsg=FAZ336;
          }
          return(0);
     }
     switch(sltchk(&objcts[57],&objcts[57],&objcts[57],"chute")) {
     case -1:
          if (iszleen(pyrptr)) {
               prfmsg(FAZ1550);
               outare(&states[336]);
               prfmsg(FAZ1551);
               outnis();
          }
          else {
               jabber(FAZ1552);
               if (invis(pyrptr)) {
                    strcpy(pyrptr->auxhdl,"Some unseen zleen");
               }
               else {
                    mspstg("***\r%s",pyrptr->auxhdl);
                    prfmsg(FAZ1553);
                    strcat(pyrptr->auxhdl,"'s zleen");
                    mspstg(" %s!\r",pyrptr->auxhdl);
                    fazoth();
               }
          }
     case 0:
          return(0);
     case 1:
     default:
          return(1);
     }
}

INT
cm337(VOID)                   /* special cmd hdlr for mongoo zapper room   */
{
     if (holdng(&objcts[2])) {
          jabber(FAZ858);
          addmbf("***\rOne of the floating globules just touched\r");
          addmbf(pyrptr->auxhdl);
          addmbf("'s mongoo, and it vanished in a cloud of smoke!\r");
          fazoth();
          rmvobp(pyrptr,objidx);
     }
     return(1);
}

INT
cm338(VOID)                   /* special cmd hdlr for object evaporator    */
{
     if (pyrptr->nobjs >= 3 && (((pyrptr->pyrobj[2])-objcts)&1)) {
          mspstg("***\rYour %s just evaporated!\r",pyrptr->pyrobj[2]->name);
          gibber();
          mspstg("***\r%s's ",pyrptr->auxhdl);
          mspstg("%s just evaporated!\r",pyrptr->pyrobj[2]->name);
          fazoth();
          rmvobp(pyrptr,2);
     }
     return(1);
}

INT
cm343(VOID)                   /* special cmd hdlr for hopper by coils      */
{
     static INT chp[5]={1,10,16,40,TOTOBJ};

     switch(sltchk(&objcts[chp[chop]],
          &objcts[chp[chop]],&objcts[chp[chop]],"hopper")) {
     case -1:
          coicct=10;
          rotcoi();
     case 0:
          return(0);
     case 1:
     default:
          if (coicct != 0 && --coicct == 0 && chop != 0) {
               if (chop == 4) {
                    rotcoi();
               }
               else {
                    chop=4;
                    rccoim();
                    chop=0;
                    states[345].sttmsg=FAZ345;
               }
          }
          return(1);
     }
}

static VOID
rotcoi(VOID)                  /* "rotate" coil readiness level             */
{
     rccoim();
     if (++chop == 5) {
          states[345].sttmsg=FAZ345;
          prfmsg(FAZ877);
          outare(&states[345]);
          chop=0;
          if (mscmsk&8) {
               states[318].sttmsg=FAZ768+(mscmsk&=~8);
               prfmsg(FAZ876);
               outare(&states[318]);
          }
     }
     else if (chop == 4) {
          states[345].sttmsg=FAZ444;
          prfmsg(FAZ874);
          outare(&states[345]);
     }
     else {
          states[345].sttmsg=FAZ443;
     }
}

static VOID
rccoim(VOID)                  /* let everybody know how the coils are doing */
{
     static INT msgbas[5]={FAZ860,FAZ865,FAZ865,FAZ865,FAZ870};

     prfmsg(msgbas[chop]);
     outare(&states[341]);
     states[341].sttmsg=((chop==4)?FAZ341:FAZ442);
     prfmsg(msgbas[chop]+1);
     outare(&states[343]);
     states[343].sttmsg=((chop==4)?FAZ343:FAZ441);
     prfmsg(msgbas[chop]+3);
     outare(&states[342]);
     states[342].sttmsg=((chop==4)?FAZ342:FAZ440);
     prfmsg(msgbas[chop]+2);
     outare(&states[345]);
}

INT
cm345(VOID)                   /* special cmd hdlr for coil control panel   */
{
     if (saying()) {
          outare(&states[318]);
     }
     else if (actsw("console")) {
          infoth("pushing the button on the console");
          if (chop == 4) {
               if ((mscmsk&8) == 0) {
                    jabber(FAZ878);
                    prfmsg(FAZ879);
                    outare(&states[345]);
                    states[318].sttmsg=FAZ768+(mscmsk|=8);
                    prfmsg(FAZ875);
                    outare(&states[318]);
               }
               else {
                    jabber(FAZ880);
               }
          }
          else {
               jabber(FAZ881);
          }
          return(0);
     }
     return(1);
}

static VOID
bxfull(VOID)                  /* make the traset/rebozo box full again     */
{
     bxempt=0;
}

INT
cm349(VOID)                   /* special cmd hdlr for traset/rebozo box    */
{
     if (urgc >= 2 && stgcmp("open",urgv[0]) == 0
      && (stgcmp("box",urgv[1]) == 0 || stgcmp("door",urgv[1]) == 0)) {
          infoth("opening the box");
          if (pyrptr->sttptr->nobjs < NOBJST && bxempt == 0) {
               insobs(&objcts[makbuf ? 61 : 21]);
               jabber(makbuf ? FAZ888 : FAZ885);
               prfmsg(makbuf ? FAZ889 : FAZ886);
               fazoth();
               makbuf=0;
               bxempt=1;
               zrtkick(30,(VOID (*)(INT))bxfull,0);
          }
          else {
               jabber(FAZ887);
          }
          return(0);
     }
     switch(sltchk(&objcts[25],&objcts[24],&objcts[18],"box")) {
     case -1:
          if (zbjptr == &objcts[25]) {
               popbox(&objcts[24]);
          }
          else if (zbjptr == &objcts[24]) {
               popbox(&objcts[18]);
          }
          else {
               popbox(&objcts[25]);
          }
     case 0:
          return(0);
     case 1:
          return(1);
     }
     return(1);
}

static VOID
popbox(                       /* rotate ravus, rommus, ruckus via box      */
struct object * zbjptr_local)
{
     mspstg("***\rA %s suddenly pops out of the box!\r", zbjptr_local->name);
     outare(&states[349]);
     if (states[349].nobjs >= NOBJST) {
          states[349].sttobj[0]= zbjptr_local;
     }
     else {
          insobs(zbjptr_local);
     }
}
