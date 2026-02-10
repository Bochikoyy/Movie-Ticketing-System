#include <stdio.h>
#include "payments.h"
#include "ui.h"
#include "utilities.h"

// ---------------------------------------------------------
// HELPER: Simple Total Calculation
// ---------------------------------------------------------
// Note: This is a legacy helper. The main logic now calculates totals 
// dynamically in main.c based on seat types (VIP/Regular) + Snacks.
float calculateTotal(int ticketCount) {
    return ticketCount * 12.50; 
}

// ---------------------------------------------------------
// CORE FUNCTION: Process the Transaction
// ---------------------------------------------------------
// This handles the "Cash Register" experience.
// It loops until the user pays enough money.
// Returns: 1 if successful, 0 if cancelled.
int processPayment(float totalAmount) {
    float payment = 0.0; // How much the user has put in so far
    float input = 0.0;   // The specific bill/coin just entered
    char buffer[50];     // Temp storage for typing

    // 1. SETUP UI
    // Wipe the screen to show a clean "Payment Gateway"
    clearScreen();
    printHeader("PAYMENT GATEWAY");

    // Format the total due message
    char msg[50];
    sprintf(msg, "Total Due: PHP %.2f", totalAmount);
    
    // 2. DISPLAY TOTAL (Centered)
    // We clear screen again briefly to ensure no overlap from previous menus
    clearScreen();
    gotoxy(40, 9);
    printf(COLOR_YELLOW "%s" COLOR_RESET, msg);
    printDivider(11);

    // 3. PAYMENT LOOP
    // Keep asking for money until the Payment >= Total
    while (payment < totalAmount) {
        // Show current status
        gotoxy(35, 13);
        printf("Amount Paid: " COLOR_GREEN "PHP %.2f    " COLOR_RESET, payment); // Spaces needed to wipe old numbers
        
        gotoxy(35, 14);
        printf(COLOR_RED "Remaining: PHP %.2f    " COLOR_RESET, totalAmount - payment);
        
        // Input Prompt
        gotoxy(35, 16);
        printf("Enter cash (or -1 to cancel): ");
        
        // Safe Input Handling
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%f", &input) == 1) {
                // Option to cancel transaction
                if (input == -1) return 0; 
                
                // Add positive cash to the pile
                if (input > 0) {
                    payment += input;
                }
            }
        }
    }

    // 4. VERIFICATION ANIMATION
    // Once full amount is reached, show a "Processing" screen
    clearScreen();
    printHeader("PAYMENT VERIFICATION");
    
    showLoadingAnimation("Verifying Bills");
    
    // 5. CALCULATE CHANGE
    float change = payment - totalAmount;
    
    // Show Success Message
    gotoxy(32, 18);
    printf(COLOR_GREEN "Payment Successful! Change: PHP %.2f\n" COLOR_RESET, change);
    
    // Pause so the user can read the success message
    pauseExecution(1500); 
    
    return 1; // Transaction Complete
}
