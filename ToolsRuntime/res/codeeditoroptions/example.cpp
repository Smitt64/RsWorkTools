#include <iostream>
#include <string>

// Определение константы
#define PI 3.14159265358979323846

// Перечисление
enum class Color {RED, GREEN, BLUE};

// Функция
int sum(int a, int b) 
{
    return a + b;
}

int main() 
{
    // Использование кастомного типа
    Color myColor = Color::RED;

    // Использование перечисления
    if(myColor == Color::RED) {
        std::cout << "Color is RED" << std::endl;
    }

    // Использование функции
    int result = sum(5, 10);
    std::cout << "Sum is " << result << std::endl;

    // Использование строки
    std::string message = "Hello, world!";
    std::cout << message << std::endl;

    // Работа с числами
    double radius = 5.0;
    double area = PI * radius * radius;
    std::cout << "Area is " << area << std::endl;

    return 0;
}
