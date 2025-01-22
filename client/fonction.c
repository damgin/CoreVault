#pragma once
#include "global.h"


void download_file(int client_fd, const char *filename){
    printf("téléchargement effectuer");
    
}

void list_file(int client_fd){
    printf("voici la list");
   
}
void upload_file(int client_fd, const char *filename){
    printf("envoie effectuer");
}


void handle_client(int client_fd) {
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
                return;
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
                return;
            }
        }


        // Commande 'list'
        else if (strncmp(buffer, "list", 4) == 0) {
            printf("Commande 'list' envoyée au serveur.\n");
            // Envoyer la commande 'list' au serveur
            if (send(client_fd, buffer, strlen(buffer), 0) < 0) {
                perror("Erreur lors de l'envoi de la commande au serveur");
                return;
            }
        }

        // en cas de couille
        else {
            printf("Commande non reconnue : %s\n", buffer);
        }
    } else {
        printf("Erreur lors de la lecture de la commande.\n");
    }
}
