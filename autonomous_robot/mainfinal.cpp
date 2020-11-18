
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "Librairie/memoire_24.h"
#include <avr/interrupt.h>
#include "Librairie/uart.h"
#include "Librairie/Roue.h"
#include "Librairie/Robot.h"
#include "Librairie/Lumiere.h"
#include "Librairie/pwmSon.h" 

//constantes utilisees pour la reception du diagnostic
const uint8_t codeVitesseGauche = 0xf8;
const uint8_t codeVitesseDroite = 0xf9;
const uint8_t codeCouleurDel = 0xfa;
const uint8_t codeRequete = 0xfb;

const uint8_t AVANCER_D = 0x7f; //broche 8 du port D a 0    0111 1111
const uint8_t AVANCER_G = 0xbf; //broche 7 du port D a 0 , 1011 1111

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

int main() {
	Memoire24CXXX mem;
    uint8_t modeTableau[1];
    mem.lecture(0x00,modeTableau, 1); 		//Lit le mode en memoire.
    mem.ecriture(0x00, !modeTableau[0]);	//Ecrit en memoire l'inverse pour la prochaine lecture.
	
	DDRB = 0xff;
	if(modeTableau[0] == 0){
		//MODE INTERFACE
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
			if(receptionUARTopCode(opCode)&&((opCode==codeVitesseDroite)|(opCode == codeVitesseGauche)|(opCode == codeCouleurDel)|(opCode==codeRequete))){
				if(receptionUARToperande(operande)){
					//DEPENDAMMENT DE CE QUE L'ORDI NOUS ENVOIT
					switch(opCode){
					
						case (codeVitesseDroite):  
							vitesseDroitee = operande;
							if (operande < 0){
								operande *= (-1); // prend la valeur absolue de la vitesse 
								PORTD |= ~AVANCER_D ; //robot recule
								robot.roues_.ajustementVitesse(vitesseGauchee, operande); // conserver la vitesse actuelle a droite, change a gauche selon l'operande
							}
							else{
								PORTD &= AVANCER_D ; //robot avance
								robot.roues_.ajustementVitesse(vitesseGauchee, operande); // conserver la vitesse actuelle a droite, change a gauche selon l'operande
							
							}
							break;
						
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
			else{//TRANSMISSION ETAT
				uart.transmissionEtat(robot);	//GROSSE NOTE:Ne pas oubleir le jumper sur intEn
			}
		}
	}
	else{
	//MODE PARCOURS
	pwmSon son;
	son.sonAjust(50);
	}
}
