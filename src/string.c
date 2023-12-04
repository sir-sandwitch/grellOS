#ifndef STRING_INCL
#define STRING_INCL
#include <string.h>
#endif

int strlen(char str[]) {
    // initializing count variable (stores the length of the string)
    int count;

    // incrementing the count till the end of the string
    for (count = 0; str[count] != '\0'; ++count);

    // returning the character count of the string
    return count;
}

char* strcat(char* str1, char* str2) {
    char str3[256];
    int i = 0, j = 0;

    while (str1[i] != '\0') {
        str3[j++] = str1[i++];
    }

    // Insert the second string in the new string
    i = 0;
    while (str2[i] != '\0') {
        str3[j++] = str2[i++];
    }
    str3[j] = '\0';

    // Copy the concatenated string back to str1
    for (i = 0; str3[i] != '\0'; ++i) {
        str1[i] = str3[i];
    }
    str1[i] = '\0';

    return str1;
}

char* strncat(char* str1, char* str2, int n) {
    int i, j;

    for (i = 0; str1[i] != '\0'; ++i) {
    }

    for (j = 0; j < n && str2[j] != '\0'; ++j) {
        str1[i++] = str2[j];
    }
    str1[i] = '\0';

    return str1;
}

char* strcpy(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] != '\0'; ++i) {
        s2[i] = s1[i];
    }

    s2[i] = '\0';

    return s2;
}

void reverse(char* str) {
    int length = strlen(str);
    int i, j;
    char temp;

    for (i = 0, j = length - 1; i < j; ++i, --j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

char* ltoa(long num, char* str, int base) {
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Handle negative numbers
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    // Convert individual digits
    while (num != 0) {
        long remainder = num % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        num = num / base;
    }

    // Append negative sign for base 10
    if (isNegative && base == 10)
        str[i++] = '-';

    // Null-terminate the string
    str[i] = '\0';

    // Reverse the string
    reverse(str);

    return str;
}