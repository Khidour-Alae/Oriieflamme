#ifndef _STRUCTURE_H  
#define _STRUCTURE_H

#include "cards.h"
#include "faction.h"

#define NB_CARDS_IN_DECK 44
#define NB_CARDS_IN_HAND 8
#define SIZE_2DBOARD 1089



typedef card deck[NB_CARDS_IN_DECK];
typedef card hand[NB_CARDS_IN_HAND];

typedef struct coupleCardFaction
{
    card c;
    faction f;
} coupleCardFaction;
typedef coupleCardFaction board2D[SIZE_2DBOARD]; 

#endif