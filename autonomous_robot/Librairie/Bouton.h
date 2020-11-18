//
//  Bouton.hpp
//  classeRobot
//
//  Created by Rosanne Larocque on 18-03-05.
//  Copyright © 2018 Rosanne Larocque. All rights reserved.
//



#ifndef Bouton_hpp
#define Bouton_hpp
#define F_CPU 8000000
#include <stdio.h>
#include <avr/io.h> 
#include <util/delay.h> 


using namespace std;
class Bouton
{
public:
    
    Bouton();

    // autres méthodes
    bool estAppuye();
    
    
private:

};

#endif /* Bouton_hpp */
