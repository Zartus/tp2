#include "serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//voir pour pas essayer de mettre des const
//traiter caractere speciaux ?
//verifier que tout les cas de mauvaise requetes sont gerer
//mettre en place fichier de test insipiration => tp de l'année derniere
//modifier retour des reponses envoyer
//mettre un switch pour les differentes commandes et mettre en place l'ajout avec un define
//modifier getExtention pas beau JPEG en double :(
//enlever printf


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
            printf("avant reception///\n\n");
            message = Reception();
            printf("\nDEBUT ////\n\n");

            if (message != NULL)
            {
                printf("J'ai recu: %s\n", message);

                r = annalyseRequete(message);

                if (!repondre(r, emissionChoice))
                    fprintf(stderr, "Il y a eu un probleme\n");

                free(message);
            }
            else
            {
                printf("\nEXCEPTION ////\n\n");
                fini = 1;
            }
            printf("\n FIN ////\n\n");
        }

        TerminaisonClient();
    }

    return 0;
}