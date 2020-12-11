#include "protocoleHTTP.h"

/**
 * @brief Permet d'extraire la commande d'une requete HTTP
 * 
 * @param requete l la chaine de caractere
 * @param r la structure qui sauvegardera notre requete
 * @return int 1 est bon autre pas bon
 */
int extractCommande(char *requete, RequeteStruct *r)
{
    /*buffer pour extraire la commande */
    /*improbable que la commande est une taille > 1000*/
    /*donc buffer asser grand*/
    char possibilite[1024] = "";

    /*on utilise sscanf pour extraire la commande*/
    int err = sscanf(requete, "%s /", possibilite);

    /*si err==1 c'est que la ommande à ete extraite*/
    if (err == 1)
    {
        /**/
        if (!strcmp(possibilite, "GET"))//les differentes commande ici
        {
            r->commande = commandeGet;
        }
        else
        {
            err = 0;
        }
    }
    /*on retourne ensuite l'erreur*/
    /*err==1 tout c'est bien passé commande reconnu*/
    /*err==0 commande non reconnnu donc erreur*/
    return err;
}

//renomer variable
//que faire en cas de juste /
void extraitFichier(char *requete, RequeteStruct *r)
{
    /*Initialisation des buffers que nous allons utiliser*/
    char fichier[512] = "";
    char prev[256] = "";
    char path[1024] = "";
    
    /*On utilise sscanf pour extraire de la requete comme nous savons que le format est bon*/
    sscanf(requete, "%s /%[^ H] HTTP/", prev, fichier);

    /*Comme tous nos fichiers se trouve dans le répertoire fichier on ajoute devant*/
    /*l'endroit ou se trouve tout les fichier*/
    sprintf(path, "fichier/%s", fichier);
    
    /*ON alloue la mémoire*/
    if((r->fichier = malloc(sizeof(char) * (strlen(path)+1)))==NULL){
        /*si il y a eu un probleme on l'affiche du coté serveur*/
        perror("Probleme allocation");
        /*Et on indique dans la réponse qu'il y a eu un probleme du coté serveur*/
        /*erreur 500*/
        r->rep->numeroReponse=envoyerReponse500;
    }else{
        /*si tout c'est bien passé on place le chemin du fichier dans notre structure*/
        strcpy(r->fichier, path);
    }
}

int verifFormat(char *requete)
{
    /*tableau de chaine de caractee qui nous sert de buffer*/
    /*Pas besoin de stocker grand chose dedans*/
    /*on veut juste pour le moment vérfier qu'il y a quelque chose dedans*/
    char verif[4][256]={"","","",""};
    
    /*variable qui gere les erreurs*/
    int err = 0;

    /*on annalyse la requete avec sscanf*/
    err = sscanf(requete, "%s /%s HTTP/%s %[^\r\n]", verif[0], verif[1], verif[2], verif[3]);

    /*si le format est bon err sera à 3*/
    /*car 3 variables extraites de sscanf*/
    /*On regarde ensuite si le numéro du protocole est correcte */
    if (err == 3 && (!strcmp(verif[2], "0.9") 
                     || !strcmp(verif[2], "1.0") 
                     || !strcmp(verif[2], "1.1") 
                     || !strcmp(verif[2], "2.0")))
    {
        return 1;//format bon
    }
    
    return 0;//format pas bon
}


RequeteStruct *annalyseRequete(char *requete)
{
    /*Allocation de la mémoire dans la heap*/
    /*On appelle la fonction qui initilise la structure*/
    RequeteStruct *r = initialisationStructure();

    /*On appelle la fonction qui vérifie le format*/
    if (!verifFormat(requete))
    {
        /*si elle retourne 0 alors on indique la réponse 400 comme réponse*/
        /*à la requete*/
        r->rep->numeroReponse = envoyerReponse400;
        /*et on retourne la requete*/
        return r;
    }

    /*extraction de la commande*/
    if (extractCommande(requete, r) != 1)
    {
        /*si elle retourne 0 alors on indique la réponse 400 comme réponse*/
        /*à la requete*/
        r->rep->numeroReponse = envoyerReponse400;
        /*et on retourne la requete*/
        return r;
    }

    /*Si on arrive la c'est que tout est bon pour le moment*/
    /*On essaye d'extraire alors le fichier de la requete*/
    extraitFichier(requete, r);
    
    return r;
}