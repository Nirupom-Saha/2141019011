#include<iostream>
#include<sys/sysinfo.h>
using namespace std;

void displayMemoryInfo(){
	struct sysinfo info;
	if(sysinfo(&info)==0){
		cout << "---------- System Memory Info ----------\n";
		cout << "Total RAM: " << info.totalram / (1024 * 1024) << " MB\n";
		cout << "Total RAM: " << info.totalram / (1024 * 1024 * 1024) << " GB\n";
		cout << "Free RAM: " << info.freeram / (1024 * 1024) << " MB\n";
		cout << "Free RAM: " << info.freeram / (1024 * 1024 * 1024) << " GB\n";
		cout << "----------------------------------------\n";
	}
	else{
		perror("sysinfo failed");
	}
}

void displayUptime(){
	struct sysinfo info;
	if(sysinfo(&info)==0){
		cout << "System Uptime: " << info.uptime << " seconds\n";
	}
}

int main(){
	displayMemoryInfo();
	displayUptime();
	return 0;
}

