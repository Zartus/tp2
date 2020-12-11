#include "protocoleHTTP.h"

void modeleSetError(RequeteStruct *r,char *erreur){
    free(r);
    r=initialisationStructure();
    r->fichier=malloc((strlen(erreur)+1)*sizeof(char));
    strcpy(r->fichier,erreur);
    r->rep->contentType = getExtension(r);
    r->rep->contentLength = longeurFichier(r);
    r->rep->contenu = envoyerContenuFichierText(r);
}

//vérifier taille strlene et envoie
//modifier la taille du buffer partout
int envoyerReponse200HTML(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    char envoie[1024] = "";
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie,strlen(envoie),0);
}

int envoyerReponse200JPG(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    char envoie[1024] = "";
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: image/jpeg\r\n\r\n", r->rep->contentLength);
    printf("nb avant ==%d",envoyer(envoie,strlen(envoie),0));
    printf("nb caractere emis ==%d",envoyer(r->rep->contenu,r->rep->contentLength,1));
    return 1;
}

int envoyerReponse200ICO(RequeteStruct *r, OperateFunctor envoyer)
{
    char envoie[1024] = "";
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: image/x-icon\r\n\r\n", r->rep->contentLength);
    printf("nb avant ==%d",envoyer(envoie,strlen(envoie),0));
    printf("nb caractere emis ==%d",envoyer(r->rep->contenu,r->rep->contentLength,1));
    return 1;//;
}

int envoyerReponse404(RequeteStruct *r, OperateFunctor envoyer)
{
    //modifier taille buffer
    char envoie[1024] = "";
    modeleSetError(r,"fichier/404.html");
    sprintf(envoie, "HTTP/1.1 404 Not Found\r\nContent-Length: %ld\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie,strlen(envoie),0);
}

int envoyerReponse400(RequeteStruct *r, OperateFunctor envoyer)
{
    (void)r;
    (void)envoyer;
    return 1;
}

int envoyerReponse500(RequeteStruct *r, OperateFunctor envoyer)
{
    char envoie[1024] = ""; //modifier here
    modeleSetError(r,"fichier/500.html");
    sprintf(envoie, "HTTP/1.1 500 Serveur Error\r\nContent-Length: %ld\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, r->rep->contentType, r->rep->contenu);
    return envoyer(envoie,strlen(envoie),0);
}