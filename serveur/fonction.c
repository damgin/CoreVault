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
   


void handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];

    // Lire la commande envoyée par le client
    memset(buffer, 0, BUFFER_SIZE);
    int data_reçus = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (data_reçus > 0) {
        buffer[data_reçus] = '\0';
        printf("Commande reçue: %s\n", buffer);

        // Analyser la commande
        if (strncmp(buffer, "upload", 6) == 0) {
            upload_file(client_fd, buffer + 7); // Envoyer le nom du fichier
        } else if (strncmp(buffer, "list", 4) == 0) {
            list_file(client_fd);
        } else if (strncmp(buffer, "download", 8) == 0) {
            download_file(client_fd, buffer + 9); // Envoyer le nom du fichier
        } else {
            const char *response = "Commande inconnue\n";
            send(client_fd, response, strlen(response), 0);
        }
    } else {
        perror("Erreur lors de la réception des données");
    }
}
}