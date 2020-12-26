#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int extraitFichier(char *requete, size_t longueurRequete, char *nomFichier, size_t maxNomFichier){
    //Déclaration des variables
    int j = 0, i = 0, c = 0;
    char *fichier=malloc(sizeof(char)*maxNomFichier+1);

    //boucle pour parcourir le contenu de la requête
    for (i = 0; i < longueurRequete; i++){
        // On cherche le premier '/' pour déduire la position du premier caractère correspondant au nom du fichier
        if (requete[i] == '/'){
            // Si on trouve un espace directement après le '/' il s'agit du fichier par défaut "index.html",
            //on renvoie donc ce nom
            if(requete[i+1] == ' '){

                strcpy(nomFichier, "index.html");

                return 0;
            }
            // Sinon s'il y a un caractère qui suit le '/', il y a donc un nom de fichier,
            // on initialise c comme l'indice du premier caractère
            else {
                c = i + 1;
                break;
            }
        }
    }
    // on initialise i à la valeur de c
    i = c;
    // Boucle pour copier le nom de fichier de la requête
    // Tant qu'on ne détecte pas un espace on copie le caractère de l'indice courant dans fichier
    while (requete[i] != ' '){

        fichier[j] = requete[i];
        i++;
        j++;

    }
    
    fichier[j]='\0';
    // On copie fichier dans nomFichier afin que la fonction retourne le nom du fichier
    strcpy(nomFichier, fichier);

    free(fichier);
    return 0;

}

int main(){
    char bonjour[80]="";
    char requete[80]="GET /index.html HTTP/1.1";
    extraitFichier(requete,strlen(requete),bonjour,80);
    printf("test :%s",bonjour);
    return 0;
}