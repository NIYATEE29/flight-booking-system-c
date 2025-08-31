# flight-booking-system-c
A command-line flight ticket booking system written in C. Manages flights, passengers, and bookings using file handling.
A comprehensive, console-based flight ticket booking and management system written entirely in C. This application demonstrates core programming concepts including data structures, file I/O for data persistence, and modular program design.

Features
1. Admin Panel: Add new flight details, including flight ID, origin, destination, time, price, and seat capacity.
2. Display Flights:
    a. View a complete list of all available flights.
    b. Filter and display flights based on specific departure and arrival stations.
4. Ticket Booking: Book a ticket for an available flight, which updates the seat count and generates a unique booking ID.
5. Booking Management:
    a. Cancel a booked ticket using the unique booking ID, which restores the seat count.
    b. Search for a specific booking to view passenger and flight details.
6. Passenger Information: View a list of all passengers for a specific flight.
7. Data Persistence: All flight and passenger booking data is saved to flights.txt and passengers.txt, ensuring data is not lost when the program closes.


How to Compile and Run
This program can be compiled with any standard C compiler, such as GCC.

1. Clone the repository.
2. Compile the source code: Open a terminal in the project directory and run the following command to compile the C file into an executable named flight_system.
3. Run the application
4. The program will start, and you can interact with it through the on-screen menu.

File Structure

The program uses two text files to store its data:

->flights.txt: Stores all the flight records in a comma-separated format.

    Format: flight_id,from,to,time,price,seats

->passengers.txt: Stores all the passenger booking records.

    Format: name,flight_id,booking_id

These files will be created automatically in the same directory where the executable is run if they do not already exist.
