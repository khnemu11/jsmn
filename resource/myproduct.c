#include "../jsmn.h"
#include "../myproduct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *JSON_STRING;
static const char filename[64];

void printProduct(product_t *p[], int pcount){
  int i;

  printf("*****************************************************************\n");
  printf("번호\t제품명\t\t제조사\t\t가격\t개수\t총가격\n");
  printf("*****************************************************************\n");

  for(i=0;i<pcount;i++){
      printf("%2d",i+1);
      printf("\t%s",p[i]->name);
      
      if(p[i]->company==NOBRAND) printf("\t%s","노브랜드");
      else if(p[i]->company==ORION) printf("\t\t%s","오리온\t"); 
      else if(p[i]->company==NONGSIM) printf("\t\t%s","농심\t"); 
      else if(p[i]->company==HAETAE)  printf("\t%s","해태\t");
      else printf("\t%s","UNKOWN");
      
      printf("\t%d",p[i]->price);
      printf("\t%d",p[i]->count);
      printf("\t%d\n",p[i]->price*p[i]->count);
  }
}

int makeProduct(const char*jsonstr, jsmntok_t *t, int tokcount, product_t *p[]){
  int i=0;
	int j;
  int k;
  int parent=-1;
  int objectcount=0;
  char count[10];
  char price[10];
  char company[20];

  for(i=0;i<tokcount;i++){
    if(t[i+1].type== JSMN_STRING && t[i+2].type == JSMN_ARRAY){
      parent++;
      continue;
    }
    if(t[i+1].type== JSMN_ARRAY || t[i+1].type == JSMN_OBJECT)  parent++;
    else if(t[i+1].type!=JSMN_ARRAY || t[i+1].type != JSMN_OBJECT)  break;
  }

  for (i = 0, j = 0; i < tokcount; i++) {
		if(t[i].parent == parent) objectcount++;
	}

	for (i = 1, j = 0; i < tokcount; i++) {
		if(t[i].parent == parent){
      p[j]=(product_t *)malloc(sizeof(product_t));
      
      sprintf(company,"%.*s",t[i+2].end - t[i+2].start, jsonstr + t[i+2].start);
      sprintf(p[j]->name,"%.*s",t[i+4].end - t[i+4].start, jsonstr + t[i+4].start);
      sprintf(price,"%.*s",t[i+6].end - t[i+6].start, jsonstr + t[i+6].start);
      sprintf(count,"%.*s",t[i+8].end - t[i+8].start, jsonstr + t[i+8].start);

      p[j]->price=atoi(price);
      p[j]->count=atoi(count);

      if(strcmp(company,"노브랜드")==0) p[j]->company = NOBRAND;
      else if(strcmp(company,"오리온")==0)  p[j]->company = ORION;
      else if(strcmp(company,"농심")==0)  p[j]->company = NONGSIM;
      else if(strcmp(company,"해태")==0)  p[j]->company = HAETAE;

		  j++;
    }
	} 

  return objectcount;
}

char *readJSONFile() {
	int size;
	char *string;

	FILE *fp = fopen("myproduct.json", "r");
   if (!fp){
      perror("파일이 존재하지 않습니다.");
      exit(1);
  }
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
  int pcount;
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
  product_t *snacklist[20];
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
  
  pcount=makeProduct(JSON_STRING,t,r,snacklist);
  printProduct(snacklist,pcount);

	return EXIT_SUCCESS;
}
