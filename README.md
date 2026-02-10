# Movie-Ticketing-System
"This Ticketing System Project" is a high-fidelity, console-based cinema management application developed in C. It simulates a complete movie theater experience, featuring a self-service kiosk for customers and a secure management console for administrators.

Key Features
👤 Guest Mode (Customer Kiosk)

**Showtime Selection** Choose from 4 distinct screening times (Matinee, Afternoon, Prime, Evening).
 
**Visual Seat Map** A color-coded grid (Green = Available, Red = Sold) specific to the selected showtime.

**Flexible Booking**
    - VIP Experience (Row A): Premium pricing (PHP 700.00).
    - Regular Seating (Rows B-D): Standard pricing (PHP 450.00).

**Smart Selection** Auto-assign seats for speed, or manually select specific seats (e.g., "A1", "C4").

**Concession Stand** Add snacks (Popcorn, Soda, Water) and merchandise to the order.

**Payment Gateway** Cash-based entry system that calculates change accurately.

**Dynamic Ticket Printing** Animated ticket generation showing the specific movie time and seat details.

🛡️ Admin Mode (Manager Console)

**Secure Login:** Password-protected access (Default Passphrase: admin).

**Sales Tracking:** View a real-time log of all transactions in sales_log.txt.

**Shift Closure (Cashout):**
    - Calculates total revenue in the drawer.
    - Archives old logs to history_archive.txt for auditing.
    - Resets the system for the next business day.
    
⚙️ Technical Highlights
**1. The 3D Seat Matrix**
Unlike basic booking systems that use a simple 2D grid, this utilizes a 3D Array logic: [Showtime][Row][Column].
    - This ensures that booking Seat A1 at 10:30 AM does not block Seat A1 at 8:00 PM.
    - Every showtime has its own independent inventory.

**2. "Cinematic" User Interface**
The system moves beyond standard console text:
    - ANSI Color Coding: Uses specific colors for status (Red for Errors/Sold, Green for   Success/Available, Gold for VIP).
    - Coordinate Positioning: Uses a custom gotoxy() function to render menus, tickets, and receipts in the exact center of the screen.
    - Input Masking & Validation: Prevents the interface from breaking if a user inputs letters instead of numbers.

3. Modular Architecture
The codebase is split into 5 distinct files for maintainability:
    - main.c: The central controller.
    - ui.c: Handles all visuals, animations, and menus.
    - tickets.c: Manages the database, logic, and file I/O.
    - payments.c: Handles the money and calculation logic.
    - utilities.c: Low-level system tools (cursors, screen clearing).
      
🚀 How to Run
**Prerequisites**
You need a C Compiler (like GCC) or an IDE (like Dev-C++, Code::Blocks, or VS Code).

**Compilation**
Because the project is modular, you must compile all .c files together.

Using Command Line (GCC):
Bash
gcc src/*.c -o cinema_system
./cinema_system

Using an IDE:
1. Create a new Empty Project.
2. Add all .c files (main.c, ui.c, tickets.c, payments.c, utilities.c) to the project.
3. Add all .h files to the project directory.
4. Click Build & Run.

📂 Project Structure
TheWickedGood/
│
├── sales_log.txt          # Active daily logs (Auto-generated)
├── history_archive.txt    # Past shift records (Auto-generated)
│
└── src/
    ├── main.c             # Main entry point & loop
    ├── ui.c               # Visuals & Menus
    ├── ui.h               # UI Prototypes
    ├── tickets.c          # Booking Logic & File I/O
    ├── tickets.h          # Ticket/Seat Structures
    ├── payments.c         # Payment Processing
    ├── payments.h         # Payment Prototypes
    ├── utilities.c        # Cursor & Input Helpers
    └── utilities.h        # Constants & Config
    
📝 Usage Guide
1. Launch the System: You will see the "The Wicked Good" splash screen.
2. Select Identity: Choose Guest to buy tickets.
3. Buy Tickets:
    Select a Showtime.
    View the map and choose VIP or Regular.
    Enter quantity (Max 24).
    Choose seats (Auto or Manual).
    (Optional) Visit the Concession Stand.
    Pay with cash.
4. Admin Tasks:
    Select Cinema Manager at the start.
    Enter password: admin.
    Use Cashout at the end of the day to save data.

