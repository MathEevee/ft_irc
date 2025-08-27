# IRC Server

Petit serveur IRC avec gestion des commandes de base.

---

## 🚀 Lancer le programme

1. **Compiler** le programme :
   ```bash
   make
   ```
   (dans le dossier où se situe le `Makefile`)

2. **Exécuter** le serveur :
   ```bash
   ./ircserv <port> <password>
   ```

---

## 🔗 Connexion

1. Lancer **netcat** :
   ```bash
   nc 127.0.0.1 <port>
   ```

2. Envoyer les commandes d’authentification dans l’ordre :
   ```text
   PASS <password>
   NICK <nickname>
   USER <name> 0 * <othername>
   ```

---

## 📝 Commandes supportées

| Commande   | Description | Exemple |
|------------|-------------|---------|
| **PASS**   | Authentifie l’utilisateur auprès du serveur. | `PASS mypassword` |
| **NICK**   | Définit le pseudonyme de l’utilisateur. | `NICK Alice` |
| **USER**   | Enregistre un nom unique (ou second nom selon le client). | `USER Alice 0 * AliceClient` |
| **PRIVMSG**| Envoie un message privé à un utilisateur ou sur un channel. | `PRIVMSG Bob Salut !`<br>`PRIVMSG #general Hello !` |
| **JOIN**   | Rejoint ou crée un salon. Peut nécessiter un mot de passe. | `JOIN #myChannel` |
| **MODE**   | Change le mode d’un utilisateur ou d’un salon. | `MODE #general +i` |
| **KICK**   | Expulse un utilisateur d’un salon. | `KICK #general Bob :Raison` |
| **TOPIC**  | Définit ou affiche le sujet du salon. | `TOPIC #general :Nouveau sujet` |
| **INVITE** | Invite un utilisateur dans un salon. | `INVITE Bob #general` |
| **PART**   | Quitte un salon. | `PART #general` |
| **WHO**    | Liste les utilisateurs connectés ou dans un salon. | `WHO #general` |

---

## ✅ Exemple d’utilisation

```text
PASS mypassword
NICK Alice
USER Alice 0 * AliceClient
JOIN #general
PRIVMSG #general Hello tout le monde !
```

---

## 📌 Notes

- Les commandes doivent respecter la **syntaxe IRC**.  
- Certaines fonctionnalités dépendent du client utilisé (ex : HexChat).  
