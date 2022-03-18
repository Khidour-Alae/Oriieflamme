#ifndef _FACTIONS_H  /* to avoid multiple inclusions */
#define _FACTIONS_H

typedef struct impl_faction *faction;


int hasTheDeckBeenShuffled(faction faction);

void reshuffleDeck(faction *faction);

void discardHand(faction *faction);

void shuffleDeck(faction *faction);

void drawCard(faction *faction);

#endif










