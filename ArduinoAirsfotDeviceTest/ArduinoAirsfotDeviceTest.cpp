// Do not remove the include below
#include "ArduinoAirsfotDeviceTest.h"

char time[9];

long value = 0;

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

void setup(){
	Serial.begin(9600);
}

void loop(){
	aunit::TestRunner::run();
}
