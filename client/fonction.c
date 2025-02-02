#pragma once
#include "global.h"


void download(int client_fd,char*user_input){

char *file_name = user_input + 9;
    printf("Commande 'download' avec fichier: %s\n", file_name);

    if (send(client_fd, user_input, strlen(user_input), 0) < 0) {
        perror("Erreur lors de l'envoi de la commande au serveur");
    }

    // Réception des données de la part du serveur.
    char file_buffer[BUFFER_SIZE];
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
    while ((data_recv = recv(client_fd, file_buffer, sizeof(file_buffer), 0)) > 0) {
        // Vérifier la fin du fichier

        //Strstr renvoie un pointeur sur le début de la sous chaine ou 0 si il la trouve pas, bonne chance pour coder ca
        if (strstr(file_buffer, "FIN_FICHIER") != NULL) {   ///si je trouve pas la sous chaine je continue a fwrite ! gg :!
            fwrite(file_buffer, 1, data_recv - strlen("FIN_FICHIER"), file); // on supprime la sous chaine pour éviter les soucis
            break;
        }

        fwrite(file_buffer, 1, data_recv, file);
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

    // Envoyer la commande au serveur
    if (send(client_fd, user_input, strlen(user_input), 0) < 0) {
        perror("Erreur lors de l'envoi de la commande au serveur");
        return;
    }
    // Ouvrir le fichier pour le lire
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier local");
        return;
    }

    char file_buffer[BUFFER_SIZE];
    ssize_t data_lenght;

    printf("Envoi du fichier : %s\n", file_name);
    //boucle qui lis le fichier puis le send
    while ((data_lenght = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0) {
        if (send(client_fd, file_buffer, data_lenght, 0) == -1) {
            perror("Erreur lors de l'envoi du fichier au serveur");
            fclose(file);
            return;
        }
    }

    // Envoyer la fin de fichier
    const char *end_marker = "FIN_FICHIER\n";
    send(client_fd, end_marker, strlen(end_marker), 0);

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
