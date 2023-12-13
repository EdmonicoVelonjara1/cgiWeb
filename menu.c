#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	fclose(file);fclose(file1);
		
	return a;	
}

int main(){
	int a = 0;
	
	a = verification();
	
	if (a==1){
		printf("Content-type:text/html\n\n");
		printf("<html lang=\"en\">"
				"<head>"
				"<meta charset=\"UTF-8\">"
				"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
				"<title>Menu</title>"
				"<style>"
					"header{margin-top:15%%;}"
					"body{background:blue;}"
					"h1,h2{text-align: center;}"
					"a{text-decoration:none;color:black;}"
					
					"div{background-color:chocolate; padding: 2px 10px 2px 10px; margin: 70px 20px 10px 20px;display:inline;}"
				"</style>"
				"</head>"
				"<body>"
				"<div><a href=\"/cgi-bin/logout.cgi\">Log out</a></div>"
				"<header><h1>Bienvenue dans notre Site Web</h1></header>"
				"<h2>Veuillez cliquer les liens pour voir les contenues des pages</h2>"
				"<center><div><a href=\"http://gasyzoo.mg/acc.html\">La faune de Madagascar</a></div>"
				
				"<div><a href=\"/cgi-bin/message.cgi\">Message d'authentification du Serveur</a></div>"
				"<div><a href=\"http://gasyzoo.mg/create_vhost.html\">Hébergez votre site Web </a></div></center>"
				"<div><a href=\"http://gasyzoo.mg/cgi-bin/access.cgi\">Access Log</a></div></center>"
				
				"</body>"
				"</html>"
		);
	}
	
	
    return 0;
}
