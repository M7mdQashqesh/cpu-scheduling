#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "PCB.h"

using namespace std;

int totalWT = 0, totalTAT = 0, totalBurstTime = 0;
Process process[5];
int memorySize, programSize, q, cs;
int currentTime;
int numberOfProcesses;
double cpuUtilization = 0;

// Function to read the data from file
void readFile() {

	ifstream inputFile;
	inputFile.open("processes.txt");
	if (!inputFile.is_open()) {
		cout << "cannt finputFiled the file " << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	//read file data 

	while (inputFile.good()) {
		inputFile >> numberOfProcesses;
		inputFile >> memorySize; // This variable likely represents the total memory size available for the system.
		inputFile >> programSize; // This variable likely represents the size of each process that needs to be executed by the CPU.
		inputFile >> q;
		inputFile >> cs;

		for (int i = 0; i < numberOfProcesses; i++) {
			inputFile >> process[i].id >> process[i].AT >> process[i].BT;
		}
	}

	inputFile.close();
}

// Gant chart for FCFS , SJF
void GanttChart() {

	currentTime = 0;
	cout << "\n  ===================" << endl;
	cout << "//    Gantt Chart   //" << endl;
	cout << "  ===================" << endl << endl;
	cout << "================================================================" << endl;
	for (int i = 0; i < numberOfProcesses; i++) {
		cout << "(" << currentTime << ")|==P" << process[i].id << "==|";
		// To calculate current time after each process finished 
		currentTime += process[i].BT + cs;
	}
	currentTime = currentTime - cs;
	cout << "(" << currentTime << ")";
	cout << "\n================================================================" << endl;
}

void findWaitingTime(Process proc[], int n, int wt[]) {
	// Waiting time for first process is 0
	wt[0] = 0;

	// calculat waiting time
	for (int i = 1; i < n; i++)
		wt[i] = proc[i - 1].BT + wt[i - 1] + proc[1].cs;
}

// Calculate turn around time
void findTurnAroundTime(Process proc[], int n, int wt[], int tat[]) {
	// Calculat turnaround time 
	for (int i = 0; i < n; i++)
		tat[i] = proc[i].BT + wt[i] + proc[1].cs;
}

// Function to calculate average time
void findavgTime(Process proc[], int n) {
	int wt[10], tat[10], totalWT = 0, totalTAT = 0;

	// Waiting time for all processes
	findWaitingTime(proc, n, wt);

	// Turn around time for all processes
	findTurnAroundTime(proc, n, wt, tat);

	// Display processes with all info
	cout << "\nProcesses  " << " Burst time  " << " Waiting time  " << " Turn around time\n";

	// Total WT and total TAT

	double sum = 0;
	for (int i = 0; i < numberOfProcesses; i++)
	{
		totalWT = totalWT + wt[i];
		totalTAT = totalTAT + tat[i];
		sum = sum + proc[i].BT;
		cout << "   " << proc[i].id << "\t\t"
			<< proc[i].BT << "\t    " << wt[i]
			<< "\t\t  " << tat[i] << endl;
	}

	cout << "\nAverage waiting time = "
		<< (float)totalWT / (float)n;
	cout << "\nAverage turn around time = "
		<< (float)totalTAT / (float)n << endl;

	cout << "cpu utlization = " << (sum * 1.0 / (sum + 12)) * 100 << "%" << endl;
}

void sorty() {
	bool swapped;
	do { // Rearrange processes upwards depending on arrival time
		swapped = false;
		for (int i = 0; i < numberOfProcesses - 1; i++) {
			if (process[i].AT > process[i + 1].AT) {
				swap(process[i], process[i + 1]);
				swapped = true;
			}
		}
	} while (swapped);
}


// Function to calculate & display the Shared Optimization Criteria in SJF & FCFS
void getInfo() {
	GanttChart(); // Call GanttChat function to display it
	totalBurstTime = 0; totalWT = 0; totalTAT = 0; // Reset totals
	process[0].finishTime = process[0].BT;
	process[0].wt = 0;
	process[0].TAT_time = process[0].finishTime - process[0].AT;

	for (int i = 1; i < numberOfProcesses; i++) { // To calculate waiting time & turnaround time & finish time for each process 
		totalBurstTime += process[i - 1].BT;
		process[i].finishTime = process[i - 1].finishTime + cs + process[i].BT;
		process[i].wt = process[i - 1].finishTime + cs - process[i].AT;
		process[i].TAT_time = process[i].finishTime - process[i].AT;
	}

	process[4].finishTime -= cs;

	for (int i = 0; i < numberOfProcesses; i++) { // To calculate the totals
		totalWT += process[i].wt;
		totalTAT += process[i].TAT_time;
	}

	totalBurstTime += process[numberOfProcesses - 1].BT;
	cpuUtilization = float(totalBurstTime * 100.0 / currentTime); // To calculate cpu utilization
	// To display Information for each process in table
	cout << "Process\t|    TAT Time   | Waiting Time\t| finish Time";
	cout << "\n";

	for (int i = 0; i < numberOfProcesses; i++) {
		cout << "P(" << process[i].id << ")\t|\t" << process[i].TAT_time << "\t|\t" << process[i].wt << "\t|\t" << process[i].finishTime << endl;

	}

	// To display the Averages
	cout << "\nAverage Wating Time = " << (double)totalWT / (numberOfProcesses * 1.0) << endl;
	cout << "Average Turnaround Time = " << (double)totalTAT / (numberOfProcesses * 1.0) << endl;
	cout << "Cpu Utilization = " << cpuUtilization << "%" << endl;
	return;
}