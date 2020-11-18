#define F_CPU 8000000
#include <avr/io.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>
#include "Librairie/uart.h"
#include "Librairie/Roue.h"
#include "Librairie/Robot.h"
#include "Librairie/Lumiere.h"

//constantes utilisees pour la reception du diagnostic
const uint8_t codeVitesseGauche = 0xf8;
const uint8_t codeVitesseDroite = 0xf9;
const uint8_t codeCouleurDel = 0xfa;
const uint8_t codeRequete = 0xfb;

const uint8_t AVANCER_D = 0x7f; //broche 8 du port D a 0    0111 1111
const uint8_t AVANCER_G = 0xbf; //broche 7 du port D a 0 , 1011 1111

	Roue roue;
	Robot robot(roue);	
	Uart uart;

volatile uint8_t minuterieExpiree = 0;

void initialisation () {
	cli();
	DDRB = 0xff;		
	DDRD = 0xf0; //port D entree pour bouton sortie pour moteur	
	sei();					
}

ISR(TIMER0_OVF_vect){	
	minuterieExpiree++;
	
	if(minuterieExpiree == 10){
		uart.transmissionEtat(robot);
		minuterieExpiree = 0;
	}
	
	//PORTB = 0x02;
	
	//uart.transmissionEtat(robot);
	
}

void partirMinuterie () {
	
	//mode CTC du timer 1 avec horloge divisee par 1024
	// interruption apres le overflow
	
	 // set up timer with prescaler = 1024
    TCCR0B |= (1 << CS00);//|(1 << CS02);
  
    // initialize counter
    TCNT0 = 0;
  
    // enable overflow interrupt
    TIMSK0 |= (1 << TOIE0);
  

}

uint8_t receptionUARToperande(int8_t& data2) {
	/* Wait for data to be received */
	if(UCSR0A & (1<<RXC0)) {
		/* Get and return received data from buffer */
		data2 = UDR0;
		return 1;
	}
	else{
		return 0;
	}
}

uint8_t receptionUARTopCode(uint8_t& data) {
	/* Wait for data to be received */
	if(UCSR0A & (1<<RXC0)) {
		/* Get and return received data from buffer */
		data = UDR0;
		return 1;
	}
	else{
		return 0;
	}
}



int main()
{
	//INITIALISATION
	
	Lumiere lumiere;
	uart.transmissionId();
	initialisation();
	
	partirMinuterie();
	
	uint8_t opCode;
	int8_t operande;

	int8_t vitesseDroitee = 0; //variables temporaires
	int8_t vitesseGauchee = 0; //variables temporaires
	
	
	for(;;){
			
			if(receptionUARTopCode(opCode)){
			
				if((opCode==codeVitesseDroite)|(opCode == codeVitesseGauche)|(opCode == codeCouleurDel)|(opCode==codeRequete)){
				
					if(receptionUARToperande(operande)){
				
				//DEPENDAMMENT DE CE QUE L'ORDI NOUS ENVOIT
						switch(opCode){
							case (codeVitesseGauche):
								vitesseGauchee = operande;
								
								if (operande < 0){
									operande *= (-1); // prend la valeur absolue de la vitesse 
									PORTD |= ~AVANCER_G; //robot recule
									robot.roues_.ajustementVitesse(operande, vitesseDroitee); // conserver la vitesse actuelle a gauche, change a droite selon l'operande
								}
								else{
									PORTD &= AVANCER_G ; //robot avance
									robot.roues_.ajustementVitesse(operande, vitesseDroitee); // conserver la vitesse actuelle a gauche, change a droite selon l'operande
								}
								
								if(receptionUARTopCode(opCode)){ //considerant que si a cette vitesse un deuxieme opCode est envoye... cest les deux roues en meme temps :
									receptionUARToperande(operande);
								if(opCode == codeRequete){ //sinon pas de break
									receptionUARToperande(operande);
									break;
								}
									
								}
							else
								
								break;
								
								
							case (codeVitesseDroite):  
								vitesseDroitee = operande;
								
								if (operande < 0){
									operande *= (-1); // prend la valeur absolue de la vitesse 
									PORTD |= ~AVANCER_D; //robot recule
									robot.roues_.ajustementVitesse(vitesseGauchee, operande); // conserver la vitesse actuelle a droite, change a gauche selon l'operande
								}
								else{
									PORTD &= AVANCER_D ; //robot avance
									robot.roues_.ajustementVitesse(vitesseGauchee, operande); // conserver la vitesse actuelle a droite, change a gauche selon l'operande
								
								}
								break;
							
							
							case (codeCouleurDel):			
								lumiere.ajusterDel(operande);
								break;
							
							case (codeRequete):
								uart.transmissionId();
								break;
								
						}
						
					}
				
				}
		
			}
	
	}
	
}

