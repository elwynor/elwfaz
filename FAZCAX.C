/*****************************************************************************
 *                                                                           *
 *   FAZCAX.C     (CMDAUX.C)                                                 *
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

static VOID sayhmc(VOID);
static VOID saytoa(struct state *sttptr,struct object *zbjptr);
static VOID zlesub(INT begin);
static VOID takutl(VOID);
static VOID grbutl(INT whofrm,INT whtgrb);
static SHORT getnch(SHORT hwmany);
static VOID givcrk(SHORT whoto, SHORT hwmany, SHORT whtnam);
static VOID givobj(INT whoto,INT whtgiv);
static VOID jstutl(VOID);

CHAR_BUFFER(CONSIDERSAID, 80, "...Consider it said.\r");
CHAR_BUFFER(WANTTOSAY, 80, "...Did you want to say something?\r");
CHAR_BUFFER(LOUDVOICEYOUHAVE, 80, "...That's a loud voice you have there.\r");
CHAR_BUFFER(WHISPERSELF, 80, "...Whispering to yourself, are you?\r");
CHAR_BUFFER(HANDSFULLALREADY, 80, "...You can't, your hands are full already.\r");
CHAR_BUFFER(CANTITSPUBLIC, 80, "...You can't, it's public property.\r");
CHAR_BUFFER(ALREADYHOLDINGIT, 80, "...You're already holding it!\r");
CHAR_BUFFER(NICETRYNODICE, 80, "...Nice try, but no dice.\r");
CHAR_BUFFER(YOUGOT, 80, "...You got it!\r");
CHAR_BUFFER(NOTHOLDINGANYTHING, 80, "...You are not holding anything.\r");
CHAR_BUFFER(NOTVALIDCRACKERCOUNT, 80, "...That is not a valid cracker count.\r");
CHAR_BUFFER(HAVETOKEEPACRACKER, 80, "...You can't, you have to keep a cracker.\r");
CHAR_BUFFER(CONSIDERDONE, 80, "...Consider it done.\r");
CHAR_BUFFER(MYSTERIOUSFORCE, 80, "...A mysterious force field seems to prevent this.\r");

INT
diertn(VOID)                  /* "die now" command handler                 */
{
     if (urgc != 2 || stgcmp(urgv[1],"now")) {
          jabber(FAZ1817);
          infoth("contemplating hari-kari");
     }
     else {
          strcpy(pyrptr->auxhdl,pyrptr->plyrid);
          jabber(FAZ1818);
          lvstt("disintegrated away into a scintillating cloud of sparkling mist");
          ntrstt(&states[1],"appeared suddenly out of a scintillating cloud of sparkling mist");
          pyrptr->nobjs=0;
     }
     return(0);
}

INT
crflun(VOID)                  /* "crack" and "crack flungoon" command handler */
{
     INT ncrcks;
     static CHAR flucrk[]={2,2,2,2,2,2,2,2,2,5,5,12};

     if (urgc == 1) {
          return(crkrtn());
     }
     if (fnobjp(urgv[1],pyrptr) == &objcts[59]) {
          ncrcks=flucrk[(getrnd()&0x7fff)%sizeof(flucrk)];
          mspstg("...You find %d crackers inside!\r",ncrcks);
          gibber();
          infoth("cracking %s flungoon open");
          rmvobp(pyrptr,objidx);
          pyrptr->crackl+=(SHORT)ncrcks;
          return(0);
     }
     else {
          return(ntnice());
     }
}

INT
crkrtn(VOID)                  /* "crack" and "crackers?" command handler   */
{
     sayhmc();
     infoth("checking %s cracker supply");
     return(0);
}

static VOID
sayhmc(VOID)                  /* cracker count checking utility            */
{
     mspstg("...You have %d crackers.\r",pyrptr->crackl);
     gibber();
}

INT
sayrtn(VOID)                  /* saying and speaking command handler       */
{
     struct state *pyrstp;

     if (urgc > 1) {
          if (pfnlvl > 1) {
               jabber(FAZ1801);
               infoth("sputtering and fuming at you");
          }
          else {
               if (hold(&objcts[35])) {
                    if (urgc > 3 && stgcmp(urgv[1],"to") == 0) {
                         if ((pyrstp=fndpyr(urgv[2])) != NULL) {
                              saytoa(pyrstp,&objcts[35]);
                              return(0);
                         }
                    }
               }
               btuxmt(pyrctr,CONSIDERSAID);
               mspstg("***\r%s ",pyrptr->auxhdl);
               mspstg("%ss:",urgv[0]);
               recomb();
               zlesub(1);
               mspstg("%s\r",arge[1]);
               fazoth();
               addmbf("***\rYou hear a voice nearby.\r");
               outner();
          }
     }
     else {
          btuxmt(pyrctr,WANTTOSAY);
          infoth("speechless");
     }
     return(0);
}

static VOID
saytoa(                       /* say to another command handler            */
struct state *local_sttptr,
struct object *local_zbjptr)
{
     if (!invis(pyrptr)) {
          mspstg("...Your %s glows brightly for a moment!\r",local_zbjptr->name);
          gibber();
          mspstg("***\r%s's ",pyrptr->auxhdl);
          mspstg("%s glows brightly for a moment!\r",local_zbjptr->name);
          fazoth();
     }
     else {
          mspstg("...Your %s glows dimly for a moment!\r",local_zbjptr->name);
          gibber();
     }
     addmbf("***\rYou hear a voice that seems to come from all directions:\r");
     recomb();
     zlesub(3);
     addmbf(arge[3]+1);
     addmbf("\r");
     outare(local_sttptr);
}

INT
yelrtn(VOID)                  /* yelling and hollering command handler     */
{
     CHAR savbyt;

     if (urgc > 1) {
          if (pfnlvl > 1) {
               jabber(FAZ1801);
               infoth("turning bright purple with suppressed rage");
          }
          else {
               btuxmt(pyrctr, LOUDVOICEYOUHAVE);
               mspstg("***\rYou hear someone %sing in the distance.\r",urgv[0]);
               fazfar();
               mspstg("***\rYou hear %s ",pyrptr->auxhdl);
               mspstg("%sing:",urgv[0]);
               recomb();
               zlesub(1);
               ucify(urgv[1]);
               mspstg("%s!\r",arge[1]);
               savbyt=prfbuf[0];             /* fazoth does damaging clrprf */
               fazoth();
               prfbuf[0]=savbyt;             /* restore byte from clrprf   */
               outner();
          }
     }
     else {
          mspstg("***\r%s is ",pyrptr->auxhdl);
          mspstg("%sing at the top of ",urgv[0]);
          mspstg("%s lungs!\r",zhisher(pyrptr));
          fazoth();
          mspstg("***\rYou hear someone %sing in the distance.\r",urgv[0]);
          savbyt=prfbuf[0];                  /* fazfar does damaging clrprf*/
          fazfar();
          prfbuf[0]=savbyt;                  /* restore byte from clrprf   */
          outner();
          *urgv[0]=(CHAR)toupper((UCHAR)*urgv[0]);
          mspstg("...%s!\r",urgv[0]);
          gibber();
     }
     return(0);
}

INT
whsrtn(VOID)                  /* whispering and muttering command handler  */
{
     INT srtwhs,stpwhs,whoto;

     if (urgc > 3) {
          if (pfnlvl > 1) {
               jabber(FAZ1801);
               infoth("muttering foul oaths under %s breath");
          }
          else {
               if (stgcmp(urgv[1],"to") == 0) {
                    srtwhs=3;
                    stpwhs=urgc;
                    whoto=2;
               }
               else if (stgcmp(urgv[urgc-2],"to") == 0) {
                    srtwhs=1;
                    stpwhs=urgc-2;
                    whoto=urgc-1;
               }
               else {
                    btuxmt(pyrctr,WHISPERSELF);
                    infoth("whispering silly things under %s breath");
                    return(0);
               }
               if ((othpyr=fndhdl(urgv[whoto])) == NULL) {
                    ainthr(whoto);
               }
               else {
                    mspstg("...%s hears you.\r",othpyr->auxhdl);
                    gibber();
                    mspstg("***\r%s is ",pyrptr->auxhdl);
                    mspstg("%sing something to ",urgv[0]);
                    mspstg("%s.\r",othpyr->auxhdl);
                    outexc(othpyr);
                    mspstg("***\r%s ",pyrptr->auxhdl);
                    mspstg("%ss to you:",urgv[0]);
                    recomb();
                    *arge[stpwhs]='\0';
                    urgc=stpwhs;
                    zlesub(srtwhs);
                    mspstg("%s\r",arge[srtwhs]);
                    outpyr(othpyr);
               }
          }
          return(0);
     }
     else {
          return(1);
     }
}

static VOID
zlesub(                       /* special zleen talk garbler                */
INT begin)
{
     INT rndbts,i,j,walk,sublen,chg,remain;
     CHAR *substg;

     if (iszleen(pyrptr)) {
          rndbts=getrnd();
          for (i=begin,walk=1 ; i < urgc ; i++,walk<<=1) {
               if (rndbts&walk) {
                    if (rndbts&(walk<<=1)) {
                         substg="(snort)";
                         sublen=7;
                    }
                    else {
                         substg="(snuffle)";
                         sublen=9;
                    }
                    chg=sublen-(INT)((arge[i+1]-urgv[i]));
                    if (strlen(pyrcmd)+chg < 200) {
                         remain=strlen(arge[i+1])+1;
                         movmem(arge[i+1],urgv[i],remain);
                         movmem(urgv[i],urgv[i]+sublen,remain);
                         movmem(substg,urgv[i],sublen);
                         arge[i+1]+=chg;
                         for (j=i+1 ; j < urgc ; j++) {
                              urgv[j]+=chg;
                              arge[j+1]+=chg;
                         }
                    }
               }
          }
     }
}

INT
moven(VOID)                   /* move north command handler                */
{
     if (iszleen(pyrptr)) {
          return(move(pyrptr->sttptr->north,"wiggled away, to the north",
                      "wiggled up from the south"));
     }
     return(move(pyrptr->sttptr->north,"moved away, to the north",
                 "appeared, from the south"));
}

INT
moves(VOID)                   /* move south command handler                */
{
     if (iszleen(pyrptr)) {
          return(move(pyrptr->sttptr->south,"wiggled away, to the south",
                      "wiggled down from the north"));
     }
     return(move(pyrptr->sttptr->south,"moved away, to the south",
                 "appeared, from the north"));
}

INT
movee(VOID)                   /* move east command handler                 */
{
     if (iszleen(pyrptr)) {
          return(move(pyrptr->sttptr->east,"wiggled away, to the east",
                      "wiggled over from the west"));
     }
     return(move(pyrptr->sttptr->east,"moved away, to the east",
                 "appeared, from the west"));
}

INT
movew(VOID)                   /* move west command handler                 */
{
     if (iszleen(pyrptr)) {
          return(move(pyrptr->sttptr->west,"wiggled away, to the west",
                      "wiggled over from the east"));
     }
     return(move(pyrptr->sttptr->west,"moved away, to the west",
                 "appeared, from the east"));
}

INT
shoven(VOID)                  /* shove someone north command handler       */
{
     return(shove(pyrptr->sttptr->north,"north","crashed in from the south"));
}

INT
shoves(VOID)                  /* shove someone south command handler       */
{
     return(shove(pyrptr->sttptr->south,"south","stumbled in from the north"));
}

INT
shovee(VOID)                  /* shove someone east command handler        */
{
     return(shove(pyrptr->sttptr->east,"east","been thrown in, from the west"));
}

INT
shovew(VOID)                  /* shove someone west command handler        */
{
     return(shove(pyrptr->sttptr->west,"west","been shoved over, from the east"));
}

INT
takrtn(VOID)                  /* "take" and "grab" command handler         */
{
     static struct kwtab pubset[6]={
          {"air",NULL},
          {"button",NULL},
          {"ceiling",NULL},
          {"floor",NULL},
          {"dirt",NULL},
          {"wall",NULL}};
     struct kwtab *fazbin();

     if (urgc == 3
      && (stgcmp(urgv[2],"apart") == 0 || stgcmp(urgv[1],"apart") == 0)) {
          return(zdsass());
     }
     zapwrd("up");
     if (pyrptr->nobjs >= NOBJHD) {
          btuxmt(pyrctr,HANDSFULLALREADY);
          infoth("looking overburdened");
     }
     else if (pyrptr->sttptr->nobjs == 1
           && (urgc == 1 || stgcmp(urgv[1],"it") == 0)) {
          objidx=0;
          zbjptr=pyrptr->sttptr->sttobj[0];
          takutl();
     }
     else if (urgc == 2) {
          if ((zbjptr=fnobjs(urgv[1])) == NULL) {
               if ((othpyr=fndhdl(urgv[1])) == NULL) {
                    *urgv[1]=(CHAR)tolower((UCHAR)*urgv[1]);
                    if (fazbin(urgv[1],pubset,6) != NULL) {
                         btuxmt(pyrctr,CANTITSPUBLIC);
                         infoth("being ridiculous");
                    }
                    else if (fnobjp(urgv[1],pyrptr) != NULL) {
                         btuxmt(pyrctr, ALREADYHOLDINGIT);
                         infoth("looking confused");
                    }
                    else {
                         return(1);
                    }
               }
               else {
                    mspstg("...%s struggles violently to get away!\r",
                           othpyr->auxhdl);
                    gibber();
                    zntifo();
               }
          }
          else {
               takutl();
          }
     }
     else if (urgc == 3 && arge[2]-urgv[1] > 2 && stgcmp(arge[2]-2,"'s") == 0) {
          *(arge[2]-=2)='\0';
          grbutl(1,2);
     }
     else if (urgc == 4 && stgcmp(urgv[2],"from") == 0) {
          grbutl(3,1);
     }
     else {
          return(1);
     }
     return(0);
}

static VOID
takutl(VOID)                  /* take command handler utility              */
{
     if (zbjptr->flags&OBJTKB) {
          mspstg("...Okay, you got the %s.\r",zbjptr->name);
          gibber();
          mspstg("***\r%s has just taken the ",pyrptr->auxhdl);
          mspstg("%s!\r",zbjptr->name);
          fazoth();
          rmvobs(objidx);
          insobp(pyrptr,zbjptr,0);
     }
     else {
          mspstg("...%s\r",zbjptr->cntake);
          gibber();
          mspstg("***\r%s is trying to take the",pyrptr->auxhdl);
          mspstg(" %s!\r",zbjptr->name);
          fazoth();
     }
}

static VOID
grbutl(                       /* grabbing from another player utility      */
INT whofrm,
INT whtgrb)
{
     if ((othpyr=fndhdl(urgv[whofrm])) == NULL) {
          ainthr(whofrm);
     }
     else if ((zbjptr=fnobjp(urgv[whtgrb],othpyr)) == NULL) {
          mspstg("...You can't, %s isn't holding one!\r",othpyr->auxhdl);
          gibber();
          infoth("looking more shifty-eyed than usual");
     }
     else if (hldoth(&objcts[37])) {
          mspstg("...%s's felafel zaps your fingers with a blue spark!\r",
                 othpyr->auxhdl);
          gibber();
          mspstg("***\r%s just tried to ",pyrptr->auxhdl);
          addmbf(urgv[0]);
          mspstg(" your %s",zbjptr->name);
          mspstg(", but your felafel zapped %s fingers with a blue spark!\r",
                 zhisher(pyrptr));
          outpyr(othpyr);
          mspstg("***\r%s just tried to ",pyrptr->auxhdl);
          addmbf(urgv[0]);
          mspstg(" %s's ",othpyr->auxhdl);
          mspstg("%s, but ",zbjptr->name);
          addmbf(othpyr->auxhdl);
          mspstg("'s felafel zapped %s fingers with a blue spark!\r",
                 zhisher(pyrptr));
          outexc(othpyr);
     }
     else if (getrnd()&((*(urgv[0]+2) == 'a')?7:15)) {
          btuxmt(pyrctr,NICETRYNODICE);
          mspstg("***\r%s just tried to ",pyrptr->auxhdl);
          addmbf(urgv[0]);
          mspstg(" your %s!\r",zbjptr->name);
          outpyr(othpyr);
          mspstg("***\r%s just tried to ",pyrptr->auxhdl);
          addmbf(urgv[0]);
          mspstg(" %s's ",othpyr->auxhdl);
          mspstg("%s, but failed.\r",zbjptr->name);
          outexc(othpyr);
     }
     else {
          btuxmt(pyrctr,YOUGOT);
          mspstg("***\r%s has just managed to ",pyrptr->auxhdl);
          addmbf(urgv[0]);
          mspstg(" your %s away from you!\r",zbjptr->name);
          outpyr(othpyr);
          mspstg("***\r%s has just managed to ",pyrptr->auxhdl);
          addmbf(urgv[0]);
          mspstg(" %s's ",othpyr->auxhdl);
          mspstg("%s away!\r",zbjptr->name);
          outexc(othpyr);
          insobp(pyrptr,zbjptr,othpyr->pobjct[objidx]);
          rmvobp(othpyr,objidx);
     }
}

VOID
invrtn(VOID)                  /* "inventory" command handler utility       */
{
     INT i,j;

     if ((j=pyrptr->nobjs) != 0) {
          addmbf("...You are holding a ");
          i=0;
          while (1) {
               addmbf(pyrptr->pyrobj[i]->name);
               if (++i == j) {
                    addmbf(".\r");
                    break;
               }
               else if (i == j-1) {
                    addmbf(" and a ");
               }
               else {
                    addmbf(", a ");
               }
          }
          gibber();
     }
     else {
          btuxmt(pyrctr,NOTHOLDINGANYTHING);
     }
}

INT
givrtn(VOID)                  /* "give" command handler                    */
{
     if (urgc < 3 || urgc > 5) {
          if (urgc == 2 && stgcmp(urgv[1],"up") == 0) {
               return(diertn());
          }
          return(1);
     }
     if (urgc == 3) {
          givobj(1,2);
     }
     else if (urgc == 5) {
          if (stgcmp(urgv[3],"to") == 0 && sameto("cracker",urgv[2])) {
               givcrk(4,1,2);
          }
          else {
               return(1);
          }
     }
     else if (stgcmp(urgv[2],"to") == 0) {
          givobj(3,1);
     }
     else if (sameto("cracker",urgv[3])) {
          givcrk(1,2,3);
     }
     else {
          return(1);
     }
     return(0);
}

static SHORT
getnch(                       /* get number in command line                */
SHORT hwmany)
{
     SHORT i;
     CHAR *dcdptr;

     i=0;
     dcdptr=urgv[hwmany];
     while (*dcdptr != '\0') {
          if (*dcdptr < '0' || *dcdptr > '9') {
               return(-1);
          }
          i=i*10+(*dcdptr++)-'0';
     }
     return(i);
}

static VOID
givcrk(                       /* give crackers utility                     */
SHORT whoto,
SHORT hwmany,
SHORT whtnam)
{
     SHORT i;

     if ((i=getnch(hwmany)) < 0) {
          btuxmt(pyrctr,NOTVALIDCRACKERCOUNT);
          infoth("in need of arithmetic lessons");
     }
     else if (i >= pyrptr->crackl) {
          btuxmt(pyrctr, HAVETOKEEPACRACKER);
          infoth("being overly generous here");
     }
     else if ((othpyr=fndhdl(urgv[whoto])) == NULL) {
          ainthr(whoto);
     }
     else {
          btuxmt(pyrctr,CONSIDERDONE);
          jstutl();
          mspstg(" you %d ",i);
          mspstg("%s.\r",urgv[whtnam]);
          outpyr(othpyr);
          jstutl();
          mspstg(" %s ",othpyr->auxhdl);
          mspstg("%d crackers.\r",i);
          outexc(othpyr);
          pyrptr->crackl-=i;
          othpyr->crackl+=i;
     }
}

static VOID
givobj(                       /* give object to so-and-so utility          */
INT whoto,
INT whtgiv)
{
     struct object *othobj;
     INT rndobx;

     if ((othpyr=fndhdl(urgv[whoto])) == NULL) {
          ainthr(whoto);
     }
     else if ((zbjptr=fnobjp(urgv[whtgiv],pyrptr)) == NULL) {
          mspstg("...Trying to %s away things you don't have, eh?\r",urgv[0]);
          gibber();
          infoth("beaming philanthropically");
     }
     else if (pyrptr == othpyr) {
          jabber(FAZ1220);
          infoth("getting very weird here");
     }
     else if (othpyr->nobjs >= NOBJHD) {
          if (pyrptr->sttptr->nobjs >= NOBJST) {
               btuxmt(pyrctr,MYSTERIOUSFORCE);
               infoth("looking mystified all of a sudden");
               return;
          }
          if (zbjptr == &objcts[62]) {
               pyrptr->pobjct[objidx]=0;
          }
          if (getrnd()&3) {
               mspstg("...%s's hands are full -- the ",othpyr->auxhdl);
               mspstg("%s falls between you and winds up",zbjptr->name);
               mobjps(zbjptr);
               addmbf(".\r");
               gibber();
               mspstg("***\r%s has just tried to ",pyrptr->auxhdl);
               mspstg("%s you a ",urgv[0]);
               mspstg("%s, but your hands are full, so it",zbjptr->name);
               addmbf(" fell between you and wound up");
               mobjps(zbjptr);
               addmbf(".\r");
               outpyr(othpyr);
               mspstg("***\r%s has just tried to ",pyrptr->auxhdl);
               addmbf(urgv[0]);
               mspstg(" %s a ",othpyr->auxhdl);
               mspstg("%s, but it fell between them",zbjptr->name);
               addmbf(" and wound up");
               mobjps(zbjptr);
               addmbf(".\r");
               outexc(othpyr);
               rmvobp(pyrptr,objidx);
               insobs(zbjptr);
          }
          else {
               rndobx=(getrnd()&1);
               othobj=othpyr->pyrobj[rndobx];
               mspstg("...%s's hands were full -- ",othpyr->auxhdl);
               mspstg("to take the %s ",zbjptr->name);
               mspstg("%s had to drop ",zheshe(othpyr));
               addmbf(zhisher(othpyr));
               mspstg(" %s, which then wound up",othobj->name);
               mobjps(othobj);
               addmbf(".\r");
               gibber();
               jstutl();
               mspstg(" you a %s -- this made you drop your ",zbjptr->name);
               mspstg("%s, which then wound up",othobj->name);
               mobjps(othobj);
               addmbf(".\r");
               outpyr(othpyr);
               jstutl();
               mspstg(" %s a ",othpyr->auxhdl);
               mspstg("%s -- this made ",zbjptr->name);
               mspstg("%s drop ",othpyr->auxhdl);
               addmbf(zhisher(othpyr));
               mspstg(" %s, which then wound up",othobj->name);
               mobjps(othobj);
               addmbf(".\r");
               outexc(othpyr);
               insobs(othobj);
               rmvobp(othpyr,rndobx);
               insobp(othpyr,zbjptr,pyrptr->pobjct[objidx]);
               rmvobp(pyrptr,objidx);
          }
     }
     else {
          btuxmt(pyrctr,CONSIDERDONE);
          jstutl();
          mspstg(" you a %s.\r",zbjptr->name);
          outpyr(othpyr);
          jstutl();
          mspstg(" %s a ",othpyr->auxhdl);
          mspstg("%s.\r",zbjptr->name);
          outexc(othpyr);
          if (zbjptr == &objcts[15] && othpyr->nobjs != 0) {
               mspstg("***\rThe nylarx just ate your %s!\r",
                      othpyr->pyrobj[othpyr->nobjs-1]->name);
               outpyr(othpyr);
               mspstg("***\rThe nylarx just ate %s's",othpyr->auxhdl);
               mspstg(" %s!\r",othpyr->pyrobj[othpyr->nobjs-1]->name);
               outprf(pyrctr);
               outexc(othpyr);
               rmvobp(othpyr,othpyr->nobjs-1);
          }
          insobp(othpyr,zbjptr,pyrptr->pobjct[objidx]);
          rmvobp(pyrptr,objidx);
     }
}

static VOID
jstutl(VOID)                  /* small utility for giving things           */
{
     mspstg("***\r%s has just ",pyrptr->auxhdl);
     if (*urgv[0] == 'g') {
          addmbf("given");
     }
     else {
          mspstg("%sed",urgv[0]);
     }
}
