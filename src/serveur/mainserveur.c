#include <stdio.h>
#include <stdlib.h>
#include "serveur.h"
#include <string.h>
int main()
{
    char *message = NULL;
    char extension[256];

    Requete requete;
    char test[256];
    
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
                //stockage type requete
				strcpy(extension,"Content type : text/");
				strcat(extension,getExtension(requete));
				strcat(extension,"\n");
                //extension = getExtension(requete);
                //stockage longueur fichier
                sprintf(test,"%ld",longeurFichier(requete));
				strcat(extension,"Content-length : ");
				strcat(extension,test);
                
                strcat(extension,"\n");
                if (Emission(extension) != 1)
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