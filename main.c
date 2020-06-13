/*
 * Tarea_ADC.c
 *
 * Created: 06/12/2020 11:35:31 p. m.
 * Author : cesh_
 */ 

#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned int analogResult = 0;
unsigned int binary_weighted_voltage_low;
unsigned int binary_weighted_voltage_high;


/*
ADC Pre-scaler needs to be set so that the ADC input frequency is between 50 - 200kHz.
Clock   Available pre-scaler values
1 MHz   8 (125kHz), 16 (62.5kHz)
20 mhZ 128 (156.25 kHz)
*/


int main(void)
{
    DDRB |= (1<<PORTB6)|(1<<PORTB3)|(1<<PORTB4);   //Set the Data Direction Register to output (PB3 = yellow led , PB6 = green led and PB4= red led)
    DDRA &= ~(1<<PORTA4);	//Set the Data Direction Register for the POT to input
    
    ADMUX =
    (0 << REFS1) |  (0 << REFS0) |   // Sets ref. voltage to VCC
    (0 << ADLAR) |				   // 0: right adjust, 1: left adjust
    (0 << MUX4) | (0 << MUX3) | (0 << MUX2)  |	(1 << MUX1)  |	(1 << MUX0);   //  MUX bits ADC3; 00011, PA4
    
    
    ADCSRA =
    (1 << ADEN)  |     // Enable ADC
    (0 << ADSC)  |     // DISABLED Start Conversion (at setup)
    (0 << ADATE)  |     // DISABLED Auto trigger
    (0 << ADIF)  |     //
    (0 << ADIE)  |     // DISABLED ADC interrupt flag
    (1 << ADPS2) | (1 << ADPS1) |  (1 << ADPS0);  // set prescaler to 128
    
    ADCSRB =
    (0<<ADTS2)|		//Free running mode bit 2
    (0<<ADTS1)|		//Free running mode bit 1
    (0<<ADTS0);		//Free running mode bit 0
    
    while (1)
    {
	    ADCSRA |= (1 << ADSC);         // start ADC measurement
	    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
	    //result = (ADCH<<8)|ADCL; 8 bits from ADCL + 2 bits from ADCH
	    binary_weighted_voltage_low = ADCL;
	    binary_weighted_voltage_high = ((unsigned int)(ADCH << 8));
	    analogResult = binary_weighted_voltage_low | binary_weighted_voltage_high;
	    //VCC = 5.0V - 1 unit =  mV
	    if(analogResult<=410) //value 0-410 (0-2.0V @vref=5.0 V)
	    {
		    PORTB|=(1<<PORTB3); // turn yellow led on
		    PORTB&=~((1<<PORTB6)|(1<<PORTB4)); // turn green led off and  turn red led off
		    _delay_ms(100);
	    }
	    else if(analogResult>=839) //value 839-1023 (4.1 - 5.0 V @vref = 5.0 V)
	    {
		    PORTB&=~((1<<PORTB3)|(1<<PORTB4)); // turn yellow led off and turn red led off
		    PORTB|=(1<<PORTB6); // turn green led on
			
		    _delay_ms(100);
	    }
	    else
	    {
		    PORTB &=~((1<<PORTB6)|(1<<PORTB3));// turn green led off and turn yellow led off
			PORTB|=(1<<PORTB4); // turn red led on
	    }
    }
}

