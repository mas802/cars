#include <avr/sleep.h>
// #include <avr/interrupt.h>

// https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/

#define CUSTOM_RESET   PCINT2
const int switchPin = 2;  // input switch 

const int leds[] = { 0, 1 };       // flasher group 1

const int t = 50;          // duration of flash
const int t2 = 19;         // duration of pause
const int rep = 10;        // repetion within flash
const int overall = 10;   // repetions until sleep

void setup() {

    pinMode(switchPin, INPUT);
    digitalWrite(switchPin, HIGH);

    for (int k = 0; k < 2; k++ ) {
        pinMode(leds[k], OUTPUT);
          flashLed(leds[k]);
//        // flashLedA(led2,led4);
//        digitalWrite(leds[0], HIGH);
////        digitalWrite(leds[1], HIGH);
////        digitalWrite(l2, HIGH);
//        delay(20);
//        digitalWrite(leds[0], LOW);
////        digitalWrite(leds[1], LOW);
//        delay(5);
    } // for
    
} 

void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(CUSTOM_RESET);                   // Use PB3 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(CUSTOM_RESET);                  // Turn off PB3 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
} 

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
}

void loop() {
    sleep();                            // go to sleep at start of loop (and wait for interrupt)
    for ( int j=0; j<overall; j++ ) {
        flashLed(leds[0]);
        flashLed(leds[1]);
    }
}

void flashLed(int l) {
    for ( int i=0; i<rep; i++ ) {
        digitalWrite(l, HIGH);
//        digitalWrite(l2, HIGH);
        delay(t);
        digitalWrite(l, LOW);
//        digitalWrite(l2, LOW);
        delay(t2);
    }
}

