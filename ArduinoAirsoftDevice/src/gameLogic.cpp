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
	case ARMING:
		if (state->owner == ATEAM){
			if (aBtnDown){
				if (state->defuseCountDown == 0){
					state->phase = ARMED;
					state->captureCountDown = config->captureTime;
					if (config->mode == KOTH){
						state->aTeamScore += config->capturePoints;
					}
				}
			}else{
				state->phase = SAFE;
				state->owner = NOTEAM;
				state->defuseCountDown = 0;
			}
		}else if (state->owner == BTEAM){
			if (bBtnDown){
				if (state->defuseCountDown == 0){
					state->phase = ARMED;
					state->captureCountDown = config->captureTime;
					if (config->mode == KOTH){
						state->bTeamScore += config->capturePoints;
					}
				}
			}else{
				state->phase = SAFE;
				state->owner = NOTEAM;
				state->defuseCountDown = 0;
			}
		}else{
			state->phase = SAFE;
			state->defuseCountDown = 0;
		}
		break;
	case ARMED:
		if((aBtnDown && state->owner == BTEAM)||(bBtnDown && state->owner == ATEAM)){
			state->phase = DEFUSING;
			state->defuseCountDown = config->defuseTime;
		}else if (state->captureCountDown > 0) {
			break; //no change
		}else{
			if (config->mode != KOTH){
				if (state->owner == ATEAM){
					state->phase = ATEAMWINS;
				}else{
					state->phase = BTEAMWINS;
				}
			} else {
				if (state->owner == ATEAM){
					state->aTeamScore += config->defendPoints;
				} else if (state->owner == BTEAM){
					state->bTeamScore += config->defendPoints;
				}
				if (state->remainingTime > 0){
					state->captureCountDown = config->captureTime;
				} else {
					if (state->aTeamScore == state->bTeamScore){
						state->phase = DRAW;
					}else if (state->aTeamScore > state->bTeamScore){
						state->phase = ATEAMWINS;
					}else if (state->aTeamScore < state->bTeamScore){
						state->phase = BTEAMWINS;
					}
				}
			}
		}
		break;
	case DEFUSING:
		if (state->owner == ATEAM){
			if (bBtnDown){
				if (state->defuseCountDown == 0){
					switch (config->mode){
					case DEMOLITION:
						state->phase = BTEAMWINS;
						break;
					case SABOTAGE:
						state->phase = SAFE;
						state->owner = NOTEAM;
						state->captureCountDown = 0;
						break;
					case KOTH:
						state->phase = ARMED;
						state->owner = BTEAM;
						state->captureCountDown = config->captureTime;
						state->bTeamScore += config->capturePoints;
						break;
					}
				}
			} else {
				state->phase = ARMED;
				state->defuseCountDown = 0;
			}
		} else if (state->owner == BTEAM){
			if (aBtnDown){
				if (state->defuseCountDown == 0){
					if (config->mode == SABOTAGE){
						state->phase = SAFE;
						state->owner = NOTEAM;
						state->captureCountDown = 0;
					} else if (config->mode == KOTH){
						state->phase = ARMED;
						state->owner = ATEAM;
						state->captureCountDown = config->captureTime;
						state->aTeamScore += config->capturePoints;
					}
				}
			} else {
				state->phase = ARMED;
				state->defuseCountDown = 0;
			}
		}
		if (config->mode == KOTH && state->remainingTime == 0){
			if (state->aTeamScore > state->bTeamScore){
				state->phase = ATEAMWINS;
			} else if (state->aTeamScore == state->bTeamScore){
				state->phase = DRAW;
			} else {
				state->phase = BTEAMWINS;
			}
		}
		break;
	}
}
