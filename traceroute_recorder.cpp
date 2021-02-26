// tracert-recorder.cpp
// Nathaniel Morrow Feb, 2021
// written for Windows machines, will require slight modifications for Unix-based systems.

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <thread>
#include <sstream>

using namespace std;

void tracertToCsv(string inFile, string outFile);
void helper(string input);

int main(int argc, char *argv[])
{
    string input;
    vector<thread> threads;

    cout << "Welcome to Traceroute Testing and Recording!\n\n";

    /* Asks user for input on what IP or URL tracert should 
    be called on launch threads for tracert until asked to stop.
    This can be easily converted to read a file instead ;) */
    while (1)
    {
        cout << "What IP(s) or URL(s) would you like to Tracert?\n"
             << "Enter x when finished: ";
        cin >> input;
        if (input == "x" || input == "X")
            break;
        threads.emplace_back(helper, input);
        cout << "\"Tracert " << input << "\" has started on thread " << threads.size() + 1 << ".\n\n";
    }

    for (thread &t : threads)
        t.join();

    cout << "All files have completed successfully!\n";

    return 0;
}

void helper(string input)
{
    stringstream ss;
    ss << this_thread::get_id();
    string threadNum = ss.str();
    system(("tracert " + input + " > traceData" + threadNum + ".txt").c_str());
    cout << "\nBeginning csv conversion for " << input << " to data" << threadNum + ".csv.\n";
    //takes user input and does a system call for tracert then redirects the tracert. it is stored in the test.txt file.
    tracertToCsv("traceData" + threadNum + ".txt", "data" + threadNum + ".csv");
    cout << "\"Tracert " << input << "\" has been successfully recorded into data" << threadNum + ".csv.\n";
}

//takes a recorded tracert and converts to csv
void tracertToCsv(string inFile, string outFile)
{
    ifstream input(inFile);
    ofstream output(outFile);
    string temp;
    for (int i = 0; i < 4; i++)
        getline(input, temp);
    input >> temp;
    while (temp != "Trace")
    {
        output << temp;
        output << ',';
        for (int i = 0; i < 3; i++)
        {
            input >> temp;
            if (temp == "*")
            {
                output << temp;
                output << ',';
            }
            else
            {
                output << temp;
                input >> temp;
                output << temp;
                output << ',';
            }
        }
        getline(input, temp);
        output << temp;
        output << ",\n";
        input >> temp;
    }
    input.close();
    output.close();
    system(("del /f " + inFile).c_str());
}
