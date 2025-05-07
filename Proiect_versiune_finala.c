#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define CYAN    "\033[36m"
#define RESET   "\033[0m"
#define MENU_COLOR "\033[1;36m"
#define BOLD_MAGENTA "\033[1;35m"

const char* FISIER_ALBUME = "D:/Facultate/Anu 1/Semestrul 2/PP/albume1.txt";
const char* FISIER_POZE = "D:/Facultate/Anu 1/Semestrul 2/PP/poze1.txt";

typedef struct {
    char nume[100];
} Album;

typedef struct {
    char nume[100];
    char album[100];
} Poza;

Album albume[100];
int numar_albume = 0;

Poza poze[1000];
int numar_poze = 0;

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void asteapta_confirmare() {
    char raspuns;
    printf(YELLOW "\nDoriti sa continuati? (y/n): " RESET);
    scanf(" %c", &raspuns);
    if (raspuns == 'n' || raspuns == 'N') {
        printf("\n" RED "La revedere!\n" RESET);
        exit(0);
    }
    clear_screen();
}

void salveaza_albume() {
    FILE* f_albume = fopen(FISIER_ALBUME, "w");
    if (f_albume != NULL) {
        for (int i = 0; i < numar_albume; i++) {
            fprintf(f_albume, "%s\n", albume[i].nume);
        }
        fclose(f_albume);
        printf(GREEN "Datele albumelor au fost salvate.\n" RESET);
    } else {
        printf(RED "Eroare la salvarea albumelor!\n" RESET);
    }
}

void salveaza_poze() {
    FILE* f_poze = fopen(FISIER_POZE, "w");
    if (f_poze != NULL) {
        for (int i = 0; i < numar_poze; i++) {
            fprintf(f_poze, "%s\n%s\n", poze[i].nume, poze[i].album);
        }
        fclose(f_poze);
        printf(GREEN "Datele pozelor au fost salvate.\n" RESET);
    } else {
        printf(RED "Eroare la salvarea pozelor!\n" RESET);
    }
}

void incarca_date() {
    FILE* f_albume = fopen(FISIER_ALBUME, "r");
    if (f_albume != NULL) {
        char linie[100];
        numar_albume = 0;

        while (fgets(linie, sizeof(linie), f_albume) != NULL && numar_albume < 100) {
            size_t len = strlen(linie);
            if (len > 0 && linie[len-1] == '\n') {
                linie[len-1] = '\0';
            }
            strcpy(albume[numar_albume].nume, linie);
            numar_albume++;
        }
        fclose(f_albume);
    }

    FILE* f_poze = fopen(FISIER_POZE, "r");
    if (f_poze != NULL) {
        char linie[200];
        numar_poze = 0;
        while (fgets(linie, sizeof(linie), f_poze) != NULL && numar_poze < 1000) {
            size_t len = strlen(linie);
            if (len > 0 && linie[len-1] == '\n') {
                linie[len-1] = '\0';
            }
            strcpy(poze[numar_poze].nume, linie);

            if (fgets(linie, sizeof(linie), f_poze) == NULL) break;  // albumul
            len = strlen(linie);
            if (len > 0 && linie[len-1] == '\n') {
                linie[len-1] = '\0';
            }
            strcpy(poze[numar_poze].album, linie);

            numar_poze++;
        }
        fclose(f_poze);
    }
    printf(BLUE "Date incarcate: %d albume si %d poze.\n" RESET, numar_albume, numar_poze);
}

int cauta_album(const char* nume) {
    for (int i = 0; i < numar_albume; i++) {
        if (strcmp(albume[i].nume, nume) == 0) {
            return i;
        }
    }
    return -1;
}

int validare_nume(const char *nume) {
    for (int i = 0; nume[i]; i++) {
        if (!isalnum(nume[i]) && nume[i] != '_' && nume[i] != '-' && nume[i] != ',') {
            return 0;
        }
    }
    return 1;
}

void creare_album() {
    char nume_album[100];
    printf("\n=== Creare Album ===\n");
    printf("Introduceti numele albumului: ");
    scanf("%99s", nume_album);

    if (!validare_nume(nume_album)) {
        printf(RED "Nume invalid! Folositi doar litere, cifre, '_' sau '-'.\n\n" RESET);
    } else if (cauta_album(nume_album) >= 0) {
        printf(YELLOW "Albumul '%s' exista deja!\n\n" RESET, nume_album);
    } else if (numar_albume < 100) {
        strcpy(albume[numar_albume].nume, nume_album);
        numar_albume++;
        salveaza_albume();
        printf(GREEN "Albumul '%s' a fost creat cu succes!\n\n" RESET, nume_album);
    } else {
        printf(RED "Nu se mai pot crea albume! Limita maxima a fost atinsa.\n\n" RESET);
    }

    asteapta_confirmare();
}

void vizualizare_albume() {
    printf("\n=== Albume Disponibile ===\n");
    if (numar_albume == 0) {
        printf(YELLOW "Nu exista albume.\n" RESET);
    } else {
        for (int i = 0; i < numar_albume; i++) {
            printf(GREEN "%d. %s\n" RESET, i + 1, albume[i].nume);
        }
    }
    asteapta_confirmare();
}

void adauga_poza_in_album() {
    char nume_poza[100], nume_album[100];
    printf("\n=== Adaugare Poza in Album ===\n");

    printf("Introduceti numele pozei: ");
    scanf("%99s", nume_poza);

    printf("Introduceti albumul in care adaugati poza: ");
    scanf("%99s", nume_album);

    if (!validare_nume(nume_poza) || !validare_nume(nume_album)) {
        printf(RED "Nume invalid! Folositi doar litere, cifre, '_' sau '-' sau ','.\n\n" RESET);
    } else if (cauta_album(nume_album) < 0) {
        printf(RED "Albumul '%s' nu exista!\n\n" RESET, nume_album);
    } else if (numar_poze < 1000) {
        strcpy(poze[numar_poze].nume, nume_poza);
        strcpy(poze[numar_poze].album, nume_album);
        numar_poze++;
        salveaza_poze();
        printf(GREEN "Poza '%s' a fost adaugata in albumul '%s'.\n\n" RESET, nume_poza, nume_album);
    } else {
        printf(RED "Nu se mai pot adauga poze! Limita maxima a fost atinsa.\n\n" RESET);
    }

    asteapta_confirmare();
}

void vizualizare_poze_album() {
    char nume_album[100];
    printf("\n=== Poze dintr-un Album ===\n");
    printf("Introduceti numele albumului: ");
    scanf("%99s", nume_album);

    if (!validare_nume(nume_album)) {
        printf(RED "Nume invalid!\n\n" RESET);
    } else if (cauta_album(nume_album) < 0) {
        printf(RED "Albumul '%s' nu exista!\n\n" RESET, nume_album);
    } else {
        int gasit = 0;
        printf(CYAN "Poze in albumul '%s':\n" RESET, nume_album);
        for (int i = 0; i < numar_poze; i++) {
            if (strcmp(poze[i].album, nume_album) == 0) {
                printf(GREEN "- %s\n" RESET, poze[i].nume);
                gasit = 1;
            }
        }
        if (!gasit) {
            printf(YELLOW "Nu exista poze in acest album.\n" RESET);
        }
    }

    asteapta_confirmare();
}

void dimensiune_album() {
    char nume_album[100];
    printf("\n=== Dimensiune Album ===\n");
    printf("Introduceti numele albumului: ");
    scanf("%99s", nume_album);

    if (!validare_nume(nume_album)) {
        printf(RED "Nume invalid!\n\n" RESET);
    } else if (cauta_album(nume_album) < 0) {
        printf(RED "Albumul '%s' nu exista!\n\n" RESET, nume_album);
    } else {
        int count = 0;
        for (int i = 0; i < numar_poze; i++) {
            if (strcmp(poze[i].album, nume_album) == 0) {
                count++;
            }
        }
        printf(CYAN "Albumul '%s' contine %d poza(e).\n\n" RESET, nume_album, count);
    }

    asteapta_confirmare();
}

void sterge_album() {
    char nume_album[100];
    printf("\n=== Sterge Album ===\n");
    printf("Introduceti numele albumului: ");
    scanf("%99s", nume_album);

    int index = cauta_album(nume_album);

    if (!validare_nume(nume_album)) {
        printf(RED "Nume invalid!\n\n" RESET);
    } else if (index < 0) {
        printf(RED "Albumul '%s' nu exista!\n\n" RESET, nume_album);
    } else {
        // Ștergere poze asociate
        int j = 0;
        for (int i = 0; i < numar_poze; i++) {
            if (strcmp(poze[i].album, nume_album) != 0) {
                poze[j++] = poze[i];
            }
        }
        numar_poze = j;

        // Ștergere album
        for (int i = index; i < numar_albume - 1; i++) {
            albume[i] = albume[i + 1];
        }
        numar_albume--;

        salveaza_albume();
        salveaza_poze();
        printf(GREEN "Albumul '%s' si pozele asociate au fost sterse.\n\n" RESET, nume_album);
    }

    asteapta_confirmare();
}

int main() {
    int optiune;

    incarca_date();

    do {
        clear_screen();
        printf(MENU_COLOR"=== MENIU PRINCIPAL ===\n"RESET);
        printf(MENU_COLOR"1. Creaza album\n");
        printf(MENU_COLOR"2. Adauga poza intr-un album\n"RESET);
        printf(MENU_COLOR"3. Vizualizare albume\n"RESET);
        printf(MENU_COLOR"4. Vizualizare poze dintr-un album\n"RESET);
        printf(MENU_COLOR"5. Vezi dimensiune album\n"RESET);
        printf(MENU_COLOR"6. Sterge album\n"RESET);
        printf(MENU_COLOR"0. Iesire\n"RESET);
        printf(BOLD_MAGENTA"\nAlegeti o optiune: "RESET);
        scanf("%d", &optiune);
        clear_screen();

        switch (optiune) {
            case 1:
                creare_album();
            break;
            case 2:
                adauga_poza_in_album();
            break;
            case 3:
                vizualizare_albume();
            break;
            case 4:
                vizualizare_poze_album();
            break;
            case 5:
                dimensiune_album();
            break;
            case 6:
                sterge_album();
            break;
            case 0:
                printf(RED"La revedere!\n"RESET);
            break;
            default:
                printf(RED"Optiune invalida! Incercati din nou.\n\n"RESET);
            asteapta_confirmare();
        }
    } while (optiune != 0);

    return 0;
}