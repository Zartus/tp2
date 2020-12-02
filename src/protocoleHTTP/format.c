#include "protocoleHTTP.h"

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
    char possibilite[LONGUEUR_TAMPON] = "";
    int err = sscanf(requete, "%s /", possibilite);
    if (err == 1)
    {
        if (!strcmp(possibilite, "GET"))//les differentes commande ici
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
//que faire en cas de juste /
int extraitFichier(char *requete, RequeteStruct *r)
{
    char com[256] = "";
    char prev[256] = "";
    char path[500] = "";//voir avec make Debug
    int err = sscanf(requete, "%s /%s HTTP/", prev, com);
    
    sprintf(path, "fichier/%s", com);
    r->fichier = malloc(sizeof(char) * (strlen(path) + 1));
    strcpy(r->fichier, path);

    return err;
}

int verifProtocol(char *requete)
{

    char verif[4][256]={"","","",""};
    int err = 0;
    err = sscanf(requete, "%s /%s HTTP/%s %[^\n]", verif[0], verif[1], verif[2], verif[3]);

    if (err == 3 && (!strcmp(verif[2], "0.9") || !strcmp(verif[2], "1.0") || !strcmp(verif[2], "1.1") || !strcmp(verif[2], "2.0")))
    {
        return 1;//format bon
    }
    
    return 0;//format pas bon
}

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
    printf("le fichier ==%s\n",r->fichier);
    return r;
}