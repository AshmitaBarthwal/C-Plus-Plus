#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

// Define a structure for Process
struct Process {
    int id, bt, at, start, comp; // id: process ID, bt: burst time, at: arrival time, start: start time, comp: completion time
};

// Function to calculate response time for each process
vector<int> find_response(vector<Process>& input, int n) {
    vector<int> rt(n);
    for (int i = 0; i < n; i++) {
        rt[i] = abs(input[i].start - input[i].at); // Response Time = Start Time - Arrival Time
    }
    return rt;
}

// Function to calculate turnaround time for each process
vector<int> find_tat(vector<Process>& input, int n) {
    vector<int> tat(n);
    for (int i = 0; i < n; i++) {
        tat[i] = abs(input[i].comp - input[i].at); // Turnaround Time = Completion Time - Arrival Time
    }
    return tat;
}

// Function to calculate waiting time for each process
vector<int> find_wt(vector<Process>& input, vector<int>& tat, int n) {
    vector<int> wt(n);
    for (int i = 0; i < n; i++) {
        wt[i] = abs(tat[i] - input[i].bt); // Waiting Time = Turnaround Time - Burst Time
    }
    return wt;
}

// Comparator function for sorting processes based on arrival time (and process ID for ties)
static bool comp(Process& a, Process& b) {
    if (a.at == b.at) return a.id < b.id; // Sort by arrival time, and by process ID if arrival times are equal
    return a.at < b.at; // Sort by arrival time
}

// Function to implement the Longest Job First (LJFS) scheduling algorithm
void schedule(vector<Process>& input, int n) {
    sort(input.begin(), input.end(), comp); // Sort processes by arrival time
    int time = 0; // Keeps track of current time

    vector<bool> completed(n, false); // Keeps track of whether a process has been completed
    for (int i = 0; i < n; i++) {
        int max_bt = INT_MIN; // Variable to store the longest burst time
        int arrived = -1; // Variable to store index of the process with the longest burst time

        // Find the process with the longest burst time that has arrived
        for (int j = 0; j < n; j++) {
            if (!completed[j] && input[j].at <= time && input[j].bt > max_bt) {
                max_bt = input[j].bt; // Update max burst time
                arrived = j; // Update arrived process
            }
        }

        // If no process has arrived, increment time and continue
        if (arrived == -1) {
            time++;
            i--;
            continue;
        }

        // Update the start and completion time for the selected process
        completed[arrived] = true;
        input[arrived].start = time;
        input[arrived].comp = time + input[arrived].bt;
        time = input[arrived].comp; // Move the time forward to the completion of the current process
    }
}

int main() {
    int n; 
    cout << "Enter the number of processes: " << endl;
    cin >> n; // Input the number of processes

    vector<Process> input(n); // Create a vector to store all processes
    // Input each process's id, burst time, and arrival time
    for (int i = 0; i < n; i++) {
        cout << "Enter the id, burst time, and arrival time of process " << i + 1 << ":" << endl;
        cin >> input[i].id >> input[i].bt >> input[i].at;
    }

    schedule(input, n); // Call the scheduling function

    // Calculate response time, turnaround time, and waiting time for each process
    vector<int> response = find_response(input, n);
    vector<int> turntime = find_tat(input, n);
    vector<int> waittime = find_wt(input, turntime, n);

    // Print the results in a tabular format
    cout << setw(8) << "Process"
         << setw(8) << "Burst"
         << setw(8) << "Arrival"
         << setw(8) << "Start"
         << setw(12) << "Completion"
         << setw(10) << "Response"
         << setw(12) << "Turn Around"
         << setw(8) << "Waiting"
         << endl;

    // Display process information
    for (int i = 0; i < n; i++) {
        cout << setw(8) << "P" + to_string(input[i].id)
             << setw(8) << input[i].bt
             << setw(8) << input[i].at
             << setw(8) << input[i].start
             << setw(12) << input[i].comp
             << setw(10) << response[i]
             << setw(12) << turntime[i]
             << setw(8) << waittime[i]
             << endl;
    }

    return 0;
}
