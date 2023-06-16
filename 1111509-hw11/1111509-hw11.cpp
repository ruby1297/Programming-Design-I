#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

struct Reservation
{
   char flightNo[ 8 ]; // flight number
   char id[ 12 ];      // ID number
   char name[ 8 ];     // name
   char mobile[ 12 ];  // mobile phone number
   char date[ 12 ];    // departure date
   int tickets[ 8 ];
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
   char flightNo[ 8 ];      // flight number
   int departureAirport;    // departure airport code
   int arrivalAirport;      // arrival airport code
   char departureTime[ 8 ]; // departure time
   char arrivalTime[ 8 ];   // arrival time
};

const char airportName[ 12 ][ 12 ] = { "", "Taipei",    "Taichung", "Chiayi",  "Tainan",
                                           "Kaohsiung", "Hualien", "Taitung",
                                           "Penghu",    "Kinmen",   "Beigan",  "Nangan" };

// row 0 and column 0 are not used,
// fullFare[ i ][ j ] is the full fare from airport i to airport j
const int fullFare[ 12 ][ 12 ] = {
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

const int discount[ 8 ] = { 0, 100, 75, 0, 50, 50, 50, 95 };
// discount[ 0 ]: not used
// discount[ 1 ]: Full Fare Ticket (���B��)
// discount[ 2 ]: Child Ticket (�ൣ��):                 25% off of the full fare
// discount[ 3 ]: Infant Ticket (���಼):                Free of charge
// discount[ 4 ]: Senior Citizen Ticket (�q�Ѳ�):        50% off of the full fare
// discount[ 5 ]: Impaired Ticket (�R�߲�):              50% off of the full fare
// discount[ 6 ]: Impaired Companion Ticket (�R�߳��P��): 50% off of the full fare
// discount[ 7 ]: Military Ticket (�x�H��):               5% off of the full fare

const int maxNumReservations = 1000;

int enterChoice();

// perform booking
void booking();

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport( int departureAirport, int &arrivalAirport );

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights );

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights,
                          int departureAirport, int arrivalAirport );

// display all flights from departureAirport to arrivalAirport
void displayFlights( const Flight flights[], int numFlights,
                     int departureAirport, int arrivalAirport, char date[] );

// choose a flight by input its number,
// and assign the number to reservation.flightNo
void chooseFlight( Reservation &reservation,
                   const Flight flights[], int numFlights );

// input the number of tickets for each ticket type,
// and assign them to reservation.tickets
void inputNumTickets( Reservation &reservation );

// check if there is a record, x, of Reservations.dat such that
// x.flightNo == reservation.flightNo, x.id == reservation.id and
// x.date == reservation.date
bool duplicate( const Reservation reservation );

void displayReservation( const Reservation reservation,
                         const Flight flights[], int numFlights );

// append reservation to the end of Reservations.dat
void saveReservation( const Reservation reservation );

// perform booking inquiry
bool bookingInquiry( const char id[] );

// perform refund application
void refundApplication();

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations( Reservation reservations[], int &numReservations );

int main()
{
   cout << "Welcome to UNI Air Domestic Flights Booking System\n";
   char id[ 12 ];
   int choice;

   while( ( choice = enterChoice() ) != 4 )
   {
      switch( choice )
      {
      case 1:
         booking(); // perform booking
         break;
      case 2:
         cout << "\nEnter ID number: ";
         cin >> id;
         bookingInquiry( id ); // perform booking inquiry
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
   system( "pause" );
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
      for( int i = 1; i <= 11; i++ )
         cout << setw( 2 ) << i << ". " << airportName[ i ] << endl;
      cout << "? ";

      cin >> departureAirport;
   } while( departureAirport < 1 || departureAirport > 11 );

   int arrivalAirport;
   // choose arrival airport by input its code, and assign the code to arrivalAirport
   chooseArrivalAirport( departureAirport, arrivalAirport );

   Reservation reservation = { "", "", "", "", "", 0, 0, 0, 0, 0, 0, 0, 0 };
   cout << "\nDeparture date (yyyy/mm/dd): ";
   cin >> reservation.date;
   cout << endl;

   Flight flights[ 20 ];
   int numFlights = 0;

   // load all flights from departureAirport to arrivalAirport,
   // and put them into the array flights
   loadFlightSchedules( flights, numFlights, departureAirport, arrivalAirport );

   // display all flights from departureAirport to arrivalAirport
   displayFlights( flights, numFlights, departureAirport, arrivalAirport, reservation.date );

   // choose a flight by input its number, and assign the number to reservation.flightNo
   chooseFlight( reservation, flights, numFlights );

   // input the number of tickets for each ticket type,
   // and assign them to reservation.tickets
   inputNumTickets( reservation );

   int totalNumTickets = 0;
   for( int i = 1; i < 8; i++ )
      totalNumTickets += reservation.tickets[ i ];
   if( totalNumTickets == 0 )
   {
      cout << "Your booking could not be completed!\n";
      return;
   }

   displayReservation( reservation, flights, numFlights );

   cout << "\nID number: ";
   cin >> reservation.id;

   // check if there is a record, x, of Reservations.dat such that
   // x.flightNo == reservation.flightNo, x.id == reservation.id and
   // x.date == reservation.date
   if( duplicate( reservation ) )
      return;

   cout << "\nName: ";
   cin >> reservation.name;
   cout << "\nMobile: ";
   cin >> reservation.mobile;

   // append reservation to the end of Reservations.dat
   saveReservation( reservation );

   cout << "\nBooking completed!\n";
}

// choose arrival airport by input its code, and assign the code to arrivalAirport
void chooseArrivalAirport( int departureAirport, int &arrivalAirport )
{
    int arr[13]={};
    arrivalAirport = 0;
    while (arr[arrivalAirport] == 0) {
        cout << endl;
        for (int i = 1; i < 12; i++) {
            if (fullFare[departureAirport][i] != 0) {
                cout << setw(2) << i << ". " << airportName[i] << endl;
                arr[i]++;
            }
        }
            cout << "?";
            cin >> arrivalAirport;
    }
}

// load all flights from Flight Schedule.dat and put them into the array flights
void loadFlightSchedules( Flight flights[], int &numFlights )
{
    ifstream inFile("Flight schedule.dat",ios::in);
    if (!inFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    Flight temp={};
    inFile.seekg(0);
    while (inFile.read(reinterpret_cast<char*>(&temp), sizeof(Flight))) {
        flights[numFlights] = temp;
        numFlights++;
    }

    inFile.close();
}

// load all flights from departureAirport to arrivalAirport,
// and put them into the array flights
void loadFlightSchedules(Flight flights[], int& numFlights,
    int departureAirport, int arrivalAirport)
{
    ifstream inFile("Flight schedule.dat", ios::in);
    Flight temp = {};
    numFlights = 1;
    while (!inFile.eof())
    {
        inFile.read(reinterpret_cast<char*>(&temp), sizeof(Flight));
        if (temp.departureAirport == departureAirport&& temp.arrivalAirport == arrivalAirport){   
            flights[numFlights] = temp;
            numFlights++;
        }
    }

    inFile.close();
}

// display all flights from departureAirport to arrivalAirport
void displayFlights( const Flight flights[], int numFlights,
                     int departureAirport, int arrivalAirport, char date[] )
{
   cout << airportName[ departureAirport ] << " to " << airportName[ arrivalAirport ]
        << " / Depart date: " << date << ".\n\n";
   cout << "Flight   Departure   Arrival   Fare\n";

   for( int i = 1; i < numFlights; i++ )
      cout << setw( 6 )  << flights[ i ].flightNo
           << setw( 12 ) << flights[ i ].departureTime
           << setw( 10 ) << flights[ i ].arrivalTime
           << setw( 7 )  << fullFare[ departureAirport ][ arrivalAirport ] << endl;
}

// choose a flight by input its number, and assign the number to reservation.flightNo
void chooseFlight( Reservation &reservation, const Flight flights[], int numFlights )
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
void inputNumTickets( Reservation &reservation )
{
   do {
      cout << "\nNumber of infant tickets (0 ~ 4): ";
      cin >> reservation.tickets[ 3 ];
   } while( reservation.tickets[ 3 ] < 0 || reservation.tickets[ 3 ] > 4 );

   int numTickets = 0;
   int numAdults = 0;
   do
   {
      numTickets = 0;

      cout << "\nNumber of full fare tickets (0 ~ 4): ";
      cin >> reservation.tickets[ 1 ];
      numTickets += reservation.tickets[ 1 ];

      cout << "\nNumber of child tickets (0 ~ 4): ";
      cin >> reservation.tickets[ 2 ];
      numTickets += reservation.tickets[ 2 ];

      cout << "\nNumber of senior citizen tickets (0 ~ 4): ";
      cin >> reservation.tickets[ 4 ];
      numTickets += reservation.tickets[ 4 ];

      cout << "\nNumber of impaired tickets (0 ~ 4): ";
      cin >> reservation.tickets[ 5 ];
      numTickets += reservation.tickets[ 5 ];

      cout << "\nNumber of impaired companion tickets (0 ~ 4): ";
      cin >> reservation.tickets[ 6 ];
      numTickets += reservation.tickets[ 6 ];

      cout << "\nNumber of military tickets (0 ~ 4): ";
      cin >> reservation.tickets[ 7 ];
      numTickets += reservation.tickets[ 7 ];

      numAdults = reservation.tickets[ 1 ] + reservation.tickets[ 4 ] +
                 reservation.tickets[ 5 ] + reservation.tickets[ 6 ] +
                 reservation.tickets[ 7 ];

      if( numTickets > 4 )
         cout << "\nThis booking system accepts a maximum of 4 passengers"
              << "( including children ) per booking.\n"
              << "If there are more than 4 passengers, please make multiple bookings.\n";
      else if( numAdults < reservation.tickets[ 3 ] )
         cout << "\nEach infant must always be accompanied by at least one adult.\n";

   } while( numTickets > 4 || numAdults < reservation.tickets[ 3 ] );

   cout << endl;
}

// check if there is a record, x, of Reservations.dat such that
// x.flightNo == reservation.flightNo, x.id == reservation.id and
// x.date == reservation.date
bool duplicate( const Reservation reservation )
{
    ifstream inFile("Reservations.dat",ios::in);

    while (inFile.good()){
        Reservation x={};
        inFile.read(reinterpret_cast<char*>(&x), sizeof(Reservation));
        if (strcmp(reservation.flightNo, x.flightNo) == 0 && strcmp(reservation.id, x.id) == 0 && strcmp(reservation.date, x.date) == 0)
            return true;
    }

    return false;

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

void saveReservation(const Reservation reservation)
{
    ofstream outFile("Reservations.dat", ios::app);
    if (!outFile)
    {
        cerr << "File could not be opened" << endl;
        exit(1);
    }

    outFile.write(reinterpret_cast<const char*>(&reservation), sizeof(Reservation));
    outFile.close();
}
// perform booking inquiry
bool bookingInquiry( const char id[] )
{
    ifstream inFile("Reservations.dat",ios::in);
    Flight flights[200] = {};
    int sum = 0;
    int num = 1;
    loadFlightSchedules(flights, num);
    inFile.seekg(0);
    while (!inFile.eof()) {
        Reservation data = {};
        inFile.read(reinterpret_cast<char*>(&data), sizeof(Reservation));
        if (strcmp(data.id, id) == 0) {
            sum++;
            cout << endl << sum << ". ";
            displayReservation(data,flights,num);
        }
    }
    if (sum!=0)
        return true;
    else {
        cout << "\nNo reservations!" << endl;
        inFile.close();
        return false;
    }
}

// perform refund application
void refundApplication()
{   
    char id[12] = {};
    cout << "\nEnter ID number: ";
    cin >> id;
    if (bookingInquiry(id)) {
        cout << "\nEnter your choice: ";
        int refund, sum = 0;
        cin >> refund;
        ifstream inFile("Reservations.dat", ios::in);
        ofstream outFile("Reservations_temp.dat", ios::out);
        inFile.seekg(0);
        while (!inFile.eof()) {
            Reservation data = {};
            inFile.read(reinterpret_cast<char*>(&data), sizeof(Reservation));
            if (strcmp(data.id, id) == 0) {
                sum++;
                if (sum != refund) {
                    outFile.write(reinterpret_cast<char*>(&data), sizeof(Reservation));
                }
            }
            else {
                outFile.write(reinterpret_cast<char*>(&data), sizeof(Reservation));
            }
        }
        inFile.close();
        outFile.close();

        remove("Reservations.dat");
        bool name=rename("Reservations_temp.dat", "Reservations.dat");

        cout << "\nThe seleted booking has been deleted.\n";

    }

}

// load all reservations from Reservations.dat and put them to the array reservations
void loadReservations( Reservation reservations[], int &numReservations )
{
    ifstream inFile("Reservations.dat", ios::in);
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>( & reservations[numReservations]), sizeof(Reservation));
        numReservations++;
    }

    inFile.close();

}