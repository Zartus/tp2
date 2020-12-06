#include "serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char *message = NULL;

    Initialisation();
    Requete r;
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
                //test(message);
                r=annalyseRequete(message);
                repondre(r,Emission);

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