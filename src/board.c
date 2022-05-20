#include "../headers/board.h"
#include "../headers/structure.h"
#include "../headers/interface.h"
#include "../headers/jsonParser.h"
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
    int p = getPositionFromCoordinates_board2D(b2D,x,y);
    return b2D->c[p];
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

    //place the card on the board
    b2D->c[p] = c; b2D->f[p] = f;
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
    int yshifted = ycenter - y;
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
    jsParser jsP = initParser("other/deck.json");
    card *cards;
    cards = malloc(sizeof(card)*getSize_json(jsP));
    for (int i = 0; i < getSize_json(jsP); i++)
    {
        cards[i] = const_card(getCardName_json(jsP,i), getCardDescription_json(jsP,i), getCardId_json(jsP,i), getCardOccurences_json(jsP,i));
    }
    
    deck d_f1 = getDeck(f1);
    deck d_f2 = getDeck(f2);

    for (int i = 0; i < getSize_json(jsP); i++)
    {
        for (int k = 0; k < getNumberOfOccurrence(cards[i]); k++)
        {
            push_deck(copy(cards[i]),d_f1);
            push_deck(copy(cards[i]),d_f2);
        }
    }
    
    free(cards);
    freeParser(jsP);
    return b;
}

void freeBoard(board b) {
    delete_board2D(b->b2D);
}


///TODO: board b consequences
int newRound(int counterRoundNumber, board b, faction f1, faction f2){
    faction f;
    switch (counterRoundNumber)
    {
    case 3:
        return 0;
        break;
    case 2:
        //check who won the round
        f = roundWinner(b, f1, f2);
        setNbRoundWin(f,getNbRoundWin(f) + 1);
        printRoundWinner(f,2);

        setFactionDdrsPoints(f1,0);
        setFactionDdrsPoints(f2,0);

        if (getNbRoundWin(f1) == 2 || getNbRoundWin(f2) == 2) return 0;
        break;
    case 1:
        //check who won the round
        f = roundWinner(b, f1, f2);
        setNbRoundWin(f,getNbRoundWin(f) + 1);
        printRoundWinner(f,1);
        
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

// CARD EFFECTS //////////////////////////////
void applyLIIensEffect(board b, int xmin, int xmax, int ymin, int ymax)
{
    int tab_lenght; int Y; int X; int X_C; int Y_C;
    card card_tab[16];
    faction fac_tab[16];
    tab_lenght = 0;
    card currentCard_boucle2; 
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
        Y --;
        X = xmin;
    }
    X_C = X;
    Y_C = Y;
    X = xmin;
    Y = ymax;

                    

    for (int k = 1; k < tab_lenght; k ++)
    {
        addCard_board2D(b->b2D, card_tab[k-1], fac_tab[k-1], X_C - k, Y_C);
    }
}


void applyFISAEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    faction f = getFaction_board2D(b->b2D,x,y);
    int s = 1;
    int Y; int X;
    card currentCard_boucle2;
    for (Y = ymax; Y >= ymin; Y--)
    {
        for (X = xmin; X <= xmax; X++)
        {
            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && getCardEnumName(currentCard_boucle2) == FISA) 
            {
                s += 1;
            }
        }
    }
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 2 * (s%2 == 0));
}



void applyFCEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int Y; int X;
    card currentCard_boucle2;
    int s;
    faction f = getFaction_board2D(b->b2D,x,y);
    for (Y = ymax; Y >= ymin; Y--)
        {
        for (X = xmin; X <= xmax; X++)
        {
            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && getCardEnumName(currentCard_boucle2) == FC)
            {
                s += 1;
            }
        }
    }
                    
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 4 * (s > 0));

}

void applySoiree_sans_alcoolEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
        int boolean = 0; // boolean = "We found a flipped alcool card"
        card currentCard_boucle2;
        int X; int Y;
        faction f;

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
}

void applyEcologIIEEffect(board b, int xmax, int xmin, int ymax, int ymin, int x, int y)
{
    faction f = getFaction_board2D(b->b2D,x,y);
    int s = 0;
    int X; int Y;
    card currentCard_boucle2;
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

}

void applyCafeEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int boolean = 0; // boolean = "flipped Ecocup card found"
    int X; int Y;
    card currentCard_boucle2;

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
    faction f = getFaction_board2D(b->b2D,x,y);
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) - 1 + 2 * boolean);                 
}

void applyTheEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int boolean = 0; // boolean = "flipped Ecocup card found"
    int X; int Y;
    card currentCard_boucle2;
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
    faction f = getFaction_board2D(b->b2D,x,y);
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) - 1 + 2 * boolean);
                    
}


void applyIsolationEffect(board b, int xmin, int xmax, int ymin, int ymax)
{
    int X; int Y;
    card currentCard_boucle2;
    faction f;
    for (X = xmin; X <= xmax; X++)
    {
        for (Y = ymax; Y >= ymin; Y--)
        {
            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
            if (currentCard_boucle2 != NULL && !getCardStatus(currentCard_boucle2))
            {
                f = getFaction_board2D(b->b2D,X,Y);
                setFactionDdrsPointsLEGIT(getFaction_board2D(b->b2D,X,Y), getFactionDdrsPoints(f) + 1);
            }
        }
    }
}

void applyParcoursEffect(board b, int xmin, int xmax, int ymin, int ymax)
{
    int bool_right; int bool_left; int Y; int X;
    card currentCard_boucle2;
    
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
}


void applyKahinaEffect(board b, int xmin, int xmax, int ymin, int ymax)
{
    int s = 0;
    int X; int Y;
    card currentCard_boucle2;
    int r;
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
}


void applyHeuresSupEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int s = 1;
    int Y; int X;
    faction f;
    card currentCard_boucle2;

    for (Y = ymax; Y >= ymin; Y--)
    {
        for (X = xmin; X <= xmax; X++)
        {
            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) && (getCardEnumName(currentCard_boucle2) == Heures_supplementaires))
            {
                s += 1;
            }
        }
    }

    f = getEnemyFaction(b, getFaction_board2D(b->b2D,x,y));
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) - 3*s);
}

void applyKevinEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int X;
    card currentCard_boucle2;
    faction f = getFaction_board2D(b->b2D,x,y);
    int r = rand()%(ymax - ymin + 1);
    for (X = xmin; X < xmax; X++)
    {
        currentCard_boucle2 = getCard_board2D(b->b2D,X,r);
        if (currentCard_boucle2 != NULL)
        {
            addCard_board2D(b->b2D, NULL, NULL, X, r);
            setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 2);
        }
    }
}


void applyMassinissaEffect(board b, int xmin, int xmax, int ymax, int x, int y)
{
    faction f = getFaction_board2D(b->b2D,x,y); faction f2;
    int boolean = 1; // we haven't found a card yet
    int X = x - 1;
    int Y = y;
    card currentCard_boucle2;
    int p;
    
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
}

void applyJonasEffect(board b, int xmin, int xmax, int ymin, int ymax)
{
    int X; int Y;
    card currentCard_boucle2;

    for (X = xmin; X <= xmax; X++)
    {
        for (Y = ymin; Y < ymax; Y++)
        {
            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == Heures_supplementaires))
            {
                addCard_board2D(b->b2D, NULL, NULL, X, Y);
            }
        }
    }
}

void applyFetiaEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int boolean = 1;
    int s = 0;
    int X; int Y;
    card currentCard_boucle2;
    faction f;

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
}


void applyCatherineEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int X = xmin;
    int Y = ymin;
    int bool_bottom = 1;
    int bool_top = 1;
    int bool_left = 1;
    int bool_right = 1;
    card currentCard_boucle2;

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
}

void applyAnneEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int X; int Y;
    card currentCard_boucle2;
    int s = 0;
    int s2 = 0;
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

    faction f = getFaction_board2D(b->b2D,x,y);
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3*s);
}


void applyMouilleronEffect(board b , int xmin, int xmax, int ymin, int ymax)
{
    int boolean = 1;
    int X = xmin;
    int Y = ymin;
    card currentCard_boucle2;
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
                    addCard_board2D(b->b2D, NULL, NULL, X, Y); // Remove card
                }
            }
        }
    }
}


void applyLimEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int boolean = 1; // boolean = "Forest isn't on the board or isn't flipped"
    int X = xmin;
    int Y = ymin;
    card currentCard_boucle2;
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
        
    int s = 0;
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
    faction f = getFaction_board2D(b->b2D,x,y);
    if (boolean)
    {
        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3*s);
    }
    else
    {
        faction f2 = getEnemyFaction(b, f);
        setFactionDdrsPointsLEGIT(f2, getFactionDdrsPoints(f2) - s);
    }
}

void applyForestEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int s = 0;
    int boolean = 1; // Boolean = "Cafe isn't on the board or isn't flipped"
    int X = xmin;
    int Y = ymin;
    card currentCard_boucle2;
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
        faction f = getFaction_board2D(b->b2D,x,y);
        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 6*s);
    }  
}


void applyWatelEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int boolean = 1; // Boolean = "The isn't on the board or isn't flipped"
    int X = xmin;
    int Y = ymin;
    card currentCard_boucle2;
    int s;

    while (X <= xmax && boolean)
    {
        while (Y <= ymax && boolean)
        {
            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1 && (getCardEnumName(currentCard_boucle2) == The))
            {
                boolean = 0;
            }
            Y++;
        }
        X++;
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
        faction f = getFaction_board2D(b->b2D,x,y);
        setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 3*s);
    }

}

void applyDjibrilEffect(board b, int xmin, int xmax, int x, int y)
{
    int X; 
    card currentCard_boucle2;
    int s = 0;
    for (X = xmin; X <= xmax; X++)
    {
        currentCard_boucle2 = getCard_board2D(b->b2D,X,y);
        if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2) == 1)
        {
            s += 1;
        }
    }
    faction f = getFaction_board2D(b->b2D,x,y);
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 5*(s >= 2)); // Since we count currentCard
}


void applyEricEffect(board b, int xmin, int xmax, int ymin, int ymax)
{
    
    int X; int Y;
    card currentCard_boucle2;
    int s = 0; //s number of flipped cards.
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
    int X_C = 0;
    int Y_C = 0;

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
    
    int s2 = -1; //s number of flipped cards.
    int k;
    faction f;

    
    int boolean = 0;
    // boolean = we found such card
    for (X = xmin; X <= xmax; X++)
    {
        for (Y = ymin; Y < ymax; Y++)
        {
            currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
            if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2))
            {
                s2 ++;
                for (k = 0; k < min_int(5,s); k++)
                {
                    if (s2 == rep[k])
                    {
                        if (getCardEnumName(currentCard_boucle2) == Catherine_Dubois || getCardEnumName(currentCard_boucle2) == Anne_Laure_Ligozat || Guillaume_Burel || getCardEnumName(currentCard_boucle2) == Christophe_Mouilleron || getCardEnumName(currentCard_boucle2) ==Thomas_Lim || getCardEnumName(currentCard_boucle2) == Julien_Forest || getCardEnumName(currentCard_boucle2) == Dimitri_Watel)
                        {
                            boolean = 1;
                        }
                    }
                }
            }
        }
    }

    if (boolean)
    {
        for (X = xmin; X <= xmax; X++)
        {
            for (Y = ymin; Y < ymax; Y++)
            {
                currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2))
                {
                    s2 ++;
                    for (k = 0; k < min_int(5,s); k++)
                    {
                        if (s2 == rep[k])
                        {
                            f = getFaction_board2D(b->b2D, X, Y);
                            addCard_board2D(b->b2D, currentCard_boucle2, f, X_C - k, Y_C);
                            addCard_board2D(b->b2D, NULL, NULL, X, Y);
                        }
                    }
                }
            }
        }
    }
    else
    {
        for (X = xmin; X <= xmax; X++)
        {
            for (Y = ymin; Y < ymax; Y++)
            {
                currentCard_boucle2 = getCard_board2D(b->b2D,X,Y);
                if (currentCard_boucle2 != NULL && getCardStatus(currentCard_boucle2))
                {
                    s2 ++;
                    for (k = 0; k < min_int(5,s); k++)
                    {
                        if (s2 == rep[k])
                        {
                            addCard_board2D(b->b2D, NULL, NULL, X, Y);
                        }
                    }
                }
            }
        }
    }

}


void applyLucienneEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int X; int Y;
    card currentCard_boucle2;
    int boolean = 1; // boolean = "we didn't find a fisa card yet"
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

    faction f = getFaction_board2D(b->b2D,x,y);
    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 5*(1 - boolean));
}


void applyKatrinEffect(board b, int xmin, int xmax, int ymin, int ymax, int x, int y)
{
    int bool_left = 1; //Djibril-Aurélien Djembele-Cabeau
    int boolean = 1; //Eric Lejeune
    int bool_right = 1; //Lucienne Pacavé
    int X; int Y;
    card currentCard_boucle2;
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
    faction f = getFaction_board2D(b->b2D,x,y);
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
}
// FIN CARD EFFECT ///////////////////

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


    int boolean;


    for (int y = ymax; y >= ymin; y--)
    {
        for (int x = xmin; x <= xmax; x++)
        {
            currentCard = getCard_board2D(b->b2D,x,y);
            if (currentCard != NULL && !getCardStatus(currentCard)) //if there is a card and it is face down 
            {
                currentCard = getCard_board2D(b->b2D,x,y);
                if (currentCard != NULL)
                {
                    setCardStatus(currentCard, 1);
                }

                //return the card flipped and boolean 1
                *c = currentCard; 

                //applies the effect
                switch (getCardEnumName(currentCard))
                {
                case FISE: 
                    /* La faction qui a posé cette carte gagne 1 point DDRS. */
                    f = getFaction_board2D(b->b2D,x,y); 
                    setFactionDdrsPointsLEGIT(f, getFactionDdrsPoints(f) + 1);

                    break;

                case FISA:
                    /* La faction qui a posé cette carte gagne 2 points DDRS si le nombre de cartes retournées sur le plateau (y compris celle-ci) est pair, et 0 sinon. */
                    applyFISAEffect(b, xmin, xmax, ymin, ymax, x, y);

                    break;

                case FC:
                    /* La faction qui a posé cette carte gagne 4 points DDRS si au moins une autre carte FC est retournée sur le plateau et 0 sinon */
                    applyFCEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case EcologIIE:
                    /* La faction qui a posé cette carte gagne 1 point DDRS par carte FISE/FISA/FC retournée. */
                    applyEcologIIEEffect(b, xmax, xmin, ymax, ymin, x, y);
                    break;

                case lIIEns:
                    applyLIIensEffect(b, xmin, xmax, ymin, ymax);
                    break;

                case Soiree_sans_alcool:
                    applySoiree_sans_alcoolEffect(b, xmin, xmax, ymin, ymax, x, y);
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
                    applyCafeEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case The:
                    applyTheEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case Ecocup:         
                    break;

                case Reprographie:
                    f = getFaction_board2D(b->b2D,x,y);
                    f2 = getEnemyFaction(b, f);
                    setFactionDdrsPointsLEGIT(f2, getFactionDdrsPoints(f2) - reprographie_nbpoints(b->b2D, xmin, xmax, ymin, ymax));  
                    break;

                    

                case Isolation_du_batiment:
                    applyIsolationEffect(b, xmin, xmax, ymin, ymax);
                    break;

                case Parcours_sobriete_numerique:
                    applyParcoursEffect(b, xmin, xmax, ymin, ymax);
                    break;

                case Heures_supplementaires:
                    applyHeuresSupEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case Kahina_Bouchama:
                    applyKahinaEffect(b, xmin, xmax, ymin, ymax);
                    break;

                case Kevin_Goilard:
                    applyKevinEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;
                
                case Massinissa_Merabet:
                    applyMassinissaEffect(b, xmin, xmax, ymax, x, y);
                    break;


                // If both team happen to have the same score, they both win 3 points since they both have the lowest score (that's the way we decided to see it)
                case Vitera_Y:
                    f = b->listFactions[0];
                    f2 = b->listFactions[1];
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
                    applyJonasEffect(b, xmin, xmax, ymin, ymax);
                    break;

                case Fetia_Bannour:
                    applyFetiaEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case Catherine_Dubois:
                    applyCatherineEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case Anne_Laure_Ligozat:
                    applyAnneEffect(b, xmin, xmax, ymin, ymax, x, y);
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
                    applyMouilleronEffect(b, xmin, xmax, ymin, ymax);
                    break;

                case Thomas_Lim:
                    applyLimEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case Julien_Forest:
                    applyForestEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;

                case Dimitri_Watel:
                    applyWatelEffect(b , xmin, xmax, ymin, ymax, x, y);
                    break;

                case Djibril_Aurelien_Dembele_Cabot:
                    applyDjibrilEffect(b, xmin, xmax, x, y);
                    break;

                case Eric_Lejeune:///mamamia la galère
                    ///TODO: il manque le 2e cas xd
                    applyEricEffect(b, xmin, xmax, ymin, ymax);
                    break;


                case Lucienne_Pacave:
                    applyLucienneEffect(b, xmin, xmax, ymin, ymax, x, y);
                    break;
                    

                case Katrin_Salhab:
                    applyKatrinEffect(b, xmin, xmax, ymin, ymax, x, y);
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

                return 1;
            }
        }
    }

    return 0;
}


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


faction roundWinner(board b, faction f1, faction f2)
{
    int f1_pts = getFactionDdrsPoints(f1);
    int f2_pts = getFactionDdrsPoints(f2);

    if (f1_pts > f2_pts)
    {
        return f1;
    }
    else if (f2_pts > f1_pts)
    {
        return f2;
    }
    else
    {
        int xmin, ymin, xmax, ymax;
        getBoundingBox(b->b2D,&xmin,&ymin,&xmax,&ymax);
        card currentCard;
        int x = xmin; 
        int y = ymax;
        int boolean = 1; //boolean = we haven't found a card yet

        while ( y >= ymin && boolean)
        {
            while ( x <= xmax && boolean)
            {
                currentCard = getCard_board2D(b->b2D, x, y);
                if (currentCard != NULL)
                {
                    faction f = getFaction_board2D(b->b2D, x, y);
                    return f;
                }
                x++;
            }
            x = xmin;
            y--; 
        }
    }
    return f1; // never happens
}

int isValidPlace(board b, int x, int y) {
    int p = getPositionFromCoordinates_board2D(b->b2D,x,y);
    if (p < 0 || p >= b->b2D->sizeBoard2D)
    {
        return 0;
    }
    else if (getCard_board2D(b->b2D,x,y) != NULL)
    {
        return 0;
    }
    else if (getCard_board2D(b->b2D,x-1,y) != NULL || getCard_board2D(b->b2D,x+1,y) != NULL || getCard_board2D(b->b2D,x,y-1) != NULL || getCard_board2D(b->b2D,x,y+1) != NULL)
    {
        return 1;
    }
    else return 0;
}