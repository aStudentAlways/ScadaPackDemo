/* --------------------------------------------------------------------
   nvMemory.h
   Global definitions for user variables that need to be non-volatile.
   Copyright 2006, Control Microsystems Inc.
   -------------------------------------------------------------------- */

/* Prevent multiple inclusions */
#ifndef NVMEMORY_H
#define NVMEMORY_H

#ifdef __cplusplus
extern "C"
{
#endif

// --------------------------------------------------------
// Include-files
// --------------------------------------------------------
#include "ctools.h"

/* --------------------------------------------------------------------
   Variables located in Static Non-Volatile Memory
   -------------------------------------------------------------------- */
// Add fields to this global structure for variables used in your 
// application file(s) that need to be non-volatile. Include nvMemory.h 
// in all files that use the variable pNvMemory to access NV memory.
typedef struct s_nvMemory
{
	TIME downloadTime;
	int totalCount; 
	int countSinceStart;

	// Add fields here for variables used in your application file(s) 
	// that need to be non-volatile.

}NV_MEMORY;

// Pointer to static non-volatile data
extern NV_MEMORY * pNvMemory;

#ifdef __cplusplus
}
#endif

#endif // NVMEMORY_H


