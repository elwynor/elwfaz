/*****************************************************************************
 *                                                                           *
 *   FAZCST.C    (CMDSTF.C)                                                  *
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

static struct kwctab {        /* user command keyword table data structure */
     CHAR *kyword;                 /* command verb                         */
     INT (*subadr)(VOID);          /* handler address                      */
     CHAR payonl;                  /* live-users-only flag                 */
} *kwcptr;

static INT drprtn(VOID);
static INT drputl(VOID);
static INT throwr(VOID);
static VOID thrutl(VOID);
static INT invcmd(VOID);
static INT looker(VOID);
static INT jckpls(VOID);
static VOID speclk(VOID);
static VOID larnd(VOID);
static INT whortn(VOID);
static VOID nnutl(CHAR *relat);
static INT brfrtn(VOID);
static INT unbrtn(VOID);
static INT assemb(VOID);
static VOID assutl(VOID);
static INT sitrtn(VOID);
static INT liertn(VOID);
static INT stnrtn(VOID);
static INT lghrtn(VOID);
static INT shfrtn(VOID);
static INT pshrtn(VOID);
static INT aimrtn(VOID);
static VOID pfish(VOID);
static INT kckrtn(VOID);
static INT tnkrtn(VOID);
static INT reartn(VOID);
static INT opnrtn(VOID);
static INT clsrtn(VOID);
static INT digrtn(VOID);
static INT usertn(VOID);
static INT sqzrtn(VOID);
static INT jmprtn(VOID);
static INT clbrtn(VOID);
static INT upyrtn(VOID);
static INT fckrtn(VOID);
static INT swgrtn(VOID);
static INT wrtrtn(VOID);
static INT smkrtn(VOID);
static INT nocndo(INT msgno);
static INT clprtn(VOID);
static INT snkrtn(VOID);
static INT grnrtn(VOID);
static INT smlrtn(VOID);
static INT nodrtn(VOID);
static INT blhrtn(VOID);
static INT pourtn(VOID);
static INT frwrtn(VOID);
static INT ptwrtn(VOID);
static INT smprtn(VOID);
static INT snrrtn(VOID);
static INT snortn(VOID);
static INT gwlrtn(VOID);
static INT wnkrtn(VOID);
static INT grirtn(VOID);
static INT grortn(VOID);
static INT gntrtn(VOID);
static INT hssrtn(VOID);
static INT boortn(VOID);
static INT scwrtn(VOID);
static INT bnkrtn(VOID);
static INT gigrtn(VOID);
static INT clkrtn(VOID);
static INT glrrtn(VOID);
static INT ywnrtn(VOID);
static INT trdrtn(VOID);
static INT bowrtn(VOID);
static INT snzrtn(VOID);
static INT cryrtn(VOID);
static INT sihrtn(VOID);
static INT srgrtn(VOID);
static INT drlrtn(VOID);
static INT pntrtn(VOID);
static INT fdgrtn(VOID);
static INT gsprtn(VOID);
static INT whzrtn(VOID);
static INT gglrtn(VOID);
static INT humrtn(VOID);
static INT brprtn(VOID);
static INT frtrtn(VOID);
static INT tlprtn(VOID);
static INT dncrtn(VOID);
static INT inject(VOID);
static INT shvrtn(VOID);
static INT mvgowk(VOID);
static INT spctch(VOID);
static INT helper(VOID);
static struct kwctab *binkwc(CHAR *stgptr,struct kwctab table[],INT length);


//static /* RH 6/20/2024 removed to avoid C4211 warning */
struct kwtab  *kwtptr;        /* scratch keyword table ptr                 */
//static /* RH 6/20/2024 removed to avoid C4211 warning */
struct kwntab *kwnptr;        /* scratch keyword-numeric table ptr         */

CHAR_BUFFER(ARENTHOLDINGANYTHING, 80, "...You aren't holding anything.\r");
CHAR_BUFFER(ATWHO, 80, "...At who?\r");
CHAR_BUFFER(YOUMISSED, 80, "...Uh-oh, you missed!\r");
CHAR_BUFFER(YOUGOTIT, 80, "...You got it.\r");
CHAR_BUFFER(OKBRIEF, 80, "...Okay, brief descriptions from now on.\r");
CHAR_BUFFER(OKLONG, 80, "...Okay, LONG descriptions from now on.\r");
CHAR_BUFFER(DONTFITTOGETHER, 80, "...They don't seem to fit together.\n");
CHAR_BUFFER(HANDSTOOFULL, 80, "...Your hands are too full to grasp it properly.\r");
CHAR_BUFFER(DOESNTWANTTOSPLIT, 80, "...It doesn't seem to want to come apart.\r");
CHAR_BUFFER(OKSTANDING, 80, "...Okay, you're standing up.\r");
CHAR_BUFFER(LAUGHING, 80, "...Har, har, har!!\r");
CHAR_BUFFER(WHYSHUFFLE, 80, "...Why shuffle nothing?\r");
CHAR_BUFFER(CONSIDERITDONE, 80, "...Consider it done.\r");
CHAR_BUFFER(NOTHINGHAPPENS, 80, "...Nothing happens.\r");
CHAR_BUFFER(ITDOESNTHAVEONE, 80, "...It doesn't have one!\r");
CHAR_BUFFER(ONWHATEXAMPLE, 80, "...On what? (for example, \"PUSH THE BUTTON ON THE GWINGUS\")\r");
CHAR_BUFFER(POORTHING, 80, "...The poor thing!\r");
CHAR_BUFFER(BEARSCONSIDERATION, 80, "...Ah yes, that does bear consideration!\r");
CHAR_BUFFER(NOWRITING, 80, "...There isn't any writing on it.\r");
CHAR_BUFFER(CLAPOUT, 80, "...Clap, clap, clap!\r");
CHAR_BUFFER(SNICKEROUT, 80, "...Heh, heh, heh!\r");
CHAR_BUFFER(GRINOUT, 80, "...Such a wicked-looking grin you have!\r");
CHAR_BUFFER(SMILEOUT, 80, "...Such a nice smile you have!\r");
CHAR_BUFFER(NODOUT, 80, "...Yes, that's so!\r");
CHAR_BUFFER(BLUSHOUT, 80, "...Oooh, your face is so red!\r");
CHAR_BUFFER(POUTOUT, 80, "...Now, now!\r");
CHAR_BUFFER(FROWNOUT, 80, "...Don't look so glum!\r");
CHAR_BUFFER(WHISTLEOUT, 80, "...Phoo-tee-weet?!\r");
CHAR_BUFFER(ZLEENNOFEET, 80, "...You can't, zleens don't have feet!\r");
CHAR_BUFFER(STOMPOUT, 80, "...Stomp, stomp, stomp!\r");
CHAR_BUFFER(SNEEROUT, 80, "...You look awfully disdainful!\r");
CHAR_BUFFER(SNORTOUT, 80, "...Snort!\r");
CHAR_BUFFER(GROWLOUT, 80, "...Grrrrrr!\r");
CHAR_BUFFER(WINKOUT, 80, "...Wink, wink!\r");
CHAR_BUFFER(GRIMACEOUT, 80, "...Such a perplexed look you have on your face there!\r");
CHAR_BUFFER(GROANOUT, 80, "...Groan!\r");
CHAR_BUFFER(GRUNTOUT, 80, "...Grunt!\r");
CHAR_BUFFER(HISSOUT, 80, "...SSSsssss!\r");
CHAR_BUFFER(BOOOUT, 80, "...BOOOO!\r");
CHAR_BUFFER(SCOWLOUT, 80, "...Such a look you have!\r");
CHAR_BUFFER(BLINKOUT, 80, "...Blink, blink!\r");
CHAR_BUFFER(GIGGLEOUT, 80, "...Hee hee hee!\r");
CHAR_BUFFER(CLUCKOUT, 80, "...Tsk, tsk!\r");
CHAR_BUFFER(GLAREOUT, 80, "...Don't look so outraged!\r");
CHAR_BUFFER(YAWNOUT, 80, "...Aaa-hhhhhr.\r");
CHAR_BUFFER(BOWOUT, 80, "...Your nose is almost touching the ground!\r");
CHAR_BUFFER(CRYOUT, 80, "...Boo, hoo, hoo!\r");
CHAR_BUFFER(SIGHOUT, 80, "...Sigh!\r");
CHAR_BUFFER(SHRUGOUT, 80, "...Hmmph!\r");
CHAR_BUFFER(DROOLOUT, 80, "...Drool, drool!\r");
CHAR_BUFFER(PANTOUT, 80, "...Huh uh-huh uh-huh!\r");
CHAR_BUFFER(FIDGETOUT, 80, "...Can't you keep your hands still?\r");
CHAR_BUFFER(GASPOUT, 80, "...Gasp!\r");
CHAR_BUFFER(WHEEZEOUT, 80, "...Wheeze!\r");
CHAR_BUFFER(GURGLEOUT, 80, "...G-gGg-gG-Gggg-g!\r");
CHAR_BUFFER(HUMOUT, 80, "...Hm hmmm hm hmm!\r");
CHAR_BUFFER(BURPOUT, 80, "...Uu-rrrrp!\r");
CHAR_BUFFER(FARTOUT, 80, "...If you say so!\r");
CHAR_BUFFER(DANCEOUT, 80, "...You seem to dance very gracefully!\r");
CHAR_BUFFER(WHICHDIRECTION, 80, "...Which direction?\r");
CHAR_BUFFER(WHICHDIRECTIONISTHAT, 80, "...Which direction is that?\r");
CHAR_BUFFER(WHICHWAY, 80, "...Which way?\r");
CHAR_BUFFER(NEEDSPECIALTECH, 80, "...You might need special technology to do that.\r");
CHAR_BUFFER(NOHELPAVAIL, 80, "...Help is not available on that subject.\r");


CHAR *
zhisher(                      /* return "his", "her", or "its", as approp. */
struct player *pyrp)
{
     return((invis(pyrp) || iszleen(pyrp))
          ? "its"
          : ((pyrp->logon&SEXBIT) ? "his" : "her"));
}

CHAR *
zheshe(                       /* return "he", "she", or "it", as approp.   */
struct player *pyrp)
{
     return((invis(pyrp) || iszleen(pyrp))
          ? "it"
          : ((pyrp->logon&SEXBIT) ? "he" : "she"));
}

struct state *
fndpyr(                       /* find player, return state (room) ptr      */
CHAR *hdlstg)
{
     *hdlstg=(CHAR)toupper((UCHAR)*hdlstg);
     for (othpyr=pyrarr; othpyr != pyrarr+nterms ; othpyr++) {
          if ((othpyr->logon&INGAME) && stgcmp(othpyr->plyrid,hdlstg) == 0) {
               return(othpyr->sttptr);
          }
     }
     *hdlstg=(CHAR)tolower((UCHAR)*hdlstg);
     return(NULL);
}

struct player *
fndhdl(                       /* find handle, return player ptr            */
CHAR *schstg)
{
     struct player *tmpypt;

     *schstg=(CHAR)toupper((UCHAR)*schstg);
     tmpypt=pyrptr->sttptr->plhead;
     if (hold(&objcts[42])) {
          while (1) {
               if (nmatch(schstg,tmpypt->plyrid)) {
                    return(tmpypt);
               }
               if ((tmpypt=tmpypt->nxtpyr) == NULL) {
                    return(NULL);
               }
          }
     }
     else {
          while (1) {
               if (nmatch(schstg,tmpypt->auxhdl) && !invis(tmpypt)) {
                    return(tmpypt);
               }
               if ((tmpypt=tmpypt->nxtpyr) == NULL) {
                    return(NULL);
               }
          }
     }
}

struct object *
fnobjp(                       /* find object in specified player's hands   */
CHAR *stgptr,
struct player *local_pyrptr)
{
     for (objidx=0 ; objidx < local_pyrptr->nobjs ; objidx++) {
          if (nmatch(stgptr,local_pyrptr->pyrobj[objidx]->name)) {
               return(local_pyrptr->pyrobj[objidx]);
          }
     }
     return(NULL);
}

struct object *
sfnobjs(                      /* find object in specified state (room)     */
CHAR *stgptr,
struct state *statpt)
{
     for (objidx=0 ; objidx < statpt->nobjs ; objidx++) {
          if (nmatch(stgptr,statpt->sttobj[objidx]->name)) {
               return(statpt->sttobj[objidx]);
          }
     }
     return(NULL);
}

struct object *
fnobjs(                       /* find object in current player's state     */
CHAR *stgptr)
{
     return(sfnobjs(stgptr,pyrptr->sttptr));
}

static INT
drprtn(VOID)                  /* "drop" command handler                    */
{
     if ((urgc == 1 || stgcmp(urgv[1],"it") == 0) && pyrptr->nobjs == 1) {
          objidx=0;
          zbjptr=pyrptr->pyrobj[0];
          drputl();
     }
     else if (urgc == 2 && stgcmp(urgv[1],"all") == 0) {
          if (pyrptr->nobjs == 0) {
               btuxmt(pyrctr,ARENTHOLDINGANYTHING);
               infoth("looking perplexed");
          }
          else {
               objidx=0;
               while (pyrptr->nobjs != 0) {
                    zbjptr=pyrptr->pyrobj[0];
                    if (drputl()) {
                         break;
                    }
               }
          }
     }
     else if (urgc != 2) {
          return(1);
     }
     else if ((zbjptr=fnobjp(urgv[1],pyrptr)) == NULL) {
          donthv();
     }
     else {
          drputl();
     }
     return(0);
}

static INT
drputl(VOID)                  /* drop command handler utility              */
{
     if (pyrptr->sttptr->nobjs >= NOBJST) {
          mspstg("...The %s",zbjptr->name);
          addmbf(" seems to want to stay in your hands.\r");
          gibber();
          return(1);
     }
     else {
          mspstg("...Okay, you dropped the %s.\r",zbjptr->name);
          gibber();
          mspstg("***\r%s has just dropped a ",pyrptr->auxhdl);
          mspstg("%s.\r",zbjptr->name);
          fazoth();
          rmvobp(pyrptr,objidx);
          insobs(zbjptr);
          return(0);
     }
}

static INT
throwr(VOID)                  /* "throw" command handler                   */
{
     if (urgc == 2) {
          btuxmt(pyrctr,ATWHO);
          infoth("getting violent");
     }
     else if (fndhdl(urgv[1]) != NULL) {
          return(shvrtn());
     }
     else {
          *urgv[1]=(CHAR)tolower((UCHAR)*urgv[1]);
          if (urgc != 4
           || (stgcmp(urgv[2],"at") != 0 && stgcmp(urgv[2],"to") != 0)) {
               return(1);
          }
          else if ((zbjptr=fnobjp(urgv[1],pyrptr)) == NULL) {
               donthv();
          }
          else if ((othpyr=fndhdl(urgv[3])) == NULL) {
               ainthr(3);
          }
          else {
               thrutl();
          }
     }
     return(0);
}

static VOID
thrutl(VOID)                  /* throw command handler utility             */
{
     if (pyrptr->sttptr->nobjs >= NOBJST) {
          mspstg("...The %s",zbjptr->name);
          addmbf(" seems to want to stay in your hands.\r");
          gibber();
     }
     else {
          btuxmt(pyrctr,YOUMISSED);
          mspstg("***\r%s just tried to ",pyrptr->auxhdl);
          mspstg("%s ",urgv[0]);
          mspstg("%s ",zhisher(pyrptr));
          addmbf(zbjptr->name);
          mspstg(" %s you, but missed!\r",urgv[2]);
          outpyr(othpyr);
          mspstg("***\r%s just tried to ",pyrptr->auxhdl);
          mspstg("%s ",urgv[0]);
          mspstg("%s ",zhisher(pyrptr));
          addmbf(zbjptr->name);
          mspstg(" %s ",urgv[2]);
          mspstg("%s, but missed!\r",othpyr->auxhdl);
          outexc(othpyr);
          rmvobp(pyrptr,objidx);
          insobs(zbjptr);
     }
}

VOID
rmvobp(                       /* remove object from player's inventory     */
struct player *local_pyrptr,
INT wchone)
{
     if (local_pyrptr->pyrobj[wchone] == &objcts[62]) {
          fashoo(local_pyrptr);
     }
     if (--(local_pyrptr->nobjs) != wchone) {
         local_pyrptr->pyrobj[wchone]= local_pyrptr->pyrobj[local_pyrptr->nobjs];
         local_pyrptr->pobjct[wchone]= local_pyrptr->pobjct[local_pyrptr->nobjs];
     }
}

VOID
insobp(                       /* insert object into player's inventory     */
struct player *local_pyrptr,
struct object *local_zbjptr,
SHORT count)
{
    local_pyrptr->pobjct[local_pyrptr->nobjs]=count;
    local_pyrptr->pyrobj[(local_pyrptr->nobjs)++]= local_zbjptr;
}

VOID
srmvobs(                      /* remove object from a specified state      */
INT wchone,
struct state *statpt)
{
     if (--(statpt->nobjs) != wchone) {
          statpt->sttobj[wchone]=statpt->sttobj[statpt->nobjs];
     }
}

VOID
rmvobs(                       /* remove object from current player's state */
INT wchone)
{
     srmvobs(wchone,pyrptr->sttptr);
}

VOID
insobs(                       /* insert object in current player's state   */
struct object *local_zbjptr)
{
     pyrptr->sttptr->sttobj[pyrptr->sttptr->nobjs++]=local_zbjptr;
}

static INT
invcmd(VOID)                  /* "inventory" command handler               */
{
     invrtn();
     infoth("taking inventory");
     return(0);
}

static INT
looker(VOID)                  /* "look" and "examine" command handler      */
{
     INT i,j;

     if (urgc > 1) {
          zapwrd("to");
          zapwrd("at");
          zapwrd("in");
          zapwrd("into");
          zapwrd("inside");
          zapwrd("under");
          zapwrd("for");
     }
     if (urgc == 2) {
          if ((zbjptr=fnobjp(urgv[1],pyrptr)) != NULL) {
               speclk();
          }
          else if ((zbjptr=fnobjs(urgv[1])) != NULL) {
               jabber(zbjptr->objmsg);
               mspstg("***\r%s is looking at the ",pyrptr->auxhdl);
               mspstg("%s.\r",zbjptr->name);
               fazoth();
          }
          else if ((othpyr=fndhdl(urgv[1])) != NULL) {
               if (invis(othpyr)) {
                    if (!hldoth(&objcts[62])) {
                         fashoo(othpyr);
                    }
                    jabber(FAZ1249);
                    mspstg("...%s",othpyr->plyrid);
               }
               else {
                    mspstg("...%s",othpyr->auxhdl);
               }
               j=othpyr->nobjs;
               if (iszleen(othpyr)) {
                    prfmsg(FAZ1699);
               }
               else if (j == 0) {
                    prfmsg((othpyr->logon&SEXBIT) ? FAZ1700 : FAZ1600);
               }
               else {
                    prfmsg(min(68,((INT)(othpyr->pyrobj[0]-objcts)))
                          +((othpyr->logon&SEXBIT) ? FAZ1701 : FAZ1601));
               }
               if (j != 0 && hldoth(&objcts[54]) == 0) {
                    addmbf(" appears to be holding a ");
                    i=0;
                    while(1) {
                         addmbf(othpyr->pyrobj[i]->name);
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
               }
               else {
                    addmbf(" does not appear to be holding anything.\r");
               }
               gibber();
               mspstg("***\r%s is looking at ",pyrptr->auxhdl);
               if (othpyr == pyrptr) {
                    addmbf((pyrptr->logon&SEXBIT) ? "himself.\r" : "herself.\r");
                    fazoth();
               }
               else {
                    mspstg("%s.\r",othpyr->auxhdl);
                    outexc(othpyr);
                    mspstg("***\r%s is looking you over.\r",pyrptr->auxhdl);
                    outpyr(othpyr);
               }
          }
          else if (nmatch(urgv[1],"Brief")) {
               addmbf("...");
               noistt=-1;
               whther();
               whoher();
               gibber();
               infoth("furtively glancing around");
          }
          else {
               larnd();
          }
     }
     else {
          larnd();
     }
     return(0);
}

static INT
jckpls(VOID)                  /* Sysop-privileged item conjuring command   */
{
     if (urgc == 3 && strcmp(urgv[2],"pls") == 0) {
          if (pyrptr->nobjs < NOBJHD && (usrptr->flags&MASTER)) {
               for (zbjptr=objcts ; zbjptr < objcts+70 ; zbjptr++) {
                    if (nmatch(urgv[1],zbjptr->name)) {
                         insobp(pyrptr,zbjptr,0);
                         btuxmt(pyrctr,YOUGOTIT);
                         return(0);
                    }
               }
          }
     }
     return(1);
}

static VOID
speclk(VOID)                  /* special look-at-object utility            */
{
     INT i;

     if (zbjptr == &objcts[63]
      && ((i=(INT)((pyrptr->sttptr)-states)) > 284 || i < 277)) {
          addmbf("...The readout on the ceeveese says ");
          mspstg("%d.\r",i);
          gibber();
     }
     else {
          jabber(zbjptr->objmsg);
     }
     mspstg("***\r%s is examining ",pyrptr->auxhdl);
     addmbf(zhisher(pyrptr));
     mspstg(" %s.\r",zbjptr->name);
     fazoth();
}

static VOID
larnd(VOID)                   /* "look around" handler                     */
{
     prfmsg(pyrptr->sttptr->sttmsg);
     whther();
     whoher();
     gibber();
     infoth("looking around");
}

static INT
whortn(VOID)                  /* who/what/where/etc. command handler       */
{
     *urgv[0]=(CHAR)toupper((UCHAR)*urgv[0]);
     mspstg("...%s do you think?\r",urgv[0]);
     gibber();
     infoth("looking philosophical");
     return(0);
}

INT
ntnice(VOID)                  /* not-nice thing to do to person or thing   */
{
     if (urgc == 1) {
          return(1);
     }
     if ((zbjptr=fnobjp(urgv[1],pyrptr)) != NULL) {
          nnutl(zhisher(pyrptr));
     }
     else if ((zbjptr=fnobjs(urgv[1])) != NULL) {
          nnutl("the");
     }
     else if ((othpyr=fndhdl(urgv[1])) != NULL) {
          mspstg("...A blue force field springs up, protecting %s.\r",
                 othpyr->auxhdl);
          gibber();
          zntifo();
     }
     else {
          ainthr(1);
     }
     return(0);
}

static VOID
nnutl(                        /* not-nice utility                          */
CHAR *relat)
{
     mspstg("...Oh, the %s doesn't like that!\r",zbjptr->name);
     gibber();
     mspstg("***\r%s is trying to ",pyrptr->auxhdl);
     addmbf(urgv[0]);
     mspstg(" %s",relat);
     mspstg(" %s!\r",zbjptr->name);
     fazoth();
}

VOID
zntifo(VOID)                  /* notify object of direct action and others */
{
     mspstg("***\r%s is trying to ",pyrptr->auxhdl);
     mspstg("%s you!\r",urgv[0]);
     outpyr(othpyr);
     mspstg("***\r%s is trying to ",pyrptr->auxhdl);
     addmbf(urgv[0]);
     mspstg(" %s!\r",othpyr->auxhdl);
     outexc(othpyr);
}

static INT
brfrtn(VOID)                  /* turn "brief" mode on or off               */
{
     if (urgc == 1 || stgcmp(urgv[1],"on") == 0) {
          pyrptr->logon|=BRFBIT;
          btuxmt(pyrctr,OKBRIEF);
     }
     else if (urgc == 2 && stgcmp(urgv[1],"off") == 0) {
          unbrtn();
     }
     else {
          return(1);
     }
     return(0);
}

static INT
unbrtn(VOID)                  /* turn "brief" mode off                     */
{
     INT i;

     pyrptr->logon&=(~BRFBIT);
     btuxmt(pyrctr,OKLONG);
     for (i=0 ; i < NVSTMN ; i++) {
          pyrptr->vsttes[i]=0;
     }
     return(0);
}

static INT
assemb(VOID)                  /* "assemble" and "attach" command handler   */
{
     INT i,j;
     struct object *obj1;

     zapwrd("together");
     if (urgc < 4) {
          return(1);
     }
     if (stgcmp(urgv[2],"with") == 0
      || stgcmp(urgv[2],"to") == 0
      || stgcmp(urgv[2],"and") == 0
      || *urgv[2] == '&'
      || stgcmp(urgv[2],"onto") == 0
      || stgcmp(urgv[2],"into") == 0
      || stgcmp(urgv[2],"on") == 0) {
          j=1;
          if ((zbjptr=fnobjp(urgv[1],pyrptr)) != NULL) {
               obj1=zbjptr;
               j=3;
               if ((zbjptr=fnobjp(urgv[3],pyrptr)) != NULL) {
                    if (obj1 != zbjptr) {
                         for (i=0 ; i < NOBJMS ; i++) {
                              if (zbjptr->mothrs[i] != 0
                                   && (zbjptr->mothrs[i]->dghtr1 == obj1
                                   || zbjptr->mothrs[i]->dghtr2 == obj1)) {
                                   rmvobp(pyrptr,objidx);
                                   fnobjp(urgv[1],pyrptr);
                                   rmvobp(pyrptr,objidx);
                                   zbjptr=zbjptr->mothrs[i];
                                   insobp(pyrptr,zbjptr,0);
                                   assutl();
                                   return(0);
                              }
                         }
                    }
                    btuxmt(pyrctr,DONTFITTOGETHER);
                    mspstg("***\r%s is trying to assemble something.\r",pyrptr->auxhdl);
                    fazoth();
                    return(0);
               }
          }
          mspstg("...You can't, you aren't holding a %s.\r",urgv[j]);
          gibber();
          infoth("fumbling around a little");
          return(0);
     }
     return(1);
}

static VOID
assutl(VOID)                  /* assemble/attach command utility           */
{
     mspstg("...They appear to fit together, making a %s!\r",zbjptr->name);
     gibber();
     mspstg("***\r%s has just constructed a ",pyrptr->auxhdl);
     mspstg("%s!\r",zbjptr->name);
     fazoth();
}

INT
zdsass(VOID)                  /* "disassemble" and "split" command handler */
{
     zapwrd("apart");
     if (urgc == 1) {
          return(1);
     }
     else if ((zbjptr=fnobjp(urgv[1],pyrptr)) == NULL) {
          donthv();
     }
     else if (pyrptr->nobjs >= NOBJHD) {
          btuxmt(pyrctr,HANDSTOOFULL);
          infoth("looking around in chagrin");
     }
     else if (zbjptr->dghtr1 == NULL) {
          btuxmt(pyrctr,DOESNTWANTTOSPLIT);
          infoth("somewhat perturbed");
     }
     else {
          mspstg("...It breaks cleanly apart into a %s",zbjptr->dghtr1->name);
          mspstg(" and a %s.\r",zbjptr->dghtr2->name);
          gibber();
          mspstg("***\r%s has just disassembled ",pyrptr->auxhdl);
          addmbf(zhisher(pyrptr));
          mspstg(" %s!\r",zbjptr->name);
          fazoth();
          rmvobp(pyrptr,objidx);
          insobp(pyrptr,zbjptr->dghtr1,0);
          insobp(pyrptr,zbjptr->dghtr2,0);
     }
     return(0);
}

VOID
infoth(                       /* inform others about something             */
CHAR *infstg)
{
     mspstg("***\r%s is ",pyrptr->auxhdl);
     mspstg(infstg,zhisher(pyrptr));
     addmbf(".\r");
     fazoth();
}

static INT
sitrtn(VOID)                  /* "sit" command handler                     */
{
     mspstg("...Okay, you're now sitting %s.\r",pyrptr->sttptr->sobjlc);
     gibber();
     infoth("sitting down to rest");
     return(0);
}

static INT
liertn(VOID)                  /* "lie" command handler                     */
{
     mspstg("...Okay, you're now lying down %s.\r",pyrptr->sttptr->sobjlc);
     gibber();
     infoth("lying down to rest a while");
     return(0);
}

static INT
stnrtn(VOID)                  /* "stand" command handler                   */
{
     btuxmt(pyrctr,OKSTANDING);
     infoth("standing up");
     return(0);
}

static INT
lghrtn(VOID)                  /* "laugh" command handler                   */
{
     btuxmt(pyrctr,LAUGHING);
     infoth("laughing %s fool head off");
     return(0);
}

static INT
shfrtn(VOID)                  /* "shuffle" command handler                 */
{
     INT i;

     if ((i=pyrptr->nobjs-1) < 0) {
          btuxmt(pyrctr,WHYSHUFFLE);
          infoth("shuffling about aimlessly");
     }
     else if (i == 0) {
          mspstg("...Your %s appreciates that.\r",pyrptr->pyrobj[0]->name);
          gibber();
          infoth("having a wonderful time");
     }
     else {
          while (1) {
               zbjptr=pyrptr->pyrobj[i-1];
               intmp1=pyrptr->pobjct[i-1];
               pyrptr->pyrobj[i-1]=pyrptr->pyrobj[i];
               pyrptr->pobjct[i-1]=pyrptr->pobjct[i];
               pyrptr->pyrobj[i]=zbjptr;
               pyrptr->pobjct[i]=(SHORT)intmp1;
               if (--i == 0) {
                    break;
               }
          }
          btuxmt(pyrctr,CONSIDERITDONE);
     }
     infoth("shuffling %s goodies around");
     return(0);
}

static INT
pshrtn(VOID)                  /* "push" command handler                    */
{
     if (urgc >= 4 && stgcmp(urgv[2],"on") == 0
      && (nmatch(urgv[1],"button") || nmatch(urgv[1],"switch")
       || nmatch(urgv[1],"pushbutton"))) {
          if ((zbjptr=fnobjp(urgv[3],pyrptr)) != NULL) {
               if (zbjptr->flags&ACTBUT) {
                    mspstg("***\r%s is pushing the button on ",pyrptr->auxhdl);
                    addmbf(zhisher(pyrptr));
                    mspstg(" %s!\r",zbjptr->name);
                    fazoth();
                    if (pyrptr->pobjct[objidx] == 0) {
                         (*(zbjptr->actvec))();
                    }
                    else if (pyrptr->pyrobj[objidx] == &objcts[62]) {
                         pyrptr->pobjct[objidx]=0;
                         fashoo(pyrptr);
                    }
                    else {
                         btuxmt(pyrctr,NOTHINGHAPPENS);
                    }
               }
               else {
                    btuxmt(pyrctr,ITDOESNTHAVEONE);
                    infoth("getting a little crazy here");
               }
          }
          else {
               donthv();
          }
          return(0);
     }
     if (urgc == 2 && nmatch(urgv[1],"button")) {
          btuxmt(pyrctr,ONWHATEXAMPLE);
          return(0);
     }
     else if (stgcmp(urgv[0],"hit") == 0) {
          return(ntnice());
     }
     else if (stgcmp(urgv[0],"push") == 0) {
          return(shvrtn());
     }
     return(1);
}

static INT
aimrtn(VOID)                  /* "aim" and "point" command handler         */
{
     struct object *objtmp;

     if (urgc >= 3) {
          if ((zbjptr=fnobjp(urgv[1],pyrptr)) != NULL) {
               pfish();
               if ((zbjptr->flags&ACTPNT) == 0 || pyrptr->pobjct[objidx] != 0) {
               }
               else if (urgc >= 4
                     && (stgcmp(urgv[2],"at") == 0 || nmatch("to",urgv[2]))) {
                    if ((objtmp=fnobjs(urgv[3])) != NULL) {
                         (*(objact)(zbjptr->actvec))(objtmp,0);
                    }
                    else if ((othpyr=fndhdl(urgv[3])) != NULL) {
                         (*(plract)(zbjptr->actvec))(othpyr,1);
                    }
                    else {
                         *urgv[3]=(CHAR)tolower((UCHAR)*urgv[3]);
                         (*(objact)(zbjptr->actvec))(NULL,3);
                    }
               }
               else {
                    (*(objact)(zbjptr->actvec))(NULL,2);
               }
          }
          else {
               donthv();
          }
          return(0);
     }
     return(1);
}

static VOID
pfish(VOID)                   /* aim/point command utility                 */
{
     mspstg("...Okay, it's %sed in that direction.\r",urgv[0]);
     gibber();
     mspstg("***\r%s is doing something fishy with ",pyrptr->auxhdl);
     addmbf(zhisher(pyrptr));
     mspstg(" %s.\r",zbjptr->name);
     fazoth();
}

static INT
kckrtn(VOID)                  /* "kick" command handler                    */
{
     if (urgc >= 2) {
          if ((zbjptr=fnobjp(urgv[1],pyrptr)) != NULL) {
               if (zbjptr->flags&ACTKCK) {
                    mspstg("***\r%s is kicking ",pyrptr->auxhdl);
                    addmbf(zhisher(pyrptr));
                    mspstg(" %s!\r",zbjptr->name);
                    fazoth();
                    if (pyrptr->pobjct[objidx] == 0) {
                         (*(zbjptr->actvec))();
                    }
                    else {
                         btuxmt(pyrctr,POORTHING);
                    }
               }
               else {
                    ntnice();
               }
          }
          else {
               ntnice();
          }
          return(0);
     }
     return(1);
}

static INT
tnkrtn(VOID)                  /* "think" command handler                   */
{
     struct state *pyrstp;

     if (urgc >= 3
      && (stgcmp(urgv[1],"about") == 0 || stgcmp(urgv[1],"at") == 0
       || stgcmp(urgv[1],"of") == 0)) {
          infoth("suddenly deep in thought");
          if ((zbjptr=fnobjp(urgv[2],pyrptr)) != NULL) {
               if (zbjptr->flags&ACTTNK) {
                    (*(zbjptr->actvec))();
               }
               else {
                    mspstg("...The %s appreciates your thoughtfulness...\r",
                           zbjptr->name);
                    gibber();
               }
          }
          else if (hold(&objcts[58]) && (pyrstp=fndpyr(urgv[2])) != NULL) {
               prfmsg(FAZ1240);
               outare(pyrstp);
               prfmsg(FAZ1241);
               prfmsg(pyrstp->sttmsg);
               prfmsg(FAZ1242);
               gibber();
          }
          else {
               btuxmt(pyrctr,BEARSCONSIDERATION);
          }
          return(0);
     }
     return(1);
}

static INT
reartn(VOID)                  /* "read" command handler                    */
{
     if (urgc >= 2) {
          if ((zbjptr=fnobjp(urgv[1],pyrptr)) != NULL
           || (zbjptr=fnobjs(urgv[1])) != NULL) {
               if (zbjptr->flags&OBJRDA) {
                    looker();
               }
               else {
                    btuxmt(pyrctr,NOWRITING);
               }
          }
          else {
               donthv();
          }
          return(0);
     }
     return(1);
}

static INT
opnrtn(VOID)                  /* "open" command handler                    */
{
     return(nocndo(FAZ1825));
}

static INT
clsrtn(VOID)                  /* "close" command handler                   */
{
     return(nocndo(FAZ1826));
}

static INT
digrtn(VOID)                  /* "dig" command handler                     */
{
     jabber(FAZ1827);
     infoth("really getting into this");
     return(0);
}

static INT
usertn(VOID)                  /* "use" command handler                     */
{
     return(nocndo(FAZ1828));
}

static INT
sqzrtn(VOID)                  /* "squeeze" command handler                 */
{
     return(nocndo(FAZ1829));
}

static INT
jmprtn(VOID)                  /* "jump" command handler                    */
{
     jabber(FAZ1830);
     infoth("getting a little jumpy here");
     return(0);
}

static INT
clbrtn(VOID)                  /* "climb" command handler                   */
{
     jabber(FAZ1831);
     infoth("looking upset about something");
     return(0);
}

static INT
upyrtn(VOID)                  /* "up yours" command handler                */
{
     if (urgc >= 2 && stgcmp(urgv[1],"yours") == 0) {
          jabber(FAZ1832);
          infoth("contemplating corrective surgery");
          return(0);
     }
     return(1);
}

static INT
fckrtn(VOID)                  /* "fuck you" command handler                */
{
     if (urgc >= 2 && stgcmp(urgv[1],"you") == 0) {
          jabber(FAZ1833);
          infoth("telepathically beaming foul imprecations at the world");
          return(0);
     }
     return(1);
}

static INT
swgrtn(VOID)                  /* "swing" command handler                   */
{
     return(nocndo(FAZ1834));
}

static INT
wrtrtn(VOID)                  /* "write" command handler                   */
{
     jabber(FAZ1835);
     infoth("checking %s pockets for something");
     return(0);
}

static INT
smkrtn(VOID)                  /* "smoke" command handler                   */
{
     jabber(FAZ1836);
     infoth("fumbling around for something");
     return(0);
}

static INT
nocndo(                       /* utility for no-can-do situations          */
INT msgno)
{
     if (urgc >= 2) {
          if ((zbjptr=fnobjp(urgv[1],pyrptr)) != NULL
           || (zbjptr=fnobjs(urgv[1])) != NULL) {
               jabber(msgno);
               infoth("trying something sneaky");
          }
          else {
               donthv();
          }
          return(0);
     }
     return(1);
}

static INT
clprtn(VOID)                  /* "clap" and "applaud" command handler      */
{
     btuxmt(pyrctr,CLAPOUT);
     infoth("applauding");
     return(0);
}

static INT
snkrtn(VOID)                  /* "snicker" and "chuckle" command handler   */
{
     btuxmt(pyrctr,SNICKEROUT);
     infoth("chuckling under %s breath");
     return(0);
}

static INT
grnrtn(VOID)                  /* "grin" command handler                    */
{
     btuxmt(pyrctr,GRINOUT);
     infoth("grinning from ear to ear");
     return(0);
}

static INT
smlrtn(VOID)                  /* "smile" command handler                   */
{
     btuxmt(pyrctr,SMILEOUT);
     infoth("smiling agreeably");
     return(0);
}

static INT
nodrtn(VOID)                  /* "nod" command handler                     */
{
     btuxmt(pyrctr,NODOUT);
     infoth("nodding in agreement");
     return(0);
}

static INT
blhrtn(VOID)                  /* "blush" command handler                   */
{
     btuxmt(pyrctr,BLUSHOUT);
     infoth("blushing");
     return(0);
}

static INT
pourtn(VOID)                  /* "pout" command handler                    */
{
     btuxmt(pyrctr,POUTOUT);
     infoth("pouting");
     return(0);
}

static INT
frwrtn(VOID)                  /* "frown" command handler                   */
{
     btuxmt(pyrctr,FROWNOUT);
     infoth("frowning");
     return(0);
}

static INT
ptwrtn(VOID)                  /* "whistle" command handler                 */
{
     btuxmt(pyrctr,WHISTLEOUT);
     infoth("whistling");
     return(0);
}

static INT
smprtn(VOID)                  /* "stomp" command handler                   */
{
     if (iszleen(pyrptr)) {
          btuxmt(pyrctr,ZLEENNOFEET);
          infoth("wiggling around awkwardly");
     }
     else {
          btuxmt(pyrctr,STOMPOUT);
          infoth("stomping %s feet");
     }
     return(0);
}

static INT
snrrtn(VOID)                  /* "sneer" command handler                   */
{
     btuxmt(pyrctr,SNEEROUT);
     infoth("sneering disdainfully");
     return(0);
}

static INT
snortn(VOID)                  /* "snort" command handler                   */
{
     btuxmt(pyrctr,SNORTOUT);
     mspstg("***\r%s just snorted!\r",pyrptr->auxhdl);
     fazoth();
     return(0);
}

static INT
gwlrtn(VOID)                  /* "growl" command handler                   */
{
     btuxmt(pyrctr,GROWLOUT);
     infoth("growling ominously");
     return(0);
}

static INT
wnkrtn(VOID)                  /* "wink" command handler                    */
{
     btuxmt(pyrctr,WINKOUT);
     zapwrd("at");
     if (urgc >= 2 && (othpyr=fndhdl(urgv[1])) != NULL) {
          mspstg("***\r%s just winked at you surreptitiously!\r",
                 pyrptr->auxhdl);
          outpyr(othpyr);
     }
     else {
          infoth("apparently having problems with %s eyes");
     }
     return(0);
}

static INT
grirtn(VOID)                  /* "grimace" command handler                 */
{
     btuxmt(pyrctr,GRIMACEOUT);
     infoth("grimacing perplexedly");
     return(0);
}

static INT
grortn(VOID)                  /* "groan" command handler                   */
{
     btuxmt(pyrctr,GROANOUT);
     infoth("groaning piteously");
     return(0);
}

static INT
gntrtn(VOID)                  /* "grunt" command handler                   */
{
     btuxmt(pyrctr,GRUNTOUT);
     infoth("grunting indifferently");
     return(0);
}

static INT
hssrtn(VOID)                  /* "hiss" command handler                    */
{
     btuxmt(pyrctr,HISSOUT);
     infoth("hissing");
     return(0);
}

static INT
boortn(VOID)                  /* "boo" command handler                     */
{
     btuxmt(pyrctr,BOOOUT);
     infoth("booing loudly");
     return(0);
}

static INT
scwrtn(VOID)                  /* "scowl" command handler                   */
{
     btuxmt(pyrctr,SCOWLOUT);
     infoth("scowling threateningly");
     return(0);
}

static INT
bnkrtn(VOID)                  /* "blink" command handler                   */
{
     btuxmt(pyrctr,BLINKOUT);
     infoth("blinking his eyes in disbelief");
     return(0);
}

static INT
gigrtn(VOID)                  /* "giggle" command handler                  */
{
     btuxmt(pyrctr,GIGGLEOUT);
     infoth("giggling uncontrollably");
     return(0);
}

static INT
clkrtn(VOID)                  /* "cluck" command handler                   */
{
     btuxmt(pyrctr,CLUCKOUT);
     infoth("clucking reproachfully");
     return(0);
}

static INT
glrrtn(VOID)                  /* "glare" command handler                   */
{
     btuxmt(pyrctr,GLAREOUT);
     zapwrd("at");
     if (urgc >= 2 && (othpyr=fndhdl(urgv[1])) != NULL) {
          mspstg("***\r%s is glaring at you as though outraged!\r",
                 pyrptr->auxhdl);
          outpyr(othpyr);
          mspstg("***\r%s is glaring at ",pyrptr->auxhdl);
          mspstg("%s as though outraged!\r",othpyr->auxhdl);
          outexc(othpyr);
     }
     else {
          infoth("glaring about in outrage");
     }
     return(0);
}

static INT
ywnrtn(VOID)                  /* "yawn" command handler                    */
{
     btuxmt(pyrctr,YAWNOUT);
     infoth("yawning openly");
     return(0);
}

static INT
trdrtn(VOID)                  /* "trade" command handler                   */
{
     jabber(FAZ1824);
     infoth("looking very businesslike all of a sudden");
     return(0);
}

static INT
bowrtn(VOID)                  /* "bow" command handler                     */
{
     btuxmt(pyrctr,BOWOUT);
     infoth("bowing in all directions");
     return(0);
}

static INT
snzrtn(VOID)                  /* "sneeze" command handler                  */
{
     jabber(FAZ1837);
     infoth("making strange ah-ing noises");
     return(0);
}

static INT
cryrtn(VOID)                  /* "cry" command handler                     */
{
     if (urgc > 1) {
          yelrtn();
     }
     else {
          btuxmt(pyrctr,CRYOUT);
          infoth("crying %s poor heart out");
     }
     return(0);
}

static INT
sihrtn(VOID)                  /* "sigh" command handler                    */
{
     btuxmt(pyrctr,SIGHOUT);
     infoth("sighing wistfully");
     return(0);
}

static INT
srgrtn(VOID)                  /* "shrug" command handler                   */
{
     btuxmt(pyrctr,SHRUGOUT);
     infoth("shrugging %s shoulders");
     return(0);
}

static INT
drlrtn(VOID)                  /* "drool" command handler                   */
{
     btuxmt(pyrctr,DROOLOUT);
     infoth("drooling obscenely");
     return(0);
}

static INT
pntrtn(VOID)                  /* "pant" command handler                    */
{
     btuxmt(pyrctr,PANTOUT);
     infoth("panting heavily all of a sudden");
     return(0);
}

static INT
fdgrtn(VOID)                  /* "fidget" command handler                  */
{
     btuxmt(pyrctr,FIDGETOUT);
     infoth("fidgeting nervously");
     return(0);
}

static INT
gsprtn(VOID)                  /* "gasp" command handler                    */
{
     btuxmt(pyrctr,GASPOUT);
     infoth("gasping for breath");
     return(0);
}

static INT
whzrtn(VOID)                  /* "wheeze" command handler                  */
{
     btuxmt(pyrctr,WHEEZEOUT);
     infoth("having trouble with %s breathing");
     return(0);
}

static INT
gglrtn(VOID)                  /* "gurgle" command handler                  */
{
     btuxmt(pyrctr,GURGLEOUT);
     infoth("gurgling noisily for some reason");
     return(0);
}

static INT
humrtn(VOID)                  /* "hum" command handler                     */
{
     btuxmt(pyrctr,HUMOUT);
     infoth("humming a little tune");
     return(0);
}

static INT
brprtn(VOID)                  /* "burp" command handler                    */
{
     btuxmt(pyrctr,BURPOUT);
     infoth("giving vent to %s feelings");
     return(0);
}

static INT
frtrtn(VOID)                  /* "fart" command handler                    */
{
     btuxmt(pyrctr,FARTOUT);
     addmbf("***\rYou suddenly smell a foul odor in the air!\r");
     fazoth();
     return(0);
}

static INT
tlprtn(VOID)                  /* "teleport" command handler                */
{
     INT dststt;

     if (urgc >= 3
      && stgcmp(urgv[1],"to") == 0
      && sscanf(urgv[2],"%d",&dststt) == 1
      && holdng(&objcts[61])
      && dststt >= 0 && dststt < TOTSTT-1) {
          if (dststt < 313 || dststt > 319) {
               rmvobp(pyrptr,objidx);
               jabber(FAZ1230);
               prfmsg(FAZ1231);
               mspstg("%s!\r",pyrptr->auxhdl);
               fazoth();
               prfmsg(FAZ1232);
               outner();
               lvstt("turned to red fire and jumped into the column");
               prfmsg(FAZ1233);
               fazoth();
               prfmsg(FAZ1234);
               outare(&states[dststt]);
               ntrstt(&states[dststt],"appeared out of the column");
               prfmsg(FAZ1232);
               outner();
               prfmsg(FAZ1235);
               fazoth();
          }
          else {
               jabber(FAZ1237);
               infoth("looking pained and holding %s baffle very gingerly");
          }
     }
     else {
          spctch();
     }
     return(0);
}

static INT
dncrtn(VOID)                  /* "dance" command handler                   */
{
     btuxmt(pyrctr,DANCEOUT);
     infoth("dancing about in glee");
     return(0);
}

static INT
inject(VOID)                  /* Sysop-privileged "inject:" command handler */
{
     if ((usrptr->flags&MASTER) && urgc > 1) {
          *urgv[1]=(CHAR)toupper((UCHAR)*urgv[1]);
          recomb();
          mspstg("***\r%s\r",urgv[1]);
          gibber();
          fazoth();
          return(0);
     }
     return(1);
}

struct kwtab *
fazbin(                       /* binary search keyword lookup routine      */
CHAR *stgptr,
struct kwtab table[],
INT length)
{
     INT cond;
     struct kwtab *low,*mid,*high;

     low=&table[0];
     high=&table[length-1];
     while (low <= high) {
          mid=low+(INT)((high-low))/2;
          if ((cond=stgcmp(stgptr,mid->kyword)) < 0) {
               if (mid == low) {
                    break;
               }
               high=mid-1;
          }
          else if (cond > 0) {
               if (mid == high) {
                    break;
               }
               low=mid+1;
          }
          else {
               return(mid);
          }
     }
     return(NULL);
}

static INT
shvrtn(VOID)                  /* "shove" command handler                   */
{
     INT argno;
     static struct kwtab shvset[]={
          {"e",      shovee},
          {"east",   shovee},
          {"n",      shoven},
          {"north",  shoven},
          {"s",      shoves},
          {"south",  shoves},
          {"w",      shovew},
          {"west",   shovew}
     };

     if (urgc >= 2) {
          if ((othpyr=fndhdl(urgv[1])) == NULL) {
               ainthr(1);
          }
          else if (urgc == 2) {
               btuxmt(pyrctr,WHICHDIRECTION);
               zntifo();
          }
          else {
               if (urgc > 3
                && (stgcmp(urgv[2],"at") == 0 || nmatch("to",urgv[2]))) {
                    argno=3;
               }
               else {
                    argno=2;
               }
               if ((kwtptr=fazbin(urgv[argno],shvset,8)) != NULL) {
                    return((*(kwtptr->subadr))());
               }
               else {
                    btuxmt(pyrctr,WHICHDIRECTIONISTHAT);
                    zntifo();
               }
          }
          return(0);
     }
     return(1);
}

static INT
mvgowk(VOID)                  /* "move", "go" and "walk" command handler   */
{
     static struct kwtab mgwset[]={
          {"e",      movee},
          {"east",   movee},
          {"n",      moven},
          {"north",  moven},
          {"s",      moves},
          {"south",  moves},
          {"w",      movew},
          {"west",   movew}
     };

     if (urgc == 1) {
          btuxmt(pyrctr,WHICHWAY);
          infoth("looking around uncertainly");
     }
     else if (urgc > 2) {
          spctch();
     }
     else if ((kwtptr=fazbin(urgv[1],mgwset,8)) != NULL) {
          --urgc;
          return((*(kwtptr->subadr))());
     }
     else {
          return(1);
     }
     return(0);
}

static INT
spctch(VOID)                  /* weird or special action rejector          */
{
     btuxmt(pyrctr,NEEDSPECIALTECH);
     infoth("trying to do something strange");
     return(0);
}

static INT
helper(VOID)                  /* "help" command handler                    */
{
#define FHLPLEN (sizeof(hlpset)/sizeof(struct kwntab))

     static struct kwntab hlpset[]={
          {"fazuul",FAZ51},
          {"moving",FAZ54},
          {"quitting",FAZ58},
          {"strategy",FAZ56},
          {"talking",FAZ53},
          {"trading",FAZ55}
     };

     infoth("getting help");
     if (urgc == 1) {
          jabber(FAZ50);
     }
     else {
          for (kwnptr=hlpset ; kwnptr-hlpset < FHLPLEN ; kwnptr++) {
               if (nmatch(urgv[1],kwnptr->kyword)) {
                    jabber(kwnptr->number);
                    return(0);
               }
          }
          btuxmt(pyrctr,NOHELPAVAIL);
     }
     return(0);
}

static struct kwctab *
binkwc(                       /* binary search for kwctab structure        */
CHAR *stgptr,
struct kwctab table[],
INT length)
{
     INT cond;
     struct kwctab *low,*mid,*high;

     low=&table[0];
     high=&table[length-1];
     while (low <= high) {
          mid=low+(INT)((high-low))/2;
          if ((cond=stgcmp(stgptr,mid->kyword)) < 0) {
               if (mid == low) {
                    break;
               }
               high=mid-1;
          }
          else if (cond > 0) {
               if (mid == high) {
                    break;
               }
               low=mid+1;
          }
          else {
               return(mid);
          }
     }
     return(NULL);
}

INT
stdcmd(VOID)                  /* standard command master handler           */
{
#define SSTLEN (sizeof(stdset)/sizeof(struct kwctab))

     static struct kwctab stdset[]={
          {"?",helper,0},
          {"admit",sayrtn,0},
          {"advise",sayrtn,0},
          {"agree",sayrtn,0},
          {"aim",aimrtn,1},
          {"allege",sayrtn,0},
          {"annihilate",ntnice,1},
          {"announce",sayrtn,0},
          {"applaud",clprtn,0},
          {"argue",sayrtn,0},
          {"ask",sayrtn,0},
          {"assemble",assemb,1},
          {"assert",sayrtn,0},
          {"attach",assemb,1},
          {"attack",ntnice,1},
          {"attest",sayrtn,0},
          {"aver",sayrtn,0},
          {"avow",sayrtn,0},
          {"babble",sayrtn,0},
          {"bawl",yelrtn,1},
          {"bellow",yelrtn,1},
          {"bite",ntnice,1},
          {"bless",ntnice,1},
          {"blink",bnkrtn,0},
          {"blubber",sayrtn,0},
          {"blurt",sayrtn,0},
          {"blush",blhrtn,0},
          {"boo",boortn,0},
          {"bow",bowrtn,0},
          {"break",zdsass,1},
          {"brief",brfrtn,0},
          {"burp",brprtn,0},
          {"cackle",sayrtn,0},
          {"catch",takrtn,1},
          {"chant",yelrtn,1},
          {"chat",sayrtn,0},
          {"chatter",sayrtn,0},
          {"cheer",clprtn,0},
          {"chirp",sayrtn,0},
          {"chortle",snkrtn,0},
          {"chuck",throwr,1},
          {"chuckle",snkrtn,0},
          {"claim",sayrtn,0},
          {"clap",clprtn,0},
          {"climb",clbrtn,1},
          {"clobber",ntnice,1},
          {"close",clsrtn,1},
          {"cluck",clkrtn,0},
          {"combine",assemb,1},
          {"comfort",ntnice,0},
          {"command",sayrtn,0},
          {"comment",sayrtn,0},
          {"concentrate",tnkrtn,1},
          {"confide",sayrtn,0},
          {"connect",assemb,1},
          {"cough",whzrtn,0},
          {"crack",crflun,1},
          {"cracker",crkrtn,0},
          {"crackers",crkrtn,0},
          {"crackers?",crkrtn,0},
          {"cry",cryrtn,0},
          {"dance",dncrtn,0},
          {"debrief",unbrtn,0},
          {"decapitate",ntnice,1},
          {"declare",sayrtn,0},
          {"demand",sayrtn,0},
          {"describe",looker,0},
          {"destroy",ntnice,1},
          {"die",diertn,1},
          {"dig",digrtn,1},
          {"direct",aimrtn,1},
          {"disassemble",zdsass,1},
          {"discard",drprtn,1},
          {"disclose",sayrtn,0},
          {"disintegrate",diertn,1},
          {"drawl",sayrtn,0},
          {"drool",drlrtn,0},
          {"drop",drprtn,1},
          {"e",movee,0},
          {"east",movee,0},
          {"eat",ntnice,1},
          {"emphasize",sayrtn,0},
          {"enquire",sayrtn,0},
          {"examine",looker,0},
          {"exclaim",sayrtn,0},
          {"explain",sayrtn,0},
          {"expostulate",sayrtn,0},
          {"exult",yelrtn,1},
          {"eye",looker,0},
          {"fart",frtrtn,1},
          {"feel",looker,0},
          {"fern,",jckpls,0},
          {"fidget",fdgrtn,0},
          {"fit",assemb,1},
          {"fling",throwr,1},
          {"flip",pshrtn,1},
          {"frown",frwrtn,0},
          {"fuck",fckrtn,1},
          {"gasp",gsprtn,0},
          {"get",takrtn,1},
          {"giggle",gigrtn,0},
          {"give",givrtn,1},
          {"glare",glrrtn,0},
          {"go",mvgowk,0},
          {"grab",takrtn,1},
          {"grasp",takrtn,1},
          {"grimace",grirtn,0},
          {"grin",grnrtn,0},
          {"groan",grortn,0},
          {"growl",gwlrtn,0},
          {"grumble",sayrtn,0},
          {"grunt",gntrtn,0},
          {"gurgle",gglrtn,0},
          {"hand",givrtn,1},
          {"harrass",ntnice,1},
          {"heh",snkrtn,0},
          {"help",helper,0},
          {"hint",sayrtn,0},
          {"hiss",hssrtn,0},
          {"hit",pshrtn,1},
          {"holler",yelrtn,1},
          {"hoot",yelrtn,1},
          {"how",whortn,0},
          {"howl",yelrtn,1},
          {"hug",ntnice,0},
          {"hum",humrtn,0},
          {"hurl",throwr,1},
          {"inject:",inject,0},
          {"inquire",sayrtn,0},
          {"insist",sayrtn,0},
          {"inspect",looker,0},
          {"interject",sayrtn,0},
          {"inv",invcmd,0},
          {"inven",invcmd,0},
          {"inventory",invcmd,0},
          {"jabber",sayrtn,0},
          {"jam",assemb,1},
          {"join",assemb,1},
          {"jump",jmprtn,0},
          {"kick",kckrtn,1},
          {"kill",ntnice,1},
          {"kiss",ntnice,1},
          {"laugh",lghrtn,0},
          {"lie",liertn,0},
          {"listen",looker,0},
          {"LONG",unbrtn,0},
          {"look",looker,0},
          {"maim",ntnice,1},
          {"maintain",sayrtn,0},
          {"mention",sayrtn,0},
          {"moan",yelrtn,1},
          {"move",mvgowk,0},
          {"mumble",sayrtn,0},
          {"munch",ntnice,1},
          {"murder",ntnice,1},
          {"murmur",whsrtn,0},
          {"mutter",whsrtn,0},
          {"n",moven,0},
          {"nail",ntnice,1},
          {"nod",nodrtn,0},
          {"nonbrief",unbrtn,0},
          {"north",moven,0},
          {"nudge",ntnice,1},
          {"object",sayrtn,0},
          {"observe",sayrtn,0},
          {"open",opnrtn,1},
          {"opine",sayrtn,0},
          {"pant",pntrtn,0},
          {"pass",givrtn,1},
          {"pick",takrtn,1},
          {"pinch",ntnice,1},
          {"plead",sayrtn,0},
          {"point",aimrtn,1},
          {"poke",ntnice,1},
          {"pout",pourtn,0},
          {"prattle",sayrtn,0},
          {"predict",sayrtn,0},
          {"press",pshrtn,1},
          {"promise",sayrtn,0},
          {"propose",sayrtn,0},
          {"protest",sayrtn,0},
          {"pull",zdsass,1},
          {"punch",ntnice,1},
          {"push",pshrtn,1},
          {"put",assemb,1},
          {"rant",sayrtn,0},
          {"rave",sayrtn,0},
          {"read",reartn,0},
          {"release",drprtn,1},
          {"remark",sayrtn,0},
          {"report",sayrtn,0},
          {"reset",diertn,1},
          {"restart",diertn,1},
          {"run",mvgowk,0},
          {"s",moves,0},
          {"say",sayrtn,0},
          {"scowl",scwrtn,0},
          {"scream",yelrtn,1},
          {"screech",yelrtn,1},
          {"screw",fckrtn,1},
          {"see",looker,0},
          {"shake",ntnice,1},
          {"shoot",ntnice,1},
          {"shout",yelrtn,1},
          {"shove",shvrtn,1},
          {"shrug",srgrtn,0},
          {"shuffle",shfrtn,0},
          {"sigh",sihrtn,0},
          {"sing",yelrtn,1},
          {"sit",sitrtn,0},
          {"slap",ntnice,1},
          {"smack",ntnice,1},
          {"smash",ntnice,1},
          {"smell",looker,0},
          {"smile",smlrtn,0},
          {"smoke",smkrtn,1},
          {"snap",sayrtn,0},
          {"snarl",sayrtn,0},
          {"snatch",takrtn,1},
          {"sneer",snrrtn,0},
          {"sneeze",snzrtn,0},
          {"snicker",snkrtn,0},
          {"sniff",sayrtn,0},
          {"snort",snortn,0},
          {"sob",sayrtn,0},
          {"sock",ntnice,1},
          {"south",moves,0},
          {"speak",sayrtn,0},
          {"split",zdsass,1},
          {"splutter",sayrtn,0},
          {"sputter",sayrtn,0},
          {"squash",ntnice,1},
          {"squawk",yelrtn,1},
          {"squeak",sayrtn,0},
          {"squeal",sayrtn,0},
          {"squeeze",sqzrtn,1},
          {"stab",ntnice,1},
          {"stammer",sayrtn,0},
          {"stand",stnrtn,0},
          {"start",diertn,1},
          {"state",sayrtn,0},
          {"steal",takrtn,1},
          {"stick",assemb,1},
          {"stomp",smprtn,0},
          {"strangle",ntnice,1},
          {"stutter",sayrtn,0},
          {"suggest",sayrtn,0},
          {"surmise",sayrtn,0},
          {"swing",swgrtn,1},
          {"take",takrtn,1},
          {"talk",sayrtn,0},
          {"teleport",tlprtn,1},
          {"tell",sayrtn,0},
          {"think",tnkrtn,1},
          {"threaten",sayrtn,0},
          {"throw",throwr,1},
          {"tickle",ntnice,1},
          {"toss",throwr,1},
          {"torture",ntnice,1},
          {"touch",looker,0},
          {"trade",trdrtn,1},
          {"turn",pshrtn,1},
          {"unbrief",unbrtn,0},
          {"undo",zdsass,1},
          {"up",upyrtn,1},
          {"urge",sayrtn,0},
          {"use",usertn,1},
          {"w",movew,0},
          {"wail",yelrtn,1},
          {"walk",mvgowk,0},
          {"warn",sayrtn,0},
          {"west",movew,0},
          {"what",whortn,0},
          {"wheeze",whzrtn,0},
          {"when",whortn,0},
          {"where",whortn,0},
          {"whine",sayrtn,0},
          {"whisper",whsrtn,0},
          {"whistle",ptwrtn,0},
          {"who",whortn,0},
          {"whoop",yelrtn,1},
          {"why",whortn,0},
          {"wiggle",mvgowk,0},
          {"wink",wnkrtn,0},
          {"write",wrtrtn,1},
          {"yawn",ywnrtn,0},
          {"yell",yelrtn,1},
          {"yelp",yelrtn,1},
          {"yowl",yelrtn,1},
          {"zap",ntnice,1}
     };

     if ((kwcptr=binkwc(urgv[0],stdset,SSTLEN)) != NULL) {
          if (kwcptr->payonl && !hasmkey(FAZKEY)) {
               btuxmt(pyrctr,fazcant);
               return(0);
          }
          return((*(kwcptr->subadr))());
     }
     else {
          return(1);
     }
}
