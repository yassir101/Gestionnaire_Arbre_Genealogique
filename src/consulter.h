#ifndef CONSULTER_H
#define CONSULTER_H

#include "types.h"
#include "getset.h"

ListePers* trouverNeveuNiece(ListePers* arbre, Pers* pers);
ListePers* trouverConjoint(ListePers* arbre, Pers* pers);
ListePers* trouverCousin(ListePers* arbre, Pers* pers);
ListePers* trouverOncleTante(ListePers* arbre, Pers* pers);
ListePers* trouverPetitsEnfants(ListePers* arbre, Pers* pers);
ListePers* trouverGrandsParents(ListePers* arbre, Pers* pers);
ListePers* trouverFratrie(ListePers* arbre, Pers* pers);
ListePers* trouverMere(ListePers* arbre, Pers* pers);
ListePers* trouverPere(ListePers* arbre, Pers* pers);
void afficherPers(Pers* pers);
void consulterMembre(ListePers* arbre);
void afficherLien(ListePers* arbre, Pers* pers1, Pers* pers2);
void consulterLien(ListePers* arbre);
void afficherTLM(ListePers* arbre);
void afficherEnfantsRecurs(ListePers* arbre, Pers* pers);
void afficherDescendance(ListePers* arbre, Pers* personne);
void afficherParentRecurs(ListePers* arbre, Pers* pers);
void afficherAscendance(ListePers* arbre, Pers* personne);
void affichage(ListePers* arbre);

#endif  // CONSULTER_H
