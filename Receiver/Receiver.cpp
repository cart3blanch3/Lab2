#include <windows.h>  
#include <iostream>    

int main() {   // �������� ������� ���������
    HANDLE fileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, L"MySharedMemory");   // �������� ������������� ������������� � ������ ����� ��� ������

    if (!fileMapping) {   // �������� ���������� �������� ����� ������������� � ������
        std::cerr << "Error opening file mapping." << std::endl;   // ����� ��������� �� ������
        return 1;   // ������� ���� ������
    }

    LPVOID mapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);   // ����������� ����� � ������ ��� ������

    if (!mapView) {   // �������� ���������� ����������� ����� � ������
        std::cerr << "Error mapping view of file." << std::endl;   // ����� ��������� �� ������
        CloseHandle(fileMapping);   // �������� ������������� � ������ �����
        return 1;   // ������� ���� ������
    }

    HANDLE dataReadyEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"DataReadyEvent");   // �������� �������

    if (!dataReadyEvent) {   // �������� ���������� �������� �������
        std::cerr << "Error opening event." << std::endl;   // ����� ��������� �� ������
        Sleep(30000);   // �������� �� 30 ������
        UnmapViewOfFile(mapView);   // ������ ����������� ����� � ������
        CloseHandle(fileMapping);   // �������� ������������� � ������ �����
        return 1;   // ������� ���� ������
    }

    WaitForSingleObject(dataReadyEvent, INFINITE);   // �������� ������� �� �������

    std::cout << "Process 2 received: " << static_cast<const char*>(mapView) << std::endl;   // ����� ���������� ������ �� �����

    UnmapViewOfFile(mapView);   // ������ ����������� ����� � ������
    CloseHandle(dataReadyEvent);   // �������� �������
    CloseHandle(fileMapping);   // �������� ������������� � ������ �����

    std::cout << "Press Enter to exit...";   // ����� ��������� �� �����
    std::cin.get();   // �������� ������� ������� Enter �������������

    return 0;   // ������� ��������� ���������� ���������
}