#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>

void LimpiarPantalla(void);
bool AntiCaracter(int& selector);
int Menu(void);

using namespace std;

int main()
{
    if (Menu() == 2)
    {
        cout << "Saliendo..." << endl;
        system("pause"); 
        return 0;
    }

    string funcion_ingresada = obtener_funcion();
    cout << "Funcion final ingresada: " << funcion_ingresada << endl;
    
    system("pause"); 
    return 0;
}

void LimpiarPantalla() 
{
    system("cls"); 
}

bool AntiCaracter(int& selector) 
{
    if (cin.fail()) 
    {
        cin.clear();
        cin.ignore(10000, '\n');
        selector = -1;
        cout << "\nEntrada invalida. Solo numeros..." << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        LimpiarPantalla();
        return true;
    }
    return false;
}

bool RangoEntrada(int selector, int min_val, int max_val)
{
    if (selector < min_val || selector > max_val)
    {
        for (int i = 3; i > 0; i--)
        {
            cout << "\rOpcion Invalida. Intentalo de nuevo en " << i << " segundos";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        LimpiarPantalla();
        return true;
    }
    return false;
}

int Menu(void)
{
    int selector;
    while(1)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "           METODO DE LA SECANTE           " << endl;
        cout << "==========================================" << endl;
        cout << "1. Ejecutar el Metodo de la Secante" << endl;
        cout << "2. Salir del Programa" << endl;
        cout << "------------------------------------------" << endl;
        cout << endl << endl << "Seleccion: ";
        cin >> selector;
        
        if (AntiCaracter(selector)) continue;
        if (RangoEntrada(selector,1,2)) continue;
        break;
    }
    return selector;
}

void mostrar_funcion(const string& funcion)
{
    LimpiarPantalla();
    cout << "========================================" << endl;
    cout << "     INGRESE SU FUNCION (f(x) = 0)      " << endl;
    cout << "========================================" << endl;
    cout << "Funciones: s:sin(, c:cos(, t:tan(, l:log(" << endl;
    cout << "Borrar: B/b | Finalizar: E/e" << endl;
    cout << "----------------------------------------" << endl;
    cout << "f(x) = " << funcion << " " << endl;
    cout << "----------------------------------------" << endl;
}

string obtener_funcion()
{
    string funcion = "";
    char tecla;

    while (true) {
        mostrar_funcion(funcion);
        
        // Lee una tecla sin esperar a presionar Enter
        tecla = _getch(); 

        // 1. Manejo de dígitos y operadores básicos (se añaden directamente)
        if (tecla >= '0' && tecla <= '9' || tecla == '.' || 
            tecla == '+' || tecla == '-' || tecla == '*' || 
            tecla == '/' || tecla == '^' || tecla == 'x' || tecla == ')') 
        {
            funcion += tecla;
        }

        // 2. Manejo de funciones (se añade el paréntesis de apertura)
        else if (tecla == 's') {
            funcion += "sin(";
        }
        else if (tecla == 'c') {
            funcion += "cos(";
        }
        else if (tecla == 't') {
            funcion += "tan(";
        }
        else if (tecla == 'l') {
            funcion += "log(";
        }
        
        // 3. Borrar (Backspace)
        else if (tecla == 'B' || tecla == 'b') {
            if (!funcion.empty()) {
                funcion.pop_back(); // Elimina el último caracter
            }
        }

        // 4. Finalizar entrada
        else if (tecla == 'E' || tecla == 'e') {
            break; 
        }
        
        // Ignorar cualquier otra tecla
    }

    // Limpia la pantalla final para mostrar el resultado
    system("cls");
    return funcion;
}
