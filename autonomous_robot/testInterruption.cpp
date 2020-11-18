#define F_CPU 8000000
#include <avr/io.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>
#include "Librairie/uart.h"
#include "Librairie/Roue.h"
#include "Librairie/Robot.h"
#include "Librairie/Bouton.h"
volatile uint8_t boutonPoussoir = 0;
void initialisation ( void ) {

cli (); 

DDRB = 0xff; //utilisation des broches 1 et 2 du port B 
DDRD = 0xf0; // Port D en  entrée


EIMSK |= (1 << INT0); // interruption externe enable INT0

EICRA |= 11; // comamnde indiquant l'évenement  qui provoque interruption:
			 // 01= n'importe quel changement de voltage
			 // 11= rising edge
sei ();
}

bool bouton_APPUYE()
{ 
    if ( PIND & 0x04 ) {
        _delay_ms(10); 
        if ( PIND & 0x04 ) {
            return true;
        }
    } 
    else {
        _delay_ms(10);
        if ( PIND & 0x04 ) {
            return bouton_APPUYE();
        }
    }
    return false;
}
ISR(INT0_vect)
{
// laisser un delai avant de confirmer la réponse du
// bouton-poussoir: environ 30 ms (anti-rebond)
	_delay_ms(30);
	if( bouton_APPUYE() ) { 
		PORTB = 0x02;
		boutonPoussoir = 1;
	}	
//EIFR |= (1 << INTF0) ; pas besoin pour ce probleme car cest pour 
//clear le flag mais cest fait automatique car on utilise rising edge pr le flag.
}

int main(){

Roue roue;
Robot robot(roue);
initialisation();

	for(;;){
		if(boutonPoussoir){
			PORTB = 0x01;
			robot.roues_.ajustementVitesse(50,50);
			boutonPoussoir =0;
		}
	}
}
