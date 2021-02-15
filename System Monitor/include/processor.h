#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

using std::string;
using std::vector;

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

class Processor {
 public:
  float Utilization();
  void set_values(vector<string> values_in);

 private:
  vector<string> cpu_values;
  unsigned long int idle = 10;
  unsigned long int non_idle = 1;
  unsigned long int prev_idle = 1;
  unsigned long int prev_non_idle = 1;
};

#endif