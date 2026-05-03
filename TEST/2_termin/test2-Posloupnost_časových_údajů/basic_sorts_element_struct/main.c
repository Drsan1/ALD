#include <stdio.h>
#include <time.h>
#include "basic_sorts.h"
#include "main_support.h"
#include "check.h"

int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));

    struct TVector vector = { 0 };

    FILE* fsrc = fopen("data_time.txt", "r");
    if (!fsrc)
        return 1;

    bool initialized = vector_init_file(&vector, fsrc);
    fclose(fsrc); 

    if (!initialized)
        return 3;

    printf("Neserazena data:\n");
    vector_for_n(vector_iterator_begin(&vector), 20, print_element);
    putchar('\n');

    // --- VZESTUPNÉ TŘÍDĚNÍ ---
    vector_element_set_comparator(EAscending);
    select_sort(&vector); 

    printf("Vzestupne serazeno:\n");
    vector_for_n(vector_iterator_begin(&vector), 20, print_element);
    putchar('\n');

    FILE* f_asc = fopen("ascending_data.txt", "w");
    if (f_asc) {
        vector_store_file(&vector, f_asc);
        fclose(f_asc);
    }

    // --- SESTUPNÉ TŘÍDĚNÍ ---
    vector_element_set_comparator(EDescending);
    select_sort(&vector); 

    printf("Sestupne serazeno:\n");
    vector_for_n(vector_iterator_begin(&vector), 20, print_element);
    putchar('\n');

    FILE* f_desc = fopen("descending_data.txt", "w");
    if (f_desc) {
        vector_store_file(&vector, f_desc);
        fclose(f_desc);
    }

    vector_destroy(&vector);
    return 0;
}