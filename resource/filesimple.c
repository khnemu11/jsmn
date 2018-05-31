#include "../jsmn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *JSON_STRING;

void selectObjectNameList(char *jsonstr,jsmntok_t *t,int *objectNameIndex) {
	int index;
	int j;

	while (1) {
		printf("Select Name's no (exit:0) >> ");
		scanf("%d", &index);

		if (index == 0) {
			printf("exit");
			break;
		}


		else {
			printf("%.*s\n",t[objectNameIndex[index - 1]].end -t[objectNameIndex[index - 1]].start,jsonstr + t[objectNameIndex[index - 1]].start);
		} 
	}
}

int* jsonObjectList(char*jsonstr, jsmntok_t *t, int tokcount){
  int i=0;
  int j;
  int parent=-1;
  int*objectNameIndex;
  int*temp;

  for(i=0;i<tokcount;i++){
    if(t[i+1].type== JSMN_ARRAY || t[i+1].type == JSMN_OBJECT)  parent++;
    else if(t[i+1].type!=JSMN_ARRAY || t[i+1].type != JSMN_OBJECT)  break;
  }

  printf("******* Object List ******\n");

	for (i = 0, j = 0; i < tokcount; i++) {
		if(t[i].parent == parent){
      printf("name[%2d] : %.*s\n",j + 1, t[i+2].end - t[i+2].start, jsonstr + t[i+2].start);
      if (j == 0) {
				objectNameIndex = (int *)malloc(sizeof(int));
				objectNameIndex[j] = i;
        j++;
      } 
      else {
				objectNameIndex = (int *)realloc(objectNameIndex, sizeof(objectNameIndex) + sizeof(int));
				objectNameIndex[j] = i;
        j++;
      }
		}
	}

  temp=objectNameIndex;

  return temp;
}

void jsonNametList(char*jsonstr, jsmntok_t *t, int tokcount){
  int i=0;
  int j;
  int parent=-1;

  for(i=0;i<tokcount;i++){
    if(t[i+1].type== JSMN_ARRAY || t[i+1].type == JSMN_OBJECT){
      //printf("parent: %d\n",parent);
      parent++;
    }
    else if(t[i+1].type!=JSMN_ARRAY || t[i+1].type != JSMN_OBJECT)  break;
  }
  //printf("parent : %d",parent);

  printf("******* Name List ******\n");

	for (i = 1, j = 0; i < tokcount; i++) {
		if(t[t[i].parent].parent == parent){
      printf("name[%2d] : %.*s\n",j + 1, t[i].end - t[i].start, jsonstr + t[i].start);
			j++;
		}
	}
  printf("\n\n");
}

char *readJSONFile() {
	int size;
	char *string;

	FILE *fp = fopen("data3.json", "rt");
	fseek(fp, 0l, SEEK_END);
	size = ftell(fp) + 1;
	rewind(fp);
	string = (char *)malloc(sizeof(char) * size);
	fread(string, 1, size, fp);
	fclose(fp);

	return string;
}

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
		strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
  int *NameIndex;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	JSON_STRING = readJSONFile();

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t) / sizeof(t[0]));

#ifdef DEBUG_MODE
	printf("token's count: %d\n", r);
#endif

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}
  
  jsonNametList(JSON_STRING,t,r);
  NameIndex=jsonObjectList(JSON_STRING,t,r);
  selectObjectNameList(JSON_STRING,t,NameIndex);
  
	return EXIT_SUCCESS;
}
