#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>

using namespace std;

void LimpiarPantalla(void);
bool AntiCaracter(int& selector);
bool RangoEntrada(int selector, int min_val, int max_val);
int Menu(void);
void Sintaxis(void);
void mostrar_funcion(const string& funcion);
string obtener_funcion();

int main()
{
    while(1)
    {
        int selector = Menu();
        if(selector == 3)
        {
            cout << "Saliendo..." << endl;
            system("pause"); 
            return 0;
        }
        if(selector == 2)
        {
            void Sintaxis();
            continue;
        }

        string funcion_ingresada = obtener_funcion();
        cout << "Funcion final ingresada: " << funcion_ingresada << endl;
        
        system("pause"); 
        return 0;
    }
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
        this_thread::sleep_for(chrono::milliseconds(1500));
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
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
        LimpiarPantalla();
        return true;
    }
    return false;
}

int Menu(void)
{
    int selector;
    while (1)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "           METODO DE LA SECANTE           " << endl;
        cout << "==========================================" << endl;
        cout << "1. Ejecutar el Metodo de la Secante." << endl;
        cout << "2. Mostrar Sintaxis Permitida." << endl;
        cout << "3. Salir del Programa." << endl;
        cout << "------------------------------------------" << endl;
        cout << endl << endl << "Seleccion: ";
        cin >> selector;
        
        if (AntiCaracter(selector)) continue;
        if (RangoEntrada(selector, 1, 3)) continue;
        break;
    }
    return selector;
}

void Sintaxis(void)
{
    LimpiarPantalla();
    cout << "==========================================" << endl;
    cout << "           SINTAXIS PERMITIDA             " << endl;
    cout << "==========================================" << endl;

    cout << "VARIABLE:" << endl;
    cout << "  x" << endl << endl;

    cout << "OPERADORES:" << endl;
    cout << "  +   suma" << endl;
    cout << "  -   resta" << endl;
    cout << "  *   multiplicacion (obligatoria)" << endl;
    cout << "  /   division" << endl;
    cout << "  p   potencia (xp2 = x^2)" << endl << endl;

    cout << "FUNCIONES DISPONIBLES (se insertan automaticamente):" << endl;
    cout << "  sin(" << endl;
    cout << "  cos(" << endl;
    cout << "  tan(" << endl;
    cout << "  log(" << endl;
    cout << "  exp(" << endl;
    cout << "  sqrt(" << endl << endl;

    cout << "REGLAS IMPORTANTES:" << endl;
    cout << "  1) No se permiten espacios." << endl;
    cout << "  2) Las funciones agregan solo el parentesis inicial; EL USUARIO debe cerrar ) manualmente." << endl;
    cout << "  3) La multiplicacion SIEMPRE debe escribirse con '*'" << endl;
    cout << "  4) La potencia se escribe con 'p', no con '^'" << endl;

    cout << "------------------------------------------" << endl;
    system("pause"); 
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
    cout << "SINTAXIS: Use '*' para multiplicar y 'p' para potencias." << endl;
    cout << "----------------------------------------" << endl;
    cout << "f(x) = " << funcion << " " << endl;
    cout << "----------------------------------------" << endl;
}

string obtener_funcion()
{
    string funcion = "";
    char tecla;

    while(1)
    {
        mostrar_funcion(funcion);
        
        tecla = _getch();

        // 1. Caracteres permitidos directos (1 caracter)
        if ((tecla >= '0' && tecla <= '9') || tecla == '.' || 
            tecla == '+' || tecla == '-' || tecla == '*' || 
            tecla == '/' || tecla == '^' || tecla == 'x' || tecla == ')') 
        {
            funcion += tecla;
        }

        // ======== NUEVO: POTENCIAS CON 'p' ========
        else if (tecla == 'p' || tecla == 'P')
        {
            char expo = _getch();  // Leemos siguiente tecla

            // si el exponente es un dígito (0–9)
            if (expo >= '0' && expo <= '9')
            {
                funcion += '^';
                funcion += expo;
            }
            // si no es un dígito, NO agregamos nada
        }
        // ===========================================
        
        // 2. Inserción de funciones trig/log
        else if (tecla == 's') funcion += "sin(";
        else if (tecla == 'c') funcion += "cos(";
        else if (tecla == 't') funcion += "tan(";
        else if (tecla == 'l') funcion += "log(";

        // 3. Bloque de Borrado (B/b)
        else if (tecla == 'B' || tecla == 'b')
        {
            if (!funcion.empty())
            {
                int len = funcion.length();

                if (len >= 4)
                {
                    string ultimo = funcion.substr(len - 4, 4);
                    if (ultimo == "sin(" || ultimo == "cos(" || 
                        ultimo == "tan(" || ultimo == "log(")
                    {
                        funcion.erase(len - 4);
                        continue;
                    }
                }

                funcion.pop_back();
            }
        }

        // 4. Finalizar entrada
        else if (tecla == 'E' || tecla == 'e')
        {
            break;
        }
        // demás teclas se ignoran
    }

    system("cls");
    return funcion;
}