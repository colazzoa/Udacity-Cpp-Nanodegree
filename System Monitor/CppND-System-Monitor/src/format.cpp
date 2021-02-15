#include <string>
#include <sstream>
#include <iostream>
#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    long int hh = seconds / 3600;
    int mm = (seconds - hh*3600) / 60;
    int ss = seconds - ((hh*3600)+(mm*60));
    std::stringstream sstream_;
    ( sstream_ << *(digits(hh)) <<  *(digits(hh)+1) << ":" << 
      *(digits(mm))  << *(digits(mm)+1) << ":" << *(digits(ss)) << *(digits(ss)+1) );
    return sstream_.str();
    }

int * digits(int number){
    static int r[2];
        
    r[0] = number / 10;
    r[1] = number - r[0]*10;

    return r;
    }