#ifndef FORMAT_H
#define FORMAT_H

#include <string>

int * digits(int number); //e.g. use to always obtain a format like 01:04:15 instead of 1:4:15

namespace Format {
std::string ElapsedTime(long times);  
};                                    // namespace Format

#endif