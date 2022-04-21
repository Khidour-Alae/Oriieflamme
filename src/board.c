#include "../headers/board.h"
#include "../headers/structure.h"
#include <stdio.h>
#include <stdlib.h>

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
void init_board2D(board2D b2D) {
    b2D->sizeBoard2D = SIZE_2DBOARD;
    b2D->c = malloc(sizeof(card)*b2D->sizeBoard2D);
    b2D->f = malloc(sizeof(faction)*b2D->sizeBoard2D);
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
            newc[(i+1)*b2D->sideLength + (j+1)] = (b2D->f[i*b2D->sideLength + j]);
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
    if (x == 0 || x == b2D->sideLength - 1 || y == 0 || y == b2D->sideLength - 1)
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
    init_board2D(b2D);
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
    init_board2D(b->b2D);
    faction f1;
    faction f2;
    initFaction(f1);
    initFaction(f2);
    b->listFactions[0] = f1;
    b->listFactions[1] = f2;
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
    return b;
}

void freeBoard(board b){
    delete_board2D(b->b2D);
}

int newRound(int counterRoundNumber, faction f1, faction f2){
    if (counterRoundNumber == 3) return 0;
    if (getNbRoundWin(f1) == 2 || getNbRoundWin(f2) == 2) return 0;
    return 1;
}

faction* listFactions(board b){
    return b->listFactions;
}

void putDownCard(board b, card c, faction f, int x, int y){
    addCard_board2D(b->b2D, c, f, x, y);
}

int flipCard(board b, card *c){
    /* Récupérer le bounding box de b avec la fonction getBoundingBox
    Il faudra ensuite parcourir le tableau de en haut à gauche jusqu'à en bas à droite, et utiliser la fonction getCard_board2D. 
    S'il n'y a aucune carte, cette fonction revoie NULL. 
    S'il y a une carte, il faudra retourner la carte. */
}
