/*
 * gameLogic.h
 *
 *  Created on: 4 ene. 2020
 *      Author: Miquel
 */

#ifndef SRC_GAMELOGIC_H_
#define SRC_GAMELOGIC_H_

enum DEVICE_OWNER {NOTEAM, ATEAM, BTEAM};
enum GAME_PHASE {SAFE, ARMING, ARMED, DEFUSING, ATEAMWINS, BTEAMWINS, DRAW};
enum GAME_MODE {SABOTAGE, DEMOLITION, KOTH};

struct gameConfig_t{
	GAME_MODE mode;
	unsigned int defuseTime;
};

struct gameState_t {
	GAME_PHASE phase;
	DEVICE_OWNER owner;
	unsigned int remainingTime;
	unsigned int captureCountDown;
	unsigned int defuseCountDown;
	unsigned int aTeamScore;
	unsigned int bTeamScore;
};

void updateGamePhase (gameConfig_t *config, gameState_t *state, bool aBtnDown, bool bBtnDown);


#endif /* SRC_GAMELOGIC_H_ */
