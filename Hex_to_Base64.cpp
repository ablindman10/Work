#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <gmpxx.h>

// compile with g++ [name].cpp -lgmpxx -lgmp
// this program converts hex(base 16) to base (64)
// It takes an input of capital leters (A-F) and numbers 0 - 9
// It converts the input into base 10
// It then creates an array of bits, (it figures out how many should be used)
// It takes those bits, and every 6 are used to calculate an index for base 64 converstion
// The program stops just before it goes out of bounds
// For example say say the array is 8 bits wide, it will stop at the 6th bit.
// The left over is then padded depending if 2 bits are left or 4 bits are left.


/*
Sample run: Input AB
base 10 value: 171
Array of bits: 10101011

Split into groups of 6

101010  11
The frist group when caluclated in base 10, converts to an index which produces "q"

The second group ends with just 2 bits. Enxtend this by 4 bits to make a second group of 6.

101010  110000

group 2 now produces "w", However we are still not done. The whole array must be divisible by 8. So check that

10101011 0000

Because it not, extend it by another 8.
10101011 00000000 

regroup by 6

101010 110000 0000

Still not divisible by 6 and 8, add 8 more, and regroup by 6
101010 110000 000000 000000

Now notice it is evenly divisble by 6 & 8. 
6 X 4 = 24
8 X 3 = 24

encode:
101010 110000 000000 000000
  q      w      =       =

*/

using namespace std;
mpz_class hex2dec(string arr, int size, mpz_class sum);
int arrayProcess(int arr[], int startLocation);
char HexVal(int dec);

int main()
{
  //allow a user to input a hex as a string
  string input;
  
  //create a large variable which returns the dec value
  mpz_class sum;
  
  cout << "Input a HEX no space:";
  cin >> input;
  
  //call the function
  sum = hex2dec(input, input.size(), sum);

  
  // make an array 2^n, makes sure it perfectly holds the array of bits
  
  int arrsize;
  for(int i = 0; i < 100000; i++) {
      if (sum < pow(2,4 * i)) {
          arrsize = i * 4;
          break;
      }
  }
  
  
  // create the array, then make all elements 0
  int arr[arrsize];
  
  for (int i = 0; i < arrsize; i++) {
      arr[i] = 0;
  }
  
  //logic for making the array of bits, by turning sum into binary
    for (int i = arrsize - 1; i >= 0; i--) {
        if(sum % 2 == 0) {
            arr[i] = 0;  
        } 
        
        else {
            arr[i] = 1;
        }
        sum /= 2;
    }
  
  
  
  //calcualte the last spot by 6, you can calculate the value.
  int lastloc = 0;
  for (int i = 1; i < arrsize; i++) {
      if (6 * i <= arrsize)
          lastloc++;
      if (6 * i > arrsize)
          break;
  }
  
    // calls the index functions to turn binary into base64
  cout << "The encoded base 64 is: ";
  for (int i = 1; i <= lastloc; i++) {
      cout << HexVal(arrayProcess(arr, i));
  }
  
  //how many elements an array has
  int arrayelements = sizeof(arr) / 4;
  
  //Padding logic
  if(arrayelements - lastloc * 6 == 2) {
        int newarr[arrayelements + 16];
        for (int i = 0; i < arrayelements + 16; i++) {
            newarr[i] = 0;
        }
        for(int i = 0; i < arrayelements; i++)
            newarr[i] = arr[i];
        cout << HexVal(arrayProcess(newarr, lastloc +1)) << "==";
    }
    
    if(arrayelements - lastloc * 6 == 4) {
        int newarr[arrayelements + 8];
        for (int i = 0; i < arrayelements + 8; i++) {
            newarr[i] = 0;
        }
        for(int i = 0; i < arrayelements; i++)
            newarr[i] = arr[i];
        cout << HexVal(arrayProcess(newarr, lastloc +1)) << "=";
    }
        
    cout << endl;
  
}

// converts any array of chars in hex to a decimal value.
mpz_class hex2dec(string arr, int size, mpz_class sum) {
    
    // for loop checks the value of each char, and converts it to dec
    for(int i = size - 1; i >= 0; i--) {
        if(arr[(size - 1) - i] <= 70 && arr[(size - 1) - i] >= 65){
	  sum += pow(16, i) * (arr[(size - 1) - i] - 55); 
	}
	else if(arr[(size - 1) - i] >= 48 && arr[(size - 1) - i] <= 57){
	  sum += pow(16, i) * (arr[(size - 1) - i] - 48);
	}
	else {
	  cout << "Invalid Input, " << arr[(size - 1) - i] << " is not a valid hex\n";
	  exit(0);
	}
    }
    return sum; //return the value
} 

//returns the sum for n'th set of 6 bits
int arrayProcess(int arr[], int startLocation) {
  int dec = 0;
  
  for (int j = startLocation; j <= startLocation ; j++) {
      for (int i = 6 * (j-1); i < 6 * j; i++) {
          dec += pow(2, 6 * j - i -1) * arr[i];
      }
  }
  return dec;
}

//turns the recived value into a character from base64
char HexVal(int dec) {
    if (dec >= 0 && dec <= 25)
        return char(65 + dec);
    if (dec >= 26 && dec <= 51)
        return char(71 + dec);
    if (dec >= 52 && dec <= 61)
        return char(dec - 4);
    if (dec == 62)
        return char(43);
    if (dec == 63)
        return char(47);
}

