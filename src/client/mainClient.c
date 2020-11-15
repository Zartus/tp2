#include <stdio.h>
#include <stdlib.h>
#include "client.h"

int main()
{
	char *message;

	if (InitialisationAvecService("www.google.fr", "80") != 1)
	{
		printf("Erreur d'initialisation\n");
		return 1;
	}

	if (Emission("GET / HTTP/1.1\n") != 1)
	{
		printf("Erreur d'emission\n");
		return 1;
	}

	if (Emission("Host:www.licence.mathinfo.upmc.fr\n") != 1)
	{
		printf("Erreur d'emission\n");
		return 1;
	}

	if (Emission("\n") != 1)
	{
		printf("Erreur d'emission\n");
		return 1;
	}

	message = Reception();
	if (message != NULL)
	{
		printf("J'ai recu: %s\n", message);
		free(message);
	}
	else
	{
		printf("Erreur de réception\n");
		return 1;
	}

	message = Reception();
	if (message != NULL)
	{
		printf("J'ai recu: %s\n", message);
		free(message);
	}
	else
	{
		printf("Erreur de réception\n");
		return 1;
	}

	while (1)
	{
		message = Reception();
		if (message != NULL)
		{
			printf("J'ai recu : %s\n", message);
			free(message);
		}
		else
		{
			printf("Erreur de reception\n");
			return 1;
		}
	}

	Terminaison();

	return 0;
}
