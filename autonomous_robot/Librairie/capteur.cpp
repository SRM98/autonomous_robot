#include "capteur.h"



   //Constructeur et destructeur
Capteur::Capteur() {
    DDRA = 0x00;	//initilise le port en entree
    poteau_= false;
    mur_ = false;
    vide_ = false;
    
}

uint8_t Capteur::calculerDistanceMediane(uint8_t broche){	//Indiquer ou sur le port a le captuer est branche.	
	
	can convertisseur; //declare un objet de la classe can
	
	uint8_t puissance[9];

		uint8_t temp;
		for(int i=0;i<9;i++) { 	// pour 10 elements
			puissance[i] = (convertisseur.lecture(broche)>> 0x2); // lu sur 10 bits, mis sur 8 bits	
		for(int j=i;j>0;j--){
			if(puissance[j]<puissance[j-1]){
				temp=puissance[j];  
				puissance[j]=puissance[j-1];  
				puissance[j-1]=temp;               
			}    
		}     
	} 
	
	uint8_t mediane = puissance[4];

	return mediane;

	
	// IMPORTANT !!
	// < 15 cm = 255 a 145 volts
	// 15 cm = 145 volts
	// pour la detection des objets entre 15 et 60 cm  c'est 145 a 50 volts
								
}

// Pour la detection des obstacles

bool Capteur::detecterVide(uint8_t broche){
   int puissance = calculerDistanceMediane(broche);				//BAd sm3ll mais inevitable pour le bon fonctionnement du code												
		if (puissance >= 150 && puissance < 255){
		return false;	
		}  
		else if (puissance >= 140 && puissance < 150 ){ 
		return false;	
		}
		else if (puissance >= 50 && puissance <  140) {
		return false;			
		}	
		else if (puissance > 50 && puissance >= 0){
		return false;		
		}	
		
		return true;
}

uint8_t Capteur::conversionDistanceCm(uint8_t broche){

	uint8_t puissance = calculerDistanceMediane(broche);
	uint8_t distanceCM = 0;
	
	if(puissance >  240 ){
		distanceCM = 0;
		}
	else if(puissance <= 240 && puissance > 190){
		distanceCM = 10;
		}
	else if(puissance <= 190 && puissance > 120){
		distanceCM = 15;
		}
	else if(puissance <= 120 && puissance > 100){
		distanceCM = 25;
	}
	else if(puissance <= 100 && puissance > 88){
		distanceCM = 30;
	}
	else if(puissance <= 88 && puissance > 75){
		distanceCM = 35;
	}
	else if(puissance <= 75 && puissance > 65){
		distanceCM = 40;
	}
	else if(puissance <= 65 && puissance > 63){
		distanceCM = 45;
	}
	else if(puissance <= 63 && puissance > 60){
		distanceCM = 50;
	}
	else if(puissance <= 60 && puissance > 51){
		distanceCM = 55;
	}
	else if(puissance <= 51 && puissance > 45){
		distanceCM = 60;
	}
	else if(puissance <= 45 && puissance > 43){
		distanceCM = 65;
	}
	else if(puissance <= 43 && puissance >= 40){
		distanceCM = 70;
	}
	else if(puissance == 39){
		distanceCM = 75;
	}
	else if(puissance == 38){
		distanceCM = 80;
	}
	else if(puissance <= 38){
		distanceCM = 100;
	}

	return distanceCM;
	 
}
