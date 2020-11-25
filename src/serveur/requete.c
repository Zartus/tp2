#include "serveur.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct s_httpRequestStruct
{
    char *commande;
    float protocoleVersion;
    char *fichier;
    //char *corps;
} RequeteStruct;

//ne pas utiliser un tableau de caractere mais un int pour reduire la place en mémoire
//GET /index.php HTTP/1.1
int commandeR(char *requete,Requete r){
    char com[256]="";
    int err=0;
    err=sscanf(requete,"%s /",com);
    r->commande=malloc(sizeof(char)*(strlen(com)+1));
    strcpy(r->commande,com);
    return err;
}

int getFichier(char *requete,Requete r){
    char com[256]="";
    char prev[256]="";
    int err=0;
    err=sscanf(requete,"%s /%s HTTP",prev,com);
    printf("valeur de com ==%s\n",com);
    r->fichier=malloc(sizeof(char)*(strlen(com)+1));
    strcpy(r->fichier,com);
    return err;
}

int getProtocol(char *requete,Requete r){
    char com[256]="";
    int err=0;
    err=sscanf(requete,"HTTP/%s ",com);
    printf("valeur de com ==%s\n",com);
    r->commande=malloc(sizeof(char)*(strlen(com)+1));
    strcpy(r->commande,com);
    return err;
}

Requete typeRequete(char *requete)
{
    Requete r = malloc(sizeof(RequeteStruct));
    if(commandeR(requete,r)!=1){
        fprintf(stderr,"Probleme sur la command envoyer\n");
        free(r);
        exit(1);
    }

    if(getFichier(requete,r)!=2){
        fprintf(stderr,"Probleme sur le fichier envoyer\n");
        free(r);
        exit(2);
    }

    return r;
}

void affichage(Requete r){
    printf("%s %s\n",r->commande,r->fichier);
}


void freeRequete(Requete sRequest){
    free(sRequest->fichier);
    free(sRequest->commande);
    free(sRequest);
}

size_t longeurFichier(Requete r){
    FILE* file;
    int size = 0; 
    if((file=fopen(r->fichier,"r"))==NULL){
        perror("pas ouvert");
        exit(3);
    }
    
    fseek(file,0, SEEK_END);
    
    size = ftell(file);
    
    if(fclose(file)==EOF){
        perror("probleme fermeture fichier");
        exit(4);
    }
    return size;
}

