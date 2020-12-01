#include "protocoleHTTP.h"

RequeteStruct *initialisationStructure()
{
    RequeteStruct *r = malloc(sizeof(RequeteStruct));
    r->commande = NULL;
    r->fichier = NULL;
    r->rep = malloc(sizeof(reponseRequete));
    r->rep->contentType = NULL;
    r->rep->contentLength = NULL;
    r->rep->numeroReponse = NULL;
    r->rep->contenu = NULL;
    return r;
}

void freeRep(reponseRequete *rep)
{
    free(rep->contentLength);
    free(rep->contentType);
    free(rep->contenu);
    free(rep);
}

void freeRequete(RequeteStruct *sRequest)
{
    free(sRequest->fichier);
    freeRep(sRequest->rep);
    free(sRequest);
}