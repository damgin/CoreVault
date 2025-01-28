#include "global.h"


int main(){

    /**
     * socket
     * Je crée le socket client
     */
    int client_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    // Si la création échoue je ferme mon programme
    if(client_fd == -1) return EXIT_FAILURE;

    /**
     * bind
     * Je relie le socket à un port et une ip avec la fonction bind()
     */
    struct sockaddr_in client_addr = {
        .sin_addr.s_addr = INADDR_ANY,
        .sin_family = AF_INET,
        .sin_port = htons(CLIENT_PORT)
    };
    int error = bind(client_fd,(struct sockaddr*)&client_addr,sizeof client_addr);perror("bind");
    if(error == -1) { close(client_fd); return EXIT_FAILURE; }



    /**
     * connect
     * Je connecte mon socket client au socket server situé en 127.0.0.1:SERVER_PORT
     */
    struct sockaddr_in server_addr = {
        .sin_addr.s_addr = inet_addr("127.0.0.1"), // Attention à ne pas mettre INADDR_ANY !
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT)
    };
    error = connect(client_fd,(struct sockaddr*)&server_addr,sizeof server_addr);perror("connect");
    if(error == -1) { close(client_fd); return EXIT_FAILURE; }

    // SOCKET CLIENT PRET A COMMUNIQUER !

printf("\n"
"\n"
"\n"
"\n");
printf(
        "   ██████╗  ██████╗ ██████╗ ███████╗    ██╗   ██╗ █████╗ ██╗   ██╗██╗     ████████╗\n"
        "  ██╔════╝ ██╔═══██╗██╔══██╗██╔════╝    ██║   ██║██╔══██╗██║   ██║██║     ╚══██╔══╝\n"
        "  ██║      ██║   ██║██████╔╝█████╗      ██║   ██║███████║██║   ██║██║        ██║   \n"
        "  ██║      ██║   ██║██╔██   ██╔══╝      ██║   ██║██╔══██║██║   ██║██║        ██║   \n"
        "  ╚██████  ╚██████╔╝██║ ██  ███████╗    ╚██████╔╝██║  ██║╚██████╔╝███████╗   ██║   \n"
        "   ╚═════╝  ╚═════╝ ╚═╝ ╚═╝ ╚══════╝     ╚═════╝ ╚═╝  ╚═╝ ╚═════╝ ╚══════╝   ╚═╝   \n"
        "\n"
        "  Bienvenue sur Core Vault - Votre plateforme sécurisée de stockage en ligne.\n"
        "\n"
        "  [1] Créer un compte\n"
        "  [2] Se connecter\n"
        "  [3] À propos\n"
        "  [4] Quitter\n"
        "\n"
        "  Veuillez saisir votre choix : "
    );

    
    //buffer pour stocker ma commande 
    char buffer[BUFFER_SIZE];memset(buffer, 0, BUFFER_SIZE);
 

    printf("Entrez une commande (upload_<nom_du_fichier>, download_<nom_du_fichier>, list) : ");

    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

     // Vérifier si la commande commence par "upload_"
if (strncmp(buffer, "upload_", 7) == 0) {///  WORK IN PROGRESS
    // Récupérer le nom du fichier après "upload_"
    char *nom_fichier = buffer + 7;
    printf("Commande 'upload' avec fichier: %s\n", nom_fichier);
    // Envoyer la commande au serveur avec le nom du fichier
    if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi de la commande au serveur");
    }

    // Ouvrir le fichier local pour le lire
    FILE *file = fopen(nom_fichier, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier local");
        close(client_fd);
        return EXIT_FAILURE;
    }

    // Buffer pour lire les données du fichier
    char buf_fichier[BUFFER_SIZE];
    ssize_t taille_lue;

    printf("Téléchargement du fichier : %s\n", nom_fichier);

    // Lire et envoyer le fichier en morceaux
    while ((taille_lue = fread(buf_fichier, 1, sizeof(buf_fichier), file)) > 0) {
        if (send(client_fd, buf_fichier, taille_lue, 0) == -1) {
            perror("Erreur lors de l'envoi du fichier au serveur");
            fclose(file);
            close(client_fd);
            return EXIT_FAILURE;
        }
    }

    if (taille_lue < 0) {
        perror("Erreur lors de la lecture du fichier");
    }

    // Indicateur de fin de fichier
    const char *fin_transfert = "FIN_FICHIER\n";
    if (send(client_fd, fin_transfert, strlen(fin_transfert), 0) == -1) {
        perror("Erreur lors de l'envoi de l'indicateur de fin de fichier au serveur");
    }

    fclose(file);
    printf("Upload terminé.\n");
}


        // Vérifier si la commande commence par "download_"
else if (strncmp(buffer, "download_", 9) == 0) {
    char *nom_fichier = buffer + 9;
    printf("Commande 'download' avec fichier: %s\n", nom_fichier);

    if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
        perror("Erreur lors de l'envoi de la commande au serveur");
    }

    // Réception des données de la part du serveur.
    char buf_fichier[BUFFER_SIZE];
    ssize_t donne_recus;

    // Ouvrir un fichier local pour écrire les données reçues
    FILE *file = fopen(nom_fichier, "wb");
    if (!file) {
        perror("Erreur d'ouverture du fichier local");
        close(client_fd);
        return EXIT_FAILURE;
    }

    printf("Téléchargement du fichier : %s\n", nom_fichier);
    // char* donner_total = recv (donne_lue)
    while ((donne_recus = recv(client_fd, buf_fichier, sizeof(buf_fichier), 0)) > 0) {
        // Vérifier la fin du fichier

        //Strstr renvoie un pointeur sur le début de la sous chaine ou 0 si il la trouve pas, bonne chance pour coder ca
        if (strstr(buf_fichier, "FIN_FICHIER") != NULL) {   ///si je trouve pas la sous chaine je continue a fwrite ! gg :!
            //urgent trouver comment suprimer le marquer 
            fwrite(buf_fichier, 1, donne_recus - strlen("FIN_FICHIER"), file); // on supprime la sous chaine pour éviter les soucis
            break;
        }

        fwrite(buf_fichier, 1, donne_recus, file);
    }

    if (donne_recus < 0) {
        perror("Erreur lors de la réception du fichier");
    }

    fclose(file);
    printf("Téléchargement terminé.\n");
}


        // Commande 'list'
        else if (strncmp(buffer, "list", 4) == 0) {
            printf("Commande 'list' envoyée au serveur.\n");

    // Envoyer la string "list" au serveur
        if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
            perror("Erreur lors de l'envoi de la commande au serveur");
            return EXIT_FAILURE;
        }

    // Réception des données
    char buf[BUFFER_SIZE];
    ssize_t donne_recus;
    printf("Liste des fichiers et répertoires :\n");

        while ((donne_recus = recv(client_fd, buf, sizeof(buf) - 1, 0)) > 0) {
            buf[donne_recus] = '\0'; // Assurez-vous que c'est une chaîne C valide
            printf("%s", buf);

                // Vérifier si le serveur indique la fin de la liste
                if (strstr(buf, "FIN_LISTE") != NULL) {//// aregarder 
                    break; // enfin la  fin de la liste
                }
        }

        if (donne_recus < 0) { perror("Erreur lors de la réception des données");}

        // en cas de couille
        } else {printf("Erreur lors de la lecture de la commande.\n");}
            close(client_fd);
            return EXIT_SUCCESS;
        }
}
