#include <CTOOLS.H>
#include "debug.h"


#ifdef ENABLE_DEBUG

#define DEBUG_PORT com3

#define DEBUG_NONE			(0) // disaables debug
#define DEBUG_SERIAL		(1) // use DEBUG_PORT
#define DEBUG_TCP			(2) // broadcast UDP packets to REMOTE_PORT

///change LOCAL_PORT, REMOTE_PORT and REMOTE_ADDRESS as required
#define LOCAL_PORT			(65504)
#define REMOTE_PORT			(65505)
#define REMOTE_ADDRESS		("192.168.0.133") // only required to send to a specific address
#define REMOTE_ADDRESS_BROADCAST		("255.255.255.255") // used for broadcast

#define VERSION_MAJOR (0)
#define VERSION_MINOR (1)
#define	VERSION_BUILD (0)

// socketOpen tracks the state of the debug socket
BOOLEAN socketOpen = FALSE;
//sock is the socket handle
int sock;
//remoteSa holds the remote address, port number, etc
sockaddr_in remoteSa;
//localSa holds the local socket information (port number)
sockaddr_in localSa;

// a buffer to hold debug messages. Copy to this buffer
// and then call SendMessage()
char dbgMsg[DBG_MSG_SIZE];

void SendInfo(void);

// set the debug type when not using a 4203
#ifndef SCADAPack4203
UINT16 enableDebug = DEBUG_TCP;
#else
// never enable debug on 4203
UINT16 enableDebug = DEBUG_NONE;
#endif

void SendMessage()
{
	if (enableDebug == DEBUG_NONE)
	{
		return;
	}
	else if (enableDebug == DEBUG_SERIAL)
	{
		fprintf(DEBUG_PORT, "%s", dbgMsg);
	}
#ifndef SP4203
	else if (enableDebug == DEBUG_TCP)
	{
		if (socketOpen == TRUE)
		{
			// send UDP packet with message. To receive these packets, use 
			// ncat or similar. Listen for UDP packets on REMOTE_PORT. 
			// Broadcast packets will usually not pass through a VPN or VLAN, but
			// it depends on the configuration of switches
			sendto(sock, dbgMsg, strlen(dbgMsg), 0, (struct sockaddr*)&remoteSa, sizeof(remoteSa));
		}
	}
#endif
}

void ConfigureDebug(void)
{

	if (enableDebug == DEBUG_NONE)
	{
		CloseDebug();
	}
	else if (enableDebug == DEBUG_SERIAL)
	{
		//set up the serial port
		struct pconfig portSettings;
		portSettings.baud = BAUD115200;
		portSettings.duplex = HALF;
		portSettings.parity = PARITY_NONE;
		portSettings.data_bits = DATA8;
		portSettings.stop_bits = STOP1;
		portSettings.flow_rx = RFC_NONE;
		portSettings.flow_tx = TFC_NONE;
		portSettings.type = RS232;
		portSettings.timeout = 600;

		PROTOCOL_SETTINGS settings;
		settings.type = NO_PROTOCOL;
		settings.station = 0;
		settings.priority = 100;
		settings.SFMessaging = FALSE;
		settings.mode = AM_standard;

		request_resource(IO_SYSTEM);
		set_port(DEBUG_PORT, &portSettings);
		setProtocolSettings(DEBUG_PORT, &settings);
		release_resource(IO_SYSTEM);
		SendInfo();
	}
#ifndef SCADAPack4203
	else if (enableDebug == DEBUG_TCP)
	{
		// create a socket and initalize it. Since we're sending 
		// datagrams, we don't need to create a connection
		sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (sock != -1)
		{
			remoteSa.sin_family = AF_INET;
			remoteSa.sin_addr.s_addr = inet_addr(REMOTE_ADDRESS_BROADCAST);
			remoteSa.sin_port = htons(REMOTE_PORT);

			localSa.sin_family = AF_INET;
			localSa.sin_addr.s_addr = htonl(INADDR_ANY);
			localSa.sin_port = htons(LOCAL_PORT);

			// set up broadcast 
			//http://www.csee.usf.edu/~kchriste/tools/udpClientBroadcast.c
			int iOptVal = 1;
			int iOptLen = sizeof(int);
			setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&iOptVal, iOptLen);
			// bind to the socket to set our port number. This step can be skipped 
			// if you don't care about the source port number. If the source port
			// number is changing ncat will miss new messages when the program is 
			// restarted, so I prefer to use a fixed port number
			int status = bind(sock, (struct sockaddr*)&localSa, sizeof(localSa));
			if (status != -1)
			{
				socketOpen = TRUE;
				SendInfo();
			}
		}
	}
#endif
}

void CloseDebug(void)
{
#ifndef SP4203
	// call this before the program is stopped to
	// cleanly close the socket
	if (socketOpen == TRUE)
	{
		sprintf(dbgMsg, "\r\nClosing debug socket %u\r\n", sock);
		SendMessage();
		close(sock);
		socketOpen = FALSE;
	}
#endif
}

void SendInfo(void)
{
	// this function sends some information about the program when debug is enabled.
	if (enableDebug == DEBUG_TCP)
	{
		sprintf(dbgMsg, "\r\n\r\nDebug opened on socket %u\r\n", sock);
		SendMessage();
	}

	sprintf(dbgMsg, "Application %d.%d.%d starting. Compiled on   ", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	SendMessage();

	sprintf(dbgMsg, __DATE__);
	SendMessage();

	sprintf(dbgMsg, "   ");
	SendMessage();

	sprintf(dbgMsg, __TIME__);
	SendMessage();

	sprintf(dbgMsg, "\r\n");
	SendMessage();
}

#else
void SendMessage()
{
	return;
}
void ConfigureDebug(void)
{
	return;
}
void CloseDebug(void)
{
	return;
}

#endif

