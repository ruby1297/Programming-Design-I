#include<iostream>
using namespace std;
int main() {
	int K,divisionx,divisiony;
	while (cin >> K) {
		if (K == 0)
			break;
		cin >> divisionx >> divisiony;
		int x, y;
		for (int i = 0; i < K; i++) {
			cin >> x >> y;
			if (x == divisionx || y == divisiony)
				cout << "divisa" << "\n";
			else if (x < divisionx && y > divisiony)
				cout << "NO" << "\n";
			else if (x > divisionx && y > divisiony)
				cout << "NE" << "\n";
			else if (x > divisionx && y < divisiony)
				cout << "SE" << "\n";
			else if (x < divisionx && y < divisiony)
				cout << "SO" << "\n";
		}
	}
	system("pause");
	return 0;
}