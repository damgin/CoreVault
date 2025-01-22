#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>

int main(){
   /// fonctionne aussi : DIR * test = opendir("tests");
    
    int *test;
    // int test;

    struct dirent *lecteur_dossier;
    test = opendir("tests");


    if (test == 0) {perror("impossible d'ouvrir le dossier\n"); return 1;}

    while ((lecteur_dossier = readdir(test)) !=0){

        if (lecteur_dossier -> d_type == DT_REG) { printf("FILE : %s\n",lecteur_dossier->d_name); }
        else if (lecteur_dossier -> d_type == DT_DIR) { printf("DIR: %s\n",lecteur_dossier ->d_name);}

    }
    if (closedir(test)== -1) { perror("impossible de fermer le dir\n"); return 1;}

    return 0;

}