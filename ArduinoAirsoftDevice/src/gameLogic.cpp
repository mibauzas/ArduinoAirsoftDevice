/*
 * gameLogic.cpp
 *
 *  Created on: 4 ene. 2020
 *      Author: Miquel
 */
#include "gameLogic.h"

void updateGamePhase (gameConfig_t *config, gameState_t *state, bool aBtnDown, bool bBtnDown){
	switch (state->phase){
	case SAFE:
		if (state->remainingTime == 0){
			switch (config->mode){
			case DEMOLITION:
				state->phase = DEFWINS;
				break;
			case KOTH:
				break;
			case SABOTAGE:
				break;
			}
		}else{
			if (aBtnDown){
				state->phase = ARMING;
				state->owner = ATEAM;
				state->defuseCountDown = config->defuseTime;
			}else if ((config->mode != DEMOLITION)&&(bBtnDown)){
				state->phase = ARMING;
				state->owner = BTEAM;
				state->defuseCountDown = config->defuseTime;
			}
		}
		break;
	}
}
