/* --------------------------------------------------------------------
   SCADAPack 350 / SCADASense 4203 C++ Application Main Program
   Copyright 2007, Control Microsystems Inc.
   -------------------------------------------------------------------- */
/* --------------------------------------------------------------------
   SCADAPack 350 / SCADASense 4203 C++ Application Main Program
   Copyright 2007, Control Microsystems Inc.
   -------------------------------------------------------------------- */
#include "ctools.h"
#include "nvMemory.h"
#include "debug.h"

/* --------------------------------------------------------------------
   C++ Function Prototypes
   -------------------------------------------------------------------- */
// add prototypes here

/* --------------------------------------------------------------------
   C Function Prototypes
   -------------------------------------------------------------------- */
extern "C"
{
	// add prototypes here
}

#define MAIN_EVENT (0)
#define MAIN_EVENT_INTERVAL	(10) // in 100s of milliseconds, 10 = 1 second

#ifdef ENABLE_DEBUG
extern char dbgMsg[];
#endif

void Shutdown(void);
void Initialize(void);

/* --------------------------------------------------------------------
   main
   
   This routine is the main application loop.
   -------------------------------------------------------------------- */
int main(void)
{
	// install the exit handler
	// when the program is stopped
	// or deleted Shutdown() will
	// be called
	TASKINFO taskStatus;
	getTaskInfo(0, &taskStatus);
	installExitHandler(taskStatus.taskID, (FUNCPTR)Shutdown);
	
#ifdef ENABLE_DEBUG
	ConfigureDebug();
#endif

	Initialize();

	// start a timed event
	startTimedEvent(MAIN_EVENT, MAIN_EVENT_INTERVAL);
	// main loop
	while (TRUE)
	{
		// add remainder of program here
		pNvMemory->totalCount++;
		pNvMemory->countSinceStart++;
		wait_event(MAIN_EVENT);
#ifdef ENABLE_DEBUG
		sprintf(dbgMsg, "Program downloaded at %02d-%02d-%d %d:%02d:%02d.%03d, total count %d, count since start %d\r\n",
			pNvMemory->downloadTime.month,
			pNvMemory->downloadTime.day,
			pNvMemory->downloadTime.year + 2000,
			pNvMemory->downloadTime.hour,
			pNvMemory->downloadTime.minute,
			pNvMemory->downloadTime.second,
			pNvMemory->downloadTime.hundredth * 10,
			pNvMemory->totalCount,
			pNvMemory->countSinceStart
			);
		SendMessage();
#endif 	
		release_processor();
	}
}


void Shutdown(void)
{
	// stop the main event
	endTimedEvent(MAIN_EVENT);

	// shut down debug
#ifdef ENABLE_DEBUG
	CloseDebug();
#endif
}

void Initialize(void)
{
	if (getProgramStatus((FUNCPTR)main) == NEW_PROGRAM)
	{
		// newly downloaded
		// store the download time and start time
		request_resource(IO_SYSTEM);
		getclock(&(pNvMemory->downloadTime));
		release_resource(IO_SYSTEM);
		pNvMemory->totalCount = 0;
		pNvMemory->countSinceStart = 0;
		// set the program status for next time
		setProgramStatus((FUNCPTR)main, PROGRAM_EXECUTED);
	}
	else if (getProgramStatus((FUNCPTR)main) == PROGRAM_EXECUTED)
	{
		// program was restarted
		pNvMemory->countSinceStart = 0;
	}
}
