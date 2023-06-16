#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <iomanip>
using std::setw;
using std::setfill;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;

#include <vector>
using std::vector;

struct Date
{
   int year;
   int month;
   int day;
   int hour;
};

struct MemberRecord
{
   char idNumber[ 12 ];   // account number
   char password[ 24 ];   // password
   char name[ 8 ];        // name
};

struct ReservationRecord
{
   char idNumber[ 12 ]; // account number
   int branchCode;      // branch code
   Date date;           // reservation date
   int numCustomers;    // number of customers
};

char branchNames[ 19 ][ 24 ] = { "", "Taipei Dunhua South",   "Taipei Zhongxiao",
                                     "Taipei Songjiang",    "Taipei Nanjing",
                                     "Taipei Linsen",       "Taipei Zhonghua New",
                                     "Banqiao Guanqian",    "Yonghe Lehua",
                                     "Taoyuan Zhonghua",    "Taoyuan Nankan",
                                     "Zhongli Zhongyang",   "Hsinchu Beida",
                                     "Taichung Ziyou",      "Chiayi Ren'ai",
                                     "Tainan Ximen",        "Kaohsiung Zhonghua New",
                                     "Kaohsiung Jianxing",  "Pingtung Kending" };

// read all memberDetails from the file Members.dat
void loadMemberDetails( vector< MemberRecord > &memberDetails );

// read all reservations that are not out of date, from the file Reservations.dat
void loadReservations( vector< ReservationRecord > &reservations );

// compute the current date
Date compCurrentDate();

// return true if and only if date1 <= date2
bool lessEqual( const Date &date1, const Date &date2 );

// input an integer in the range [ begin, end ]
int inputAnInteger( int begin, int end );

// login and display the submenu
void login( const vector< MemberRecord > &memberDetails,
            vector< ReservationRecord > &reservations );

// there exists a member with specified idNumber and password
bool valid( char idNumber[], char password[],
            const vector< MemberRecord > &memberDetails );

// add a new reservation for the member with specified IDnumber
void reservation( char idNumber[], vector< ReservationRecord > &reservations );

// compute 7 dates which is starting from the current date
void compAvailableDates( Date availableDates[] );

// display all fields of reservation
void output( ReservationRecord reservation );

// display all reservations for the member with specified IDnumber,
// then let the member to choose one of them to delete
void queryDelete( char idNumber[], vector< ReservationRecord > &reservations );

// add a new member
void registration( vector< MemberRecord > &memberDetails );

// return true if idNumber is a legal ID number
//bool legalID( char idNumber[] );

// return true if idNumber belongs to memberDetails
bool existingID( char idNumber[],const vector< MemberRecord > &memberDetails );

// write all memberDetails into the file Members.dat
void saveMemberDetails( const vector< MemberRecord > &memberDetails );

// write all reservations that are not out of date, into the file Reservations.dat
void saveReservations( const vector< ReservationRecord > &reservations );

int main()
{
   vector< MemberRecord > memberDetails; // member details for all members
   vector< ReservationRecord > reservations; // all reservations

   loadMemberDetails( memberDetails );
   loadReservations( reservations );

   cout << "Welcome to the Cashbox Party World!\n\n";

   int choice;
   while( true )
   {
      cout << "1 - Login\n";
      cout << "2 - Registration\n";
      cout << "3 - End\n";

      do cout << "\nEnter your choice (1~3): ";
      while( ( choice = inputAnInteger( 1, 3 ) ) == -1 );
      cout << endl;

      switch ( choice )
      {
      case 1:
         login( memberDetails, reservations );
         break;

      case 2:
         registration( memberDetails );
         break;

      case 3:
         saveMemberDetails( memberDetails );
         saveReservations( reservations );
         cout << "Thank you! Goodbye!\n\n";
         system( "pause" );
         return 0;

      default:
         cout << "\nIncorrect choice!\n";
         break;
      }
   }

   system( "pause" );
}

// read all memberDetails from the file Members.dat
void loadMemberDetails( vector< MemberRecord > &memberDetails )
{
    ifstream inFile("Members.dat", ios::in, ios::binary);
    if (!inFile)
    {
        cout << "Members.dat could not be opened." << endl;
        system("pause");
        exit(1);
    }
    inFile.seekg(0);
    MemberRecord temp={};
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&temp), sizeof(MemberRecord));
        memberDetails.push_back(temp);
    }
    
    inFile.close();

}

// read all reservations that are not out of date, from the file Reservations.dat
void loadReservations(vector< ReservationRecord >& reservations)
{
    ifstream inFile("Reservations.dat", ios::in, ios::binary);
    if (!inFile)
    {
        cout << "Reservations.dat could not be opened." << endl;
        system("pause");
        exit(1);
    }
    Date current = compCurrentDate();
    inFile.seekg(0);
    ReservationRecord temp = {};
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&temp), sizeof(ReservationRecord));
        if (!lessEqual(current,temp.date) && !inFile.eof()) {//modify
            reservations.push_back(temp);
        }
    }

    inFile.close();
}

Date compCurrentDate()
{
   Date currentDate={};
   tm structuredTime;
   time_t rawTime = time( 0 );
   localtime_s( &structuredTime, &rawTime );

   currentDate.year = structuredTime.tm_year + 1900;
   currentDate.month = structuredTime.tm_mon + 1;
   currentDate.day = structuredTime.tm_mday;
   currentDate.hour = structuredTime.tm_hour;

   return currentDate;
}


bool lessEqual( const Date &date1, const Date &date2 )
{
    if (date1.year > date2.year)
        return false;
    else if (date1.year < date2.year)
        return true;
    else {
        if (date1.month > date2.month)
            return false;
        else if (date1.month < date2.month)
            return true;
        else {
            if (date1.day > date2.month)
                return false;
        }
    }
    return true;
}

int inputAnInteger( int begin, int end )
{
   char string[ 80 ];
   cin.getline( string, 80, '\n' );

   if( strlen( string ) == 0 )
      exit( 0 );

   for( unsigned int i = 0; i < strlen( string ); i++ )
      if( string[ i ] < '0' || string[ i ] > '9' )
         return -1;

   int number = atoi( string );
   if( number >= begin && number <= end )
      return number;
   else
      return -1;
}

void login( const vector< MemberRecord > &memberDetails,
            vector< ReservationRecord > &reservations )
{
   char idNumber[ 12 ] = "";
   char password[ 24 ] = "";

   do {
      cout << "Please enter your ID number: ";
      cin >> idNumber;
      cout << "Enter your password: ";
      cin >> password;
   } while( !valid( idNumber, password, memberDetails ) );

   cin.ignore();

   int choice;

   while( true )
   {      
      cout << "\n1 - Make Reservation\n";
      cout << "2 - Reservation Enquiry/Canceling\n";
      cout << "3 - End\n";

      do cout << "\nEnter your choice (1~3): ";
      while( ( choice = inputAnInteger( 1, 3 ) ) == -1 );
      cout << endl;

      switch ( choice )
      {
      case 1:
         reservation( idNumber, reservations );
         break;

      case 2:
         queryDelete( idNumber, reservations );
         break;

      case 3:
         return;

      default:
         cout << "\nIncorrect choice!\n";
         break;
      }
   }
}
// there exists a member with specified idNumber and password
bool valid( char idNumber[], char password[], const vector< MemberRecord > &memberDetails )
{
    MemberRecord temp = {};
    size_t size = 0;
    while (memberDetails.size() > size) {
        temp = memberDetails.back();
        if (!strcmp(memberDetails[size].idNumber, idNumber) && !strcmp(memberDetails[size].password, password)) { //modify
            return true;
        }
        size++;
    }

    cout << "\nInvalid account number or password. Please try again.\n\n";
    return false;
}
// add a new reservation for the member with specified IDnumber
void reservation( char idNumber[], vector< ReservationRecord > &reservations )
{
   ReservationRecord newReservation={};
   for (int i = 1; i < 19; i++) {
       cout << setw(2) << i << ". " << branchNames[i] << endl;
   }
    //place to go
   while (newReservation.branchCode < 1 || newReservation.branchCode>18) {
       cout << "\nEnter your choice (0 to end): ";
       cin >> newReservation.branchCode;
       cin.ignore();//add
       if (newReservation.branchCode == 0)
           return;
   }
   //time choose
   Date availableDates[200];
   int ch = -1;
   while (ch > 7 || ch < 0) {//modify

       compAvailableDates(availableDates);
       cout << "\nEnter your choice (0 to end): ";
       cin >> ch;
       if (ch == 0)
           return;
   }

   newReservation.date.year = availableDates[ch].year;
   newReservation.date.month = availableDates[ch].month;
   newReservation.date.day = availableDates[ch].day;

   if (ch == 1) {
       while (newReservation.date.hour < availableDates[1].hour + 1 || newReservation.date.hour > 23) {
           cout << "Enter hour (" << availableDates[1].hour + 1 << "~23): ";
           cin >> newReservation.date.hour;
       }
   }
   else {
       cout << "Enter hour (0~23): ";
       newReservation.date.hour = -1;
       while (newReservation.date.hour < 0 || newReservation.date.hour > 23) {
           cin >> newReservation.date.hour;
       }
   }

   //number of customers
   while (newReservation.numCustomers < 1 || newReservation.numCustomers>30) {
       cout << "Enter the number of customers (1~30, 0 to end): ";
       cin >> newReservation.numCustomers;
       if (newReservation.numCustomers == 0)
           return;
   }

   //idnumber
   for (int i = 0; i < 12; i++)
       newReservation.idNumber[i] = idNumber[i];
   
   cin.ignore();
   cout << endl << setw( 26 ) << "Branch"
                << setw( 14 ) << "Date" << setw( 8 ) << "Hour"
                << setw( 19 ) << "No of Customers" << endl;

   output( newReservation );

   cout << "\nReservation Completed!\n";

   reservations.push_back( newReservation );
}

void compAvailableDates( Date availableDates[] )
{
   int monthInday[14] = { 0.31,28,31,30,31,30,31,31,30,31,30,31 };
   bool leapyear = false;
   availableDates[1] = compCurrentDate();
   cout << "\nThe current hour is " << availableDates[1].year << "/" << availableDates[1].month << "/" 
        << availableDates[1].day << ": " << availableDates[1].hour << endl;

   //leapyear
   if (availableDates[1].year / 4 == 0 && availableDates[1].year / 100 != 0)
       leapyear = true;
   else if (availableDates[1].year / 400 == 0)
       leapyear = true;
   else
       leapyear = false;
   
   if (leapyear)
       monthInday[2] = 29;
   else
       monthInday[2] = 28;

   if (availableDates[1].hour == 23) {
       availableDates[1].hour = 0;
       //day
       availableDates[1].day = availableDates[1].day + 1;
       //month
       if (availableDates[1].day > monthInday[availableDates[1].month]) {
           availableDates[1].month = availableDates[1].month + 1;
           availableDates[1].day -= monthInday[availableDates[1].month];
       }
       else
           availableDates[1].month = availableDates[1].month;
       //year
       if (availableDates[1].month > 12) {
           availableDates[1].year = availableDates[1].year + 1;
           availableDates[1].month -= 12;
          //leapyear
           if (availableDates[1].year / 4 == 0 && availableDates[1].year / 100 != 0)
               leapyear = true;
           else if (availableDates[1].year / 400 == 0)
               leapyear = true;
           else
               leapyear = false;

           if (leapyear)
               monthInday[2] = 29;
           else
               monthInday[2] = 28;
       }
       else
           availableDates[1].year = availableDates[1].year;
   }

   cout << "\nAvailable days:\n";
   for (int i = 1; i < 8; i++) {
       cout << i << ". " << availableDates[i].year << "-" << setw(2) << availableDates[i].month << "-" << setw(2) << availableDates[i].day << endl;
       //day
       availableDates[i + 1].day = availableDates[i].day + 1;
       //month
       if (availableDates[i + 1].day > monthInday[availableDates[i].month]) {
           availableDates[i + 1].month = availableDates[i].month + 1;
           availableDates[i + 1].day -= monthInday[availableDates[i].month];
       }
       else
           availableDates[i + 1].month = availableDates[i].month;
       //year
       if (availableDates[i + 1].month > 12) {
           availableDates[i + 1].year = availableDates[i].year + 1;
           availableDates[i + 1].month -= 12;

           //leapyear
           if (availableDates[1].year / 4 == 0 && availableDates[1].year / 100 != 0)
               leapyear = true;
           else if (availableDates[1].year / 400 == 0)
               leapyear = true;
           else
               leapyear = false;

           if (leapyear)
               monthInday[2] = 29;
           else
               monthInday[2] = 28;
       }
       else
           availableDates[i + 1].year = availableDates[i].year;

   }
   
}

void output( ReservationRecord reservation )
{
   cout << setw( 26 ) << branchNames[ reservation.branchCode ]
        << setw( 8 ) << reservation.date.year << '-'
        << setw( 2 ) << setfill( '0' ) << reservation.date.month << '-'
        << setw( 2 ) << setfill( '0' ) << reservation.date.day
        << setw( 8 ) << setfill( ' ' ) << reservation.date.hour
        << setw( 19 ) << reservation.numCustomers << endl;
}

void queryDelete( char idNumber[], vector< ReservationRecord > &reservations )
{
    Date currentDate = compCurrentDate();
    int sum = 0;
    bool found = false;
    ReservationRecord temp = {};
    vector< ReservationRecord > re;
    //loadReservations(reservations); delete
    // display all reservations for the member with specified IDnumber
    while (!reservations.empty()) {
        temp = reservations.back();
        if (!strcmp(temp.idNumber, idNumber)) {
            if (sum == 0)
                cout << endl << setw(26) << "Branch"
                << setw(14) << "Date" << setw(8) << "Hour"
                << setw(19) << "No of Customers" << endl;
            sum++;
            cout << sum << ".";
            output(temp);
            found = true;
        }
        re.push_back(temp);
        reservations.pop_back();
    }
    if (!found) {
        cout << "No Reservations!\n";
        return;
    }
    //delete reservation
    else {

        int ch = -1;
        while (ch > sum || ch < 0) {
            cout << "\nChoose a reservation to cancel (0: keep all reservations): ";
            cin >> ch;
            cin.ignore();
            if (ch == 0) {
                while (!re.empty()) {
                    temp = re.back();
                    reservations.push_back(re.back());
                    re.pop_back();
                }
                return;
            }
            else {
                int sumdel = 0;
                while (!re.empty()) {
                    temp = re.back();
                    if (!strcmp(temp.idNumber, idNumber)) {
                        sumdel++;
                        if (sumdel != sum - ch + 1) {
                            reservations.push_back(re.back());
                        }
                    }
                    else
                        reservations.push_back(re.back());

                    re.pop_back();

                }
            }
        }
    }
}


void registration( vector< MemberRecord > &memberDetails )
{
   MemberRecord newMember={};

   cout << "Input your ID Number: ";
   cin >> newMember.idNumber;
   cin.ignore();

   if( existingID( newMember.idNumber, memberDetails ) )
   {
      cout << "\nYou are already a member!\n\n";
      return;
   }

   cout << "Input your Name: ";
   cin >> newMember.name;

   cout << "Choose a password: ";
   cin >> newMember.password;

   cin.ignore();

   memberDetails.push_back( newMember );

   cout << "\nRegistration Completed!\n\n";
}

// return true if idNumber belongs to memberDetails
bool existingID( char idNumber[],const vector< MemberRecord > &memberDetails )
{
    MemberRecord temp;
    size_t size = 0;
    while (memberDetails.size() > size) {
        temp = memberDetails.back();
        if (!strcmp(memberDetails[size].idNumber, idNumber)) //modify
            return true;
        size ++;
    }
    return false;
}
// write all memberDetails into the file Members.dat
void saveMemberDetails( const vector< MemberRecord > &memberDetails )
{
    ofstream outFile("Members.dat", ios::out, ios::binary);
    for (int i = 0; i < memberDetails.size();i++) { //modify
        outFile.write(reinterpret_cast<const char*>(&memberDetails[i]), sizeof(MemberRecord));//modify
    }
}
// write all reservations that are not out of date, into the file Reservations.dat
void saveReservations(const vector< ReservationRecord >& reservations)
{
    ifstream inFile("Reservations.dat", ios::in, ios::binary);
    if (!inFile)
    {
        cout << "Reservations.dat could not be opened." << endl;
        system("pause");
        exit(1);
    }
    ofstream outFile("Reservations_temp.dat", ios::out, ios::binary);
    if (!outFile)
    {
        cout << "Reservations_temp.dat could not be opened." << endl;
        system("pause");
        exit(1);
    }
    Date current = compCurrentDate();
    inFile.seekg(0);
    outFile.seekp(0);
    ReservationRecord temp = {};

    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&temp), sizeof(ReservationRecord));
        if (lessEqual(current,temp.date)) {
            outFile.write(reinterpret_cast<char*>(&temp), sizeof(ReservationRecord));
        }
    }
    for (int i = 0; i < reservations.size(); i++) {//modify
        outFile.write(reinterpret_cast<const char*>(&reservations[i]), sizeof(ReservationRecord));//modify
    }

    inFile.close(); //add
    outFile.close(); //add
    remove("Reservations.dat");
    bool name = rename("Reservations_temp.dat", "Reservations.dat");

}