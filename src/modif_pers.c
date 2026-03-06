#include "modif_pers.h"
#include "types.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void modifierNom(Pers *pers){
    printf("Entrez le nouveau nom:\n ");
    scanf("%49s", pers->nom);
}

void modifierPrenom(Pers *pers){
    printf("Entrez le nouveau prenom: \n");
    scanf("%49s", pers->prenom);
}

void modifierNaissance(Pers *pers) {
    printf("Entrez la nouvelle date de naissance (jj mm aaaa): \n");
    scanf("%d %d %d", &pers->naissance.jour, &pers->naissance.mois, &pers->naissance.annee);
}

void modifierDeces(Pers *pers)
{
    printf("Entrez la nouvelle date de deces (jj mm aaaa):\n ");
    scanf("%d %d %d", &pers->deces.jour, &pers->deces.mois, &pers->deces.annee);
}

void modifierVivant(Pers *pers)
{
    printf("La personne est-elle toujours vivante? (1 pour oui, 0 pour non):\n ");
    scanf("%d", &pers->vivant);
}

void ajouterLienParentReci(Pers *parent, Pers *enfant, char *mpt)
{
    if (strcmp(mpt, "pere") == 0) {
        parent->pere = enfant;
        enfant->np = parent->n;
    } else if (strcmp(mpt, "mere") == 0) {
        parent->mere = enfant;
        enfant->nm = parent->n;
    }
}

void supprimerLienParentReci(Pers *parent, Pers *enfant) {
    if (parent->pere == enfant) {
        parent->pere = NULL;
        enfant->np = 0;
    } else if (parent->mere == enfant) {
        parent->mere = NULL;
        enfant->nm = 0;
    }
}

void ajouterEnfant(ListePers *arbre, Pers *pers) {
    int numEnfant;
    printf("Entrez le num�ro de l'enfant a ajouter : ");
    scanf("%d", &numEnfant);

    Pers *enfant = retrouverPersavecNumero(arbre, numEnfant);
    if (enfant != NULL) {
        pers->enfants = fix_pers_liste_creer(pers->enfants, enfant);
        printf("Enfant ajoute avec succes.\n");
    } else {
        printf("Enfant introuvable.\n");
    }
}

void ajouterPere(ListePers *arbre, Pers *pers)
{
    int numPere;
    printf("Entrez le numero du pere: ");
    scanf("%d", &numPere);

    Pers *pere = retrouverPersavecNumero(arbre, numPere);
    if (pere != NULL) {
        pers->pere = pere;
        printf("Pere ajoute avec succes.\n");
    } else {
        printf("Pere introuvable.\n");
    }
}

void ajouterMere(ListePers *arbre, Pers *pers) {
    int numMere;
    printf("Entrez le numero de la mere a ajouter : ");
    scanf("%d", &numMere);

    Pers *mere = retrouverPersavecNumero(arbre, numMere);
    if (mere != NULL) {
        pers->mere = mere;
        printf("Mere ajoutee avec succes.\n");
    } else {
        printf("Mere introuvable.\n");
    }
}

void ajouterConj(ListePers *arbre, Pers *pers) {
    int numConjoint;
    printf("Entrez le numero du conjoint a ajouter : ");
    scanf("%d", &numConjoint);

    Pers *conjoint = retrouverPersavecNumero(arbre, numConjoint);
    if (conjoint != NULL) {
        pers->conjoint = conjoint;
        printf("Conjoint ajoute avec succes.\n");
    } else {
        printf("Conjoint introuvable.\n");
    }
}

void supprimerEnfant(Pers *pers)
{
    if (pers->enfants != NULL) {
        int numEnfant;
        printf("Entrez le num�ro de l'enfant que l'on souhaite supprimer: ");
        scanf("%d", &numEnfant);

        ListePers *nouvelleListe = sup_maillon_liste(pers->enfants, retrouverPersavecNumero(pers->enfants, numEnfant));
        pers->enfants = nouvelleListe;
        printf("Enfant supprime avec succes.\n");
    } else {
        printf("Pas d'enfants.\n");
    }
}

void supprimerPere(Pers *pers) {
    if (pers->pere != NULL) {
        pers->pere = NULL;
        printf("Pere supprime avec succes.\n");
    } else {
        printf("Pas de pere.\n");
    }
}

void supprimerMere(Pers *pers) {
    if (pers->mere != NULL) {
        pers->mere = NULL;
        printf("Mere supprimee avec succes.\n");
    } else {
        printf("Pas de mere.\n");
    }
}

void supprimerConj(Pers *pers) {
    if (pers->conjoint != NULL) {
        pers->conjoint = NULL;
        printf("Conjoint supprime avec succes.\n");
    } else {
        printf("La personne est celibataire.\n");
    }
}

void modifierPere(ListePers *arbre, Pers *pers) {
    if (pers->pere != NULL) {
        printf("Modifier le pere de %s %s :\n", pers->prenom, pers->nom);
        ajouterPere(arbre, pers);
    } else {
        printf("La personne n'a pas de pere.\n");
    }
}

void modifierMere(ListePers *arbre, Pers *pers) {
    if (pers->mere != NULL) {
        printf("Modifier la mere de %s %s :\n", pers->prenom, pers->nom);
        ajouterMere(arbre, pers);
    } else {
        printf("La personne n'a pas de mere.\n");
    }
}

void modifierEnfants(ListePers *arbre, Pers *pers) {
    if (pers->enfants != NULL) {
        printf("Modifier les enfants de %s %s :\n", pers->prenom, pers->nom);
        ajouterEnfant(arbre, pers);
    } else {
        printf("La personne n'a pas d'enfants.\n");
    }
}

void modifierConj(ListePers *arbre, Pers *pers)
{
    if (pers->conjoint != NULL) {
        printf("Modifier le conjoint de %s %s :\n", pers->prenom, pers->nom);
        ajouterConj(arbre, pers);
    } else {
        printf("La personne n'a pas de conjoint.\n");
    }
}

int coherenceDates(Pers *parent, Pers *enfant, char *mpt)
{
    if (strcmp(mpt, "pere") == 0) {
        if (parent->deces.annee != 0 && parent->deces.annee < enfant->naissance.annee) {
            return 0;
        }
    } else if (strcmp(mpt, "mere") == 0) {
        if (parent->deces.annee != 0 && parent->deces.annee < enfant->naissance.annee) {
            return 0;
        }
    }
    return 1;
}
