#include <CUnit/CUnit.h>
#include "../src/board.c"
#include "../src/structure.c"
#include "../src/cards.c"
#include "../src/faction.c"
#include "../src/interface.c"


void testFactionCreation(void){
    CU_ASSERT_PTR_NOT_NULL(createFaction());
}

void testBoardCreation(void){
    CU_ASSERT_PTR_NOT_NULL(createBoard());
}