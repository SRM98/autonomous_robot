/**************************************************************************************************************

Equipe 9899: Hugo Lirette, Sergiu R. Mihaltan, Jennifer Girard, Rosanne Larocque et Ines Goddi.

Note: ce code est la propriete de l'equipe 9899. Quiconque tente d'en faire utitilisation sans notre autorisation
va se voir decerner plusieurs consequences atroces. 
**************************************************************************************************************/

/******Inclusion de tous les fichiers qu\u2019on a besoins (librairie, librairie d\u2019interrupt, etc.)*****/

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Librairie/Robot.h"
#include "Librairie/instruction.h"
#include "Librairie/Lumiere.h"
#include "Librairie/memoire_24.h"
#include "Librairie/pwmSon.h"



#define vitesseGauche 0xf8 
#define vitesseDroite 0xf9 
#define CouleurDel    0xfa 
#define Requete       0xfb

/* TODO : 
 * NE PAS OUBLIER DE FAIRE LA ROUTINE DINTERRUPTION ********************************
 * DETECTION DE POTEAUX POUR LES DEUX CAPTEURS EN MEME TEMPS
 * 
 * */



int main(){
	DDRA = 0x00;
	DDRB = 0xff;     // PORT B est en mode sortie  
	DDRC = 0xff;     // PORT C est en mode sortie  
	DDRD = 0xff;     // PORT D est en mode sortie  
	Capteur capteur;
	Robot robot;
	pwmSon son;
		
		
		
		
	/**************************************************************************************************************
	* TEST 01 : Le robot doit detecter le mur entre 10 et 60 cm.
	* Methodologie : placer le robot a 10 cm du mur LUM ROUGE et a 60 cm LUM VERTE.
	* VERIFIE ??? : non. 
	* 
	**************************************************************************************************************/
	/*for (;;){
		int puissance = capteur.calculerDistanceMediane(1);				// 1 = capteur droit
																		// 2 = capteur gauche
		if (puissance >= 150 && puissance < 255){
		PORTB = 0x01;   //rouge proche <15cm
		}  
		else if (puissance >= 140 && puissance < 150 ){ 
		PORTB = 0x02;  // vert        a 15 cm
		}
		else if (puissance >= 50 && puissance <  140) {
				PORTB = 0x02;		// clignote vert dans le vide 
			_delay_ms (100);
			PORTB = 0x00; 
			_delay_ms (100);	 // eteint entre 15 et 60 cm
		}
		else if (puissance > 50 && puissance >= 0){
				PORTB = 0x01;		// clignote rouge en bas de 10 cm
			_delay_ms (100);
			PORTB = 0x00; 
			_delay_ms (100);
		}	  
	
	}*/

/**************************************************************************************************************
	* TEST  : detecter vide 
	* 
	**************************************************************************************************************/
	/*for (;;){
		PORTB=0x00;
	uint8_t distance = capteur.calculerDistanceMediane(1);
    if(distance <= 255 && distance >= 0){
		PORTB=0x01; //rouge pas vide
    }
    else {
		PORTB =0x02; //vert videmake 
	}
}*/
	
	/*for (;;){
		int puissance = capteur.calculerDistanceMediane(0);				// 1 = capteur droit
		PORTB = 0x00;																// 2 = capteur gauche
		if (puissance >= 150 && puissance < 255){
		PORTB = 0x01;  
		}  
		else if (puissance >= 140 && puissance < 150 ){ 
		PORTB = 0x01; 
		}
		else if (puissance >= 50 && puissance <  140) {
		PORTB = 0x01;		
		}	
		else if (puissance > 50 && puissance >= 0){
		PORTB = 0x01;		
		}	  
	
	}*/
	

/*
for (;;){
	if(capteur.detecterVide(1)){
		PORTB= 0x01;
	}
	if (!capteur.detecterVide(1)){
		PORTB= 0x02;
	}
}*/

/**************************************************************************************************************
* TEST : calculer medianne
* 
**************************************************************************************************************/
/*robot.initialisation();
for (;;){
	PORTB=0x00;
	uint8_t distance = robot.capteurG_.calculerDistanceMediane(0);
	while(distance >235 ){
		PORTB=0x01;
		for (int i=0; i<15;i++){
		PORTD |= 0xbf ; //robot recule
		robot.roues_.ajustementVitesse(60,60);  
		PORTD &= ~0xbf ; //robot avance	
	}
	break;
	}
	if (distance <140 && distance >=50){											// 145 correspond a 15cm et on veut un range a peu pres pour pas tourner tout le temps
		robot.roues_.ajustementVitesse(40,60);			// on tourne a droite et on veut tourner fluidement donc on fait appel a la methode ajustementvitesse
		 uint8_t distance2 = distance; 
		distance = robot.capteurG_.calculerDistanceMediane(0); 
		if(distance < distance2){ 
		  robot.tournerDroite(60); 
		} 
	}
	if (distance < 235 && distance > 145){ 
		robot.roues_.ajustementVitesse(60,40);
		uint8_t distance2 = distance; 
		distance = robot.capteurG_.calculerDistanceMediane(0); 
		if(distance > distance2){ 
		  robot.roues_.ajustementVitesse(60, 40); 
		} 
	}
	if (distance >= 140 && distance <= 145){
		robot.roues_.ajustementVitesse(60,60);			
	}
}
}*/
/**************************************************************************************************************
* TEST 02 : Le robot doit suivre le mur a 15 cm en sajustant. 
* Methodologie 1 : placer le robot pres dun mur droit et observer la trajectoire
* VERIFIE 1  ??? : non. 
* * Methodologie 2 : placer le robot pres dun mur triangle et observer la trajectoire
* VERIFIE  2 ??? : non. 
* 
**************************************************************************************************************/



/*for (;;){ 
	
/*	uint8_t distance = robot.capteurD_.calculerDistance(1);
	
	if (distance <140){									
		//robot.roues_.ajustementVitesse(80, 10); 			//Good
		robot.roues_.ajustementVitesse(80, 60); 
		if (distance > 145){
			robot.tournerDroite(50);
		}
	}
	else if (distance > 145){
			robot.roues_.ajustementVitesse(0, 80);				
	}
	else{
		robot.roues_.ajustementVitesse(80,80); 
	}
	*/
	//robot.roues_.ajustementVitesse(50,50);
	//robot.ajustementPositionG(80);
	//robot.ajustementPositionG(80);
 // la vitesse max == 255 mais il faut le mettre a 0 pour avoir le max...

 
 /*
 	uint8_t intensiteDroite = 255-(100/100*255); 		// la vitesse (en %) est transforme sur 255 (intensite) 
	uint8_t intensiteGauche = 255-(0/100*255);

	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	// page 177 de la description technique du ATmega324PA)
	
	OCR1A = intensiteDroite;				// rester a une seule fonction pour les deux roues sinon ca ne marche pas pour le parcours !! On doit TOUJOURS setter les 2 roues en même temps sinon ça fait de la marde
	OCR1B = intensiteGauche;  
	
	// division d'horloge par 8 - implique une frequence de PWM fixe
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1) | (1<<COM1A0) | (1<<COM1B0);
	TCCR1B =  (1<<CS11) ;
	TCCR1C = 0;*/
//}
 
/**************************************************************************************************************
* TEST 03  : detection d'obstacles (mur vide et poteau marchent)
* 
**************************************************************************************************************/
/*
 for(;;){
	
	
robot.detectionObstacleD(80);
if(robot.capteurD_.mur_ == true){
	PORTB =0x01;
	_delay_ms(200);
}
if(robot.capteurD_.vide_ == true){
	PORTB=0x02;
	_delay_ms(200);
}
if(robot.capteurD_.poteau_ ==true){
	PORTB =0x01;
	_delay_ms(20);
	PORTB =0x02;
	_delay_ms(20);
	PORTB =0x01;
	_delay_ms(20);
	PORTB =0x02;
	_delay_ms(20);
	PORTB =0x01;
	_delay_ms(20);
	PORTB =0x02;
	_delay_ms(20);
}
}*/

/**************************************************************************************************************
* TEST 04  : enum states
* 
**************************************************************************************************************/
/*	
for(;;){
			enum STATES {E0,E1};
			STATES state;
			state = E0;
			robot.detectionObstacleD(80);

	if(robot.capteurD_.mur_){
				state = E0;		// si on voit un mur a gauche le robot va a gauche
			}	
			if(robot.capteurD_.vide_){
				state = E1;		// si on voit un mur a gauche le robot va a droite
			}

			switch (state){
				
				case E0: PORTB=0x01; break;
				
				case E1: PORTB=0x02; break;
			}
}*/
/**************************************************************************************************************
* TEST 04  : Le main a moitie
* 
**************************************************************************************************************/
	// manque tourner brusque (180 ou autre)
	/*robot.initialisation();
	for(;;){
	PORTB = 0x01;
					robot.ajustementPositionD(60);
					if(!robot.capteurG_.detecterVide(0)){	// il capte quelque chose a gauche
						for (int i = 0; i<6; i++){
							robot.ajustementPositionD(60);
							//_delay_ms (80);
						}
						if(robot.capteurG_.detecterVide(0)){	// verifie si c'est un poteau
							robot.ajustementPositionD(60);
							//_delay_ms (100);
							son.detectionPoteau();	
							robot.capteurG_.poteau_ = true;		
							robot.capteurG_.mur_ = false;
						}
						if(!robot.capteurG_.detecterVide(0) && robot.droit_de_tourner_ == true){
							robot.ajustementPositionD(60);
							//_delay_ms (100);
							robot.droit_de_tourner_ = false;	
					
						


						}
						}
						}
					}*/


/**************************************************************************************************************
* TEST 04  : ajustement position
* 
**************************************************************************************************************/
/*robot.initialisation();
for(;;){
	robot.ajustementPositionG(60);
	PORTB=0x00;
}*/

/**************************************************************************************************************
* TEST :1 seul case a la fois
* 
*******************************************************************************************************/
/*robot.initialisation();
	enum STATES {E0,E1,E3};
	STATES state;
	state = E3;
			

        for(;;){

			switch (state){
			
				case E0:  //postion droite
					robot.ajustementPositionD(60);
					
					if(!robot.capteurG_.detecterVide(0) && !robot.capteurD_.detecterVide(1)){	// il capte quelque chose a gauche
						robot.ajustementPositionD(60);
						_delay_ms (200);
						if(!robot.capteurG_.detecterVide(0) && !robot.capteurD_.detecterVide(1)){	// il capte quelque chose a gauche
						robot.ajustementPositionD(60);
						_delay_ms (200);
							if(!robot.capteurG_.detecterVide(0)){	// verifie si c'est un poteau
								robot.ajustementPositionD(60);
								_delay_ms (100);
								son.detectionPoteau();	
								robot.capteurG_.poteau_ = true;		
								robot.capteurG_.mur_ = false;
								PORTB = 0x01;
							}
							else if(robot.capteurG_.detecterVide(0) ){
								robot.ajustementPositionD(60);
								_delay_ms (100);
								robot.droit_de_tourner_ = false;	
								PORTB=0x02;
								//state = E1;		// suit le mur a gauche
								break;
							}
						}
					
					}
						
					if(!robot.capteurD_.detecterVide(1) && robot.capteurG_.detecterVide(0)){	// il capte quelque chose a droite
						robot.droit_de_tourner_ = true;
						robot.ajustementPositionD(60);
						_delay_ms (200);
					}
					if (robot.capteurG_.detecterVide(0) && robot.capteurD_.detecterVide(1)){
						//robot.tournerDroite(80);
					}	
					break;
									
				case E1:
					
				
				case E3: 		// state du depart
				robot.droit_de_tourner_ = true;
					if(!robot.capteurD_.detecterVide(1) && robot.capteurG_.detecterVide(0)){
						state = E0;		// si on voit un mur a gauche le robot va a gauche
						break;
					}
				PORTB=0x02;
				_delay_ms(100);
				PORTB=0x00;
				_delay_ms(100);				
		}
	}

		*/
/**************************************************************************************************************
	* TEST  : test son 
	* 
	**************************************************************************************************************/
	//son.sonAjust(50);	
				
/**************************************************************************************************************
* TEST 05  : Le main 
* 
*******************************************************************************************************/

	robot.initialisation();
	enum STATES {E0,E1,E3};
	STATES state;
	state = E3;
	//uint8_t tmp = 0;
	//bool doubleObs = false;
			

        for(;;){

			switch (state){
			
				case E0:  //postion droite
					PORTB = 0x01;
					robot.ajustementPositionD(70);
					
					if(!robot.capteurG_.detecterVide(0) && !robot.capteurD_.detecterVide(1)){	// il capte quelque chose a gauche
						//tmp = robot.capteurG_.calculerDistanceMediane(0);
						for(int i = 0; i < 4; ++i){
							robot.ajustementPositionD(70);
							_delay_ms (50);
						}
						if(!robot.capteurG_.detecterVide(0) && !robot.capteurD_.detecterVide(1)){	// il capte quelque chose a gauche
							for(int i = 0; i < 4; ++i){
								robot.ajustementPositionD(70);
								_delay_ms (50);
							}
							/*if(robot.capteurG_.calculerDistanceMediane(0) != tmp){
								doubleObs = true;
							}*/
							if((!robot.capteurG_.detecterVide(0) && robot.droit_de_tourner_)){	// verifie si c'est un poteau
								robot.ajustementPositionD(70);
								_delay_ms (100);
								robot.droit_de_tourner_ = false;
								
								/*if(robot.capteurG_.calculerDistanceMediane(0) < 60){  //si le mur est trop loin 			
										for ( int i =0; i<110;i++){
										robot.changerMurGauche(70); 
										
								}
							}	*/
								state = E1;		// suit le mur a gauche
								break;
							}
							else if(robot.capteurG_.detecterVide(0)){
								robot.ajustementPositionD(70);
								_delay_ms (100);
								PORTB=0X01;
								
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
									
					
				case E1: 		// position gauche
					PORTB=0x02;
					robot.ajustementPositionG(70);
					if(!robot.capteurD_.detecterVide(1) && !robot.capteurG_.detecterVide(0)){	// il capte quelque chose a droite
						//tmp = robot.capteurG_.calculerDistanceMediane(0);
						for(int i = 0; i < 4; ++i){
							robot.ajustementPositionG(70);
							_delay_ms (50);
						}
						if(!robot.capteurD_.detecterVide(1) && !robot.capteurG_.detecterVide(0)){	// il capte quelque chose a droite
							for(int i = 0; i < 4; ++i){
								robot.ajustementPositionG(70);
								_delay_ms (50);
							}
							/*if(robot.capteurG_.calculerDistanceMediane(0) != tmp){
								doubleObs = true;
							}*/
							if(!robot.capteurD_.detecterVide(1) && robot.droit_de_tourner_){	// verifie si c'est un poteau
								robot.droit_de_tourner_ = false;
								robot.ajustementPositionG(70);
								_delay_ms (100);
								/*if(robot.capteurD_.calculerDistanceMediane(1) < 70){  //si le mur est trop loin 
									for ( int i =0; i<16;i++){
										robot.changerMurDroit(70); 
										
									}
									
								}	*/
								state = E0;		// suit le mur a droite
								break;
							}
							else if(robot.capteurD_.detecterVide(1)){
								robot.ajustementPositionG(70);
								_delay_ms (100);
								PORTB=0x00;
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
				
				
				case E3: 		// state du depart
				robot.droit_de_tourner_ = true;
					if(!robot.capteurD_.detecterVide(1) && robot.capteurG_.detecterVide(0)){
						state = E0;		// si on voit un mur a gauche le robot va a gauche
						break;
					}
					if(!robot.capteurG_.detecterVide(0) && robot.capteurD_.detecterVide(1)){
						state = E1;		// si on voit un mur a gauche le robot va a droite
						break;
					}				
		}
	}

}

