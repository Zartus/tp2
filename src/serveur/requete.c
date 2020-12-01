#include "serveur.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include <assert.h>

//\r\n ? dans la requete
//\n \n
//faire traitement en fonction du code réponse
//optimisation get ouvrir 2 fois une fois pour la taille et une fois pour le contenue pas ouf non ?
//voir pour pas essayer de mettre des const
typedef struct s_httpRequestStruct
{
    void (*commande)(struct s_httpRequestStruct *);
    char *fichier;
    struct reponseRequeteS *rep;
} RequeteStruct;

typedef struct reponseRequeteS
{
    char *contentType;
    char *contentLength;
    char *contenu;
    int (*numeroReponse)(struct s_httpRequestStruct *, OperateFunctor);
} reponseRequete;

//mettre les réponses dans un autre fichier pas ouf la
int envoyerReponse200HTML(Requete r, OperateFunctor envoyer)
{
    //modifier taille buffer
    char envoie[1024] = "";
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %s\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse400(Requete r, OperateFunctor envoyer)
{
    (void)r;
    char envoie[256] = ""; //modifier here
    sprintf(envoie, "HTTP/1.1 400 Bad Request");
    return envoyer(envoie);
}

int envoyerReponse404(Requete r, OperateFunctor envoyer)
{
    (void)r;
    char envoie[256] = ""; //modifier here
    sprintf(envoie, "HTTP/1.1 404 Serveur Error");
    return envoyer(envoie);
}

int envoyerReponse500(Requete r, OperateFunctor envoyer)
{
    (void)r;
    char envoie[256] = ""; //modifier here
    sprintf(envoie, "HTTP/1.1 500 Not Found");
    return envoyer(envoie);
}

int envoyerReponse200JPG(Requete r, OperateFunctor envoyer)
{
    (void)r;
    (void)envoyer;
    return 1;
}

int envoyerReponse200ICO(Requete r, OperateFunctor envoyer)
{
    (void)r;
    (void)envoyer;
    return 1;
}

Requete initialisationStructure()
{
    Requete r = malloc(sizeof(RequeteStruct));
    r->commande = NULL;
    r->fichier = NULL;
    r->rep = malloc(sizeof(reponseRequete));
    r->rep->contentType = NULL;
    r->rep->contentLength = NULL;
    r->rep->numeroReponse = NULL;
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
    r->fichier = malloc(sizeof(char) * (strlen(com) + 1));
    strcpy(r->fichier, com);

    return err;
}

//voir si on laisse int ici
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
        r->rep->numeroReponse = envoyerReponse400;
        //free(r);
        //renvoyer NULL ?
        return r;
    }
    /*extraction de la commande*/
    if (extractCommande(requete, r) != 1)
    {
        r->rep->numeroReponse = envoyerReponse400;
        //free(r);
        //renvoyer NULL?
        return r;
    }

    extraitFichier(requete, r);

    return r;
}

//renvoie 1 si l'envoie c'est bien passé sinon 0 à voir la gestion d'erreur qu'on peut
//mettre en place
int repondre(Requete r, OperateFunctor envoyer)
{
    //verifier que r existe assert + verif ou que assert
    //assert();
    int err = 0;
    if (r->commande != NULL)
    {
        r->commande(r);
    }

    //gestion du retour ?
    err = r->rep->numeroReponse(r, envoyer);

    //freeRequete(r);
    return err;
}

size_t longeurFichier(Requete r)
{
    FILE *file;
    int size = 0;
    if ((file = fopen(r->fichier, "r")) == NULL)
    {
        perror("pas ouvert");
        r->rep->numeroReponse = envoyerReponse404;
        return 0;
    }

    fseek(file, 0, SEEK_END);

    size = ftell(file);

    if (fclose(file) == EOF)
    {
        perror("probleme fermeture URL1");
        r->rep->numeroReponse = envoyerReponse500;
        return 0;
    }

    return size;
}

//on stocke EOF?// truc à voir bizarre je trouve
char *getExtension(Requete r)
{
    char *content = NULL;
    char com[256] = "";
    char prev[256] = "";
    sscanf(r->fichier, "%[^.].%s", com, prev);
    //+4 car EOF fait 4
    content = malloc(sizeof(char) * (strlen(prev) + 4));
    strcpy(content, prev);
    return content;
}

char *envoyerContenuFichier(Requete r)
{
    //peut etre truc à revoir ici +1 +4 ??
    char *content = malloc(sizeof(char) * (longeurFichier(r) + 1));
    FILE *fichier;
    char ch;
    int i = 0;
    //Ouverture du fichier à copier et affichage d'une erreur si impossible
    if ((fichier = fopen(r->fichier, "rt")) == NULL)
    {
        perror("Probleme à l'ouverture de l'fichier 2: ");
        r->rep->numeroReponse = envoyerReponse404;
        return NULL;
    }

    while ((ch = fgetc(fichier)) != EOF)
    {
        content[i] = ch;
        ++i;
    }

    content[i] = '\0';

    if (fclose(fichier) == EOF)
    {
        perror("probleme fermeture fichier");
        r->rep->numeroReponse = envoyerReponse500;
        return NULL;
    }

    return content;
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
    free(sRequest->fichier);
    freeRep(sRequest->rep);
    free(sRequest);
}

void commandeGet(Requete r)
{
    r->rep->contentType = getExtension(r);

    char *envoie = malloc(sizeof(char) * 50); //modication here
    sprintf(envoie, "%ld", longeurFichier(r));

    r->rep->contentLength = envoie;
    //voir pour pas utiliser code de retour non ? pas meilleur
    if ((r->rep->contenu = envoyerContenuFichier(r)) != NULL)
    {
        r->rep->numeroReponse = envoyerReponse200HTML;
    }
}