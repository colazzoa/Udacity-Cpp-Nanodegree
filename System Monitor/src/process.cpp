#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

void Process::SetPid(int p) { pid_ = p; }

void Process::SetCpu(vector<string> vs) {
    pid_cpu_timers = vs;
}

float Process::CpuUtilization() const {
float pid_cpu_usage = 0;
const long int hertz = sysconf(_SC_CLK_TCK); 

const long int utime = std::stoi(pid_cpu_timers[13]);
const long int stime = std::stoi(pid_cpu_timers[14]);
const long int cutime = std::stoi(pid_cpu_timers[15]);
const long int cstime = std::stoi(pid_cpu_timers[16]);
const long int starttime = std::stoi(pid_cpu_timers[21]);

const long int total_time = utime + stime + cutime + cstime;

const long int seconds = sys_uptime - (starttime / hertz);

if (seconds&hertz)
{
    pid_cpu_usage = (float) (total_time / hertz) / seconds;
}

return pid_cpu_usage;
}

// Set the command that generated this process
void Process::SetCommand(std::string s) { command_ = s; }

// Return the command that generated this process
string Process::Command() { return command_; }

// Set this process's memory utilization
void Process::SetRam(string s) { ram_ = s; }

// Return this process's memory utilization in MB
string Process::Ram() { return std::to_string(std::stoi(ram_ )/1024); }

string Process::User() { return user_; }

void Process::SetUser(string s) { user_ = s; }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    const long int hertz = sysconf(_SC_CLK_TCK); 
    const long int starttime = std::stoi(pid_cpu_timers[21]);
    const long int uptime = starttime / hertz;
    return sys_uptime - uptime;
}


// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
   float const &CPUutilization = CpuUtilization();
   float const &aUtilization = a.CpuUtilization();
   return CPUutilization > aUtilization;
 }