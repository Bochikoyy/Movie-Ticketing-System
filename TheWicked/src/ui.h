#ifndef UI_H
#define UI_H

// ---------------------------------------------------------
// UI CONSTANTS
// ---------------------------------------------------------
#define MOVIE_TITLE "THE WICKED GOOD"
#define TICKET_PRICE 12.50 // Base price reference (legacy)

// ---------------------------------------------------------
// CORE VISUAL PROTOTYPES
// ---------------------------------------------------------
// Clears the screen and draws the purple/yellow box header with a title.
void printHeader(const char* title);

// Displays the "The Wicked Good" ASCII art logo at the start.
void showSplashScreen();

// Shows a message (e.g., "Processing") with a 3-dot animation.
void showLoadingAnimation(const char* message);

// Draws a decorative horizontal line at a specific Y vertical position.
void printDivider(int y);

// Displays the movie plot, rating, and runtime.
void printMovieInfo();

// Shows the visual grid of seats (Red=Sold, Green=Available).
void showSeatMap(); 

// ---------------------------------------------------------
// TRANSACTION DISPLAY
// ---------------------------------------------------------
// included tickets.h here because we need the 'SeatSelection' structure definition.
#include "tickets.h" 

// Displays the final receipt showing individual ticket details and the snack total.
void showTransactionSummary(int qty, SeatSelection* seats, float ticketTotal, float snacksTotal); // UPDATED

// ---------------------------------------------------------
// (Cinema Experience)
// ---------------------------------------------------------
// Displays the list of 4 showtimes and returns the selected index (0-3).
int selectShowtime(char* buffer); // Picks a time

// Opens the Concession Stand menu loop and returns the total cost of snacks bought.
float buyConcessions();            // Buys food/drinks

// ---------------------------------------------------------
// MENUS & AUTHENTICATION
// ---------------------------------------------------------
// The first menu: Asks if you are a Guest, Admin, or want to Exit.
int showRoleSelection();        

// Asks for the password ("admin") to access the Manager Console.
int showAdminLogin();           

// Displays the Admin options (View Sales, Cashout, Logout).
int showAdminMenu();            

// Displays the Guest options (Buy Tickets, Watch Movie, Return).
int showGuestMenu();            

#endif
