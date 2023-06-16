#include <iostream>
#include <iomanip>
#include <fstream>
// #include <ctime>
#include <time.h>
//#include <cstdlib>
#include <cstring>

using namespace std;

struct Date
{
    int year;
    int month;
    int day;
};

struct Reservation
{
    char flightNo[8]; // flight number
    char id[12];      // ID number
    char name[8];     // name
    char mobile[12];  // mobile phone number
    char date[12];    // departure date
    int tickets[8];
    // tickets[ 0 ]: not used
    // tickets[ 1 ]: number of full fare tickets
    // tickets[ 2 ]: number of child tickets
    // tickets[ 3 ]: number of infant tickets
    // tickets[ 4 ]: number of senior citizen tickets
    // tickets[ 5 ]: number of impaired tickets
    // tickets[ 6 ]: number of impaired companion tickets
    // tickets[ 7 ]: number of military tickets
};

struct Flight
{
    char flightNo[8];      // flight number
    char aircraft[8];      // aircraft model
    int departureAirport;    // departure airport code
    int arrivalAirport;      // arrival airport code
    char departureTime[8]; // departure time
    char arrivalTime[8];   // arrival time
    int dayWeek[7];        // the value of dayWeek[ i ] is 0 or 1
    // dayWeek[ 0 ] == 1 if and only if the flight flies on Sunday
    // dayWeek[ 1 ] == 1 if and only if the flight flies on Monday
    // dayWeek[ 2 ] == 1 if and only if the flight flies on Tuesday
    // dayWeek[ 3 ] == 1 if and only if the flight flies on Wednesday
    // dayWeek[ 4 ] == 1 if and only if the flight flies on Thursday
    // dayWeek[ 5 ] == 1 if and only if the flight flies on Friday
    // dayWeek[ 6 ] == 1 if and only if the flight flies on Saturday
};

struct FlightSeats
{
    char date[12];      // date
    char flightNo[8];   // flight number
    int departureAirport; // departure airport code
    int arrivalAirport;   // arrival airport code
    int availableSeats;   // the number of available seats
};

const char airportName[12][12] = { "", "Taipei",    "Taichung", "Chiayi",  "Tainan",
                                         "Kaohsiung", "Hualien", "Taitung",
                                         "Penghu",    "Kinmen",   "Beigan",  "Nangan" };

// row 0 and column 0 are not used,
// fullFare[ i ][ j ] is the full fare from airport i to airport j
const int fullFare[12][12] = {
   { 0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0,    0,    0,    0,    0,    0, 1781, 2460, 2296, 2618, 2197, 2197 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1769, 2236,    0, 2686 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1721, 2217,    0,    0 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1697, 2231,    0,    0 },
   { 0,    0,    0,    0,    0,    0,    0,    0, 1876, 2393,    0,    0 },
   { 0, 1781,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0, 2460,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0, 2296, 1769, 1721, 1697, 1876,    0,    0,    0, 1768,    0,    0 },
   { 0, 2618, 2236, 2217, 2231, 2393,    0,    0, 1768,    0,    0,    0 },
   { 0, 2197,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 },
   { 0, 2197, 2686,    0,    0,    0,    0,    0,    0,    0,    0,    0 } };

const int discount[8] = { 0, 100, 75, 0, 50, 50, 50, 95 };

const int maxNumReservations = 1000;

int enterChoice();

// perform booking
void booking();

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport(int departureAirport, int& arrivalAirport);

// input the departure date
void inputDepartureDate(char date[], int& departureDayWeek);

// return the number of days between currentDate and departureDate
int difference(Date departureDate, Date currentDate);

bool leapYear(int year);

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules(Flight flights[], int& numFlights);

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules(Flight flights[], int& numFlights,
    int departureAirport, int arrivalAirport, int departureDayWeek);

// load all flightSeats from departureAirport to arrivalAirport on departureDate,
// and put them into the array flightSeats
void loadFlightSeats(FlightSeats flightSeats[], int& numFlightSeats, char departureDate[],
    int departureAirport, int arrivalAirport);

// display all flights from departureAirport to arrivalAirport
void displayFlights(const Flight flights[], int numFlights,
    FlightSeats flightSeats[], int& numFlightSeats,
    int departureAirport, int arrivalAirport, char date[]);

// choose a flight by input its number,
// and assign the number to reservation.flightNo
void chooseFlight(Reservation& reservation, const Flight flights[], int numFlights);

// input the number of tickets for each ticket type,
// and assign them to reservation.tickets
void inputNumTickets(Reservation& reservation, FlightSeats flightSeats[], int& numFlightSeats);

// during booking and refund application,
// the corresponding availableSeats in Flight Seats.dat should be modified
void modifyFlightSeatsFile(Reservation& reservation, int totalNumTickets);

void displayReservation(const Reservation reservation,
    const Flight flights[], int numFlights);

// append reservation to the end of Reservations.dat
void saveReservation(const Reservation reservation);

// perform booking inquiry
bool bookingInquiry(const char id[]);

// perform refund application
void refundApplication();

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations(Reservation reservations[], int& numReservations);

int main()
{
    cout << "Welcome to UNI Air Domestic Flights Booking System\n";
    char id[12];
    int choice;

    while ((choice = enterChoice()) != 4)
    {
        switch (choice)
        {
        case 1:
            booking(); // perform booking
            break;
        case 2:
            cout << "\nEnter ID number: ";
            cin >> id;
            bookingInquiry(id); // perform booking inquiry
            break;
        case 3:
            refundApplication(); // perform refund application
            break;

        default:
            cerr << "\nInput error!\n";
            break;
        }
    }

    cout << endl;
    system("pause");
}

int enterChoice()
{
    cout << "\nInput your choice:\n"
        << "1. Booking\n"
        << "2. Booking enquiry\n"
        << "3. Refund application\n"
        << "4. End\n? ";

    int menuChoice;
    cin >> menuChoice;
    return menuChoice;
}

// perform booking
void booking()
{
    // choose departure airport by input its code,
    // and assign the code to departureAirport
    int departureAirport;
    do {
        cout << "\nDeparture airport:\n";
        for (int i = 1; i <= 11; i++)
            cout << setw(2) << i << ". " << airportName[i] << endl;
        cout << "? ";

        cin >> departureAirport;
    } while (departureAirport < 1 || departureAirport > 11);

    int arrivalAirport;
    // choose arrival airport by input its code, and assign the code to arrivalAirport
    chooseArrivalAirport(departureAirport, arrivalAirport);

    Reservation reservation = { "", "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0 };

    int departureDayWeek; // departureDayWeek == 0 if departure date is Sunday
    // departureDayWeek == 1 if departure date is Monday
    // departureDayWeek == 2 if departure date is Tuesday
    // departureDayWeek == 3 if departure date is Wednesday
    // departureDayWeek == 4 if departure date is Thursday
    // departureDayWeek == 5 if departure date is Friday
    // departureDayWeek == 6 if departure date is Saturday

    inputDepartureDate(reservation.date, departureDayWeek);

    Flight flights[20];
    int numFlights = 0;

    // load all flights from departureAirport to arrivalAirport,
    // and put them into the array flights
    loadFlightSchedules(flights, numFlights, departureAirport, arrivalAirport,
        departureDayWeek);

    FlightSeats flightSeats[20];
    int numFlightSeats = 0;

    loadFlightSeats(flightSeats, numFlightSeats, reservation.date,
        departureAirport, arrivalAirport);

    // display all flights from departureAirport to arrivalAirport
    displayFlights(flights, numFlights, flightSeats, numFlightSeats,
        departureAirport, arrivalAirport, reservation.date);

    // choose a flight by input its number, and assign the number to reservation.flightNo
    chooseFlight(reservation, flights, numFlights);

    // input the number of tickets for each ticket type,
    // and assign them to reservation.tickets
    inputNumTickets(reservation, flightSeats, numFlightSeats);

    int totalNumTickets = 0;
    for (int i = 1; i < 8; i++)
        totalNumTickets += reservation.tickets[i];

    if (totalNumTickets > 0)
        modifyFlightSeatsFile(reservation, totalNumTickets);
    else
    {
        cout << "Your booking could not be completed!\n";
        return;
    }

    displayReservation(reservation, flights, numFlights);

    cout << "\nID number: ";
    cin >> reservation.id;
    cout << "\nName: ";
    cin >> reservation.name;
    cout << "\nMobile: ";
    cin >> reservation.mobile;

    // append reservation to the end of Reservations.dat
    saveReservation(reservation);

    cout << "\nBooking completed!\n";
}

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport(int departureAirport, int& arrivalAirport)
{
    cout << "\nArrival airport:\n";
    for (int i = 1; i <= 11; i++) {
        if (fullFare[departureAirport][i] != 0) {
            cout << setw(2) << i << ". " << airportName[i] << endl;
        }
    }
    cout << "? ";
    cin >> arrivalAirport;
}

void inputDepartureDate(char date[], int& departureDayWeek)
{
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    tm structuredTime;
    time_t rawTime = time(nullptr);
    localtime_s(&structuredTime, &rawTime);

    int year = structuredTime.tm_year + 1900;
    int month = structuredTime.tm_mon + 1;
    int day = structuredTime.tm_mday;

    Date currentDate = { year, month, day };
    month--;
    cout << "\nDeparture Date (Month):\n";
    // print 6 month of 20XX-0X
    for (int i = 0; i < 6; i++) {
        cout << i + 1 << ". " << year << "-" << setw(2) << month + 1 << endl;
        month++;
        year += month / 12;
        month %= 12;
    }
    cout << "? ";
    int op;
    cin >> op;
    // calculate the departure date
    int departureYear = currentDate.year + (currentDate.month + op - 1) / 12;
    int departureMonth = (currentDate.month + op - 2) % 12 + 1;
    int departureDay;
    if (departureYear == currentDate.year && departureMonth == currentDate.month)
        cout << "Day (" << currentDate.day << " ~ ";
    else
        cout << "Day (1 ~ ";
    if (departureMonth == 2 && leapYear(departureYear))
        cout << "29";
    else
        cout << daysInMonth[departureMonth];

    cout << "): ";
    cin >> departureDay;

    Date departureDate = { departureYear, departureMonth, departureDay }; // all 1-based
    // cout << "Current Date: " << currentDate.year << "-" << currentDate.month << "-" << currentDate.day << endl;
    // cout << "Departure Date: " << departureDate.year << "-" << departureDate.month << "-" << departureDate.day << endl;
    sprintf_s(date, 20, "%d/%d/%d", departureDate.year, departureDate.month, departureDate.day);

    tm time_in = { 0, 0, 0, // second, minute, hour
        departureDate.day, departureDate.month - 1, departureDate.year - 1900 }; // 1-based day, 0-based month, year since 1900

    time_t time_temp = mktime(&time_in);
    localtime_s(&time_in, &time_temp);
    // cout << "weekday: " << (int)time_in.tm_wday << endl;
    departureDayWeek = time_in.tm_wday;
}

int difference(Date departureDate, Date currentDate)
{
    int daysInMonth[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int days = 0;

    for (int i = currentDate.year; i < departureDate.year; i++)
        days += leapYear(i) ? 366 : 365;

    for (int i = 1; i < departureDate.month; i++) {
        days += daysInMonth[i];
        if (i == 2 && leapYear(departureDate.year))
            days++;
    }

    days += departureDate.day - 1;

    for (int i = 1; i < currentDate.month; i++) {
        days -= daysInMonth[i];
        if (i == 2 && leapYear(currentDate.year))
            days--;
    }

    days -= currentDate.day - 1;

    return days;
}

bool leapYear(int year)
{
    return (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0));
}

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules(Flight flights[], int& numFlights)
{
    ifstream inFile("Flight schedule.dat", ios::in);
    if (!inFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    numFlights = 1;
    Flight temp = {};
    inFile.seekg(0);
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Flight))) {
        flights[numFlights++] = temp;
    }
    inFile.close();
}

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules(Flight flights[], int& numFlights,
    int departureAirport, int arrivalAirport, int departureDayWeek)
{
    ifstream inFile("Flight Schedule.dat", ios::in);
    if (!inFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    numFlights = 1;
    Flight temp = {};
    inFile.seekg(0);
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Flight)))
    {
        if (temp.departureAirport == departureAirport && temp.arrivalAirport == arrivalAirport
            && temp.dayWeek[departureDayWeek] == 1) {
            flights[numFlights++] = temp;
        }
    }
    inFile.close();
}

void loadFlightSeats(FlightSeats flightSeats[], int& numFlightSeats, char departureDate[],
    int departureAirport, int arrivalAirport)
{
    ifstream inFile("Flight Seats.dat", ios::in);
    if (!inFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    numFlightSeats = 1;
    FlightSeats temp = {};
    inFile.seekg(0);
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(FlightSeats))) {
        if (strcmp(temp.date, departureDate) == 0 && temp.departureAirport == departureAirport
            && temp.arrivalAirport == arrivalAirport) {
            flightSeats[numFlightSeats++] = temp;
        }
    }

    inFile.close();

}

// display all flights from departureAirport to arrivalAirport
void displayFlights(const Flight flights[], int numFlights,
    FlightSeats flightSeats[], int& numFlightSeats,
    int departureAirport, int arrivalAirport, char date[])
{
    cout << airportName[departureAirport] << " to " << airportName[arrivalAirport]
        << " / Depart date: " << date << ".\n\n";
    cout << "Flight   Departure   Arrival   Fare   No. Available Seats\n";

    for (int i = 1; i <= numFlights; i++)
        if (flightSeats[i].availableSeats > 0)
            cout << setw(6) << flights[i].flightNo
            << setw(12) << flights[i].departureTime
            << setw(10) << flights[i].arrivalTime
            << setw(7) << fullFare[departureAirport][arrivalAirport]
            << setw(22) << flightSeats[i].availableSeats << endl;
}

// choose a flight by input its number, and assign the number to reservation.flightNo
void chooseFlight(Reservation& reservation, const Flight flights[], int numFlights)
{
    cout << "\nFlight number: ";
    cin >> reservation.flightNo;
    bool flightFound = false;
    for (int i = 1; i < numFlights; i++) {
        if (strcmp(reservation.flightNo, flights[i].flightNo) == 0) {
            flightFound = true;
            break;
        }
    }
    if (!flightFound)
        chooseFlight(reservation, flights, numFlights);
}

// input the number of tickets for each ticket type, and assign them to reservation.tickets
void inputNumTickets(Reservation& reservation, FlightSeats flightSeats[], int& numFlightSeats)
{
    int i;
    for (i = 1; i <= numFlightSeats; i++)
        if (strcmp(reservation.flightNo, flightSeats[i].flightNo) == 0)
            break;

    int numAvailableSeats = flightSeats[i].availableSeats;
    int maxNumTickets = (numAvailableSeats < 4) ? numAvailableSeats : 4;

    do {
        cout << "\nNumber of infant tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[3];
    } while (reservation.tickets[3] < 0 || reservation.tickets[3] > maxNumTickets);

    int numTickets = 0;
    int numAdults = 0;
    do
    {
        numTickets = 0;

        cout << "\nNumber of full fare tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[1];
        numTickets += reservation.tickets[1];

        cout << "\nNumber of child tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[2];
        numTickets += reservation.tickets[2];

        cout << "\nNumber of senior citizen tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[4];
        numTickets += reservation.tickets[4];

        cout << "\nNumber of impaired tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[5];
        numTickets += reservation.tickets[5];

        cout << "\nNumber of impaired companion tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[6];
        numTickets += reservation.tickets[6];

        cout << "\nNumber of military tickets (0 ~ " << maxNumTickets << "): ";
        cin >> reservation.tickets[7];
        numTickets += reservation.tickets[7];

        numAdults = reservation.tickets[1] + reservation.tickets[4] +
            reservation.tickets[5] + reservation.tickets[6] +
            reservation.tickets[7];

        if (numTickets > maxNumTickets)
            cout << "\nYou can books a maximum of " << maxNumTickets
            << " passengers ( including children ).\n";
        else if (numAdults < reservation.tickets[3])
            cout << "\nEach infant must always be accompanied by at least one adult.\n";
    } while (numTickets > maxNumTickets || numAdults < reservation.tickets[3]);

    cout << endl;
}

void modifyFlightSeatsFile(Reservation& reservation, int totalNumTickets)
{
    fstream iofile("Flight Seats.dat", ios::in | ios::out | ios::binary);
    if (!iofile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    FlightSeats temp = {};
    iofile.seekg(0);
    while (iofile.read(reinterpret_cast<char*>(&temp), sizeof(FlightSeats))) {
        if (!strcmp(reservation.flightNo, temp.flightNo) && !strcmp(reservation.date, temp.date)) {
            temp.availableSeats -= totalNumTickets;
            iofile.seekp(-static_cast<int>(sizeof(FlightSeats)), ios::cur);
            iofile.write(reinterpret_cast<char*>(&temp), sizeof(FlightSeats));
            break;
        }
    }

    iofile.close();

}

void displayReservation(const Reservation reservation,
    const Flight flights[], int numFlights)
{
    int k;
    for (k = 1; k <= numFlights; k++)
        if (strcmp(reservation.flightNo, flights[k].flightNo) == 0)
            break;

    cout << "Ticket information:\n\n";
    cout << "Date: " << reservation.date << endl;
    cout << "Flight: B7-" << reservation.flightNo << endl << endl;

    int departureAirport = flights[k].departureAirport;
    int arrivalAirport = flights[k].arrivalAirport;

    cout << setw(9) << right << airportName[departureAirport] << " -> "
        << setw(9) << left << airportName[arrivalAirport] << endl;

    cout << setw(9) << right << flights[k].departureTime << "    ";
    cout << setw(9) << left << flights[k].arrivalTime << endl << endl;

    char ticketType[8][24] = { "", "Full Fare", "Child Fare", "Infant Fare",
                                       "Senior Citizen Fare",     "Impaired Fare",
                                       "Impaired Companion Fare", "Military Fare" };

    int total = 0;
    int fare;
    for (int i = 1; i <= 7; i++)
        if (reservation.tickets[i] > 0)
        {
            fare = fullFare[departureAirport][arrivalAirport] * discount[i] / 100;
            total += (fare * reservation.tickets[i]);
            cout << right << setw(23) << ticketType[i] << "  TWD "
                << setw(4) << fare << " x " << reservation.tickets[i] << endl;
        }

    cout << "\nTotal: " << total << endl;
}

// append reservation to the end of Reservations.dat
void saveReservation(const Reservation reservation)
{
    ofstream outFile("Reservations.dat", ios::out | ios::app | ios::binary);
    if (!outFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    outFile.write(reinterpret_cast<const char*>(&reservation), sizeof(Reservation));
    outFile.close();
}

// perform booking inquiry
bool bookingInquiry(const char id[])
{
    ifstream inFile("Reservations.dat", ios::in);
    Flight flights[200] = {};
    int sum = 0;
    int num = 1;
    loadFlightSchedules(flights, num);

    inFile.seekg(0);
    Reservation data = {};
    while (inFile.read(reinterpret_cast<char*>(&data), sizeof(Reservation))) {
        if (strcmp(data.id, id) == 0) {
            sum++;
            cout << endl << sum << ". ";
            displayReservation(data, flights, num);
        }
    }
    if (sum != 0)
        return true;
    else {
        cout << "\nNo reservations!" << endl;
        return false;
    }
}

// perform refund application
void refundApplication()
{
    char id[12];

    cout << "\nEnter ID number: ";
    cin >> id;

    if (!bookingInquiry(id))
        return;

    int choice;
    cout << "\nEnter your choice: ";
    cin >> choice;

    Reservation reservations[maxNumReservations];
    int numReservations;
    loadReservations(reservations, numReservations);

    int sum = 0, i = 0, found = 0;
    
    while (i <= numReservations) {
        if (!strcmp(reservations[i].id, id)) {
			sum++;
			if (sum == choice) {
                found = i;
                int totalNumTickets = -reservations[i].tickets[3];
                for (int h = 1; h < 8; h++)
                    totalNumTickets += reservations[i].tickets[h];

                fstream ioFile("Flights Seats.dat", ios::in | ios::out | ios::binary);
                FlightSeats temp = {};
                while (ioFile.read(reinterpret_cast<char*>(&temp), sizeof(FlightSeats))) {
                    if (!strcmp(reservations[i].date, temp.date) && !strcmp(reservations[i].flightNo, temp.flightNo)) {
                        temp.availableSeats += totalNumTickets;
                        ioFile.seekp(-static_cast<int>(sizeof(FlightSeats)), ios::cur);
                        ioFile.write(reinterpret_cast<char*>(&temp), sizeof(FlightSeats));
                        ioFile.close();
                        break;
                    }
                }
			}
        }
        i++;
    }
    
    remove("Reservations.dat");
    ofstream outFile("Reservations.dat", ios::out);
    outFile.seekp(0);
    for (int j = 1; j <= numReservations; j++) {
        if (found != j) {
            j++;
            outFile.write(reinterpret_cast<char*>(&reservations[j]), sizeof(Reservation));
        }
    }

    outFile.close();
    
    cout << "\nThe seleted booking has been deleted.\n";
}

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations(Reservation reservations[], int& numReservations)
{
    ifstream inFile("Reservations.dat", ios::in);
    if (!inFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    numReservations = 1;
    Reservation temp = {};
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&temp), sizeof(Reservation));
        reservations[numReservations++] = temp;
    }

    inFile.close();


    return;
}