/** \file main.c
 * \brief Hlavní program realizující třídění komplexních čísel
 */

#include <stdio.h>
#include <time.h>
#include "basic_sorts.h"
#include "check.h"

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));

    struct TVector vector = { 0 };

    FILE* fsrc = fopen("unsorted_data_complex.txt", "r");
    if (!fsrc) {
        perror("Nepodarilo se otevrit vstupni soubor");
        return 1;
    }

    if (!vector_init_file(&vector, fsrc)) {
        fclose(fsrc);
        fprintf(stderr, "Chyba pri inicializaci vektoru ze souboru.\n");
        return 2;
    }
    fclose(fsrc);

    printf("Nactena data:\n");
    vector_for_each(vector_iterator_begin(&vector), print_element);
    putchar('\n');

    printf("\n--- Trideni dle absolutni hodnoty (Select Sort) ---\n");
    vector_element_set_comparator(EByAbs);
    select_sort(&vector);
    vector_for_each(vector_iterator_begin(&vector), print_element);
    putchar('\n');

    printf("\n--- Trideni dle imaginarni slozky (Shaker Sort) ---\n");
    vector_element_set_comparator(EByIm);
    shaker_sort(&vector);
    vector_for_each(vector_iterator_begin(&vector), print_element);
    putchar('\n');

    // Musime pouzit stabilni metodu (Insert/Shaker/Bubble), aby se nerozbilo trideni
    printf("\n--- Stabilni trideni dle realne slozky (Insert Sort) ---\n");
    vector_element_set_comparator(EByRe);
    insert_sort(&vector);
    vector_for_each(vector_iterator_begin(&vector), print_element);
    putchar('\n');

    FILE* fdst = fopen("sorted_data_complex.txt", "w");
    if (fdst) {
        vector_store_file(&vector, fdst);
        fclose(fdst);
        printf("\nData byla uspesne ulozena do sorted_data_complex.txt\n");
    }
    else {
        fprintf(stderr, "Chyba pri zapisu do vystupniho souboru.\n");
        vector_destroy(&vector);
        return 3;
    }

    vector_destroy(&vector);
    return 0;
}