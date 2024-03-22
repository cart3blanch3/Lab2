#include <windows.h>   
#include <iostream>    

int main() {   // �������� ������� ���������
    HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 4096, L"MySharedMemory");   // �������� ������ ������������� � ������ ����� ��� ������

    if (!fileMapping) {   // �������� ���������� �������� ����� ������������� � ������
        std::cerr << "Error creating file mapping." << std::endl;   // ����� ��������� �� ������
        return 1;   // ������� ���� ������
    }

    LPVOID mapView = MapViewOfFile(fileMapping, FILE_MAP_WRITE, 0, 0, 0);   // ����������� ����� � ������ ��� ������

    if (!mapView) {   // �������� ���������� ����������� ����� � ������
        std::cerr << "Error mapping view of file." << std::endl;   // ����� ��������� �� ������
        CloseHandle(fileMapping);   // �������� ������������� � ������ �����
        return 1;   // ������� ���� ������
    }

    HANDLE dataReadyEvent = CreateEvent(nullptr, FALSE, FALSE, L"DataReadyEvent");   // �������� �������

    if (!dataReadyEvent) {   // �������� ���������� �������� �������
        std::cerr << "Error creating event." << std::endl;   // ����� ��������� �� ������
        UnmapViewOfFile(mapView);   // ������ ����������� ����� � ������
        CloseHandle(fileMapping);   // �������� ������������� � ������ �����
        return 1;   // ������� ���� ������
    }

    char message[] = "Hello from Process 1!";   // �������� ���������
    CopyMemory(mapView, message, sizeof(message));   // ����������� ��������� � ������������ � ������ ����
    SetEvent(dataReadyEvent);   // ��������� �������, ���������������� � ���������� ������

    std::cout << "Press Enter to exit...";   // ����� ��������� �� �����
    std::cin.get();   // �������� ������� ������� Enter �������������

    UnmapViewOfFile(mapView);   // ������ ����������� ����� � ������
    CloseHandle(dataReadyEvent);   // �������� �������
    CloseHandle(fileMapping);   // �������� ������������� � ������ �����

    return 0;   // ������� ��������� ���������� ���������
}