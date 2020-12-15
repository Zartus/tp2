#include "protocoleHTTP.h"
#include "requete.h"

RequeteStruct *initialisationStructure()
{
    RequeteStruct *r = malloc(sizeof(RequeteStruct));
    r->commande = NULL;
    r->fichier = NULL;
    r->rep = malloc(sizeof(reponseRequete));
    r->rep->contentType = 0;
    r->rep->contentLength = 0;
    r->rep->numeroReponse = NULL;
    r->rep->contenu = NULL;
    return r;
}

/**
 * @brief Libere la mémoire qui correspond à la réponse de la requete
 * 
 * @param rep la reponse de la requete
 */
void freeRep(reponseRequete *rep)
{
    if (rep->contenu)
    {
        free(rep->contenu);
        rep->contenu = NULL;
    }

    free(rep);
    rep = NULL;
}

/**
 * @brief Libere la mémoire de toute la requete
 * 
 * @param sRequest La requete
 */
void freeRequete(RequeteStruct *sRequest)
{
    if (sRequest->fichier)
    {
        free(sRequest->fichier);
        sRequest->fichier = NULL;
    }

    freeRep(sRequest->rep);
    sRequest->rep = NULL;
    free(sRequest);
    sRequest = NULL;
}