#include <iostream>
using namespace std;

int main()
{
   int year;
   int common = 0, leap = 0;
   cout << "Enter a year ( 1583-3000 ): ";
   cin >> year;
   if (year % 400 == 0) {
	   leap = 1;
	   common = 0;
   }
   else if (year % 4 == 0 && year % 100 != 0) {
	   leap = 1;
	   common = 0;
   }
   else {
	   leap = 0;
	   common = 1;
   }
   cout << "Year " << year << " is a ";
   if (leap == 1)
	   cout << "leap year.\n";
   if (common == 1)
	   cout << "common year.\n";

   system("pause");
   return 0;
}