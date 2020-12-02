#include "protocoleHTTP.h"

/**
 * @brief Ouverture du fichier avec le mode indiquer
 *
 * @param fileName le nom du fichier
 * @param s le mode
 * @return FILE* le pointeur de la structure
 */
FILE *ouvertureFichier(char *fileName, char *s)
{
    FILE *fp;
    if ((fp = fopen(fileName, s)) == NULL)
    {
        perror("Probleme à l'ouverture du fichier");
        return NULL;
    }
    return fp;
}

/**
 * @brief Permet la fermeture du fichier
 *
 * @param f le fichier à fermer
 */
int fermetureFichier(FILE *f)
{
    if (fclose(f) == EOF)
    {
        perror("Probleme à la fermeture du fichier");
        return 0;
    }
    return 1;
}

//vois si pas probleme du à taille 0 me fier à la réponse !
size_t longeurFichier(RequeteStruct *r)
{
    FILE *file;
    size_t size = 0; //ir gerer erreur -1
    if ((file = ouvertureFichier(r->fichier, "r")) == NULL)
    {
        perror("ouverture du fichier rater\n");
        r->rep->numeroReponse = envoyerReponse404;
        return 0;
    }
    //on se positionne à la fin
    fseek(file, 0, SEEK_END);
    //on regarde la posiition
    size = ftell(file);

    if (!fermetureFichier(file))
    {
        perror("probleme fermeture du fichier\n");
        r->rep->numeroReponse = envoyerReponse500;
        return 0;
    }

    return size;
}

//on stocke EOF?// truc à voir bizarre je trouve
char *getExtension(RequeteStruct *r)
{
    char *content = NULL;
    char com[256] = "";
    char prev[256] = "";
    sscanf(r->fichier, "%[^.].%s", com, prev);
    //+4 car EOF fait 4
    content = malloc(sizeof(char) * (strlen(prev) + 4));
    strcpy(content, prev);
    return content;
}

char *envoyerContenuFichierText(RequeteStruct *r)
{
    //peut etre truc à revoir ici +1 +4 ??
    char *content = malloc(sizeof(char) * (longeurFichier(r) + 1));
    FILE *fichier;
    char ch;
    int i = 0;

    //Ouverture du fichier à copier et affichage d'une erreur si impossible
    if ((fichier = fopen(r->fichier, "rt")) == NULL)
    {
        perror("Probleme à l'ouverture de l'fichier 2: ");
        r->rep->numeroReponse = envoyerReponse404;

        return NULL;
    }

    while ((ch = fgetc(fichier)) != EOF)
    {
        content[i] = ch;
        ++i;
    }

    content[i] = '\0';

    if (!fermetureFichier(fichier))
    {
        perror("probleme fermeture fichier");
        r->rep->numeroReponse = envoyerReponse500;
        return NULL;
    }

    return content;
}

//vois si pas probleme du à taille 0 me fier à la réponse !
size_t longeurFichierBinaire(RequeteStruct *r)
{
    FILE *file;
    size_t size = 0; //ir gerer erreur -1
    if ((file = ouvertureFichier(r->fichier, "rb")) == NULL)
    {
        perror("ouverture du fichier rater\n");
        r->rep->numeroReponse = envoyerReponse404;
        return 0;
    }
    //on se positionne à la fin
    fseek(file, 0, SEEK_END);
    //on regarde la posiition
    size = ftell(file);
    printf("la taille est de : %ld\n\n\n\n", size);
    if (!fermetureFichier(file))
    {
        perror("probleme fermeture du fichier\n");
        r->rep->numeroReponse = envoyerReponse500;
        return 0;
    }

    return size;
}

char *envoyerContenuFichierBinaire(RequeteStruct *r)
{
    //peut etre truc à revoir ici +1 +4 ??

    FILE *fichier;

    //Ouverture du fichier à copier et affichage d'une erreur si impossible
    if ((fichier = fopen(r->fichier, "r+b")) == NULL)
    {
        perror("Probleme à l'ouverture de l'fichier 2: ");
        r->rep->numeroReponse = envoyerReponse404;
        return NULL;
    }

    fseek(fichier, 0, SEEK_END); //go to end
    //char *buffer = malloc(size * sizeof(char)); //allocation dynamique
    int taille = ftell(fichier);
    printf("taille");
    char *content = (char *)malloc(taille * sizeof(char(*)));
    fseek(fichier, 0, SEEK_SET);        //go to beg.
    if(fread(content, taille, 1, fichier)){
        printf("bonjour");
    } //read into buffer
    r->rep->contentLength=taille;

    if (!fermetureFichier(fichier))
    {
        perror("probleme fermeture fichier");
        r->rep->numeroReponse = envoyerReponse500;
        return NULL;
    }

    return content;
}