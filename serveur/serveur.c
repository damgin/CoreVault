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
    //on gère les erreurs:
    if(data_recus < 0){perror("erreur de receptions des données, c con :/");close(client_fd);}

    //on regarde ce que le client envoie :
    
    ///si c'est LIST
    if(strncmp(buf,"list",4) == 0){;list_file(client_fd);};
    // ici on ce fait le download
    if (strncmp(buf, "download_", 9) == 0) {download(client_fd, buf + 9);}
    // Si la commande commence par "upload_"
    if (strncmp(buf, "upload_", 7) == 0) { upload(client_fd, buf + 7);}
    // Si la commande commence par "break"
    


    close(client_fd);
    return EXIT_SUCCESS;

    }











   
