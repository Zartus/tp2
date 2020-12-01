#include "protocoleHTTP.h"

//modifier la taille du buffer partout
int envoyerReponse200HTML(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    char envoie[1024] = "";
    printf("r->rep->contenu %s", r->rep->contenu);
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %s\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse400(RequeteStruct *r, OperateFunctor envoyer)
{
    (void)r;
    char envoie[256] = ""; //modifier here
    sprintf(envoie, "HTTP/1.1 400 Bad Request\r\nContent-Length: %s\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse404(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    printf("here \n\n\n");
    char envoie[1024] = "";
    strcpy(r->fichier, "fichier/404.html");
    r->rep->contenu = envoyerContenuFichier(r);
    r->rep->contentType = getExtension(r);
    char *envoie2 = malloc(sizeof(char) * 50); //modication here
    sprintf(envoie2, "%ld", longeurFichier(r));

    r->rep->contentLength = envoie2;
    printf("r->rep->contenu %s", r->rep->contenu);
    sprintf(envoie, "HTTP/1.1 404 Not Found\r\nContent-Length: %s\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie);
}

int envoyerReponse500(RequeteStruct *r, OperateFunctor envoyer)
{
    (void)r;
    char envoie[256] = ""; //modifier here
    sprintf(envoie, "HTTP/1.1 500 Not Found");
    return envoyer(envoie);
}

int envoyerReponse200JPG(RequeteStruct *r, OperateFunctor envoyer)
{
    (void)r;
    (void)envoyer;
    return 1;
}