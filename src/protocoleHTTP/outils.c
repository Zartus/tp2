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

/**
 * @brief On renvoie le type de l'extension
 * 
 * @param ext 
 * @return enum type retourn le type si il est pas connu renvoie UNKNOW
 */
enum type extension(char *ext)
{
    /*on crée un tableau avec toutes les extensions */
    char all[SIZETYPEMEDIA][32] = {TYPEMEDIA};

    /*on fait un tableau avec tout les types possible*/
    enum type enumALL[SIZETYPEMEDIA] = {enumeration};

    unsigned char i = 0;

    /*on parcour notre tableau avec les extensions pour voir si on la trouve*/
    for (; strcmp(all[i], ext) && i < SIZETYPEMEDIA; ++i)
        ;
    /*si on la trouve pas c'est que le type n'est pas connu*/
    if (i == SIZETYPEMEDIA)
        return UNKNOW;

    return enumALL[i];
}

enum type getExtension(RequeteStruct *r)
{
    char com[256] = "";
    char ext[256] = "";
    /*On extrait l'extension du nom du fichier*/
    sscanf(r->fichier, "%[^.].%s", com, ext);
    /*on retourne l'extension*/
    return extension(ext);
}

char *envoyerContenuFichierText(RequeteStruct *r)
{
    /*allocation de la mémoire pour le contenue du fichier*/
    char *content = NULL;
    if ((content = malloc(sizeof(char) * (longeurFichier(r) + 1))) == NULL)
    {
        /*Si probleme on l'affiche pour le serveur et on indique l'erreur 500*/
        perror("Probleme allocation de la mémoire");
        r->rep->numeroReponse = envoyerReponse500;
        /*et on revoie NULL*/
        return NULL;
    }

    FILE *fichier;
    int i = 0;

    //Ouverture du fichier à copier et affichage d'une erreur si impossible
    if ((fichier = fopen(r->fichier, "rt")) == NULL)
    {
        perror("Probleme à l'ouverture de l'fichier : ");
        r->rep->numeroReponse = envoyerReponse404; /*on indique l'erreur 404*/

        return NULL;
    }
    /*on copie tout le contenue de notre fichier dans la mémoire alloué*/
    for (; (*(content + i) = fgetc(fichier)) != EOF; ++i)
        ;
    //on rejoute \0
    *(content + i) = '\0';

    /*on ferme le fichier*/
    if (fclose(fichier) == EOF)
    {
        /*on indique l'erreur au serveur*/
        perror("probleme fermeture fichier");
        /*on initialise l'erreur 500*/
        r->rep->numeroReponse = envoyerReponse500;
        /*Et on renvoie NULL*/
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
        perror("Probleme à l'ouverture de l'fichier: ");
        r->rep->numeroReponse = envoyerReponse404;
        return NULL;
    }

    taille = longeurFichier(r);
    char *content = NULL;
    if((content=malloc(taille * sizeof(char)))==NULL){
        perror("Probleme à l'allocation de la mémoire");
        /*On initiailise l'erreur 500*/
        r->rep->numeroReponse = envoyerReponse500;
        /*On renvoie NULL*/
        return NULL;
    }
    //on se place au début du fichier
    fseek(fichier, 0, SEEK_SET);
    
    /*on copie le contenue du fichier binaire dans la variable*/
    if (!fread(content, taille, 1, fichier))
    {
        fprintf(stderr, "Probleme à la lecture.");
        r->rep->numeroReponse = envoyerReponse500;
        fclose(fichier);//pas besoin de faire plus si probleme car déja probleme
        return NULL;
    }
    //on place la taille dans la structure
    r->rep->contentLength = taille;
  
    //on ferme le fichier
    if (fclose(fichier) == EOF)
    {
        perror("Probleme fermeture fichier");
        r->rep->numeroReponse = envoyerReponse500;
        return NULL;
    }
    //on renvoie notre variable allouer
    return content;
}