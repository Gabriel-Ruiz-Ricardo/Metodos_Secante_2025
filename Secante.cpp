#include <iostream>
#include <locale>
#include <thread>
#include <chrono>

void clearScreen(void);
void menu(void);

using namespace std;

int main()
{
	setlocale(LC_CTYPE,"spanish");
	menu();

    return 0;
}

void clearScreen() 
{
    std::cout << "\x1B[2J\x1B[H";
}

void menu(void)
{
    int selector;
    do
    {
        clearScreen();
        cout << "==========================================" << endl;
        cout << "          MÉTODO DE LA SECANTE            " << endl;
        cout << "==========================================" << endl;
        cout << "1. Ejecutar el Método de la Secante" << endl;
        cout << "2. Salir del Programa" << endl;
        cout << "------------------------------------------" << endl;
        cout << endl << endl << "Selección: ";
        cin >> selector;
        
        if (selector<1 || selector>2)
        {
            for (int i = 3; i > 0; i--)
            {
                cout << "\rOpción Inválida. Inténtalo de nuevo en " << i << " segundos" << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
    }
    while(selector<1 || selector>2);
}
