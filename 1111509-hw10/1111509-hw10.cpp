// This program reads a binary file,
// updates grades previously written to the file,
// adds new grades in the file,
// and deletes data previously stored in the file.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
using namespace std;

struct StudentData
{
   char studentId[ 8 ];
   char lastName[ 12 ];
   char firstName[ 12 ];
   int grade;
};

enum class Choices{ PRINT = 1, UPDATE, NEW, DELETE, END };

// enable user to input menu choice
Choices enterChoice();

// create text file for printing
void createTextFile( fstream &readFromFile );

// update grade in record
void updateRecord( fstream &updateFile );

// add a new grade in record
void newRecord( fstream &insertInFile );

// delete an existing record
void deleteRecord( fstream &deleteFromFile );

// obtain student ID from user
void getID( const char * const prompt, char studentId[] );

int main()
{
   // open file for reading and writing
   fstream inOutGrade( "grades.dat", ios::in | ios::out | ios::binary );

   // exit program if fstream cannot open file
   if( !inOutGrade )
   {
      cerr << "File could not be opened." << endl;
      system( "pause" );
      exit( 1 );
   }

   Choices choice; // store user choice

   // enable user to specify action
   while( ( choice = enterChoice() ) != Choices::END )
   {
      switch( choice )
      {
      case Choices::PRINT:
         createTextFile( inOutGrade ); // create text file from binary file
         break;
      case Choices::UPDATE:
         updateRecord( inOutGrade ); // update grade
         break;
      case Choices::NEW:
         newRecord( inOutGrade ); // add a new grade
         break;
      case Choices::DELETE:
         deleteRecord( inOutGrade ); // delete existing record
         break;
      default:
         cerr << "Incorrect choice" << endl;
         break;
      }





      cout << endl;
   }

   inOutGrade.close();

   system( "pause" );
}

// enable user to input menu choice
Choices enterChoice()
{
   // display available options
   cout << "Enter your choice" << endl
      << "1 - store a text file of all students' information" << endl
      << "    called \"grades.txt\" for printing" << endl
      << "2 - update a student's grade" << endl
      << "3 - add a student's grade" << endl
      << "4 - delete a student's information" << endl
      << "5 - end program\n? ";

   int menuChoice;
   cin >> menuChoice;
   return static_cast< Choices >( menuChoice );
}

// create text file for printing
void createTextFile( fstream &readFromFile )
{   
   ofstream outFile("grade.txt",ios::out);
   if (!outFile) {
       cerr << "File could not be opened" << endl;
       exit(1);
   }
    cout << setw(13) << left << "student ID"<< setw(12) << "Last Name"
         << setw(13) << "First Name"<< setw(5) << right << "Grade" << endl;
    outFile << setw(13) << left << "student ID" << setw(12) << "Last Name"
        << setw(13) << "First Name" << setw(5) << right << "Grade" << endl;

    StudentData data={};
    readFromFile.seekg(0);
    readFromFile.read(reinterpret_cast<char*>(&data), sizeof(StudentData));

   while (!readFromFile.eof()) {
       if (strcmp(data.studentId, "") != 0) {
           cout << setw(13) << left << data.studentId << setw(12) << data.lastName
               << setw(13) << data.firstName << setw(5) << right << data.grade << endl;
           outFile << setw(13) << left << data.studentId << setw(12) << data.lastName
               << setw(13) << data.firstName << setw(5) << right << data.grade << endl;

       }
           readFromFile.read(reinterpret_cast<char*>(&data), sizeof(StudentData));
   }

   readFromFile.clear();
 }


// update grade in record

void updateRecord(fstream& updateFile)
{
    char studentId[8];
    getID("\nEnter student ID to update", studentId);

    updateFile.seekg(0);
    while (!updateFile.eof()) {
        StudentData data = {};
        updateFile.read(reinterpret_cast<char*>(&data), sizeof(data));
        if (strcmp(data.studentId, studentId) == 0) {
            if (data.grade == -1) {
                cout << "\nStudent ID " << studentId << " has no grade." << endl;
                return;
            }

            cout << endl << setw(13) << left << data.studentId << setw(12) << data.lastName
                << setw(13) << data.firstName << setw(5) << right << data.grade << endl;
            
            cout  << "\nEnter grade: ";
            cin >> data.grade;

            updateFile.seekp(-static_cast<int>(sizeof(StudentData)), ios::cur);//將位置移動到要修改的成績位置
            //負號表示目前的位置向左移(因為剛剛已經讀到要改的grade了)
            updateFile.write(reinterpret_cast<char*>(&data), sizeof(StudentData));
            
            cout << endl << setw(13) << left << data.studentId << setw(12) << data.lastName
                << setw(13) << data.firstName << setw(5) << right << data.grade << endl;
            return;
        }
    }
    cout << "\nStudent ID " << studentId << " is empty." << endl;
    updateFile.clear();

}

// add a new grade in record
void newRecord( fstream &insertInFile )
{
    char studentId[8];
    getID("\nEnter student ID to add", studentId);
    insertInFile.seekg(0);
    while (!insertInFile.eof()) {
        StudentData data = {};
        insertInFile.read(reinterpret_cast<char*>(&data), sizeof(data));
        if (strcmp(data.studentId, studentId) == 0) {
            if (data.grade != -1) {
                cout << "\nStudent ID " << studentId << " has grade." << endl;
                return;
            }
            cout << endl << setw(13) << left << data.studentId << setw(12) << data.lastName
                << setw(13) << data.firstName << setw(5) << right << data.grade << endl;
            cout << endl << "Enter grade: ";
            cin >> data.grade;
            insertInFile.seekp(-static_cast<int>(sizeof(StudentData)), ios::cur);//將位置移動到要修改的成績位置
            //負號表示目前的位置向左移(因為剛剛已經讀到要改的grade了)
            insertInFile.write(reinterpret_cast<char*>(&data), sizeof(StudentData));
            cout << endl << setw(13) << left << data.studentId << setw(12) << data.lastName
                << setw(13) << data.firstName << setw(5) << right << data.grade << endl;
            return;
        }
    }
    cout << "\nStudent ID " << studentId << " is empty." << endl;
    insertInFile.clear();
}

// delete an existing record
void deleteRecord(fstream& deleteFromFile)
{
    char studentId[8];
    getID("Enter student ID to delete: ", studentId);

    deleteFromFile.seekg(0);

    StudentData data = {};
    deleteFromFile.read(reinterpret_cast<char*>(&data), sizeof(StudentData));
    while (!deleteFromFile.eof())
    {
        if (strcmp(data.studentId, studentId) == 0){
            deleteFromFile.seekp(-static_cast<int>(sizeof(StudentData)), ios::cur);
            StudentData tempData = {};
            deleteFromFile.write(reinterpret_cast<char*>(&tempData), sizeof(StudentData));

            cout << "\nStudent ID " << studentId << " deleted.\n";
            return;
        }

        deleteFromFile.read(reinterpret_cast<char*>(&data), sizeof(StudentData));
    }

    cout <<"\nStudent ID " << studentId << " is empty." << endl;
    deleteFromFile.clear();
}


// obtain student ID from user
void getID( const char * const prompt, char studentId[] )
{
   do
   {
      cout << prompt << " ( 1111401 - 1111450 ): ";
      cin >> studentId;
   } while( strcmp( studentId, "1111401" ) == -1 || strcmp( studentId, "1111450" ) == 1 );
}