
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
#include "Librairie/capteur.h"

//constantes utilisees pour la reception du diagnostic
const uint8_t codeVitesseGauche = 0xf8;
const uint8_t codeVitesseDroite = 0xf9;
const uint8_t codeCouleurDel = 0xfa;
const uint8_t codeRequete = 0xfb;

//constante pour forcer une direction aux roues
const uint8_t AVANCER_D = 0x7f; //broche 8 du port D a 0    0111 1111
const uint8_t AVANCER_G = 0xbf; //broche 7 du port D a 0 , 1011 1111

Roue roue;
Robot robot(roue);
pwmSon son;
Capteur capteur;

enum STATES {switchD,switchG,DROITE,GAUCHE,INITIAL};
STATES state;

volatile uint8_t capteur1 = 0;
volatile uint8_t capteur2 = 0;

void detectionPoteau(uint8_t broche ){
	capteur1 = capteur.conversionDistanceCm(broche);
	_delay_ms(30);
	capteur2 = capteur.conversionDistanceCm(broche);

	if((capteur1 - capteur2) > 10){
		son.detectionPoteau();
		
	}
	capteur1 = 0;
	capteur2 = 0;
}

void initialisation () {
	cli();
	DDRB = 0xff; // port B genere PWM son et allumer del	
	DDRD = 0xf0; //port D entree pour bouton sortie pour moteur	
	sei();					
}

/*Varibles necessaires pour ISR*/
volatile uint8_t minuterieExpiree = 0;

Roue roueISR;
Robot robotISR(roueISR);	
Uart uartISR;

ISR(TIMER0_OVF_vect){ 	
	minuterieExpiree++;
	if(minuterieExpiree == 64){
		
		minuterieExpiree = 0;
	}
	
}

void partirMinuterie () {

	// prescaler 1024
    TCCR0B |= (1 << CS00);
  
    // initialize counter
    TCNT0 = 0;
  
    // enable overflow interrupt
    TIMSK0 |= (1 << TOIE0);
  

}

void initialisationISR() {

	cli (); 

	DDRB = 0xff; //utilisation des broches 1 et 2 du port B 
	DDRD = 0xf0; // Port D en  entrée


	EIMSK |= (1 << INT0); // interruption externe enable INT0

	EICRA |= 11; // comamnde indiquant l'évenement  qui provoque interruption:
				 // 01= n'importe quel changement de voltage
				 // 11= rising edge
	sei ();
	}

ISR(INT0_vect){
	if(state == DROITE || state == switchD){
		robot.roues_.ajustementVitesse(0,60);
		_delay_ms(2600);
		robot.roues_.ajustementVitesse(0,0);
	}

	if(state == GAUCHE || state == switchG){
		robot.roues_.ajustementVitesse(60,0);
		_delay_ms(2550);
		robot.roues_.ajustementVitesse(0,0);
	}
}	

int main() {
	
	/*                     *****Declaration des variables*****                          */
	/*NOTE: les initialisations propre aux classes sont appelees dans les constructeurs */
	
	Lumiere lumiere;
	//variables communes
	


	//variables DIAGNOSTIC
	Uart uart;
	uint8_t opCode;
	int8_t operande;
	int8_t vitesseDroitee = 0; //variables temporaires, mode interface
	int8_t vitesseGauchee = 0; //variables temporaires, mode interface
	
	//variables PARCOURS
	
	//pwmSon son;

	
	//declaration variable fin
	
	

	/*****************************___________ALTERNANCE DES MODES___________***********************************************/
	Memoire24CXXX mem;
    uint8_t modeTableau[1];
    mem.lecture(0x00,modeTableau, 1); 		//Lit le mode en memoire.
    mem.ecriture(0x00, !modeTableau[0]);	//Ecrit en memoire l'inverse pour la prochaine lecture.
	
	initialisation();//initialisation commmune aux deux modes
	
	if(modeTableau[0] == 0){
		
	/********************************___________MODE DIAGNOSTIC___________*****************************************/
		initialisation();
		uart.transmissionId();
		son.sonOff();
		
		partirMinuterie(); // lancera une transmission d'etat lorsqu'expiree,sinon on attend les instructions dans la boucle for
		
		for(;;){
			uartISR.transmissionEtat(robotISR);
			//RECEPTION
			
			if(uart.receptionUARTopCode(opCode)){
			
				if((opCode==codeVitesseDroite)|(opCode == codeVitesseGauche)|(opCode == codeCouleurDel)|(opCode==codeRequete)){
					
					if(uart.receptionUARToperande(operande)){
				
				//DEPENDAMMENT DE CE QUE L'ORDI NOUS ENVOIT
						switch(opCode){
							case (codeVitesseGauche):
								vitesseGauchee = operande;//se souvenir de la valeur
								
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
																
							case (codeVitesseDroite):  
								vitesseDroitee = operande;//se souvenir de la valeur
								
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
								
						} // switchs
					} // if opCode
				} // if valide
			}//if operande
		}//for
	}//if
	else{
	
	/********************************************______MODE PARCOURS______*********************************************/
	
	state = INITIAL;
	bool vide = false;
	initialisationISR();

	//son.sonAjust(50);		
	
    for(;;){
		
		switch (state){
			
			
			case DROITE:  //postion droite

			
			detectionPoteau(0);
			detectionPoteau(1);
			if(capteur.conversionDistanceCm(0) > 60){
					vide = true;
				}
		
			if(capteur.conversionDistanceCm(1) == 15){
				PORTB= 0x01;
				robot.roues_.ajustementVitesse(60,55);
				
				if(capteur.conversionDistanceCm(0) <= 50 && vide){
					_delay_ms(10);
					if(capteur.conversionDistanceCm(0) <= 50){
					state = switchG;
					}
				}
			}
			if(capteur.conversionDistanceCm(1) < 15){
				robot.roues_.ajustementVitesse(20,55);
				PORTB=0x02;
				if(capteur.conversionDistanceCm(0) <= 50 && vide){
					_delay_ms(10);
					if(capteur.conversionDistanceCm(0) <= 50){
					state = switchG;
					}
				}
			}
			if(capteur.conversionDistanceCm(1) > 15){
				robot.roues_.ajustementVitesse(70,40);
				PORTB=0x02;
				if(capteur.conversionDistanceCm(0) <= 50 && vide){
					_delay_ms(10);
					if(capteur.conversionDistanceCm(0) <= 50){
					state = switchG;
					}
				}
			}
			
			break;

			case switchD:
			
			detectionPoteau(0);
			detectionPoteau(1);
			PORTB=0x00;
			vide = false;
			while(capteur.conversionDistanceCm(1) != 15){
				robot.roues_.ajustementVitesse(70,55);
					
				if(capteur.conversionDistanceCm(1) <= 20){
					state = DROITE;
					break;
				}
			}

			break;

			case GAUCHE: // position gauche 
				detectionPoteau(0);
				detectionPoteau(1);
				if(capteur.conversionDistanceCm(1) > 60){
					vide = true;
				}

				if(capteur.conversionDistanceCm(0) == 15){
				PORTB= 0x01;
				robot.roues_.ajustementVitesse(70,60);
				robot.roues_.ajustementVitesse(65,60);

				if(capteur.conversionDistanceCm(1) <= 50 && vide){
					_delay_ms(10);
					if(capteur.conversionDistanceCm(1) <= 50){
						state = switchD;
					}
				}
			}
			if(capteur.conversionDistanceCm(0) < 15){
				robot.roues_.ajustementVitesse(55,20);
				PORTB=0x02;
				if(capteur.conversionDistanceCm(1) <= 50 && vide){
					_delay_ms(10);
					if(capteur.conversionDistanceCm(1) <= 50){
						state = switchD;
					}
				}
			}
			if(capteur.conversionDistanceCm(0) > 15){
				PORTB=0x02;
				robot.roues_.ajustementVitesse(40,70);

				if(capteur.conversionDistanceCm(1) <= 50 && vide){
					_delay_ms(10);
					if(capteur.conversionDistanceCm(1) <= 50){
						state = switchD;
					}
				}
			}

				break;
			
			case switchG:
				
				detectionPoteau(0);
				detectionPoteau(1);
				PORTB=0x00;
				vide = false;
				while(capteur.conversionDistanceCm(0) != 15){
					robot.roues_.ajustementVitesse(60,57);
					
				if(capteur.conversionDistanceCm(0) <= 20){
					state = GAUCHE;
					break;
					}
				}
				break;

			case INITIAL: 	// state du depart


			if(capteur.conversionDistanceCm(0) <= 20){
				state = GAUCHE;
				
				break;
			}
			if(capteur.conversionDistanceCm(1) <= 20){
				state = DROITE;
				break;
			}	
		}
	}

	
	}
}
