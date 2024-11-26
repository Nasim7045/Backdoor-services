#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

// Function to display the status of services
void DisplayServicesStatus() {
    SC_HANDLE hSCManager = OpenSCManager(nullptr, nullptr, SC_MANAGER_ENUMERATE_SERVICE);
    if (!hSCManager) {
        std::cerr << "Failed to open Service Control Manager. Error: " << GetLastError() << std::endl;
        return;
    }

    DWORD bytesNeeded, servicesReturned, resumeHandle = 0;
    DWORD bufSize = 4096;
    LPBYTE buffer = new BYTE[bufSize];

    if (!EnumServicesStatusEx(
            hSCManager,
            SC_ENUM_PROCESS_INFO,
            SERVICE_WIN32,
            SERVICE_STATE_ALL,
            buffer,
            bufSize,
            &bytesNeeded,
            &servicesReturned,
            &resumeHandle,
            nullptr)) {
        if (GetLastError() == ERROR_MORE_DATA) {
            delete[] buffer;
            bufSize += bytesNeeded;
            buffer = new BYTE[bufSize];
            EnumServicesStatusEx(
                hSCManager,
                SC_ENUM_PROCESS_INFO,
                SERVICE_WIN32,
                SERVICE_STATE_ALL,
                buffer,
                bufSize,
                &bytesNeeded,
                &servicesReturned,
                &resumeHandle,
                nullptr);
        } else {
            std::cerr << "Failed to enumerate services. Error: " << GetLastError() << std::endl;
            delete[] buffer;
            CloseServiceHandle(hSCManager);
            return;
        }
    }

    LPENUM_SERVICE_STATUS_PROCESS services = (LPENUM_SERVICE_STATUS_PROCESS)buffer;
    for (DWORD i = 0; i < servicesReturned; i++) {
        std::wcout << L"Service Name: " << services[i].lpServiceName << std::endl;
        std::wcout << L"Display Name: " << services[i].lpDisplayName << std::endl;
        std::wcout << L"Status: " << (services[i].ServiceStatusProcess.dwCurrentState == SERVICE_RUNNING ? L"Running" : L"Stopped") << std::endl;
        std::wcout << L"Process ID: " << services[i].ServiceStatusProcess.dwProcessId << std::endl;
        std::wcout << L"--------------------------" << std::endl;
    }

    delete[] buffer;
    CloseServiceHandle(hSCManager);
}

int main() {
    std::wcout << L"Fetching list of services and their statuses..." << std::endl;

    // Call the function to display services status
    DisplayServicesStatus();

    // Wait for 10 seconds before exiting
    std::wcout << L"Exiting in 10 seconds..." << std::endl;
    Sleep(10000);

    return 0;
}
