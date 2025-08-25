Pour lancer le programme :
-Compiler le programme -> make dans le dossier où se situe le Makefile
-Lancer avec -> ./ircserv <port> <password>

Pour se connecter :

- Lancer nc -> nc 127.0.0.1
- Commencer les commandes :
  
\-\> PASS \<password\>\n
\-\> NICK \<nickname\>\n
\-\> USER \<name\> 0 * \<othername\>

*PASS*
-Permet d'être authentifier par le serveur et permet par la suite de créer l'utilisateur

*NICK*
-Enregistrer son nickname.

*USER*
-Enregistrer un nom unique ou un second nom suivant les clients, la commande donnée et celle pour les paramètres par défaut de Hexchat.

Liste des commandes :
-PRIVMSG
-JOIN
-MODE
-KICK
-TOPIC
-INVITE
-PART
-WHO

*PRIVMSG*
-Si on écrit :
  -"PRIVMSG #<NameOfChannel> <message>" alors on l'envoie sur un channel/salon
  -"PRIVMSG <User> <message>" alors on l'envoie à un utilisateur en privé

*JOIN*
-Permet de rejoindre un salon ou de le créer si il n'existe pas:
  -"Join #myChannel" Si il existe déjà, on peut ajouter des options comme un mot de passe pour le rejoindre si c'est demandé

