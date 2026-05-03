/** \file basic_sorts.c
 * \brief Implementace API přímých metod třídění
 * \author Petyovský (upraveno)
 * \version 2026
 * $Id: basic_sorts.c 3260 2026-03-16 16:21:51Z petyovsky $
 */

#include "basic_sorts.h"
#include <stdio.h>

void print_element(const struct TVectorIterator* aIter)
{
    if (!vector_element_store_file(vector_iterator_value(aIter), stdout))
        fputs("Function 'vector_element_store_file()' failed!\n", stderr);
}

void insert_sort(struct TVector* aVector)
{
    if (aVector == NULL || vector_size(aVector) < 2)
        return;

    for (size_t i = 1; i < vector_size(aVector); i++)
    {
        TVectorElement tmp = vector_value(aVector, i);
        size_t j = i;

        // Posouvání prvků doprava, dokud nenajdeme správné místo pro 'tmp'
        // Použití <= 0 zajišťuje stabilitu algoritmu
        while (j > 0 && vector_compare_position_value(aVector, j - 1, tmp) > 0)
        {
            vector_move_positions(aVector, j, aVector, j - 1);
            j--;
        }
        vector_set_value(aVector, j, tmp);
    }
}

void select_sort(struct TVector* aVector)
{
    if (aVector == NULL || vector_size(aVector) < 2)
        return;

    size_t n = vector_size(aVector);
    for (size_t i = 0; i < n - 1; i++)
    {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; j++)
        {
            if (vector_compare_positions(aVector, j, aVector, min_idx) < 0)
                min_idx = j;
        }

        if (min_idx != i)
            vector_swap_positions(aVector, i, aVector, min_idx);
    }
}

void bubble_sort(struct TVector* aVector)
{
    if (aVector == NULL || vector_size(aVector) < 2)
        return;

    size_t n = vector_size(aVector);
    for (size_t i = 0; i < n - 1; i++)
    {
        for (size_t j = 0; j < n - i - 1; j++)
        {
            if (vector_compare_positions(aVector, j, aVector, j + 1) > 0)
                vector_swap_positions(aVector, j, aVector, j + 1);
        }
    }
}

void bubble_sort_optimized(struct TVector* aVector)
{
    if (aVector == NULL || vector_size(aVector) < 2)
        return;

    size_t n = vector_size(aVector);
    for (size_t i = 0; i < n - 1; i++)
    {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; j++)
        {
            if (vector_compare_positions(aVector, j, aVector, j + 1) > 0)
            {
                vector_swap_positions(aVector, j, aVector, j + 1);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

void bubble_sort_optimized2(struct TVector* aVector)
{
    if (aVector == NULL || vector_size(aVector) < 2)
        return;

    size_t limit = vector_size(aVector) - 1;
    while (limit > 0)
    {
        size_t last_swap = 0;
        for (size_t j = 0; j < limit; j++)
        {
            if (vector_compare_positions(aVector, j, aVector, j + 1) > 0)
            {
                vector_swap_positions(aVector, j, aVector, j + 1);
                last_swap = j;
            }
        }
        // Vše za posledním swapem už je seřazeno
        limit = last_swap;
    }
}

void shaker_sort(struct TVector* aVector)
{
    if (aVector == NULL || vector_size(aVector) < 2)
        return;

    size_t left = 0;
    size_t right = vector_size(aVector) - 1;

    while (left < right)
    {
        size_t last_swap = left;

        // Průchod doprava (hledání maxima)
        for (size_t i = left; i < right; i++)
        {
            if (vector_compare_positions(aVector, i, aVector, i + 1) > 0)
            {
                vector_swap_positions(aVector, i, aVector, i + 1);
                last_swap = i;
            }
        }
        right = last_swap;

        // Pokud se pravá hranice posunula až na začátek, je hotovo
        if (left >= right)
            break;

        // Průchod doleva (hledání minima)
        for (size_t i = right; i > left; i--)
        {
            if (vector_compare_positions(aVector, i - 1, aVector, i) > 0)
            {
                vector_swap_positions(aVector, i - 1, aVector, i);
                last_swap = i;
            }
        }
        left = last_swap;
    }
}