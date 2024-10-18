#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <climits>  // For INT_MAX

using namespace std;

struct Process {
    int id, bt, at, start = -1, comp = -1;
};

// Function to calculate response times
vector<int> find_response(vector<Process>& input, int n) {
    vector<int> rt(n);
    for (int i = 0; i < n; i++) {
        rt[i] = input[i].start - input[i].at;
    }
    return rt;
}

// Function to calculate turnaround times
vector<int> find_tat(vector<Process>& input, int n) {
    vector<int> tat(n);
    for (int i = 0; i < n; i++) {
        tat[i] = input[i].comp - input[i].at;
    }
    return tat;
}

// Function to calculate waiting times
vector<int> find_wt(vector<Process>& input, vector<int>& tat, int n) {
    vector<int> wt(n);
    for (int i = 0; i < n; i++) {
        wt[i] = tat[i] - input[i].bt;
    }
    return wt;
}

// Comparison function for sorting processes by arrival time, and then by ID
static bool comp(Process& a, Process& b) {
    if (a.at == b.at) return a.id < b.id;
    return a.at < b.at;
}

// Function to perform SRTF scheduling
void schedule(vector<Process>& input, int n) {
    sort(input.begin(), input.end(), comp);
    int time = 0, completed = 0;

    vector<int> remaining(n);
    for (int i = 0; i < n; i++) {
        remaining[i] = input[i].bt;
    }

    while (completed != n) {
        int min_bt = INT_MAX;
        int shortest = -1;

        // Find the process with the shortest remaining time that has arrived
        for (int j = 0; j < n; j++) {
            if (remaining[j] > 0 && input[j].at <= time && remaining[j] < min_bt) {
                min_bt = remaining[j];
                shortest = j;
            }
        }

        // If no process is ready, increment time
        if (shortest == -1) {
            time++;
            continue;
        }

        // If the process is being executed for the first time
        if (remaining[shortest] == input[shortest].bt) {
            input[shortest].start = time;
        }

        remaining[shortest]--;  // Reduce remaining time for the selected process
        time++;  // Increment current time

        // If the process has finished
        if (remaining[shortest] == 0) {
            input[shortest].comp = time;
            completed++;
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of processes: " << endl;
    cin >> n;

    vector<Process> input(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter the id, burst time and arrival time of process " << i + 1 << ":" << endl;
        cin >> input[i].id >> input[i].bt >> input[i].at;
    }

    schedule(input, n);

    vector<int> response = find_response(input, n);
    vector<int> turntime = find_tat(input, n);
    vector<int> waittime = find_wt(input, turntime, n);

    // Output the results in a table format
    cout << setw(8) << "Process"
         << setw(8) << "Burst"
         << setw(8) << "Arrival"
         << setw(8) << "Start"
         << setw(12) << "Completion"
         << setw(10) << "Response"
         << setw(12) << "Turn Around"
         << setw(8) << "Waiting"
         << endl;

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
