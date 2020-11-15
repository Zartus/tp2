#ifndef __CLIENT_H__
#define __CLIENT_H__

/**
 * @brief Initialisation.
 * Connexion au serveur sur la machine donnee.
 * Utilisez localhost pour un fonctionnement local.
 * @param machine la machine
 * @return int 1 si ça c'est bien passé 0 sinon
 */
int Initialisation(char *machine);

/**
 * @brief Initialisation.
 * Connexion au serveur sur la machine donnee et au service donne.
 * Utilisez localhost pour un fonctionnement local.
 * @param machine la machine
 * @param service le service
 * @return renvoie 1 si ça c'est bien passé 0 sinon
 */
int InitialisationAvecService(char *machine, char *service);

/**
 * @brief Recoit un message envoye par le serveur.
 * Note : il faut liberer la memoire apres traitement.
 * @return char* retourne le message ou NULL en cas d'erreur.
 */
char *Reception();

/**
 * @brief Envoie un message au serveur.
 * Attention, le message doit etre termine par \n
 * @param message 
 * @return int renvoie 1 si ça c'est bien passé 0 sinon
 */
int Emission(char *message);

/**
 * @brief Recoit des donnees envoyees par le serveur.
 * 
 * @param donnees 
 * @param tailleMax 
 * @return int renvoie le nombre d'octets reçus, 0 si la connexion est fermée,
 * un nombre négatif en cas d'erreur
 */
int ReceptionBinaire(char *donnees, size_t tailleMax);

/**
 * @brief Envoie des données au serveur en précisant leur taille.
 * 
 * @param donnees 
 * @param taille 
 * @return int renvoie le nombre d'octets envoyés, 0 si la connexion est fermée
 * un nombre négatif en cas d'erreur
 */
int EmissionBinaire(char *donnees, size_t taille);

/**
 * @brief Ferme la connexion.
 * 
 */
void Terminaison();

#endif
