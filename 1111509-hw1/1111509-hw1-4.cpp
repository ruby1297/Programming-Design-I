// Exercise 4.18 Solution: ex04_18.cpp
// Display decimal, binary, octal and hexadecimal numbers.
#include <iostream>
using namespace std; 

int main()
{
   int number; // a positive integer less than 32
   int bit1;   // the rightmost bit of number
   int bit2;
   int bit3;
   int bit4;
   int bit5;   // the leftmost bit of number
   cout << "Enter a positive integer less than 32: ";
   cin >> number;
   int tem = number;
   bit1 = tem % 2;
   tem /= 2;
   bit2 = tem % 2;
   tem /= 2;
   bit3 = tem % 2;
   tem /= 2;
   bit4 = tem % 2;
   tem /= 2;
   bit5 = tem % 2;
   tem /= 2;
   cout << "The binary equivalent of "<<number<<" is ";
   cout << bit5 << bit4 << bit3 << bit2 << bit1 << "\n";

   system("pause");
   return 0;
}