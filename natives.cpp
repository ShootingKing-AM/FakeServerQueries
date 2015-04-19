#include "amxxmodule.h"
#include "SendInfoBase.h"
#include "natives.h"

CSendInfoBase* g_SendInfo;

static cell AMX_NATIVE_CALL fq_set_hostname(AMX *amx, cell *params) /* 1 param */
{
	int iLen;
	char* szHostName = MF_GetAmxString(amx, params[1], 0, &iLen);

	if( (iLen < 1) && (g_SendInfo->GetFlags() & FL_SVNAME) )
	{
		g_SendInfo->unSetFlags(FL_SVNAME);
		return 0;
	}

	g_SendInfo->SetServerName( szHostName, iLen );
	return 0;
}

static cell AMX_NATIVE_CALL fq_set_mapname(AMX *amx, cell *params) /* 1 param */
{
	int iLen;
	char* szMapName = MF_GetAmxString(amx, params[1], 0, &iLen);

	if( (iLen < 1) && (g_SendInfo->GetFlags() & FL_MAPNAME) )
	{
		g_SendInfo->unSetFlags(FL_MAPNAME);
		return 0;
	}

	g_SendInfo->SetMapName( szMapName, iLen );
	return 0;
}

static cell AMX_NATIVE_CALL fq_set_gamename(AMX *amx, cell *params) /* 1 param */
{
	int iLen;
	char* szGameName = MF_GetAmxString(amx, params[1], 0, &iLen);

	if( (iLen < 1) && (g_SendInfo->GetFlags() & FL_GAMENAME) )
	{
		g_SendInfo->unSetFlags(FL_GAMENAME);
		return 0;
	}

	g_SendInfo->SetGameName( szGameName, iLen );
	return 0;
}

static cell AMX_NATIVE_CALL fq_set_players(AMX *amx, cell *params) /* 1 param */
{
	int players = params[1];

	if( (players < 0) && (g_SendInfo->GetFlags() & FL_PLAYERS) )
	{
		g_SendInfo->unSetFlags(FL_PLAYERS);
		return 0;
	}

	if( players > 127 )
		return -1;

	g_SendInfo->SetPlayers( players );
	return 0;
}

static cell AMX_NATIVE_CALL fq_set_maxplayers(AMX *amx, cell *params) /* 1 param */
{
	int maxplayers = params[1];

	if( (maxplayers < 0) && (g_SendInfo->GetFlags() & FL_MAXPLAYERS) )
	{
		g_SendInfo->unSetFlags(FL_MAXPLAYERS);
		return 0;
	}

	if( maxplayers > 127 )
		return -1;

	g_SendInfo->SetMaxPlayers( maxplayers );
	return 0;
}

static cell AMX_NATIVE_CALL fq_set_botsnum(AMX *amx, cell *params) /* 1 param */
{
	int bots = params[1];

	if( (bots < 0) && (g_SendInfo->GetFlags() & FL_BOTSNUM) )
	{
		g_SendInfo->unSetFlags(FL_BOTSNUM);
		return 0;
	}

	if( bots > 127 )
		return -1;

	g_SendInfo->SetBotsNum( bots );
	return 0;
}

static cell AMX_NATIVE_CALL fq_unset(AMX *amx, cell *params) /* 1 param */
{
	int iFlags = params[1];
	if( iFlags < FL_SVNAME && 
		iFlags > (FL_SVNAME|FL_MAPNAME|FL_GAMENAME|FL_PLAYERS|FL_MAXPLAYERS|FL_BOTSNUM) )
		return -1;

	g_SendInfo -> unSetFlags( iFlags );
	return 0;
}

AMX_NATIVE_INFO fq_Natives[] = {

	{ "fq_set_hostname",		fq_set_hostname},
	{ "fq_set_mapname",			fq_set_mapname},
	{ "fq_set_gamename",		fq_set_gamename},

	{ "fq_set_players",			fq_set_players },
	{ "fq_set_maxplayers",		fq_set_maxplayers },
	{ "fq_set_botsnum",			fq_set_botsnum }, 

	{ "fq_unset",				fq_unset },
	{ NULL, NULL }
};

void OnAmxxAttach()
{
	MF_AddNatives(fq_Natives);
}