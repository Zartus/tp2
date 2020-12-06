#include "protocoleHTTP.h"

void modeleSetError(RequeteStruct *r,char *erreur){
    strcpy(r->fichier,erreur);
    r->rep->contenu = envoyerContenuFichierText(r);
    r->rep->contentType = getExtension(r);
    r->rep->contentLength = longeurFichier(r);
}

//modifier la taille du buffer partout
int envoyerReponse200HTML(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    char envoie[1024] = "";
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse200JPG(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    (void)envoyer;
    char envoie[1024] = "";
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: image/jpeg\r\n\r\n", r->rep->contentLength);
    printf("nb avant ==%d",Emission(envoie));
    printf("nb caractere emis ==%d",EmissionBinaire(r->rep->contenu,r->rep->contentLength));
    return 1;//;
}

int envoyerReponse200ICO(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    (void)envoyer;
    char envoie[1024] = "";
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: image/x-icon\r\n\r\n", r->rep->contentLength);
    printf("nb avant ==%d",Emission(envoie));
    printf("nb caractere emis ==%d",EmissionBinaire(r->rep->contenu,r->rep->contentLength));
    return 1;//;
}

int envoyerReponse400(RequeteStruct *r, OperateFunctor envoyer)
{
    (void)envoyer;
    (void)r;
    printf("here");
    //char envoie[1024] = "";
    //modeleSetError(r,"fichier/400.html");
    //sprintf(envoie, "HTTP/1.1 400 Bad Request\r\nContent-Length: %ld\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    //return envoyer(envoie);
    return 1;
}

int envoyerReponse404(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    printf("here \n\n\n");
    char envoie[1024] = "";
    modeleSetError(r,"fichier/404.html");
    //strcpy(r->fichier, "fichier/404.html");
    //r->rep->contenu = envoyerContenuFichierText(r);
    //r->rep->contentType = getExtension(r);
    //r->rep->contentLength = longeurFichier(r);

    sprintf(envoie, "HTTP/1.1 404 Not Found\r\nContent-Length: %ld\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse500(RequeteStruct *r, OperateFunctor envoyer)
{
    (void)r;
    char envoie[256] = ""; //modifier here
    sprintf(envoie, "HTTP/1.1 500 Not Found");
    return envoyer(envoie);
}