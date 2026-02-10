#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   
#include "ui.h"
#include "tickets.h"
#include "payments.h"
#include "utilities.h"

int main() {
    // 1. INITIALIZATION
    // Seed the random number generator for ticket IDs
    srand(time(NULL));
    
    // Initialize the 3D Seat Matrix (Clears seats for all 4 showtimes)
    initSeats(); 
    
    // Show the "Welcome" Intro Screen
    showSplashScreen();

    int systemRunning = 1;

    // 2. MAIN SYSTEM LOOP
    // This keeps the application open until "Exit System" is chosen
    while (systemRunning) {
        
        // Ask: Are you a Guest or an Admin?
        int role = showRoleSelection();

        // --- OPTION 3: EXIT ---
        if (role == 3) { 
            systemRunning = 0;
            printHeader("SHALOOM!"); // Exit Message
            break; 
        }

        // --- OPTION 1: GUEST MODE ---
        if (role == 1) { 
            int guestActive = 1;
            
            // Guest Loop: Keeps user in Guest Menu until they choose "Return"
            while (guestActive) {
                // Show Menu: 1. Buy, 2. Info, 3. Watch, 4. Return
                int choice = showGuestMenu(); 
                
                // === FLOW 1: BUY TICKETS (The Core Logic) ===
                if (choice == 1) { 
                    
                    // STEP 1: SELECT SHOWTIME (V2 Feature)
                    // We need the 'showtimeIdx' (0-3) to know WHICH seat map to load.
                    char selectedTime[20];
                    int showtimeIdx = selectShowtime(selectedTime);

                    // STEP 2: SHOW MAP & SELECT CLASS
                    // Pass 'showtimeIdx' so we see availability for THAT specific time
                    showSeatMap(showtimeIdx);
                    int ticketType = getTicketTypeInput(); // VIP or Regular?
                    
                    // STEP 3: QUANTITY SELECTION
                    printHeader("TICKET COUNTER");
                    
                    // Uses coordinates (35, 9) to fix visual glitches
                    int qty = getIntInput(35, 9, "How many tickets? (1-24): ", 1, 24);
                    
                    // Check if enough seats exist for this Time + Class combo
                    if (!checkAvailability(qty, ticketType, showtimeIdx)) {
                        gotoxy(20, 12);
                        printf(COLOR_RED "Sorry! Not enough seats available in this class." COLOR_RESET);
                        pauseExecution(2000);
                        continue; // Restart loop if full
                    }

                    // STEP 4: SEAT SELECTION METHOD
                    printDivider(14);
                    gotoxy(30, 16); printf("1. Auto-Assign Best Seats");
                    gotoxy(30, 17); printf("2. Select Seats Manually");
                    
                    int method = getIntInput(40, 19, "Choose Method > ", 1, 2);

                    // Create array to hold the selected seats
                    SeatSelection selectedSeats[24]; 

                    if (method == 1) {
                        // Computer picks the best available seats automatically
                        reserveSeats(qty, ticketType, showtimeIdx, selectedSeats);
                    } else {
                        // User types "A1", "B2", etc. manually
                        manualSeatSelect(qty, ticketType, showtimeIdx, selectedSeats);
                    }
                    
                    // STEP 5: CONCESSIONS / EXTRAS
                    // Ask user if they want to buy food/drinks
                    printHeader("EXTRAS");
                    printCentered(12, "Would you like to visit the Concession Stand?", COLOR_CYAN);
                    gotoxy(37, 14); printf(COLOR_WHITE "1. Yes (Buy Food/Drinks)" COLOR_RESET);
                    gotoxy(37, 15); printf(COLOR_WHITE "2. No (Proceed to Checkout)" COLOR_RESET);
                    
                    int wantSnacks = getIntInput(41, 17, "Select > ", 1, 2);
                    
                    float snacksTotal = 0.0;
                    if (wantSnacks == 1) {
                        // Opens the Concession Menu and returns total cost of snacks
                        snacksTotal = buyConcessions();
                    }

                    // STEP 6: CALCULATION
                    float ticketTotal = 0;
                    float total;
                    int i;
                    // Sum up ticket prices (VIP might be 700, Reg 450)
                    for(i=0; i<qty; i++) ticketTotal += selectedSeats[i].price;
                    
                    // Grand Total = Tickets + Popcorn/Drinks
                    float grandTotal = ticketTotal + snacksTotal;
                    
                    // STEP 7: PAYMENT GATEWAY
                    if (processPayment(grandTotal)) {
                        // If payment success:
                        
                        // A. Print Tickets (Animation Loop)
                        int i;
                        for (i = 0; i < qty; i++) {
                            // Pass 'selectedTime' so the ticket prints "10:30 AM" etc.
                            generateTicket(selectedSeats[i], i+1, qty, selectedTime);
                            pauseExecution(3000); // Wait 3s to simulate printing
                        }
                        
                        // B. Finalize Data (Mark seats as Sold in memory)
                        markSeatsSold(qty, selectedSeats, showtimeIdx);
                        
                        // C. Save to File (For Admin Sales Log)
                        saveTransaction(qty, grandTotal);
                        
                        // D. Show Receipt (Lists seats + snack total)
                        showTransactionSummary(qty, selectedSeats, ticketTotal, snacksTotal);
                        
                    } else {
                        printf(COLOR_RED "\n  [Transaction Cancelled]\n" COLOR_RESET);
                        pauseExecution(1500);
                    }
                    
                } 
                // === FLOW 2: MOVIE INFO ===
                else if (choice == 2) {
                    printMovieInfo();
                } 
                // === FLOW 3: WATCH MOVIE (V2 Feature) ===
                else if (choice == 3) {
                    // Ask WHICH showtime they want to simulate watching
                    char watchTime[20];
                    int watchIdx = selectShowtime(watchTime);
                    
                    // Plays animation showing audience count for that specific time
                    playMovieSequence(watchIdx, watchTime);
                } 
                // === EXIT GUEST MODE ===
                else {
                    guestActive = 0;
                }
            }
        }
        // --- OPTION 2: ADMIN MODE ---
        else if (role == 2) { 
             // Security Check (Password: "admin")
             if (showAdminLogin()) { 
                int adminActive = 1;
                while (adminActive) {
                    int choice = showAdminMenu();
                    
                    if (choice == 1) viewSalesLog();      // Read sales_log.txt
                    else if (choice == 2) performCashout(); // Archive logs and clear drawer
                    else if (choice == 3) adminActive = 0;  // Logout
                }
            }
        }
    }
    return 0;
}
