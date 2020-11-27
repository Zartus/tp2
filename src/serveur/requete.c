#include "serveur.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <assert.h>

//faire traitement en fonction du code réponse
//optimisation get ouvrir 2 fois une fois pour la taille et une fois pour le contenue pas ouf non ?
//voir pour pas essayer de mettre des const
typedef struct s_httpRequestStruct
{
    void (*commande)(struct s_httpRequestStruct *);
    char *URL;
    struct reponseRequeteS *rep;
} RequeteStruct;

typedef struct reponseRequeteS
{
    char *contentType;
    char *contentLength;
    int numeroReponse;
    char *contenu;
} reponseRequete;

Requete initialisationStructure()
{
    Requete r = malloc(sizeof(RequeteStruct));
    r->commande = NULL;
    r->URL = NULL;
    r->rep = malloc(sizeof(reponseRequete));
    r->rep->contentType = NULL;
    r->rep->contentLength = NULL;
    r->rep->numeroReponse = 0; //modification here
    r->rep->contenu = NULL;
    return r;
}

/**
 * @brief Permet d'extraire la commande d'une requete HTTP
 * 
 * @param requete l la chaine de caractere
 * @param r la structure qui sauvegardera notre requete
 * @return int 1 est bon autre pas bon
 */
int extractCommande(char *requete, Requete r)
{
    char possibilite[256] = "";
    int err = sscanf(requete, "%s /", possibilite);
    if (err == 1)
    {
        /*on va chercher la bonne commande*/
        /*pour ajouter commande c'est ici*/
        if (!strcmp(possibilite, "GET"))
        {
            r->commande = commandeGet;
        }
        else
        {
            err = 0;
        }
    }
    return err;
}

int extraitFichier(char *requete, Requete r)
{
    char com[256] = "";
    char prev[256] = "";
    int err = sscanf(requete, "%s /%s HTTP/", prev, com);
    //que faire en cas de juste /
    r->URL = malloc(sizeof(char) * (strlen(com) + 1));
    strcpy(r->URL, com);
    
    return err;
}

//mettre en place des define pour verifier le protocole plus simple
//faire un truc avec le numero du protocle ?
int verifProtocol(char *requete)
{
    char rep[256] = ""; //modification here
    char aze[256] = "";
    char aze2[256] = "";
    char all[256] = "";
    int err = 0;
    err = sscanf(requete, "%s %s HTTP/%s %[^\n]", aze, aze2, rep, all);
    //modification here
    if (err == 3 && (!strcmp(rep, "0.9") || !strcmp(rep, "1.0") || !strcmp(rep, "1.1") || !strcmp(rep, "2.0")))
    {
        return 1;
    }

    return 0;
}

//possiblement mettre en place une grande initilisaiton pour rendre le code plus propre
//mettre en place des defines
Requete annalyseRequete(char *requete)
{
    /*Allocation de la mémoire dans la heap*/
    Requete r = initialisationStructure();

    if (!verifProtocol(requete))
    {
        fprintf(stderr, "400 Bad Request\n");//modification here
        r->rep->numeroReponse=400;
        free(r);
        return NULL;
    }
    /*extraction de la commande*/
    if (extractCommande(requete, r) != 1)
    {
        fprintf(stderr, "400 Bad Request\n");//modification here
        r->rep->numeroReponse=400;
        free(r);
        return NULL;
    }

    if (extraitFichier(requete, r) != 2)
    {
        fprintf(stderr, "404 not found\n");//modification here
        //possiblement enlever ca d'ici
        r->rep->numeroReponse=404;
        free(r);
        return NULL;
    }

    return r;
}
//renvoie 1 si l'envoie c'est bien passé sinon 0 à voir la gestion d'erreur qu'on peut
//mettre en place
int repondre(Requete r, OperateFunctor envoyer)
{
    //verifier que r existe assert + verif ou que assert
    //assert();
    int err=0;
    if (r->commande != NULL)
    {
        r->commande(r);
    }

    err=envoyerReponse(r, envoyer);
    //freeRequete(r);
    return err;
}

size_t longeurFichier(Requete r)
{
    FILE *file;
    int size = 0;
    if ((file = fopen(r->URL, "r")) == NULL)
    {
        //ici mettre en place erreur 404
        perror("pas ouvert");
        exit(3);
    }

    fseek(file, 0, SEEK_END);

    size = ftell(file);

    if (fclose(file) == EOF)
    {
        perror("probleme fermeture URL");
        exit(4);
    }

    return size;
}

//on stocke EOF?// truc à voir bizarre je trouve
char *getExtension(Requete r)
{
    char *content = NULL;
    char com[256] = "";
    char prev[256] = "";
    printf("on a ici : %s\n", r->URL);
    sscanf(r->URL, "%[^.].%s", com, prev);
    //+4 car EOF fait 4
    content = malloc(sizeof(char) * (strlen(prev) + 4));
    strcpy(content, prev);
    return content;
}

void fermetureURL(FILE *f)
{
    if (fclose(f) == EOF)
    {
        perror("Probleme à la fermeture du URL");
        exit(-3);
    }
}

char *envoyerContenuURL(Requete r)
{
    //peut etre truc à revoir ici +1 +4 ??
    char *content = malloc(sizeof(char) * (longeurFichier(r)+1+4));
    FILE *fichier;
    char ch;
    int i = 0;
    //Ouverture du URL à copier et affichage d'une erreur si impossible
    if ((fichier = fopen(r->URL, "rt")) == NULL)
    {
        perror("Probleme à l'ouverture de l'URL : ");
        exit(-1);
    }
    
    while ((ch = fgetc(fichier)) != EOF)
    {
        content[i] = ch;
        ++i;
    }

    fermetureURL(fichier);
    content[i+1]='\0';

    return content;
}

int envoyerReponse200HTML(Requete r,OperateFunctor envoyer){
    char envoie[256];//modifier here
    sprintf(envoie, "HTTP/1.1 200 OK\nContent-Length: %s\nContent-Type: text/%s\n%s",r->rep->contentLength,r->rep->contentType,r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse400(Requete r,OperateFunctor envoyer){
    char envoie[256];//modifier here
    sprintf(envoie, "HTTP/1.1 200 OK\nContent-Length: %s\nContent-Type: text/%s\n%s",r->rep->contentLength,r->rep->contentType,r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse404(Requete r,OperateFunctor envoyer){
    (void)r;
    (void)envoyer;
    return 1;
}

int envoyerReponse500(Requete r,OperateFunctor envoyer){
    (void)r;
    (void)envoyer;
    return 1;
}

int envoyerReponse(Requete r, OperateFunctor envoyer)
{
    if(r->rep->numeroReponse==200 && !strcmp(r->rep->contentType,"html")){
        return envoyerReponse200HTML(r,envoyer);
    }
    
    return 1;
}

void freeRep(reponseRequete *rep)
{
    free(rep->contentLength);
    free(rep->contentType);
    free(rep->contenu);
    free(rep);
}

void freeRequete(Requete sRequest)
{
    free(sRequest->URL);
    freeRep(sRequest->rep);
    free(sRequest);
}

void commandeGet(Requete r)
{
    r->rep->contentType = getExtension(r);

    char *envoie = malloc(sizeof(char) * 50); //modication here
    sprintf(envoie, "%ld",longeurFichier(r));

    r->rep->contentLength = envoie;
    r->rep->contenu = envoyerContenuURL(r);
    r->rep->numeroReponse = 200;
}