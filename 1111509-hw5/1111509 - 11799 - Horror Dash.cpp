#include <iostream>
#include <algorithm>
using namespace std;

int recMaxSpeed( int numRemaining );
int num{0};

int main()
{
	int T;
	cin >> T;
	for( int i = 1; i <= T; i++ )
	{
		int N;
		cin >> N;
		cout << "Case " << i << ": " << recMaxSpeed( N ) << endl;
		num=0;
	}

	system( "pause" );
}

int recMaxSpeed( int numRemaining )
{
		
	int speed;
	cin >> speed;
	if(num>speed){
		speed = num;
	}
	if(numRemaining==1)
		return speed;
	else {
		num = speed;
		return recMaxSpeed(numRemaining - 1);
	}
}