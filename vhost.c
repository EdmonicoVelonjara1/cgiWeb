#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef struct Domain{
	char domain[256];
}Domain;

typedef struct Data{
    char conf_name[20];
    char server_name[50] ;
    char server_admin[50];
    char doc_root[100];
    char first_option[10];
    char second_option[20];
    char Allow_over_ride[10];
    char script_alias[100];
    char script_option[15];
    char add_handler[30];
}Data;

void activate_site(char *rec, char* cmd);
void disactivate_site(char *rec, char* cmd, char* conf_name1);
void display_recinfo(Data data, char* error, char* acces,FILE* file2,char* dom);
void edite_vhost(FILE* file, FILE* file2, Data data, char* error, char* access,char* dir_alias, char* dir_src,char *addh1,char* addh2);
void edite_hosts(FILE* file2, Data data);

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
    Data data;
    Domain *domain1;
    
    char *rec = (char*)malloc(sizeof(char)*600);
    rec = getenv("QUERY_STRING");
   
    
    char *able1 = (char*)malloc(sizeof(char)*9);
    char *conf_name1 = (char*)malloc(sizeof(char)*40);
    char *cmd = (char*)malloc(sizeof(char)*100);
    char *dir_src = (char*)malloc(sizeof(char)*20);
    char *dir_alias = (char*)malloc(sizeof(char)*20);
    char *error=malloc(20);
    char *access = malloc(20);
    char *addh1 = malloc(20);
    char *addh2 = malloc(10);
	char *command = (char*)malloc(sizeof(char)*256);
    char *dom = (char*)malloc(sizeof(char)*256);
    
    
   /********************************************/
    
    strcpy(data.server_name," ");
    strcpy(data.server_admin," ");
    strcpy(data.doc_root," ");
    strcpy(data.Allow_over_ride," ");
    strcpy(data.script_alias," ");
    strcpy(data.script_option," ");
    strcpy(data.add_handler," ");
    strcpy(data.conf_name," ");
    strcpy(error," ");
    strcpy(access," ");
    strcpy(conf_name1," ");
    strcpy(able1, " ");

    /*****************************************************/
    
    CURL *curl = curl_easy_init();
    if (curl) {
        char *decoded_url = curl_easy_unescape(curl, rec, 0, NULL);
        if (decoded_url) {
            strcpy(rec,decoded_url);
            curl_free(decoded_url);
        }
        curl_easy_cleanup(curl);
    }

    /******************************************************/

    sscanf(rec,
        "server_name=%[^&]&server_admin=%[^&]&doc_root=%[^&]&error=%[^&]&access=%[^&]&alias=%[^&]&options=%[^&]&handler=%[^&]&%*[\n]\n",
        data.server_name, data.server_admin, data.doc_root,error,access,data.script_alias, data.script_option, data.add_handler
     );

     sscanf(data.server_name,"%*[^.].%[^.].%*[^\n]\n",data.conf_name);
     strcpy(conf_name1,data.conf_name);
     sprintf(cmd,"sudo a2ensite %s",conf_name1);
     strcat(data.conf_name,".conf");
     sscanf(data.script_alias,"%[^+]+%[^\n]",dir_alias,dir_src);
     sscanf(data.add_handler,"%[^+]+%[^\n]\n",addh1,addh2);

    /**********************************************************************/
        
        FILE *file = fopen(data.conf_name,"w");
        FILE *file2 = fopen("/etc/hosts","a+");
		char * cat = "cat /etc/hosts";
		popen(cat, "r");
        
         if (file != NULL){printf("Fichier ouvert avec succès\n");}
        else{perror("<p> Erreur lors de l'ouverture du fichier !!!<p>");}
        
        if (file2 != NULL){printf("Fichier ouvert avec succès\n");}
        else{perror("<p> Erreur lors de l'ouverture du fichier !!!");}
   
        edite_vhost( file, file2, data, error, access,dir_alias,dir_src,addh1,addh2);

    /********************************************************************/
    
   
    char *destination = "/etc/apache2/sites-available/";
    sprintf(command," mv \"/var/www/html/cgi-bin/%s\" \"%s\"", data.conf_name, destination);
    int result = system(command);

/*********************************************************************/

	display_recinfo(data,error,access,file2,dom);
    activate_site(rec,cmd);
	disactivate_site(rec,cmd,conf_name1);
}
/*********************************************************************/

    return 0;
}

void activate_site(char *rec, char* cmd){
    if (strstr(rec,"a2ensite")!=NULL){
    strcat(cmd," && systemctl reload apache2");   
        printf("<h1>ATO ZAHO 1</h1>");
        popen(cmd,"r");
    }
}

void disactivate_site(char *rec, char* cmd,char* conf_name1){
    if (strstr(rec,"a2dissite")!=NULL){
        sprintf(cmd,"sudo a2dissite %s && systemctl reload apache2",conf_name1);   
        popen(cmd,"r");
    }
}

void display_recinfo(Data data, char* error, char* access,FILE* file2,char* dom){
	printf("dom = %s\t",dom);
	char *d = strchr(dom,'\t');
	printf( "<html lang=\"en\">"
            "<head>"
                "<meta charset=\"UTF-8\">"
                "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
                "<title>Créer Virtualhost</title>"
            "</head>"
            "<body>" 
            "%s\t"
            "<button><a href=\"/cgi-bin/vhost.cgi?server_name=%s&server_admin=%s&doc_root=%s&error=%s&access=%s&alias=%s&options=%s&handler=%s&submit=submit&able=a2ensite\">Activer</a></button>"
            "<button><a href=\"/cgi-bin/vhost.cgi?server_name=%s&server_admin=%s&doc_root=%s&error=%s&access=%s&alias=%s&options=%s&handler=%s&submit=submit&able=a2dissite\">Désactiver</a></button>"
        ,d,data.server_name, data.server_admin, data.doc_root ,error,access, data.script_alias, data.script_option,data.add_handler
        ,data.server_name, data.server_admin, data.doc_root ,error,access, data.script_alias, data.script_option,data.add_handler);
	edite_hosts(file2,data);	
}

void edite_vhost(FILE* file, FILE* file2, Data data, char* error, char* access,char* dir_alias, char* dir_src,char *addh1,char* addh2){
	fprintf(file,"<Virtualhost *:80>\n");
    fprintf(file,"\tServerName %s\n",data.server_name);
    fprintf(file,"\tServerAdmin %s\n",data.server_admin);
    fprintf(file,"\tDocumentRoot %s\n\n",data.doc_root);
    fprintf(file,"\tErrorLog /var/log/apache2/%s\n",error);
    fprintf(file,"\tCustomLog /var/log/apache2/%s combined\n\n",access);
    fprintf(file,
        "\t<Directory %s>\n"
        "\t\tOptions +Indexes +FollowSymLinks\n"
        "\t\tAllowOverride All\n"
        "\t\tRequire all granted\n"
        "\t</Directory>\n\n"
     ,data.doc_root);
       
     if (data.script_alias != NULL && (strcmp(data.script_alias," ")!=0) && (strcmp(data.script_alias,"\0")!=0)){
		fprintf(file,"\tScriptAlias %s %s\n",dir_alias,dir_src);
		fprintf(file,
			"\t<Directory %s>\n"
			"\t\tOptions +%s\n"
			"\t\tAddHandler %s %s\n"
			"\t\tRequire all granted\n"
			"\t</Directory>\n"
		,dir_src,data.script_option,addh1,addh2);
     }
     fprintf(file,"</Virtualhost>\n");
     
}

void edite_hosts(FILE* file2, Data data){
    if (file2 != NULL) {
        char line[256];
        int exists = 0;

        // Cherche si l'entrée existe déjà dans le fichier
        while (fgets(line, sizeof(line), file2)) {
            if (strstr(line, data.server_name) != NULL) {
                exists = 1;
                break;
            }
        }

        // Si l'entrée n'existe pas, on l'ajoute
        if (!exists) {
            fseek(file2, 0, SEEK_END);  // Se déplace à la fin du fichier
            fprintf(file2, "192.168.1.107\t%s\n", data.server_name);
        }
    } else {
        printf("Erreur lors de l'ouverture du fichier /etc/hosts");
    }
}
