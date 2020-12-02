#include "protocoleHTTP.h"

int repondre(RequeteStruct *r, OperateFunctor envoyer)
{
    assert(r != NULL); //verifie que r existe

    int err = 0;
    if (r->commande != NULL)
    {
        r->commande(r);
    }

    err = r->rep->numeroReponse(r, envoyer);
    //freeRequete(r);
    //assert((r == NULL) && (err == 0 || err == 1));//permet de verifier err et que r est bien liberer
    return err;
}

void commandeGet(Requete r)
{
    r->rep->contentType = getExtension(r);

    //voir pour pas utiliser code de retour non ? pas meilleur

    if (!strcmp(r->rep->contentType, "jpg"))
    {
        r->rep->contentLength = longeurFichierBinaire(r);
        if ((r->rep->contenu = envoyerContenuFichierBinaire(r)) != NULL)
        {
            r->rep->numeroReponse = envoyerReponse200JPG;
        }
    }
    else if (!strcmp(r->rep->contentType, "ico"))
    {
        r->rep->contentLength = longeurFichierBinaire(r);
        if ((r->rep->contenu = envoyerContenuFichierBinaire(r)) != NULL)
        {
            r->rep->numeroReponse = envoyerReponse200ICO;
        }
    }
    else
    {
        r->rep->contentLength = longeurFichier(r);
        if ((r->rep->contenu = envoyerContenuFichierText(r)) != NULL)
        {
            r->rep->numeroReponse = envoyerReponse200HTML;
        }
    }
}
//si il est toujours impossible d'ouvrir le fichier ou de le fermer // probleme System !