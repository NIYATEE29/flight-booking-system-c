#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Flight
{
    int flight_id;
    char from[100];
    char to[100];
    char time[100];
    float price;
    int seats;
};

struct Passenger
{
    char name[30];
    int flight_id;
    int booking_id;
};

int print_menu();
void add_flight();
int flightExists(int flightID);
void display_flights();
void displayFlightsByStations();
void bookTicket();
// void displayBookings(); not neccesary

void displayPassengersByFlight();
void cancelTicket();
void searchBooking();
int generateBookingID();
 
//void strToLower(char *str);
void updateSeats(int flightID, int increment);
int bookingExists(int bookingID);


int main()
{
    int choice = 1;
    int n;
    while (choice == 1)
    {
        n = print_menu();
        switch (n)
        {
        case 1:
            add_flight();
            break;
        case 2:
            display_flights();
            break;
        case 3:
            displayFlightsByStations();
            break;
        case 4:
            bookTicket();
            break;
        case 5:
            displayPassengersByFlight();
            break;
        case 6:
            searchBooking();
            break;
        case 7:
            cancelTicket();
            break;
        case 8:
            printf("Thank you for using the system!\n");
            choice = 0;
            break;
        default:
            printf("Invalid Choice!!");
        }
    }
}

// prints menu
int print_menu()
{
    int x;
    printf("\n=== Flight Ticket Booking System ===\n");
    printf("1. Add Flight (Admin)\n");
    printf("2. Display All Available Flights\n");
    printf("3. Display Flights by From/To Stations\n");
    printf("4. Book a Ticket\n");
    printf("5. Display Passengers by Flight ID\n");
    printf("6. Search Booking by Booking ID\n");
    printf("7. Cancel Ticket by Booking ID\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &x);
    getchar();
    return x;
}

void add_flight()
{
    struct Flight flight;
    FILE *file = fopen("flights.txt", "a");
    if (file == NULL)
    {
        printf("Error opening flights file!\n");
        return;
    }

    printf("Enter new Flight ID :");
    scanf("%d", &flight.flight_id);
    getchar(); // to remove buffer if user enter any char

    if (flightExists(flight.flight_id))
    {
        printf("Flight ID already exists!\n");
        fclose(file);
        return;
    }

    printf("Enter From destination: ");
    fgets(flight.from, 100, stdin);
    flight.from[strcspn(flight.from, "\n")] = '\0';
    // strcspn using to to find new line,(replace with \0 , trimming string to avoide un-neccesary char)
    printf("Enter To destination: ");
    fgets(flight.to, 100, stdin);
    flight.to[strcspn(flight.to, "\n")] = '\0';

    printf("Enter Time (e.g., 14:30): ");
    fgets(flight.time, 100, stdin);
    flight.time[strcspn(flight.time, "\n")] = '\0';

    printf("Enter Price (INR): ");
    scanf("%f", &flight.price);

    printf("Enter Number of Seats Available: ");
    scanf("%d", &flight.seats);
    getchar();

    fprintf(file, "%d,%s,%s,%s,%.2f,%d\n", flight.flight_id, flight.from, flight.to, flight.time, flight.price, flight.seats);
    // adding to the flight.txt
    fclose(file);
    printf("Flight added successfully!\n");
}

// to check if a flight exists using flight id
int flightExists(int flightID)
{
    struct Flight flight;
    char line[256];
    FILE *file = fopen("flights.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d,", &flight.flight_id);
        if (flight.flight_id == flightID)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Display all flights from flights.txt
void display_flights()
{
    struct Flight flight;
    char line[256];
    FILE *file = fopen("flights.txt", "r");
    if (file == NULL)
    {
        printf("No flights available or error opening file!\n");
        return;
    }

    printf("\n=== All Available Flights ===\n");
    printf("ID\tFrom\tTo\tTime\tPrice\tSeats\n");
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%f,%d", &flight.flight_id, flight.from, flight.to, flight.time, &flight.price, &flight.seats);
        printf("%d\t%s\t%s\t%s\t%.2f\t%d\n", flight.flight_id, flight.from, flight.to, flight.time, flight.price, flight.seats);
    }
    fclose(file);
}

// Display flights for given From and To stations
void displayFlightsByStations()
{   
    char inputFrom[100], inputTo[100];
    char fileFrom[100], fileTo[100];
    struct Flight flight;
    char line[256];
    int found = 0;
    printf("Enter From destination: ");
    fgets(inputFrom,100, stdin);
    inputFrom[strcspn(inputFrom, "\n")] = 0;

    printf("Enter To destination: ");
    fgets(inputTo, 100, stdin);
    inputTo[strcspn(inputTo, "\n")] = 0;

    FILE *file = fopen("flights.txt", "r");
    if (file == NULL)
    {
        printf("No flights available or error opening file!\n");
        return;
    }
    printf("\n=== Flights from %s to %s ===\n", inputFrom, inputTo);
    printf("ID\tFrom\tTo\tTime\tPrice\tSeats\n");

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%f,%d", &flight.flight_id, flight.from, flight.to, flight.time, &flight.price, &flight.seats);

        strcpy(fileFrom, flight.from);
        strcpy(fileTo, flight.to);

        if (strcmpi(fileFrom, inputFrom) == 0 && strcmpi(fileTo, inputTo) == 0)
        {
            printf("%d\t%s\t%s\t%s\t%.2f\t%d\n", flight.flight_id, flight.from, flight.to, flight.time, flight.price, flight.seats);
            found = 1;
        }
    }
    fclose(file);

    if (!found)
    {
        printf("No flights found for %s to %s.\n", inputFrom, inputTo);
    }
}

void bookTicket()
{
    struct Passenger passenger;
    int flightID, choice;
    char inputFrom[100], inputTo[100];
    FILE *file = fopen("passengers.txt", "a");
    if (file == NULL)
    {
        printf("Error opening passengers file!\n");
        return;
    }

    printf("Enter your name: ");
    fgets(passenger.name, 100, stdin);
    passenger.name[strcspn(passenger.name, "\n")] = 0;

    printf("Book by:\n1. Flight ID\n2. From/To Stations\nEnter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1)
    {
        printf("Enter Flight ID to book: ");
        scanf("%d", &flightID);
        getchar();
    }
    else if (choice == 2)
    {
        displayFlightsByStations();
        printf("Enter Flight ID from the list to book: ");
        scanf("%d", &flightID);
        getchar();
    }
    else
    {
        printf("Invalid choice!\n");
        fclose(file);
        return;
    }

    if (!flightExists(flightID))
    {
        printf("Invalid Flight ID!\n");
        fclose(file);
        return;
    }

    // Check seat availability
    struct Flight flight;
    char line[256];
    FILE *flightFile = fopen("flights.txt", "r");
    int seats = 0;
    while (fgets(line, sizeof(line), flightFile))
    {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%f,%d", &flight.flight_id, flight.from, flight.to, flight.time, &flight.price, &flight.seats);
        if (flight.flight_id == flightID)
        {
            seats = flight.seats;
            break;
        }
    }
    fclose(flightFile);

    if (seats <= 0)
    {
        printf("No seats available for Flight ID %d!\n", flightID);
        fclose(file);
        return;
    }

    passenger.flight_id = flightID;
    passenger.booking_id = generateBookingID();

    fprintf(file, "%s,%d,%d\n", passenger.name, passenger.flight_id, passenger.booking_id);
    fclose(file);

    // Decrease seat count
    updateSeats(flightID, -1);

    printf("Ticket booked successfully! Booking ID: %d\n", passenger.booking_id);
}

void displayPassengersByFlight()
{
    int flightID;
    struct Passenger passenger;
    char line[256];
    int found = 0;

    printf("Enter Flight ID: ");
    scanf("%d", &flightID);
    getchar();

    if (!flightExists(flightID))
    {
        printf("Invalid Flight ID!\n");
        return;
    }

    FILE *file = fopen("passengers.txt", "r");
    if (file == NULL)
    {
        printf("No passengers or error opening file!\n");
        return;
    }

    printf("\n=== Passengers for Flight ID %d ===\n", flightID);
    printf("Name\tBooking ID\n");
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%d,%d", passenger.name, &passenger.flight_id, &passenger.booking_id);
        if (passenger.flight_id == flightID)
        {
            printf("%s\t%d\n", passenger.name, passenger.booking_id);
            found = 1;
        }
    }
    fclose(file);

    if (!found)
    {
        printf("No passengers found for Flight ID %d.\n", flightID);
    }
}

void cancelTicket()
{
    int bookingID;
    struct Passenger passenger;
    char line[256];
    int found = 0;
    int flightID = 0;

    printf("Enter Booking ID to cancel: ");
    scanf("%d", &bookingID);
    getchar();

    if (!bookingExists(bookingID))
    {
        printf("Invalid Booking ID!\n");
        return;
    }

    FILE *file = fopen("passengers.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL)
    {
        printf("Error processing cancellation!\n");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%[^,],%d,%d", passenger.name, &passenger.flight_id, &passenger.booking_id);
        if (passenger.booking_id == bookingID)
        {
            found = 1;
            flightID = passenger.flight_id;
            continue; // Skip this passenger
        }
        fprintf(temp, "%s,%d,%d\n", passenger.name, passenger.flight_id, passenger.booking_id);
    }
    fclose(file);
    fclose(temp);

    if (found)
    {
        remove("passanger.txt");
        rename("temp.txt", "passengers.txt");
        updateSeats(flightID, 1); // Increment seat count
        printf("Ticket with Booking ID %d canceled successfully!\n", bookingID);
    }
    else
    {
        remove("temp.txt");
        printf("Booking ID %d not found!\n", bookingID);
    }
}

// Search for a booking by Booking ID
void searchBooking()
{
    int bookingID;
    struct Passenger passenger;
    struct Flight flight;
    char line[256];
    int found = 0;

    printf("Enter Booking ID to search: ");
    scanf("%d", &bookingID);
    getchar();

    FILE *passengerFile = fopen("passengers.txt", "r");
    if (passengerFile == NULL)
    {
        printf("No bookings available or error opening file!\n");
        return;
    }

    // Find passenger
    while (fgets(line, sizeof(line), passengerFile))
    {
        sscanf(line, "%[^,],%d,%d", passenger.name, &passenger.flight_id, &passenger.booking_id);
        if (passenger.booking_id == bookingID)
        {
            found = 1;
            break;
        }
    }
    fclose(passengerFile);

    if (!found)
    {
        printf("Booking ID %d not found!\n", bookingID);
        return;
    }

    // Find flight details
    FILE *flightFile = fopen("flights.txt", "r");
    if (flightFile == NULL)
    {
        printf("Error opening flights file!\n");
        return;
    }

    found = 0;
    while (fgets(line, sizeof(line), flightFile))
    {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%f,%d", &flight.flight_id, flight.from, flight.to, flight.time, &flight.price, &flight.seats);
        if (flight.flight_id == passenger.flight_id)
        {
            found = 1;
            break;
        }
    }
    fclose(flightFile);

    printf("\n=== Booking Details ===\n");
    printf("Booking ID: %d\n", passenger.booking_id);
    printf("Passenger Name: %s\n", passenger.name);
    printf("Flight ID: %d\n", passenger.flight_id);
    if (found)
    {
        printf("Flight Route: %s to %s\n", flight.from, flight.to);
        printf("Time: %s\n", flight.time);
        printf("Price: ₹%.2f\n", flight.price);
        printf("Seats Available: %d\n", flight.seats);
    }
    else
    {
        printf("Flight details not found!\n");
    }
}


// Check if a Booking ID exists in passengers.txt
int bookingExists(int bookingID)
{
    struct Passenger passenger;
    char line[256];
    FILE *file = fopen("passengers.txt", "r");
    if (file == NULL)
    {
        return 0;
    }

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%*[^,],%*d,%d", &passenger.booking_id);
        if (passenger.booking_id== bookingID)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Generate a unique Booking ID
int generateBookingID()
{
    static int lastID = 0;
    char line[256];
    FILE *file = fopen("passengers.txt", "r");
    if (file == NULL)
    {
        return ++lastID;
    }

    while (fgets(line, sizeof(line), file))
    {
        int currentID;
        sscanf(line, "%*[^,],%*d,%d", &currentID);
        if (currentID > lastID)
        {
            lastID = currentID;
        }
    }
    fclose(file);
    return ++lastID;
}

// Update seatsAvailable in flights.txt
void updateSeats(int flightID, int increment)
{
    struct Flight flight;
    char line[256];
    FILE *file = fopen("flights.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (file == NULL || temp == NULL)
    {
        printf("Error updating seats!\n");
        if (file)
            fclose(file);
        if (temp)
            fclose(temp);
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%d,%[^,],%[^,],%[^,],%f,%d", &flight.flight_id, flight.from, flight.to, flight.time, &flight.price, &flight.seats);
        if (flight.flight_id == flightID)
        {
            flight.seats += increment;
        }
        fprintf(temp, "%d,%s,%s,%s,%.2f,%d\n", flight.flight_id, flight.from, flight.to, flight.time, flight.price, flight.seats);
    }
    fclose(file);
    fclose(temp);
    remove("flights.txt");
    rename("temp.txt", "flights.txt");
}
