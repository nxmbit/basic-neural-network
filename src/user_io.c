#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include "user_io.h"

int input_integer(int range_min, int range_max, const char* input_message) {
    const int buffer_length = 1024;
    char buffer[buffer_length];
    long val = 0;
    int error = 0;
    do {
        printf("%s", input_message);
        while (!fgets(buffer, buffer_length, stdin)) {
            clearerr(stdin); //usuniecie bledu jesli wprowadzono EOF
            printf("\nBlad odczytu danych! Sprobuj ponownie.\n%s", input_message);
        }
        if (!strchr(buffer, '\n')) { // zabezpieczenie, gdyby wprowadzono wiecej znakow niz moze zmiescic sie w buforze
            int ch = 0;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        char *endptr;
        errno = 0;
        val = strtol(buffer, &endptr, 10); //dopuszczone jest jedynie wprowadzanie liczb w postaci dziesietnej
        if (endptr == buffer || *endptr) { //jesli strtol zakonczyl konwersje przed znakiem null oznacza to ze wprowadzone dane sa niepoprawne (np wprowadzono wartosc 55a5)
            printf("Wprowadzono niepoprawna wartosc!\n");
            error = 1;
        } else if (errno == ERANGE || val < range_min || val > range_max || val < INT_MIN || val > INT_MAX) {
            printf("Wprowadzono liczbę spoza dozwolonego zakresu!\n");
            error = 1;
        } else {
            error = 0;
        }
    } while (error);
    return (int) val;
}

char *input_string(int max_len, const char* input_message) {
    const int buffer_length = 1024;
    char *buffer = malloc(buffer_length * sizeof(char));
    do {
        printf("%s", input_message);
        while (!fgets(buffer, buffer_length, stdin)) {
            clearerr(stdin); //usuniecie bledu jesli wprowadzono EOF
            printf("\nBlad odczytu danych! Sprobuj ponownie.\n%s", input_message);
        }
        if (!strchr(buffer, '\n')) { // zabezpieczenie, gdyby wprowadzono wiecej znakow niz moze zmiescic sie w buforze
            int ch = 0;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        if (strlen(buffer) > max_len) {
            printf("Wprowadzono zbyt dluga wartosc!\n");
        }
    } while (strlen(buffer) > max_len);
    const size_t result_length = strlen(buffer) + 1;
    char *result = malloc(result_length * sizeof(char));
    strcpy(result, buffer);
    return result;
}


double input_double(const char* input_message) {
    const int buffer_length = 1024;
    char buffer[buffer_length];
    double val = 0;
    int error = 0;
    do {
        printf("%s", input_message);
        while (!fgets(buffer, buffer_length, stdin)) {
            clearerr(stdin); //usuniecie bledu jesli wprowadzono EOF
            printf("\nBlad odczytu danych! Sprobuj ponownie.\n%s", input_message);
        }
        if (!strchr(buffer, '\n')) { // zabezpieczenie, gdyby wprowadzono wiecej znakow niz moze zmiescic sie w buforze
            int ch = 0;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        char *endptr;
        errno = 0;
        val = strtod(buffer, &endptr);
        if (endptr == buffer || *endptr) { //jesli strtol zakonczyl konwersje przed znakiem null oznacza to ze wprowadzone dane sa niepoprawne (np wprowadzono wartosc 55a5)
            printf("Wprowadzono niepoprawna wartosc!\n");
            error = 1;
        } else if (errno == ERANGE) {
            printf("Wprowadzono liczbę spoza dozwolonego zakresu!\n");
            error = 1;
        } else {
            error = 0;
        }
    } while (error);
    return val;
}