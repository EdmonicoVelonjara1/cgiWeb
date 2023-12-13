#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

int count(FILE *file){
	int i =0;
	char c;
	while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            i++;
        }
    }
     fseek(file, 0, SEEK_SET);
	return i;
}


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
	int a = verification();
	if (a == 1){
	printf("Content-type:text/html\n\n");
	
	printf(
	"<html><head><title>ACCESS LOG</title><style>"
	"body{width:99%%;text-align:center;justify-content:center;height:auto;}"
	"table{width:100%%;border:2px solid rgb(91, 127, 204);}"
	"tbody{color: black;	/*background-color: rgb(19, 155, 30);*/}"
	"thead{width:max-content;color:rgb(224, 223, 241);border:1px solid white;background-color: green;}"
	"tr{text-align:center;border:2px solid rgb(91, 127, 204);}"
	"tr th{background:yellowgreen;font-size:110%%;}"
	"tr:hover td{background:yellowgreen;font-size:110%%;}"
	"td{width:max-content;text-align:center;border:2px solid rgb(91, 127, 204);border-collapse:collapse; text-align:center;}"
	"@media screen and (max-width:900px){tbody,thead{background-color:green;font-size:80%%;}"
	"td{width:max-content;padding:0%% 4%% 0 1%%;text-align:center;border:collapse; text-align:center;}"
	
	"thead,th{padding:0%% 2%% 0%% 2%%;}}	a{color:rgb(18, 11, 41);text-decoration:none;}"
	"a:hover{color:red;}header{top:0;left:0;background:lightgrey;color:black;position:fixed;width: 100vw;padding: 20px;margin-bottom: 40px;z-index: -1;}"
	"span{background-color: red;color:green;}h2{margin-top:100px;}@media screen and (max-width:600px){tbody,thead{font-size:65%%;}"
	"td{padding:1%% 3%% 1%% 3%%}"
	"thead,th{width: 40%%;padding:1%% 2%% 1%% 2%%;}body{background:-webkit-linear-gradient(95deg,blue,rgb(29, 167, 24));}}"
	"</style></head><body>");
	
	printf("<a href=\"http://gasyzoo.mg/cgi-bin/logout.cgi\">Log out</a>");
	printf("<table border><tr>"
			"<th>ad ip</th>"
			"<th>Date</th>"
			"<th>Method</th>"
			"<th>Request Url</th>"
			"<th>Version du protocole HTTP</th>"
			"<th>Referer</th>"
		);
	
	char *recover =  (char*)malloc(sizeof(char)*256);
	char *ip = (char*)malloc(sizeof(char)*50);
	char *client_id = (char*)malloc(sizeof(char)*200);
	char *user_id = (char*)malloc(sizeof(char)*200);
	char *date = (char*)malloc(sizeof(char)*300);
	char *method = (char*)malloc(sizeof(char)*500);
	char *requestUrl = (char*)malloc(sizeof(char)*100);
	char *versionHttp = (char*)malloc(sizeof(char)*100);
	char *status_answer = (char*)malloc(sizeof(char)*100);
	char *size_answer = (char*)malloc(sizeof(char)*100);
	char *referer = (char*)malloc(sizeof(char)*300);
	char *user_agent = (char*)malloc(300*sizeof(char));
	
	FILE *file = fopen("/var/log/apache2/access.log","r");
	if (file !=NULL){
		printf("Fichier ouvert avec succès\n");
	}
	else{perror("Erreur!\n");exit(1);}
	//~ int c = count(file);
	//~ rewind(file);
	
	int j = 0;
	//~ printf("%d",c);
	
	while(fgets(recover,255,file) && !feof(file)){
		
		//~ printf("<tr><td>%d</td></tr>",j);
		sscanf(recover,"%[^ ] %*[^ ] %*[^ ] %[^\"]\"%[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^ ] %[^\n]\n"
		,ip,date,method,requestUrl,versionHttp,status_answer,size_answer,referer,user_agent);
		
		 CURL *curl = curl_easy_init();
		if (curl) {
			char *decoded_url = curl_easy_unescape(curl, referer, 0, NULL);
			if (decoded_url) {
				strcpy(referer,decoded_url);
				curl_free(decoded_url);
			}
			curl_easy_cleanup(curl);
		}
		printf("<tr>"
			"<td>%d %s</td><td>%s</td>"
			"<td>%s</td><td>%s</td>"
			"<td>\"%s</td><td>%s</td></tr>"
			,j,ip,date,method,requestUrl,versionHttp,referer);
			
	j++;
	}
	
	printf("</table></html></body>");
	free(recover);free(ip);free(client_id);free(user_id);free(date);free(method);
	free(requestUrl);free(versionHttp);free(status_answer);free(size_answer);free(referer);free(user_agent);
	
	fclose(file);
	}
	return 0;
} 
