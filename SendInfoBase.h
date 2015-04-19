#ifndef _SEND_INFO_BASE_H
#define _SEND_INFO_BASE_H

#define FL_SVNAME		(1 << 0)
#define FL_MAPNAME		(1 << 1)
#define FL_GAMENAME		(1 << 2)
#define FL_PLAYERS		(1 << 3)
#define FL_MAXPLAYERS	(1 << 4)
#define FL_BOTSNUM		(1 << 5)

class CSendInfoBase
{
	char* pszServerName;
	char* pszMap;
	char* pszGameName;

	int iPlayers;
	int iMaxPlayers;
	int iBots;

	int bitFlags;
public:

	CSendInfoBase();
	~CSendInfoBase();

	void SetServerName( char* buf, int len );
	void SetMapName( char* buf, int len );
	void SetGameName( char* buf, int len );

	void SetPlayers( int len );
	void SetMaxPlayers( int len );
	void SetBotsNum( int len );

	char* GetInfoChar( int flag );
	int GetInfoInt( int flag );

	inline int GetFlags() { return bitFlags; }
	inline void unSetFlags( int flag ) { bitFlags &= ~flag; }
};

class CSocketBuffer
{
	char* pszOrigMessage;
	char* pszMessage;
	int iOrigLen, iLength;
	int iCurrPos, diff;

	int GetCurrentStringLen( char* Buffer, int pos );
public:
	
	CSocketBuffer( char* OrigMessage, int length, CSendInfoBase* info );
	~CSocketBuffer();

	inline char* GetMessage() { return pszMessage; }
	inline int GetLength() { return iLength; }

	int GetCurrentStringLen( bool set = true );
	void SkipCurrentString();
	void CopyCurrentString();
	void SetCurrentString( char* szSource );
	void CopyDataTillPos( int pos = 0 );
	void SkipShort();
	void SetCurrentByte( int ibyte )
	{ 
		pszMessage[iCurrPos++] = ibyte;
	}
	void CopyCurrentByte() { pszMessage[iCurrPos++] = pszOrigMessage[iCurrPos+diff]; }
};

#endif // _SEND_INFO_BASE_H
