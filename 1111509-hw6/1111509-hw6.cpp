#include<iostream>
using std::cin;
using std::cout;
using std::endl;

#include<cmath>
using std::log10;

// returns true if and only if the specified huge integer is zero
bool isZero( int *hugeInt, int size );

// return true if and only if hugeInt1 == hugeInt2
bool equal( int *hugeInt1, int *hugeInt2, int size1, int size2 );

// returns true if and only if hugeInt1 < hugeInt2
bool less( int *hugeInt1, int *hugeInt2, int size1, int size2 );

// --hugeInt
void decrement( int *&hugeInt, int &size );

// addend += adder
void addAssign( int *&addend, int *adder, int &addendSize, int adderSize );

// minuend -= subtrahend provided that minuend > subtrahend
void subtraAssign( int *&minuend, int *subtrahend, int &minuendSize, int subtrahendSize );

// multiplicand *= multiplier
void multiAssign( int *&multiplicand, int *multiplier, int &multiplicandSize, int multiplierSize );

// quotient = dividend / divisor; remainder = dividend % divisor
void division( int *dividend, int *divisor, int *&quotient, int *&remainder,
               int dividendSize, int divisorSize, int &quotientSize, int &remainderSize );

// hugeInt /= 10, or equivalently, shifts right by one position
void divideBy10( int *&hugeInt, int &size );

// hugeInt1 = hugeInt2; assignment
void assign( int *&hugeInt1, int *hugeInt2, int &size1, int size2 );

void reset( int *&hugeInt, int &size );

const int maxSize = 200;

int main()
{
   int t, a, b;
   while( cin >> t >> a >> b )
   {
      cout << "(" << t << "^" << a << "-1)/(" << t << "^" << b << "-1) ";

      if( t == 1 )
         cout << "is not an integer with less than 100 digits.\n";
      else if( a == b )
         cout << "1\n";
      else if( a < b )
         cout << "is not an integer with less than 100 digits.\n";
      else
      {
         int *dividend = new int[ 1 ]();
         int *divisor = new int[ 1 ]();
         int *quotient = new int[ 1 ]();
         int *remainder = new int[ 1 ]();

         int dividendSize = 1;
         int divisorSize = 1;
         int quotientSize = 1;
         int remainderSize = 1;

         // put all digits of t into base
         int *base = new int[ 10 ]();
         int baseSize = 0;
         for( int i = t; i > 0; i /= 10 )
            base[ baseSize++ ] = i % 10;

         // dividend = pow( t, a )
         dividend[ 0 ] = 1;
         for( int i = 0; i < a; ++i )
         {
            multiAssign( dividend, base, dividendSize, baseSize );
            if( dividendSize > maxSize - baseSize )
               break;
         }

         if( dividendSize > maxSize - baseSize )
            cout << "is not an integer with less than 100 digits.\n";
         else
         {
            // divisor = pow( t, b )
            divisor[ 0 ] = 1;
            for( int i = 0; i < b; ++i )
            {
               multiAssign( divisor, base, divisorSize, baseSize );
               if( divisorSize > maxSize - baseSize )
                  break;
            }

            if( divisorSize > maxSize - baseSize )
               cout << "is not an integer with less than 100 digits.\n";
            else
            {
               decrement( dividend, dividendSize ); // --dividend
               decrement( divisor, divisorSize );   // --divisor

               division( dividend, divisor, quotient, remainder,
                  dividendSize, divisorSize, quotientSize, remainderSize );

               if( quotientSize > 1 && quotient[ quotientSize - 1 ] == 0 )
                  cout << "quotient has a leading zero!\n";

               if( remainderSize > 1 && remainder[ remainderSize - 1 ] == 0 )
                  cout << "remainder has a leading zero!\n";

               // quotient is an integer with less than 100 digits
               if( quotientSize < 100 && isZero( remainder, remainderSize ) )
                  for( int i = quotientSize - 1; i >= 0; i-- )
                     cout << quotient[ i ];
               else
                  cout << "is not an integer with less than 100 digits.";
               cout << endl;
            }
         }
      }
   }
}

// returns true if and only if the specified huge integer is zero
bool isZero( int *hugeInt, int size )
{
   return ( size == 1 && hugeInt[ 0 ] == 0 );
}

// return true if and only if hugeInt1 == hugeInt2
bool equal( int *hugeInt1, int *hugeInt2, int size1, int size2 )
{
   if( size1 != size2 )
      return false;

   for( int i = size1 - 1; i >= 0; i-- )
      if( hugeInt1[ i ] != hugeInt2[ i ] )
         return false;

   return true;
}

// returns true if and only if hugeInt1 < hugeInt2
bool less( int *hugeInt1, int *hugeInt2, int size1, int size2 )
{
    if (size1 < size2) 
        return true;
    if (size1 == size2) {
        for (int i = size1 - 1; i >= 0; i--) {
            if (hugeInt1[i] < hugeInt2[i])
                return true;
            if (hugeInt1[i] > hugeInt2[i])
                return false;
        }
    }
    return false;

}

// --hugeInt
void decrement( int *&hugeInt, int &size )
{
    int bufferSize = size;
    int *buffer = new int[bufferSize]();
    for(int i=0;i<size;i++){
        buffer[i] = hugeInt[i];
    }
    buffer[0]--;
    for (int i = 0; i < bufferSize - 1; i++) {
        if (buffer[i] < 0) {
            buffer[i] += 10;
            buffer[i + 1]--;
        }
    }
    for (int i = bufferSize - 1; i >= 0; i--) {
        if (buffer[i] == 0)
            bufferSize--;
        else break;
    }
   if( bufferSize > 1 && buffer[ bufferSize - 1 ] == 0 )
      cout << "buffer has a leading zero!\n";

   assign( hugeInt, buffer, size, bufferSize ); // hugeInt = buffer

   delete[] buffer;
}

// addend += adder
void addAssign( int *&addend, int *adder, int &addendSize, int adderSize )
{
   int sumSize = ( addendSize >= adderSize ) ? addendSize + 1 : adderSize + 1;
   int *sum = new int[ sumSize ]();

   for( int i = 0; i < addendSize; i++ )
      sum[ i ] = addend[ i ];

   for( int i = 0; i < adderSize; i++ )
      sum[ i ] += adder[ i ];

   for( int i = 0; i < sumSize - 1; i++ )
      if( sum[ i ] > 9 ) // carrying
      {
         sum[ i ] -= 10;
         sum[ i + 1 ]++;
      }

   if( sum[ sumSize - 1 ] == 0 )
      sumSize--;

   if( sumSize > 1 && sum[ sumSize - 1 ] == 0 )
      cout << "sum has a leading zero!\n";

   assign( addend, sum, addendSize, sumSize ); // addend = sum

   delete[] sum;
}

// minuend -= subtrahend provided that minuend > subtrahend
void subtraAssign( int *&minuend, int *subtrahend, int &minuendSize, int subtrahendSize )
{
   if( equal( minuend, subtrahend, minuendSize, subtrahendSize ) )
   {
      reset( minuend, minuendSize );
      return;
   }
   int differenceSize=minuendSize;
   int *difference = new int[ differenceSize ]();
   for (int i = 0; i < subtrahendSize; i++)
       difference[i] = minuend[i] - subtrahend[i];
   for (int i = subtrahendSize; i < minuendSize; i++)
       difference[i] = minuend[i];
   for(int i=0;i<differenceSize-1;i++){
      if(difference[i]<0){
         difference[i]+=10;
         difference[i+1]--;
      }
   }
   for(int i=differenceSize-1;i>=0;i--){
      if(difference[i]==0)
         differenceSize--;
      else break;
   }

   if( differenceSize > 1 && difference[ differenceSize - 1 ] == 0 )
      cout << "difference has a leading zero!\n";

   assign( minuend, difference, minuendSize, differenceSize ); // minuend = difference
   
   delete[] difference;
}

// multiplicand *= multiplier
void multiAssign( int *&multiplicand, int *multiplier, int &multiplicandSize, int multiplierSize )
{
   int productSize = multiplicandSize + multiplierSize; 
   int *product= new int[productSize](); 
   for(int i=0;i<multiplierSize;i++){
      for(int j=0;j<multiplicandSize;j++){
         product[i+j]+=(multiplicand[j]*multiplier[i]);
      }
   }
   for(int i=0;i<productSize;i++){
      if(product[i]>=10){
         product[i+1]+=(product[i] / 10);
         product[i]%=10;
      }
   }

   for (int i = productSize - 1; i >= 0; i--) {
       if (product[i] == 0)
           productSize--;
       else break;
   }

   if( productSize > 1 && product[ productSize - 1 ] == 0 )
      cout << "product has a leading zero!\n";

   assign( multiplicand, product, multiplicandSize, productSize ); // multiplicand = product

   delete[] product;
}

// quotient = dividend / divisor; remainder = dividend % divisor
void division( int *dividend, int *divisor, int *&quotient, int *&remainder,
               int dividendSize, int divisorSize, int &quotientSize, int &remainderSize )
{
   assign( remainder, dividend, remainderSize, dividendSize ); // remainder = dividend

   if( less( dividend, divisor, dividendSize, divisorSize ) )
      reset( quotient, quotientSize );
   else
   {
      int bufferSize = dividendSize;
      int *buffer = new int[ bufferSize ]();
      //buffer = divisor shift left by (dividendSize-divissorSize) position
      for(int i=0;i<divisorSize;i++){
         buffer[i+(dividendSize-divisorSize)]=divisor[i];
      }
      quotientSize=dividendSize-divisorSize;
      if(less(remainder, buffer, remainderSize, bufferSize))
          divideBy10(buffer, bufferSize);
      else
         quotientSize++;

      delete[] quotient;
      quotient = new int[quotientSize]();

      for(int k=quotientSize-1;k>=0;k--){
          while (!less(remainder, buffer, remainderSize, bufferSize)){
              subtraAssign(remainder, buffer, remainderSize, bufferSize);
              quotient[k]++;
              if (isZero(remainder, remainderSize)){
                  break;
              }
          }
          divideBy10(buffer, bufferSize);
      }
      delete[] buffer;

   }
}

// hugeInt /= 10, or equivalently, shifts right by one position
void divideBy10( int *&hugeInt, int &size )
{
   if( size == 1 )
      hugeInt[ 0 ] = 0;
   else
   {
      int *buffer = new int[ 1 ];
      int bufferSize = 1;
      assign( buffer, hugeInt, bufferSize, size ); // buffer = hugeInt

      size--;
      delete[] hugeInt;
      hugeInt = new int[ size ]();

      for( int i = 1; i <= size; i++ )
         hugeInt[ i - 1 ] = buffer[ i ];
   }
}

// hugeInt1 = hugeInt2; assignment
void assign( int *&hugeInt1, int *hugeInt2, int &size1, int size2 )
{
   size1 = size2;
   delete[] hugeInt1;
   hugeInt1 = new int[ size1 ];
   for( int i = 0; i < size1; i++ )
      hugeInt1[ i ] = hugeInt2[ i ];
}

void reset( int *&hugeInt, int &size )
{
   size = 1;
   delete[] hugeInt;
   hugeInt = new int[ 1 ]();
}