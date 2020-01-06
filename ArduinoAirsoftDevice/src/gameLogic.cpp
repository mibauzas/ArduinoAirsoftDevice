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
				state->phase = BTEAMWINS;
				break;
			case KOTH:
				if (state->aTeamScore == state->bTeamScore){
					state->phase = DRAW;
				}else if (state->aTeamScore > state->bTeamScore){
					state->phase = ATEAMWINS;
				}else if (state->aTeamScore < state->bTeamScore){
					state->phase = BTEAMWINS;
				}
				break;
			case SABOTAGE:
				state->phase = DRAW;
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
