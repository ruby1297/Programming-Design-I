#include<iostream>
using namespace std;
int main() {
	int num, f;
	cin >> num;
	for (int i = 0; i < num; i++) {
		cin >> f;
		int sum = 0;
		for (int j = 0; j < f; j++) {
			int farmyard, animal, value;
			cin >> farmyard >> animal >> value;
			sum += farmyard * value;
		}
		cout << sum << "\n";
	}
	system("pause");
	return 0;
}
