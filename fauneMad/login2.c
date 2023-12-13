#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char *info;
    char *pseudo = NULL;
    char *password = NULL;
    char *confirm = NULL;
    
    pseudo = (char*)malloc(sizeof(char)*60);
    password = (char*)malloc(sizeof(char)*60);
    confirm = (char*)malloc(sizeof(char)*60);
    
    
    info = getenv("QUERY_STRING");
    sscanf(info,"identifiant=%[^&]&motdepasse=%[^&]&confpasswd=%[^&]&%*[^\n]\n",pseudo,password,confirm);

    FILE *pt1 = NULL;
    pt1 = fopen("logina.txt","a+");
	FILE *pt2 = fopen("connected.txt","w");
  
    
    if(strstr(info,"inscription")!=NULL){
		char tmp[256];
        char tmp1[60];
        char tmp2[60];
        strcpy(tmp , " ");
        while(!feof(pt1)){
			fscanf(pt1,"%s",tmp);
			
			if(strcmp(tmp," ")==0){
				
				fprintf(pt1,"%s:%s\n",pseudo,password);
				fprintf(pt2,"%s:%s\n",pseudo,password);
				
				printf("Location: http://gasyzoo.mg/cgi-bin/menu.cgi\n\n");
				break;
			}
			else{
				if(strstr(tmp,password)!=NULL && strstr(tmp,pseudo)!=NULL){
					printf("Location: http://gasyzoo.mg/index.html\n\n");
					break;
				}
				
				if(strstr(tmp,password)==NULL && strstr(tmp,pseudo)==NULL){
					fprintf(pt1,"%s:%s\n",pseudo,password);
					fprintf(pt2,"%s:%s\n",pseudo,password);
					printf("Location: http://gasyzoo.mg/cgi-bin/menu.cgi\n\n");
					break;
				}
			}
		}
		
		
    }
   
    rewind(pt1);
    
	
    
    if(strstr(info,"connection")){
        char tmp[256];
        char tmp1[60];
        char tmp2[60];
        
        while(!feof(pt1)){
            fgets(tmp,255,pt1);
            sscanf(tmp,"%[^:]:%[^\n]\n",tmp1,tmp2);
			fprintf(pt2,"%s:%s\n",pseudo,password);
            if(strcmp(tmp1,pseudo)!=0){
				continue;
			}
            if(strcmp(tmp1,pseudo)==0){
				if(strcmp(tmp2,password)==0){
					printf("Location: http://gasyzoo.mg/cgi-bin/menu.cgi\n\n");
				
				}
				if((strcmp(tmp2,password)!=0)){
					printf("Location: http://gasyzoo.mg/passwd.html\n\n");
					break;
				}
            }
            if(strcmp(tmp1,pseudo)!=0){	
				printf("Location: http://gasyzoo.mg/iderror.html\n\n");
				break;
            }

            if(strcmp(tmp1,pseudo)!=0 && strcmp(tmp2,password)!=0){
                printf("Location: http://gasyzoo.mg/verify.html\n\n");
				break;
            }
        }
    }
    fclose(pt1);
    return 0;
}
