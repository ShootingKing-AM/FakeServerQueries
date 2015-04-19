
#include "amxxmodule.h"
#include "main.h"
#include "natives.h"

CHooker*	Hooker = new CHooker;
Func_Sendto SendtoOriginal = NULL;

CFunc* SendtoHook = NULL;

int iSendToFwd = -1;

void OnMetaAttach()
{
	bool result = hookSendto();
	
	printf( "\n %s v%s - by %s.\n -\n", MODULE_NAME, MODULE_VERSION, MODULE_AUTHOR );
	printf( " %s.\n\n", result ? "Loaded with success" : "Failed to create hook" );

	g_SendInfo = new CSendInfoBase;
}

void OnMetaDetach( void )
{
	delete g_SendInfo;
	SendtoHook->Restore();
}

void OnPluginsLoaded() 
{ 
    iSendToFwd = MF_RegisterForward("Server_SendTo", ET_STOP, FP_STRING, FP_STRING, FP_DONE);
}

#ifdef _TESTING
void dump_buffer(int len, const unsigned char* szBuffer)
{
	int i, j, iPrinted = 0;

	for( i = 0; i < len; i++ )
	{
        printf( "%02X ", szBuffer[i] );
        iPrinted++;

        if( iPrinted == 16 || i == len-1)
        {
            for( j = 0; j < ((16-iPrinted)+2)*3; j++ ) printf( " " );

            for( j = i-iPrinted+1; j <= i; j++ )
            {
                printf( "%c", (isprint(szBuffer[j])?szBuffer[j]:'.') );
            }
            printf( "\n" );
            iPrinted = 0;
        }
    }
    printf( "\n" );
}
#endif

size_t PASCAL OnNewSendto( int socket, const void* message, size_t length, int flags, const struct sockaddr* dest_addr, socklen_t dest_len )
{
	const unsigned char* origMessage = ( unsigned char* )message;

	size_t ret = 0;
	struct sockaddr_in *addr_in = (struct sockaddr_in *)dest_addr;

	if(MF_ExecuteForward( iSendToFwd, inet_ntoa( addr_in->sin_addr ), origMessage ))
		return 0;

	if( length > 5 && origMessage[0] == 0xff && origMessage[1] == 0xff && origMessage[2] == 0xff && origMessage[3] == 0xff)
	{
#ifdef _TESTING
		MF_Log( "Original and Sent Messages" );
		dump_buffer( (int)length, (unsigned char *)message );
#endif

		if( g_SendInfo->GetFlags() && origMessage[4] == 'I' )
		{
			CSocketBuffer Message( (char *)message, (int)length, g_SendInfo );

			Message.CopyDataTillPos(6); // Copy the all the header stuff

			if( g_SendInfo->GetFlags() & FL_SVNAME )
				 Message.SetCurrentString( g_SendInfo->GetInfoChar(FL_SVNAME) );
			else
				 Message.CopyCurrentString();

			if( g_SendInfo->GetFlags() & FL_MAPNAME )
				 Message.SetCurrentString( g_SendInfo->GetInfoChar(FL_MAPNAME) );
			else
				 Message.CopyCurrentString();

			Message.CopyCurrentString(); // Copy and Skip Folder

			if( g_SendInfo->GetFlags() & FL_GAMENAME )
				 Message.SetCurrentString( g_SendInfo->GetInfoChar(FL_GAMENAME) );
			else
				 Message.CopyCurrentString();

			Message.SkipShort(); // Copy and Skip AppID

			if( g_SendInfo->GetFlags() & FL_PLAYERS )
				 Message.SetCurrentByte( g_SendInfo->GetInfoInt(FL_PLAYERS) );
			else
				 Message.CopyCurrentByte();

			if( g_SendInfo->GetFlags() & FL_MAXPLAYERS )
				 Message.SetCurrentByte( g_SendInfo->GetInfoInt(FL_MAXPLAYERS) );
			else
				 Message.CopyCurrentByte();

			Message.CopyDataTillPos();  //Copy rest all data in OrigMessage as it is
			
#ifdef _TESTING
			dump_buffer( Message.GetLength(), (unsigned char *)Message.GetMessage() );
#endif
			if( SendtoHook->Restore() )
			{
				ret = SendtoOriginal( socket, Message.GetMessage(), Message.GetLength(), flags, dest_addr, dest_len );
				SendtoHook->Patch();
			}
			
			return ret;
		}
	}

	if( SendtoHook->Restore() )
	{
		ret = SendtoOriginal( socket, message, length, flags, dest_addr, dest_len );
		SendtoHook->Patch();
	}

	return ret;
}

bool hookSendto( void )
{
	#ifdef WIN32

		SendtoOriginal = ( Func_Sendto )GetProcAddress( GetModuleHandle( "wsock32.dll" ), "sendto" );
	
	#else

		// metamod-p parses elf structures, we find function easier & better way
		void * sym_ptr = ( void* )&sendto;
		
		while( *( unsigned short* )sym_ptr == 0x25ff ) 
		{
			sym_ptr = **( void*** )( ( char* )sym_ptr + 2 );
		}

		SendtoOriginal = ( Func_Sendto )sym_ptr;

	#endif

	SendtoHook = Hooker->CreateHook( SendtoOriginal, ( void* )OnNewSendto, TRUE );

	return SendtoHook ? true : false;
}
