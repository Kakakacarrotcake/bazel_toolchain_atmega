#include "hdr/math.h"
#include <assert.h>
#include "Unity-2.6.1/src/unity.h"
#include <avr/io.h>
#include <sleep.h>
#define F_CPU 20000000UL

#include <util/delay.h>

void setUp(void) {
    DDRB |= (1 << PB5); // Set PB5 (Arduino pin 13) as output
    DDRB |= (1 << PB4); // Set PB5 (Arduino pin 12) as output
}

void tearDown(void) {
    // clean stuff up here
}

void positive_test(void) {
    if (add(5, 3) == (5 + 3))
    {
        // if the assert is true the green led diode shall be lit for 10 seconds
        PORTB |= (1 << PB5); // Set PB5 HIGH
        _delay_ms(10000); // This should wait for 10 second. 
        PORTB &= ~(1 << PB5); // Set PB5 LOW
        _delay_ms(1000); // This should wait for 1 second
    }
    else {
        // if the assert is true the red led diode shall be lit for 10 seconds
        PORTB |= (1 << PB4); // Set PB4 HIGH
        _delay_ms(10000); // This should wait for 10 second. 
        PORTB &= ~(1 << PB4); // Set PB4 LOW
        _delay_ms(1000); // This should wait for 1 second
    }
    
}

void negative_test(void) {
    if (add(5, 3) == (5 + 3 + 2))
    {
        // if the assert is true the green led diode shall be lit for 10 seconds
        PORTB |= (1 << PB5); // Set PB5 HIGH
        _delay_ms(10000); // This should wait for 10 second. 
        PORTB &= ~(1 << PB5); // Set PB5 LOW
        _delay_ms(1000); // This should wait for 1 second
    }
    else {
        // if the assert is true the red led diode shall be lit for 10 seconds
        PORTB |= (1 << PB4); // Set PB4 HIGH
        _delay_ms(10000); // This should wait for 10 second. 
        PORTB &= ~(1 << PB4); // Set PB4 LOW
        _delay_ms(1000); // This should wait for 1 second
    }
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(positive_test);
    RUN_TEST(negative_test);
    return UNITY_END();
}