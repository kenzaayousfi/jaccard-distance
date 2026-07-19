#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>

#include "lecture.h"
#include "hashtable.h"
#include "holdall.h"
#include "distance.h"
#include "graph.h"

#define MAX_FILES 64

int main(int argc, char *argv[]) {
  setlocale(LC_COLLATE, "");
  int max_len = 0;
  int ponctuation = 0;
  int graph = 0;
  if (argc < 2) {
    fprintf(stderr, "Usage: jdis [OPTION]... FILE1 FILE2 [FILE]...\n");
    return EXIT_FAILURE;
  }
  char *fichiers[MAX_FILES];
  int nb_fichiers = 0;
  int index = 1;
  while (index < argc) {
    if (argv[index][0] == '-' && strcmp(argv[index], "-") != 0) {
      if (strcmp(argv[index], "-h") == 0) {
        index++;
        continue;
      } else if (strcmp(argv[index], "--help") == 0 || strcmp(argv[index],
          "-?") == 0) {
        puts("Utilisation : jdis [options] fichier1 fichier2 [...]\n"
            "Ce programme calcule la dissimilarité de Jaccard entre les"
            " ensembles de mots\n"
            "présents dans plusieurs fichiers texte.\n"
            "Pour chaque paire de fichiers, la dissimilarité est affichée avec"
            " 4 décimales.\n\n"
            "Options disponibles :\n"
            "  -p, --punctuation-like-space\n"
            "      Considérer les signes de ponctuation comme des séparateurs"
            " de mots (comme les espaces).\n\n"
            "  -i N, -iN, --initial=N\n"
            "      Tronquer les mots à N lettres significatives. Si N = 0, les"
            " mots ne sont pas tronqués.\n\n"
            "  -g, --graph\n"
            "      Afficher le graphe d’appartenance des mots aux fichiers"
            " (présence marquée par 'x').\n\n"
            "  -?, --help\n"
            "      Afficher ce message d’aide et quitter.\n\n"
            "  --\n"
            "      Forcer la fin de lecture des options, les arguments suivants"
            " seront traités comme des fichiers.\n\n"
            "  -h \n"
            "      Pareil que --words-processing=hash-table.\n\n"
            "Remarques :\n"
            "  - Chaque mot est défini comme une séquence de caractères non"
            " blancs.\n"
            "  - Au moins deux fichiers doivent être fournis.\n"
            "  - L’ordre des fichiers est conservé dans les affichages.\n"
            "  - Limite : 64 fichiers maximum peuvent être comparés par"
            " exécution.\n");
        return EXIT_SUCCESS;
      } else if (strcmp(argv[index], "--version") == 0) {
        puts("jdis - version 25.04.1");
        return EXIT_SUCCESS;
      } else if (strcmp(argv[index], "--usage") == 0) {
        puts("Usage: jdis [OPTION]... FILE1 FILE2 [FILE]...");
        return EXIT_SUCCESS;
      } else if (strcmp(argv[index], "--") == 0) {
        index++;
        while (index < argc) {
          fichiers[nb_fichiers++] = argv[index++];
        }
        break;
      } else if (strcmp(argv[index], "-p") == 0 || strcmp(argv[index],
          "--punctuation-like-space") == 0) {
        ponctuation = 1;
      } else if (strcmp(argv[index], "-g") == 0 || strcmp(argv[index],
          "--graph") == 0) {
        graph = 1;
      } else if (strncmp(argv[index], "-i", 2) == 0 || strncmp(argv[index],
          "--initial", 9) == 0) {
        const char *val = nullptr;
        if (strncmp(argv[index], "-i", 2) == 0 && argv[index][2] != '\0') {
          // -iN
          val = argv[index] + 2;
        } else if (strcmp(argv[index], "-i") == 0) {
          // -i N
          if (index + 1 < argc) {
            val = argv[++index];
          } else {
            fprintf(stderr, "Erreur : valeur manquante pour l'option -i\n");
            return EXIT_FAILURE;
          }
        } else if (strncmp(argv[index], "--initial=", 10) == 0) {
          // --initial=N
          val = argv[index] + 10;
        } else if (strcmp(argv[index], "--initial") == 0) {
          // --initial N
          if (index + 1 < argc) {
            val = argv[++index];
          } else {
            fprintf(stderr,
                "Erreur : valeur manquante pour l'option --initial\n");
            return EXIT_FAILURE;
          }
        } else {
          fprintf(stderr, "Erreur : option mal formée : %s\n", argv[index]);
          return EXIT_FAILURE;
        }
        max_len = atoi(val);
        if (max_len < 0) {
          fprintf(stderr,
              "Erreur : valeur invalide pour l'option initial : %s\n", val);
          return EXIT_FAILURE;
        }
      } else {
        fprintf(stderr, "Option inconnue : %s\n", argv[index]);
        return EXIT_FAILURE;
      }
    } else {
      fichiers[nb_fichiers++] = argv[index];
    }
    index++;
  }
  if (nb_fichiers < 2) {
    fprintf(stderr,
        "Erreur : au moins deux fichiers doivent être spécifiés.\n");
    return EXIT_FAILURE;
  }
  fichier_data **fichiers_data = malloc((size_t) nb_fichiers
      * sizeof(fichier_data *));
  hashtable *table_global = hashtable_empty(
      (int (*)(const void *, const void *)) strcmp,
      (size_t (*)(const void *)) strlen,
      0.75);
  holdall *infos = holdall_empty();
  lecture_fichiers(fichiers, nb_fichiers, fichiers_data, ponctuation, max_len,
      table_global, infos);
  if (graph) {
    afficher_graphe(fichiers_data, nb_fichiers);
  } else {
    afficher_distances(fichiers_data, nb_fichiers);
  }
  free_fichier_data(fichiers_data, nb_fichiers);
  holdall_apply(infos, liberer);
  holdall_dispose(&infos);
  hashtable_dispose(&table_global);
}
