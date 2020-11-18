#define F_CPU 8000000
#include "Roue.h"


// A FAIRE: vitesse dans ROUE, ajuster selon pourcentage pour l'envoi des infos a l'ordi
Roue::Roue( uint8_t broche, uint8_t vitesseD, uint8_t vitesseG): 
	vitesseD_(vitesseD),
	vitesseG_(vitesseG), 
	broche_(broche){
	
}




void Roue::ajustementVitesse(uint8_t vitesseG,uint8_t vitesseD){
	vitesseD_ = vitesseD;
	vitesseG_ = vitesseG;
	
	uint8_t intensiteDroite = 255 - ((vitesseD * 255)/100); 		// la vitesse (en %) est transforme sur 255 (intensite) 
	uint8_t intensiteGauche = 255 - ((vitesseG * 255)/100);

	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	// page 177 de la description technique du ATmega324PA)
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1) | (1<<COM1A0) | (1<<COM1B0);
	TCCR1B =  (1<<CS11) ;
	TCCR1C = 0;
	
	OCR1A = intensiteDroite;				// rester a une seule fonction pour les deux roues sinon ca ne marche pas pour le parcours !! On doit TOUJOURS setter les 2 roues en même temps sinon ça fait de la marde
	OCR1B = intensiteGauche;  
	
	// division d'horloge par 8 - implique une frequence de PWM fixe

}

void Roue::ajustementVitesseParcours(uint8_t vitesseG,uint8_t vitesseD){
	vitesseD_ = vitesseD;
	vitesseG_ = vitesseG;
	vitesseD = vitesseD-19; // ajustement pour que les roues soient droites;
	
	uint8_t intensiteDroite = 255 - ((vitesseD * 255)/100); 		// la vitesse (en %) est transforme sur 255 (intensite) 
	uint8_t intensiteGauche = 255 - ((vitesseG * 255)/100);

	// mise à un des sorties OC1A et OC1B sur comparaison
	// réussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe à 0xFF (mode #1 de la table 17-6
	// page 177 de la description technique du ATmega324PA)
	TCCR1A |= (1<<WGM10) | (1<<COM1A1) | (1<<COM1B1) | (1<<COM1A0) | (1<<COM1B0);
	TCCR1B =  (1<<CS11) ;
	TCCR1C = 0;
	
	OCR1A = intensiteDroite;				// rester a une seule fonction pour les deux roues sinon ca ne marche pas pour le parcours !! On doit TOUJOURS setter les 2 roues en même temps sinon ça fait de la marde
	OCR1B = intensiteGauche;  
	
	// division d'horloge par 8 - implique une frequence de PWM fixe
}

uint8_t Roue::obtenirVitesseD(){
	
	return vitesseD_;
	
}


uint8_t Roue::obtenirVitesseG(){
	
	return vitesseG_;
	
}
