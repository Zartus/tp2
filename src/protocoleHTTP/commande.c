#include "protocoleHTTP.h"

int repondre(RequeteStruct* r, OperateFunctor envoyer)
{
    assert(r != NULL); //verifie que r existe
    
    int err = 0;
    
    if (r->commande != NULL) //si il y a une commande dans notre structure on la lance
    {
        r->commande(r); //application de changement sur la structure
    }

    err = r->rep->numeroReponse(r, envoyer);
    
    /*On libere la mémoire*/
    freeRequete(r);
    
    //assert((r == NULL) && (err == 0 || err == 1));//permet de verifier err et que r est bien liberer
    return err;
}

void commandeGet(Requete r)
{
    r->rep->contentType = getExtension(r);

    //voir pour pas utiliser code de retour non ? pas meilleur
    r->rep->contentLength = longeurFichier(r);

    if(!strcmp(r->rep->contentType, "html")){
        if ((r->rep->contenu = envoyerContenuFichierText(r)) != NULL)
        {
            r->rep->numeroReponse = envoyerReponse200HTML;
        }
    }else{
        if ((r->rep->contenu = envoyerContenuFichierBinaire(r)) != NULL)
        {
            if(!strcmp(r->rep->contentType, "jpg"))
                r->rep->numeroReponse = envoyerReponse200JPG;
            if(!strcmp(r->rep->contentType, "ico"))
                r->rep->numeroReponse = envoyerReponse200ICO;
        }
    }
}