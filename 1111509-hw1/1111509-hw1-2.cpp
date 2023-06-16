#include <iostream> // allows program to perform input and output
#include <algorithm>
using namespace std; // program uses names from the std namespace

int main()
{
	int number[3];
	cout <<"Enter three distinct integers: ";
	for (int i = 0; i < 3; i++) 
		cin >> number[i];
	sort(number, number + 3);
	cout << "These numbers in ascending order: ";
	for (int i = 0; i < 3; i++)
		cout << number[i] << " ";
	cout << "\n";

	system("pause");
	return 0;
}