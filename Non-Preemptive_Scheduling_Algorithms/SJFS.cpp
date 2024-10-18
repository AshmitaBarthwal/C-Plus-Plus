#include <iostream> 
#include <vector> 
#include <algorithm> 
#include <iomanip> 
using namespace std; 

struct Process { 
    int id, bt, at, start, comp; 
}; 

// Function to calculate response time for each process
vector<int> find_response(vector<Process>& input, int n) { 
    vector<int> rt(n); 
    for (int i = 0; i < n; i++) { 
        rt[i] = input[i].start - input[i].at;  // Response time = start time - arrival time
    } 
    return rt; 
} 

// Function to calculate turnaround time for each process
vector<int> find_tat(vector<Process>& input, int n) { 
    vector<int> tat(n); 
    for (int i = 0; i < n; i++) { 
        tat[i] = input[i].comp - input[i].at;  // Turnaround time = completion time - arrival time
    } 
    return tat; 
} 

// Function to calculate waiting time for each process
vector<int> find_wt(vector<Process>& input, vector<int>& tat, int n) { 
    vector<int> wt(n); 
    for (int i = 0; i < n; i++) { 
        wt[i] = tat[i] - input[i].bt;  // Waiting time = turnaround time - burst time
    } 
    return wt; 
} 

// Comparator function to sort processes based on arrival time (and ID if same arrival time)
static bool comp(Process& a, Process& b) { 
    if (a.at == b.at) return a.id < b.id; 
    return a.at < b.at; 
} 

// Function to implement non-preemptive SJF scheduling
void schedule(vector<Process>& input, int n) { 
    sort(input.begin(), input.end(), comp);  // Sort by arrival time and process ID
    int time = 0; 

    vector<bool> completed(n, false); 
    int completedCount = 0; 

    while (completedCount < n) { 
        int min_bt = INT_MAX; 
        int arrived = -1; 

        for (int j = 0; j < n; j++) { 
            // Select the process with the shortest burst time that has arrived and is not completed
            if (!completed[j] && input[j].at <= time && input[j].bt < min_bt) { 
                min_bt = input[j].bt; 
                arrived = j; 
            } 
        } 

        if (arrived == -1) { 
            time++;  // No process has arrived yet, so increment time
            continue; 
        } 

        // Process the selected process
        completed[arrived] = true; 
        input[arrived].start = time; 
        input[arrived].comp = time + input[arrived].bt; 
        time = input[arrived].comp; 
        completedCount++; 
    } 
} 

int main() { 
    int n; 
    cout << "Enter the number of processes: " << endl; 
    cin >> n; 

    vector<Process> input(n); 
    for (int i = 0; i < n; i++) { 
        cout << "Enter the ID, burst time, and arrival time of process " << i + 1 << ":" << endl; 
        cin >> input[i].id >> input[i].bt >> input[i].at; 
    } 

    schedule(input, n); 
    vector<int> response = find_response(input, n); 
    vector<int> turntime = find_tat(input, n); 
    vector<int> waittime = find_wt(input, turntime, n); 

    // Output table
    cout << setw(8) << "Process"  
         << setw(8) << "Burst"  
         << setw(8) << "Arrival"  
         << setw(8) << "Start"  
         << setw(12) << "Completion"  
         << setw(10) << "Response"  
         << setw(12) << "Turnaround"  
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
