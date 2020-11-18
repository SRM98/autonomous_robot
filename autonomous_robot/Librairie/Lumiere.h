#ifndef LUMIERE_H
#define LUMIERE_H
#include <avr/io.h>

// Pour la Del nous utilisons la convention que la Del est toujours branchée sur les 2 premières broches du portB.

class Lumiere
{
public:
    
    Lumiere();
    
    void ajusterDel(uint8_t couleur);

private:

};
#endif /* lumiere_h */
