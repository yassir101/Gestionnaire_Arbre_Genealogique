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

    /* Reconstruit les pointeurs pere/mere/conjoint a partir des numeros */
    arbre = retrouverLiens(arbre);

    return arbre;
}
