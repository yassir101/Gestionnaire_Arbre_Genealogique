#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion.h"

Pers* creerPers() {
    Pers* nouvellePersonne = (Pers*)malloc(sizeof(Pers));
    if (nouvellePersonne != NULL) {
        strcpy(nouvellePersonne->nom, "");
        strcpy(nouvellePersonne->prenom, "");
        nouvellePersonne->naissance.jour = 0;
        nouvellePersonne->naissance.mois = 0;
        nouvellePersonne->naissance.annee = 0;
        nouvellePersonne->deces.jour = 0;
        nouvellePersonne->deces.mois = 0;
        nouvellePersonne->deces.annee = 0;
        nouvellePersonne->pere = NULL;
        nouvellePersonne->mere = NULL;
        nouvellePersonne->conjoint = NULL;
        nouvellePersonne->enfants = NULL;
        nouvellePersonne->vivant = 1;
        nouvellePersonne->n = 0;
        nouvellePersonne->np = 0;
        nouvellePersonne->nm = 0;
        nouvellePersonne->nc = 0;
    }
    return nouvellePersonne;
}

ListePers* ajouterPersonne(ListePers* arbre) {
    Pers* p = creerPers();
    if (p == NULL) {
        printf("  Erreur d'allocation.\n");
        return arbre;
    }

    printf("\n=== AJOUTER UNE PERSONNE ===\n");
    printf("Nom        : "); scanf("%49s", p->nom);
    printf("Prenom     : "); scanf("%49s", p->prenom);
    printf("Naissance (jj mm aaaa, 0 0 0 si inconnue) : ");
    scanf("%d %d %d", &p->naissance.jour, &p->naissance.mois, &p->naissance.annee);
    printf("Vivant ? (1=oui 0=non) : ");
    scanf("%d", &p->vivant);
    if (!p->vivant) {
        printf("Deces (jj mm aaaa, 0 0 0 si inconnue) : ");
        scanf("%d %d %d", &p->deces.jour, &p->deces.mois, &p->deces.annee);
    }

    ListePers* maillon = (ListePers*)malloc(sizeof(ListePers));
    if (maillon == NULL) {
        free(p);
        printf("  Erreur d'allocation.\n");
        return arbre;
    }
    maillon->pers    = p;
    maillon->suivant = arbre;
    printf("  -> %s %s ajoute(e).\n", p->prenom, p->nom);
    return maillon;
}



void supprimerFiliationPers(Pers* pers){
    if (pers->pere != NULL) {
        ListePers* enfantsPere = pers->pere->enfants;
        ListePers* maillonEnfant = enfantsPere;
        ListePers* maillonPrecedent = NULL;

        while (maillonEnfant != NULL && maillonEnfant->pers != pers) {
            maillonPrecedent = maillonEnfant;
            maillonEnfant = maillonEnfant->suivant;
        }

        if (maillonEnfant != NULL) {
            if (maillonPrecedent != NULL) {
                maillonPrecedent->suivant = maillonEnfant->suivant;
            } else {
                pers->pere->enfants = maillonEnfant->suivant;
            }
            free(maillonEnfant);
        }
        pers->pere = NULL;
    }

    if (pers->mere != NULL) {
        ListePers* enfantsMere = pers->mere->enfants;
        ListePers* maillonEnfant = enfantsMere;
        ListePers* maillonPrecedent = NULL;

        while (maillonEnfant != NULL && maillonEnfant->pers != pers) {
            maillonPrecedent = maillonEnfant;
            maillonEnfant = maillonEnfant->suivant;
        }

        if (maillonEnfant != NULL) {
            if (maillonPrecedent != NULL) {
                maillonPrecedent->suivant = maillonEnfant->suivant;
            } else {
                pers->mere->enfants = maillonEnfant->suivant;
            }
            free(maillonEnfant);
        }
        pers->mere = NULL;
    }

    if (pers->enfants != NULL) {
        ListePers* maillonEnfant = pers->enfants;

        while (maillonEnfant != NULL) {
            maillonEnfant->pers->pere = NULL;
            maillonEnfant->pers->mere = NULL;

            ListePers* maillonSuivant = maillonEnfant->suivant;
            free(maillonEnfant);
            maillonEnfant = maillonSuivant;
        }
        pers->enfants = NULL;
    }
}

ListePers* supprimerPers(ListePers* arbre)
{
    if (arbre != NULL) {
        Pers* personneASupprimer = arbre->pers;
        supprimerFiliationPers(personneASupprimer);
        free(personneASupprimer);
        ListePers* maillonASupprimer = arbre;
        arbre = arbre->suivant;
        free(maillonASupprimer);
    }
    return arbre;
}


ListePers* viderArbre(ListePers* arbre) {
    while (arbre != NULL) {
        arbre = supprimerPers(arbre);
    }
    return arbre;
}
