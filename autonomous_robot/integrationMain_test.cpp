
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

//constante pour forcer une direction aux roues
const uint8_t AVANCER_D = 0x7f; //broche 8 du port D a 0    0111 1111
const uint8_t AVANCER_G = 0xbf; //broche 7 du port D a 0 , 1011 1111


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
		uartISR.transmissionEtat(robotISR);
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

int main() {
	
	/*                     *****Declaration des variables*****                          */
	/*NOTE: les initialisations propre aux classes sont appelees dans les constructeurs */
	
	//variables communes
	Roue roue;
	Robot robot(roue);	
	Lumiere lumiere;

	//variables DIAGNOSTIC
	Uart uart;
	uint8_t opCode;
	int8_t operande;
	int8_t vitesseDroitee = 0; //variables temporaires, mode interface
	int8_t vitesseGauchee = 0; //variables temporaires, mode interface
	
	//variables PARCOURS
	
	//pwmSon son;
	Capteur capteur;
	enum STATES {DROITE,GAUCHE,INITIAL};
	STATES state;
	
	//declaration variable fin
	
	

	/*****************************___________ALTERNANCE DES MODES___________***********************************************/
	Memoire24CXXX mem;
    uint8_t modeTableau[1];
    mem.lecture(0x00,modeTableau, 1); 		//Lit le mode en memoire.
    mem.ecriture(0x00, !modeTableau[0]);	//Ecrit en memoire l'inverse pour la prochaine lecture.
	
	initialisation();//initialisation commmune aux deux modes
	
	if(modeTableau[0] == 0){
		
	/********************************___________MODE DIAGNOSTIC___________*****************************************/
			
		uart.transmissionId();
	
		
		partirMinuterie(); // lancera une transmission d'etat lorsqu'expiree,sinon on attend les instructions dans la boucle for
		
		for(;;){
			
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

	pwmSon son;
	//son.sonAjust(50);// pour debugg
	
	DDRA = 0x00;
	
	state = INITIAL;	

    for(;;){
		
		switch (state){
			
			
			case DROITE:  //postion droite
				PORTB=0x00;
				robot.ajustementPositionD(70);	//On s'ajuste en tout temps
				if(!robot.capteurG_.detecterVide(0) && !robot.capteurD_.detecterVide(1)){//Verification 1 debounce a gauche
					for(int i = 0; i < 4; ++i){
						robot.ajustementPositionD(70);
						_delay_ms (50);
					}
					if(!robot.capteurG_.detecterVide(0) && !robot.capteurD_.detecterVide(1)){//Verification 2 debounce a gauche
						for(int i = 0; i < 4; ++i){
							robot.ajustementPositionD(70);
							_delay_ms (50);
						}
						
						if((!robot.capteurG_.detecterVide(0) && robot.droit_de_tourner_)){//Verification 3 debounce a gauche
							robot.ajustementPositionD(70);
							_delay_ms (100);
							robot.droit_de_tourner_ = false;
							
							if(robot.capteurG_.calculerDistanceMediane(0) < 60){  //si le mur est trop loin, on change de mur drastiquement 			
								for ( int i =0; i<110;i++){
									robot.changerMurGauche(70); 		
								}
							}	
							state = GAUCHE;		//va en mode suivre le mur a gauche
							break;
						}
						else if(robot.capteurG_.detecterVide(0)){	//C'est un poteau
							robot.ajustementPositionD(70);
							_delay_ms (100);
							son.detectionPoteau();	
							robot.capteurG_.poteau_ = true;		
							robot.capteurG_.mur_ = false;
						}
					}
				
				}
					
				if(!robot.capteurD_.detecterVide(1) && robot.capteurG_.detecterVide(0)){	// il capte quelque chose a droite
					robot.droit_de_tourner_ = true;
					robot.ajustementPositionD(70);
					_delay_ms (200);
				}
				if (robot.capteurG_.detecterVide(0) && robot.capteurD_.detecterVide(1)){
					robot.finMurDroite(70);
				}
				break;
								
				
			case GAUCHE:// position gauche 
				PORTB=0x00;
				robot.ajustementPositionG(70);
				if(!robot.capteurD_.detecterVide(1) && !robot.capteurG_.detecterVide(0)){	// il capte quelque chose a droite
					
					for(int i = 0; i < 4; ++i){
						robot.ajustementPositionG(70);
						_delay_ms (50);
					}
					if(!robot.capteurD_.detecterVide(1) && !robot.capteurG_.detecterVide(0)){	// il capte quelque chose a droite
						for(int i = 0; i < 4; ++i){
							robot.ajustementPositionG(70);
							_delay_ms (50);
						}
						
						if(!robot.capteurD_.detecterVide(1) && robot.droit_de_tourner_){	// verifie si c'est un poteau
							robot.droit_de_tourner_ = false;
							robot.ajustementPositionG(70);
							_delay_ms (100);
							if(robot.capteurD_.calculerDistanceMediane(1) < 60){  //si le mur est trop loin 
								for ( int i =0; i<16;i++){
									robot.changerMurDroit(70); 	
								}
								
							}	
							state = DROITE;		// suit le mur a droite
							break;
						}
						else if(robot.capteurD_.detecterVide(1)){
							robot.ajustementPositionG(70);
							_delay_ms (100);
							son.detectionPoteau();	
							robot.capteurD_.poteau_ = true;		
							robot.capteurD_.mur_ = false;
						}
					}
				}
				
					
				if(!robot.capteurG_.detecterVide(0) && robot.capteurD_.detecterVide(1)){	// il capte quelque chose a droite
					robot.droit_de_tourner_ = true;
					robot.ajustementPositionG(70);
					_delay_ms (100);
				}
				if (robot.capteurG_.detecterVide(0) && robot.capteurD_.detecterVide(1)){
					robot.finMurGauche(70);
				}
				break;
			
			
			case INITIAL: 	// state du depart
				PORTB=0x00;
				robot.droit_de_tourner_ = true;
				if(!robot.capteurD_.detecterVide(1) && robot.capteurG_.detecterVide(0)){
					state = DROITE;		// si on voit un mur a gauche le robot va a gauche
					break;
				}
				if(!robot.capteurG_.detecterVide(0) && robot.capteurD_.detecterVide(1)){
					state = GAUCHE;		// si on voit un mur a gauche le robot va a droite
					break;
				}				
		}
	}

	
	}
}
