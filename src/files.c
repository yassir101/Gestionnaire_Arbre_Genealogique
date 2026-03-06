#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* saisirNomFichier() {
    char* nomFichier = (char*)malloc(50 * sizeof(char));
    printf("Saisissez le nom du fichier: \n");
    scanf("%49s", nomFichier);
    return nomFichier;
}

ListePers* numero(ListePers* arbre) {
    int n = 1;
    ListePers* temp = arbre;

    while (temp != NULL) {
        temp->pers->n = n++;
        temp = temp->suivant;
    }

    return arbre;
}

void sauvegarderArbreFichier(ListePers* arbre, char* nomFich) {
    FILE* fichier = fopen(nomFich, "w");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    /* Numerote les personnes avant la sauvegarde pour coherence des liens */
    numero(arbre);

    ListePers* courant = arbre;
    while (courant != NULL) {
        Pers* p = courant->pers;
        fprintf(fichier, "%s;%s;%d/%d/%d;%d/%d/%d;%d;%d;%d;%d;%d\n",
                p->nom, p->prenom,
                p->naissance.jour, p->naissance.mois, p->naissance.annee,
                p->deces.jour, p->deces.mois, p->deces.annee,
                p->vivant, p->n,
                (p->pere != NULL) ? p->pere->n : 0,
                (p->mere != NULL) ? p->mere->n : 0,
                (p->conjoint != NULL) ? p->conjoint->n : 0);
        courant = courant->suivant;
    }

    fclose(fichier);
}

Pers* retrouverPersavecNumero(ListePers* arbre, int n) {
    while (arbre != NULL) {
        if (arbre->pers->n == n) {
            return arbre->pers;
        }
        arbre = arbre->suivant;
    }
    return NULL;
}

ListePers* retrouverLiens(ListePers* arbre) {
    ListePers* debut = arbre;

    while (arbre != NULL) {
        arbre->pers->pere    = (arbre->pers->np != 0) ? retrouverPersavecNumero(debut, arbre->pers->np) : NULL;
        arbre->pers->mere    = (arbre->pers->nm != 0) ? retrouverPersavecNumero(debut, arbre->pers->nm) : NULL;
        arbre->pers->conjoint = (arbre->pers->nc != 0) ? retrouverPersavecNumero(debut, arbre->pers->nc) : NULL;

        arbre = arbre->suivant;
    }

    return debut;
}

ListePers* chargerArbre(char* nomFich) {
    FILE* fichier = fopen(nomFich, "r");

    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    ListePers* arbre = NULL;

    char nom[50], prenom[50];
    int j_n, m_n, a_n, j_d, m_d, a_d;
    int vivant, n, np, nm, nc;

    while (fscanf(fichier, "%49[^;];%49[^;];%d/%d/%d;%d/%d/%d;%d;%d;%d;%d;%d\n",
                  nom, prenom,
                  &j_n, &m_n, &a_n,
                  &j_d, &m_d, &a_d,
                  &vivant, &n, &np, &nm, &nc) == 13) {

        Pers* p = (Pers*)malloc(sizeof(Pers));
        if (p == NULL) {
            perror("Erreur d'allocation");
            fclose(fichier);
            return arbre;
        }

        strncpy(p->nom, nom, sizeof(p->nom) - 1);
        p->nom[sizeof(p->nom) - 1] = '\0';
        strncpy(p->prenom, prenom, sizeof(p->prenom) - 1);
        p->prenom[sizeof(p->prenom) - 1] = '\0';

        p->naissance.jour  = j_n; p->naissance.mois  = m_n; p->naissance.annee  = a_n;
        p->deces.jour      = j_d; p->deces.mois      = m_d; p->deces.annee      = a_d;
        p->vivant          = vivant;
        p->n               = n;
        p->np              = np;
        p->nm              = nm;
        p->nc              = nc;
        p->pere            = NULL;
        p->mere            = NULL;
        p->conjoint        = NULL;
        p->enfants         = NULL;

        arbre = fix_pers_liste_creer(arbre, p);
    }

    fclose(fichier);

    /* 1. Reconstruit les pointeurs pere/mere/conjoint a partir des numeros */
    arbre = retrouverLiens(arbre);

    /* 2. Reconstruit la liste enfants de chaque pere
          (seul le parent primaire gere la liste pour eviter les doublons) */
    {
        ListePers* courant = arbre;
        while (courant != NULL) {
            Pers* p = courant->pers;
            if (p->pere != NULL) {
                p->pere->enfants = fix_pers_liste_creer(p->pere->enfants, p);
            } else if (p->mere != NULL) {
                p->mere->enfants = fix_pers_liste_creer(p->mere->enfants, p);
            }
            courant = courant->suivant;
        }
    }

    return arbre;
}

/* ------------------------------------------------------------------ */
/*  Export livret de famille                                            */
/* ------------------------------------------------------------------ */

static void ecrireDate(FILE* f, Date d, const char* label) {
    if (d.annee > 0)
        fprintf(f, "  %-16s: %02d/%02d/%d\n", label, d.jour, d.mois, d.annee);
}

void exporterLivretFamille(ListePers* arbre, const char* nomFich) {
    FILE* f = fopen(nomFich, "w");
    if (f == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    fprintf(f, "================================================\n");
    fprintf(f, "            LIVRET DE FAMILLE\n");
    fprintf(f, "================================================\n\n");

    ListePers* courant = arbre;
    int fiche = 1;
    while (courant != NULL) {
        Pers* p = courant->pers;

        fprintf(f, "--- FICHE N%d ---\n\n", fiche++);

        /* Identite */
        fprintf(f, "  IDENTITE\n");
        fprintf(f, "  %-16s: %s %s\n", "Nom complet", p->prenom, p->nom);
        ecrireDate(f, p->naissance, "Naissance");
        fprintf(f, "  %-16s: %s\n", "Statut", p->vivant ? "Vivant(e)" : "Decede(e)");
        if (!p->vivant) ecrireDate(f, p->deces, "Deces");
        fprintf(f, "\n");

        /* Parents */
        if (p->pere != NULL || p->mere != NULL) {
            fprintf(f, "  PARENTS\n");
            if (p->pere != NULL) {
                fprintf(f, "  Pere             : %s %s", p->pere->prenom, p->pere->nom);
                if (p->pere->naissance.annee > 0)
                    fprintf(f, " (ne le %02d/%02d/%d)",
                            p->pere->naissance.jour,
                            p->pere->naissance.mois,
                            p->pere->naissance.annee);
                fprintf(f, "\n");
            }
            if (p->mere != NULL) {
                fprintf(f, "  Mere             : %s %s", p->mere->prenom, p->mere->nom);
                if (p->mere->naissance.annee > 0)
                    fprintf(f, " (nee le %02d/%02d/%d)",
                            p->mere->naissance.jour,
                            p->mere->naissance.mois,
                            p->mere->naissance.annee);
                fprintf(f, "\n");
            }
            fprintf(f, "\n");
        }

        /* Conjoint */
        if (p->conjoint != NULL) {
            fprintf(f, "  CONJOINT(E)\n");
            fprintf(f, "  %-16s: %s %s\n", "Nom complet",
                    p->conjoint->prenom, p->conjoint->nom);
            ecrireDate(f, p->conjoint->naissance, "Naissance");
            fprintf(f, "\n");
        }

        /* Enfants */
        if (p->enfants != NULL) {
            fprintf(f, "  ENFANTS\n");
            ListePers* e = p->enfants;
            int i = 1;
            while (e != NULL) {
                fprintf(f, "  %2d. %s %s", i++, e->pers->prenom, e->pers->nom);
                if (e->pers->naissance.annee > 0)
                    fprintf(f, "  (ne(e) le %02d/%02d/%d)",
                            e->pers->naissance.jour,
                            e->pers->naissance.mois,
                            e->pers->naissance.annee);
                fprintf(f, "\n");
                e = e->suivant;
            }
            fprintf(f, "\n");
        }

        fprintf(f, "------------------------------------------------\n\n");
        courant = courant->suivant;
    }

    fclose(f);
}
