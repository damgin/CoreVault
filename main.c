#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <dirent.h>

int main(){
    
    DIR *dir_ptr;

    struct dirent *read_dir;

    dir_ptr = opendir("tests");

    if (dir_ptr == 0) {perror("impossible d'ouvrir le dossier\n"); return 1;}

    while ((read_dir = readdir(dir_ptr)) !=0){

        if (read_dir -> d_type == DT_REG) { printf("FILE : %s\n",read_dir->d_name); }
        else if (read_dir -> d_type == DT_DIR) { printf("DIR: %s\n",read_dir ->d_name);}

    }
    if (closedir(dir_ptr)== -1) { perror("impossible de fermer le dir\n"); return 1;}

    return 0;

}