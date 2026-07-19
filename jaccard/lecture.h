#ifndef LECTURE_H
#define LECTURE_H

#include <stdbool.h>
#include "hashtable.h"
#include "holdall.h"

// Structure pour stocker les données d'un fichier
typedef struct fichier_data fichier_data;

// Fonction pour lire un seul fichier
fichier_data *lecture_fichier(const char *nom_fichier, int max_len,
    int ponctuation, hashtable *table_global, holdall *infos);

// Libération propre d'un fichier_data
void fichier_data_dispose(fichier_data **ptr);

// Lecture de plusieurs fichiers
void lecture_fichiers(char **noms_fichiers, int nb_fichiers,
    fichier_data **donnees, bool ponctuation, int longueur_max,
    hashtable *table_global, holdall *infos);

// Libération d’un tableau de fichiers
void free_fichier_data(fichier_data **fichiers, int nb_fichiers);

// Libération d’un pointeur générique (ex : pour holdall_apply)
int liberer(void *p);

// Fonctions d'accès
const char *fichier_data_get_nom(const fichier_data *fd);
int fichier_data_get_mots_count(const fichier_data *fd);
const char *fichier_data_get_mot(const fichier_data *fd, int index);

holdall *fichier_data_get_holdall(const fichier_data *fd);

#endif
