
#include "instruction.h"


Instruction::Instruction(uint8_t opCode, uint8_t operande):opCode_(opCode), operande_(operande) {}

void Instruction::lireMem(uint16_t adresseI, Memoire24CXXX mem){
	
	uint8_t tableau[2];
	
	mem.lecture(adresseI,tableau,sizeof(tableau));    //Note: la methode lecture nous retourne un uint8_
	opCode_ = tableau[0];
	operande_ = tableau[1];
	
}

uint8_t Instruction::getOperande(){
	return operande_;
}
