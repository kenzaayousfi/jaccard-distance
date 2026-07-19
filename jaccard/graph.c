#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "graph.h"
#include "hashtable.h"
#include "holdall.h"
#include "lecture.h"

#define CAPACITE_INITIALE 128

static int comparer_strcoll(const void *a, const void *b) {
  const char *s1 = *(const char **) a;
  const char *s2 = *(const char **) b;
  return strcoll(s1, s2);
}

static bool mot_dans_fichier(const char *mot, const fichier_data *fd) {
  int n = fichier_data_get_mots_count(fd);
  for (int i = 0; i < n; i++) {
    const char *mot_fd = fichier_data_get_mot(fd, i);
    if (strcmp(mot, mot_fd) == 0) {
      return true;
    }
  }
  return false;
}

static void *contexte_direct(void *ctx, void *ref) {
  (void) ref;
  return ctx;
}

static int ajouter_mot(void *mot, void *ctxptr) {
  struct {
    char ***liste;
    int *taille;
    int *capacite;
  } *ctx = ctxptr;
  for (int i = 0; i < *ctx->taille; i++) {
    if (strcmp((*ctx->liste)[i], mot) == 0) {
      return 0;
    }
  }
  if (*ctx->taille >= *ctx->capacite) {
    *ctx->capacite *= 2;
    char **tmp = realloc(*ctx->liste, (size_t) *ctx->capacite * sizeof(char *));
    if (!tmp) {
      fprintf(stderr, "Erreur realloc dans ajouter_mot\n");
      exit(EXIT_FAILURE);
    }
    *ctx->liste = tmp;
  }
  (*ctx->liste)[*ctx->taille] = strdup(mot);
  if (!(*ctx->liste)[*ctx->taille]) {
    fprintf(stderr, "Erreur d’allocation mémoire\n");
    exit(EXIT_FAILURE);
  }
  (*ctx->taille)++;
  return 0;
}

void afficher_graphe(fichier_data **fichiers, int nb_fichiers) {
  setlocale(LC_COLLATE, "");
  int taille = 0;
  int capacite = CAPACITE_INITIALE;
  char **liste_mots = malloc((size_t) capacite * sizeof(char *));
  if (!liste_mots) {
    fprintf(stderr, "Erreur malloc liste mots\n");
    return;
  }
  struct {
    char ***liste;
    int *taille;
    int *capacite;
  } ctx = {
    &liste_mots, &taille, &capacite
  };
  for (int i = 0; i < nb_fichiers; ++i) {
    holdall_apply_context(fichier_data_get_holdall(fichiers[i]), &ctx,
        contexte_direct, ajouter_mot);
  }
  qsort(liste_mots, (size_t) taille, sizeof(char *), comparer_strcoll);
  printf("\t");
  for (int i = 0; i < nb_fichiers; i++) {
    const char *nom_fichier = fichier_data_get_nom(fichiers[i]);
    printf("%s", strcmp(nom_fichier, "-") == 0 ? "\"\"" : nom_fichier);
    if (i < nb_fichiers - 1) {
      printf("\t");
    }
  }
  printf("\n");
  for (int i = 0; i < taille; i++) {
    printf("%s", liste_mots[i]);
    for (int j = 0; j < nb_fichiers; j++) {
      printf("\t%c", mot_dans_fichier(liste_mots[i], fichiers[j]) ? 'x' : '-');
    }
    printf("\n");
    free(liste_mots[i]);
  }
  free(liste_mots);
}
