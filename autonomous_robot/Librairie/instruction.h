
#ifndef Instruction_h
#define Instruction_h

#include <stdio.h>
#include "memoire_24.h"

using namespace std;
class Instruction
{
public:
 
    
    Instruction(uint8_t opcode = 0x00, uint8_t operande = 0x00);	//constructeur
    
    void lireMem(uint16_t adresseI, Memoire24CXXX mem);
    uint8_t getOperande();
    

    uint8_t opCode_;
    uint8_t operande_;

};

#endif /* insctruction_h */

