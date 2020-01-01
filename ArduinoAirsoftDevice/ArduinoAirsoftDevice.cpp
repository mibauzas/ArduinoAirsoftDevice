// Do not remove the include below
#include "ArduinoAirsoftDevice.h"

/*
 * LCD - The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * LCD A (Backlight) pin to 3.3V
 * 10K resistor
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 *
 * define the constants based on the circuit
 */
const uint8_t LCD_RS_PIN = 12,
              LCD_EN_PIN = 11,
              LCD_D4_PIN = 4,
              LCD_D5_PIN = 5,
              LCD_D6_PIN = 6,
              LCD_D7_PIN = 7;
LiquidCrystal lcd(LCD_RS_PIN, LCD_EN_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

// Display variables
bool update_display = true;
volatile long interrupt_count = 0;
char time_output[9];

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

	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	lcd.clear();
	lcd.print("Time:");

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
		update_display = true;
		interrupt_count++;
		t1_semaphore = false;
	}
	interrupts();

	//Toggle led1 if necessary
	if (update_led1){
		digitalWrite(LED1_PIN, led1State);
		update_led1 = false;
	}

	// Update the display with the interrupt count
	if(update_display){
		lcd.setCursor(8, 0);
		lcd.print(interrupt_count);
		update_display = false;
	}
}

// ISR handling 1 second interrupts
ISR(TIMER1_COMPA_vect){
   t1_semaphore = true;
}
