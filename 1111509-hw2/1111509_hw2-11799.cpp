#include<iostream>
using namespace std;
int main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; i++) {
		int num,sec[10001],max{0};
		cin >> num;
		for (int j = 0; j < num; j++) {
			cin >> sec[j];
			if (sec[j] > max)
				max = sec[j];
		}
		cout << "Case " << i + 1 << ": " << max << "\n";
	}
	system("pause");
	return 0;
}