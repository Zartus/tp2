#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"
#include <string.h>
int main()
{
    char *message = NULL;

    Requete requete;

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
                //stockage type requete
				//strcpy(extension,"Content type : text/");
				//strcat(extension,getExtension(requete));
				//strcat(extension,"\n");
                //extension = getExtension(requete);
                //stockage longueur fichier
                //sprintf(test,"%ld",longeurFichier(requete));
				//strcat(extension,"Content-length : ");
				//strcat(extension,test);
                //strcat(extension,"\n");

                Reponse b;
                b=rep(requete);
                
                if (Emission("hop\n") != 1)
                {
                    printf("Erreur d'emission\n");
                }

                envoyerReponse(b,Emission);
                
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