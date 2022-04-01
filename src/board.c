#include "../headers/board.h"
#include "../headers/structure.h"
#include <stdio.h>
#include <stdlib.h>

struct board
{
    board2D board;
    faction listFactions[2];
};

board createBoard(){

}

void freeBoard(board b){
    delete_board2D(b->b2D);
}

int newRound(int counterRoundNumber, faction f1, faction f2){
    if (counterRoundNumber == 3) return 0;
    if (f1->nbRoundWin == 2 || f2->nbRoundWin == 2) return 0;
    return 1;
}

faction* listFactions(board b){

}

void putDownCard(board *b, card c, faction f, int p){
    addCard_board2D(b->b2D, c, f, p)
}

int flipCard(board *b, card *c){
    /* Récupérer le bounding box de b avec la fonction getBoundingBox
    Il faudra ensuite parcourir le tableau de en haut à gauche jusqu'à en bas à droite, et utiliser la fonction getCard_board2D. 
    S'il n'y a aucune carte, cette fonction revoie NULL. 
    S'il y a une carte, il faudra retourner la carte. */
}

int getCenterOfBoard(board b){
    getCenter_board2D(b->b2D);
}
