#include <stdio.h>
#include <iostream>
#include <gmp.h>


/********************************************************************************
 * This program calculates the approximate pi, by using the Leibniz forumla expansion
 * The program prompts you to enter the n'th term to aproximate to.
 * It usses gmp, as some very large n'th terms may be used.
 * The limitation for the max term you can calculate to is based on the max size of a string
 * The largest max term would be 9999999...... repeated 4,294,967,294 times. Very large!
 * Compile with g++ pi.cpp -lgmp
 ********************************************************************************/



using namespace std;

int main(void) {
    
    //ints
    mpz_t i, max;
    
    //floats
    mpf_t sum, term, denom, i_temp, const_1;
    
    //string for n'th term
    string n_term;
    
    cout << "Enter the n'th term to sum to: ";
    cin >> n_term;
    
    //set i to 1, max to 0
    mpz_init_set_ui(i, 1);
    mpz_init(max);
    
    //set max to user input
    mpz_set_str(max, n_term.c_str(), 10);
    
    //initialize
    mpf_init(term);
    mpf_init_set_ui(const_1, 1);
    mpf_init_set_ui(sum, 0);
    mpf_init_set_ui(i_temp, 1);
    mpf_init(denom);
    
    
    //for loop, i = 0; i < max, i++
     for(;mpz_cmp(i,max) < 0 ; mpz_add_ui(i,i, 1)) {
         
         //logic for forumla
         mpf_mul_ui(denom, i_temp, 2);
         mpf_sub_ui(denom, denom, 1);
         mpf_div(term, const_1, denom);
         
         //number mod 2 = 0, make term negative!
         if(mpz_divisible_ui_p(i, 2) != 0) {
             mpf_neg(term, term);
         }
         
         mpf_add(sum, sum, term);
         mpf_add_ui(i_temp,i_temp, 1);
     }
     
    printf("\n");
    mpf_mul_ui(sum, sum, 4); //times by 4
    mpf_out_str(stdout, 10, 100000, sum); //print
    
    //clear mpz
    mpz_clear(i);
    mpz_clear(max);
    
    //clear mpf
    mpf_clear(sum);
    mpf_clear(term);
    mpf_clear(denom);
    mpf_clear(i_temp);
    mpf_clear(const_1);
    
    
}
