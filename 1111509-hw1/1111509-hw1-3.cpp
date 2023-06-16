#include <iostream>
using namespace std;

int main()
{
   int number;
   cout << "Enter a 5-digit positive integer: ";
   cin >> number;
   int tem = 0,ans=0;
   while (number != 0){
	   tem = number % 10;
	   ans=ans*10 +tem;
	   number=number / 10;
   }
   cout << "The digits in the reverse order : ";
   cout << ans<<"\n";
   system("pause");
   return 0;
}