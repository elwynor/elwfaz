###############################################################################
#####               This makefile generates ELWFAZ.DLL                    #####
###############################################################################
!include $(WGDEV)\src\gcrules.mak

VIRDIR = .\GCVIRDAT

PARSEOPTS = $(PARSEOPTS) -DIS30

DEFILE = 

VIRFILES =          \
     GCVIRDAT\elwfaz.vir

DLLLIB =            \
     $(DLLLIB)      \
     wgserver.lib   \
     galgsbl.lib

OBJFILES =          \
     fazcax.obj     \
     fazcst.obj     \
     fazcut.obj     \
     fazgam.obj     \
     fazgob.obj     \
     fazgst.obj     \
     fazgut.obj     \
     fazmst.obj     \
     fazoru.obj     \
     fazrtk.obj     \
     fazsro.obj     \
     fazuul.obj

elwfaz.dll:         \
     $(GCBUILD)     \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
