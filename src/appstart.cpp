/* --------------------------------------------------------------------
   appstart.cpp
   C++ Application Startup Parameters
   Copyright 2006, Control Microsystems Inc.
   -------------------------------------------------------------------- */

#include "ctools.h"
#include "nvMemory.h"

/* --------------------------------------------------------------------
   Global Variables
   -------------------------------------------------------------------- */
// These parameters are used when the task main() is created.

// Priority of the task main().
// Priority 100 is recommended for a continuously running task.
// A task with priority > 100 will never be given the CPU. See manual for details.
UINT32 mainPriority = 100;

// Stack space allocated to the task main().
// Note that at least 5 stack blocks are needed when calling fprintf().
UINT32 mainStack = 16;

// Application group assigned to the task main().
// A unique value is assigned by the system to the applicationGroup 
// for this application. Use this variable in all calls to create_task()
// by this application. See manual for details.
UINT32 applicationGroup = 0;

// Pointer to static non-volatile data.
// Define the structure NV_MEMORY in nvMemory.h
NV_MEMORY * pNvMemory = NULL;

// Size of structure in static non-volatile memory
UINT32 nvMemorySize = sizeof(NV_MEMORY);

// applicationType and applicationTypeLimit may be used to limit
// the number of executable instances of this application.
// Valid values for applicationType are 0 to 65535. Default type is 0.
// Valid values for applicationTypeLimit are 0 to 32. 
// Default limit is 0 which = no limit
UINT32 applicationType      = 0; 	// valid types : 0 to 65535
UCHAR  applicationTypeLimit = 1;	// valid limits: 0 to 32; 0 = no limit

/* --------------------------------------------------------------------
   appstart

   This function is the initial C++ Application startup task. The 
   function initializes the pointer to static non-volatile data and 
   calls the user-written function main(). 
   -------------------------------------------------------------------- */
void appstart(void)
{
	void *  pAddress;	// pointer to snvram section assigned to this application
	UINT32  size;		// size of snvram section assigned to this application
	BOOLEAN	status;

	status = getSnvramAddress((FUNCPTR) main, &pAddress, &size);
	if (status)
	{
		// initialize pointer to static non-volatile data
		pNvMemory = (NV_MEMORY *) pAddress;
	}

	// call user application
	main();

	// end this application group
	end_group(applicationGroup);
}
