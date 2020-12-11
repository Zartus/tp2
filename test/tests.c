#include <stdio.h>
#include <stdlib.h>

void test(char *requete){
    (void)requete;
    //printf("Test avec un fichier extraire le nom du fichier +connaitre sa taille !\n");

    /*on verifie le format*/
    //Requete req= annalyseRequete(requete);

    /*on recupere le nom du fichier à traiter*/
    //printf("tout les fichiers sont dans le repertoire fichier donc :\n");

    //printf("Le nom du fichier à traiter est : %s\n",req->fichier);
    /*on recupere la longeur du fichier*/
    //printf("La taille du fichier : ");
    //printf("%ld octets.\n",longeurFichier(req));

    /*on libere la mémoire*/
    //freeRequete(req);
}

int main(){
    int *test=malloc(sizeof(int)*10);
    int i=0;

    while(i<10){
        *test++;
        test=i;
        ++i;
    }
    for(int i=0;i<10;i++)
        printf("%d\n",test[i]);
    return 0;
}