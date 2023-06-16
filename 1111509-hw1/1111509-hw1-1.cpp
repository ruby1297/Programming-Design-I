#include <iostream> // allows program to perform input and outpu
#include <algorithm>
using namespace std; // program uses names from the std namespace

int main()
{
  
	int number[3];
	cout << "Enter three distinct integers: ";
	for(int i=0;i<3;i++)
	cin >> number[i];
	sort(number, number + 3);
	cout << "Median is ";
	cout << number[1] << "\n";

	system("pause");
   return 0;
}
