#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>


/* 
 * This program takes a string of any size less than or equal to 55 chars
 * It takes those characters and calculates the sha-1 hash as laid out by the NSA
 * For the sake of time I limited it to 55 chars, however it could be extended
 * NSA Write up: https://tools.ietf.org/html/rfc3174
 */




#define MAX 100000 * sizeof(char)
uint32_t rotl32 (uint32_t value, unsigned int count);
void toBinary (char input, int *output);
void messageLengthPad(uint32_t length, uint32_t *array);
void binaryToHex(int *input, int *output);

int main(void) {
    //Initialize variables
    int i, j;
    uint32_t h0, h1, h2, h3, h4;
    uint32_t ml = 0;
    
    //password can be size of 100,000 chars, however program currently only supports 55 chars
    char password[MAX];

    uint32_t hash_block[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE ,0x10325476, 0xC3D2E1F0};

    //grab the password
    printf("Enter the password you wish to encrypt using sha-1: ");
    fgets(password, MAX, stdin);
    
    printf("The input was: %s", password);
    
    
    
    //calucate messege legth
    for(i = 0; password[i] != '\n'; i++)
        ml++;
    
    //array of bits for each char value given, 512 bits
    int bitarray[64][8];
    
    
    //take the password and turn it into an array of bits
    for(i = 0; i < ml; i++) {
        toBinary(password[i], bitarray[i]);
    }
    
    
    //start the padding process
    ml = ml * 8;
    
    
    
    //pad rest of the 512 bits to 0
    for(i = ml / 8; i < 64; i++) {
        for(j = 0; j < 8; j++)
            bitarray[i][j] = 0;
    }
    
    //pad the 1 on the ml + 1 location, example if input is abcde, ml = 40, pad 41 location to be 1 
    bitarray[ml / 8][0] = 1;
    
    
    //pad the last 64 bits big endian, so that it contains the message length
    messageLengthPad(ml, *bitarray);
    
    //array to hold hex, all only 448 bits will be used last 64 are reserved
    //can move this to the top later, leave for now
    uint32_t hexArray[16];
    
    //insert call the binaryToHex function here
    binaryToHex(*bitarray, hexArray);
    
    
    
    //last step of the padding process, store message length (ml) as a big endian
    //create large hex array for extention, should be extened to hold 80 total "numbers"
    uint32_t largeHexArray[80];
    
    //copy old values into new one
    for(i = 0; i < 16; i++)
        largeHexArray[i] = hexArray[i];
    
    //extend 16 hex words into 80
    for(i = 16; i < 80; i++) {
        largeHexArray[i] = rotl32((largeHexArray[i - 3] ^ largeHexArray[i - 8] ^ largeHexArray[i - 14] ^ largeHexArray[i - 16]), 1);
    }
    
    
    //needed for main loop
    uint32_t a, b, c, d, e, f, k, temp;
    a = hash_block[0];
    b = hash_block[1];
    c = hash_block[2];
    d = hash_block[3];
    e = hash_block[4];
    
    
    
    //main loop
    for(i = 0; i < 80; i++) {
        if(i >= 0 && i <= 19) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        
        else if (i >= 20 && i <= 39) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        
        else if (i >= 40 && i <= 59) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        
        else if (i >= 60 && i <= 79) {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        
        temp = (rotl32(a, 5)) + f + e + k + largeHexArray[i];
        e = d;
        d = c;
        c = rotl32(b, 30);
        b = a;
        a = temp;
    }
    
    
    //add back hash_block
    hash_block[0] += a;
    hash_block[1] += b;
    hash_block[2] += c;
    hash_block[3] += d;
    hash_block[4] += e;
    
    
    uint8_t hash[20];
    //now has is 160 bits little endian
    
    
    //using NSA method
    for(i = 0; i < 20; i++) {
        hash[i] = hash_block[i>>2] >> 8 * (3 - ( i & 0x03));
    }
    
    printf("The Final hash is: ");
    for(i = 0; i < 20; i++) {
        printf("%02X", hash[i]);
    }
    printf("\n");
        
}



void binaryToHex(int *input, int *output) {
    int i, j;
    uint32_t hexValue = 0;
    int temp[32];
    
    //copy the frist 32 bits
    for(i = 0; i < 16; i++) {
        for(j = (31 * (i + 1)) + i; j >= (32 * i); j--) {
            temp[j - (32 * i)] = *(input + j);
        }
        //calculate hexvalue
        for(j = 31; j >= 0; j--) {
            if(temp[j] == 1)
                hexValue += pow(2, 31 - j);
        }
        *(output + i) = hexValue;
        hexValue = 0;
    }
    
}


//pads the last 64 bits
void messageLengthPad(uint32_t length, uint32_t *array) {
    
    int i;
    
    //start from the 512 bit and work backwards
    for(i = 511; i >= 417; i--) {
        if(length % 2 == 0) {
        *(array + i) = 0;
        }
        
        else
            *(array + i) = 1;
        length /= 2;
    }
}


//left shift rotate
uint32_t rotl32 (uint32_t value, unsigned int count) {
    const unsigned int mask = (CHAR_BIT*sizeof(value)-1);
    count &= mask;
    return (value<<count) | (value>>( (-count) & mask ));
}

//converts the array of bits to bit value of char password
void toBinary(char input, int *output) {
        
    int i, j, value;
    value = input;
    
    for(i = 7; i >= 0; i--) {
        if(value % 2 == 0) {
            output[i] = 0;  
        } 
        
        else
            output[i] = 1;
        value /= 2;
    }
}
