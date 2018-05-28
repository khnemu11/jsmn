#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

char *readJSONFile() {
        int size;
        char*string;
      
        FILE*fp=fopen("data.json","rt");

        fseek(fp,0l,SEEK_END);
        size=ftell(fp)+1;
	rewind(fp);
        string=(char*)malloc(sizeof(char)*size);
        fread(string,1,size,fp); 
        fclose(fp);

        return string;
}

static const char* JSON_STRING;

void jsonNameList(char*jsonstr,jsmntok_t*t,int tokcount,int *nameTokIndex){
int i;
int j;
int count;

  for(i = 1,j=0,count=1;i<tokcount;i++){
    if(t[i].type == JSMN_ARRAY || t[i].type == JSMN_OBJECT)  continue;
    else if (t[i].size > 0){
      nameTokIndex[j]=i;
      count++;
        j++;
    }
  }
}

int* printObjectNameList(char*jsonstr,jsmntok_t *t,int tokcount,int* objectNameIndex){
  int i;
  int j;

  printf("*******Object List******\n");

  for(i = 1,j =0; i < tokcount; i++){
      if(t[i].size == 8){
        printf("name[%2d] : %.*s\n",j+1,t[i+2].end-t[i+2].start,jsonstr + t[i+2].start);
        if(j==0){
          objectNameIndex=(int*)malloc(sizeof(int));
          objectNameIndex[j]=i;
        }
        else{
          objectNameIndex=(int*)realloc(objectNameIndex,sizeof(objectNameIndex)+sizeof(int));
          objectNameIndex[j]=i;
        }
        j++;
      }
  }
  return objectNameIndex;
}

void printNameList(char*jsonstr,jsmntok_t *t, int *nameTokIndex){
 int i=0;

 printf("***** Name List *******\n");

 while(1){
   if(nameTokIndex[i]==0) break;
   else{
      printf("name[%2d] : %.*s\n",i+1,t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start);
      i++;
   }
 } 
}

void selectNameList(char *jsonstr, jsmntok_t *t, int *nameTokIndex){
  int index;
  int j;

  while(1){
    printf("\nSelect Name's no (exit:0) >> ");
    scanf("%d",&index);

    if(index==0){
      printf("exit\n");
      break;
    }
    else if(nameTokIndex[index-1]!=0){
      printf("name[%2d] : %.*s\n",index,t[nameTokIndex[index-1]].end-t[nameTokIndex[index-1]].start,jsonstr + t[nameTokIndex[index-1]].start);
      printf("%.*s\n",t[nameTokIndex[index-1]+1].end-t[nameTokIndex[index-1]+1].start,jsonstr + t[nameTokIndex[index-1]+1].start);
    }
    else{
      printf("input right index\n");
    }
  }
}

void selectObjectNameList(char *jsonstr, jsmntok_t *t, int *objectNameIndex){
  int index;
  int j;

  while(1){
    printf("Select Name's no (exit:0) >> ");
    scanf("%d",&index);
    
    if(index==0){
      printf("exit");
      break;
    }

    else if(index-1<=sizeof(objectNameIndex)/sizeof(int)){
      printf("name[%2d] : %.*s\n",index,t[objectNameIndex[index-1]+2].end-t[objectNameIndex[index-1]+2].start,jsonstr + t[objectNameIndex[index-1]+2].start);
      printf("%.*s\n",t[objectNameIndex[index-1]].end-t[objectNameIndex[index-1]].start,jsonstr + t[objectNameIndex[index-1]].start);
    }
    else{
      printf("input right index\n");
    }
  }
}


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int main() {
	int i;
	int r;
	int nameTokIndex[100]={0,};
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */
	int *objectNameIndex;	

  	JSON_STRING=readJSONFile();
  
	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0]));

	#ifdef DEBUG_MODE
	printf("token's count: %d\n",r);
	#endif

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an array */
	if (r < 1 || t[0].type != JSMN_ARRAY) {
		printf("Array expected\n");
		return 1;
	}

	jsonNameList(JSON_STRING,t,r,nameTokIndex);	
	objectNameIndex=printObjectNameList(JSON_STRING,t,r,objectNameIndex);
	//printNameList(JSON_STRING,t,nameTokIndex);
	//selectNameList(JSON_STRING,t, nameTokIndex);
	selectObjectNameList(JSON_STRING,t,objectNameIndex);	

	return EXIT_SUCCESS;
}
