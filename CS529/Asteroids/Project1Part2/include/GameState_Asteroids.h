// ---------------------------------------------------------------------------
// Project Name		:	Asteroid Game
// File Name		:	GameState_Play.h
// Author			:	Sun Tjen Fam
// Creation Date	:	2008/01/31
// Purpose			:	header file for the 'play' game state
// History			:
// - 2008/01/31		:	- initial implementation
// ---------------------------------------------------------------------------

#ifndef GAME_STATE_PLAY_H
#define GAME_STATE_PLAY_H

#include "Math2D.h"
#include "Matrix2D.h"
#include "Vector2D.h"
#include <cstdlib>
#include <ctime>


// ---------------------------------------------------------------------------

void GameStateAsteroidsLoad(void);
void GameStateAsteroidsInit(void);
void GameStateAsteroidsUpdate(void);
void GameStateAsteroidsDraw(void);
void GameStateAsteroidsFree(void);
void GameStateAsteroidsUnload(void);

// ---------------------------------------------------------------------------

#endif // GAME_STATE_PLAY_H


