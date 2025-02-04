#pragma once
#include "global.h"


    void download(int client_fd, const char *filename) {
        char chemin_acces_fichier[BUFFER_SIZE];
        snprintf(chemin_acces_fichier, sizeof(chemin_acces_fichier), "public/%s", filename); // Chemin du fichier

        // Ouverture du fichier en lecture seule
        int file_fd = open(chemin_acces_fichier, O_RDONLY);
        if (file_fd < 0) {
            perror("Erreur d'ouverture du fichier");
            char *erreur_msg = "ERREUR: Fichier introuvable\n";
            send(client_fd, erreur_msg, strlen(erreur_msg), 0);
        } else {
            char buf_fichier[BUFFER_SIZE];
            ssize_t donnees_lues;

            // Envoi du contenu du fichier au client
            while ((donnees_lues = read(file_fd, buf_fichier, sizeof(buf_fichier))) > 0) {
                if (send(client_fd, buf_fichier, donnees_lues, 0) == -1) {
                    perror("Erreur d'envoi du fichier");
                    break;
                }
            }

            if (donnees_lues < 0) perror("Erreur de lecture du fichier");

            close(file_fd);

            // Indicateur de fin de fichier
            const char *fin_transfert = "FIN_FICHIER";
            send(client_fd, fin_transfert, strlen(fin_transfert), 0);
        }

        close(client_fd);
    }

    void list_file(int client_fd) {
        char tampon[BUFFER_SIZE]; // Tampon pour stocker les données à envoyer
        struct dirent *lecteur_dossier;
        
        printf("Début de la fonction list_file\n");

        DIR *dir = opendir("public");
        if (dir == NULL) {
            perror("Impossible d'ouvrir le dossier");
            return;
        }

        while ((lecteur_dossier = readdir(dir)) != NULL) {
            printf("Lecture d'un fichier/dossier\n");

            if (lecteur_dossier->d_type == DT_REG) {
                snprintf(tampon, sizeof(tampon), "FILE: %s\n", lecteur_dossier->d_name);

            } else if (lecteur_dossier->d_type == DT_DIR) {
                snprintf(tampon, sizeof(tampon), "DIR: %s\n", lecteur_dossier->d_name);

            } else {
                continue; // Ignore les autres types de fichiers (liens symboliques, etc.)
            }

            printf("Tampon: %s", tampon);

            if (send(client_fd, tampon, strlen(tampon), 0) == -1) {
                perror("Erreur d'envoi au client");
                closedir(dir);
                return;
            }
        }

        closedir(dir); // Fermeture du dossier après la lecture complète

        // Envoi du message de fin
        const char *fin_liste = "FIN_LISTE\n";
        if (send(client_fd, fin_liste, strlen(fin_liste), 0) == -1) {
            perror("Erreur d'envoi de l'indicateur de fin au client");
        }

        printf("Fin de la fonction list_file\n");
    }

    void upload(int client_fd, const char *filename) {
    char chemin_acces_fichier[BUFFER_SIZE];
    snprintf(chemin_acces_fichier, sizeof(chemin_acces_fichier), "public/%s", filename);

    // Ouvrir le fichier en mode écriture binaire
    FILE *file = fopen(chemin_acces_fichier, "wb");
    if (!file) {
        perror("Erreur d'ouverture du fichier pour l'upload");
        char *erreur_msg = "ERREUR: Impossible de créer le fichier\n";
        send(client_fd, erreur_msg, strlen(erreur_msg), 0);
        return;
    }

    char buf_fichier[BUFFER_SIZE];
    ssize_t donne_recue;

    printf("Réception du fichier : %s\n", chemin_acces_fichier);

    // Lire et écrire les données reçu dans le fichier
    while ((donne_recue = recv(client_fd, buf_fichier, sizeof(buf_fichier), 0)) > 0) {
        // comme pour dwl si le client a envoyé le marqueur de fin de fichier
            if (strstr(buf_fichier, "FIN_FICHIER") != NULL) {
                // On enlève "FIN_FICHIER" du contenu reçu
               //// vérifier l'utilisation de "memset"
            fwrite(buf_fichier, 1, donne_recue - strlen("FIN_FICHIER"), file); // on supprime la sous chaine pour éviter les soucis
            break;
        }
        
            
        fwrite(buf_fichier, 1, donne_recue, file);
    }

    if (donne_recue < 0) {
        perror("Erreur lors de la réception du fichier");
    }

    fclose(file);
    printf("Upload terminé : %s\n", chemin_acces_fichier);
    close(client_fd);
}
