#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm> 

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    cpu_.set_values(LinuxParser::CpuUtilization());
    return cpu_;
   }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 

vector<Process>().swap(processes_); //empty the vector of processes

vector<int> pids = LinuxParser::Pids();

for(unsigned int i=0; i< pids.size(); i++){
   Process process;
   process.SetPid(pids[i]);
   process.SetUser(LinuxParser::User(pids[i]));
   process.SetCpu(LinuxParser::CpuUtilization(pids[i]));
   process.sys_uptime = System::UpTime();
   process.SetCommand(LinuxParser::Command(pids[i]));
   process.SetRam(LinuxParser::Ram(pids[i]));
   processes_.push_back(process);
}

std::sort(processes_.begin(), processes_.end()); 
return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }