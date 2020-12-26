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
    /*on cherche l'extension*/
    r->rep->contentType = getExtension(r);
    /*on cherche la longeur du fichier*/
    r->rep->contentLength = longeurFichier(r);

    /*quand on arrive pas à ouvrir le fichier erreur 404*/
    if (r->rep->numeroReponse == NULL)
    {
        /*si c'est un fichier texte*/
        if (r->rep->contentType < lastText)
        {  
            /*si on arrive à avoir le contenu*/
            if ((r->rep->contenu = envoyerContenuFichierText(r)))
                /*on initiailise la réponse comme 200HTML*/
                r->rep->numeroReponse = envoyerReponse200HTML;
        }
        /*si c'est un fichier binaire*/
        else
        {
            /*Si on arrive à avoir le contenue*/
            if ((r->rep->contenu = envoyerContenuFichierBinaire(r)))
                /*on initialise la réponse comme 200binaire*/
                r->rep->numeroReponse = envoyerReponse200Binaire;
        }
    }
}