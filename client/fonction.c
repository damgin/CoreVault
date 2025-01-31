#pragma once
#include "global.h"


void download(int client_fd,char*user_input){

char *file_name = user_input + 9;
    printf("Commande 'download' avec fichier: %s\n", file_name);

    if (send(client_fd, user_input, strlen(user_input), 0) < 0) {
        perror("Erreur lors de l'envoi de la commande au serveur");
    }

    // Réception des données de la part du serveur.
    char buf_fichier[BUFFER_SIZE];
    ssize_t data_recv;

    // Ouvrir un fichier local pour écrire les données reçues
    FILE *file = fopen(file_name, "wb");
    if (!file) {
        perror("Erreur d'ouverture du fichier local");
        close(client_fd);
        return ;
    }

    printf("Téléchargement du fichier : %s\n", file_name);
    // char* donner_total = recv (donne_lue)
    while ((data_recv = recv(client_fd, buf_fichier, sizeof(buf_fichier), 0)) > 0) {
        // Vérifier la fin du fichier

        //Strstr renvoie un pointeur sur le début de la sous chaine ou 0 si il la trouve pas, bonne chance pour coder ca
        if (strstr(buf_fichier, "FIN_FICHIER") != NULL) {   ///si je trouve pas la sous chaine je continue a fwrite ! gg :!
            fwrite(buf_fichier, 1, data_recv - strlen("FIN_FICHIER"), file); // on supprime la sous chaine pour éviter les soucis
            break;
        }

        fwrite(buf_fichier, 1, data_recv, file);
    }

    if (data_recv < 0) {
        perror("Erreur lors de la réception du fichier");
    }

    fclose(file);
    printf("Téléchargement terminé.\n");
}

void upload(int client_fd,char*user_input){
    // Récupérer le nom du fichier après "upload_"
    char *file_name = user_input + 7;
    printf("Commande 'upload' avec fichier: %s\n", file_name);
    // Envoyer la commande au serveur avec le nom du fichier
    if (send(client_fd, user_input, strlen(user_input), 0) < 0) {
        perror("Erreur lors de l'envoi de la commande au serveur");
    }

    // Ouvrir le fichier local pour le lire
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier local");
        close(client_fd);
        return ;
    }

    // user_input pour lire les données du fichier
    char buf_fichier[BUFFER_SIZE];
    ssize_t data_lenght;

    printf("Téléchargement du fichier : %s\n", file_name);

    // Lire et envoyer le fichier en morceaux
    while ((data_lenght = fread(buf_fichier, 1, sizeof(buf_fichier), file)) > 0) {
        if (send(client_fd, buf_fichier, data_lenght, 0) == -1) {
            perror("Erreur lors de l'envoi du fichier au serveur");
            fclose(file);
            close(client_fd);
            return ;
        }
    }

    if (data_lenght < 0) {
        perror("Erreur lors de la lecture du fichier");
    }

    // Indicateur de fin de fichier
    const char *end_marker = "FIN_FICHIER\n";
    if (send(client_fd, end_marker, strlen(end_marker), 0) == -1) {
        perror("Erreur lors de l'envoi de l'indicateur de fin de fichier au serveur");
    }

    fclose(file);
    printf("Upload terminé.\n");
}

void list(int client_fd,char*user_input){
    printf("Commande 'list' envoyée au serveur.\n");

    // Envoyer la string "list" au serveur
        if (send(client_fd, user_input, strlen(user_input), 0) < 0) {
            perror("Erreur lors de l'envoi de la commande au serveur");
            return ;
        }

    // Réception des données
    char buf[BUFFER_SIZE];
    ssize_t data_recv;
    printf("Liste des fichiers et répertoires :\n");

        while ((data_recv = recv(client_fd, buf, sizeof(buf) - 1, 0)) > 0) {
            buf[data_recv] = '\0'; // Assurez-vous que c'est une chaîne C valide
            printf("%s", buf);

                // Vérifier si le serveur indique la fin de la liste
                if (strstr(buf, "FIN_LISTE") != NULL) {//// aregarder 
                    break; // enfin la  fin de la liste
                }
        }

        if (data_recv < 0) { perror("Erreur lors de la réception des données");}
}
