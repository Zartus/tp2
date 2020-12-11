#include "serveur.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//voir pour pas essayer de mettre des const

/**
 * @brief Fonction interface qui envoie le message à l'api fournis par les professeurs pour le TP2
 * 
 * @param message le message à envoyer
 * @param contentLength la taille du message
 * @param binaire si 1 alors c'est un message binaire si 0 UTF8
 * @return int 
 */
int emissionChoice(char *message,size_t contentLength,int binaire){
    
    if(binaire)
        return EmissionBinaire(message,contentLength);
    
    return Emission(message);
}

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

                repondre(r,emissionChoice);

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