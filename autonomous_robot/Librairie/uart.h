#ifndef DEF_UART
#define DEF_UART
#define F_CPU 8000000
#include "Bouton.h"
#include <Robot.h>
#include <avr/io.h>
#include "memoire_24.h"
#include <util/delay.h> 
#include <stdio.h>
#include <avr/interrupt.h>
//Definition des premiers octets a envoyer vers pc
const uint8_t nomBot        =0xf0 
			 ,numEquipe     =0xf1
			 ,numSection    =0xf2
             ,sessionHiv    =0xf3
             ,couleurBase	=0xf4
             ,etatBouton	=0xf5
             ,distGauche	=0xf6
             ,distDroite	=0xf7;

//Definition des premiers octets recu par pc
#define vitesseGauche 0xf8 
#define vitesseDroite 0xf9 
#define CouleurDel    0xfa 
#define Requete       0xfb

class Uart
{
public:

void initialisationUART();
Uart();


//du robot vers le PC
void transmissionUART(uint8_t donnee);

// Du PC vers l'USART
//int8_t receptionUART(); //ancienne version

uint8_t receptionUARTopCode(uint8_t& data);
int8_t receptionUARToperande(int8_t& data2);

void transmissionId();

void transmissionEtat(Robot robot);

};

#endif
