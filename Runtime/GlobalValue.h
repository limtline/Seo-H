#pragma once

enum GameState
{
	GAME_READY = 0,
	RACE_READY,
	RACE_ING,
	RACE_END,
	GStateCount,
};

extern GameState g_GameState;	//게임의 상태
