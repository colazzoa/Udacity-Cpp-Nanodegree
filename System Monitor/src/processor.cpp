#include "processor.h"

float Processor::Utilization() { 
float utilization = 0;

// idel = idle + iowait
idle = std::stoi(cpu_values[kIdle_]) +  std::stoi(cpu_values[kIOwait_]); 
//non_idle = user + nice + system + irq + softirq + steal
non_idle = std::stoi(cpu_values[kUser_]) +  std::stoi(cpu_values[kNice_]) + std::stoi(cpu_values[kSystem_]) 
+ std::stoi(cpu_values[kIRQ_]) + std::stoi(cpu_values[kSoftIRQ_]) + std::stoi(cpu_values[kSteal_]);

const unsigned long int total = idle + non_idle;
const unsigned long int prev_total = prev_idle + prev_non_idle;

const int total_d = total - prev_total;
const int idle_d = idle - prev_idle;

if(total_d != 0)
    utilization =  (float)(total_d - idle_d) / total_d;

prev_idle = idle;
prev_non_idle = non_idle;

return utilization;
}

void Processor::set_values(vector<string> values_in){
cpu_values = values_in;
}