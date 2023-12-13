#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "message_auth.h"


int verification(){
	int a = 0;
	
	FILE* file = fopen("/var/www/html/cgi-bin/connected.txt","r");
	FILE* file1 = fopen("/var/www/html/cgi-bin/logina.txt","r");
	
	char *rec = (char*)malloc(256*(sizeof(char)));
	char *rec1 = (char*)malloc(256*(sizeof(char)));
	if(file == NULL){
		printf("Location: http://gasyzoo.mg/verify.html\n\n");
		return a;
	}
	else{
		while(!feof(file)){	
			fscanf(file,"%[^\n]\n",rec);
		}
		while(!feof(file1)){	
			fscanf(file1,"%[^\n]\n",rec1);
			if(strcmp(rec,rec1)==0){
				a = 1;
				break;
			}
		}
	}
	if (a != 1){
		printf("Location: http://gasyzoo.mg/verify.html\n\n");
	}
	fclose(file);fclose(file1);
		
	return a;	
}

 int main(){
    
    char *url = (char*)malloc(sizeof(char)*60);
    char *recUser = NULL;
    int page = 1;
    int item = 10;
    int count = 0;
    int nbL = 0;
    int a = verification();
	
	if (a == 1){
    printf("Content-type: text/html\n\n");
    
	 printf(
            "<html>"
            "<head>"
                "<meta charset=\"UTF-8\">"
                "<title>LogAuth</title>"
	            "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
	            "<link rel=\"stylesheet\" href=\"http://gasyzoo.mg/styles/style.css\" />"
            "</head>"
                "<body>");
    
   
    url = getenv("QUERY_STRING");
	recUser= (char*)malloc(sizeof(char)*21);
    
    sscanf(url,"page=%d&item=%d",&page,&item);
    FILE* file = NULL;
    file = fopen( "/var/log/auth.log" , "r" );
    if(file == NULL ){
        perror("Erreur lors de l'ouverture du fichier");
    }
    nbL = compter_nbre_ligne(file);
    
    int currentPage = 1;
    int itemsPerPage = 10;
    int totalItems = nbL;
    
    int totalPages = (totalItems + itemsPerPage - 1) / itemsPerPage; // Calculez le nombre total de pages
    printf("%d\n ",totalPages);
    if(url!=NULL){
        if (sscanf(url, "page=%d&items=%d", &currentPage, &itemsPerPage) != 2){
            currentPage = 1;
            itemsPerPage = 10;
        }
    }
   
    Info * msg = NULL;
    msg = (Info*)malloc(sizeof(Info)*nbL);
    
    int startIndex = (currentPage - 1) * itemsPerPage;
    int endIndex = startIndex + itemsPerPage;
   // printf("nbl1= %d\n",nbL);
    nbL = rec_info_log(file,msg,nbL);
   // printf("totalPages= %d\n",totalPages);
        printf( //"<header>" 
                "<a href=\"/cgi-bin/log.cgi\" style =\"background-color:blue;margin-top:50px;padding:3px 10px 3px 10px;justify-content:center;align-content:center;border-radius:2%%;color:white;\">RECHERCHER</a>"
                "<h2>Les Authentifications Réussites</h2>"
               // "</header>"
                "<center>"
                "<table border = \"1\">"
	                "<tr>"
                        "<thead>"
                                "<th>Temps</th>"
	                            "<th>Session d'Ouverture/Fermeture</th>"
	                            "<th>Utilisateur</th>"
                        "</thead>"
                    "</tr>"
                
    );

    for(int i=0,j=0; i< nbL; i++,j++){
        generer_jour( msg, nbL);
        trad_mois_malagasy(msg,nbL);
        
       //if(strcmp(recUser,msg[i].user)==0 || *recUser==0){
            if((j>=startIndex && j < endIndex && j<=totalPages)){
                printf(
                    "<tr style=\"background:teal;\">"
                    "<td>%d %s, %d %s %s</td>",i,msg[i].jour,msg[i].num_jour,msg[i].mois,msg[i].heure
                );
                printf("<td>%s</td>",msg[i].session);
                printf("<td><a href=\"/cgi-bin/log.cgi?user=%s\">%s</a></td>\n",msg[i].user,msg[i].user);
                printf("</tr>");
            }			 
        //}
    }
   
   
    printf("</table></center>\n");
     if (currentPage > 1) {
        printf("<a href=\"/cgi-bin/message.cgi?page=%d&items=%d\"><button>Page précédente</button></a> ",currentPage - 1, itemsPerPage);
    }
    for(int i=0;i<totalPages;i++){
        int a= 0;
        if(((i+1)%10)!=0){
          a=(i+1)%10;
        }
        if(i<endIndex && i>=startIndex){
            int t=0;
                if(i<90  || currentPage>=(totalPages/(item))+1){
                    t=0;
                }
                else{
                    t=10;
                    t=t*(currentPage/10);
                }
            if(currentPage>(totalPages/(item))+1){
                break;
            }

            if(((i+1)%10)==0 && ((i+1)%2)==0 ){
                int b=0;
                b=(i+1)/(i+1)*10;
                if((b+t)<=(totalPages/10)+1){
                printf(
                    "<a href=\"/cgi-bin/message.cgi?page=%d&items=%d\">"
                    "<button>%d</button></a>",b+t,item,b+t);
                }
            }
                else{
                    if((a+t)<=(totalPages/10)+1){

                    printf(
                        "<a href=\"/cgi-bin/message.cgi?page=%d&items=%d\">"
                        "<button>%d</button></a>",a+t,item,a+t);
                }
            }
            
        }
    }
    if (currentPage < totalPages) {
        printf("<a href=\"/cgi-bin/message.cgi?page=%d&items=%d\"><button>Page suivante</button></a>", currentPage + 1, itemsPerPage);
    }
    printf("</body>\n");
    printf("</html>\n");
    free(msg);
    fclose(file);
	}
    return 0;
}
