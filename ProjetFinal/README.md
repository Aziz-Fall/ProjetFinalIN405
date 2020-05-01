- Le programme est découpé en 3 parties:
    - La gestion de la mémoire: bibliothèque memory_manager.h et  list_page.h ( cette structure gére la page le moins récent ).
    - La communication (envoie et reception de données): bibliothèque communication.h   
    - Le programme principale.

- L'exécution du programme se fait via la commande make. 
  Le résultat du programme est redirectionné dans un fichier nommé result.txt. La commande make clean supprimer le fichier exécutabe ainsi que le fichier qui contient le résultat du programme.

- Le programme utilise la structure de donnée Communication 
  pour gérer l'exclusion mutex entre les threads fils au niveau de l'accéss aux tubes (nommés) de communications. Le nom du fichier de configuration nommé data est passé en argument dans le main.   

- Pour l'argorithme LRU j'ai utilisé une liste chainée (voir la    
  bibliothèque list_pages.h ) pour gérer la page le moins récent. Lorqu'une nouvelle page est ajoutée dans le frame, elle est mise au début de la liste et si une page demandée est déjà dans le frame cette page sera mise en tête de liste. Avec cette liste chainée la page le moins récent se trouve toujours en fin de liste et elle est récupée par la fonction get_least_recent_page(...). 

- Chaque thread fils affiche le message qu'il a reçu et à la
  fin de l'exécution du programme le thread parent affiche le pourcentage de hits de chaque thread fils.