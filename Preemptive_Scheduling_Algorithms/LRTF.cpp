#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <climits>  // for INT_MIN

using namespace std;

struct Process {
    int id, bt, at, start = -1, comp;
};

vector<int> find_response(vector<Process>& input, int n) {
    vector<int> rt(n);
    for (int i = 0; i < n; i++) {
        rt[i] = input[i].start - input[i].at;
    }
    return rt;
}

vector<int> find_tat(vector<Process>& input, int n) {
    vector<int> tat(n);
    for (int i = 0; i < n; i++) {
        tat[i] = input[i].comp - input[i].at;
    }
    return tat;
}

vector<int> find_wt(vector<Process>& input, vector<int>& tat, int n) {
    vector<int> wt(n);
    for (int i = 0; i < n; i++) {
        wt[i] = tat[i] - input[i].bt;
    }
    return wt;
}

static bool comp(Process& a, Process& b) {
    if (a.at == b.at) return a.id < b.id;
    return a.at < b.at;
}

void schedule(vector<Process>& input, int n) {
    sort(input.begin(), input.end(), comp);  // Sort processes based on arrival time
    int time = 0, completed = 0;

    vector<int> remaining(n);
    for (int i = 0; i < n; i++) {
        remaining[i] = input[i].bt;
    }

    while (completed != n) {
        int max_bt = INT_MIN;
        int longest = -1;

        for (int j = 0; j < n; j++) {
            if (remaining[j] > 0 && input[j].at <= time && remaining[j] > max_bt) {
                max_bt = remaining[j];
                longest = j;
            }
        }

        if (longest == -1) {  // No process is available to execute at this time
            time++;
            continue;
        }

        if (input[longest].start == -1) {  // Mark start time only when process starts
            input[longest].start = time;
        }

        remaining[longest]--;
        time++;

        if (remaining[longest] == 0) {  // Process completed
            input[longest].comp = time;
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
