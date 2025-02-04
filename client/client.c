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
        "  Bienvenue sur Core Vault - Votre plateforme 'sécurisée*' de stockage en ligne.\n"
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

    if(fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        // Vérifier si la commande commence par "upload_"
        if (strncmp(buffer, "upload_", 7) == 0) {upload(client_fd,buffer);}

        // Vérifier si la commande commence par "download_"
        else if (strncmp(buffer, "download_", 9) == 0) { download(client_fd,buffer);}

        // Commande 'list'
        else if (strncmp(buffer, "list", 4) == 0) {list(client_fd,buffer);}
        
       
        
        // en cas de péping
        } else {printf("Erreur lors de la lecture de la commande.\n");}
            close(client_fd);
            return EXIT_SUCCESS;
}
    

