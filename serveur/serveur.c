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
        }
            return EXIT_SUCCESS;    
}


   
