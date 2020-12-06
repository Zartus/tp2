#include "protocoleHTTP.h"

RequeteStruct *initialisationStructure()
{
    RequeteStruct *r = malloc(sizeof(RequeteStruct));
    r->commande = NULL;
    r->fichier = NULL;
    r->rep = malloc(sizeof(reponseRequete));
    r->rep->contentType = NULL;
    r->rep->contentLength = 0;
    r->rep->numeroReponse = NULL;
    r->rep->contenu = NULL;
    return r;
}

void useCommande(Requete r){
    r->commande(r);
}

/**
 * @brief Libere la mémoire qui correspond à la réponse de la requete
 * 
 * @param rep la reponse de la requete
 */
void freeRep(reponseRequete *rep)
{
    if(rep->contentType)
        free(rep->contentType);
    if(rep->contenu)
        free(rep->contenu);
    free(rep);
}

/**
 * @brief Libere la mémoire de toute la requete
 * 
 * @param sRequest La requete
 */
void freeRequete(RequeteStruct *sRequest)
{
    if(sRequest->fichier!=NULL)
        free(sRequest->fichier);
    freeRep(sRequest->rep);
    free(sRequest);
}