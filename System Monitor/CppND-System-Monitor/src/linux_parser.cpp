#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>


#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the memory utilization 
float LinuxParser::MemoryUtilization() { 
  float mem_total{1},mem_free{0};
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "MemTotal:") {
          mem_total = std::stof(value);
        }
        if (key == "MemFree:") {
          mem_free = std::stof(value);
        }
      }
    }
  return ((mem_total-mem_free)/mem_total);
 }

long LinuxParser::UpTime() {
  long up_time = 0;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
    up_time = std::stol(value);
    }
  return up_time;
 }

// Read and return the timing counters of the CPU
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_timers;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
      for(int i = 0; i < 11; i++){
        linestream >> value;
        cpu_timers.push_back(value);
      }
    }
  cpu_timers.erase(cpu_timers.begin());
  return cpu_timers;
}

// Read and return the timing counters of the CPU for a specif PID
vector<string> LinuxParser::CpuUtilization(int pid) { 
  vector<string> cpu_timers;
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
      for(int i = 0; i < 22; i++){
        linestream >> value;
        cpu_timers.push_back(value);
      }
    }
  return cpu_timers;
}

// Read and return the number of total processes
int LinuxParser::TotalProcesses() {
  int tot_proc = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "processes") {
          tot_proc = std::stoi(value);
        }
      }
    }
  return tot_proc;
 }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int running_proc = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "procs_running") {
          running_proc = std::stoi(value);
        }
      }
    }
  return running_proc;
 }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    }
  return line; 
 }

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "VmSize:") {
          return value;
        }
      }
    }
  return value;  }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
        if (key == "Uid:") {
          return value;
        }
      }
    }
  return value; 
  }

//Read and return the user associated to a PID/UID
string LinuxParser::User(int pid) { 
  string line;
  string user;
  string value;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> value >> value) {
        if (value == uid) {
          return user;
        }
      }
    }
  }
  return user;
}