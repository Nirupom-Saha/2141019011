#include <iostream>
#include <filesystem> // Used for listing directories
#include <fstream>    // Helps in reading files
#include <sstream>    // Helps in parsing file content
#include <algorithm>  // Needed for std::all_of
#include <unistd.h>   // For sysconf and _SC_CLK_TCK

namespace fs = std::filesystem;

bool isNumber(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    // Checks if all characters in s are digits
}

std::string getProcessName(int pid) {
    std::ifstream file("/proc/" + std::to_string(pid) + "/stat"); // Open the file
    std::string line, processName;
    
    if (file.is_open()) {
        std::getline(file, line); // Read the first line
        std::istringstream ss(line);
        std::string token;
        int count = 0;
        
        while (ss >> token) {
            count++;
            if (count == 2) { // Process name is the second token
                processName = token;
                break;
            }
        }
    }
    
    return processName;
}

long getMemoryUsage(int pid) {
    std::ifstream file("/proc/" + std::to_string(pid) + "/status"); // Open the file
    std::string line, key;
    long memoryUsage = 0;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            ss >> key;
            if (key == "VmRSS:") { // Look for the memory usage line
                ss >> memoryUsage;
                break;
            }
        }
    }
    
    return memoryUsage;
}

double getCPUUsage(int pid) {
    std::ifstream file("/proc/" + std::to_string(pid) + "/stat");
    std::string line;
    long utime, stime, starttime;
    double cpuUsage = 0.0;

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream ss(line);
        std::string token;
        int count = 0;

        while (ss >> token) {
            count++;
            if (count == 14) utime = std::stol(token);  // User CPU time
            if (count == 15) stime = std::stol(token);  // System CPU time
            if (count == 22) starttime = std::stol(token); // Start time
        }
    }

    double uptime = std::ifstream("/proc/uptime").get() ? std::stod(std::string(std::istreambuf_iterator<char>(std::ifstream("/proc/uptime").rdbuf()), std::istreambuf_iterator<char>())) : 0.0;
    long total_time = utime + stime;
    cpuUsage = (static_cast<double>(total_time) / sysconf(_SC_CLK_TCK)) / uptime * 100;
    
    return cpuUsage;
}

int main() {
    std::cout << "Active Processes:\n";

    // Extract process list
    for (const auto &entry : fs::directory_iterator("/proc")) {
        if (entry.is_directory()) {
            std::string filename = entry.path().filename().string();
            if (isNumber(filename)) {
                int pid = std::stoi(filename);
                std::string processName = getProcessName(pid);
                long memoryUsage = getMemoryUsage(pid);
                double cpuUsage = getCPUUsage(pid);
                std::cout << "PID: " << pid << " | Name: " << processName 
                          << " | CPU: " << cpuUsage << "% | Memory: " << memoryUsage << " KB\n";
            }
        }
    }
    
    return 0;
}

