
/**************************************************************************************************************

Equipe 9899: Hugo Lirette, Sergiu R. Mihaltan, Jennifer Girard, Rosanne Larocque et Ines Goddi.

Note: ce code est la propriete de l'equipe 9899. Quiconque tente d'en faire utitilisation sans notre autorisation
va se voir decerner plusieurs consequences atroces. 
**************************************************************************************************************/


#ifndef PWMSON_H
#define PWMSON_H
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

using namespace std;

class pwmSon{
    
    public:
    
		//Constructeur  
		pwmSon();

		//Mettre le signal a disable
		void sonOff();
		
		//Permet d'ajuster la frequence a laquelle on veut notre pwm
		void sonAjust(int note);
		
		// quand on detecte un poteau
		void detectionPoteau();
		
	private:
		uint8_t tabDesNotes[37] ;

};

#endif
