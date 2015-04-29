/*

	Fake Queries - v1.0
		By Shooting King

*/

#if defined _sbutil_included
  #endinput
#endif
#define _sbutil_included

#if AMXX_VERSION_NUM >= 175
 #pragma reqlib fakequeries
 #if !defined AMXMODX_NOAUTOLOAD
  #pragma loadlib fakequeries
 #endif
#else
 #pragma library fakequeries
#endif

/* 
   Set HostName on Sendto Quries
   MaxLen In SvBrowser - 63 + 1 (null)

   if an empty string ("") is passed, FL_SVNAME flag will be unset.  
*/
native fq_set_hostname( const szHostName[] );

/* 
   Set MapName on Sendto Quries
   MaxLen In SvBrowser - 31 + 1 (null)

   if an empty string ("") is passed, FL_MAPNAME flag will be unset.
*/
native fq_set_mapname( const szMapName[] );

/* 
   Set GameName on Sendto Quries
   MaxLen In SvBrowser - 63 + 1 (null)

   if an empty string ("") is passed, FL_GAMENAME flag will be unset.
*/
native fq_set_gamename( const szGameName[] );

/* 
   Set PlayersNum on Sendto Quries
   If iPlayers is less than 0 i.e a negative number,
   original playersnum will be passed in queries and FL_PLAYERS flag will be unset.

   returns 0 On Success and -1 on Failure ( i.e when iPlayers is greater than 127 )
*/
native fq_set_players( const iPlayers );

/* 
   Set MaxPlayers on Sendto Quries
   If iMaxPlayers is less than 0 i.e a negative number
   original maxplayers will be set and FL_MAXPLAYERS flag will be unset.

   returns 0 On Success and -1 on Failure ( i.e when iMaxPlayers is greater than 127 )
*/
native fq_set_maxplayers( const iMaxPlayers );

/* 
   Set BotsNumber on Sendto Quries
   If iBotsNum is less than 0 i.e a negative number
   original botsnum will be set and FL_BOTSNUM flag will be unset.

   returns 0 On Success and -1 on Failure ( i.e when iBotsNum is greater than 127 )
*/
native fq_set_botsnum( const iBotsNum );

enum FFlags(<<=1)
{
	FL_SVNAME = ( 1<<0 ),
	FL_MAPNAME,
	FL_GAMENAME,
	FL_PLAYERS,
	FL_MAXPLAYERS,
	FL_BOTSNUM
}

/* 
   UnSet respective variable flags. You can even pass the bitsum of the above flags
   eg. fq_unset( FL_SVNAME | FL_BOTSNUM );

   Before modifing the information in query responses, Module will be checking if the
   variable flags are set and if yes, proceeds to substitute the respective variables into the response.
   
   returns 0 on Success and -1 on Failure
*/
native fq_unset( const FFlags:iFlags );

/*
   Called Before replying to a request.
   To block sending Response to an IP, return PLUGIN_HANDLED. 

   Header is a part of actuall response (till first \x00 encounter) which would be sent by the server.
*/
forward Server_SendTo( const szIP[], const szHeader[] );