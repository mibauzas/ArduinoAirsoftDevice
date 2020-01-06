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
		.defuseCountDown = 0,
		.aTeamScore = 0U,
		.bTeamScore = 0U
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
	assertEqual(gameState.aTeamScore,0U);
	assertEqual(gameState.bTeamScore,0U);

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
	assertEqual(gameState.aTeamScore,0U);
	assertEqual(gameState.bTeamScore,0U);

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
	assertEqual(gameState.aTeamScore,0U);
	assertEqual(gameState.bTeamScore,0U);
}

test(safePhaseChange, gameTimeRunsOut){

	// In a Demolition game successfully defended the BTeam is the winner
	aPressed = false;
	bPressed = false;
	gameState.remainingTime = 0;
	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	gameState.captureCountDown = 0;
	gameState.aTeamScore = 0;
	gameState.bTeamScore = 0;

	gameConfig.mode = DEMOLITION;

	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,BTEAMWINS);
	assertEqual(gameState.owner,NOTEAM);
	assertEqual(gameState.remainingTime,0U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,0U);
	assertEqual(gameState.aTeamScore,0U);
	assertEqual(gameState.bTeamScore,0U);

	// In a Sabotage game, if there is no winner, it's a draw
	gameConfig.mode = SABOTAGE;

	gameState.remainingTime = 0;
	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	gameState.captureCountDown = 0;
	gameState.aTeamScore = 0;
	gameState.bTeamScore = 0;

	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,DRAW);
	assertEqual(gameState.owner,NOTEAM);
	assertEqual(gameState.remainingTime,0U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,0U);
	assertEqual(gameState.aTeamScore,0U);
	assertEqual(gameState.bTeamScore,0U);

	// KOTH game ending without a single capture ends with a draw
	gameConfig.mode = KOTH;

	gameState.remainingTime = 0;
	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	gameState.captureCountDown = 0;
	gameState.aTeamScore = 0;
	gameState.bTeamScore = 0;

	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,DRAW);
	assertEqual(gameState.owner,NOTEAM);
	assertEqual(gameState.remainingTime,0U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,0U);
	assertEqual(gameState.aTeamScore,0U);
	assertEqual(gameState.bTeamScore,0U);

	// KOTH game ending where ATeam has more points than BTeam
	gameConfig.mode = KOTH;

	gameState.remainingTime = 0;
	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	gameState.captureCountDown = 0;
	gameState.aTeamScore = 100U;
	gameState.bTeamScore = 50U;

	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,ATEAMWINS);
	assertEqual(gameState.owner,NOTEAM);
	assertEqual(gameState.remainingTime,0U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,0U);
	assertEqual(gameState.aTeamScore,100U);
	assertEqual(gameState.bTeamScore,50U);

	// KOTH game ending where BTeam has more points than ATeam
	gameConfig.mode = KOTH;

	gameState.remainingTime = 0;
	gameState.phase = SAFE;
	gameState.owner = NOTEAM;
	gameState.defuseCountDown = 0;
	gameState.captureCountDown = 0;
	gameState.aTeamScore = 50;
	gameState.bTeamScore = 100;

	updateGamePhase (&gameConfig, &gameState, aPressed, bPressed);
	assertEqual(gameState.phase,BTEAMWINS);
	assertEqual(gameState.owner,NOTEAM);
	assertEqual(gameState.remainingTime,0U);
	assertEqual(gameState.captureCountDown,0U);
	assertEqual(gameState.defuseCountDown,0U);
	assertEqual(gameState.aTeamScore,50U);
	assertEqual(gameState.bTeamScore,100U);
}

void setup(){
	Serial.begin(9600);
}

void loop(){
	aunit::TestRunner::run();
}
