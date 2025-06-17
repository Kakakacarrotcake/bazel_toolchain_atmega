#include "hdr/math.h"
#include "stdio.h"
#include <avr/io.h>
#include <sleep.h>
#include <util/delay.h>


int main(){
    int x = 5;
    int y = 4;

    printf("%d", add(x, y));

    // This code will blink an LED connected to PB5 (Arduino pin 13) every 5 seconds
    DDRB |= (1 << PB5); // Set PB5 (Arduino pin 13) as output
    while(1)
    {
        PORTB |= (1 << PB5); // Set PB5 HIGH
        _delay_ms(5000); // Wait for 5 second
        PORTB &= ~(1 << PB5); // Set PB5 LOW
        _delay_ms(5000); // Wait for 5 second
    }
    return 0;
}
