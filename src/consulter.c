#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "consulter.h"
#include "determiner_lien.h"
#include "getset.h"

ListePers* trouverNeveuNiece(ListePers* arbre, Pers* pers) {
    ListePers* neveuxNieces = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if (maillon->pers->pere != NULL && maillon->pers->pere->enfants != NULL) {
            ListePers* freresSoeurs = maillon->pers->pere->enfants;

            while (freresSoeurs != NULL) {
                if (freresSoeurs->pers != pers) {
                    neveuxNieces = fix_pers_liste_creer(neveuxNieces, freresSoeurs->pers);
                }

                freresSoeurs = freresSoeurs->suivant;
            }
        }

        maillon = maillon->suivant;
    }

    return neveuxNieces;
}

ListePers* trouverConjoint(ListePers* arbre, Pers* pers) {
    ListePers* conjoints = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if (maillon->pers->conjoint != NULL && maillon->pers->conjoint != pers) {
            conjoints = fix_pers_liste_creer(conjoints, maillon->pers->conjoint);
        }

        maillon = maillon->suivant;
    }

    return conjoints;
}

int sontFreresSoeurs(Pers* pers1, Pers* pers2) {
    if (pers1->pere != NULL && pers2->pere != NULL) {
        return pers1->pere == pers2->pere;
    }
    if (pers1->mere != NULL && pers2->mere != NULL) {
        return pers1->mere == pers2->mere;
    }
    return 0;
}

ListePers* trouverCousin(ListePers* arbre, Pers* pers) {
    ListePers* cousins = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if (sontFreresSoeurs(maillon->pers, pers)) {
            cousins = fix_pers_liste_creer(cousins, maillon->pers);
        }

        maillon = maillon->suivant;
    }

    return cousins;
}

ListePers* trouverOncleTante(ListePers* arbre, Pers* pers) {
    ListePers* onclesTantes = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if (maillon->pers->pere != NULL && sontFreresSoeurs(maillon->pers->pere, pers)) {
            onclesTantes = fix_pers_liste_creer(onclesTantes, maillon->pers);
        }

        if (maillon->pers->mere != NULL && sontFreresSoeurs(maillon->pers->mere, pers)) {
            onclesTantes = fix_pers_liste_creer(onclesTantes, maillon->pers);
        }

        maillon = maillon->suivant;
    }

    return onclesTantes;
}

ListePers* trouverPetitsEnfants(ListePers* arbre, Pers* pers) {
    ListePers* petitsEnfants = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if ((maillon->pers->pere != NULL && (maillon->pers->pere->pere == pers || maillon->pers->pere->mere == pers)) ||
            (maillon->pers->mere != NULL && (maillon->pers->mere->pere == pers || maillon->pers->mere->mere == pers))) {
            petitsEnfants = fix_pers_liste_creer(petitsEnfants, maillon->pers);
        }

        maillon = maillon->suivant;
    }

    return petitsEnfants;
}

ListePers* trouverGrandsParents(ListePers* arbre, Pers* pers) {
    ListePers* grandsParents = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if ((maillon->pers->pere != NULL && ((maillon->pers->pere->pere != NULL && maillon->pers->pere->pere == pers) ||
             (maillon->pers->pere->mere != NULL && maillon->pers->pere->mere == pers))) ||
            (maillon->pers->mere != NULL && ((maillon->pers->mere->pere != NULL && maillon->pers->mere->pere == pers) ||
             (maillon->pers->mere->mere != NULL && maillon->pers->mere->mere == pers)))) {
            grandsParents = fix_pers_liste_creer(grandsParents, maillon->pers);
        }

        maillon = maillon->suivant;
    }

    return grandsParents;
}

ListePers* trouverFratrie(ListePers* arbre, Pers* pers) {
    ListePers* fratrie = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if (sontFreresSoeurs(maillon->pers, pers) && maillon->pers != pers) {
            fratrie = fix_pers_liste_creer(fratrie, maillon->pers);
        }

        maillon = maillon->suivant;
    }

    return fratrie;
}

ListePers* trouverMere(ListePers* arbre, Pers* pers) {
    ListePers* mere = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if (maillon->pers->mere != NULL && maillon->pers->mere == pers) {
            mere = fix_pers_liste_creer(mere, maillon->pers);
        }

        maillon = maillon->suivant;
    }

    return mere;
}

ListePers* trouverPere(ListePers* arbre, Pers* pers) {
    ListePers* pere = NULL;
    ListePers* maillon = arbre;

    while (maillon != NULL) {
        if (maillon->pers->pere != NULL && maillon->pers->pere == pers) {
            pere = fix_pers_liste_creer(pere, maillon->pers);
        }

        maillon = maillon->suivant;
    }

    return pere;
}

void afficherPers(Pers* pers) {
    if (pers != NULL) {
        printf("Nom: %s\n", pers->nom);
        printf("Prenom: %s\n", pers->prenom);
        printf("Date de naissance: %d/%d/%d\n", pers->naissance.jour, pers->naissance.mois, pers->naissance.annee);

        if (pers->vivant) {
            printf("Vivant\n");
        } else {
            printf("Mort le %d/%d/%d\n", pers->deces.jour, pers->deces.mois, pers->deces.annee);
        }

        printf("\n");
    }
}

void afficherLien(ListePers* arbre, Pers* pers1, Pers* pers2) {
    char* lien = determinerLien(arbre, pers1, pers2);
    printf("Lien: %s\n", lien);
}

void afficherTLM(ListePers* arbre) {
    if (arbre != NULL) {
        while (arbre != NULL) {
            afficherPers(arbre->pers);
            arbre = arbre->suivant;
        }
    } else {
        printf("L'arbre est vide.\n");
    }
}

void afficherEnfantsRecurs(ListePers* arbre, Pers* pers) {
    ListePers* enfants = get_enfants_pers(pers);

    while (enfants != NULL) {
        afficherPers(enfants->pers);
        afficherEnfantsRecurs(arbre, enfants->pers);
        enfants = get_suiv_liste(enfants);
    }
}

void afficherDescendance(ListePers* arbre, Pers* personne) {
    printf("Descendance de %s %s:\n", get_nom_pers(personne), get_prenom_pers(personne));
    afficherEnfantsRecurs(arbre, personne);
}

void afficherParentRecurs(ListePers* arbre, Pers* pers) {
    if (pers == NULL) {
        return;
    }

    Pers* pere = get_pere_pers(pers);
    Pers* mere = get_mere_pers(pers);

    if (pere != NULL) {
        printf("P�re:\n");
        afficherPers(pere);
        afficherParentRecurs(arbre, pere);
    }

    if (mere != NULL) {
        printf("M�re:\n");
        afficherPers(mere);
        afficherParentRecurs(arbre, mere);
    }
}

void afficherAscendance(ListePers* arbre, Pers* personne) {
    printf("Ascendance de %s %s:\n", get_nom_pers(personne), get_prenom_pers(personne));
    afficherParentRecurs(arbre, personne);
}

void affichage(ListePers* arbre) {
    afficherArbre(arbre);
}

/* ------------------------------------------------------------------ */
/*  Affichage en format arbre ASCII                                     */
/* ------------------------------------------------------------------ */

/* Affiche une personne sur une ligne avec ses annees et conjoint */
static void afficherLigneNoeud(Pers* p) {
    printf("%s %s", p->prenom, p->nom);
    if (p->naissance.annee > 0) {
        printf(" (%d", p->naissance.annee);
        if (!p->vivant && p->deces.annee > 0)
            printf("-%d", p->deces.annee);
        printf(")");
    }
    if (p->conjoint != NULL)
        printf("  x  %s %s", p->conjoint->prenom, p->conjoint->nom);
    printf("\n");
}

/* Recursion : affiche les enfants de 'pers' en arbre ASCII.
   prefixe : chaine de decoration accumulee pour l'indentation.
   estDernier : 1 si c'est le dernier enfant dans la liste. */
static void afficherArbreRecurs(Pers* pers, const char* prefixe, int estDernier) {
    if (pers == NULL) return;

    printf("%s", prefixe);
    printf(estDernier ? "\\-- " : "|-- ");
    afficherLigneNoeud(pers);

    /* Construire le nouveau prefixe pour les descendants */
    char nouveauPrefixe[512];
    snprintf(nouveauPrefixe, sizeof(nouveauPrefixe), "%s%s",
             prefixe, estDernier ? "    " : "|   ");

    ListePers* enfants = pers->enfants;
    while (enfants != NULL) {
        int dernier = (enfants->suivant == NULL);
        afficherArbreRecurs(enfants->pers, nouveauPrefixe, dernier);
        enfants = enfants->suivant;
    }
}

void afficherArbre(ListePers* arbre) {
    if (arbre == NULL) {
        printf("  L'arbre est vide.\n");
        return;
    }

    printf("\n================================================\n");
    printf("            ARBRE GENEALOGIQUE\n");
    printf("================================================\n\n");

    /* Afficher d'abord les racines (personnes sans parents connus) */
    ListePers* courant = arbre;
    int nbRacines = 0;
    while (courant != NULL) {
        Pers* p = courant->pers;
        if (p->pere == NULL && p->mere == NULL) {
            afficherLigneNoeud(p);
            ListePers* enfants = p->enfants;
            while (enfants != NULL) {
                int dernier = (enfants->suivant == NULL);
                afficherArbreRecurs(enfants->pers, "", dernier);
                enfants = enfants->suivant;
            }
            printf("\n");
            nbRacines++;
        }
        courant = courant->suivant;
    }

    /* Si aucune racine (tout le monde a des parents dans l'arbre),
       afficher toutes les personnes en liste simple */
    if (nbRacines == 0) {
        printf("  (Aucune racine identifiee - affichage liste)\n\n");
        courant = arbre;
        while (courant != NULL) {
            afficherPers(courant->pers);
            courant = courant->suivant;
        }
    }
}








