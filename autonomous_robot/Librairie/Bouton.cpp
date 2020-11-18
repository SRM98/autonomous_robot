//
//  Bouton.cpp
//  classeRobot
//
//  Created by Rosanne Larocque on 18-03-05.
//  Copyright © 2018 Rosanne Larocque. All rights reserved.
//

#include "Bouton.h"



/****************************************************************************
 * Fonction: Bouton::Bouton
 * Description: Constructeur par default
 * Paramètres: Aucun
 * Retour: aucun
 ****************************************************************************/

Bouton::Bouton(){}

bool Bouton::estAppuye(){	//Connecte au port d2
	bool test=false;
	if ( PIND & 0x04 ){ //si le bouton est appuye
		_delay_ms(10);									
		if ( PIND & 0x04 ) 
		test = true; 		
	}
	return test;
}
