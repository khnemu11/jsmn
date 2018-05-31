#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

typedef struct _token{
  char* name;
  int count;
} Token;

char *readFile() {
  int size;
  char*string;
      
  FILE*fp=fopen("a.txt","rt");
  fseek(fp,0l,SEEK_END);
  size=ftell(fp)+1;
  rewind(fp);
  string=(char*)malloc(sizeof(char)*size);    
  fread(string,1,size,fp);
  fclose(fp);

  return string;
}

int main(void){ 
  Token *token;
  char *str;
  char *cpstr;
  int *p;
  int i=0;
  int j=0;
  int tokcount=0;

  str=readFile();
  printf("%s",str);
  cpstr=(char*)malloc(sizeof(str));
  strcpy(cpstr,str);
  
  p=strtok(cpstr," \n.");
   while(p!=NULL){

    p=strtok(NULL," \n.");
    tokcount++;
  }

  printf("%d\n",tokcount);
   token=(Token*)malloc(sizeof(Token)*tokcount);

  for(i=0;i<strlen(str);i++){
    str[i] =toupper(str[i]);
  }

  p=strtok(str," \n.");

  while(p!=NULL){
    for(i=0;i<j;i++){
      if(strcmp(token[i].name,p)==0){
        token[i].count++;
      }
    }
    
    printf("%s\n",p);
    token[j].name =p;
    token[j].count=1;
    p=strtok(NULL," \n.");
    j++;
  }

  for(i=0;i<tokcount;i++){
    printf("%s\n",token[i].name);
    printf("%d\n",token[i].count);
  }
  
  free(str);
  free(cpstr);
  free(token);
 }
