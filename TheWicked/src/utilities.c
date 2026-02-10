#include "utilities.h"
#include <time.h>
#include <stdio.h>

// ---------------------------------------------------------
// OS-SPECIFIC LIBRARIES
// ---------------------------------------------------------
// This block ensures the code compiles on both Windows and Mac/Linux.
#ifdef _WIN32
    #include <windows.h> // Windows specific
#else
    #include <unistd.h>  // Unix/Linux specific
#endif

// ---------------------------------------------------------
// CURSOR MANAGEMENT
// ---------------------------------------------------------

// Function: gotoxy
// Purpose: Moves the blinking cursor to a specific coordinate (X, Y).
// Essential for drawing the UI in specific places (centering, tables, etc).
void gotoxy(int x, int y) {
    #ifdef _WIN32
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #else
        // ANSI Escape code for Unix terminals
        printf("\033[%d;%dH", y, x);
    #endif
}

// Function: clearScreen
// Purpose: Wipes the terminal clean. Used between menus.
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function: pauseExecution
// Purpose: Stops the program for X milliseconds. Used for animations.
void pauseExecution(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Function: clearInputBuffer
// Purpose: Removes leftover 'Enter' keys from the keyboard buffer.
// Prevents the program from skipping inputs (a common C bug).
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ---------------------------------------------------------
// INPUT HANDLING
// ---------------------------------------------------------

// Function: getIntInput
// Purpose: Safely gets an integer from the user.
// Features: 
// 1. Validates range (min/max).
// 2. Checks if input is actually a number.
// 3. Locks the cursor position (x, y) so errors don't ruin the UI layout.
int getIntInput(int x, int y, const char* prompt, int min, int max) {
    int value;
    char buffer[100];
    
    while (1) {
        // 1. Force Cursor to the specific input location
        gotoxy(x, y);
        
        // 2. Clear the line before printing prompt (removes old bad input)
        printf("                                                  "); 
        gotoxy(x, y);
        
        printf("%s", prompt);
        
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            // Remove newline
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n') buffer[len-1] = '\0';

            // Check if input is a number using sscanf
            if (sscanf(buffer, "%d", &value) == 1) {
                // Check if number is within allowed range (e.g. 1-4)
                if (value >= min && value <= max) {
                    // Valid! Clear the error line below just in case
                    gotoxy(0, y+1); 
                    printf("                                                                                ");
                    return value;
                } else {
                    // Out of Range Error - Printed Centered below prompt
                    gotoxy(30, y+1); 
                    printf(COLOR_RED "    [!] Input out of range (%d-%d)." COLOR_RESET, min, max);
                }
            } else {
                // Invalid Input Error - Printed Centered below prompt
                gotoxy(30, y+1);
                printf(COLOR_RED "        [!] Invalid input. Numbers only." COLOR_RESET);
            }
            
            // Wait so user sees the red error message
            pauseExecution(1000);
            
            // Clear the error message line so the screen stays clean
            gotoxy(0, y+1); 
            printf("                                                                                ");
            
            // LOOP REPEATS: It will go back to gotoxy(x,y) and repaint the prompt perfectly.
        }
    }
}

// Function: getStringInput
// Purpose: Safely gets a string from the user (e.g., Seat Code "A1").
// Removes the annoying newline character automatically.
void getStringInput(const char* prompt, char* buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0'; 
        }
    }
}
