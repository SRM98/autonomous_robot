#define F_CPU 8000000
#include <avr/io.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>
#include "source/uart.h"
#include "source/Roue.h"
#include "source/Robot.h"
#include "source/Lumiere.h"

//constantes utilisees pour la reception du diagnostic
const uint8_t codeVitesseGauche = 0xf8;
const uint8_t codeVitesseDroite = 0xf9;
const uint8_t codeCouleurDel = 0xfa;
const uint8_t codeRequete = 0xfb;

uint8_t RECULER_D = 0x7f; //broche 8 du port D a 0    0111 1111
uint8_t RECULER_G = 0xbf; //broche 7 du port D a 0 , 1011 1111

void initialisation () {
	DDRB = 0xff;		
	DDRD = 0xf0; //port D entree pour bouton sortie pour moteur						
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

int8_t receptionUARToperande(int8_t& data2) {
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

//2 problemes : -quand on set les ports, on fait de l'interference avec le port d2 
// On dirait que le concept de se definir des variables temporaires pour garder vitesse a lancienne valeur ca chie
// quand on fait les 2 roues en meme temps.  


int main()
{
	//INITIALISATION
	Roue roue;
	Robot robot(roue);	
	Uart uart;
	Lumiere lumiere;
	uart.transmissionId();
	initialisation();

	
	uint8_t opCode;
	int8_t operande;

	int8_t vitesseDroitee = 0; //variables temporaires
	int8_t vitesseGauchee = 0; //variables temporaires
	
	for(;;){
		//RECEPTION

		if(receptionUARTopCode(opCode)){
			
			if(receptionUARToperande(operande)){
				
				//DEPENDAMMENT DE CE QUE L'ORDI NOUS ENVOIT
				switch(opCode){
				
					case (codeVitesseDroite):  
						vitesseDroitee = operande;
						
						if (operande < 0){
							operande *= (-1); // prend la valeur absolue de la vitesse 
							PORTD |= RECULER_G ; //robot recule
							robot.roues_.ajustementVitesse(vitesseGauchee, operande); // conserver la vitesse actuelle a droite, change a gauche selon l'operande
						}
						else{
							PORTD &= ~RECULER_G ; //robot avance
							robot.roues_.ajustementVitesse(vitesseGauchee, operande); // conserver la vitesse actuelle a droite, change a gauche selon l'operande
						
						}
						break;
					
					case (codeVitesseGauche):
						vitesseGauchee = operande;
						
						if (operande < 0){
							operande *= (-1); // prend la valeur absolue de la vitesse 
							PORTD |= RECULER_D; //robot recule
							robot.roues_.ajustementVitesse(operande, vitesseDroitee); // conserver la vitesse actuelle a gauche, change a droite selon l'operande
						}
						else{
							PORTD &= ~RECULER_D; //robot avance
							robot.roues_.ajustementVitesse(operande, vitesseDroitee); // conserver la vitesse actuelle a gauche, change a droite selon l'operande
						}
						break;
					
					case (codeCouleurDel):			
						lumiere.ajusterDel(operande);
						break;
					
					case (codeRequete):
						uart.transmissionId();
						break;
						
					default:
						break;	
				}
			}
		}
		else{//TRANSMISSION ETAT
			uart.transmissionEtat(robot);	//GROSSE NOTE:Ne pas oubleir le jumper sur intEn
		}
	}
}

