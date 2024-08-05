/*****************************************************************************
 *                                                                           *
 *   FAZDAT.H                                                                *
 *                                                                           *
 *   This is the header file for all FAZUUL game sources.                    *
 *                                                                           *
 *                                                 - T. Stryker 1/24/91      *
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

#define USERSZ 30             /* User ID Size                          */
#define ALSSIZ 10             /* maz size of alias name                */
#define AUXSIZ 18             /* max size of auxilliary handle (SUF)   */
#define NVSTMN 5              /* number of visited-rooms maintained    */
#define NOBJHD 6              /* max no. of objects player can hold    */
#define NOBJST 6              /* max no. of objs to fit in one state   */
#define NOBJMS 4              /* max no. of mothers an object can have */
#define SPRSIZ 36             /* amt of spare space, dec when adding   */

#define SGOBIT 0x01           /* pyrptr->logon bit for signed on       */
#define SEXBIT 0x02           /* pyrptr->logon bit for sex (m=1, f=0)  */
#define INGAME 0x04           /* pyrptr->logon bit for in game         */
#define BRFBIT 0x20           /* pyrptr->logon bit for brief descrip   */

#define FLOBIT 0x01           /* pyrptr->newflg bit for first logon    */

#define TOTOBJ 154            /* number of different objects */
#define TOTSTT 353            /* number of different states  */

/*
Player struct: 132

handle: 0
plyrid: 30
sttptr: 40
nxtpyr: 44
pyrobj: 48
logon: 72
crackl: 74
portno: 76
newflg: 78
vsttes: 80
vages: 90
nobjs: 100
pobjct: 102
auxhdl: 114
*/

//extern /* removed due to VS2019 by Rick 4/18/23 */
struct player {                    /* player-specific data:                */
     CHAR handle[USERSZ];               /* User-ID                         */ // 000-029
     CHAR plyrid[ALSSIZ];               /* Alias Name                      */ // 030-039
     struct state *sttptr;              /* room struc ptr that player is in*/ // 040-043
     struct player *nxtpyr;             /* room linked list link ptr       */ // 044-047
     struct object *pyrobj[NOBJHD];     /* ptrs to obj strucs of held objs */ // 048-071
     SHORT logon;                       /* flag for player-logged-on       */ // 072-073
     SHORT crackl;                      /* crackers left                   */ // 074-075
     SHORT portno;                      /* port number that player is on   */ // 076-077
     SHORT newflg;                      /* "new" player - first-logon bit  */ // 078-079
     SHORT vsttes[NVSTMN];              /* recently-visited-states codes   */ // 080-089
     SHORT vages[NVSTMN];               /* recently-visited-states ages    */ // 090-099
     SHORT nobjs;                       /* number of objects in hands      */ // 100-101
     SHORT pobjct[NOBJHD];              /* command-counter per object held */ // 102-113
     CHAR auxhdl[AUXSIZ];               /* "auxillary" handle              */ // 114-131
}; // size 132

extern
struct state {                     /* state (room) specific data:          */
     CHAR *sdscrp;                      /* state description string        */
     CHAR *sobjlc;                      /* object "location" within state  */
     INT sttmsg;                        /* msg no. of state long descrip.  */
     INT (*cmhdlr)(VOID);               /* state-specific command handler  */
     struct player *plhead;             /* player linked-list header       */
     INT nobjs;                         /* number of objects in state      */
     struct object *sttobj[NOBJST];     /* ptrs to obj strucs of room objs */
     struct state *north;               /* state northward (NULL if none)  */
     struct state *south;               /* state southward (NULL if none)  */
     struct state *east;                /* state eastward (NULL if none)   */
     struct state *west;                /* state westward (NULL if none)   */
} states[TOTSTT];                  /* state array defined in FAZGST.C      */

extern
struct object {                    /* object (item) specific data:         */
     CHAR *name;                        /* object name                     */
     CHAR *positn;                      /* position sub-description        */
     INT flags;                         /* bit flags (see below)           */
     CHAR *cntake;                      /* if not takable and take attempt */
     INT objmsg;                        /* msg no. of object long descrip. */
     VOID (*actvec)(VOID);              /* activation vector               */
     VOID (*objtvc)(VOID);              /* timeout (pobjct==0) vector      */
     struct object *mothrs[NOBJMS];     /* objs that this obj makes        */
     struct object *dghtr1;             /* object 1 that obj is made of    */
     struct object *dghtr2;             /* object 2 that obj is made of    */
} objcts[TOTOBJ];                  /* object array defined in FAZGOB.C     */

typedef VOID (*voidfunc)(VOID);    /* void function type                   */
typedef VOID (*objact)(struct object *,INT); /* object action vector type  */
typedef VOID (*plract)(struct player *,INT); /* player action vector type  */

#define OBJNVS 1                 /* zbjptr->flags bit for invisible object */
#define OBJTKB 2                 /* zbjptr->flags bit for takable object   */
#define OBJRDA 4                 /* zbjptr->flags bit for readable object  */
#define ACTBUT 0x0100            /* zbjptr->flags activate via button      */
#define ACTPNT 0x0200            /* zbjptr->flags activate by pointing     */
#define ACTKCK 0x0400            /* zbjptr->flags activate by kicking      */
#define ACTTNK 0x0800            /* zbjptr->flags activate by thinking abt */

extern
struct player *pyrarr,        /* dynam alloc'd player data structure array */
              *pyrptr,        /* ptr to current user's pyrarr element      */
              *othpyr;        /* "other" player ptr -- for actions etc.    */
extern
struct state  *sttptr;        /* global scratch vbl for room structure ptr */
extern
struct object *zbjptr;        /* global scratch vbl for object (item) ptr  */
extern
INT pyrctr,                   /* "player counter" - same as usrnum in svr  */
    urgc;                     /* argument count - same as margc in server  */
extern
CHAR pyrcmd[256],             /* "player command" - same as input in server*/
     *fzcptr,                 /* global scratch vbl for scanning pyrcmd    */
     *urgv[64],               /* argument values - same as margv in server */
     *arge[65],               /* argument ends - same as marge in server   */
     *fazcant;                /* not allowed to play FAZUUL fully (msg)    */
extern
INT intmp1,                 /* scratch temporary vbl 1                   */
      intmp2,                 /* scratch temporary vbl 2                   */
      objidx,                 /* global scratch vbl for object indexes     */
      noistt;                 /* flag to whther() for uncond output        */

extern
struct kwtab {                /* keyword table data structure layouts      */
     CHAR *kyword;
     INT (*subadr)(VOID);
} *kwtptr;
extern
struct kwntab {
     CHAR *kyword;
     INT number;
} *kwnptr;

// This is from V10, backporting it into WG32 build here
#ifndef CHAR_BUFFER
#define CHAR_BUFFER(name, size, lit)     CHAR name[size] = lit
#endif 

/* FAZCAX.C 19/03/92 15.56.12 */
INT diertn(VOID);
INT crflun(VOID);
INT crkrtn(VOID);
INT sayrtn(VOID);
INT yelrtn(VOID);
INT whsrtn(VOID);
INT moven(VOID);
INT moves(VOID);
INT movee(VOID);
INT movew(VOID);
INT shoven(VOID);
INT shoves(VOID);
INT shovee(VOID);
INT shovew(VOID);
INT takrtn(VOID);
VOID invrtn(VOID);
INT givrtn(VOID);

/* FAZCUT.C 19/03/92 17.34.26 */
VOID game(VOID);
INT move(struct state *dstspt,const char *lvmsg,const char *entmsg);
INT shove(struct state *dstspt,const char *lvdir,const char *entmsg);
VOID ntrstt(struct state *svcptr,const char *txtptr);
VOID whoher(VOID);
VOID whther(VOID);
VOID mobjps(struct object *local_zbjptr);
VOID lvstt(const char *txtptr);
VOID zapwrd(CHAR *word);
VOID donthv(VOID);
VOID ainthr(INT argno);
INT holdng(struct object *obj);
INT hold(struct object *obj);
INT hldoth(struct object *obj);
INT invis(struct player *pyrp);
INT iszleen(struct player *pyrp);

/* FAZGAM.C 19/03/92 17.38.02 */
INT foon(VOID);
INT umflun(VOID);
INT osmo(VOID);
VOID osmwat(VOID);
VOID gwirg(VOID);

/* FAZGUT.C 19/03/92 17.41.04 */
VOID ucify(CHAR *stgptr);
SHORT getrnd(VOID);
VOID addmbf(const char *stgptr);
VOID mspstg(const char *stgptr,...);
VOID prscmd(VOID);
VOID recomb(VOID);
INT nmatch(CHAR *trmstg,CHAR *untstg);
INT stgcmp(CHAR *cmdstg,CHAR *posmat);

/* FAZMST.C 19/03/92 17.42.42 */
VOID jabber(INT msgnum);
VOID gibber(VOID);
VOID outnis(VOID);
VOID fazall(VOID);
VOID fazoth(VOID);
VOID outpyr(struct player *othpyr);
VOID outexc(struct player *ptrpyr);
VOID outner(VOID);
VOID fazfar(VOID);
VOID outare(struct state *spoint);

/* FAZORU.C 19/03/92 17.47.34 */
VOID gwing(VOID);
VOID snuge(VOID);
VOID klonk(VOID);
VOID bilge(VOID);
VOID bork(VOID);
VOID rhuba(VOID);
VOID moolk(VOID);
VOID wufflar(VOID);
VOID suvar(VOID);
VOID seppe(struct object *dntcar,INT narg);
VOID palt(struct object *dntcar,INT narg);
VOID cistl(VOID);
VOID rommu(VOID);
VOID rucku(VOID);
VOID bundl(VOID);
VOID nabob(VOID);
VOID bagl3(struct object *atmptr,INT typcod);
VOID bagl4(struct object *atmptr,INT typcod);
VOID bagl5(struct object *atmptr,INT typcod);
VOID dweez(VOID);
VOID koot4(VOID);
VOID koot5(VOID);
VOID koot6(VOID);
VOID kwish(VOID);
VOID quoru(VOID);
VOID cacap(VOID);
VOID wappl(VOID);
VOID nebbi(struct player *othpyr,INT typcod);
VOID wongu(VOID);
VOID wongo(VOID);
VOID narre(VOID);
VOID drive(VOID);
VOID pasta(VOID);
VOID noodl(VOID);
VOID scroo(struct player *othpyr,INT typcod);
VOID fashn(VOID);
VOID fasho(VOID);
VOID fashoo(struct player *pyrp);
VOID quice(VOID);
VOID snaac(VOID);
VOID babba(struct player *pyr,INT typcod);
VOID quilbert(struct object *atmptr,INT typcod);
VOID quilb(VOID);
VOID zarpnt(struct object *atmptr,INT typcod);

/* FAZRTK.C 20/03/92 10.33.34 */
VOID zinirtk(VOID);
VOID zrtkick(INT time,VOID (*rouptr)(INT),INT parm);
VOID zrtksvc(VOID);

/* FAZSRO.C 20/03/92 09.33.58 */
VOID inists(VOID);
INT nocmd(VOID);
INT cm108(VOID);
INT cm152(VOID);
INT cm77(VOID);
INT cm251(VOID);
INT cm284(VOID);
INT cm137(VOID);
INT cm104(VOID);
INT cm109(VOID);
INT cm113(VOID);
INT cm116(VOID);
INT cm193(VOID);
INT cm290(VOID);
INT cm330(VOID);
INT cm351(VOID);
INT sltchk(struct object *objps1,struct object *objps2,struct object *
        objps3,CHAR *apstg);
INT cm302(VOID);
INT cm303(VOID);
INT cm306(VOID);
INT cm307(VOID);
INT cm310(VOID);
INT cm311(VOID);
INT cm312(VOID);
INT cm318(VOID);
INT cm352(VOID);
INT cm321(VOID);
INT cm320(VOID);
INT cm322(VOID);
INT cm331(VOID);
INT cm333(VOID);
INT cm334(VOID);
INT cm336(VOID);
INT cm337(VOID);
INT cm338(VOID);
INT cm343(VOID);
INT cm345(VOID);
INT cm349(VOID);

/* FAZUUL.C 20/03/92 09.39.54 */
VOID EXPORT init__fazuul(VOID);

/* FAZCST.C 20/03/92 11.13.02 */
CHAR *zhisher(struct player *pyrp);
CHAR *zheshe(struct player *pyrp);
struct state *fndpyr(CHAR *hdlstg);
struct player *fndhdl(CHAR *schstg);
struct object *fnobjp(CHAR *stgptr,struct player *local_pyrptr);
struct object *sfnobjs(CHAR *stgptr,struct state *statpt);
struct object *fnobjs(CHAR *stgptr);
VOID rmvobp(struct player *local_pyrptr,INT wchone);
VOID insobp(struct player *local_pyrptr,struct object *local_zbjptr,SHORT count);
VOID srmvobs(INT wchone,struct state *statpt);
VOID rmvobs(INT wchone);
VOID insobs(struct object *local_zbjptr);
INT ntnice(VOID);
VOID zntifo(VOID);
INT zdsass(VOID);
VOID infoth(CHAR *infstg);
struct kwtab *fazbin(CHAR *stgptr,struct kwtab table[],INT length);
INT stdcmd(VOID);
