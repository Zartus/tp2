#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <errno.h>

#include "serveur.h"

#define TRUE 1
#define FALSE 0
#define LONGUEUR_TAMPON 4096

#ifdef WIN32
#define perror(x) printf("%s : code d'erreur : %d\n", (x), WSAGetLastError())
#define close closesocket
#define socklen_t int
#endif

/*
typedef struct s_httpRequestStruct
{
    char *commande;
    float protocoleVersion;
    char *fichier;
    char *extension;
    //char *corps;
} RequeteStruct;*/

/* Variables cachees */

/* le socket d'ecoute */
int socketEcoute;
/* longueur de l'adresse */
socklen_t longeurAdr;
/* le socket de service */
int socketService;
/* le tampon de reception */
char tamponClient[LONGUEUR_TAMPON];
int debutTampon;
int finTampon;

/* Initialisation.
 * Creation du serveur.
 */
int Initialisation()
{
	return InitialisationAvecService("13214");
}

/* Initialisation.
 * Creation du serveur en précisant le service ou numéro de port.
 * renvoie 1 si ça c'est bien passé 0 sinon
 */
int InitialisationAvecService(char *service)
{
	int n;
	const int on = 1;
	struct addrinfo hints, *res, *ressave;

#ifdef WIN32
	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR)
	{
		printf("WSAStartup() n'a pas fonctionne, erreur : %d\n", WSAGetLastError());
		WSACleanup();
		exit(1);
	}
	memset(&hints, 0, sizeof(struct addrinfo));
#else
	bzero(&hints, sizeof(struct addrinfo));
#endif

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((n = getaddrinfo(NULL, service, &hints, &res)) != 0)
	{
		printf("Initialisation, erreur de getaddrinfo : %s", gai_strerror(n));
		return 0;
	}
	ressave = res;

	do
	{
		socketEcoute = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketEcoute < 0)
			continue; /* error, try next one */

		setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
#ifdef BSD
		setsockopt(socketEcoute, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
#endif
		if (bind(socketEcoute, res->ai_addr, res->ai_addrlen) == 0)
			break; /* success */

		close(socketEcoute); /* bind error, close and try next one */
	} while ((res = res->ai_next) != NULL);

	if (res == NULL)
	{
		perror("Initialisation, erreur de bind.");
		return 0;
	}

	/* conserve la longueur de l'addresse */
	longeurAdr = res->ai_addrlen;

	freeaddrinfo(ressave);

	/* attends au max 4 clients */
	printf("listent ==%d\n",listen(socketEcoute, -1));
	
	printf("Creation du serveur reussie sur %s.\n", service);

	return 1;
}

/* Attends qu'un client se connecte.
 */
int AttenteClient()
{
	struct sockaddr *clientAddr;
	char machine[NI_MAXHOST];

	clientAddr = (struct sockaddr *)malloc(longeurAdr);
	socketService = accept(socketEcoute, clientAddr, &longeurAdr);
	if (socketService == -1)
	{
		perror("AttenteClient, erreur de accept.");
		return 0;
	}
	if (getnameinfo(clientAddr, longeurAdr, machine, NI_MAXHOST, NULL, 0, 0) == 0)
	{
		printf("Client sur la machine d'adresse %s connecte.\n", machine);
	}
	else
	{
		printf("Client anonyme connecte.\n");
	}
	free(clientAddr);
	/*
	 * Reinit buffer
	 */
	debutTampon = 0;
	finTampon = 0;

	return 1;
}

/* Recoit un message envoye par le serveur.
 */
char *Reception()
{
	printf("la");
	char message[LONGUEUR_TAMPON];
	int index = 0;
	int fini = FALSE;
	int retour = 0;
	
	while (!fini)
	{
		/* on cherche dans le tampon courant */
		while ((finTampon > debutTampon) &&
			   (tamponClient[debutTampon] != '\n'))
		{
			printf("danas la boucle \n");
			message[index++] = tamponClient[debutTampon++];
		}
		/* on a trouve ? */
		if ((index > 0) && (tamponClient[debutTampon] == '\n'))
		{
			message[index++] = '\n';
			message[index] = '\0';
			debutTampon++;
			fini = TRUE;
#ifdef WIN32
			return _strdup(message);
#else
			return strdup(message);
#endif
		}
		else
		{
			/* il faut en lire plus */
			debutTampon = 0;
			retour = recv(socketService, tamponClient, LONGUEUR_TAMPON, 0);
			if (retour < 0)
			{
				perror("Reception, erreur de recv.");
				return NULL;
			}
			else if (retour == 0)
			{
				fprintf(stderr, "Reception, le client a ferme la connexion.\n");
				return NULL;
			}
			else
			{
				/*
				 * on a recu "retour" octets
				 */
				finTampon = retour;
			}
		}
	}
	return NULL;
}

/* Envoie un message au client.
 * Attention, le message doit etre termine par \n
 */
int Emission(char *message)
{
	int taille;
	if (strstr(message, "\n") == NULL)
	{
		fprintf(stderr, "Emission, Le message n'est pas termine par \\n.\n");
		return 0;
	}
	taille = strlen(message);
	if (send(socketService, message, taille, 0) == -1)
	{
		perror("Emission, probleme lors du send.");
		return 0;
	}
	printf("Emission de %d caracteres.\n", taille + 1);
	return 1;
}

/* Recoit des donnees envoyees par le client.
 */
int ReceptionBinaire(char *donnees, size_t tailleMax)
{
	size_t dejaRecu = 0;
	int retour = 0;
	/* on commence par recopier tout ce qui reste dans le tampon
	 */
	while ((finTampon > debutTampon) && (dejaRecu < tailleMax))
	{
		donnees[dejaRecu] = tamponClient[debutTampon];
		dejaRecu++;
		debutTampon++;
	}
	/* si on n'est pas arrive au max
	 * on essaie de recevoir plus de donnees
	 */
	if (dejaRecu < tailleMax)
	{
		retour = recv(socketService, donnees + dejaRecu, tailleMax - dejaRecu, 0);
		if (retour < 0)
		{
			perror("ReceptionBinaire, erreur de recv.");
			return -1;
		}
		else if (retour == 0)
		{
			fprintf(stderr, "ReceptionBinaire, le client a ferme la connexion.\n");
			return 0;
		}
		else
		{
			/*
			 * on a recu "retour" octets en plus
			 */
			return dejaRecu + retour;
		}
	}
	else
	{
		return dejaRecu;
	}
}

/* Envoie des données au client en précisant leur taille.
 */
int EmissionBinaire(char *donnees, size_t taille)
{
	int retour = 0;
	retour = send(socketService, donnees, taille, 0);
	if (retour == -1)
	{
		perror("Emission, probleme lors du send.");
		return -1;
	}
	else
	{
		return retour;
	}
}

/* Ferme la connexion avec le client.
 */
void TerminaisonClient()
{
	close(socketService);
}

/* Arrete le serveur.
 */
void Terminaison()
{
	close(socketEcoute);
}
/**
 * @brief Permet d'envoyer un message sois binaire sois texte
 * 
 * @param message le message à envoyer
 * @param contentLength la taille du message
 * @param binaire si ==1 alors c'est une émission binaire sinon c'est une emission texte
 * @return int 
 */
int emissionChoice(char *message,size_t contentLength,int binaire){
    
    if(binaire)
        return EmissionBinaire(message,contentLength);
    
    return Emission(message);
}
