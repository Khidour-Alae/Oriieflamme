#include "../headers/board.h"
#include "../headers/structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> //for time()

// #######################################################
// ### The board2D structure
// #######################################################

typedef struct board2DBase * board2D;

/**
* It's recommended to read the comments of board2D before.
* board2DBoundingBox defines the boundingBox of the board2D.
* It just represent a rectangle with two points :
* one point is the lowest left point
* one point is the highest right point
*
**/
typedef struct {
    int xmin; int ymin; int xmax; int ymax;
} board2DBoundingBox;

/**
* board2D is a complex structure. It represents the board,
* that is to say a 2D plane on which you can place cards 
* belonging to factions. The cards can only be placed on 
* integer coordinates (for example : (0,0), (3,2), (-1,-2)).
* 
* To do this, we make two 2D-arrays, one that store the cards
* and one that stores from which faction the card was placed.
* 
* The 2D-arrays are implemented as a 1D-array with the following
* transformation : 1DArray[sizeOfLine * y + x] <=> 2DArray[x][y]
* 
* To represent an infinite 2D plane, the arrays can be
* dynamically resized. For example, if the array covers the
* 2D plane for :
*   # -5 <= x <= 5
*   # -5 <= y <= 5
* and we need to place a card at (5,6), then the array will
* grow in order to cover enough of the 2D plane to place the card
*
* As the array could be covering unecessary parts of the 2D plane
* we are using a bounding box. For example :
* if the array is covering -50 <= x <= 50 and -50 <= y <= 50
* but there are cards only in -10 <= x <= 3 and -7 <= y <= 4
* then the bounding box is used to get this information
* This is usefull mostly for printing the board or iterating
* through the board.
*
**/
struct board2DBase {
    card *c;
    faction *f;
    board2DBoundingBox box;
    int sideLength; //the board is a square
    int sizeBoard2D; //the 1D-array length
};


//defined later but used some functions
int getPositionFromCoordinates_board2D(board2D b2D, int x, int y);
int getXFromPosition_board2D(board2D b2D, int p);
int getYFromPosition_board2D(board2D b2D, int p);

/**
* \brief Initializes the board (mostly memory allocation)
* \param b2D is the board2D
**/
board2D init_board2D() {
    board2D b2D = malloc(sizeof(struct board2DBase));
    b2D->sizeBoard2D = SIZE_2DBOARD;
    b2D->c = malloc(sizeof(card)*(b2D->sizeBoard2D));
    b2D->f = malloc(sizeof(faction)*(b2D->sizeBoard2D));
    for (int i = 0; i < b2D->sizeBoard2D; i++)
    {
        b2D->c[i] = NULL;
        b2D->f[i] = NULL;
    }
    b2D->sideLength = 4*NB_CARDS_IN_HAND + 1;
    
    b2D->box.xmin = -1;
    b2D->box.ymin = -1;
    b2D->box.xmax = 1;
    b2D->box.ymax = 1;
    return b2D;
}

/**
* \brief Check if the board is empty
* \param b2D is the board2D
* \return 0 if there is at least one card placed on the board, 1 otherwise
**/
int isEmpty_board2D(board2D b2D) {
    for (int i = 0; i < b2D->sizeBoard2D; i++)
    {
        if (b2D->c[i] != NULL)
        {
            return 1;
        }
    }
    return 0;
}


/**
* \brief get the card placed at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param x is the x-coordinate
* \param y is the y-coordinate
* \return NULL if there are no card placed at at coordinates ( \a x, \a y); return the card otherwise
**/
card getCard_board2D(board2D b2D, int x, int y) {
    return b2D->c[getPositionFromCoordinates_board2D(b2D,x,y)];
}

/**
* \brief get the faction that played the card placed at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param x is the x-coordinate
* \param y is the y-coordinate
* \return NULL if there are no card placed at coordinates ( \a x, \a y); return the faction that played the card otherwise
**/
faction getFaction_board2D(board2D b2D, int x, int y) {
    return b2D->f[getPositionFromCoordinates_board2D(b2D,x,y)];
}

void resize_board2D(board2D b2D) {
    b2D->sideLength += 2;
    b2D->sizeBoard2D = b2D->sideLength * b2D->sideLength;
    card *newc = malloc(sizeof(card)*b2D->sizeBoard2D);
    faction *newf = malloc(sizeof(card)*b2D->sizeBoard2D);
    
    for (int i = 0; i < b2D->sideLength-2; i++)
    {
        for (int j = 0; j < b2D->sideLength-2; j++)
        {
            newc[(i+1)*b2D->sideLength + (j+1)] = (b2D->c[i*b2D->sideLength + j]);
            newf[(i+1)*b2D->sideLength + (j+1)] = (b2D->f[i*b2D->sideLength + j]);
        }
    }

    free(b2D->c); free(b2D->f);
    b2D->c = newc; b2D->f = newf;
}

int min(int a, int b) {return a < b ? a : b;}
int max(int a, int b) {return a < b ? b : a;}

/**
* \brief place a card \a c at coordinates ( \a x, \a y)
* \param b2D is the board2D
* \param c is the card
* \param f is the faction that played the card
* \param x is the x-coordinate at which you want to place the card
* \param y is the y-coordinate at which you want to place the card
**/
void addCard_board2D(board2D b2D, card c, faction f, int x, int y) {
    //check if we need to resize the board
    int p = getPositionFromCoordinates_board2D(b2D,x,y);
    if (p < 0 || p >= b2D->sizeBoard2D)
    {
        resize_board2D(b2D);
    }

    //resize the boundingBox if needed
    b2D->box.xmin = min(x,b2D->box.xmin);
    b2D->box.ymin = min(y,b2D->box.ymin);
    b2D->box.xmax = max(x,b2D->box.xmax);
    b2D->box.ymax = max(y,b2D->box.ymax);

    //check if we can place the card in the board
    if (getCard_board2D(b2D,x,y) != NULL)
    {
        //there already is a card there
        //raise error
    }
    else //we place the card
    {
        //place the card
        int p = getPositionFromCoordinates_board2D(b2D,x,y);
        b2D->c[p] = c; b2D->f[p] = f;
    }
}

/**
* \brief puts the board back to its original state (that is to say the empty board)
* \param b2D is the board2D
**/
void reset_board2D(board2D b2D) {
    free(b2D->c);
    free(b2D->f);
    
    b2D->sizeBoard2D = SIZE_2DBOARD;
    b2D->c = malloc(sizeof(card)*(b2D->sizeBoard2D));
    b2D->f = malloc(sizeof(faction)*(b2D->sizeBoard2D));
    for (int i = 0; i < b2D->sizeBoard2D; i++)
    {
        b2D->c[i] = NULL;
        b2D->f[i] = NULL;
    }
    b2D->sideLength = 4*NB_CARDS_IN_HAND + 1;
    
    b2D->box.xmin = -1;
    b2D->box.ymin = -1;
    b2D->box.xmax = 1;
    b2D->box.ymax = 1;
}

/**
* \brief Deletes the board2D (free allocated memory)
* \param b2D is the board2D
**/
void delete_board2D(board2D b2D) {
    free(b2D->c);
    free(b2D->f);
}

/**
* \brief get the bounding box of the board2D
* \param b2D is the board2D
* \param xmin is the x coordinate of the lowest left point of the bounding box (=rectangle)
* \param ymin is the y coordinate of the lowest left point of the bounding box (=rectangle)
* \param xmax is the x coordinate of the highest right point of the bounding box (=rectangle)
* \param ymax is the y coordinate of the highest right point of the bounding box (=rectangle)
**/
void getBoundingBox(board2D b2D, int *xmin, int *ymin, int *xmax, int *ymax) {
    (*xmin) = b2D->box.xmin;
    (*ymin) = b2D->box.ymin;
    (*xmax) = b2D->box.xmax;
    (*ymax) = b2D->box.ymax;
}

/**
* \brief get the position (index) correstponding to the point ( \a x, \a y)
* \param b2D is the board2D, we need the board2D argument as the board can be dynamically extended
* \param x is the x coordinate of the point
* \param y is the y coordinate of the point
* \return \a p the position corresponding to the point ( \a x, \a y)
**/
int getPositionFromCoordinates_board2D(board2D b2D, int x, int y) {
    int xcenter = b2D->sideLength / 2;
    int ycenter = b2D->sideLength / 2;
    int xshifted = xcenter + x;
    int yshifted = ycenter + y;

    return b2D->sideLength * yshifted + xshifted;
}

/**
* \brief get the x coordinate correstponding to the position \a p
* \param b2D is the board2D, we need the board2D argument as the board can be dynamically extended
* \param p is the position
* \return \a x the x coordinate correstponding to the position \a p
**/
int getXFromPosition_board2D(board2D b2D, int p) {
    int xcenter = b2D->sideLength / 2;
    int xshifted = p % b2D->sideLength;
    return xshifted - xcenter;
}

/**
* \brief get the y coordinate correstponding to the position \a p
* \param b2D is the board2D, we need the board2D argument as the board can be dynamically extended
* \param p is the position
* \return \a y the y coordinate correstponding to the position \a p
**/
int getYFromPosition_board2D(board2D b2D, int p) {
    int ycenter = b2D->sideLength / 2;
    int yshifted = (p - (p % b2D->sideLength))/b2D->sideLength;
    return yshifted - ycenter;
}

// #######################################################
// ### The board structure
// #######################################################

struct board_base
{
    board2D b2D; // Board of the game
    faction listFactions[2]; // List of factions playing on the board 
};





board createBoard(){
    board b;
    b = malloc(sizeof(struct board_base));
    b->b2D = init_board2D();
    faction f1 = initFaction("Communiste");
    faction f2 = initFaction("Capitaliste");
    b->listFactions[0] = f1;
    b->listFactions[1] = f2;
    //set up deck of factions
    card fise = const_card("FISE", "La faction qui a posé cette carte gagne 1 point DDRS.", FISE, 4);
    card fisa = const_card("FISA", "La faction qui a posé cette carte gagne 2 points DDRS si le nombre de cartes retournées sur le plateau (y compris celle-ci) est pair, et 0 sinon.", FISA, 4);
    card fc = const_card("FC", "La faction qui a posé cette carte gagne 4 points DDRS si au moins une autre carte FC est retournée sur le plateau et 0 sinon", FC, 4);
    card ecologiie = const_card("EcologIIE", "La faction qui a posé cette carte gagne 1 point DDRS par carte FISE/FISA/FC retournée.", EcologIIE, 2);
    card liiens = const_card("lIIEns", "Prenez toutes les cartes FISE/FISA/FC retournées, retirez les du plateau, mélangez les et reposez les face cachées une par une sur la gauche de la carte la plus en haut à gauche du plateau, dans cet ordre. Les prochaines cartes à être retournées sont ces cartes là.", lIIEns, 2);
    card soiree_sans_alcool = const_card("Soiree_sans_alcool", "Si au moins une carte alcool est retournée, supprimez toutes les cartes FISE/FISA/FC retournées du plateau. Supprimez ensuite la première et la dernière ligne du plateau. Sinon la faction qui a posé cette carte gagne 5 points DDRS.", Soiree_sans_alcool, 1);
    card alcool = const_card("Alcool", "Supprimez du plateau toutes les cartes qui touchent cette carte-ci (mais laissez la carte Alcool sur le plateau).", Alcool, 1);
    card cafe = const_card("Cafe", "Supprimez toutes les cartes Thé et Alcool retournées sur le plateau. Si une carte Ecocup est retournée sur le plateau, la faction qui a posé cette carte gagne 1 point DDRS. Sinon elle perd 1 point DDRS.", Cafe, 3);
    card the = const_card("The", "Supprimez toutes les cartes Café et Alcool retournées sur le plateau. Si une carte Ecocup est retournée sur le plateau, la faction qui a posé cette carte gagne 1 point DDRS. Sinon elle perd 1 point DDRS.", The, 3);
    card ecocup = const_card("Ecocup", "Cette carte est sans effet.", Ecocup, 1);
    card reprographie = const_card("Reprographie", "La faction adverse de celle qui a posé cette carte perd 1 points DDRS pour chaque paire de cartes retournées et identiques sur le plateau. (S'il y a 3 cartes identiques, cela fait 3 paires).", Reprographie, 1);
    card isolation_du_batiment = const_card("Isolation_du_batiment", "Chaque faction gagne 1 point DDRS par carte non retournée et non supprimée du plateau qu'elle a posée sur le plateau.", Isolation_du_batiment, 1);
    card parcours_sobriete_numerique = const_card("Parcours_sobriete_numerique", "Retournez toutes les cartes non retournées les plus à gauche et à droite de chaque ligne, sans appliquer leur effet.", Parcours_sobriete_numerique, 1);
    card heures_supplementaires = const_card("Heures_supplementaires", "La faction adverse de celle qui a posé cette carte perd 3 points DDRS par carte Heures supplémentaires retournée sur le plateau (y compris celle-ci).", Heures_supplementaires, 1);
    card kahina_bouchama = const_card("Kahina_Bouchama", "Supprimez une carte non retournée du plateau choisie au hasard.", Kahina_Bouchama, 1);
    card kevin_goilard = const_card("Kevin_Goilard", "Supprimez une ligne au hasard, la faction qui a posé cette carte gagne 2 points DDRS par carte supprimée ainsi.", Kevin_Goilard, 1);
    card massinissa_merabet = const_card("Massinissa_Merabet", "La faction qui a posé cette carte réactive l'effet de la dernière carte retournée avant Massinissa Merabet, en faisant comme elle l'avait posée elle-même, même si ce n'est pas le cas.", Massinissa_Merabet, 1);
    card vitera_y = const_card("Vitera_Y", "La faction qui a le moins de points DDRS gagne 3 points DDRS.", Vitera_Y, 1);
    card jonas_senizergues = const_card("Jonas_Senizergues", "Supprimez toutes les cartes Heures supplémentaires retournées du plateau.", Jonas_Senizergues, 1);
    card fetia_bannour = const_card("Fetia_Bannour", "Si la carte Heures supplémentaires est retournée sur le plateau, supprimez toutes les cartes de la ligne et de la colonne où est posée cette carte (y compris celle-ci). Sinon la faction qui a posé cette carte gagne 1 point DDRS par carte Catherine Dubois, Anne-Laure Ligozat, Guillaume Burel, Christophe Mouilleron, Thomas Lim, Julien Forest et Dimitri Watel retournée sur le plateau.", Fetia_Bannour, 1);
    card catherine_dubois = const_card("Catherine_Dubois", "Supprimez la première et la dernière cartes de la ligne et de la colonne où est posée cette carte.", Catherine_Dubois, 1);
    card anne_laure_ligozat = const_card("Anne_Laure_Ligozat", "Pour chaque carte EcologIIE, Ecocup, Isolation du bâtiment et parcours Sobriété numérique retournée, la faction qui a posé cette carte gagne 3 points DDRS et la dernière carte non retournée du plateau est supprimée. (L'effet continue même si Anne-Laure Ligozat ou une des 4 cartes précédemment mentionnées est supprimée).", Anne_Laure_Ligozat, 1);
    card guillaume_burel = const_card("Guillaume_Burel", "Si la faction adverse de celle qui a posé cette carte a plus de points DDRS, la seconde lui vole 3 points DDRS.", Guillaume_Burel, 1);
    card christophe_mouilleron = const_card("Christophe_Mouilleron", "Si la carte Heures supplémentaires est retournée sur le plateau, supprimez toutes les cartes retournées du plateau sauf les cartes Christophe Mouilleron et Heures supplémentaires.", Christophe_Mouilleron, 1);
    card thomas_lim = const_card("Thomas_Lim", "Si Julien Forest n'est par retourné sur le plateau, la faction qui a posé cette carte gagne 3 points DDRS par carte FISE retournée sur le plateau. Sinon la faction adverse perd 1 point DDRS par carte FISE retournée sur le plateau.", Thomas_Lim, 1);
    card julien_forest = const_card("Julien_Forest", "La faction qui a posé cette carte gagne 6 points DDRS par carte FISE retournée sur le plateau si au moins une carte Café est retournée sur le plateau.", Julien_Forest, 1);
    card dimitri_watel = const_card("Dimitri_Watel", "La faction qui a posé cette carte gagne 3 points DDRS par carte FISA ou FC retournée sur le plateau si au moins une carte Thé est retournée sur le plateau.", Dimitri_Watel, 1);
    card djibril_aurelien_dembele_cabot = const_card("Djibril_Aurelien_Dembele_Cabot", "S'il y a plus de 3 cartes retournées sur la ligne de cette carte, la faction qui a posé cette carte gagne 5 points DDRS.", Djibril_Aurelien_Dembele_Cabot, 1);
    card eric_lejeune = const_card("Eric_Lejeune", "Prenez au hasard 5 cartes retournées du plateau (ou toutes les cartes retournées du plateau s'il y a moins de 5). Si une de ces cartes est une carte Catherine Dubois, Anne-Laure Ligozat, Guillaume Burel, Christophe Mouilleron, Thomas Lim, Julien Forest ou Dimitri Watel, mélangez les et placez les à gauche de la case la plus à gauche de la première ligne. Les prochaines cartes à être retournées sont ces cartes là. Sinon, supprimez ces cartes du plateau.", Eric_Lejeune, 1);
    card lucienne_pacave = const_card("Lucienne_Pacave", "S'il y a une carte FISA retournée dans la même ligne ou la même colonne que cette carte, la faction qui a posé cette carte gagne 5 points DDRS.", Lucienne_Pacave, 1);
    card katrin_salhab = const_card("Katrin_Salhab", "Si les cartes Djibril-Aurélien Djembele-Cabeau, Eric Lejeune et Lucienne Pacavé sont retournées, la faction qui a posé cette carte gagne 10 points DDRS. Sinon, retournez toutes les cartes dans la même ligne de cette carte sans appliquer leurs effets.", Katrin_Salhab, 1);
    card laurent_prevel = const_card("Laurent_Prevel", "Si Laurent Prével est la dernière carte retournée du plateau, la faction qui a posé cette carte gagne la manche, quel que soit le nombre de points DDRS des deux factions.", Laurent_Prevel, 1);

    deck d_f1 = getDeck(f1);
    deck d_f2 = getDeck(f2);
    push_deck(fise,d_f1);
    push_deck(copy(fise),d_f1);
    push_deck(copy(fise),d_f1);
    push_deck(copy(fise),d_f1);
    push_deck(fisa,d_f1);
    push_deck(copy(fisa),d_f1);
    push_deck(copy(fisa),d_f1);
    push_deck(copy(fisa),d_f1);
    push_deck(fc,d_f1);
    push_deck(copy(fc),d_f2);
    push_deck(copy(fc),d_f2);
    push_deck(copy(fc),d_f2);
    push_deck(ecologiie,d_f1);
    push_deck(copy(ecologiie),d_f1);
    push_deck(liiens,d_f1);
    push_deck(copy(liiens),d_f1);
    push_deck(soiree_sans_alcool,d_f1);
    push_deck(alcool,d_f1);
    push_deck(cafe,d_f1);
    push_deck(copy(cafe),d_f1);
    push_deck(copy(cafe),d_f1);
    push_deck(the,d_f1);
    push_deck(copy(the),d_f1);
    push_deck(copy(the),d_f1);
    push_deck(ecocup,d_f1);
    push_deck(reprographie,d_f1);
    push_deck(isolation_du_batiment,d_f1);
    push_deck(parcours_sobriete_numerique,d_f1);
    push_deck(heures_supplementaires,d_f1);
    push_deck(kahina_bouchama,d_f1);
    push_deck(kevin_goilard,d_f1);
    push_deck(massinissa_merabet,d_f1);
    push_deck(vitera_y,d_f1);
    push_deck(jonas_senizergues,d_f1);
    push_deck(fetia_bannour,d_f1);
    push_deck(catherine_dubois,d_f1);
    push_deck(anne_laure_ligozat,d_f1);
    push_deck(guillaume_burel,d_f1);
    push_deck(christophe_mouilleron,d_f1);
    push_deck(thomas_lim,d_f1);
    push_deck(julien_forest,d_f1);
    push_deck(dimitri_watel,d_f1);
    push_deck(djibril_aurelien_dembele_cabot,d_f1);
    push_deck(eric_lejeune,d_f1);
    push_deck(lucienne_pacave,d_f1);
    push_deck(katrin_salhab,d_f1);
    push_deck(laurent_prevel,d_f1);

    push_deck(copy(fise),d_f2);
    push_deck(copy(fise),d_f2);
    push_deck(copy(fise),d_f2);
    push_deck(copy(fise),d_f2);
    push_deck(copy(fisa),d_f2);
    push_deck(copy(fisa),d_f2);
    push_deck(copy(fisa),d_f2);
    push_deck(copy(fisa),d_f2);
    push_deck(copy(fc),d_f2);
    push_deck(copy(fc),d_f2);
    push_deck(copy(fc),d_f2);
    push_deck(copy(fc),d_f2);
    push_deck(copy(ecologiie),d_f2);
    push_deck(copy(ecologiie),d_f2);
    push_deck(copy(liiens),d_f2);
    push_deck(copy(liiens),d_f2);
    push_deck(copy(soiree_sans_alcool),d_f2);
    push_deck(copy(alcool),d_f2);
    push_deck(copy(cafe),d_f2);
    push_deck(copy(cafe),d_f2);
    push_deck(copy(cafe),d_f2);
    push_deck(copy(the),d_f2);
    push_deck(copy(the),d_f2);
    push_deck(copy(the),d_f2);
    push_deck(copy(ecocup),d_f2);
    push_deck(copy(reprographie),d_f2);
    push_deck(copy(isolation_du_batiment),d_f2);
    push_deck(copy(parcours_sobriete_numerique),d_f2);
    push_deck(copy(heures_supplementaires),d_f2);
    push_deck(copy(kahina_bouchama),d_f2);
    push_deck(copy(kevin_goilard),d_f2);
    push_deck(copy(massinissa_merabet),d_f2);
    push_deck(copy(vitera_y),d_f2);
    push_deck(copy(jonas_senizergues),d_f2);
    push_deck(copy(fetia_bannour),d_f2);
    push_deck(copy(catherine_dubois),d_f2);
    push_deck(copy(anne_laure_ligozat),d_f2);
    push_deck(copy(guillaume_burel),d_f2);
    push_deck(copy(christophe_mouilleron),d_f2);
    push_deck(copy(thomas_lim),d_f2);
    push_deck(copy(julien_forest),d_f2);
    push_deck(copy(dimitri_watel),d_f2);
    push_deck(copy(djibril_aurelien_dembele_cabot),d_f2);
    push_deck(copy(eric_lejeune),d_f2);
    push_deck(copy(lucienne_pacave),d_f2);
    push_deck(copy(katrin_salhab),d_f2);
    push_deck(copy(laurent_prevel),d_f2);
    return b;
}

void freeBoard(board b) {
    delete_board2D(b->b2D);
}

int newRound(int counterRoundNumber, faction f1, faction f2){
    switch (counterRoundNumber)
    {
    case 3:
        return 0;
        break;
    case 2:
        //check who won the round
        if (getFactionDdrsPoints(f1) > getFactionDdrsPoints(f2))
        {
            setNbRoundWin(f1,1); setNbRoundWin(f2,0);
        }
        else
        {
            setNbRoundWin(f1,0); setNbRoundWin(f2,1);
        }
        setFactionDdrsPoints(f1,0);
        setFactionDdrsPoints(f2,0);

        if (getNbRoundWin(f1) == 2 || getNbRoundWin(f2) == 2) return 0;
        break;
    case 1:
        //check who won the round
        if (getFactionDdrsPoints(f1) > getFactionDdrsPoints(f2))
        {
            setNbRoundWin(f1,1); setNbRoundWin(f2,0);
        }
        else
        {
            setNbRoundWin(f1,0); setNbRoundWin(f2,1);
        }
        setFactionDdrsPoints(f1,0);
        setFactionDdrsPoints(f2,0);
        break;
    default:
        return 1;
    }
    return 1;
}

faction* listFactions(board b){
    return b->listFactions;
}

void putDownCard(board b, card c, faction f, int x, int y){
    addCard_board2D(b->b2D, c, f, x, y);
}


faction getEnemyFaction(board b, faction f)
{
    if (getFactionName(f) == getFactionName(b->listFactions[0]))
    {
        return (b->listFactions[1]);
    }
    else
    {
        return (b->listFactions[0]);
    }
}



int reprographie_nbpoints(board2D b2D, int xmin, int xmax, int ymin, int ymax)
{
    card currentCard;
    int tab[32];
    int k;

    for (k = 0; k < 32; k++)
    {
        tab[k] = 0;
    }

    for (int x = xmin; x <= xmax; x++)
    {
        for (int y = ymin; y < ymax; y++)
        {
            currentCard = getCard_board2D(b2D, x, y);
            if (currentCard !=  NULL && getCardStatus(currentCard))
            {
                switch(getCardEnumName(currentCard))
                {
                    case FISE: 
                        tab[0] = tab[0] + 1;
                        break;
                    
                    case FISA:
                        tab[1] = tab[1] + 1;
                        break;

                    case FC:
                        tab[2] = tab[1] + 1;
                        break;

                    case EcologIIE:
                        tab[3] = tab[1] + 1;
                        break;

                    case lIIEns:
                        tab[4] = tab[1] + 1;
                        break;

                    case Soiree_sans_alcool:
                        tab[5] = tab[5] + 1;
                        break;

                    case Alcool:
                        tab[6] = tab[6] + 1;
                        break;

                    case Cafe:
                        tab[7] = tab[7] + 1;
                        break;

                    case The:
                        tab[8] = tab[8] + 1;
                        break;

                    case Ecocup:
                        tab[9] = tab[9] + 1;
                        break;

                    case Reprographie:
                        tab[10] = tab[10] + 1;
                        break;

                    case Isolation_du_batiment:
                        tab[11] = tab[11] + 1;
                        break;

                    case Parcours_sobriete_numerique:
                        tab[12] = tab[12] + 1;
                        break;

                    case Heures_supplementaires:
                        tab[13] = tab[13] + 1;
                        break;

                    case Kahina_Bouchama:
                        tab[14] = tab[14] + 1;
                        break;

                    case Kevin_Goilard:
                        tab[15] = tab[15] + 1;
                        break;

                    case Massinissa_Merabet:
                        tab[16] = tab[16] + 1;
                        break;

                    case Vitera_Y:
                        tab[17] = tab[17] + 1;
                        break;

                    case Jonas_Senizergues:
                        tab[18] = tab[18] + 1;
                        break;

                    case Fetia_Bannour:
                        tab[19] = tab[19] + 1;
                        break;

                    case Catherine_Dubois:
                        tab[20] = tab[20] + 1;
                        break;

                    case Anne_Laure_Ligozat:
                        tab[21] = tab[21] + 1;
                        break;

                    case Guillaume_Burel:
                        tab[22] = tab[22] + 1;
                        break;

                    case Christophe_Mouilleron:
                        tab[23] = tab[23] + 1;
                        break;

                    case Thomas_Lim:
                        tab[24] = tab[24] + 1;
                        break;

                    case Julien_Forest:
                        tab[25] = tab[25] + 1;
                        break;

                    case Dimitri_Watel:
                        tab[26] = tab[26] + 1;
                        break;

                    case Djibril_Aurelien_Dembele_Cabot:
                        tab[27] = tab[27] + 1;
                        break;
                        
                    case Eric_Lejeune:
                        tab[28] = tab[28] + 1;
                        break;

                    case Lucienne_Pacave:
                        tab[29] = tab[29] + 1;
                        break;

                    case Katrin_Salhab:
                        tab[30] = tab[30] + 1;
                        break;

                    case Laurent_Prevel:
                        tab[31] = tab[31] + 1;
                        break;

                    default:
                        break;
                }
            }
        }
    }
    int s = 0;
    for (k = 0; k < 32; k++)
    {
        if (tab[k] >= 2)
        {
            s += tab[k] * (tab[k] - 1) / 2;
        }
    }
    return s;
}

// We want the scores not to remain >=0
void setFactionDdrsPointsLEGIT(faction f, int s)
{
    setFactionDdrsPoints(f, s*(s>0));
}



///TODO: Verifier les arguments des getFaction_board2D (normalement c'est bon mais on sait jamais)

///DONE: refaire les currendcardboucle2 qui ont x y au lieu de X Y et vérifier qu'on les affecte toujours avant utilisation

///DONE: Virer tous les "int" X dans les boucles 

///TODO: Flip les cards après avoir demandé à damien si c'est bien une erreur de pas avoir flip dans FISE

int min_int(int a, int b)
{
    if (a>b)
    {
        return b;
    }
    else
    {
        return a;
    }
}

int flipCard(board b, card * c){
    /* Récupérer le bounding box de b avec la fonction getBoundingBox
    Il faudra ensuite parcourir le tableau de en haut à gauche jusqu'à en bas à droite, et utiliser la fonction getCard_board2D. 
    S'il n'y a aucune carte, cette fonction revoie 0. 
    S'il y a une carte, il faudra retourner la carte. */
    int xmin, ymin, xmax, ymax;
    getBoundingBox(b->b2D,&xmin,&ymin,&xmax,&ymax);
    card currentCard;

    card currentCard_boucle2;

    faction f;
    faction f2;

    int X, Y;
    int s;
    int s2;
    int p;
    int r;

    card card_tab[17];
    faction fac_tab[17];
    int tab_lenght;
    int X_C, Y_C;

    int k;

    int boolean;
    int bool_top;
    int bool_bottom;
    int bool_left;
    int bool_right;

    for (int y = ymax; y >= ymin; y--)
    {
        for (int x = xmin; x <= xmax; x++)
        {
            currentCard = getCard_board2D(b->b2D,x,y);
            if (currentCard != NULL && !getCardStatus(currentCard)) //if there is a card and it is face down /// TODO: Soit je me fais int soit jsp comment les int marchent, à demander
            {
                //applies the effect
                switch (getCardEnumName(currentCard))
                {
                case FISE: 
                    /* La faction qui a posé cette carte gagne 1 point DDRS. */
                    f = getFaction_board2D(b->b2D,x,y); 
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 1);
                    
                    break;

                case FISA:
                    f = getFaction_board2D(b->b2D,x,y);
                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            s = 1; // We haven't flipped currentCard yet but we have to count it
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && getCardEnumName(currentCard_boucle2) == FISA) 
                            {
                                s += 1;
                            }
                        }
                    }
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 2 * (s%2 == 0));
                    
                    break;

                case FC:
                    f = getFaction_board2D(b->b2D,x,y);
                    for (Y = ymax; Y >= ymin; Y--)
                        {
                        for (X = xmin; X <= xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            s = 0;
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && getCardEnumName(currentCard_boucle2) == FC)
                            {
                                s += 1;
                            }
                        }
                    }
                    
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 4 * (s > 0));
                    break;

                case EcologIIE:
                    f = getFaction_board2D(b->b2D,x,y);
                    s = 0;
                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == FC || getCardEnumName(currentCard_boucle2) == FISE || getCardEnumName(currentCard_boucle2) == FISA))
                            {
                                s += 1;
                            }
                        }
                    }
                    
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + s);
                    break;


                case lIIEns:
                    tab_lenght = 0;
                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == FC || getCardEnumName(currentCard_boucle2) == FISE || getCardEnumName(currentCard_boucle2) == FISA))
                            {
                                setCardStatus(currentCard_boucle2, 0);
                                card_tab[tab_lenght] = currentCard_boucle2;
                                fac_tab[tab_lenght] = getFaction_board2D(b->b2D, X, Y);
                                addCard_board2D(b->b2D, NULL, NULL, X, Y);
                                tab_lenght++;

                                ///DONE: Remettre ces cartes à gauche de la carte le plus en haut à gauche du tableau.
                                

                            }
                        }
                    }
                    // Now we need to find the top leftmost card of the board. 
                    currentCard_boucle2 = NULL; 
                    X_C = 0;
                    Y_C = 0;

                    X = xmin;
                    Y = ymax;
                    while (currentCard_boucle2 == NULL && Y >= ymin)
                    {
                        while (X <= xmax && currentCard_boucle2 == NULL)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            X ++;     
                        }
                        Y ++;
                        x = xmin;
                    }
                    X_C = X;
                    Y_C = Y;
                    X = xmin;
                    Y = ymax;

                    

                    for (int k = 1; k < tab_lenght; k ++)
                    {
                        addCard_board2D(b->b2D, card_tab[k-1], fac_tab[k-1], X_C - k, Y_C);
                    }
                    /// TODO: Ces cartes sont à nouveau cachées et doivent être les premières à être retournées par la suite.
                    
                    break;

                case Soiree_sans_alcool:
                    boolean = 0; // boolean = "We found a flipped alcool card"

                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && getCardEnumName(currentCard_boucle2) == Alcool) /// !=NULL nécessaire?
                            {
                                boolean = 1;
                            }
                        }
                    }
                    if (boolean)
                    {
                        // If boolean, delete all FISE / FISA / FC cards
                        for (Y = ymax; Y >= ymin; Y--)
                        {
                            for (X = xmin; X <= xmax; X++)
                            {
                                currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);

                                if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == FC || getCardEnumName(currentCard_boucle2) == FISE || getCardEnumName(currentCard_boucle2) == FISA))
                                {
                                    p = getPositionFromCoordinates_board2D(b->b2D,X,Y);
                                    addCard_board2D(b->b2D, NULL, NULL, X, Y);
                                }
                            }
                        }

                        // Then delete the first and the last line.
                        for (X = xmin; Y <= xmax; X++)
                        {

                            addCard_board2D(b->b2D, NULL, NULL, X, ymin);
                            addCard_board2D(b->b2D, NULL, NULL, X, ymax);
                        }   
                    }
                    else
                    {
                        f = getFaction_board2D(b->b2D,x,y);
                        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 5);
                    }
                    
                    break;

                
                case Alcool:
                    // Left card
                    if (x - 1 >= xmin)
                    {
                        addCard_board2D(b->b2D, NULL, NULL, x-1, y);
                    }

                    // Right card
                    if (x + 1 <= xmax)
                    {
                        addCard_board2D(b->b2D, NULL, NULL, x+1, y);
                    }

                    // Bottom card
                    if (y - 1 >= ymin)
                    {
                        addCard_board2D(b->b2D, NULL, NULL, x, y-1);
                    }

                    // Top card
                    if (y + 1 <= ymax)
                    {
                        addCard_board2D(b->b2D, NULL, NULL, x, y+1);
                    }
                    
                    break;

                case Cafe:
                    boolean = 0; // boolean = "flipped Ecocup card found"
                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == Alcool || getCardEnumName(currentCard_boucle2) == The))
                            {
                                addCard_board2D(b->b2D, NULL, NULL, X, Y);
                            }

                            else if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == Ecocup))
                            {
                                boolean = 1;
                            }
                        }
                    } 
                    f = getFaction_board2D(b->b2D,x,y);
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) - 1 + 2 * boolean);
                    
                    break;

                case The:
                    boolean = 0; // boolean = "flipped Ecocup card found"
                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; X < xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == Alcool || getCardEnumName(currentCard_boucle2) == Cafe))
                            {
                                addCard_board2D(b->b2D, NULL, NULL, X, Y);
                            }

                            else if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == Ecocup))
                            {
                                boolean = 1;
                            }
                        }
                    } 
                    f = getFaction_board2D(b->b2D,x,y);
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) - 1 + 2 * boolean);
                    
                    break;

                case Ecocup:
                   
                    break;

                case Reprographie:
                    f = getFaction_board2D(b->b2D,x,y);
                    f2 = getEnemyFaction(b, f);
                    setFactionDdrsPointsLEGIT(f2, getFactionDdrsPoints(f2) - reprographie_nbpoints(b->b2D, xmin, xmax, ymin, ymax));
                    
                    break;

                    

                case Isolation_du_batiment:
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymax; Y >= ymin; Y--)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,x,y);
                            if (currentCard_boucle2 != NULL && !getCardStatus(currentCard_boucle2))
                            {
                                f = getFaction_board2D(b->b2D,X,Y);
                                setFactionDdrsPointsLEGIT(getFaction_board2D(b->b2D,X,Y), getFactionDdrsPoints(f) + 1);
                            }
                        }
                    }
                    break;

                case Parcours_sobriete_numerique:
                    for (Y = ymin; Y <= ymax; Y++)
                    {
                        bool_right = 1; // "we haven't found a card yet"
                        bool_left = 1;  // Same
                        X = xmin;
                        while (bool_left && X <= xmax)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && !getCardStatus(currentCard_boucle2))
                            {
                                setCardStatus(currentCard_boucle2, 1);
                                bool_left = 0;
                            }
                            X++;
                        }
                        X = xmax;
                        while (bool_right && X >= xmin)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X, Y);
                            if (currentCard_boucle2 != NULL && !getCardStatus(currentCard_boucle2))
                            {
                                setCardStatus(currentCard_boucle2, 1);
                                bool_right = 0;
                            }
                            Y --;
                        }
                    }
                    break;

                case Heures_supplementaires:
                    s = 1;
                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            s += 1;
                        }
                    }

                    f = getEnemyFaction(b, getFaction_board2D(b->b2D,x,y));
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) - 3*s);
                    break;

                case Kahina_Bouchama:
                    s = 0;
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && !getCardStatus(currentCard_boucle2))
                            {
                                s += 1;
                            }
                        }
                    }
                    r = rand() % (s + 1);
                    s = 0;
                    X = xmin;
                    Y = ymin;
                    /// Vérifier que le X et le Y gardent leur valeur à la sortie de la boucle while
                    while (X <= xmax && s != r)
                    {
                        while (Y <= ymax && s != r)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && !getCardStatus(currentCard_boucle2))
                            {
                                s += 1;
                            }
                            Y++;
                        }
                        X++;
                        Y = ymin;
                    }
                    addCard_board2D(b->b2D, NULL, NULL, X, Y);
                    break;

                case Kevin_Goilard:
                    f = getFaction_board2D(b->b2D,x,y);
                    r = rand()%(ymax - ymin + 1);
                    for (X = xmin; X < xmax; X++)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,X,r);
                        if (currentCard_boucle2 != NULL)
                        {
                            addCard_board2D(b->b2D, NULL, NULL, X, r);
                            setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 2);
                        }
                    }
                    break;
                
                case Massinissa_Merabet:
                    f = getFaction_board2D(b->b2D,x,y);
                    boolean = 1; // we haven't found a card yet
                    X = x - 1;
                    Y = y;
                    
                    while (boolean && Y <= ymax)
                    {
                        while (boolean && X >= xmin)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2))
                            {
                                f2 = getFaction_board2D(b->b2D,X,Y);
                                boolean = 0;
                                setCardStatus(currentCard_boucle2, 0);
                                p = getPositionFromCoordinates_board2D(b->b2D,X,Y);
                                b->b2D->f[p] = f;
                                flipCard(b, &currentCard_boucle2);
                                p = getPositionFromCoordinates_board2D(b->b2D,X,Y);
                                b->b2D->f[p] = f2;
                            }
                            X--;
                        }
                        Y++;
                        X = xmax;
                    }
                    break;


                // If both team happen to have the same score, they both win 3 points since they both have the lowest score (that's the way we decided to see it)
                case Vitera_Y:
                    f = b->listFactions[0];
                    f2 = b->listFactions[0];
                    s = getFactionDdrsPoints(f);
                    s2 = getFactionDdrsPoints(f2);
                    if (s >= s2)
                    {
                        setFactionDdrsPointsLEGIT(f2, getFactionDdrsPoints(f2) + 3);
                    }
                    if (s <= s2)
                    {
                        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3);
                    }
                    break;

                case Jonas_Senizergues:
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,x,y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Heures_supplementaires))
                            {
                                addCard_board2D(b->b2D, NULL, NULL, X, Y);
                            }
                        }
                    }
                    break;

                case Fetia_Bannour:
                    boolean = 1;
                    s = 0;
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Heures_supplementaires))
                            {
                                boolean = 0;
                            }
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Catherine_Dubois || getCardEnumName(currentCard_boucle2) == Anne_Laure_Ligozat || getCardEnumName(currentCard_boucle2) == Guillaume_Burel || getCardEnumName(currentCard_boucle2) == Christophe_Mouilleron || getCardEnumName(currentCard_boucle2) == Thomas_Lim || getCardEnumName(currentCard_boucle2) == Julien_Forest || getCardEnumName(currentCard_boucle2) == Dimitri_Watel))
                            {
                                s += 1;
                            }
                        }
                    }
                    if (boolean)
                    {
                        f = getFaction_board2D(b->b2D,x,y);
                        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + s);
                    }
                    else
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            addCard_board2D(b->b2D, NULL, NULL, X, y);
                        }

                        for (Y = ymin; Y <= ymax; Y++)
                        {
                            addCard_board2D(b->b2D, NULL, NULL, x, Y);
                        }
                    }
                    break;

                case Catherine_Dubois:
                    X = 0;
                    Y = 0;
                    bool_bottom = 1;
                    bool_top = 1;
                    bool_left = 1;
                    bool_right = 1;
                    // Left card
                    while (X <= xmax && bool_left)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,X,y);
                        if (currentCard_boucle2 != NULL)
                        {
                            addCard_board2D(b->b2D, NULL, NULL, X, y);
                            bool_left = 0;
                        }
                        X++;
                    }
                    X = xmax;
                    // Right card
                    while (X >= xmin && bool_right)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,X,y);
                        if (currentCard_boucle2 != NULL)
                        {
                            addCard_board2D(b->b2D, NULL, NULL, X, y);
                            bool_right = 0;
                        }
                        X--;
                    }
                    // Top card
                    while (Y <= ymax && bool_top)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,x,Y);
                        if (currentCard_boucle2 != NULL)
                        {
                            addCard_board2D(b->b2D, NULL, NULL, x, Y);
                            bool_top = 0;
                        }
                        Y++;
                    }
                    Y = ymax;
                    // Bottom card
                    while (Y >= ymin && bool_bottom)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,x,Y);
                        if (currentCard_boucle2 != NULL)
                        {
                            addCard_board2D(b->b2D, NULL, NULL, x, Y);
                            bool_bottom = 0;
                        }
                        Y--;
                    }
                    break;

                case Anne_Laure_Ligozat:
                    s = 0;
                    s2 = 0;
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == EcologIIE || getCardEnumName(currentCard_boucle2) == Ecocup || getCardEnumName(currentCard_boucle2) == Isolation_du_batiment || getCardEnumName(currentCard_boucle2) ==  Parcours_sobriete_numerique))
                            {
                                s ++;
                            }
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 0)
                            {
                                s2++;
                            }
                        }
                    }
                    for (Y = ymax; Y >= ymin; Y--)
                    {
                        for (X = xmin; Y <= ymax; X++)
                        {
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 0)
                            {
                                s2 += 1;
                                if (s == s2)
                                {
                                    addCard_board2D(b->b2D, NULL, NULL, X, Y);
                                }
                            }
                        }
                    }

                    f = getFaction_board2D(b->b2D,x,y);
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3*s);
                    break;

                case Guillaume_Burel:
                    f = getFaction_board2D(b->b2D,x,y);
                    f2 = getEnemyFaction(b, f);
                    s = getFactionDdrsPoints(f);
                    s2 = getFactionDdrsPoints(f2);
                    // Here we decided to allow the faction f to steal points only when their score is stricly inferior since it seemed to be more in the spirit of the card
                    if (s < s2)
                    {
                        if (s2 < 3)
                        {
                            setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + getFactionDdrsPoints(f2));
                            setFactionDdrsPointsLEGIT(f2, 0);
                        }
                        else
                        {
                            setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3);
                            setFactionDdrsPointsLEGIT(f2, getFactionDdrsPoints(f2) - 3);
                        }
                    }
                    break;

                case Christophe_Mouilleron:
                    boolean = 1;
                    X = xmin;
                    Y = ymin;
                    while (X <= xmax && boolean)
                    {
                        while (Y <= ymax && boolean)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Heures_supplementaires))
                            {
                                boolean = 0;
                            }
                            Y++;
                        }
                        Y = ymin;
                        X++;
                    }
                    if (!boolean)
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            for (Y = ymin; Y < ymax; Y++)
                            {
                                currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                                if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && getCardEnumName(currentCard_boucle2) != Christophe_Mouilleron && getCardEnumName(currentCard_boucle2) != Heures_supplementaires)
                                {
                                    addCard_board2D(b->b2D, NULL, NULL, X, Y);
                                }
                            }
                        }
                    }
                    break;

                case Thomas_Lim:
                    boolean = 1; // boolean = "Forest isn't on the board or isn't flipped"
                    X = xmin;
                    Y = ymin;
                    while (X <= xmax && boolean)
                    {
                        while (Y <= ymax && boolean)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Julien_Forest))
                            {
                                boolean = 0;
                            }
                            Y ++;
                        }
                        X ++;
                        Y = ymin;
                    }
        
                    s = 0;
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == FISE))
                            {
                            }
                        }
                    }
                    f = getFaction_board2D(b->b2D,x,y);
                    if (boolean)
                    {
                        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3*s);
                    }
                    else
                    {
                        f2 = getEnemyFaction(b, f);
                        setFactionDdrsPointsLEGIT(f2, getFactionDdrsPoints(f2) - s);
                    }
                    break;

                case Julien_Forest:
                    boolean = 1; // Boolean = "Cafe isn't on the board or isn't flipped"
                    X = xmin;
                    Y = ymin;
                    while (X <= xmax && boolean)
                    {
                        while (Y <= ymax && boolean)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Cafe))
                            {
                                boolean = 0;
                            }
                            Y++;
                        }
                        X++;
                        Y = ymin;
                    }
                    if (!boolean)
                    {
                        s = 0;
                        for (X = xmin; X <= xmax; X++)
                        {
                            for (Y = ymin; Y < ymax; Y++)
                            {
                                currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                                if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == FISE))
                                {
                                    s += 1;
                                }
                            }
                        }
                        f = getFaction_board2D(b->b2D,x,y);
                        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 6*s);
                    }
                    break;
                
                case Dimitri_Watel:
                    boolean = 1; // Boolean = "The isn't on the board or isn't flipped"
                    X = xmin;
                    Y = ymin;
                    while (X <= xmax || boolean)
                    {
                        while (Y <= ymax || boolean)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == The))
                            {
                                boolean = 0;
                            }
                        }
                    }
                    if (!boolean)
                    {
                        s = 0;
                        for (X = xmin; X <= xmax; X++)
                        {
                            for (Y = ymin; Y < ymax; Y++)
                            {
                                currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                                if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == FISA || getCardEnumName(currentCard_boucle2) == FC))
                                {
                                    s += 1;
                                }
                            }
                        }
                        f = getFaction_board2D(b->b2D,x,y);
                        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3*s);
                    }
                    break;

                case Djibril_Aurelien_Dembele_Cabot:
                    s = 0;
                    for (X = xmin; X <= xmax; X++)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,X,y);
                        if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1)
                        {
                            s += 1;
                        }
                    }
                    f = getFaction_board2D(b->b2D,x,y);
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 5*(s >= 2)); // Since we count currentCard
                    break;

                case Eric_Lejeune:///mamamia la galère


                    s = 0; //s number of flipped cards.
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2))
                            {
                                s ++;
                            }
                        }
                    }

                    int rep[2*NB_CARDS_IN_HAND];
                    int n;
                    for (n = 0; n < s; n++)
                    {
                        rep[n] = n;
                    }
                    
                    //Fisher–Yates shuffle algorithm
                    srand(time(NULL));
                    int j;
                    for (int i = 0; i < s -2; i++)
                    {
                        j = rand()%((s)-i) + i;
                        int tmp = rep[i];
                        rep[i] = rep[j];
                        rep[j] = tmp;
                    }

                    // Now we need to find the top leftmost card of the board. 
                    currentCard_boucle2 = NULL; 
                    X_C = 0;
                    Y_C = 0;

                    X = xmin;
                    Y = ymax;
                    while (Y >= ymin && currentCard_boucle2 == NULL)
                    {
                        while (currentCard_boucle2 == NULL && X <= xmax)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            Y ++;     
                        }
                        Y --;
                        X = xmin;
                    }
                    X_C = X;
                    Y_C = Y;
                    
                    s2 = -1; //s number of flipped cards.
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2))
                            {
                                s2 ++;
                                k = 0;
                                for (k = 0; k < min_int(5,s); k++)
                                {
                                    if (s2 == rep[k])
                                    {
                                        addCard_board2D(b->b2D, currentCard_boucle2, f, X_C - k, Y_C);
                                        addCard_board2D(b->b2D, NULL, NULL, X, Y);
                                    }
                                }
                            }
                        }
                    }

                    break;


                case Lucienne_Pacave:
                    boolean = 1; // boolean = "we didn't find a fisa card yet"
                    for (X = xmin; X <= xmax; X++)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,X,y);
                        if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == FISA))
                        {
                            boolean = 0;
                        }
                    }
                    for (Y = ymin; Y <= ymax; Y++)
                    {
                        currentCard_boucle2 = getCard_board2D(b->b2D,x,Y);
                        if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == FISA))
                        {
                            boolean = 0;
                        }
                    }

                    f = getFaction_board2D(b->b2D,x,y);
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 5*(1 - boolean));
                    break;
                    

                case Katrin_Salhab:
                
                    bool_left = 1; //Djibril-Aurélien Djembele-Cabeau
                    boolean = 1; //Eric Lejeune
                    bool_right = 1; //Lucienne Pacavé
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Djibril_Aurelien_Dembele_Cabot))
                            {
                                bool_left = 0;
                            }
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Eric_Lejeune))
                            {
                                boolean = 0;
                            }
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Lucienne_Pacave))
                            {
                                bool_right = 0;
                            }
                        }
                    }
                    f = getFaction_board2D(b->b2D,x,y);
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 10*(bool_left && boolean && bool_right));
                    if (!(bool_left && boolean && bool_right))
                    {
                        for (X = xmin; X <= xmax; X++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,y);
                            if (currentCard_boucle2 != NULL)
                            {
                                setCardStatus(currentCard_boucle2, 1);
                            }
                        }
                    }
                    break;

                case Laurent_Prevel:
                    boolean = 1;
                    for (X = xmin; X <= xmax; X++)
                    {
                        for (Y = ymin; Y < ymax; Y++)
                        {
                            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 0 && (getCardEnumName(currentCard_boucle2) != Laurent_Prevel))
                            {
                                boolean = 0;
                            }
                        }
                    }
                    f = getFaction_board2D(b->b2D,x,y);
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 1000 * (boolean));
                    break;            
                
                default:
                    break;
                }

                currentCard = getCard_board2D(b->b2D,x,y);
                if (currentCard != NULL)
                {
                    setCardStatus(currentCard, 1);
                }

                //return the card flipped and boolean 1
                *c = currentCard; 
                return 1;
            }
        }
    }

    return 0;
}

/* Trucs utiles

p = getPositionFromCoordinates_board2D(b->b2D,x,Y);
b2D->c[p] = NULL; 
b2D->f[p] = NULL;



f = getFaction_board2D(b->b2D,x,y);
setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3*s);


for (X = xmin; X <= xmax; X++)
{
    for (Y = ymin; Y < ymax; Y++)
    {
        currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
        if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Name_Card))
        {
        }
    }
}


    card currentCard_boucle2;

    faction f;
    faction f2;

    int X, Y;
    int s;
    int s2;
    int p;
    int r;

    card card_tab[17];
    faction fac_tab[17];
    int tab_lenght;
    int X_C, Y_C;
    card tmp_card;

    int k;

    bool boolean;
    bool bool_gauche;
    bool bool_droit;
*/

void getBoundingBoxOfTheBoardToPrint(board b, int *xmin, int *ymin, int *xmax, int *ymax) {
    getBoundingBox(b->b2D,xmin,ymin,xmax,ymax);
}

int isCardPlacedAt(board b, int x, int y) {
    int p = getPositionFromCoordinates_board2D(b->b2D,x,y);
    if (p < b->b2D->sizeBoard2D && p >= 0 && getCard_board2D(b->b2D,x,y) != NULL) return 1;
    else return 0;
}

int isFlipped(board b, int x, int y)
{
    board2D b2D = b->b2D;
    card c = getCard_board2D(b2D, x, y);
    return getCardStatus(c);
}

void clearBoard(board b) {
    reset_board2D(b->b2D);
}