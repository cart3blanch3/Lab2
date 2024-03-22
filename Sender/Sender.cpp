#include <windows.h>   
#include <iostream>    

int main() {   // Основная функция программы
    HANDLE fileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 4096, L"MySharedMemory");   // Создание нового отображенного в память файла для записи

    if (!fileMapping) {   // Проверка успешности создания файла отображенного в память
        std::cerr << "Error creating file mapping." << std::endl;   // Вывод сообщения об ошибке
        return 1;   // Возврат кода ошибки
    }

    LPVOID mapView = MapViewOfFile(fileMapping, FILE_MAP_WRITE, 0, 0, 0);   // Отображение файла в память для записи

    if (!mapView) {   // Проверка успешности отображения файла в память
        std::cerr << "Error mapping view of file." << std::endl;   // Вывод сообщения об ошибке
        CloseHandle(fileMapping);   // Закрытие отображенного в память файла
        return 1;   // Возврат кода ошибки
    }

    HANDLE dataReadyEvent = CreateEvent(nullptr, FALSE, FALSE, L"DataReadyEvent");   // Создание события

    if (!dataReadyEvent) {   // Проверка успешности создания события
        std::cerr << "Error creating event." << std::endl;   // Вывод сообщения об ошибке
        UnmapViewOfFile(mapView);   // Отмена отображения файла в память
        CloseHandle(fileMapping);   // Закрытие отображенного в память файла
        return 1;   // Возврат кода ошибки
    }

    char message[] = "Hello from Process 1!";   // Создание сообщения
    CopyMemory(mapView, message, sizeof(message));   // Копирование сообщения в отображенный в память файл
    SetEvent(dataReadyEvent);   // Установка события, сигнализирующего о готовности данных

    std::cout << "Press Enter to exit...";   // Вывод сообщения на экран
    std::cin.get();   // Ожидание нажатия клавиши Enter пользователем

    UnmapViewOfFile(mapView);   // Отмена отображения файла в память
    CloseHandle(dataReadyEvent);   // Закрытие события
    CloseHandle(fileMapping);   // Закрытие отображенного в память файла

    return 0;   // Возврат успешного завершения программы
}