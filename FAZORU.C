/*****************************************************************************
 *                                                                           *
 *   FAZORU.C   (OBJROU.C)                                                   *
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

static VOID genobj(struct object *pbjptr);
static VOID zorbo(INT modnum);
static VOID wngeat(INT objidx);
static VOID matobj(struct object *atmptr);

VOID
gwing(VOID)                   /* gwingus just wobbles around               */
{
     jabber(FAZ1204);
}

VOID
snuge(VOID)                   /* snuge sends telepathic message            */
{
     jabber(FAZ1300);
}

VOID
klonk(VOID)                   /* dampish generates mongoos                 */
{
     genobj(&objcts[2]);
}

static VOID
genobj(                       /* object generator utility                  */
struct object *pbjptr)
{
     if (pyrptr->sttptr->nobjs < NOBJST) {
          pyrptr->pobjct[objidx]=2;
          mspstg("...A %s drops out ",pbjptr->name);
          mspstg("%s!\r",pyrptr->sttptr->sobjlc);
          gibber();
          mspstg("***\rA %s drops ",pbjptr->name);
          mspstg("%s from ",pyrptr->sttptr->sobjlc);
          addmbf(pyrptr->auxhdl);
          mspstg("'s %s!\r",zbjptr->name);
          fazoth();
          insobs(pbjptr);
     }
     else {
          mspstg("...The %s bulges briefly and then sits still.\r",
                 zbjptr->name);
          gibber();
     }
}

VOID
bilge(VOID)                   /* bilge plays silly message                 */
{
     jabber(FAZ1301);
     fazoth();
}

VOID
bork(VOID)                    /* bork telepathizes hint about coils        */
{
     jabber(FAZ1302);
}

VOID
rhuba(VOID)                   /* rhubarb telepathizes hint about tower     */
{
     jabber(FAZ1303);
}

VOID
moolk(VOID)                   /* moolkoo announces hint about grinfrazzitz */
{
     jabber(FAZ1304);
     fazoth();
}

VOID
wufflar(VOID)                 /* wufflar generates torkii                  */
{
     genobj(&objcts[1]);
}

VOID
suvar(VOID)                   /* suvar generates wigglesnorts              */
{
     genobj(&objcts[4]);
}

VOID
seppe(                        /* sepper dissolves east wall near osmotic   */
struct object *dntcar,
INT narg)
{
     (VOID)dntcar;
     if (narg >= 2 && pyrptr->sttptr == &states[299]
          && stgcmp(urgv[narg],"east") == 0 && states[299].sttmsg != FAZ455) {
          prfmsg(FAZ1206);
          outare(&states[299]);
          prfmsg(FAZ1207);
          outare(&states[304]);
          prfmsg(FAZ1208);
          outare(&states[287]);
          states[299].sttmsg=FAZ455;
          states[304].sttmsg=FAZ456;
          states[299].east=&states[304];
     }
}

VOID
palt(                         /* palt restores east wall near osmotic      */
struct object *dntcar,
INT narg)
{
     (VOID)dntcar;
     if (narg >= 2 && pyrptr->sttptr == &states[299]
          && stgcmp(urgv[narg],"east") == 0 && states[299].sttmsg == FAZ455) {
          prfmsg(FAZ1211);
          outare(&states[299]);
          prfmsg(FAZ1212);
          outare(&states[304]);
          prfmsg(FAZ1213);
          outare(&states[287]);
          states[299].sttmsg=FAZ299;
          states[304].sttmsg=FAZ304;
          states[299].east=NULL;
     }
}

VOID
cistl(VOID)                   /* cistle generates snuges                   */
{
     genobj(&objcts[3]);
}

VOID
rommu(VOID)                   /* rommus gives silly message                */
{
     jabber(FAZ1306);
}

VOID
rucku(VOID)                   /* ruckus announces poor joke                */
{
     jabber(FAZ1307);
     fazoth();
}

VOID
bundl(VOID)                   /* bundle telepathizes hint about green lights */
{
     jabber(FAZ1308);
}

VOID
nabob(VOID)                   /* nabob generates gwingii                   */
{
     genobj(&objcts[0]);
}

VOID
bagl3(                        /* level-3 bagloon duplicates level 1-2 objs */
struct object *atmptr,
INT typcod)
{
     if (typcod == 0 && pyrptr->sttptr->nobjs < NOBJST
      && atmptr-objcts < 15) {
          matobj(atmptr);
     }
}

VOID
bagl4(                        /* level-4 bagloon duplicates level 1-3 objs */
struct object *atmptr,
INT typcod)
{
     if (typcod == 0 && pyrptr->sttptr->nobjs < NOBJST
      && atmptr-objcts < 25) {
          matobj(atmptr);
     }
}

VOID
bagl5(                        /* level-5 bagloon duplicates level 1-4 objs */
struct object *atmptr,
INT typcod)
{
     if (typcod == 0 && pyrptr->sttptr->nobjs < NOBJST
      && atmptr-objcts < 35) {
          matobj(atmptr);
     }
}

VOID
dweez(VOID)                   /* dweezle implodes takable items lying about */
{
     INT i;

     jabber(FAZ1239);
     prfmsg(FAZ1238);
     fazoth();
     for (i=0 ; i < pyrptr->sttptr->nobjs ; ) {
          if (((zbjptr=pyrptr->sttptr->sttobj[i])->flags&OBJTKB)
           && zbjptr-objcts > 4) {
               mspstg("***\rThe %s that was",zbjptr->name);
               mobjps(zbjptr);
               addmbf(" just imploded with a loud pop!\r");
               gibber();
               fazoth();
               rmvobs(i);
          }
          else {
               i+=1;
          }
     }
}

static VOID
zorbo(                        /* kootpod turns into another obj at random  */
INT modnum)
{
     CHAR *objnam;

     pyrptr->pyrobj[objidx]=&objcts[(getrnd()&0x7fff)%modnum];
     pyrptr->pobjct[objidx]=0;
     objnam=pyrptr->pyrobj[objidx]->name;
     mspstg("...The kootpod suddenly turns into a %s!\r",objnam);
     gibber();
     mspstg("***\r%s's kootpod suddenly turned into a ",pyrptr->auxhdl);
     mspstg("%s!\r",objnam);
     fazoth();
}

VOID
koot4(VOID)                   /* level-4 kootpod turns into a level 1-3    */
{
     zorbo(25);
}

VOID
koot5(VOID)                   /* level-5 kootpod turns into a level 1-4    */
{
     zorbo(44);
}

VOID
koot6(VOID)                   /* level-6 kootpod turns into a level 1-5    */
{
     zorbo(55);
}

VOID
kwish(VOID)                   /* kwish displays hint about monoliths       */
{
     jabber(FAZ1305);
}

VOID
quoru(VOID)                   /* quorum gives hint about dongle            */
{
     jabber(FAZ1309);
}

VOID
cacap(VOID)                   /* gives clue about building nebbish         */
{
     jabber(FAZ1319);
     fazoth();
     rmvobp(pyrptr,objidx);
}

VOID
wappl(VOID)                   /* wapple gives hint about finding Orb       */
{
     jabber(FAZ1310);
     fazoth();
}

VOID
nebbi(                        /* nebbish gets hot/cold due to relative bps */
struct player *othpyr_local,
INT typcod)
{
     if (typcod == 1) {
          if (usroff(othpyr_local->portno)->baud > usrptr->baud) {
               jabber(FAZ1216);
          }
          else if (usroff(othpyr_local->portno)->baud < usrptr->baud) {
               jabber(FAZ1217);
          }
     }
}

VOID
wongu(VOID)                   /* wongum waits five moves then eats object  */
{
     jabber(FAZ1219);
     pyrptr->pobjct[objidx]=5;
}

VOID
wongo(VOID)                   /* routine to eat obj and wait 5 more moves  */
{
     pyrptr->pobjct[objidx]=5;
     if (pyrptr->nobjs >= 2) {
          if (pyrptr->pyrobj[0] != &objcts[49]) {
               wngeat(0);
          }
          else {
               wngeat(1);
          }
     }
}

static VOID
wngeat(                       /* wongum object eater utility               */
INT objidx_local)
{
     mspstg("***\rThe wongum just ate your %s!\r",
            pyrptr->pyrobj[objidx_local]->name);
     gibber();
     mspstg("***\r%s's wongum just ate ",pyrptr->auxhdl);
     addmbf(zhisher(pyrptr));
     mspstg(" %s!\r",pyrptr->pyrobj[objidx_local]->name);
     fazoth();
     rmvobp(pyrptr,objidx_local);
}

VOID
narre(VOID)                   /* narret gives misc high-level hints        */
{
     jabber(FAZ1311);
}

VOID
drive(VOID)                   /* drivel gives hint about Galaxenter        */
{
     jabber(FAZ1312);
}

VOID
pasta(VOID)                   /* narn gives hint about quilbert/gwirg      */
{
     jabber(FAZ1313);
}

static VOID
matobj(                       /* materialize-object utility                */
struct object *atmptr)
{
     fnobjp(urgv[1],pyrptr);
     pyrptr->pobjct[objidx]=2;
     mspstg("***\rAnother %s suddenly materializes ",atmptr->name);
     mspstg("%s!\r",pyrptr->sttptr->sobjlc);
     outare(pyrptr->sttptr);
     insobs(atmptr);
}

VOID
noodl(VOID)                   /* misuse of noodle beams mix of impressions */
{
     jabber(FAZ1314);
}

VOID
scroo(                        /* scroom makes victim drop possessions      */
struct player *othpyr_local,
INT typcod)
{
     INT i;

     if (typcod == 1) {
          fnobjp(urgv[1],pyrptr);
          pyrptr->pobjct[objidx]=10;
          if (hldoth(&objcts[48])) {
               mspstg("***\rA ray shoots toward you from %s's",pyrptr->auxhdl);
               addmbf(" scroom, but your quahog absorbs it!\r");
               outpyr(othpyr_local);
               mspstg("***\rA ray shoots out of the scroom, but %s's",
                      othpyr_local->auxhdl);
               addmbf(" quahog absorbs it!\r");
               gibber();
               mspstg("***\rA ray shoots toward %s",othpyr_local->auxhdl);
               mspstg(" from %s's scroom, but",pyrptr->auxhdl);
               mspstg(" %s's quahog absorbs it!\r",othpyr_local->auxhdl);
               outexc(othpyr_local);
          }
          else {
               for (i=0 ; i < othpyr_local->nobjs ; ) {
                    if (pyrptr->sttptr->nobjs >= NOBJST) {
                         break;
                    }
                    mspstg("***\rYour %s suddenly falls ",
                           othpyr_local->pyrobj[i]->name);
                    mspstg("%s!\r", othpyr_local->sttptr->sobjlc);
                    outpyr(othpyr_local);
                    mspstg("***\r%s just dropped a ", othpyr_local->auxhdl);
                    addmbf(othpyr_local->pyrobj[i]->name);
                    mspstg(" %s!\r", othpyr_local->sttptr->sobjlc);
                    gibber();
                    outexc(othpyr_local);
                    insobs(othpyr_local->pyrobj[i]);
                    rmvobp(othpyr_local,i);
               }
          }
     }
}

VOID
fashn(VOID)                   /* fashnork turns player invisible           */
{
     jabber(FAZ1245);
     pyrptr->pobjct[objidx]=15;
     mspstg("***\r%s",pyrptr->auxhdl);
     prfmsg(FAZ1247);
     fazoth();
     strcpy(pyrptr->auxhdl,iszleen(pyrptr) ? "Some unseen zleen"
                                           : "Some unseen force");
}

VOID
fasho(VOID)                   /* fashnork explodes if used too long        */
{
     jabber(FAZ1243);
     prfmsg(FAZ1244);
     fazoth();
     rmvobp(pyrptr,objidx);
}

VOID
fashoo(                       /* fashnork voluntary turn-off handler       */
struct player *pyrp)
{
     if (invis(pyrp)) {
          prfmsg(FAZ1246);
          outpyr(pyrp);
          prfmsg(FAZ1248);
          strcpy(pyrp->auxhdl,pyrp->plyrid);
          mspstg("%s appears!\r",pyrp->auxhdl);
          if (pyrp == pyrptr) {
               fazoth();
          }
          else {
               gibber();
               outexc(pyrp);
          }
     }
}

VOID
quice(VOID)                   /* quice gives clue about trail to nofzinger */
{
     jabber(FAZ1317);
}

VOID
snaac(VOID)                   /* snaack gives clue about trail to blub     */
{
     jabber(FAZ1318);
}

VOID
babba(                        /* babbalisk grabs victim's stuff            */
struct player *pyr,
INT typcod)
{
     if (typcod == 1) {
          othpyr=pyr;
          if (hldoth(&objcts[51])) {
               prfmsg(FAZ1250);
               addmbf(othpyr->auxhdl);
               prfmsg(FAZ1251);
               gibber();
               prfmsg(FAZ1252);
               addmbf(pyrptr->auxhdl);
               prfmsg(FAZ1253);
               outpyr(othpyr);
               prfmsg(FAZ1254);
               addmbf(pyrptr->auxhdl);
               prfmsg(FAZ1255);
               addmbf(othpyr->auxhdl);
               prfmsg(FAZ1256);
               outexc(othpyr);
          }
          else if (pyrptr->nobjs >= NOBJHD || othpyr->nobjs == 0) {
               jabber(FAZ1257);
               prfmsg(FAZ1258);
               addmbf(pyrptr->auxhdl);
               prfmsg(FAZ1259);
               fazoth();
          }
          else {
               prfmsg(FAZ1260);
               addmbf(othpyr->auxhdl);
               mspstg("'s %s",othpyr->pyrobj[0]->name);
               prfmsg(FAZ1261);
               mspstg("%s in your hands!\r",othpyr->pyrobj[0]->name);
               gibber();
               prfmsg(FAZ1262);
               addmbf(pyrptr->auxhdl);
               prfmsg(FAZ1263);
               addmbf(othpyr->pyrobj[0]->name);
               prfmsg(FAZ1264);
               addmbf(othpyr->pyrobj[0]->name);
               mspstg(" in %s's hands!\r",pyrptr->auxhdl);
               outpyr(othpyr);
               prfmsg(FAZ1265);
               addmbf(pyrptr->auxhdl);
               prfmsg(FAZ1266);
               addmbf(othpyr->auxhdl);
               mspstg("'s %s",othpyr->pyrobj[0]->name);
               prfmsg(FAZ1267);
               addmbf(othpyr->pyrobj[0]->name);
               mspstg(" in %s's hands!\r",pyrptr->auxhdl);
               outexc(othpyr);
               insobp(pyrptr,othpyr->pyrobj[0],othpyr->pobjct[0]);
               rmvobp(othpyr,0);
          }
     }
}

VOID
quilbert(                     /* quilbert picks up gwirg                   */
struct object *atmptr,
INT typcod)
{
     if (typcod == 0 && atmptr == &objcts[116]) {
          jabber(FAZ1545);
          prfmsg(FAZ1546);
          mspstg("%s's hands!\r",pyrptr->auxhdl);
          mspstg("***\rThe gwirg just ate %s's quilbert!\r",pyrptr->auxhdl);
          fazoth();
          fnobjp("quilbert",pyrptr);
          pyrptr->pyrobj[objidx]=&objcts[116];
          fnobjs("gwirg");
          rmvobs(objidx);
     }
}

VOID
quilb(VOID)                   /* gwirg is pointed up to enter tower        */
{
     if (urgc < 3) {
          ntnice();
          return;
     }
     if (pyrptr->sttptr == &states[301] && stgcmp(urgv[2],"up") == 0) {
          lvstt("floated up, up, and away into the darkness above");
          jabber(FAZ1225);
          ntrstt(&states[313],"floated up through the floor");
     }
     else if (pyrptr->sttptr == &states[313]
           && stgcmp(urgv[2],"down") == 0) {
          lvstt("sunk down into the floor, and vanished");
          jabber(FAZ1226);
          ntrstt(&states[301],"floated down out of the darkness above you");
     }
     else if (stgcmp(urgv[2],"up") == 0) {
          jabber(FAZ1227);
     }
}

VOID
zarpnt(                       /* zarkon is pointed at oolgorboid to free it */
struct object *atmptr,
INT typcod)
{
     if (typcod == 0 && atmptr-objcts == 117 && atmptr->flags == OBJNVS) {
          atmptr->flags=OBJTKB;
          atmptr->name="free oolgorboid";
          states[238].sttmsg=FAZ120;
          jabber(OOGFRE);
          fazoth();
     }
}
