#include "uart.h"



void Uart::initialisationUART() {
	// 2400 bauds. Nous vous donnons la valeur des deux
	// premier registres pour vous eviter des complications
	UBRR0L = 0xCF;
	UBRR0H = 0;

	//UCSR0A = _BV(UDRE0);
	//permettre reception et transmission
	UCSR0B |=_BV(RXEN0) | _BV(TXEN0);  // _BV(RXCIE0) | _BV(TXCIE0) | _BV(UDRIE0) | 

	UCSR0C |= 0x06; // mot de 8 bits 
}


Uart::Uart(){
	
	initialisationUART(); //initialisation faite à la construction 
	
}


/****************************************************************************
* ROBOT vers PC, envoie l'octer en parametre dans le buffer
****************************************************************************/
void Uart::transmissionUART(uint8_t donnee) {
	//Wait for empty transmit buffer.
	while (!(UCSR0A & (1 << UDRE0)));		
	//Put data into buffer, sends the data.
	UDR0 = donnee;							
}

/****************************************************************************
* PC vers ROBOT, retourne l'octet recu 
****************************************************************************/
//Ancienne version..
/*int8_t Uart::receptionUART() {
	 Wait for data to be received 
	while ( !(UCSR0A & (1<<RXC0)) );
	 Get and return received data from buffer 
	return UDR0;
}
* 
*/

int8_t Uart::receptionUARToperande(int8_t& data2) {
	/* Wait for data to be received */
	if(UCSR0A & (1<<RXC0)) {
		/* Get and return received data from buffer */
		data2 = UDR0;
		return 1;
	}
	else{
		return 0;
	}
}

uint8_t Uart::receptionUARTopCode(uint8_t& data) {
	/* Wait for data to be received */
	if(UCSR0A & (1<<RXC0)) {
		/* Get and return received data from buffer */
		data = UDR0;
		return 1;
	}
	else{
		return 0;
	}
}
/****************************************************************************
* Nom: transmissionEtat 
* Description: fonction qui sert à lire l'état des capteurs, des roues
* et de l'interruption INT0 du robot, puis les envoit dans le buffer
* parametres : Robot robot
* retour : Aucun 
****************************************************************************/
void Uart::transmissionEtat(Robot robot){
	
	uint8_t etat_Bouton = 1 ; // etat bouton donnée par la lecture du port D
	
	Bouton bouton;
	bool etat_Bool = bouton.estAppuye(); //on regarde letat du bouton interrupt
	
	
	if(etat_Bool)
		etat_Bouton = 0; // pour avoir en uint8
		
	uint8_t dist_Gauche = robot.capteurG_.conversionDistanceCm(0);//dans classe capteur //1 octet
	uint8_t dist_Droite = robot.capteurD_.conversionDistanceCm(1); // dans classe capteur //1 octet	
	
	//bon tableau
	uint8_t infoMEM[] = {etatBouton,etat_Bouton,distGauche,dist_Gauche,distDroite,dist_Droite}; // avec un _ : valeur et  sans _: code de l'info (voir define) 
	
	//tableau pour debugg
	//uint8_t infoMEM[] = {etatBouton,etat_Bouton,distGauche,20,distDroite,50}; 
	
	for (uint8_t i=0; i<sizeof(infoMEM);i++){
		transmissionUART(infoMEM[i]);		
	}
}
	
/****************************************************************************
* Nom: transmissionId
* Description: fonction qui sert à envoyer les infos d'indentification du robot:
*nom, noEquipe, section, session, couleur. Infos envoyé au buffer  
* parametres : aucun
* retour : Aucun 
****************************************************************************/

void Uart::transmissionId(){
	uint8_t infoMEM[]= {nomBot,'B','B','9',numEquipe,'9','8','9','9',numSection,5, 
		sessionHiv,'1','8','-','1',couleurBase, 1};	
	
	for (uint8_t i=0; i<sizeof(infoMEM);i++){
		transmissionUART(infoMEM[i]);
	}
}

	
	


