
/**************************************************************************************************************

Equipe 9899: Hugo Lirette, Sergiu R. Mihaltan, Jennifer Girard, Rosanne Larocque et Ines Goddi.

Note: ce code est la propriete de l'equipe 9899. Quiconque tente d'en faire utitilisation sans notre autorisation
va se voir decerner plusieurs consequences atroces. 
**************************************************************************************************************/
/**************************************************************************************************************
Selon la documentation Atmel, a la page 137, quand on utilise le mode clear timer on compare match (CTC mode),
la frequence de l'onde est definie par f_OCnx = f_clk_I_O / (2 * N * (1+OCRnx)). Dans ce cas-ci, on a les 
frequences que l'on veut obtenir. De ce fait, il faut tout simplement isoler  OCRnx.

Note: N correspond au prescaler.(Le prescaler est ici 256)

On isole OCRnx et on obtient la formule suivante:

OCRnx = (f_clk_I_O/(2*N*(f_OCnx ) - 1)

**************************************************************************************************************/


#include "pwmSon.h"
#include <util/delay.h> 


/**************************************************************************************************************
PWMINIT: Permet d'initialiser les timers du microContoller.
**************************************************************************************************************/
void pwmInit(){
    PORTB &= 0xfb; //simule un GND a la broche 3 du port B , 1111 1011 
    
    TCNT0 = 0; //p.210
    
    TCCR0A |= (1<<WGM01)|(1<<COM0A0); 
    
    TCCR0B |= (1<<CS02); //p.208 division horloge par 256
}


/**************************************************************************************************************
CONSTRUCTEUR: On initialise notre attribut tableau au tableau de frequence fournit sur le site web du cours.

**************************************************************************************************************/

pwmSon::pwmSon(){
	uint8_t temp []= {141,133,126,118,113,106,
				 99,94,88,83,79,74,70,66,
				 62,59,55,52,49,46,44,41,
				 39,37,35,33,31,29,27,26,
				 25,23,21,20,19,18,17}; //voir ci-haut pour la formule des valeurs
				 
	for(int i = 0 ; i<37; i++)
		tabDesNotes[i]=temp[i];
	
	pwmInit();//On initialise nos timers.			 
}


/**************************************************************************************************************
Sert lorsque le robot detecte un poteau
**************************************************************************************************************/
// a ameliorer avec un meilleur son (la note a ete mis au hasard ici)

void pwmSon::detectionPoteau(){
	sonAjust(50);
	_delay_ms (200);
	sonOff();
	_delay_ms (100);
	sonAjust(50);
	_delay_ms (200);
	sonOff();
	_delay_ms (100);
	sonAjust(50);
	_delay_ms (200);
	sonOff();
}


/**************************************************************************************************************
PWMOFF: Permet de mettre le signal du pwm a disable.
**************************************************************************************************************/
void  pwmSon::sonOff(){
   	
   TCCR0A &= ~(_BV(COM0A1)) & ~(_BV(COM0A0));

   
}

/**************************************************************************************************************
PWMAJUST: Permet d'ajuster la frequence du pwm.
**************************************************************************************************************/
void pwmSon::sonAjust(int note){
	
	
	if(note >= 45 && note <=81){
		pwmInit();
		uint8_t frequence = tabDesNotes[note-45];
		OCR0A = frequence; //p.211
		

	}

}



