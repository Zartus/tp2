1ère Année STRI – André Aoun, Patrice Torguet  TP Langage C 

TP – Protocole http (version de base) ![](TP\_Protocole\_HTTP\_base.001.png)

But du TP : Développer le protocole http (version de base) en utilisant une bibliothèque de fonctions permettant d’échanger des messages applicatifs au dessus d’une connexion TCP en mode client/serveur. 

1) Récupérez la bibliothèque de fonctions client/serveur (client.h , client.c et serveur.h, serveur.c) ; 
1) Récupérez les programmes d’exemples (mainClient.c et mainServeur.c) ; 
1) Créez un projet client et ajoutez-lui les fichiers du client ; 
1) Créez un projet serveur et ajoutez-lui les fichiers du serveur ; 
1) Compilez les deux projets et exécutez le serveur puis le client. 
1) Utilisez la commande telnet pour observer le fonctionnement du protocole HTTP (voir l’annexe ci-dessous). 
1) Modifiez le main du client pour qu’il se connecte à un serveur web, lui envoie une requête HTTP bien formatée et affiche la réponse HTTP. 
1) Modifiez le serveur pour le transformer en mini serveur web. Dans un premier temps on ne gèrera que les requêtes GET pour des fichiers textes formatés en HTML. 
1. Ecrivez une fonction qui permet d’extraire d’une requête HTTP contenue dans une chaine de caractères (passée en paramètre), le nom du fichier demandé : **int extraitFichier(char \*requete, size\_t longueurRequete, char \*nomFichier, size\_t maxNomFichier) ;** 
1. Ecrivez une fonction qui permet de calculer la longueur d’un fichier. Cf. TP 2 : **size\_t longueur\_fichier(char \*nomFichier) ;** 
1. Ecrivez une fonction qui permet d’envoyer à un client le contenu d’un fichier texte dont le nom est passé en paramètre de la fonction. **int envoyerContenuFichierTexte(char \*nomFichier) ;** 
1. Ecrivez une fonction qui permet d’envoyer une réponse HTTP de type « 200 OK » et les entêtes minimales pour qu’un  client  puisse  afficher  une  page  HTML.  Note :  on  enverra  au  minimum  les  entêtes  « Content-type : text/html » et « Content-length : <longueur du fichier> ». **int envoyerReponse200HTML(char \*nomFichier) ;** 
1. Ecrivez une fonction qui permet d’envoyer une réponse HTTP de type « 404 Not Found », les entêtes minimales et une page HTML simple indiquant le nom du fichier qui n’a pas pu être trouvé. Note : on enverra au minimum les  entêtes  « Content-type :  text/html »  et  « Content-length :  <longueur  de  la  page  d’erreur> ».  **int envoyerReponse404(char \*nomFichier) ;** 
1. Ecrivez  une  fonction  qui  permet  d’envoyer  une  réponse  HTTP  de  type  « 500  Server  Error»,  les  entêtes minimales et une page HTML simple indiquant un message d’erreur. Note : on enverra au minimum les entêtes 
   1. Content-type :  text/html »  et  « Content-length :  <longueur  de  la  page  d’erreur> ».  **int envoyerReponse500(char \*message) ;** 
1. Modifiez le main du serveur pour qu’il reçoive une requête HTTP, vérifie que c’est bien une requête GET, en extraie le nom du fichier demandé, vérifie si le fichier existe et renvoie une réponse HTTP adéquate (soit « 200 OK », soit « 404 Not Found », soit « 500 Server Error ») au client. 
9) Tapez une page HTML simple. 
9) Testez votre serveur en utilisant un navigateur web comme Konqueror ou Firefox pour récupérer le page HTML simple. 
9) Modifiez le serveur pour qu’il puisse renvoyer des images JPG. 
1. Ecrivez une fonction qui permet d’envoyer à un client le contenu d’un fichier binaire dont le nom est passé en paramètre de la fonction. **int envoyerContenuFichierBinaire(char \*nomFichier) ;** 
1. Ecrivez une fonction qui permet d’envoyer une réponse HTTP de type « 200 OK » et les entêtes minimales pour qu’un client puisse afficher une image JPG. Note : on enverra au minimum les entêtes « Content-type : text/jpg» et « Content-length : <longueur du fichier> ». **int envoyerReponse200JPG(char \*nomFichier) ;** 
1. Modifiez le main pour gérer les images JPG. 
12) Testez votre serveur en utilisant un navigateur web comme Konqueror ou Firefox pour récupérer une image JPG. 
12) Modifiez le serveur pour qu’il puisse renvoyer des images ICO. 
1. Ecrivez une fonction qui permet d’envoyer une réponse HTTP de type « 200 OK » et les entêtes minimales pour qu’un  client  puisse  afficher  une  image  ICO.  Note :  on  enverra  au  minimum  les  entêtes  « Content-type : image/vnd.microsoft.icon » et « Content-length : <longueur du fichier> ». **int envoyerReponse200ICO(char \*nomFichier) ;** 
1. Modifiez le main pour gérer les images ICO. 
14) Testez votre serveur en utilisant un navigateur web comme Konqueror ou Firefox pour récupérer une image ICO. 

Annexe : **Le protocole HTTP** 

Le client ouvre une connexion TCP avec le serveur (en general sur le port 80) et lui envoie une requête HTTP. Le serveur reçoit la requête, la traite et répond avec une réponse HTTP. 

Le format de la requête HTTP est le suivant : 

Requête           =  Commande <uri> VersionDuProtocole <CR, LF>                           [Entêtes] 

`                          `<CR, LF> 

[CorpsDeLaRequête] 

VersionDuProtocole = HTTP/1.0 | HTTP/1.1 … 

URI (**U**niform **R**esource **I**dentifier) : chaîne identifiant l’objet (par exemple une URL – Uniform Resource Locator). 

**principales commandes :** 

**GET**                  :     demande l'envoi des données identifiées par l’URI. 

**HEAD**                 :              même chose que GET mais retourne seulement les entêtes HTTP sans le corps du document. **POST**                   :             permet l'envoi des données d'un formulaire. 

**PUT**                    :             transmet un fichier au serveur. 

Les entêtes ont le format : 

Mot-clé : Valeur<CR><LF> 

Par exemple le mot-clé Host permet de préciser le nom DNS du site web si la machine en gère plusieurs, Accept permet de préciser les types de données que sait gérer le navigateur Web, le mot-clé User-Agent permet de préciser le nom et la version du navigateur. 

Exemple de requête : 

`     `**GET /index.php HTTP/1.1** 

`     `Host : www.stri.ups-tlse.fr 

`     `Accept : text/html 

`     `Accept : text/plain 

`     `User-Agent : Lynx/2.4 libwww/2.1.4 

La réponse renvoyée par le serveur se compose d'une première ligne précisant la version du protocole, un code de réponse et un message précisant la réponse, suivent éventuellement des entêtes (avec le même format que pour la requête), une ligne vide et le corps de la réponse. 

Le format de la réponse HTTP est donc le suivant : 

Réponse =  VersionDuProtocole Code Message<CR, LF>                           [Entêtes] 

`                          `<CR, LF> 

[CorpsDeLaRéponse] 



|Code |Type |Signification |
| - | - | - |
|1xx |Informationnel |La commande a été reçue – le traitement est en cours. |
|2xx |Succès |La commande a été reçue correctement, comprise et acceptée. |
|3xx |Redirection |La commande ne peut pas être exécutée telle qu'elle il faudra faire autre chose pour la gérer totalement (par exemple aller chercher le document ailleurs). |
|4xx |Erreur client |La commande n'est pas correcte syntaxiquement ou ne peut pas être traitée. |
|5xx |Erreur serveur |Le serveur n'a pu traiter correctement une commande apparemment correcte. |
***Figure 1: types de code de réponse*** 

Exemple de réponse : 

`     `HTTP/1.0 200 OK 

`     `Server : NCSA/1.4.2 

`     `MIME-Version : 1.0 

`     `Content-type : text/html      Content-length : 97 

`     `<HTML>      <HEAD>      <TITLE> 

`     `Exemple de document HTML      </TITLE> 

`     `<HEAD> 

`     `<BODY> 

`     `(Corps du document) 

`     `</BODY> 

`     `</HTML> 

Pour  plus  de  détail  sur  HTTP  vous  pouvez  consulter  la  RFC  2616  qui  décrit  la  spécification  de  HTTP/1.1 (http://abcdrfc.free.fr/rfc-vf/rfc2616.htm). 

**Exemple d'observation avec Telnet** 

Pour examiner ce protocole nous allons nous connecter avec telnet sur le port 80 (port utilisé par les serveurs Web). Voici un exemple d'une transaction pour récupérer la page de garde du CICT. Tout ce qui est écrit en gras est tapé par l'utilisateur le reste est renvoyé par le serveur. 

***telnet www.stri.ups-tlse.fr 80*** 

Trying 195.220.59.62... 

Connected to lifou-www-nom-ups.cict.fr. Escape character is '^]'. 

***GET / HTTP/1.1*** 

***Host: www.stri.ups-tlse.fr*** 

***(Il faut taper 2 fois sur la touche entrée)*** 

HTTP/1.1 200 OK 

Date: Fri, 30 Sep 2011 07:46:02 GMT Server: Apache 

Transfer-Encoding: chunked Content-Type: text/html 

20bd 

<html> 

<head> 

<title>STRI - Télécoms & Réseaux </title> 

<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1"> <link rel="stylesheet" href="style\_site.css" type="text/css"> 

<script src="Scripts/swfobject\_modified.js" type="text/javascript"></script> <script language="JavaScript"> 

… 

… 

</body> 

</html> 

0*** 
