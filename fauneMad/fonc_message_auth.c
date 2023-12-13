#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message_auth.h"

const char mois_annee[12][15] = {   "Janoary",
                                    "Febroay",
                                    "Martsa",
                                    "Aprily",
                                    "Mey",
                                    "Jona",
                                    "Jolay",
                                    "Aogositra",
                                    "Septambra",
                                    "Oktobra",
                                    "Novambra",
                                    "Desambra"
                                };
const char jours_semaine[7][15] = { "Alatsinainy",
                                    "Talata",
                                    "Alarobia",
                                    "Alakamisy",
                                    "Zoma" ,
									"Asabotsy", 
                                    "Alahady"
                                  };
								  
const int fin_mois[12]={31,28,31,30,31,30,31,31,30,31,30,31};
const int sept_premiers_jours_an[7] = {0,1,2,3,4,5,6};
const char mois_abr[12][6]={"Jan","Feb","Mar","Apr","Mey","jun","Jul","Aug","Sep","Oct","Nov","Dec"};

int compter_nbre_ligne(FILE *f){
	int i=0;
	char mois[20];
    char jour[20];
    char session[15];
    char user[20];
    char heure[20];
    int num_jour =0;
	char *s = (char*)malloc(sizeof(char)*256);

	while(!feof(f)){
		fgets(s,255,f);
        if((strstr(s,"opened")!=NULL )|| ( strstr(s, "closed" ) != NULL)){
			 sscanf(s,"%[^ ] %d %[^ ] %*[^ ] %*[^ ] %*[^ ] %*[^ ] %[^ ] %*[^ ] %*[^ ] %[^ ] %*[^\n]\n",mois,&num_jour,heure,session,user);
			 i++;
		}
	}

	rewind(f);
	free(s);
	return i;
}

int rec_info_log(FILE* file,Info *msg, int nbL){
	int i = 0;
	char * recuperateur_ligne = NULL;
    recuperateur_ligne = (char*)malloc(sizeof(char)*256);
    while(!feof(file)){
		fgets(recuperateur_ligne,255,file);
        if((strstr(recuperateur_ligne,"opened")!=NULL ) || ( strstr(recuperateur_ligne, "closed" ) != NULL)){
			sscanf(recuperateur_ligne,"%[^ ] %d %[^ ] %*[^ ] %*[^ ] %*[^ ] %*[^ ] %[^ ] %*[^ ] %*[^ ] %[^ ] %*[^\n]\n",msg[i].mois,&msg[i].num_jour,msg[i].heure,msg[i].session,msg[i].user);
			i++;
		}
    }
	free(recuperateur_ligne);
	return i;
}

void generer_jour(Info *msg,int nbL){

 	int annee = 2023;
    int numMonth = 0;
    int rangDay = 0;
	for(int j=0;j<nbL;j++){
		for(int i=0;i<12;i++){
			if(strcasecmp(msg[j].mois,mois_abr[i])==0){
				numMonth = i+1;
				if(i<3){
					numMonth = i+12;
					strcpy(msg[j].mois,mois_abr[i+12]);
					annee -= 1;
				}
			}
		}
		int K = annee%100;
		int J = annee/100;
		rangDay = ((msg[j].num_jour+(2*numMonth) + (3*(numMonth+1))/5 + K + K/4 + J/4 - 2*J))%7;
		
		strcpy(msg[j].jour , jours_semaine[rangDay]);
	}
 }
 
void trad_mois_malagasy(Info *msg,int nbL){
		for(int j=0;j<nbL;j++){
		for(int i=0;i<12;i++){
			int comp = strcmp(msg[j].mois,*(mois_annee+i));
			if(comp==0){
				switch(i){
					case 0: strcpy(msg[j].mois,"Janoary");
					break;
					case 1: strcpy(msg[j].mois,"Febroary");
					break;
					case 2: strcpy(msg[j].mois,"Martsa");
					break;
					case 3: strcpy(msg[j].mois,"Aprily");
					break;
					case 4: strcpy(msg[j].mois,"Mey");
					break;
					case 5: strcpy(msg[j].mois,"Jona");
					break;
					case 6: strcpy(msg[j].mois,"Jolay");
					break;
					case 7: strcpy(msg[j].mois,"Aogositra");
					break;
					case 8: strcpy(msg[j].mois,"Septambra");
					break;
					case 9: strcpy(msg[j].mois,"Oktobra");
					break;
					case 10: strcpy(msg[j].mois,"Novambra");
					break;
					case 11: strcpy(msg[j].mois,"Desambra");
					break;   
				}
			}
		}
	}
}
 