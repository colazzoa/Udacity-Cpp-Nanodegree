#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
#include <unistd.h>
using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();
  void SetPid(int p);
  std::string User();
  void SetUser(std::string s);
  std::string Command();
  void SetCommand(std::string s);
  void SetCpu(std::vector<string> vs);
  float CpuUtilization() const;
  std::string Ram();
  void SetRam(std::string s);
  long int UpTime();
  bool operator<(Process const& a) const;
  long int sys_uptime = 0;

 private:
  int pid_ = 0;
  std::string user_ = {};
  std::vector<string> pid_cpu_timers = {};
  std::string command_ = {};
  std::string ram_ = {};  

};

#endif