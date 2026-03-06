#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menus.h"
#include "gestion.h"
#include "getset.h"
#include "files.h"
#include "consulter.h"
#include "modif_pers.h"

/* ------------------------------------------------------------------ */
/*  Helpers internes                                                    */
/* ------------------------------------------------------------------ */

static void ligne() {
    printf("================================================\n");
}

static void titre(const char* t) {
    printf("\n");
    ligne();
    printf("  %s\n", t);
    ligne();
    printf("\n");
}

/* Numerote l'arbre, affiche toutes les personnes et demande un numero.
   Retourne le pointeur sur la personne choisie, ou NULL. */
static Pers* selectionner(ListePers* arbre) {
    if (arbre == NULL) {
        printf("  L'arbre est vide.\n");
        return NULL;
    }
    numero(arbre);
    printf("\n--- Personnes disponibles ---\n");
    afficherTLM(arbre);
    printf("Numero (0 pour annuler) : ");
    int n;
    scanf("%d", &n);
    if (n == 0) return NULL;
    Pers* p = retrouverPersavecNumero(arbre, n);
    if (p == NULL) printf("  Numero introuvable.\n");
    return p;
}

/* ------------------------------------------------------------------ */
/*  Menu principal                                                      */
/* ------------------------------------------------------------------ */

ListePers* menuPrincipal(ListePers* arbre) {
    int choix;

    do {
        titre("GESTIONNAIRE D'ARBRE GENEALOGIQUE");
        printf("  [1]  Ajouter une personne\n");
        printf("  [2]  Modifier une personne\n");
        printf("  [3]  Gerer les liens familiaux\n");
        printf("  [4]  Supprimer une personne\n");
        printf("  [5]  Afficher la liste\n");
        printf("  [6]  Afficher l'arbre\n");
        printf("  [7]  Consulter une personne\n");
        printf("  [8]  Charger depuis un fichier\n");
        printf("  [9]  Sauvegarder l'arbre\n");
        printf("  [10] Exporter le livret de famille\n");
        printf("  [0]  Quitter\n");
        printf("\nVotre choix : ");
        if (scanf("%d", &choix) != 1) { choix = -1; }

        switch (choix) {
            case 1:  arbre = ajouterPersonne(arbre);  break;
            case 2:  menuModifier(arbre);             break;
            case 3:  menuLiens(arbre);                break;
            case 4:  arbre = menuSupprimer(arbre);    break;
            case 5:
                numero(arbre);
                printf("\n");
                afficherTLM(arbre);
                break;
            case 6:  afficherArbre(arbre);            break;
            case 7:  menuConsulter(arbre);            break;
            case 8:  arbre = menuCharger(arbre);      break;
            case 9:  menuSauvegarder(arbre);          break;
            case 10: menuLivret(arbre);               break;
            case 0:  arbre = quitterSauvegarder(arbre); break;
            default: printf("  Choix invalide.\n");   break;
        }

        if (choix != 0) {
            printf("\n[Appuyez sur Entree pour continuer...]");
            while (getchar() != '\n');
            getchar();
        }

    } while (choix != 0);

    return arbre;
}

ListePers* quitterSauvegarder(ListePers* arbre) {
    char c;
    printf("Sauvegarder avant de quitter ? (o/n) : ");
    scanf(" %c", &c);
    if (c == 'o' || c == 'O') {
        menuSauvegarder(arbre);
    }
    return arbre;
}

/* ------------------------------------------------------------------ */
/*  Sous-menus                                                          */
/* ------------------------------------------------------------------ */

void menuModifier(ListePers* arbre) {
    Pers* p = selectionner(arbre);
    if (p == NULL) return;

    int choix;
    do {
        printf("\n=== MODIFIER : %s %s ===\n", p->prenom, p->nom);
        printf("  [1] Nom\n");
        printf("  [2] Prenom\n");
        printf("  [3] Date de naissance\n");
        printf("  [4] Date de deces\n");
        printf("  [5] Statut vivant/decede\n");
        printf("  [0] Retour\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: modifierNom(p);       break;
            case 2: modifierPrenom(p);    break;
            case 3: modifierNaissance(p); break;
            case 4: modifierDeces(p);     break;
            case 5: modifierVivant(p);    break;
        }
    } while (choix != 0);

    printf("\nFiche mise a jour :\n");
    afficherPers(p);
}

void menuLiens(ListePers* arbre) {
    Pers* p = selectionner(arbre);
    if (p == NULL) return;

    int choix;
    do {
        printf("\n=== LIENS DE : %s %s ===\n", p->prenom, p->nom);
        printf("  Pere     : %s\n", p->pere     ? p->pere->nom     : "(aucun)");
        printf("  Mere     : %s\n", p->mere     ? p->mere->nom     : "(aucune)");
        printf("  Conjoint : %s\n", p->conjoint ? p->conjoint->nom : "(aucun)");
        printf("\n");
        printf("  [1] Definir le pere\n");
        printf("  [2] Definir la mere\n");
        printf("  [3] Definir le conjoint\n");
        printf("  [4] Ajouter un enfant\n");
        printf("  [0] Retour\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: ajouterPere(arbre, p);   break;
            case 2: ajouterMere(arbre, p);   break;
            case 3: ajouterConj(arbre, p);   break;
            case 4: ajouterEnfant(arbre, p); break;
        }
    } while (choix != 0);
}

ListePers* menuSupprimer(ListePers* arbre) {
    if (arbre == NULL) { printf("  L'arbre est vide.\n"); return arbre; }
    Pers* p = selectionner(arbre);
    if (p == NULL) return arbre;

    char c;
    printf("Supprimer %s %s ? (o/n) : ", p->prenom, p->nom);
    scanf(" %c", &c);

    if (c == 'o' || c == 'O') {
        ListePers* courant = arbre;
        ListePers* prec    = NULL;
        while (courant != NULL && courant->pers != p) {
            prec    = courant;
            courant = courant->suivant;
        }
        if (courant != NULL) {
            if (prec == NULL) arbre = courant->suivant;
            else              prec->suivant = courant->suivant;
            supprimerFiliationPers(p);
            free(p);
            free(courant);
            printf("  Personne supprimee.\n");
        }
    }
    return arbre;
}

void menuConsulter(ListePers* arbre) {
    Pers* p = selectionner(arbre);
    if (p == NULL) return;

    int choix;
    do {
        printf("\n=== CONSULTER : %s %s ===\n", p->prenom, p->nom);
        printf("  [1] Afficher les enfants\n");
        printf("  [2] Afficher la descendance\n");
        printf("  [3] Afficher les parents\n");
        printf("  [4] Afficher l'ascendance\n");
        printf("  [0] Retour\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: afficherEnfantsRecurs(arbre, p); break;
            case 2: afficherDescendance(arbre, p);   break;
            case 3: afficherParentRecurs(arbre, p);  break;
            case 4: afficherAscendance(arbre, p);    break;
        }
    } while (choix != 0);
}

ListePers* menuCharger(ListePers* arbre) {
    if (arbre != NULL) {
        char c;
        printf("Un arbre est deja charge. Ecraser ? (o/n) : ");
        scanf(" %c", &c);
        if (c != 'o' && c != 'O') return arbre;
        arbre = viderArbre(arbre);
    }
    char* nom = saisirNomFichier();
    ListePers* nouvel = chargerArbre(nom);
    free(nom);
    if (nouvel != NULL) {
        printf("  Chargement reussi !\n");
        return nouvel;
    }
    printf("  Echec du chargement.\n");
    return arbre;
}

void menuSauvegarder(ListePers* arbre) {
    if (arbre == NULL) { printf("  L'arbre est vide.\n"); return; }
    char* nom = saisirNomFichier();
    sauvegarderArbreFichier(arbre, nom);
    printf("  Arbre sauvegarde dans '%s'.\n", nom);
    free(nom);
}

void menuLivret(ListePers* arbre) {
    if (arbre == NULL) { printf("  L'arbre est vide.\n"); return; }
    char* nom = saisirNomFichier();
    exporterLivretFamille(arbre, nom);
    printf("  Livret de famille exporte dans '%s'.\n", nom);
    free(nom);
}
