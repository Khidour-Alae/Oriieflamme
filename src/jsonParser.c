#include "../headers/jsonParser.h"
#include <stdio.h>
#include <json-c/json.h>

#define BUF_SIZE 20000 //needs to be the >= size of the json you want to read
#define MAX_NAME_SIZE 100
#define MAX_DESCRIPTION_SIZE 2000

struct jsonParser_base {
    int size;
    int *ids; // the id's of the cards ids[0] returns the id of the first card in the json
    char **names; //names[0] returns the char array containing the name of the first card in the json
    int *nb_occurences; //nb_occurences[0] returns the nb_occurences of the first card read in the json
    char **descriptions; //description[0] returns the char array containing the description of the first card in the json
};

jsParser initParser(char *filePath) {
    //reading json file
    FILE *fp;
    char buf[BUF_SIZE];

	fp = fopen(filePath,"r");
	fread(buf, BUF_SIZE, 1, fp);
	fclose(fp);

    //preparing json parsing
    struct json_object *parsed_json;
    struct json_object *card;
    struct json_object *id;
    struct json_object *name;
	struct json_object *nb_occurences;
	struct json_object *description;
    size_t n_cards;

	size_t i;

	parsed_json = json_tokener_parse(buf);
    n_cards = json_object_array_length(parsed_json);
    
    //mem allocation
    jsParser jsP;
    jsP = malloc(sizeof(struct jsonParser_base));
    jsP->size = (int) n_cards;
    jsP->ids = malloc(sizeof(int)*n_cards);
    jsP->names = malloc(sizeof(char *)*n_cards);
    for (i = 0; i < n_cards; i++)
    {
        jsP->names[i] = malloc(sizeof(char)*MAX_NAME_SIZE);
    }
    jsP->nb_occurences = malloc(sizeof(int)*n_cards);
    jsP->descriptions = malloc(sizeof(char *)*n_cards);
    for (i = 0; i < n_cards; i++)
    {
        jsP->descriptions[i] = malloc(sizeof(char)*MAX_DESCRIPTION_SIZE);
    }

    //storing data in the jsonParser structure
    for (i = 0; i < n_cards; i++)
    {
        card = json_object_array_get_idx(parsed_json, i);
	    json_object_object_get_ex(card, "id", &id);
        json_object_object_get_ex(card, "name", &name);
	    json_object_object_get_ex(card, "nb_occurences", &nb_occurences);
	    json_object_object_get_ex(card, "description", &description);

        jsP->ids[i] = json_object_get_int(id);

        const char *buf;
        buf = json_object_get_string(name);
        int k = 0;
        while ((k < MAX_NAME_SIZE) && (buf[k] != '\0'))
        {
            jsP->names[i][k] = buf[k];
            k++;
        }
        jsP->names[i][k] = '\0';

        jsP->nb_occurences[i] = json_object_get_int(nb_occurences);

        buf = json_object_get_string(description);
        k = 0;
        while ((k < MAX_DESCRIPTION_SIZE) && (buf[k] != '\0'))
        {
            jsP->descriptions[i][k] = buf[k];
            k++;
        }
        jsP->descriptions[i][k] = '\0';
    }

    return jsP;
}

int getCardId_json(jsParser jsP, int index) {
    return jsP->ids[index];
}
char * getCardName_json(jsParser jsP, int index) {
    return jsP->names[index];
}
int getCardOccurences_json(jsParser jsP, int index) {
    return jsP->nb_occurences[index];
}
char * getCardDescription_json(jsParser jsP, int index) {
    return jsP->descriptions[index];
}
int getSize_json(jsParser jsP) {
    return jsP->size;
}

void freeParser(jsParser jsP) {
    free(jsP->ids);
    free(jsP->nb_occurences);
    for (int i = 0; i < jsP->size; i++)
    {
        free(jsP->names[i]);
        free(jsP->descriptions[i]);
    }
    free(jsP->names);
    free(jsP->descriptions);
}