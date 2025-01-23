#include "global.h"



int main(){
char buf[255];

    /**
     * SOCKET
     * Je crée le socket serveur
     */
    int server_fd = socket(AF_INET,SOCK_STREAM,0);perror("socket");
    // Si la création échoue je ferme mon programme
    if(server_fd == -1) return EXIT_FAILURE;

    /**
    * BIND
    * Je relie le socket à un port et une ip avec la fonction bind()
    */
    struct sockaddr_in server_addr = {
        .sin_addr.s_addr = INADDR_ANY,
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT)
    };
    int error = bind(server_fd,(struct sockaddr*)&server_addr,sizeof server_addr);perror("bind");
    if(error == -1) { close(server_fd); return EXIT_FAILURE; }


    /**
     * LISTEN obligatoire 
     */
    error = listen(server_fd,BUFSIZ);perror("listen");
    if(error == -1) { close(server_fd); return EXIT_FAILURE; }

    printf("Server listen on port : %d\n",SERVER_PORT);
    

    /**
     * ACCEPT
     * J'attend qu'un client se connecter grâce à la fonction bloquante accept()
     * accept renvoi le client accepté ou -1 en cas d'erreur.
     */
    struct sockaddr_in client_addr;
    socklen_t len;
    int client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&len);
    
    // LE PROGRAMME EST EN PAUSE ET ATTEND UN CLIENT ...

    perror("accept");
    if(client_fd == -1){ close(client_fd); close(server_fd); return EXIT_FAILURE; }
 
    // Prêt à communiquer avec le client
    // ...
    memset(buf,0,255);

    //on récv la data du client
    int data_recus = 0;
    data_recus = recv(client_fd,buf,sizeof(buf),0);perror("reception des données");
    //on gere les erreurs:
    if(data_recus < 0){perror("erreur de receptions des données, c con :/");close(client_fd);}

    //on regarde ce que le client envoie :
    ///si c'est LIST
        if(strncmp(buf,"list",4) == 0){ 
            char tampon[BUFFER_SIZE]; // Tampon pour capturer le printf
            struct dirent *lecteur_dossier;
            DIR * dir = opendir("public");
                if (dir == 0) {perror("impossible d'ouvrir le dossier\n"); return 1;}

                while ((lecteur_dossier = readdir(dir)) !=0){

            // Vérification du type de fichier
                    if (lecteur_dossier->d_type == DT_REG) {
                        snprintf(tampon, sizeof(tampon), "FILE: %s\n", lecteur_dossier->d_name);
                        printf("Tampon: %s", tampon); // Affichage pour débugg
                            } else if (lecteur_dossier->d_type == DT_DIR) {
                                snprintf(tampon, sizeof(tampon), "DIR: %s\n", lecteur_dossier->d_name);
                                printf("Tampon: %s", tampon); // Affichage pour débugg
                            }

            
            if (send(client_fd, tampon, strlen(tampon), 0) == -1) {
                perror("Erreur d'envoi au client");
                break;
            }
            }if(closedir(dir)== -1) { perror("impossible de fermer le dir\n"); return 1;}
            // tentative d'envoie un "message fin de la liste"
            const char *fin_liste = "FIN_LISTE\n";
            if (send(client_fd, fin_liste, strlen(fin_liste), 0) == -1) {
                perror("Erreur d'envoi de l'indicateur de fin au client");
            }
            return EXIT_SUCCESS;    
        }
        // ici on ce fait le download
    if (strncmp(buf, "download_", 9) == 0) {
        char *nom_fichier = buf + 9; // Nom du fichier
        char chemin_acces_fichier[BUFFER_SIZE];
        snprintf(chemin_acces_fichier, sizeof(chemin_acces_fichier), "public/%s", nom_fichier); // Chemin du fichier

        // Ouverture du fichier en lecture
        int file_fd = open(chemin_acces_fichier, O_RDONLY);
        if (file_fd < 0) {
            perror("Erreur d'ouverture du fichier");
            char *erreur_msg = "ERREUR: Fichier introuvable\n";
            send(client_fd, erreur_msg, strlen(erreur_msg), 0);
        } else {
            char buf_fichier[BUFFER_SIZE];
            ssize_t donne_lue;

            // Envoi du contenu du fichier au client
            while ((donne_lue = read(file_fd, buf_fichier, sizeof(buf_fichier))) > 0) {
                if (send(client_fd, buf_fichier, donne_lue, 0) == -1) {
                    perror("Erreur d'envoi du fichier");
                    break;
                }
            }

            if (donne_lue < 0) perror("Erreur de lecture du fichier");

            close(file_fd);

            // Indicateur de fin de fichier
            const char *fin_transfert = "FIN_FICHIER\n";
            send(client_fd, fin_transfert, strlen(fin_transfert), 0);
        }
    close(client_fd);
    return EXIT_SUCCESS;
    }






// Si la commande commence par "upload_"
if (strncmp(buf, "upload_", 7) == 0) {


    char *nom_fichier = buf + 7; // Nom du fichier
    char chemin_acces_fichier[BUFFER_SIZE];
    snprintf(chemin_acces_fichier, sizeof(chemin_acces_fichier), "public/%s", nom_fichier); // Chemin du fichier

    // Ouvrir le fichier en écriture (création ou écrasement)
    FILE *file = fopen(chemin_acces_fichier, "wb");
    if (!file) {
        perror("Erreur d'ouverture du fichier pour l'upload");
        char *erreur_msg = "ERREUR: Impossible de créer le fichier\n";
        send(client_fd, erreur_msg, strlen(erreur_msg), 0);
    } else {
        char buf_fichier[BUFFER_SIZE];
        ssize_t donne_recue;

        // on recoit les octes en boucles
        while ((donne_recue = recv(client_fd, buf_fichier, sizeof(buf_fichier), 0)) > 0) {
            fwrite(buf_fichier, 1, donne_recue, file);

            // comme pour dwl si le client a envoyé le marqueur de fin de fichier
            if (strstr(buf_fichier, "FIN_FICHIER") != NULL) {
                // On enlève "FIN_FICHIER" du contenu reçu
                fseek(file, -strlen("FIN_FICHIER"), SEEK_CUR);
                break;
            }
        }

        if (donne_recue < 0) {
            perror("Erreur lors de la réception du fichier");
        }

        fclose(file);
        printf("Upload terminé : %s\n", chemin_acces_fichier);
    }

    close(client_fd);
    return EXIT_SUCCESS;
}

}






   
