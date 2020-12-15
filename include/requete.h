#ifndef __REQUETE_H__
#define __REQUETE_H__
#include <stddef.h>

typedef struct s_httpRequestStruct
{
    void (*commande)(struct s_httpRequestStruct *);
    char *fichier;
    struct reponseRequeteS *rep;
}RequeteStruct;

typedef struct reponseRequeteS
{
    enum type contentType;
    size_t contentLength;
    char *contenu;
    int (*numeroReponse)(struct s_httpRequestStruct *, OperateFunctor);
} reponseRequete;

#endif