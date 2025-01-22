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
    ///handle_client(client_fd);
char buffer[BUFFER_SIZE];memset(buffer, 0, BUFFER_SIZE);
 

    printf("Entrez une commande (upload_<nom_du_fichier>, download_<nom_du_fichier>, list) : ");

    if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

     // Vérifier si la commande commence par "upload_"
        if (strncmp(buffer, "upload_", 7) == 0) {
            // Récupérer le nom du fichier après "upload_"
            char *filename = buffer + 7;
            printf("Commande 'upload' avec fichier: %s\n", filename);
            // Envoyer la commande au serveur avec le nom du fichier
            if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi de la commande au serveur");
                
            }
        }

        // Vérifier si la commande commence par "download_"
        else if (strncmp(buffer, "download_", 9) == 0) {
            // Récupérer le nom du fichier après "download_"
            char *filename = buffer + 9;
            printf("Commande 'download' avec fichier: %s\n", filename);
            // Envoyer la commande au serveur avec le nom du fichier
            if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi de la commande au serveur");
               
            }
        }


        // Commande 'list'
        else if (strncmp(buffer, "list", 4) == 0) {
    printf("Commande 'list' envoyée au serveur.\n");

    // Envoyer la commande 'list' au serveur
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
        if (strstr(buf, "FIN_LISTE") != NULL) {
            break; // Fin de la liste
        }
    }

    if (donne_recus < 0) { perror("Erreur lors de la réception des données");}

        // en cas de couille
} else {printf("Erreur lors de la lecture de la commande.\n");}

    close(client_fd);

    return EXIT_SUCCESS;

}
