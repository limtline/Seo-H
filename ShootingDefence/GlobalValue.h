#pragma once

enum GameState
{
	GAME_READY = 0,
	GAME_START,
	NEXT_LEVEL,
	GAME_OVER,
};

extern GameState g_GameState;
extern int  g_TimeOut;
extern int  g_DiffLevel;    //≥≠¿Ãµµ

