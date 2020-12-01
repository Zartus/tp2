#include "protocoleHTTP.h"


//renvoie 1 si l'envoie c'est bien passé sinon 0 à voir la gestion d'erreur qu'on peut
//mettre en place
int repondre(RequeteStruct *r, OperateFunctor envoyer)
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