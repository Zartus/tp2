#include "serveur.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct s_httpRequestStruct
{
    char *commande;
    float protocoleVersion;
    char *fichier;
    
} RequeteStruct;

typedef struct reponseRequeteS
{
    char *contentType;
    char *contentLength;
    int numeroReponse;
    char *contenu;
} reponseRequete;

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

char *getExtension(Requete r){
    char *content=NULL;
    char com[256]="";
    char prev[256]="";
    printf("on a ici : %s\n",r->fichier);
    sscanf(r->fichier,"%[^.].%s",com,prev);
    content=malloc(sizeof(char)*strlen(prev)+1);
    strcpy(content,prev);
    strcat(content,"\n");
    return content;
}

void fermetureFichier(FILE *f){
    if(fclose(f)==EOF){
        perror("Probleme à la fermeture du fichier");
        exit(-3);
    }
}

char *envoyerContenuFichier(Requete r){
    char *content=malloc(sizeof(char)*(longeurFichier(r)+1));
    FILE *f_in;
    char ch;
    int i=0;
    //Ouverture du fichier à copier et affichage d'une erreur si impossible
    if ((f_in = fopen(r->fichier,"rt")) == NULL)
    {
        perror("Probleme à l'ouverture du fichier : ");
        exit(-1);
    }
    while((ch = fgetc(f_in))!=EOF){
        content[i]=ch;
        ++i;
    }

    fermetureFichier(f_in);
    content[i]='\n';
    return content;
}

Reponse rep(Requete r){
    Reponse reponse = malloc(sizeof(reponseRequete));
    reponse->contentType=getExtension(r);
    char *envoie = malloc(sizeof(char)*50); /*modification ici*/
    sprintf(envoie,"%ld",longeurFichier(r));
    strcat(envoie,"\n");
    reponse->contentLength=envoie;
    reponse->contenu=envoyerContenuFichier(r);
    reponse->numeroReponse=200;
    return reponse;
}

void envoyerReponse(Reponse rep,OperateFunctor where){
    where(rep->contenu);
    where(rep->contentType);
    where(rep->contentLength);
}

void freeRep(Reponse rep){
    free(rep->contentLength);
    free(rep->contentType);
    free(rep->contenu);
    free(rep);
}

void libeHeap(){
    
}