#ifndef DEBUG_H
#define DEBUG_H

// the size of the debug message buffer
// care must be taken to make sure 
// all messages are less than this size
#define DBG_MSG_SIZE (1024)
// comment out ENABLE_DEBUG to disable debugging
#define ENABLE_DEBUG

#ifdef __cplusplus
extern "C" 
{
#endif
	void SendMessage();
	void ConfigureDebug(void);
	void CloseDebug(void);
#ifdef __cplusplus
}
#endif

#endif

