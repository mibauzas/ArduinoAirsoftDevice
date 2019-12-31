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
	long seconds = timeToSeconds("00:00:00");
	assertEqual(seconds,0L);
}

void setup(){
	Serial.begin(9600);
}

void loop(){
	aunit::TestRunner::run();
}
