// Do not remove the include below
#include "ArduinoAirsfotDeviceTest.h"

char time[9];

test(pass){
	assertEqual(1,1);
}

test(fail){
	assertNotEqual(1,1);
}

test(secondsConverter){
	secondsToTime(0L,time);
	assertStringCaseEqual(time,"00:00:00");
	secondsToTime(60L,time);
	assertStringCaseEqual(time,"00:01:00");
	secondsToTime(3600L,time);
	assertStringCaseEqual(time,"01:00:00");
}

test(timeConverter){
	strlcpy(time,"00:00:00",9);
	assertEqual(0UL,timeToSeconds(time));
	strlcpy(time,"00:00:01",9);
	assertEqual(1UL,timeToSeconds(time));
	strlcpy(time,"00:00:59",9);
	assertEqual(59UL,timeToSeconds(time));
	strlcpy(time,"00:01:00",9);
	assertEqual(60UL,timeToSeconds(time));
	strlcpy(time,"00:59:59",9);
	assertEqual(3599UL,timeToSeconds(time));
	strlcpy(time,"01:00:00",9);
	assertEqual(3600UL,timeToSeconds(time));
}

/*
 * Initial state of a game
 * 3600s remaining time, without owner capture counters set to 0s
 */
gameState_t gameState = {
		.phase = SAFE,
		.owner = NOTEAM,
		.remainingTime = 3600,
		.captureCountDown = 0,
		.defuseCountDown = 0
};

/*
 * Set game mode to SABOTAGE
 */
const unsigned int defuseTimer = 5U;

struct gameConfig_t gameConfig = {
		.mode = SABOTAGE,
		.defuseTime = defuseTimer
};

bool aPressed = true;
bool bPressed = false;

test(safePhaseChange, buttonActivation){
	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,ARMING);
	assertEqual(gameState.owner,ATEAM);
	assertEqual(gameState.remainingTime,3600U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,defuseTimer);

	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	aPressed = false;
	bPressed = true;
	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,ARMING);
	assertEqual(gameState.owner,BTEAM);
	assertEqual(gameState.remainingTime,3600U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,defuseTimer);

	//bTeam button must be ignored in DEMOLITION mode
	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	gameConfig.mode = DEMOLITION;
	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,SAFE);
	assertEqual(gameState.owner,NOTEAM);
	assertEqual(gameState.remainingTime,3600U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,0U);
}

test(safePhaseChange, gameTimeRunsOut){
	aPressed = false;
	bPressed = false;
	gameState.remainingTime = 0;
	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	gameState.captureCountDown = 0;

	gameConfig.mode = DEMOLITION;

	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,DEFWINS);
	assertEqual(gameState.owner,NOTEAM);
	assertEqual(gameState.remainingTime,0U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,0U);

}

void setup(){
	Serial.begin(9600);
}

void loop(){
	aunit::TestRunner::run();
}
