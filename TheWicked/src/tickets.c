#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tickets.h"
#include "ui.h"
#include "utilities.h"

// ---------------------------------------------------------
// DATA STRUCTURE: The Seating Chart
// ---------------------------------------------------------
// This is a 3D Matrix: [TimeSlot][Row][Col]
// It allows us to track seat availability separately for each showtime.
// Example: seatMatrix[0][0][0] is Row A, Seat 1 for the 10:30 AM show.
static int seatMatrix[NUM_SHOWTIMES][ROWS][COLS]; 

// Function: initSeats
// Purpose: Resets the entire cinema to empty (0) when the program starts.
void initSeats() {
    int t, i, j;
    // Loop through all showtimes (t), rows (i), and columns (j)
    for(t=0; t<NUM_SHOWTIMES; t++) {
        for(i=0; i<ROWS; i++) {
            for(j=0; j<COLS; j++) {
                seatMatrix[t][i][j] = 0; // 0 = Available, 1 = Sold
            }
        }
    }
}

// Function: isSeatBooked
// Purpose: Checks if a specific seat is taken for a specific time.
// Returns: 1 (True) if booked, 0 (False) if available.
int isSeatBooked(int r, int c, int showtimeIndex) {
    return seatMatrix[showtimeIndex][r][c];
}

// Function: countSoldSeats
// Purpose: Calculates the TOTAL number of tickets sold across ALL showtimes.
// Used by the Admin "View Sales" feature to show overall activity.
int countSoldSeats() {
    int count = 0;
    int t, i, j;
    for(t=0; t<NUM_SHOWTIMES; t++) {
        for(i=0; i<ROWS; i++) {
            for(j=0; j<COLS; j++) {
                if(seatMatrix[t][i][j] == 1) {
                    count++;
                }
            }
        }
    }
    return count;
}

// Function: checkAvailability
// Purpose: Verifies if there are enough contiguous empty seats in a specific class.
// Used before booking to prevent "sold out" errors during seat selection.
int checkAvailability(int qty, int type, int showtimeIndex) {
    int freeCount = 0;
    int startRow, endRow;

    // Define row ranges for VIP (Row A) vs Regular (Rows B-D)
    if (type == TYPE_VIP) { startRow = 0; endRow = 1; } 
    else { startRow = 1; endRow = ROWS; }

    int i, j;
    for(i = startRow; i < endRow; i++) {
        for(j = 0; j < COLS; j++) {
            // Only count seats for the SELECTED showtime
            if(seatMatrix[showtimeIndex][i][j] == 0) freeCount++;
        }
    }
    // Return True if we have at least 'qty' seats free
    return (freeCount >= qty);
}

// Function: reserveSeats
// Purpose: Automatically assigns the first available seats found.
// Note: This modifies the 'outputSeats' array with the chosen seat details.
void reserveSeats(int qty, int type, int showtimeIndex, SeatSelection* outputSeats) {
    int count = 0;
    int startRow, endRow;

    if (type == TYPE_VIP) { startRow = 0; endRow = 1; } 
    else { startRow = 1; endRow = ROWS; }
    
    int i, j;
    for(i = startRow; i < endRow; i++) {
        for(j = 0; j < COLS; j++) {
            // Find an empty seat
            if (seatMatrix[showtimeIndex][i][j] == 0) {
                // Save seat coordinates and price
                outputSeats[count].r = i;
                outputSeats[count].c = j;
                outputSeats[count].rowChar = 'A' + i;
                
                if (type == TYPE_VIP) outputSeats[count].price = PRICE_VIP;
                else                  outputSeats[count].price = PRICE_REG;
                
                count++;
                if (count >= qty) return; // Stop once we have enough seats
            }
        }
    }
}

// Function: markSeatsSold
// Purpose: The "Commit" function. Permanently changes seat status to 1 (Sold).
// This happens ONLY after payment is successful.
void markSeatsSold(int qty, SeatSelection* seats, int showtimeIndex) {
    int i;
    for(i=0; i<qty; i++) {
        int r = seats[i].r;
        int c = seats[i].c;
        // Mark specific showtime matrix
        seatMatrix[showtimeIndex][r][c] = 1; 
    }
}

// Function: generateTicket
// Purpose: Prints the ASCII ticket animation.
// Updated to use the specific 'timeStr' (e.g. "10:30 AM") instead of current clock.
void generateTicket(SeatSelection seat, int current, int total, char* timeStr) {
    clearScreen(); 
    
    // Choose border colors (Gold for VIP, Magenta for Regular)
    char* borderColor = (seat.rowChar == 'A') ? COLOR_YELLOW : COLOR_MAGENTA;
    char* titleColor  = (seat.rowChar == 'A') ? COLOR_RED : COLOR_YELLOW;

    // Centering Logic: (Screen Width 100 - Ticket Width 46) / 2 = 27
    int x = 27; 
    int y = 8; 

    gotoxy(35, 5);
    printf("Printing Ticket %d of %d...", current, total);

    // DRAWING THE TICKET (Line by Line)
    gotoxy(x, y);   
    printf("%s+--------------------------------------------+  " COLOR_RESET, borderColor);

    gotoxy(x, y+1); 
    printf("%s|               %sTHE WICKED GOOD             %s |  " COLOR_RESET, borderColor, titleColor, borderColor);

    gotoxy(x, y+2); 
    printf("%s|                TICKET #%04d                |  " COLOR_RESET, borderColor, rand() % 9999);
    
    gotoxy(x, y+3); 
    printf("%s|--------------------------------------------|  " COLOR_RESET, borderColor);
    
    // Seat and Price Line (Aligned with padding)
    gotoxy(x, y+4); 
    printf("%s|     Seat: " COLOR_CYAN "%c-%02d" COLOR_RESET "           Price: " COLOR_GREEN "PHP%-6.2f" COLOR_RESET "%s  |  " COLOR_RESET, 
           borderColor, seat.rowChar, seat.c + 1, seat.price, borderColor);
                           
    // Date/Time Line (Centered)
    gotoxy(x, y+5); 
    printf("%s|          %16s                  |  " COLOR_RESET, borderColor, timeStr);
    
    // Tag Line (VIP vs Standard)
    char* tag = (seat.rowChar == 'A') ? "[ VIP ACCESS ]" : "[ STD ADMIT  ]";
    gotoxy(x, y+6); 
    printf("%s|               %s               |  " COLOR_RESET, borderColor, tag);
    
    gotoxy(x, y+7); 
    printf("%s+--------------------------------------------+  " COLOR_RESET, borderColor);
}

// Function: saveTransaction
// Purpose: Writes the sale to 'sales_log.txt' for the Admin.
void saveTransaction(int count, float total) {
    FILE *f = fopen("sales_log.txt", "a");
    if (f == NULL) return;
    
    time_t t = time(NULL);
    char *timeStr = ctime(&t);
    timeStr[strlen(timeStr)-1] = '\0'; // Remove newline
    
    fprintf(f, "[%s] Sold: %d tickets | Total: PHP %.2f\n", timeStr, count, total);
    fclose(f);
}

// Function: viewSalesLog
// Purpose: Admin feature to read and display the sales log file.
void viewSalesLog() {
    printHeader("ADMIN: SALES LOG");
    FILE *f = fopen("sales_log.txt", "r");
    if (f == NULL) {
        gotoxy(35, 10);
        printf(COLOR_YELLOW "No sales history found.\n" COLOR_RESET);
        printDivider(12);
    } else {
        char line[256];
        printf(COLOR_CYAN);
        int y = 9; 
        
        while (fgets(line, sizeof(line), f)) {
            // Print log lines slightly indented for readability
            gotoxy(18, y++);
            size_t len = strlen(line);
            if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
            printf("%s", line);
            if(y > 20) break; // Simple pagination (stop after 20 lines)
        }
        printf(COLOR_RESET);
        fclose(f);
    }
    
    printDivider(22);
    gotoxy(38, 24);
    printf(COLOR_WHITE "[Press Enter to return]" COLOR_RESET);
    getchar();
}

// Function: performCashout
// Purpose: Reads all sales, calculates total revenue, archives the data, and resets the log.
void performCashout() {
    printHeader("SHIFT CLOSURE");

    FILE *f = fopen("sales_log.txt", "r");
    if (f == NULL) {
        gotoxy(30, 9);
        printf(COLOR_RED "Error: No active sales to cashout." COLOR_RESET);
        getchar();
        return;
    }

    float totalRevenue = 0.0;
    char line[256];
    char fileContent[10000] = ""; 

    // Read file and sum up totals
    while (fgets(line, sizeof(line), f)) {
        strcat(fileContent, line); 
        char *ptr = strrchr(line, '$'); 
        if(ptr == NULL) ptr = strstr(line, "PHP");
        
        if (ptr != NULL) {
            float lineTotal = 0.0;
            // Skip currency symbol to get the number
            if(*ptr == '$') ptr++;
            else if(strncmp(ptr, "PHP", 3) == 0) ptr+=3;
            
            sscanf(ptr, "%f", &lineTotal);
            totalRevenue += lineTotal;
        }
    }
    fclose(f);

    if (totalRevenue == 0.0) {
        gotoxy(32, 9);
        printf(COLOR_YELLOW "   	  Drawer is empty." COLOR_RESET);
        getchar();
        return;
    }

    // 1. Centered Header
    gotoxy(39, 9);  
    printf("Total Cash in Drawer:");

    // 2. Centered Amount
    gotoxy(42, 11); 
    printf(COLOR_GREEN "PHP %.2f" COLOR_RESET, totalRevenue); 
    
    // 3. Divider
    printDivider(13);
    
    // 4. Centered Prompt
    gotoxy(30, 15);
    printf(COLOR_YELLOW "Confirm Cashout? (1 = Yes, 0 = Cancel): " COLOR_RESET);
    
    int confirm;
    scanf("%d", &confirm);
    clearInputBuffer();

    // CLEAR SCREEN TO PREVENT VISUAL BUGS
    clearScreen();

    if (confirm == 1) {
        printHeader("PROCESSING TRANSFER");
        showLoadingAnimation("Securing Funds");

        // Move current logs to History Archive
        FILE *archive = fopen("history_archive.txt", "a");
        if (archive != NULL) {
            time_t t = time(NULL);
            char *timeStr = ctime(&t);
            timeStr[strlen(timeStr)-1] = '\0';

            fprintf(archive, "\n=== SHIFT CLOSED [%s] | CASHOUT: PHP%.2f ===\n", timeStr, totalRevenue);
            fprintf(archive, "%s", fileContent); 
            fprintf(archive, "=======================================================\n");
            fclose(archive);
        }

        // Wipe current sales log
        f = fopen("sales_log.txt", "w");
        if (f != NULL) fclose(f);

        // Centered Success Message
        gotoxy(35, 17);
        printf(COLOR_GREEN "Shift Closed. Funds Secured." COLOR_RESET);
        gotoxy(32, 18);
        printf("Log moved to history_archive.txt");
    } else {
        printHeader("SHIFT CLOSURE");
        gotoxy(40, 15);
        printf(COLOR_RED "Cashout Cancelled." COLOR_RESET);
    }

    gotoxy(38, 22);
    printf("[Press Enter to return]");
    getchar();
}

// Placeholder for future feature
void viewArchives() {
    printHeader("ARCHIVES");
    gotoxy(30, 10); 
    printf("Feature not yet implemented."); 
    getchar();
}
