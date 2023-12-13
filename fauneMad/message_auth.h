#ifndef _MESSAGE_H_
#define _MESSAGE_H_

typedef struct {
    char mois[20];
    char jour[20];
    char session[15];
    char user[20];
    char heure[20];
    int num_jour ;
}Info;

int compter_nbre_ligne(FILE* f);
int rec_info_log(FILE* file,Info *msg, int nbL);
void generer_jour(Info* msg,int nbL);
void trad_mois_malagasy(Info *msg,int nbL);

#endif