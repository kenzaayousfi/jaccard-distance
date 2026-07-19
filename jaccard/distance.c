#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "distance.h"
#include "lecture.h"

static bool mot_dans_fichier(const char *mot, const fichier_data *fd) {
  int count = fichier_data_get_mots_count(fd);
  for (int i = 0; i < count; i++) {
    const char *mot_fd = fichier_data_get_mot(fd, i);
    if (strcmp(mot, mot_fd) == 0) {
      return true;
    }
  }
  return false;
}

static int compter_union(const fichier_data *a, const fichier_data *b) {
  int total = 0;
  int count_a = fichier_data_get_mots_count(a);
  int count_b = fichier_data_get_mots_count(b);
  total += count_a;
  for (int i = 0; i < count_b; i++) {
    const char *mot_b = fichier_data_get_mot(b, i);
    if (!mot_dans_fichier(mot_b, a)) {
      total++;
    }
  }
  return total;
}

static int compter_intersection(const fichier_data *a, const fichier_data *b) {
  int total = 0;
  int count_a = fichier_data_get_mots_count(a);
  for (int i = 0; i < count_a; i++) {
    const char *mot_a = fichier_data_get_mot(a, i);
    if (mot_dans_fichier(mot_a, b)) {
      total++;
    }
  }
  return total;
}

void afficher_distances(fichier_data **fichiers, int nb_fichiers) {
  for (int i = 0; i < nb_fichiers; ++i) {
    for (int j = i + 1; j < nb_fichiers; ++j) {
      int inter = compter_intersection(fichiers[i], fichiers[j]);
      int uni = compter_union(fichiers[i], fichiers[j]);
      double distance = (uni == 0) ? 0.0 : 1.0 - ((double) inter / uni);
      printf("%.4f\t%s\t%s\n",
          distance,
          strcmp(fichier_data_get_nom(fichiers[i]),
          "-") == 0 ? "\"\"" : fichier_data_get_nom(fichiers[i]),
          strcmp(fichier_data_get_nom(fichiers[j]),
          "-") == 0 ? "\"\"" : fichier_data_get_nom(fichiers[j]));
    }
  }
}
