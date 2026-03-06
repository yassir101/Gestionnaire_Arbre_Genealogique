#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "gestion.h"
#include "files.h"
#include "getset.h"

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    ListePers* arbre = NULL;

    do {
        arbre = menuPrincipal(arbre);
        printf("\nAppuyez sur Entree pour revenir au menu principal...");
        getchar();
        getchar();

        clearScreen();

    } while (1);

    arbre = viderArbre(arbre);

    return 0;
}
