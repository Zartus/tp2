#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"
#include <string.h>
int main()
{
	char *message = NULL;
	Requete requete;

	//(void)requete;
	/*requete = typeRequete("GET /fichier/index.html HTTP/1.1");
	
	affichage(requete);
	printf("longeurfichier==%ld\n",longeurFichier(requete));
	freeRequete(requete);*/

	Initialisation();

	while (1)
	{
		int fini = 0;

		AttenteClient();

		while (!fini)
		{
			message = Reception();

			if (message != NULL)
			{
				printf("J'ai recu: %s\n", message);
				requete = typeRequete(message);

				affichage(requete);
				
				char test[25];
				sprintf(test,"%ld",longeurFichier(requete));
				strcat(test,"\n");
				
				if (Emission(test) != 1)
				{
					printf("Erreur d'emission\n");
				}
				freeRequete(requete);
				free(message);
			}
			else
			{
				fini = 1;
			}
		}

		TerminaisonClient();
	}

	return 0;
}
