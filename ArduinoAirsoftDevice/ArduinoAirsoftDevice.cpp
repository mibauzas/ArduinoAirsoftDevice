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
 * 4x4 Keypad - The circuit:
 * ROW0 to digital pin 19
 * ROW1 to digital pin 18
 * ROW2 to digital pin 17
 * ROW3 to digital pin 16
 * COL0 to digital pin 15
 * COL1 to digital pin 14
 * COL2 to digital pin 10
 * COL3 to digital pin 3
 */
const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;

const uint8_t KEYPAD_ROW0 = 19,
			  KEYPAD_ROW1 = 18,
			  KEYPAD_ROW2 = 17,
			  KEYPAD_ROW3 = 16,
			  KEYPAD_COL0 = 15,
			  KEYPAD_COL1 = 14,
			  KEYPAD_COL2 = 10,
			  KEYPAD_COL3 = 3;

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'#','0','*','D'}
};

byte rowPins[KEYPAD_ROWS] = {KEYPAD_ROW0,KEYPAD_ROW1,KEYPAD_ROW2,KEYPAD_ROW3};
byte colPins[KEYPAD_COLS] = {KEYPAD_COL0,KEYPAD_COL1,KEYPAD_COL2,KEYPAD_COL3};

Keypad customKeypad = Keypad( makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);

char customKey;

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
	lcd.setCursor(0, 1);
	lcd.print("Key:");

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

	customKey = customKeypad.getKey();

	if (customKey != NO_KEY){
		lcd.setCursor(8,1);
		lcd.print(customKey);
	}

	//Toggle led1 if necessary
	if (update_led1){
		digitalWrite(LED1_PIN, led1State);
		update_led1 = false;
	}

	// Update the display with the interrupt count
	if(update_display){
		secondsToTime(interrupt_count, time_output);
		lcd.setCursor(8, 0);
		lcd.print(time_output);
		update_display = false;
	}
}

// ISR handling 1 second interrupts
ISR(TIMER1_COMPA_vect){
   t1_semaphore = true;
}
