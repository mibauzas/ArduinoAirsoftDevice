/*
 * gameLogic.h
 *
 *  Created on: 4 ene. 2020
 *      Author: Miquel
 */

#ifndef SRC_GAMELOGIC_H_
#define SRC_GAMELOGIC_H_

enum DEVICE_OWNER {NOTEAM, ATTACK, DEFENSE};
enum GAME_PHASE {SAFE, ARMING, ARMED, DEFUSING, ATTWINS, DEFWINS, DRAW};

struct gameState {
	GAME_PHASE phase, previousPhase;
	DEVICE_OWNER owner;
	unsigned int remainingTime;
	unsigned int captureCountDown;
	unsigned int defuseCountDown;
};

void updateGamePhase (gameState state);


#endif /* SRC_GAMELOGIC_H_ */
