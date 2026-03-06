#ifndef MENUS_H
#define MENUS_H

#include "types.h"

/* Menu principal - retourne l'arbre (potentiellement modifie) */
ListePers* menuPrincipal(ListePers* arbre);

/* Sous-menus */
void       menuModifier(ListePers* arbre);
void       menuLiens(ListePers* arbre);
ListePers* menuSupprimer(ListePers* arbre);
void       menuConsulter(ListePers* arbre);
ListePers* menuCharger(ListePers* arbre);
void       menuSauvegarder(ListePers* arbre);
void       menuLivret(ListePers* arbre);

/* Quitter avec sauvegarde optionnelle */
ListePers* quitterSauvegarder(ListePers* arbre);

#endif /* MENUS_H */
