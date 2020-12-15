#include "protocoleHTTP.h"
#include "requete.h"

size_t longeurFichier(RequeteStruct *r)
{
    //les diffentes variables
    FILE *file;
    /*size_t car un fichier ne peut pas étre de taille négative*/
    /*donc on peut monter beaucoup plus haut en taille*/
    size_t size = 0;

    /*On ouvre le fichier présent dans la requete en lecture*/
    if ((file = fopen(r->fichier, "rb")) == NULL)
    {
        /*si le fichier n'arrive pas à étre ouvert*/
        /*existe pas ou pas acces*/
        perror("ouverture du fichier rater\n");
        /*on envoie au client l'erreur 404*/
        r->rep->numeroReponse = envoyerReponse404;
        return 0;
    }
    //on se positionne à la fin du fichier
    fseek(file, 0, SEEK_END);
    //on regarde la position à la fin
    size = ftell(file);

    /*on ferme le fichier*/
    if (fclose(file) == EOF)
    {
        /*si on arrive pas à fermer le fichier erreur serveur*/
        /*car il y a possiblement d'autre erruer que celle ci*/
        /*donc prefere envoyer au client une erreur 500*/
        perror("probleme fermeture du fichier\n");
        r->rep->numeroReponse = envoyerReponse500;
        return 0;
    }
    /*si tout c'est bien passé on retourne la taille du fichier*/
    return size;
}

enum type extension(char *ext)
{
    char all[SIZETYPEMEDIA][32] = {TYPEMEDIA};
    enum type enumALL[SIZETYPEMEDIA] = {HTML, JPEG, JPEG, ICO, UNKNOW};

    unsigned char i = 0;

    for (; strcmp(*(all + i), ext) && i < SIZETYPEMEDIA; ++i)
        ;

    return enumALL[i];
}

enum type getExtension(RequeteStruct *r)
{
    char com[256] = "";
    char prev[256] = "";
    sscanf(r->fichier, "%[^.].%s", com, prev);

    return extension(prev);
}

char *envoyerContenuFichierText(RequeteStruct *r)
{
    //peut etre truc à revoir ici +1 +4 ??
    char *content = malloc(sizeof(char) * (longeurFichier(r) + 1));
    FILE *fichier;
    int i = 0;

    //Ouverture du fichier à copier et affichage d'une erreur si impossible
    if ((fichier = fopen(r->fichier, "rt")) == NULL)
    {
        perror("Probleme à l'ouverture de l'fichier 2: ");
        r->rep->numeroReponse = envoyerReponse404;

        return NULL;
    }

    for (; (*(content + i) = fgetc(fichier)) != EOF; ++i)
        ;

    *(content + i) = '\0';

    if (fclose(fichier) == EOF)
    {
        perror("probleme fermeture fichier");
        r->rep->numeroReponse = envoyerReponse500;
        return NULL;
    }

    return content;
}

char *envoyerContenuFichierBinaire(RequeteStruct *r)
{

    FILE *fichier;
    size_t taille;

    //Ouverture du fichier à copier et affichage d'une erreur si impossible
    if ((fichier = fopen(r->fichier, "r+b")) == NULL)
    {
        perror("Probleme à l'ouverture de l'fichier 2: ");
        r->rep->numeroReponse = envoyerReponse404;
        return NULL;
    }

    taille = longeurFichier(r);
    char *content = malloc(taille * sizeof(char));

    fseek(fichier, 0, SEEK_SET);

    if (!fread(content, taille, 1, fichier))
    {
        fprintf(stderr, "Probleme à la lecture.");
        r->rep->numeroReponse = envoyerReponse500;
    }

    r->rep->contentLength = taille;

    if (fclose(fichier) == EOF)
    {
        perror("Probleme fermeture fichier");
        r->rep->numeroReponse = envoyerReponse500;
        return NULL;
    }

    return content;
}