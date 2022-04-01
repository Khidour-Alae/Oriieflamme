    #include "../headers/cards.h"
#include <stdlib.h>
#include <stdio.h>

#define SIZE_NAME 150
#define EFFECT_DESCRIPTION 1000

typedef enum cardName cardName;
enum cardName {
    FISE,
    FISA,
    FC,
    EcologIIE,
    lIIEns,
    Soiree_sans_alcool,
    Alcool,
    Cafe,
    The,
    Ecocup,
    Reprographie,
    Iolation_du_batiment,
    Parcours_sobriete_numerique,
    Heures_supplementaires,
    Kahina_Bouchama,
    Kevin_Goilard,
    Massinissa_Merabet,
    Vitera_Y,
    Jonas_Senizergues,
    Fetia_Bannour,
    Catherine_Dubois,
    Anne_Laure_Ligozat,
    Guillaume_Burel,
    Christophe_Mouilleron,
    Thomas_Lim,
    Julien_Forest,
    Dimitri_Watel,
    Djibril_Aurélien_Dembele_Cabot,
    Eric_Lejeune,
    Lucienne_Pacave,
    Katrin_Salhab,
    Laurent_Prevel
};


struct impl_card{
    char c_name[SIZE_NAME]; 
    cardName c_enumName;
    char c_effectDescription[EFFECT_DESCRIPTION];
    int numberOfOccurrence;
    int cardStatus; // 0 if Face Down, 1 if Face Up
};

char getCardName(card card){
    return card->c_name;
}

char getCardEffectDescription(card card){
    return card->c_effectDescription;
}

int getNumberOfOccurrence(card card){
    return card->numberOfOccurrence;
}

int getCardStatus(card card){
    return card->cardStatus;
}
