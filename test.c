#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    char requete[256]="GET /bonjour HTTP/1.1";
    printf("%s\n",requete);
    char com[256] = "";
    sscanf(requete, "%*[^GET /]/%s HTTP/", com);
    printf("%s fin",com);
    return 0;
}