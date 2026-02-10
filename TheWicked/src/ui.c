#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"
#include "tickets.h" 
#include "utilities.h"

// Function: printCentered
// Purpose: A helper to print text perfectly in the middle of a 100-character wide screen.
// Formula: X = (100 - TextLength) / 2
void printCentered(int y, const char* text, const char* color) {
    int len = strlen(text);
    int x = (SCREEN_WIDTH - len) / 2;
    if (x < 0) x = 0;

    gotoxy(x, y);
    printf("%s%s%s", color, text, COLOR_RESET);
}

// Function: printHeader
// Purpose: Draws the standard purple/yellow box at the top of every screen.
void printHeader(const char* title) {
    clearScreen();
    // Draws the box borders
    printCentered(2, "o======================================================o", COLOR_MAGENTA);
    printCentered(3, "|                                                      |", COLOR_MAGENTA);
    // Draws the title inside
    printCentered(4, title, COLOR_YELLOW);
    printCentered(5, "|                                                      |", COLOR_MAGENTA);
    printCentered(6, "o======================================================o", COLOR_MAGENTA);
}

// Function: printDivider
// Purpose: Draws a decorative line to separate sections of a menu.
void printDivider(int y) {
    printCentered(y, "+--------------------------------------------+", COLOR_MAGENTA);
}

// Function: showSplashScreen
// Purpose: The "Loading Screen" logo displayed when the program starts.
void showSplashScreen() {
    clearScreen();
    // ASCII Art box
    printCentered(6, ".======================================================.", COLOR_MAGENTA);
    printCentered(7, "||                                                      ||", COLOR_MAGENTA);
    printCentered(8, "||                                                      ||", COLOR_MAGENTA);
    printCentered(9, "||                                                      ||", COLOR_MAGENTA);
    printCentered(10,"||                                                      ||", COLOR_MAGENTA);
    printCentered(11,"||                                                      ||", COLOR_MAGENTA);
    printCentered(12,"'======================================================'", COLOR_MAGENTA);
    // Text inside
    printCentered(9, "THE WICKED GOOD", COLOR_CYAN);
    printCentered(4, "WELCOME TO THE CINEMA OF CIT-U", COLOR_WHITE);
    printCentered(15, "[ PRESS ENTER TO ENTER THE VOID ]", COLOR_YELLOW);
    gotoxy(0, 18);
    getchar(); 
}

// Function: showLoadingAnimation
// Purpose: Prints a message followed by 3 dots (...) with delays to simulate work.
void showLoadingAnimation(const char* message) {
    int len = strlen(message) + 4; 
    int x = (SCREEN_WIDTH - len) / 2;
    gotoxy(x, 15); 
    printf(COLOR_CYAN "%s", message);
    fflush(stdout);
    int i;
    for(i = 0; i < 3; i++) {
        pauseExecution(400); // Wait 0.4 seconds per dot
        printf(".");
        fflush(stdout);
    }
    printf(COLOR_RESET "\n");
    pauseExecution(300);
}

// Function: printMovieInfo
// Purpose: Displays static text about the movie (Plot, Price, etc).
void printMovieInfo() {
    printHeader("MOVIE INFORMATION");
    printCentered(9,  "Title: THE WICKED GOOD", COLOR_YELLOW);
    printCentered(10, "Genre: Fantasy / Musical", COLOR_YELLOW);
    printCentered(11, "Runtime: 2h 15m", COLOR_YELLOW);
    printCentered(12, "Rating: R-16", COLOR_YELLOW);
    
    char priceStr[50];
    sprintf(priceStr, "VIP: PHP %.2f | REG: PHP %.2f", PRICE_VIP, PRICE_REG);
    printCentered(13, priceStr, COLOR_YELLOW);
    
    printCentered(15, "Synopsis:", COLOR_MAGENTA);
    printCentered(16, "Wicked tells the untold story of Elphaba, a misunderstood", COLOR_WHITE);
    printCentered(17, "green-skinned girl who forms an unlikely friendship with", COLOR_WHITE);
    printCentered(18, "the popular Glinda. As Elphaba uncovers corruption in Oz", COLOR_WHITE);
    printCentered(19, "and stands up for justice, she becomes labeled the Wicked Witch,", COLOR_WHITE);
    printCentered(20, "revealing that heroes and villains aren't always what they seem.", COLOR_WHITE);
    
    printDivider(22);
    printCentered(24, "[Press Enter to return]", COLOR_GREEN);
    getchar(); 
}

// Function: showRoleSelection
// Purpose: The first menu asking if the user is a Guest or Admin.
// Uses manual coordinates to align the list 1-3 perfectly vertically.
int showRoleSelection() {
    printHeader("IDENTITY VERIFICATION");
    
    printCentered(9, "Who approaches the gate?", COLOR_CYAN);
    
    gotoxy(37, 11); printf(COLOR_WHITE "1. Guest (Buy Tickets)" COLOR_RESET);
    gotoxy(37, 12); printf(COLOR_WHITE "2. Cinema Manager (Admin)" COLOR_RESET);
    gotoxy(37, 13); printf(COLOR_WHITE "3. Exit System" COLOR_RESET);
    
    printDivider(15);
    
    // Returns 1, 2, or 3
    return getIntInput(41, 17, COLOR_YELLOW "Select Identity > " COLOR_RESET, 1, 3);
}

// Function: selectShowtime
// Purpose: Displays the 4 movie times and returns the index (0-3).
int selectShowtime(char* buffer) {
    clearScreen();
    printHeader("SELECT SHOWTIME");
    
    // Fixed X coordinate to align the options nicely
    gotoxy(39, 9);  printf(COLOR_WHITE "1. 10:30 AM (Matinee)" COLOR_RESET);
    gotoxy(39, 10); printf(COLOR_WHITE "2. 01:15 PM (Afternoon)" COLOR_RESET);
    gotoxy(39, 11); printf(COLOR_WHITE "3. 04:45 PM (Prime)" COLOR_RESET);
    gotoxy(39, 12); printf(COLOR_WHITE "4. 08:00 PM (Evening)" COLOR_RESET);
    
    printDivider(14);
    
    int choice = getIntInput(43, 16, COLOR_YELLOW "Select Time > " COLOR_RESET, 1, 4);
    
    // Copy the string to display on the ticket later
    if (choice == 1) strcpy(buffer, "10:30 AM");
    else if (choice == 2) strcpy(buffer, "01:15 PM");
    else if (choice == 3) strcpy(buffer, "04:45 PM");
    else strcpy(buffer, "08:00 PM");
    
    clearScreen();
    showLoadingAnimation("Setting Projector");
    return choice - 1; // Convert 1-4 to 0-3 index
}

// Function: buyConcessions
// Purpose: A sub-menu for buying snacks. It loops until the user finishes ordering.
// Returns the total cost of all snacks bought.
float buyConcessions() {
    float totalSnacks = 0.0;
    int buying = 1;
    while (buying) {
        clearScreen();
        printHeader("CONCESSION STAND");
        // Display Menu with Prices
        printCentered(8,  "1. Salted Popcorn ... PHP 150.00", COLOR_YELLOW);
        printCentered(9,  "2. Large Soda ....... PHP 80.00", COLOR_YELLOW);
        printCentered(10, "3. Mineral Water .... PHP 40.00", COLOR_YELLOW);
        printCentered(11, "4. Wicked T-Shirt ... PHP 500.00", COLOR_CYAN);
        printCentered(12, "5. Finish Order", COLOR_GREEN);
        
        printDivider(14);
        
        // Show running total
        char totalStr[50];
        sprintf(totalStr, "Current Extra Total: PHP %.2f", totalSnacks);
        printCentered(16, totalStr, COLOR_MAGENTA);
        
        int choice = getIntInput(41, 18, COLOR_YELLOW "Select Item > " COLOR_RESET, 1, 5);
        
        // Add prices to total
        if (choice == 1) { totalSnacks += 150.00; showLoadingAnimation("Popping Corn"); }
        else if (choice == 2) { totalSnacks += 80.00;  showLoadingAnimation("Filling Cup"); }
        else if (choice == 3) { totalSnacks += 40.00;  showLoadingAnimation("Getting Water"); }
        else if (choice == 4) { totalSnacks += 500.00; showLoadingAnimation("Wrapping Merch"); }
        else if (choice == 5) { buying = 0; }
    }
    return totalSnacks;
}

// Function: showSeatMap
// Purpose: Draws the visual grid of seats. It colors them Green (Available) or Red (Sold).
// It checks the specific 'showtimeIndex' to see which seats are taken for that time.
void showSeatMap(int showtimeIndex) {
    clearScreen();
    printHeader("SEAT AVAILABILITY");

    // Draw Screen graphic
    printCentered(8, "________________________________________________________", COLOR_CYAN);
    printCentered(9, "|                      C I N E M A                     |", COLOR_CYAN);
    printCentered(10,"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", COLOR_CYAN);
    printCentered(11,"[                     S C R E E N                      ]", COLOR_CYAN);

    int r, c;
    for(r = 0; r < ROWS; r++) {
        char rowLabel[20];
        char rowColor[10];
        
        // Differentiate Row A (VIP)
        if (r == 0) { strcpy(rowLabel, "Row A (VIP)"); strcpy(rowColor, COLOR_YELLOW); } 
        else { sprintf(rowLabel, "Row %c      ", 'A' + r); strcpy(rowColor, COLOR_WHITE); }

        int startX = 28; int y = 14 + (r * 2); 
        gotoxy(startX, y); printf("%s%s  ", rowColor, rowLabel);

        // Loop columns to draw [A1] [A2] etc.
        for(c = 0; c < COLS; c++) {
            // Check status in 3D Matrix
            if (isSeatBooked(r, c, showtimeIndex)) { printf(COLOR_RED "[%c%d] " COLOR_RESET, 'A'+r, c+1); } 
            else { printf(COLOR_GREEN "[%c%d] " COLOR_RESET, 'A'+r, c+1); }
        }
    }

    // Legend
    gotoxy(34, 23); printf(COLOR_MAGENTA "Status: " COLOR_GREEN "[Available]  " COLOR_RED "    [Sold Out]" COLOR_RESET);
    gotoxy(34, 24); printf(COLOR_MAGENTA "Pricing: " COLOR_YELLOW "VIP PHP %.2f  " COLOR_WHITE "REG PHP %.2f" COLOR_RESET, PRICE_VIP, PRICE_REG);
    
    printDivider(26);
    gotoxy(32, 27); printf("[Press Enter to proceed to booking]");
    getchar();
}

// Function: getTicketTypeInput
// Purpose: Asks the user if they want VIP (Row A) or Regular (Rows B-D).
int getTicketTypeInput() {
    clearScreen();
    printHeader("SELECT TICKET CLASS");
    
    char vipStr[50], regStr[50];
    sprintf(vipStr, "1. VIP EXPERIENCE (Row A) - PHP %.2f", PRICE_VIP);
    sprintf(regStr, "2. REGULAR SEATING (Row B-D) - PHP %.2f", PRICE_REG);
    
    gotoxy(30, 9);  printf("%s%s%s", COLOR_YELLOW, vipStr, COLOR_RESET);
    gotoxy(30, 11); printf("%s%s%s", COLOR_WHITE, regStr, COLOR_RESET);
    
    printDivider(14);
    return getIntInput(41, 16, COLOR_YELLOW "Select Class > " COLOR_RESET, 1, 2);
}

// Function: manualSeatSelect
// Purpose: Allows user to type seat codes manually (e.g. "A1").
// It includes error checking for Invalid Input, Sold Seats, and Wrong Class.
void manualSeatSelect(int qty, int ticketType, int showtimeIndex, SeatSelection* outputSeats) {
    clearScreen();
    printHeader("MANUAL SEAT SELECTION");
    if (ticketType == TYPE_VIP) printCentered(8, "Mode: VIP (Select seats in Row A)", COLOR_YELLOW);
    else printCentered(8, "Mode: REGULAR (Select seats in Rows B-D)", COLOR_WHITE);

    int count = 0;
    while (count < qty) {
        char input[10]; char prompt[50];
        int inputX = 37; int inputY = 10 + count;

        gotoxy(inputX, inputY); sprintf(prompt, "Enter Seat for Ticket %d: ", count + 1);
        getStringInput(prompt, input, sizeof(input));
        
        // --- Validation Logic ---
        if (strlen(input) < 2) { 
            gotoxy(inputX+30, inputY); printf(COLOR_RED "Invalid" COLOR_RESET); 
            pauseExecution(800); gotoxy(inputX, inputY); printf("                                            "); 
            continue; 
        }

        char rowChar = toupper(input[0]); int rowIdx = rowChar - 'A'; int colIdx = atoi(&input[1]) - 1; 

        // Check if seat is within bounds (A-D, 1-6)
        if (rowIdx < 0 || rowIdx >= ROWS || colIdx < 0 || colIdx >= COLS) { 
            gotoxy(inputX+30, inputY); printf(COLOR_RED "No such seat" COLOR_RESET); 
            pauseExecution(800); gotoxy(inputX, inputY); printf("                                            "); 
            continue; 
        }
        
        // Check if VIP tried to pick Regular or vice versa
        if (ticketType == TYPE_VIP && rowIdx != 0) { 
            gotoxy(inputX+30, inputY); printf(COLOR_RED "Not VIP" COLOR_RESET); 
            pauseExecution(800); gotoxy(inputX, inputY); printf("                                            "); 
            continue; 
        }
        if (ticketType == TYPE_REG && rowIdx == 0) { 
            gotoxy(inputX+30, inputY); printf(COLOR_RED "Is VIP" COLOR_RESET); 
            pauseExecution(800); gotoxy(inputX, inputY); printf("                                            "); 
            continue; 
        }
        
        // Check if seat is already sold
        if (isSeatBooked(rowIdx, colIdx, showtimeIndex)) { 
            gotoxy(inputX+30, inputY); printf(COLOR_RED "Taken" COLOR_RESET); 
            pauseExecution(800); gotoxy(inputX, inputY); printf("                                            "); 
            continue; 
        }

        // Check for duplicates in current selection
        int duplicate = 0; int k;
        for(k=0; k<count; k++) { if(outputSeats[k].r == rowIdx && outputSeats[k].c == colIdx) { duplicate = 1; break; } }
        if (duplicate) { 
            gotoxy(inputX+30, inputY); printf(COLOR_RED "Duplicate" COLOR_RESET); 
            pauseExecution(800); gotoxy(inputX, inputY); printf("                                            "); 
            continue; 
        }

        // Success: Save seat to array
        outputSeats[count].r = rowIdx; outputSeats[count].c = colIdx; outputSeats[count].rowChar = rowChar;
        outputSeats[count].price = (ticketType == TYPE_VIP) ? PRICE_VIP : PRICE_REG;
        gotoxy(inputX+30, inputY); printf(COLOR_GREEN "[OK]" COLOR_RESET);
        count++;
    }
    pauseExecution(500);
}

// Function: showTransactionSummary
// Purpose: Displays the final receipt showing Tickets + Snacks + Total.
void showTransactionSummary(int qty, SeatSelection* seats, float ticketTotal, float snacksTotal) {
    clearScreen();
    printHeader("RECEIPT");
    printCentered(8, "Booking Confirmed!", COLOR_GREEN);
    int y = 10;
    int i;
    // List all tickets
    for(i=0; i<qty; i++) {
        char line[100];
        sprintf(line, "Seat %c-%02d (%s) ......... PHP %.2f", seats[i].rowChar, seats[i].c+1, (seats[i].rowChar == 'A' ? "VIP" : "REG"), seats[i].price);
        printCentered(y++, line, COLOR_WHITE);
    }
    // List snacks if any
    if (snacksTotal > 0) {
        y++;
        char snackLine[50];
        sprintf(snackLine, "CONCESSIONS/EXTRAS ...... PHP %.2f", snacksTotal);
        printCentered(y++, snackLine, COLOR_CYAN);
    }
    printDivider(y + 1);
    char totalStr[50];
    sprintf(totalStr, "GRAND TOTAL: PHP %.2f", ticketTotal + snacksTotal);
    printCentered(y + 3, totalStr, COLOR_YELLOW);
    gotoxy(38, y + 5); printf("[Press Enter to Finish]");
    getchar();
}

// Function: showAdminLogin
// Purpose: Asks for a password to enter Admin Mode.
// Hardcoded password is "admin".
int showAdminLogin() {
    printHeader("SECURITY CHECK");
    printCentered(9, "ENTER PASSPHRASE", COLOR_RED);
    printDivider(11);
    char password[50];
    gotoxy(40, 13); printf(COLOR_YELLOW "Passphrase: "); scanf("%s", password); clearInputBuffer(); 
    if (strcmp(password, "admin") == 0) { showLoadingAnimation("Access Granted"); return 1; } 
    else { printCentered(15, "ACCESS DENIED. INTRUDER DETECTED.", COLOR_RED); pauseExecution(1500); return 0; }
}

// Function: playMovieSequence
// Purpose: Shows the "Now Screening" animation with audience stats.
void playMovieSequence() {
    clearScreen();
    printHeader("NOW SCREENING");
    int sold = countSoldSeats(); // Gets total sold count
    int capacity = ROWS * COLS * NUM_SHOWTIMES; // Total possible seats
    
    printCentered(9, "WELCOME TO THE WICKED MOVIE", COLOR_CYAN);
    char statStr[50]; sprintf(statStr, "Total Sold: %d tickets", sold);
    printCentered(11, statStr, COLOR_YELLOW);
    
    printDivider(13);
    
    // Simple text animation
    gotoxy(38, 15); printf("The lights are dimming..."); pauseExecution(1500);
    gotoxy(38, 16); printf("The projector hums..."); pauseExecution(1500);
    gotoxy(35, 18); printf(COLOR_RED "THE WICKED GOOD IS NOW PLAYING..." COLOR_RESET); pauseExecution(2000);
    
    printDivider(20);
    gotoxy(35, 22); printf("[Press Enter to leave the cinema]");
    getchar();
}

// Function: showGuestMenu
// Purpose: Displays the main menu for Guests.
int showGuestMenu() {
    printHeader("TICKET KIOSK");
    gotoxy(41, 9);  printf(COLOR_WHITE "1. Buy Tickets");
    gotoxy(41, 10); printf(COLOR_WHITE "2. Movie Info");
    gotoxy(41, 11); printf(COLOR_CYAN  "3. Watch Movie");
    gotoxy(41, 12); printf(COLOR_WHITE "4. Return to Start");
    printDivider(14);
    return getIntInput(41, 16, COLOR_YELLOW "Select an option > " COLOR_RESET, 1, 4);
}

// Function: showAdminMenu
// Purpose: Displays the main menu for Admins.
int showAdminMenu() {
    printHeader("MANAGER CONSOLE");
    gotoxy(38, 9);  printf(COLOR_WHITE "1. View Current Sales");
    gotoxy(38, 10); printf(COLOR_GREEN "2. Cashout (Close Shift)");
    gotoxy(38, 11); printf(COLOR_WHITE "3. Logout");
    printDivider(13);
    return getIntInput(41, 15, COLOR_YELLOW "Command > " COLOR_RESET, 1, 3);
}
