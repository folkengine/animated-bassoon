#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#define LOG_ME (0==1)

// COMMON MACROS

#define ARRAY_OF_STRINGS_SIZE(x) ( sizeof(x) / sizeof((x)[0]) )

/**
 * Safe asprintf macro
 *
 * http://modelingwithdata.org/arch/00000062.htm
 * https://github.com/b-k/21st-Century-Examples/blob/master/sasprintf.c
 */
#define Sasprintf(write_to,  ...) {           \
    char *tmp_string_for_extend = (write_to); \
    asprintf(&(write_to), __VA_ARGS__);       \
    free(tmp_string_for_extend);              \
}

// CONSTANTS

const int ROMAN_VALUES[26] = {0, 0, 100, 500, 0, 0, 0, 0, 1, 0, 0, 50, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 10, 0, 0};

const char *sieve[] = {"M","CM","D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};

// Methods

static int get_char_arabic_value(char roman_numeral)
{
    roman_numeral = toupper(roman_numeral);
    roman_numeral = roman_numeral - 'A';
    if (roman_numeral > 25) {
        return 0;
    }
    return ROMAN_VALUES[(unsigned char) roman_numeral];
}

static int get_arabic_value(char *roman_numerals)
{
    int sum = 0;
    size_t len = strlen(roman_numerals);
    int last_char = 0;

    while(len--) {
        int arabic_value = get_char_arabic_value(roman_numerals[len]);

        if (last_char > arabic_value) {
            sum -= arabic_value;
        } else {
            sum += arabic_value;
            last_char = arabic_value;
        }
    }
    return sum;
}

static char * get_roman_value(int arabic)
{
    if (arabic < 1)
        return "";

    char *roman_numerals = strdup("");
    int sieve_size = ARRAY_OF_STRINGS_SIZE(sieve);

    int i;
    for (i = 0; i < sieve_size; i++ ) {
        int sieve_arabic_value = get_arabic_value((char*)sieve[i]);

        while (sieve_arabic_value <= arabic) {

            if (LOG_ME) {
                openlog("roman", LOG_PID | LOG_CONS, LOG_USER);
                syslog(LOG_WARNING, "sieve:%d arabic:%d roman:%s", sieve_arabic_value, arabic, roman_numerals);
                closelog();
            }

            Sasprintf(roman_numerals, "%s%s", roman_numerals, sieve[i]);
            arabic = arabic - sieve_arabic_value;
        }
    }
    return roman_numerals;
}

char * add_roman(char *augend, char *addend)
{
    return get_roman_value(get_arabic_value(augend) + get_arabic_value(addend));
}

char * subtract_roman(char *minuend, char *subtrahend)
{
    return get_roman_value(get_arabic_value(minuend) - get_arabic_value(subtrahend));
}

