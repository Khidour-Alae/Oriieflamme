#ifndef _FACTIONS_H  /* to avoid multiple inclusions */
#define _FACTIONS_H

typedef struct impl_Faction *Faction;


int hasTheDeckBeenShuffled(Faction *faction);

void reshuffleDeck(Faction *faction);

void discardHand(Faction *faction);

void shuffleDeck(Faction *faction);

void drawCard(Faction *faction);

#endif










