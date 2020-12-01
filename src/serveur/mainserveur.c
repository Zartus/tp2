#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"
#include <string.h>

int main()
{
    char *message = NULL;

    Requete requete = NULL;
    
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
                requete = annalyseRequete(message);

                if (repondre(requete, Emission) != 1)
                {
                    printf("Erreur d'emission");
                }

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