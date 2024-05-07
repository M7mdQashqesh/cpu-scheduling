#pragma once

// This Algorithm without ContxtSwitch Time..

#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;


void displayGanttChart(vector<pair<int, int>>& gantt) {
    // Print the top border of the Gantt chart
    cout << "+";
    for (auto& p : gantt) {
        cout << string(p.second * 4, '-') << "+"; // Adjust cell width to fit more information
    }
    cout << "\n|";

    // Print the process IDs with the start and end times in the Gantt chart
    int currentTime = 0;
    for (auto& p : gantt) {
        cout << "p" << p.first << "(" << currentTime << "-" << currentTime + p.second << ") |";
        currentTime += p.second;
    }

    cout << "\n+";
    for (auto& p : gantt) {
        cout << string(p.second * 4, '-') << "+"; // Match the width from the top border
    }
    cout << endl; // Removed the printing of the time line
}

void SRTF() {
    ifstream infile("processesForSRTF.txt");
    if (!infile) {
        cerr << "Error opening file." << endl;
        return;
    }

    int n;
    infile >> n;
    struct processOfSRTF p[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time = 0;
    int total_idle_time = 0;
    int burst_remaining[100];
    int is_completed[100];
    memset(is_completed, 0, sizeof(is_completed));

    cout << setprecision(2) << fixed;

    for (int i = 0; i < n; i++) {
        infile >> p[i].arrival_time >> p[i].burst_time;
        p[i].pid = i + 1;
        burst_remaining[i] = p[i].burst_time;
    }
    infile.close();

    vector<pair<int, int>> gantt; // Stores pair of PID and duration for Gantt chart
    int current_time = 0;
    int completed = 0;
    int prev = 0;

    while (completed != n) {
        int idx = -1;
        int mn = 10000000;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (burst_remaining[i] < mn) {
                    mn = burst_remaining[i];
                    idx = i;
                }
                if (burst_remaining[i] == mn) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        mn = burst_remaining[i];
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            if (burst_remaining[idx] == p[idx].burst_time) { // process starts now
                p[idx].start_time = current_time;
                total_idle_time += p[idx].start_time - prev;
            }
            burst_remaining[idx] -= 1;
            if (gantt.empty() || gantt.back().first != p[idx].pid) {
                gantt.push_back({ p[idx].pid, 1 });
            }
            else {
                gantt.back().second++;
            }
            current_time++;
            prev = current_time;

            if (burst_remaining[idx] == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

                total_turnaround_time += p[idx].turnaround_time;
                total_waiting_time += p[idx].waiting_time;

                is_completed[idx] = 1;
                completed++;
            }
        }
        else {
            current_time++;
        }
    }

    int min_arrival_time = 10000000;
    int max_completion_time = -1;
    for (int i = 0; i < n; i++) {
        min_arrival_time = min(min_arrival_time, p[i].arrival_time);
        max_completion_time = max(max_completion_time, p[i].completion_time);
    }

    avg_turnaround_time = (float)total_turnaround_time / n;
    avg_waiting_time = (float)total_waiting_time / n;
    cpu_utilisation = ((max_completion_time - total_idle_time) / (float)max_completion_time) * 100;

    cout << endl << endl;

    cout << "#P\t" << "ArrivalTime\t" << "BurstTime\t" << "StartTime\t" << "FinishTime\t" << "TAT\t" << "WT\t" << "\n" << endl;

    for (int i = 0; i < n; i++) {
        cout << p[i].pid << "\t" << p[i].arrival_time << "\t\t" << p[i].burst_time << "\t\t" << p[i].start_time << "\t\t" << p[i].completion_time << "\t\t" << p[i].turnaround_time << "\t" << p[i].waiting_time << "\t" << "\n" << endl;
    }
    cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
    cout << "Average Waiting Time = " << avg_waiting_time << endl;
    cout << "CPU Utilization = " << cpu_utilisation << "%" << endl;

    displayGanttChart(gantt);
}