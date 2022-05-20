#ifndef _JSONPARSER_H  
#define _JSONPARSER_H

typedef struct jsonParser_base *jsParser;

jsParser initParser(char *filePath);

int getCardId_json(jsParser jsP, int index);
char * getCardName_json(jsParser jsP, int index);
int getCardOccurences_json(jsParser jsP, int index);
char * getCardDescription_json(jsParser jsP, int index);
int getSize_json(jsParser jsP);

void freeParser(jsParser jsP);

#endif