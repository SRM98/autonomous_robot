//
//  interrupt.cpp
//  
//
//  Created by Simon Barrette on 18-04-17.
//

// Init du necessaire pour "permettre" les interrupts

void Initialize()
{
    cli();
    
    //DDRB = XXXX
    //DDRD = XXXX
    EIMSK |= _BV(INT0);
    EICRA |= _BV(ISC00);
    
    sei();
}



// Pour attraper l'interrupt

ISR (INT0_vect)
{
    //Action a faire ici
    //
    
    //Reset de linterrupt pour attraper la prochaine action
    EIFR |= (1 << INTF0) ;
}

//Fonction pour demarrer la minuterie
volatile uint8_t minuterieTerminee;
void partirMinuterie(uint16_t duree){
    minuterieTerminee = 0;
    TCNT2 = 1;
    OCR2A = duree;
    TCCR2A |= (1 << COM2A1)|(1 << COM2A0)|(0 << COM2B1)|(0 << COM2B0)|(0 << WGM21)|(0 << WGM20);
    TCCR2B |= (1 << CS20)|(0 << CS21)|(1 << CS22)|(1 << WGM21);
    TIMSK2 |= (1 << OCIE2A);
}
