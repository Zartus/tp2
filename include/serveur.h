#ifndef __SERVEUR_H__
#define __SERVEUR_H__
#include "protocoleHTTP.h"
/**
 * @brief Initialisation.
 * Creation du serveur.
 * @return int renvoie 1 si à c'est bien passé 0 sinon
 */
int Initialisation();

/**
 * @brief Initialisation.
 * Creation du serveur en précisant le service ou numéro de port.
 * @param service 
 * @return int renvoie 1 si à c'est bien passé 0 sinon
 */
int InitialisationAvecService(char *service);

/**
 * @brief Attends qu'un client se connecte.
 * 
 * @return int renvoie 1 si ça c'est bien passé 0 sinon
 */
int AttenteClient();

/**
 * @brief Recoit un message envoye par le client.
 * @return char* retourne le message ou NULL en cas d'erreur.
 * Note : il faut liberer la memoire apres traitement.
 */
char *Reception();

/**
 * @brief Envoie un message au client.
 * 
 * @param message Attention, le message doit etre termine par \n
 * @return int renvoie 1 si ça c'est bien passé 0 sinon
 */
int Emission(char *message);

/**
 * @brief Recoit des donnees envoyees par le client.
 * 
 * @param donnees 
 * @param tailleMax 
 * @return int renvoie le nombre d'octets reçus, 0 si la connexion est fermée,
 * un nombre négatif en cas d'erreur
 */
int ReceptionBinaire(char *donnees, size_t tailleMax);

/**
 * @brief Envoie des données au client en précisant leur taille.
 * 
 * @param donnees 
 * @param taille 
 * @return int renvoie le nombre d'octets envoyés, 0 si la connexion est fermée,
 * un nombre négatif en cas d'erreur
 */
int EmissionBinaire(char *donnees, size_t taille);

/**
 * @brief Ferme la connextion avec le client
 * 
 */
void TerminaisonClient();

/**
 * @brief Arrete le serveur
 * 
 */
void Terminaison();



#endif
