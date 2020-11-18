
#ifndef ROUE_H
#define ROUE_H
#include <avr/io.h> 
#include <util/delay.h> 



class Roue
{
public:
	Roue( uint8_t broche=0x05, uint8_t vitesseD = 0, uint8_t vitesseG =0);
	
	void ajustementVitesse(uint8_t intensiteDroite,uint8_t intensiteGauche);
	void ajustementVitesseParcours(uint8_t intensiteDroite,uint8_t intensiteGauche);
	uint8_t obtenirVitesseD();
	uint8_t obtenirVitesseG();
	
private:
	uint8_t vitesseD_;
	uint8_t vitesseG_;
	uint8_t broche_;
    
};
#endif /* roue_h */
