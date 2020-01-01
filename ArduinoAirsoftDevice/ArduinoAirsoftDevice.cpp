// Do not remove the include below
#include "ArduinoAirsoftDevice.h"

/*
 * LED - The circuit:
 * Onboard led pin 13
 */
const uint8_t LED1_PIN = 13;
int led1State = LOW;
bool update_led1 = false;

//Timer1 semaphore
volatile bool t1_semaphore = true;


//The setup function is called once at startup of the sketch
void setup()
{
	//set timer1 interrupt at 1Hz
	noInterrupts();
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1  = 0;//initialize counter value to 0
	// set compare match register for 1hz increments
	OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	interrupts();

	// Initialize digital pin LED1_PIN as an output
	pinMode(LED1_PIN, OUTPUT);
}

// The loop function is called in an endless loop
void loop()
{
	// Start of critical section
	noInterrupts();
	// Check if 1 second has passed.
	if (t1_semaphore){
		led1State = !led1State;
		update_led1 = true;
		t1_semaphore = false;
	}
	interrupts();

	//Toggle led1 if necessary
	if (update_led1){
		digitalWrite(LED1_PIN, led1State);
		update_led1 = false;
	}
}

// ISR handling 1 second interrupts
ISR(TIMER1_COMPA_vect){
   t1_semaphore = true;
}
