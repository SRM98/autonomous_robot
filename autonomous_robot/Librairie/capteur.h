#ifndef DEF_CAPTEUR
#define DEF_CAPTEUR

#include <stdio.h>
#include <avr/io.h> 
#include "can.h"


class Capteur
{
public:
    
    //Constructeur et destructeur
    Capteur();
    
    uint8_t conversionDistanceCm(uint8_t broche);
    uint8_t calculerDistanceMediane(uint8_t broche);

    //obstacles
    bool detecterVide(uint8_t broche);
    
    // les attributs public afin de mieux travailler avec dans le main    

    

    bool vide_;
	bool mur_;
	bool poteau_;
	
};

#endif
