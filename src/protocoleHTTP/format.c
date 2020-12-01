//#include "serveur.h"
#include "protocoleHTTP.h"

//#include <assert.h>

//\r\n ? dans la requete
//\n \n
//faire traitement en fonction du code réponse
//optimisation get ouvrir 2 fois une fois pour la taille et une fois pour le contenue pas ouf non ?
//voir pour pas essayer de mettre des const

/**
 * @brief Permet d'extraire la commande d'une requete HTTP
 * 
 * @param requete l la chaine de caractere
 * @param r la structure qui sauvegardera notre requete
 * @return int 1 est bon autre pas bon
 */
int extractCommande(char *requete, RequeteStruct *r)
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

//renomer variable
int extraitFichier(char *requete, RequeteStruct *r)
{
    char com[256] = "";
    char prev[256] = "";
    int err = sscanf(requete, "%s /%s HTTP/", prev, com);
    //que faire en cas de juste /
    printf("com avant :%s\n", com);
    sprintf(prev, "fichier/%s", com); //permet qu'on aille directement dans le dossier fichier
    printf("com apres :%s\n", prev);
    r->fichier = malloc(sizeof(char) * (strlen(prev) + 1));
    strcpy(r->fichier, prev);
    printf("notre fichier c'est :%sfin\n", r->fichier);
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
    err = sscanf(requete, "%s /%s HTTP/%s %[^\n]", aze, aze2, rep, all);
    //modification here
    if (err == 3 && (!strcmp(rep, "0.9") || !strcmp(rep, "1.0") || !strcmp(rep, "1.1") || !strcmp(rep, "2.0")))
    {
        //format bon
        return 1;
    }
    //format pas bon
    return 0;
}

//possiblement mettre en place une grande initilisaiton pour rendre le code plus propre
//mettre en place des defines
RequeteStruct *annalyseRequete(char *requete)
{
    /*Allocation de la mémoire dans la heap*/
    RequeteStruct *r = initialisationStructure();

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