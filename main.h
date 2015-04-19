
#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>
	#include <sys/types.h>
	#include <string.h>
	#include <WinSock.h>
	#pragma comment(lib, "Ws2_32.lib" )

	typedef int socklen_t;
#else
	#include <sys/types.h>
	#include <sys/time.h> 
	#include <sys/socket.h>
	#include <unistd.h>
	#include <arpa/inet.h>

	#define PASCAL
#endif

#include <malloc.h>
#include "chooker.h"
#include "SendInfoBase.h"

// #define _TESTING

typedef size_t ( PASCAL *Func_Sendto )( int, const void*, size_t, int, const struct sockaddr*, socklen_t );
size_t PASCAL OnNewSendto( int socket, const void* message, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len );
bool hookSendto( void );

#endif // MAIN_H
