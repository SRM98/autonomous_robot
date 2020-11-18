#include "Robot.h"

uint8_t RECULER_G = 0x7f; //broche 8 du port D a 0    0111 1111
uint8_t RECULER_D = 0xbf; //broche 7 du port D a 0 , 1011 1111

Robot::Robot (){}

Robot::Robot (Roue roue):roues_(roue){
}

/**************************************************************************** 
* Initialisation du robot 
****************************************************************************/ 
void Robot::initialisation(){ 
    DDRB = 0xff;     // PORT B est en mode sortie 
    DDRC = 0xff;     // PORT C est en mode sortie 
    DDRD = 0xff;     // PORT D est en mode sortie 
} 

/****************************************************************************
* MOUVEMENTS
****************************************************************************/

///////	 avec les capteurs	 //////////


// les vitesses ne sont pas encore en %

void Robot::ajustementPositionG(uint8_t vitesse){
	uint8_t distance = capteurG_.calculerDistanceMediane(0);
	while(distance >235 ){									// on fait un ajustement tres sec afin de s'eloigner du mur le plus vite possible et ne pas foncer dedans
		PORTB=0x02;
		for (int i=0; i<1000;i++){
			PORTD |= RECULER_D ; //robot recule
			roues_.ajustementVitesseParcours(vitesse,vitesse);  
			PORTD &= ~RECULER_D ; //robot avance	
		}
		break;
	}
	while(distance < 80 && distance >= 50){
		PORTB=0x02;
		changerMurDroit(vitesse);
		break;
		}
	if (distance <140 && distance >=80){											// 145 correspond a 15cm et on veut un range a peu pres pour pas tourner tout le temps
		PORTB=0x02;
		roues_.ajustementVitesseParcours(vitesse-20,vitesse);			// on tourne a droite et on veut tourner fluidement donc on fait appel a la methode ajustementvitesse
	}
	if (distance > 145 && distance <= 235){ 			// quand il est trop proche
		PORTB=0x02;
		roues_.ajustementVitesseParcours(vitesse,vitesse-20);
		
	}
	if (distance >= 140 && distance <= 145){		// quand il est a la bonne place
		PORTB = 0x01;
		roues_.ajustementVitesseParcours(vitesse,vitesse);			
	}
}

	// IMPORTANT !!
	// < 15 cm = 255 a 145 volts
	// 15 cm = 145 volts
	// pour la detection des objets entre 15 et 60 cm  c'est 145 a 50 volts


void Robot::ajustementPositionD(uint8_t vitesse){
	uint8_t distance = capteurD_.calculerDistanceMediane(1);				
	while (distance >235){	
		PORTB=0x02;
		for (int i=0; i<1000;i++){
			PORTD |= RECULER_G; //robot recule
			roues_.ajustementVitesseParcours(vitesse,vitesse);  
			PORTD &= ~RECULER_G ; //robot avance	
		}
		break;
	}
	while(distance < 80 && distance >= 50){
		PORTB=0x02;
		changerMurGauche(vitesse);
		break;
	}
	if (distance < 140 && distance >= 80){											//quand il est trop loin
		PORTB=0x02;
		roues_.ajustementVitesseParcours(vitesse, vitesse-20);
		 
	}
	if (distance >145 && distance <= 235){
		PORTB=0x02;
		roues_.ajustementVitesseParcours(vitesse-20, vitesse);		// quand il est trop proche	
		
	}
	if (distance >= 140 && distance <= 145) {			// quand il est a la vitesse desiree on veut quil avance droit
		PORTB=0x01;
		roues_.ajustementVitesseParcours(vitesse,vitesse); 
	}
}

void Robot::changerMurGauche(uint8_t vitesse){
	/*for(int i = 0; i < 6; i++){
		roues_.ajustementVitesseParcours(30,vitesse);
	}
	for(int i = 0; i < 5; i++){
		roues_.ajustementVitesseParcours(vitesse,vitesse);
	}
	
	for (int i=0; i<1;i++){
		PORTD |= RECULER_D ; //robot recule
		roues_.ajustementVitesseParcours(vitesse,vitesse);  
		PORTD &= ~RECULER_D; //robot avance
	}*/
	
	while(capteurG_.calculerDistanceMediane(0) < 100){
			PORTB = 0x01;
			roues_.ajustementVitesseParcours(30,vitesse);
			_delay_ms(40);
			roues_.ajustementVitesseParcours(vitesse, vitesse-10);
			_delay_ms(40);
			PORTB = 0x02;
		}
		
}
void Robot::changerMurDroit(uint8_t vitesse){
	/*for(int i = 0; i < 50; i++){
		roues_.ajustementVitesseParcours(30,vitesse);
	}
	for(int i = 0; i < 50; i++){
		roues_.ajustementVitesseParcours(vitesse,vitesse);
	}
	
	for (int i=0; i<3;i++){
		PORTD |= RECULER_G ; //robot recule
		roues_.ajustementVitesseParcours(vitesse-20,vitesse-20);  
		PORTD &= ~RECULER_G ; //robot avance
	}*/
	while(capteurD_.calculerDistanceMediane(1) < 100){
		PORTB = 0x01;
		roues_.ajustementVitesseParcours(vitesse,30);
		_delay_ms(40);
		roues_.ajustementVitesseParcours(vitesse-10, vitesse);
		_delay_ms(40);
		PORTB = 0x02;
	}
}

void Robot::finMurGauche(uint8_t vitesse){
	for(int i = 0; i <6000; i++){
		roues_.ajustementVitesseParcours(vitesse,vitesse);
	}
	for (int i=0; i<2000;i++){
		roues_.ajustementVitesseParcours(0,vitesse+20);
		PORTD |= RECULER_G ; //robot recule
		roues_.ajustementVitesseParcours(vitesse,vitesse);  
		PORTD &= ~RECULER_G ; //robot avance
	}
}

void Robot::finMurDroite(uint8_t vitesse){
	for(int i = 0; i < 7000; i++){
		roues_.ajustementVitesseParcours(vitesse,vitesse);
	}
	for (int i=0; i<1500;i++){
		roues_.ajustementVitesseParcours(vitesse,0);
		PORTD |= RECULER_D ; //robot recule
		roues_.ajustementVitesseParcours(vitesse,vitesse);  
		PORTD &= ~RECULER_D ; //robot avance
	}
}

/****************************************************************************
* MOTEUR
****************************************************************************/
													
void Robot::arreterMoteurs(){ 
   roues_.ajustementVitesse(0,0);
} 


