///#ifndef GLOBAL_H /// a voir avec massi, nouvelle facon d'include 1 seule fois global.h
#pragma once
#define BUFFER_SIZE 1024

#define BUFSIZ 255

#define CLIENT_PORT 8010
#define SERVER_PORT 3014

#define GLOBAL_H



#include <dirent.h> // Pour lister les fichiers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>  // Pour manipuler les fichiers
#include <sys/stat.h> // Pour les fichiers
#include "fonction.c"

///#define chemin_coffre "./?????????/"


////#endif /// a voir avec massi, nouvelle facon d'include 1 seule fois global.h