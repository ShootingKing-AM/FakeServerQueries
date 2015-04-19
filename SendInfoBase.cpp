
#include "amxxmodule.h"
#include "SendInfoBase.h"
#include <stdio.h>
#include <string.h>

CSendInfoBase::CSendInfoBase()
{
	pszServerName = NULL;
	pszMap = NULL;
	pszGameName = NULL;
	bitFlags = 0;
}

CSendInfoBase::~CSendInfoBase()
{
	delete [] pszServerName;
	delete [] pszMap;
	delete [] pszGameName;
}

void CSendInfoBase::SetServerName( char* buf, int len )
{
	if( pszServerName!=NULL ) delete [] pszServerName;
	pszServerName = new char [len+1];

	memcpy( pszServerName, buf, len+1 );
	bitFlags |= FL_SVNAME;
}

void CSendInfoBase::SetMapName( char* buf, int len )
{
	if( pszMap!=NULL ) delete [] pszMap;
	pszMap = new char [len+1];

	memcpy( pszMap, buf, len+1 );
	bitFlags |= FL_MAPNAME;
}

void CSendInfoBase::SetGameName( char* buf, int len )
{
	if( pszGameName!=NULL ) delete [] pszGameName;
	pszGameName = new char [len+1];

	memcpy( pszGameName, buf, len+1 );
	bitFlags |= FL_GAMENAME;
}

void CSendInfoBase::SetPlayers( int len )
{
	iPlayers = len;
	bitFlags |= FL_PLAYERS;
}

void CSendInfoBase::SetMaxPlayers( int len )
{
	iMaxPlayers = len;
	bitFlags |= FL_MAXPLAYERS;
}

void CSendInfoBase::SetBotsNum( int len )
{
	iBots = len;
	bitFlags |= FL_BOTSNUM;
}

char* CSendInfoBase::GetInfoChar( int flag )
{
	switch (flag)
	{
		case FL_SVNAME:		return pszServerName;
		case FL_MAPNAME:	return pszMap;
		default:			return pszGameName;
	}
}

int CSendInfoBase::GetInfoInt( int flag )
{
	switch (flag)
	{
		case FL_PLAYERS:	return iPlayers;
		case FL_MAXPLAYERS: return iMaxPlayers;
		default:			return iBots;
	}
}

CSocketBuffer::CSocketBuffer( char* OrigMessage, int length, CSendInfoBase* info )
{
	pszOrigMessage = new char [length];
	memcpy( pszOrigMessage, OrigMessage, length );

	iOrigLen = length;
	diff = 0;
	iCurrPos = 6;

    int tdiff = 0;	

	if( info->GetFlags() & FL_SVNAME )
    	tdiff += GetCurrentStringLen()-strlen( info->GetInfoChar(FL_SVNAME) );
	else
		SkipCurrentString();

	if( info->GetFlags() & FL_MAPNAME )
		tdiff += GetCurrentStringLen()-strlen( info->GetInfoChar(FL_MAPNAME) );
	else
		SkipCurrentString();

	SkipCurrentString(); // Skip Folder

	if( info->GetFlags() & FL_GAMENAME )
		tdiff += GetCurrentStringLen()-strlen( info->GetInfoChar(FL_GAMENAME) );
	else
		SkipCurrentString();
	
	iLength = length-tdiff;
	pszMessage = new char [iLength];
	memset( pszMessage, 0, iLength );
	
	iCurrPos = 0;
}

CSocketBuffer::~CSocketBuffer()
{
	delete [] pszOrigMessage;
	delete [] pszMessage;
}

int CSocketBuffer::GetCurrentStringLen( bool set )
{
	int len = 0, temp = iCurrPos;
	while( pszOrigMessage[temp+diff] != '\x00' )
    {
        temp++; len++;
    }

	if( set ) iCurrPos = temp+1;
	return len;
}

void CSocketBuffer::SkipCurrentString()
{
	while( pszOrigMessage[iCurrPos++] != '\x00' ); //iCurrPos++;
}

void CSocketBuffer::CopyCurrentString()
{
    while( pszOrigMessage[iCurrPos+diff] != '\x00' )
	{
		pszMessage[iCurrPos] = pszOrigMessage[iCurrPos+diff];
        iCurrPos++;
	}
    iCurrPos += 1;
}

void CSocketBuffer::CopyDataTillPos( int pos )
{
	pos = (pos)?pos:iLength;
	
	for( iCurrPos; iCurrPos < pos; iCurrPos++ )
	{
		pszMessage[iCurrPos] = pszOrigMessage[iCurrPos+diff];
	}
}

void CSocketBuffer::SetCurrentString( char* szSource )
{
	int i = 0, temp;
	while( szSource[i] != '\x00' )
	{
		pszMessage[i+iCurrPos] = szSource[i];
		i++;
	}
    temp = GetCurrentStringLen(false);
    
	diff += temp-i;
	iCurrPos += i+1;
}

void CSocketBuffer::SkipShort()
{
	pszMessage[iCurrPos++] = pszOrigMessage[iCurrPos+diff];
	pszMessage[iCurrPos++] = pszOrigMessage[iCurrPos+diff];
}