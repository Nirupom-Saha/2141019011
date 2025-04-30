#include<iostream>
#include<filesystem> //iterate /proc directory
#include<fstream> //read files(/proc data)
#include<sstream>//parse data
#include<algorithm> //sort processes
#include<vector> //store processes
#include<unistd.h> // sysconf for _SC_CLK_TCK
#include<dirent.h>

namespace fs = std::filesystem;

struct ProcessInfo {
	int pid;
	std::string name;
	double cpuUsage;
	long memoryUsage;
};

std::string readFileValue(const std::string &path) {
	std::ifstream file(path);
	std::string value;
	if(file.is_open()) {
		std::getline(file , value);
	}
	return value;
}

double getSystemUptime() {
	std::ifstream file("/proc/uptime");
	double uptime;
	if(file.is_open()) {
		file >> uptime;
	}
	return uptime;
}

ProcessInfo getProcessInfo(int pid, double systemUptime) {
	ProcessInfo pinfo;
	pinfo.pid = pid;
	pinfo.cpuUsage = 0.0;
	pinfo.memoryUsage = 0;

	std::ifstream file("/proc/" + std::to_string(pid)+ "/stat");
	std::string line;
	if(file.is_open()) {
		std::getline(file , line);
		std::istringstream ss(line);
		std::string token;
		int count = 0;
		long utime = 0, stime = 0, starttime = 0;

		while(ss >> token) {
			count++;
			if(count == 2) pinfo.name = token;
			else if (count == 14) utime = std::stol(token);
			else if (count == 15) stime = std::stol(token);
			else if (count == 22) starttime = std::stol(token);
		}

		long clk_tck = sysconf(_SC_CLK_TCK);
		long total_time = utime + stime;
		double seconds = systemUptime - (starttime / (double)clk_tck);

		if (seconds > 0)
			pinfo.cpuUsage = ((total_time / (double)clk_tck) / seconds) * 100.0;
	}

	std::ifstream memFile("/proc/" + std::to_string(pid) + "/status");
	if(memFile.is_open()) {
		std::string line;
		while(std::getline(memFile, line)) {
			if(line.find("VmRSS:") == 0) {
				std::istringstream iss(line);
				std::string key;
				long mem;
				std::string unit;
				iss >> key >> mem >> unit;
				pinfo.memoryUsage = mem;
				break;
			}
		}
	}
	return pinfo;
}

std::vector<ProcessInfo> getAllProcesses(){
	std::vector<ProcessInfo> processes;
	double systemUptime = getSystemUptime();

	for(const auto &entry : fs::directory_iterator("/proc")){
		if(entry.is_directory()){
			std::string filename = entry.path().filename().string();

			if(all_of(filename.begin(), filename.end(), ::isdigit)){
				int pid = std::stoi(filename);
				processes.push_back(getProcessInfo(pid, systemUptime));
			}
		}
	}
	return processes;
}

void sortProcesses(std::vector<ProcessInfo> &processes, bool sortByCPU){
	if(sortByCPU){
		sort(processes.begin(), processes.end(), [](const ProcessInfo &a, const ProcessInfo &b) {
			return a.cpuUsage > b.cpuUsage;
		});
	}else{
		sort(processes.begin(), processes.end(), [](const ProcessInfo &a, const ProcessInfo &b){
			return a.memoryUsage > b.memoryUsage;
		});
	}
}

int main() {
	std::vector<ProcessInfo> processes = getAllProcesses();
	sortProcesses(processes, true); 

	std::cout << "PID\tCPU%\tMemory (kB)\tName\n";

	for(size_t i = 0; i < std::min(processes.size(), size_t(10)); ++i) {
		std::cout << processes[i].pid << "\t"
		          << processes[i].cpuUsage << "%\t"
		          << processes[i].memoryUsage << "\t"
		          << processes[i].name << "\n";
	}

	return 0;
}

