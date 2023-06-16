#include<iostream>
using namespace std;
int main() {
	int T;
	cin >> T;
	for (int i = 0; i < T; i++) {
		int num,height[51]{0};
		cin >> num;
		for (int j = 0; j < num; j++) {
			cin >> height[j];
		}
		int high{ 0 }, low{ 0 };
		for (int j = 1; j < num; j++) {
			if (height[j] > height[j - 1])
				high += 1;
			else if (height[j] < height[j - 1])
				low += 1;
		}
		cout << "Case " << i+1 <<": " << high << ' ' << low << "\n";
	}
	system("pause");
	return 0;
}