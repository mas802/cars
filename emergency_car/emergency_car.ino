#include <avr/sleep.h>

#define CUSTOM_RESET PCINT3
const int switchPin = 3;  // input switch 

const int leds[] = { 0, 1, 2, 4};  // flasher group 1

const int durationOn  = 100;   // duration of flash
const int durationOff = 20;    // duration of pause
const int noFlashes   = 1;     // repetion within flash
const int repetitions = 25;   // repetions until sleep

void setup() {
    pinMode(switchPin, INPUT);
    digitalWrite(switchPin, HIGH);

    for (int k = 0; k < 4; k++ ) {
        pinMode(leds[k], OUTPUT);
    }
}

// https://bigdanzblog.wordpress.com/2014/08/10/attiny85-wake-from-sleep-on-pin-state-change-code-example/
void sleep() {

    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(CUSTOM_RESET);             // Use PBx as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(CUSTOM_RESET);            // Turn off PBx as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
}

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
}

void loop() {
    for (int j=0; j<repetitions; j++) {
        flashLed(leds[0]);
        flashLed(leds[0]);
        flashLed(leds[1]);
        flashLed(leds[2]);
        flashLed(leds[3]);
        flashLed(leds[3]);
        flashLed(leds[2]);
        flashLed(leds[1]);
    }
    sleep();
}

void flashLed(int led) {
    for ( int i=0; i<noFlashes; i++ ) {
        digitalWrite(led, HIGH);
        delay(durationOn);
        digitalWrite(led, LOW);
        delay(durationOff);
    }
}
