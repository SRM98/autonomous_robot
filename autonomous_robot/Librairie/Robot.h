#ifndef ROBOT_H
#define ROBOT_H
#define F_CPU 8000000
#include <stdio.h>
#include <avr/io.h> 
#include <util/delay.h> 
#include "Roue.h"
#include "capteur.h"
//#define capteurG 0x00
//#define capteurD 0x01


using namespace std;
class Robot
{
public:
    Robot ();
    Robot (Roue roues);
        
    //INITIALISATION DU ROBOT 
    void initialisation(); 
     
    //MOUVEMENTS
    void ajustementPositionG(uint8_t vitesse);
    void ajustementPositionD(uint8_t vitesse);
    void changerMurGauche(uint8_t vitesse);
    void changerMurDroit(uint8_t vitesse);
    void finMurGauche(uint8_t vitesse);
    void finMurDroite(uint8_t vitesse);
	
	//MOTEURS 
	void arreterMoteurs();
    
    bool droit_de_tourner_;
   
    Capteur capteurG_; //sur la borche 0 du port A 
    Capteur capteurD_; // sur la broche 1 du port A 
    Roue roues_;
   
};
#endif /* robot_h */
