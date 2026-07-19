#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "lecture.h"
#include "hashtable.h"
#include "holdall.h"

struct fichier_data {
  const char *nom;
  hashtable *table;
  char **mots_array;
  int mots_count;
  int mots_capacity;
  holdall *mots;
};

static bool mot_deja_present(char **mots_array, int count, const char *mot) {
  for (int i = 0; i < count; i++) {
    if (strcmp(mots_array[i], mot) == 0) {
      return true;
    }
  }
  return false;
}

fichier_data *lecture_fichier(const char *nom_fichier, int max_len,
    int ponctuation, hashtable *table_global, holdall *infos) {
  FILE *f = (strcmp(nom_fichier, "-") == 0) ? stdin : fopen(nom_fichier, "r");
  if (f == nullptr) {
    fprintf(stderr, "Erreur ouverture fichier : %s\n", nom_fichier);
    return nullptr;
  }
  if (f == stdin) {
    printf("--- starts reading for #1 FILE\n");
  }
  fichier_data *fd = malloc(sizeof(fichier_data));
  if (!fd) {
    if (f != stdin) {
      fclose(f);
    }
    return nullptr;
  }
  fd->nom = strdup(nom_fichier);
  fd->table = table_global;
  fd->mots = holdall_empty();
  fd->mots_count = 0;
  fd->mots_capacity = 16;
  fd->mots_array = malloc((size_t) fd->mots_capacity * sizeof(char *));
  if (!fd->mots || !fd->mots_array) {
    if (f != stdin) {
      fclose(f);
    }
    holdall_dispose(&fd->mots);
    free(fd->mots_array);
    free(fd);
    return nullptr;
  }
  char ligne[2048];
  int ligne_num = 0;
  while (fgets(ligne, sizeof ligne, f)) {
    ligne_num++;
    if (ponctuation) {
      for (char *p = ligne; *p != '\0'; ++p) {
        if (ispunct((unsigned char) *p)) {
          *p = ' ';
        }
      }
    }
    char *mot = strtok(ligne, " \t\n\r");
    while (mot != nullptr) {
      if (max_len > 0 && (int) strlen(mot) > max_len) {
        fprintf(stderr, "cut: %s:%d: %.5s...\n", nom_fichier, ligne_num, mot);
        mot[max_len] = '\0';
      }
      if (mot[0] != '\0' && !mot_deja_present(fd->mots_array, fd->mots_count,
          mot)) {
        char *copie = strdup(mot);
        if (!copie) {
          perror("strdup");
          fichier_data_dispose(&fd);
          if (f != stdin) {
            fclose(f);
          }
          return nullptr;
        }
        holdall_put(infos, copie);
        if (fd->mots_count == fd->mots_capacity) {
          fd->mots_capacity *= 2;
          char **tmp = realloc(fd->mots_array,
              (size_t) fd->mots_capacity * sizeof(char *));
          if (!tmp) {
            perror("realloc");
            free(copie);
            fichier_data_dispose(&fd);
            if (f != stdin) {
              fclose(f);
            }
            return nullptr;
          }
          fd->mots_array = tmp;
        }
        fd->mots_array[fd->mots_count++] = copie;
        holdall_put(fd->mots, copie);
        if (hashtable_search(table_global, copie) == nullptr) {
          hashtable_add(table_global, copie, copie);
        }
      }
      mot = strtok(nullptr, " \t\n\r");
    }
  }
  if (f == stdin) {
    printf("--- ends reading for #1 FILE\n");
  } else {
    fclose(f);
  }
  return fd;
}

int liberer(void *p) {
  free(p);
  return 0;
}

void fichier_data_dispose(fichier_data **ptr) {
  if (!ptr || !*ptr) {
    return;
  }
  fichier_data *fd = *ptr;
  if (fd->mots) {
    holdall_dispose(&fd->mots);
  }
  free((void *) fd->nom);
  free(fd->mots_array);
  free(fd);
  *ptr = nullptr;
}

void free_fichier_data(fichier_data **fichiers, int nb_fichiers) {
  if (!fichiers) {
    return;
  }
  for (int i = 0; i < nb_fichiers; ++i) {
    fichier_data_dispose(&fichiers[i]);
  }
  free(fichiers);
}

void lecture_fichiers(char **noms_fichiers, int nb_fichiers,
    fichier_data **donnees, bool ponctuation, int longueur_max,
    hashtable *table_global, holdall *infos) {
  for (int i = 0; i < nb_fichiers; ++i) {
    donnees[i] = lecture_fichier(noms_fichiers[i], longueur_max, ponctuation,
        table_global, infos);
    if (!donnees[i]) {
      fprintf(stderr, "Erreur lors de la lecture du fichier : %s\n",
          noms_fichiers[i]);
      for (int j = 0; j < i; ++j) {
        fichier_data_dispose(&donnees[j]);
      }
      exit(EXIT_FAILURE);
    }
  }
}

const char *fichier_data_get_nom(const fichier_data *fd) {
  return fd->nom;
}

int fichier_data_get_mots_count(const fichier_data *fd) {
  return fd->mots_count;
}

const char *fichier_data_get_mot(const fichier_data *fd, int index) {
  if (index < 0 || index >= fd->mots_count) {
    return nullptr;
  }
  return fd->mots_array[index];
}

holdall *fichier_data_get_holdall(const fichier_data *fd) {
  return fd->mots;
}
