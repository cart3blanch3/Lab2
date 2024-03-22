#include <windows.h>  
#include <iostream>    

int main() {   // Основная функция программы
    HANDLE fileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, L"MySharedMemory");   // Открытие существующего отображенного в память файла для чтения

    if (!fileMapping) {   // Проверка успешности открытия файла отображенного в память
        std::cerr << "Error opening file mapping." << std::endl;   // Вывод сообщения об ошибке
        return 1;   // Возврат кода ошибки
    }

    LPVOID mapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);   // Отображение файла в память для чтения

    if (!mapView) {   // Проверка успешности отображения файла в память
        std::cerr << "Error mapping view of file." << std::endl;   // Вывод сообщения об ошибке
        CloseHandle(fileMapping);   // Закрытие отображенного в память файла
        return 1;   // Возврат кода ошибки
    }

    HANDLE dataReadyEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"DataReadyEvent");   // Открытие события

    if (!dataReadyEvent) {   // Проверка успешности открытия события
        std::cerr << "Error opening event." << std::endl;   // Вывод сообщения об ошибке
        Sleep(30000);   // Задержка на 30 секунд
        UnmapViewOfFile(mapView);   // Отмена отображения файла в память
        CloseHandle(fileMapping);   // Закрытие отображенного в память файла
        return 1;   // Возврат кода ошибки
    }

    WaitForSingleObject(dataReadyEvent, INFINITE);   // Ожидание сигнала от события

    std::cout << "Process 2 received: " << static_cast<const char*>(mapView) << std::endl;   // Вывод полученных данных на экран

    UnmapViewOfFile(mapView);   // Отмена отображения файла в память
    CloseHandle(dataReadyEvent);   // Закрытие события
    CloseHandle(fileMapping);   // Закрытие отображенного в память файла

    std::cout << "Press Enter to exit...";   // Вывод сообщения на экран
    std::cin.get();   // Ожидание нажатия клавиши Enter пользователем

    return 0;   // Возврат успешного завершения программы
}