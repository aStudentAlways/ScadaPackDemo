# -----------------------------------------------------------------------------
# makefile
#
# Make file for SCADAPack C++ Tools application for TelePACE firmware
# Copyright 2015 Schneider Electric Canada Inc.
#
# usage:
#    make                      - makes application
#    make TARGET=SCADAPack350  - link just for SCADAPack 350
#    make TARGET=SCADAPack33x  - link just for SCADAPack 330 and 334 
#    make TARGET=SCADAPack31x  - link just for SCADAPack 314
#    make TARGET=SCADAPack4203 - link just for SCADAPack 4203 
#    make TARGET=NOCT60        - link just for Foxboro NOCT60
#    make clean                - deletes output files
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
# set the name of the output file here
# -----------------------------------------------------------------------------

APPLICATION_NAME = demo

# -----------------------------------------------------------------------------
# list all object files here
# -----------------------------------------------------------------------------

objects = appstart.o main.o debug.o

# -----------------------------------------------------------------------------
# controllers to build
# -----------------------------------------------------------------------------

TARGET = SCADAPack350 SCADAPack33x SCADAPack31x SCADAPack4203 NOCT60

# -----------------------------------------------------------------------------
# set location of C Tools files
# -----------------------------------------------------------------------------

# take the C Tools path from the environment, or set default if it's not there (default may not be correct for all installations)
ifeq ($(strip $(CTOOLS_PATH)),)
#CTOOLS_PATH = C:\Program Files\Control Microsystems\CTools
CTOOLS_PATH = C:\CTools
endif

# -----------------------------------------------------------------------------
# set location of TelePACE specific files
# -----------------------------------------------------------------------------

INCLUDE_PATH = $(CTOOLS_PATH)\Controller\TelePACE

# -----------------------------------------------------------------------------
# compiler flags
# -----------------------------------------------------------------------------

CFLAGS = -O3 -Wall -mapcs-32 -mlittle-endian -march=armv4 -ansi -fno-builtin -DARMEL -I"$(INCLUDE_PATH)" -DCPU=ARMARCH4 -DTOOL_FAMILY=gnu -DTOOL=gnu -std=c99

# -----------------------------------------------------------------------------
# list of file suffixes used in this makefile
# -----------------------------------------------------------------------------

.SUFFIXES:
.SUFFIXES: .cpp .c .o .out

# -----------------------------------------------------------------------------
# determine intermediate link target(s) used to check
# if all symbols can be resolved in firmware
# -----------------------------------------------------------------------------

stripTarget = $(strip $(TARGET))

ifneq ($(findstring SCADAPack350,$(stripTarget)),)
intermediate_objects += imLink_SCADAPack350.o
endif

ifneq ($(findstring SCADAPack33x,$(stripTarget)),)
intermediate_objects += imLink_SCADAPack33x.o
endif

ifneq ($(findstring SCADAPack31x,$(stripTarget)),)
intermediate_objects += imLink_SCADAPack31x.o
endif

ifneq ($(findstring SCADAPack4203,$(stripTarget)),)
intermediate_objects += imLink_SCADAPack4203.o
endif

ifneq ($(findstring NOCT60,$(stripTarget)),)
intermediate_objects += imLink_NOCT60.o
endif
 
# -----------------------------------------------------------------------------
# rules for making .out file
# -----------------------------------------------------------------------------

$(APPLICATION_NAME).out : imImage.o $(intermediate_objects)
# Process CPP constructors and destructors
	@echo
	@echo --------------------
	@echo Building output file
	@echo --------------------
	nmarm imImage.o | "$(CTOOLS_PATH)\Arm7\tcl\bin\tclsh84.exe" "$(CTOOLS_PATH)\Arm7\host\x86-win32\bin\munch.tcl" -c arm > ctdt.c
	ccarm $(CFLAGS) -c -fdollars-in-identifiers ctdt.c -o ctdt.o
	
# Link downloadable application.
	ccarm -I. -r -nostdlib -Wl,-X -Wl,-EL -T "$(CTOOLS_PATH)\Arm7\target\h\tool\gnu\ldscripts\link.OUT" imImage.o ctdt.o -o $(APPLICATION_NAME).out

# Clean up temporary files
	del ctdt.c ctdt.o

# -----------------------------------------------------------------------------
# rules for making intermediate objects
# -----------------------------------------------------------------------------

imImage.o: $(objects) 
# Merge all object files into one
	ccarm -I. -r -nostdlib -Wl,-X -Wl,-EL -Wl $(objects) -o imImage.o
	
# -----------------------------------------------------------------------------
# link with controller specific CTools library to check for unresolved externals
# -----------------------------------------------------------------------------

imLink_SCADAPack350.o: imImage.o
	@echo
	@echo -------------------------------------------------------------------
	@echo Checking for unresolved externals with SCADAPack 350 CTools library
	@echo -------------------------------------------------------------------
	ldarm -e0 imImage.o "$(INCLUDE_PATH)\SCADAPack_350_TelePACE_Firmware_Image" -o imLink_SCADAPack350.o

imLink_SCADAPack33x.o: imImage.o
	@echo
	@echo -------------------------------------------------------------------
	@echo Checking for unresolved externals with SCADAPack 33x CTools library
	@echo -------------------------------------------------------------------
	ldarm -e0 imImage.o "$(INCLUDE_PATH)\SCADAPack_33x_TelePACE_Firmware_Image" -o imLink_SCADAPack33x.o

imLink_SCADAPack31x.o: imImage.o
	@echo
	@echo -------------------------------------------------------------------
	@echo Checking for unresolved externals with SCADAPack 31x CTools library
	@echo -------------------------------------------------------------------
	ldarm -e0 imImage.o "$(INCLUDE_PATH)\SCADAPack_31x_TelePACE_Firmware_Image" -o imLink_SCADAPack31x.o

imLink_SCADAPack4203.o: imImage.o
	@echo
	@echo --------------------------------------------------------------------
	@echo Checking for unresolved externals with SCADAPack 4203 CTools library
	@echo --------------------------------------------------------------------
	ldarm -e0 imImage.o "$(INCLUDE_PATH)\SCADASense_4203_TelePACE_Firmware_Image" -o imLink_SCADAPack4203.o

imLink_NOCT60.o: imImage.o
	@echo
	@echo ------------------------------------------------------------
	@echo Checking for unresolved externals with NOCT60 CTools library
	@echo ------------------------------------------------------------
	ldarm -e0 imImage.o "$(INCLUDE_PATH)\Noct60_Telepace_Firmware_Image" -o imLink_NOCT60.o

# -----------------------------------------------------------------------------
# list all source file dependencies here
# -----------------------------------------------------------------------------

appstart.o: appstart.cpp nvMemory.h
main.o:     main.cpp nvMemory.h
debug.o: debug.cpp

# -----------------------------------------------------------------------------
# rules for making files
# -----------------------------------------------------------------------------

%.o : %.c
	ccarm $(CFLAGS) -c $< -o $@

%.o : %.cpp
	ccarm $(CFLAGS) -c $< -o $@

# -----------------------------------------------------------------------------
# clean up all output files
# -----------------------------------------------------------------------------
.PHONY: clean
clean:
	del *.o
	del *.out


