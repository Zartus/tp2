#include "protocoleHTTP.h"
#include "requete.h"

int repondre(RequeteStruct *r, OperateFunctor envoyer)
{
    assert(r != NULL); //mode DEBUG

    int err = 0;

    if (r->commande)    //si il y a une commande dans notre structure on la lance
        r->commande(r); //application de changement sur la structure

    err = r->rep->numeroReponse(r, envoyer);

    /*On libere la mémoire*/
    freeRequete(r);

    return err;
}

void commandeGet(Requete r)
{

    r->rep->contentType = getExtension(r);

    r->rep->contentLength = longeurFichier(r);

    /*quand on arrive pas à ouvrir le fichier erreur 404*/
    if (r->rep->numeroReponse == NULL)
    {
        if (r->rep->contentType < lastText)
        {

            if ((r->rep->contenu = envoyerContenuFichierText(r)))
                r->rep->numeroReponse = envoyerReponse200HTML;
        }
        else
        {
            if ((r->rep->contenu = envoyerContenuFichierBinaire(r)))
                r->rep->numeroReponse = envoyerReponse200Binaire;
        }
    }
}