#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <thread>
#include <chrono>

// Function to list all running processes
void listRunningProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;

    // Take a snapshot of all processes in the system
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to take process snapshot." << std::endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process
    if (!Process32First(hProcessSnap, &pe32)) {
        std::cerr << "Failed to retrieve process information." << std::endl;
        CloseHandle(hProcessSnap);
        return;
    }

    std::cout << "Listing all running processes:" << std::endl;

    // Iterate through all processes
    do {
        std::wcout << L"Process Name: " << pe32.szExeFile
                   << L" | Process ID: " << pe32.th32ProcessID << std::endl;
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

// Main function
int main() {
    std::cout << "Program will monitor running processes for 10 seconds..." << std::endl;

    // Run the process listing in a separate thread
    std::thread monitorThread([]() {
        while (true) {
            system("cls"); // Clear the console for better readability
            listRunningProcesses();
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Refresh every second
        }
    });

    // Run the program for 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    // Stop the monitoring thread
    monitorThread.detach();
    std::cout << "Monitoring stopped after 10 seconds." << std::endl;

    return 0;
}
