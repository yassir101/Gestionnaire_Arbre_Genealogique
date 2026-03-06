#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menus.h"
#include "gestion.h"
#include "getset.h"
#include "files.h"
#include "consulter.h"


void chargement() {
    char* nomFichier = saisirNomFichier();

    ListePers* nouvelArbre = chargerArbre(nomFichier);

    if (nouvelArbre != NULL) {
        printf("Chargement reussi !\n");
    } else {
        printf("Erreur lors du chargement de l'arbre.\n");
    }

    free(nomFichier);
}

ListePers* quitterSauvegarder(ListePers* arbre) {
    char choix;
    printf("Voulez-vous sauvegarder l'arbre avant de quitter ? (O/N): ");
    scanf(" %c", &choix);

    if (choix == 'O' || choix == 'o') {
        char* nomFichier = saisirNomFichier();
        sauvegarderArbreFichier(arbre, nomFichier);
        free(nomFichier);
        printf("Arbre sauvegarde!\n");
    }
    return arbre;
}

void sauvegarder(ListePers* arbre) {
    printf("Sauvegarde en cours...\n");
    arbre = quitterSauvegarder(arbre);
    printf("Sauvegarde terminee.\n");
}

ListePers* menuPrincipal(ListePers* arbre) {
    int choix;

    do {
        printf("\nMenu Principal :\n");
        printf("1. Gestion\n");
        printf("2. Consulter l'arbre\n");
        printf("3. Afficher l'arbre\n");
        printf("4. Sauvegarder et quitter\n");
        printf("Entrez votre choix (1-4) ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                arbre = gerer(arbre);
                break;
            case 2:
                consulter(arbre);
                break;
            case 3:
                affichage(arbre);
                break;
            case 4:
                arbre = quitterSauvegarder(arbre);
                break;
            default:
                printf("Choix non valide. Veuillez entrer un nombre entre 1 et 4.\n");
                break;
        }

    } while (choix != 4);

    return arbre;
}

ListePers* gerer(ListePers* arbre) {
    int choix;

    do {
        printf("\nMenu Gestion :\n");
        printf("1. Ajouter une personne\n");
        printf("2. Modifier une personne\n");
        printf("3. Vider l'arbre\n");
        printf("4. Retourner au menu principal\n");
        printf("Entrez votre choix (1-4) : ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                arbre = ajouterPersonne(arbre);
                break;
            case 2:
                modifierPers(arbre);
                break;
            case 3:
                arbre = viderArbre(arbre);
                break;
            case 4:
                printf("Retour au menu principal.\n");
                break;
            default:
                printf("Choix non valide. Veuillez entrer un nombre entre 1 et 4.\n");
                break;
        }

    } while (choix != 4);

    return arbre;
}

void modifierPers(ListePers* arbre) {
    int choix;
    Pers* personneChoisie = NULL;

    numero(arbre);
    afficherTLM(arbre);

    printf("Choisissez le numero de la personne que vous souhaitez modifier : ");
    scanf("%d", &choix);

    personneChoisie = retrouverPersavecNumero(arbre, choix);

    if (personneChoisie == NULL) {
        printf("Personne introuvable.\n");
        return;
    }

    printf("\nInformations actuelles :\n");
    afficherPers(personneChoisie);

    int choixModification;
    do {
        printf("\nMenu de modification :\n");
        printf("1. Modifier le nom\n");
        printf("2. Modifier le prenom\n");
        printf("3. Modifier la date de naissance\n");
        printf("4. Modifier la date de deces\n");
        printf("5. Modifier la vitalite\n");
        printf("6. Quitter le menu de modification\n");
        printf("Votre choix : ");
        scanf("%d", &choixModification);

        switch (choixModification) {
            case 1:
                modifierNom(personneChoisie);
                break;
            case 2:
                modifierPrenom(personneChoisie);
                break;
            case 3:
                modifierNaissance(personneChoisie);
                break;
            case 4:
                modifierDeces(personneChoisie);
                break;
            case 5:
                modifierVivant(personneChoisie);
                break;
            case 6:
                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }

    } while (choixModification != 6);

    printf("\nInformations mises a jour :\n");
    afficherPers(personneChoisie);
}

static Pers* choisirPersonne(ListePers* arbre) {
    numero(arbre);
    afficherTLM(arbre);
    printf("Entrez le numero de la personne : ");
    int choix;
    scanf("%d", &choix);
    return retrouverPersavecNumero(arbre, choix);
}

void consulter(ListePers* arbre) {
    Pers* personneAConsulter = choisirPersonne(arbre);

    if (personneAConsulter == NULL) {
        printf("La personne n'a pas ete trouvee.\n");
        return;
    }

    printf("\nInformations de la personne :\n");
    afficherPers(personneAConsulter);

    int choixConsultation;
    do {
        printf("\nMenu de consultation :\n");
        printf("1. Afficher les enfants\n");
        printf("2. Afficher la descendance\n");
        printf("3. Afficher les parents\n");
        printf("4. Afficher l'ascendance\n");
        printf("5. Quitter le menu de consultation\n");
        printf("Votre choix : ");
        scanf("%d", &choixConsultation);

        switch (choixConsultation) {
            case 1:
                afficherEnfantsRecurs(arbre, personneAConsulter);
                break;
            case 2:
                afficherDescendance(arbre, personneAConsulter);
                break;
            case 3:
                afficherParentRecurs(arbre, personneAConsulter);
                break;
            case 4:
                afficherAscendance(arbre, personneAConsulter);
                break;
            case 5:
                break;
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
        }

    } while (choixConsultation != 5);
}

