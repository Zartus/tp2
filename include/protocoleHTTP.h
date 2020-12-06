#ifndef __PROTOCOLE_H__
#define __PROTOCOLE_H__
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "serveur.h"

#define LONGUEUR_TAMPON 4096
//possiblement faire 2 .h car probleme sur les acces

/**
 * @brief functor quoi doit respecter le format suivant int functor(char* something)
 * utiliser ici pour émission mais peut etre utiliser pour d'autre chose put ...
 */
typedef int (*OperateFunctor)(char *);

typedef struct s_httpRequestStruct
{
    void (*commande)(struct s_httpRequestStruct *);
    char *fichier;
    struct reponseRequeteS *rep;
}RequeteStruct;

typedef struct reponseRequeteS
{
    char *contentType;
    size_t contentLength;
    char *contenu;
    int (*numeroReponse)(struct s_httpRequestStruct *, OperateFunctor);
} reponseRequete;

typedef struct s_httpRequestStruct* Requete;

/**
 * @brief Permet d'ouvrir un fichier
 * 
 * @param fileName le chemin du fichier
 * @param s modes d'ouverture
 * @return FILE* renvoie un pointeur vers se fichier
 */
FILE *ouvertureFichier(char *fileName, char *s);

/**
 * @brief Permet de fermer un fichier
 * 
 * @param f le fichier
 * @return int 1 la fermeture c'est bien passé, 0 probléme lors de la fermeture
 */
int fermetureFichier(FILE *f);

/**
 * @brief Permet d'initialiser la requete
 * 
 * @return RequeteStruct* renvoie un pointeur vers la requete
 */
RequeteStruct* initialisationStructure();

/**
 * @brief annalyse la requete 
 * 
 * @param requete la requete à annalyser
 * @return RequeteStruct* renvoie 
 */
RequeteStruct* annalyseRequete(char *requete);

/**
 * @brief Permet de liberer la mémoire de la requete
 * 
 * @param sRequest 
 */
void freeRequete(RequeteStruct* sRequest);

/**
 * @brief Permet d'obtenir la longeur d'un fichier en caractere UTF-8
 * le fichier doit etre present dans la structure requete r->fichier
 * @param r la requete qui contient le fichier
 * @return size_t le nombre de de caractere dans le fichier
 */
size_t longeurFichier(RequeteStruct* r);

size_t longeurFichierBinaire(RequeteStruct *r);
/**
 * @brief Permet d'obtenir l'extension d'un fichier
 * 
 * @param r la requete qui contient le fichier
 * @return char* retourne une pointeur vers l'extension du fichier
 */
char *getExtension(RequeteStruct* r);

/**
 * @brief Permet d'obtenir le contenue du fichier text 
 * présent dans la requete passer en parametre
 * @param r la requete qui contient le fichier
 * @return char* retourne une pointeur vers le contenue du fichier
 */
char *envoyerContenuFichierText(RequeteStruct* r);

/**
 * @brief Permet d'obtenir le contenue du binaire
 * présent dans la requete passer en parametre
 * @param r la requete qui contient le fichier
 * @return char* retourne une pointeur vers le contenue du fichier
 */
char *envoyerContenuFichierBinaire(RequeteStruct* r);

/**
 * @brief Ce que doit réaliser la commande GET
 * 
 * @param r La requete sur la quelle appliqué la commande GET
 */
void commandeGet(RequeteStruct* r);

/**
 * @brief Permet d'envoyer une réponse au travers du Functor 
 * 
 * @param r La requete à la quelle on doit répondre
 * @param Envoyer notre functor qui nous permet de manipuler notre requete
 * @return int Renvoie 1 si y a pas eu de probleme sinon 0 si le Functor à échouer
 */
int repondre(RequeteStruct* r,OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 200
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse200HTML(RequeteStruct *r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 200
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse200JPG(RequeteStruct *r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 400
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse400(RequeteStruct* r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 404
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse404(RequeteStruct* r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 500
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse500(RequeteStruct* r, OperateFunctor envoyer);

int envoyerReponse200ICO(RequeteStruct *r, OperateFunctor envoyer);

void test(char *requete);

#endif