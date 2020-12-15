#ifndef __PROTOCOLE_H__
#define __PROTOCOLE_H__
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "serveur.h"

/**
 * @brief Enumeartion avec les differents type de média possible
 * 
 */
enum type { HTML, JPEG, ICO, UNKNOW };

#define LONGUEUR_TAMPON 4096

#define TYPEMEDIA "html", "jpg", "jpeg", "ico","unknow"

#define lastText 1

#define SIZETYPEMEDIA 5

#define MEDIACONTENTREPONSE "text/html","image/jpeg","image/x-icon","unknow/unknow"

#define mediaReponseLength 4

//possiblement faire 2 .h car probleme sur les acces
/**
 * @brief TypeOpaque
 * Pour pas que l'utilisateur de notre API puisse avoir accer directement au contenu de notre structure
 * 
 */
typedef struct s_httpRequestStruct* Requete;

/**
 * @brief functor quoi doit respecter le format suivant int functor(char* something)
 * utiliser ici pour émission mais peut etre utiliser pour d'autre chose put ...
 */
typedef int (*OperateFunctor)(char *,size_t,int);

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
Requete initialisationStructure();

/**
 * @brief annalyse la requete 
 * 
 * @param requete la requete à annalyser
 * @return RequeteStruct* renvoie 
 */
Requete annalyseRequete(char *requete);

/**
 * @brief Permet de liberer la mémoire de la requete
 * 
 * @param sRequest 
 */
void freeRequete(Requete sRequest);

/**
 * @brief Permet d'obtenir la longeur d'un fichier en caractere UTF-8
 * le fichier doit etre present dans la structure requete r->fichier
 * @param r la requete qui contient le fichier
 * @return size_t le nombre de de caractere dans le fichier
 */
size_t longeurFichier(Requete r);

/**
 * @brief Permet d'obtenir l'extension d'un fichier
 * 
 * @param r la requete qui contient le fichier
 * @return char* retourne une pointeur vers l'extension du fichier
 */
enum type getExtension(Requete r);

/**
 * @brief Permet d'obtenir le contenue du fichier text 
 * présent dans la requete passer en parametre
 * @param r la requete qui contient le fichier
 * @return char* retourne une pointeur vers le contenue du fichier
 */
char *envoyerContenuFichierText(Requete r);

/**
 * @brief Permet d'obtenir le contenue du binaire
 * présent dans la requete passer en parametre
 * @param r la requete qui contient le fichier
 * @return char* retourne une pointeur vers le contenue du fichier
 */
char *envoyerContenuFichierBinaire(Requete r);

/**
 * @brief Ce que doit réaliser la commande GET
 * 
 * @param r La requete sur la quelle appliqué la commande GET
 */
void commandeGet(Requete r);

/**
 * @brief Permet d'envoyer une réponse au travers du Functor 
 * 
 * @param r La requete à la quelle on doit répondre
 * @param Envoyer notre functor qui nous permet de manipuler notre requete
 * @return int Renvoie 1 si y a pas eu de probleme sinon 0 si le Functor à échouer
 */
int repondre(Requete r,OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 200
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse200HTML(Requete r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 200
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse200JPG(Requete r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 400
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse400(Requete r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 404
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse404(Requete r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 500
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse500(Requete r, OperateFunctor envoyer);

/**
 * @brief Permet d'envoyer la réponse HTTP 200
 * 
 * @param r La requete à la quelle on doit répondre
 * @param envoyer Le functor qui nous permet de répondre
 * @return int retourne 1 si le Functor c'est bien passé sinon 0
 */
int envoyerReponse200ICO(Requete r, OperateFunctor envoyer);

int envoyerReponse200HTML(Requete r, OperateFunctor envoyer);

int envoyerReponse200Binaire(Requete r, OperateFunctor envoyer);

void test(char *requete);

#endif