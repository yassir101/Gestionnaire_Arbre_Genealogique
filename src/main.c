#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "gestion.h"

static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    ListePers* arbre = NULL;

    clearScreen();
    arbre = menuPrincipal(arbre);
    arbre = viderArbre(arbre);

    return 0;
}
