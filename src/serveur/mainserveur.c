#include "serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//voir pour pas essayer de mettre des const
//un null qui arrive quand fermeture go voir pourquoi
//traiter caractere speciaux ?
//revoir la partie sur l'envoie
//verifier que tout les cas de mauvaise requetes sont gerer
//gerrer les autres erreurs
//mettre en place fichier de test insipiration => tp de l'année derniere
//amélioration de envoyer contenue fichier
//amélioration tableau contentRenvoieType
//modifier retour des reponses envoyer
//mettre en place un .h avec tous les defines de configuration
//revoir valeur de base pour enumType
//mettre un sicth pour les differentes commandes et mettre en place l'ajout avec un define
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
            message = Reception();

            if (message != NULL)
            {
                printf("J'ai recu: %s\n", message);
                
                r=annalyseRequete(message);

                if(!repondre(r,emissionChoice))
                    fprintf(stderr,"Il y a eu un probleme\n");        

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