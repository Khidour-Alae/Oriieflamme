#ifndef _STRUCTURE_H  
#define _STRUCTURE_H

#include "cards.h"
#include "faction.h"





typedef card deck[NB_CARDS_IN_DECK];
typedef card hand[NB_CARDS_IN_HAND];

typedef struct coupleCardFaction
{
    card c;
    faction f;
} coupleCardFaction;
typedef coupleCardFaction board2D[SIZE_2DBOARD]; 

#endif