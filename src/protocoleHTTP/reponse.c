#include "protocoleHTTP.h"
#include "requete.h"

char *allocationReponse(int size)
{
    char *message = NULL;
    if ((message = malloc(sizeof(char) * size)) == NULL)
    {
        perror("Probleme allocation");
        return NULL;
    }
    return message;
}

int envoyerReponse200HTML(RequeteStruct *r, OperateFunctor envoyer)
{
    char *envoie = NULL;
    int err = 0;
    char all[mediaReponseLength][32] = {MEDIACONTENTREPONSE};

    if((envoie = allocationReponse(r->rep->contentLength + 100))==NULL){
        return envoyerReponse500(r,envoyer);//on retourne l'erreur 500 en cas de
    }

    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: %s\r\n\r\n%s", r->rep->contentLength, all[r->rep->contentType], r->rep->contenu);
    err = envoyer(envoie, strlen(envoie), 0);
    printf("\n\nJ'ai envoyé texte :%s\n\n",all[r->rep->contentType]);
    free(envoie);
    return err;
}

int envoyerReponse200Binaire(RequeteStruct *r, OperateFunctor envoyer)
{
    char *envoie = NULL;
    char all[mediaReponseLength][32] = {MEDIACONTENTREPONSE};
    int err = 0;
    if((envoie = allocationReponse(r->rep->contentLength + 100))==NULL){
        return envoyerReponse500(r,envoyer);
    }
    sprintf(envoie, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nContent-Type: %s\r\n\r\n", r->rep->contentLength, all[r->rep->contentType]);
    err = envoyer(envoie, strlen(envoie), 0) && envoyer(r->rep->contenu, r->rep->contentLength, 1);
    free(envoie);
    printf("\n\nJ'ai envoyé binaire :%s\n\n",all[r->rep->contentType]);
    return err;
}

RequeteStruct *modeleSetError(RequeteStruct *r, char *fichier)
{
    if (r->rep->contenu)
    {
        free(r->rep->contenu);
        r->rep->contenu = NULL;
    }
    if (r->fichier)
    {
        free(r->fichier);
        r->fichier = NULL;
    }

    r->fichier = malloc(sizeof(char) * strlen(fichier) + 1);
    strcpy(r->fichier, fichier);
    r->rep->contentLength = longeurFichier(r);
    r->rep->contenu = envoyerContenuFichierText(r);

    return r;
}

int envoyerReponse404(RequeteStruct *r, OperateFunctor envoyer)
{
    int err = 0;
    r = modeleSetError(r, "fichier/404.html");
    char *envoie = allocationReponse(r->rep->contentLength + 100);
    sprintf(envoie, "HTTP/1.1 404 Not Found\r\nContent-Length: %ld\r\nContent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, "html", r->rep->contenu);
    err = envoyer(envoie, strlen(envoie), 0);
    free(envoie);
    return err;
}

int envoyerReponse400(RequeteStruct *r, OperateFunctor envoyer)
{
    //on s'en occupe pas mais elle est la
    (void)r;
    (void)envoyer;
    return 1;
}

int envoyerReponse500(RequeteStruct *r, OperateFunctor envoyer)
{
    int err = 0;
    char *envoie=NULL;
    r = modeleSetError(r, "fichier/500.html");
    if((envoie = allocationReponse(r->rep->contentLength + 100))==NULL){
        //gros probleme =>rien qui va si on arrive la
        return 0;
    }
    sprintf(envoie, "HTTP/1.1 500 \r\nContent-Length: %ld\r\nCotent-Type: text/%s\r\n\r\n%s", r->rep->contentLength, "html", r->rep->contenu);
    err = envoyer(envoie, strlen(envoie), 0);
    free(envoie);
    return err;
}