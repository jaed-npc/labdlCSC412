#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Convert a word to its ASCII representation
void convertToAscii(const char* word, int* result) 
{
    int j = 0;
    for (int i = 0; word[i] != '\0' && j < 99; i++) 
    {  
        result[j++] = (int)word[i];
    }
    result[j] = -1;  // End the ASCII array with -1 to mark the end
}

// Compare two rows of ASCII values
int compareRows(int row1[], int row2[]) 
{
    int i = 0;
    while (row1[i] != -1 && row2[i] != -1) // while not at the end of the row
    {
        if (row1[i] < row2[i]) return -1; // compare
        if (row1[i] > row2[i]) return 1;
        i++;
    }
    // Handle case when one word is a prefix of the other
    if (row1[i] == -1 && row2[i] != -1) return -1;
    if (row1[i] != -1 && row2[i] == -1) return 1;
    return 0;
}

// Bubble sort to sort the words based on their ASCII representation
void bubbleSort(int matrix[][100], int wordIndex) 
{
    int i, j;
    bool swapped;
    for (i = 0; i < wordIndex - 1; i++) 
    {
        swapped = false;
        for (j = 0; j < wordIndex - i - 1; j++) 
        {
            if (compareRows(matrix[j], matrix[j + 1]) > 0) 
            {
                int temp[100];
                memcpy(temp, matrix[j], sizeof(temp));
                memcpy(matrix[j], matrix[j + 1], sizeof(temp));
                memcpy(matrix[j + 1], temp, sizeof(temp));
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// Sort the string of words and return the sorted result
char* sort(const char* array) 
{
    int maxLen = strlen(array) + 1;
    int matrix[100][100];  // To store ASCII values of words
    int wordIndex = 0;
    int wordLength = 0;
    char word[100];

    for (int i = 0; i < maxLen; i++) {
        if (array[i] == ' ' || array[i] == '\0') 
        {
            if (wordLength > 0) 
            {
                word[wordLength] = '\0';  // Null-terminate the word
                if (wordIndex < 100)
                {
                    convertToAscii(word, matrix[wordIndex]);
                    wordIndex++;
                }
                wordLength = 0;
            }
            if (array[i] == '\0') break;  // End of input
        } else {
            word[wordLength++] = array[i];  // Build the word
        }
    }

    // Perform the sorting
    bubbleSort(matrix, wordIndex);

    // Allocate memory for the sorted result
    size_t resultSize = maxLen * wordIndex + wordIndex + 1;
    char* result = (char*)malloc(resultSize);
    if (result == NULL) return NULL;
    result[0] = '\0';

    // Construct the result string from sorted words
    for (int i = 0; i < wordIndex; i++) 
    {
        char tempWord[100];
        int j = 0;
        while (matrix[i][j] != -1) 
        {
            
            tempWord[j] = (char)matrix[i][j];
            j++;
        }
        tempWord[j] = '\0';
        
        strcat(result, tempWord);
        if (i < wordIndex - 1) strcat(result, " ");  // Add space between words
    }

    return result;
}

int main() {
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) 
    {
        buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character

        char* result = sort(buffer);  // Sort the input
        if (result != NULL) 
        {

            printf("%s\n", result);  // Print the sorted result
            free(result);  // Free allocated memory
        } else {
            fprintf(stderr, "Error: Memory allocation failed\n");
        }
    }
    return 0;
}
