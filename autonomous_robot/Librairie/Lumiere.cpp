#define F_CPU 8000000
#include "Lumiere.h"

Lumiere::Lumiere(){};

void Lumiere::ajusterDel(uint8_t couleur){
	PORTB = couleur;
}






