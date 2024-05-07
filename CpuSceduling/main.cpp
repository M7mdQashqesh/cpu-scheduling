// This project was written by Mohammed Qashqesh
// My Id is 211014
// Dr.Radwan Tahboub

#include <iostream>

#include "PCB.h"
#include "Helpers.h"
#include "Algorithms.h"
#include "SRTF.h"

using namespace std;

int main() {
    int firstChoice;

    do {
        cout << "1) FCFS, SJF, and RR Algorithms\n"
            << "2) SRTF Algorithm\n"
            << "3) Exit.\n"
            << "Enter your choice: ";
        cin >> firstChoice;

        switch (firstChoice) {
        case 1: {
            readFile();
            cout << "Your file information:" << endl << endl;
            cout << "Process\t| Arrival Time  | Burst Time \n";

            for (int i = 0; i < numberOfProcesses; i++) {
                cout << "P(" << process[i].id << ")\t|\t" << process[i].AT << "\t|\t" << process[i].BT << "\n";
            }

            int choice;
            do {
                cout << "=======================================\n"
                    << "1) First-Come First-Served (FCFS)\n"
                    << "2) Short Job First (SJF)\n"
                    << "3) Round-Robin (RR)\n"
                    << "4) Go Back.\n"
                    << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                case 1:
                    FCFS();
                    system("pause");
                    break;

                case 2:
                    SJF();
                    system("pause");
                    break;

                case 3:
                    RR();
                    system("pause");
                    break;

                case 4:
                    break;

                default:
                    cout << "Invalid Choice" << endl;
                    system("pause");
                    break;
                }

            } while (choice != 4);
            break;
        }

        case 2: {
            SRTF();
            system("pause");
            break;
        }

        case 3:
            cout << "Bye <3.\n";
            system("pause");
            break;

        default:
            cout << "Invalid Choice!\n";
            system("pause");
            break;
        }
    } while (firstChoice != 3);

    return 0;
}