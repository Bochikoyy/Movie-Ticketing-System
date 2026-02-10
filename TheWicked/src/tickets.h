#ifndef TICKETS_H
#define TICKETS_H

// ---------------------------------------------------------
// PRICING & CONFIGURATION
// ---------------------------------------------------------
// Constants for ticket prices (PHP)
#define PRICE_VIP 700.00
#define PRICE_REG 450.00

// Identifiers for ticket types (Used in logic flow)
#define TYPE_VIP 1
#define TYPE_REG 2

// Cinema Dimensions (4 Rows x 6 Columns = 24 Seats per show)
#define ROWS 4
#define COLS 6

// NEW: We have 4 distinct showtimes (10:30am, 1:15pm, 4:45pm, 8:00pm)
// This creates the 3rd dimension for our seat matrix.
#define NUM_SHOWTIMES 4

// ---------------------------------------------------------
// DATA STRUCTURES
// ---------------------------------------------------------
// Structure to hold data for a single selected seat.
// Used to pass seat details between calculation and printing functions.
typedef struct {
    int r;       // Matrix Row Index (0-3)
    int c;       // Matrix Column Index (0-5)
    float price; // Price of this specific seat
    char rowChar;// Display Character ('A', 'B', 'C', 'D')
} SeatSelection;

// ---------------------------------------------------------
// FUNCTION PROTOTYPES
// ---------------------------------------------------------

// Initializes the 3D Seat Matrix (Sets all seats, for all times, to Empty/0)
void initSeats(); 

// Checks if there are enough contiguous seats available for a specific Type and Time.
// Returns: 1 (True) if available, 0 (False) if full.
int checkAvailability(int qty, int type, int showtimeIndex); 

// Finds the best available seats and stores them in the 'outputSeats' array.
// Does NOT mark them as sold yet (that happens after payment).
void reserveSeats(int qty, int type, int showtimeIndex, SeatSelection* outputSeats); 

// The "Commit" function. Updates the global 3D Matrix to mark seats as Sold (1).
// Called only after payment is verified.
void markSeatsSold(int qty, SeatSelection* seats, int showtimeIndex); 

// Draws the ASCII art ticket on the screen.
// 'timeStr' is passed here to print the specific showtime on the ticket.
void generateTicket(SeatSelection seat, int current, int total, char* timeStr); 

// Appends the transaction details (Date, Count, Total) to 'sales_log.txt'.
void saveTransaction(int count, float total);

// Reads 'sales_log.txt' and displays history to the Admin.
void viewSalesLog();

// Calculates total revenue, archives current logs, and clears the active log file.
void performCashout(); 

// Placeholder for viewing old history files (Future feature).
void viewArchives(); 

// Helper: Returns 1 if a specific seat at a specific time is taken.
// Used by the UI to draw Red (Sold) or Green (Available) seats.
int isSeatBooked(int r, int c, int showtimeIndex);

// Helper: Counts total number of sold seats across all showtimes.
// Used for the "Audience Count" in the movie animation.
int countSoldSeats(); 

#endif
