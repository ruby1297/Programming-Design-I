#include <iostream>
using namespace std;

long long int recFinalPremium( int numFarmersRemaining );

int main()
{
   int n;
   cin >> n;
   for( int i = 0; i < n; i++ )
   {
      int f;
      cin >> f;
      cout << recFinalPremium( f ) << endl;
   }

   system( "pause" );
}

long long int recFinalPremium( int numFarmersRemaining )
{
   long long int size;
   int numAnimals;
   long long int friendliness;
   if(numFarmersRemaining==1){
   cin >> size >> numAnimals >> friendliness;
      return size* friendliness;
    }
    else{
   cin >> size >> numAnimals >> friendliness;
   return recFinalPremium(numFarmersRemaining-1)+size* friendliness;
    } 
}