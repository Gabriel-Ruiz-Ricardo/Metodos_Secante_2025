#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>
#include <map>
#include <vector>

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
            Sintaxis();
            continue;
        }

        string funcion_ingresada = obtener_funcion();
        cout << "Funcion final ingresada: " << funcion_ingresada << endl;
        
        system("pause"); 
        continue;
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
        for (int i = 3; i > 0; i--)
        {
            cout << "\rEntrada invalida. Solo numeros... Intentalo de nuevo en " << i << " segundos";
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
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

cout << "VARIABLE: x" << endl << endl;

    cout << "OPERADORES:" << endl;
    cout << "  +  : suma" << endl;
    cout << "  -  : resta" << endl;
    cout << "  * : multiplicacion" << endl;
    cout << "  /  : division" << endl;
    cout << "  p  : potencia (^)" << endl;
    cout << "  ^  : potencia" << endl << endl;

    cout << "FUNCIONES DISPONIBLES:" << endl;
    cout << "  s  : sin(" << endl;
    cout << "  c  : cos(" << endl;
    cout << "  t  : tan(" << endl;
    cout << "  l  : log(  (Natural: ln) )" << endl; 
    cout << "  L  : log10( (Base 10) )" << endl; 
    cout << "  e  : exp(" << endl; 
    cout << "  r  : sqrt(" << endl << endl;
    
    cout << "  cs : csc(" << endl; 
    cout << "  sc : sec(" << endl; 
    cout << "  ct : cot(" << endl; 
    cout << "  as : asin(" << endl; 
    cout << "  ac : acos(" << endl; 
    cout << "  at : atan(" << endl; 
    cout << "  d  : abs(" << endl;
    
    cout << "CONSTANTES:" << endl;
    cout << "  i  : pi" << endl;
    cout << "  u  : e (Constante de Euler)" << endl << endl;

    cout << "NOTACION CIENTIFICA:" << endl;
    cout << "  e-  " << endl;  
    cout << "  e+  " << endl << endl;   

    cout << "REGLAS IMPORTANTES:" << endl;
    cout << "  1) No se permiten espacios." << endl;
    cout << "  2) Todo se escribe con minusculas a excepcion de funciones especiales." << endl;
    cout << "  3) Las funciones agregan solo el parentesis inicial; EL usuario debe cerrar con ')' manualmente." << endl;
    cout << "  4) La multiplicacion siempre debe escribirse con '*'" << endl;
    cout << "  5) La potencia se escribe con 'p' o '^'" << endl;
    cout << "  6) Para usar la notacion cientifica debes colocar un numero antes." << endl;
    cout << "------------------------------------------" << endl;
    system("pause"); 
}


void mostrar_funcion(const string& funcion)
{
    LimpiarPantalla();
    cout << "========================================" << endl;
    cout << "     INGRESE SU FUNCION (f(x) = 0)      " << endl;
    cout << "========================================" << endl;
    cout << "Borrar: Backspace | Finalizar: Enter" << endl;
    cout << "----------------------------------------" << endl;
    cout << "SINTAXIS: Use '*' para multiplicar y 'p' o '^' para potencias." << endl;
    cout << "----------------------------------------" << endl;
    cout << "f(x) = " << funcion << " " << endl;
    cout << "----------------------------------------" << endl;
}

string obtener_funcion()
{
    string funcion = "";
    char tecla;
    
    // Mapeo interno para el BORRADO (Longitudes de terminación)
    const map<string, int> terminaciones_insercion = {
        {"log10(", 6}, {"asin(", 5}, {"acos(", 5}, {"atan(", 5}, {"sqrt(", 5}, 
        {"sin(", 4}, {"cos(", 4}, {"tan(", 4}, {"log(", 4}, {"exp(", 4}, 
        {"abs(", 4}, {"csc(", 4}, {"sec(", 4}, {"cot(", 4}, {"pi", 2}, {"e", 1}
    };

    // Mapeo interno para INSERCIÓN (Simple y Doble)
    const map<string, string> mapeo_insercion = {
        {"s", "sin("},   {"c", "cos("},   {"t", "tan("},   {"l", "log("}, 
        {"L", "log10("}, {"r", "sqrt("},  {"d", "abs("},   {"i", "pi"},
        {"u", "e"}, 
        {"cs", "csc("},  {"sc", "sec("},  {"ct", "cot("},  {"as", "asin("},
        {"ac", "acos("}, {"at", "atan("}, {"ex", "exp("} 
    };


    while(1)
    {
        mostrar_funcion(funcion);
        tecla = _getch(); 

        // 1. Manejo de Teclas de Control (Enter: 13, Backspace: 8)
        if (tecla == 13) break; 
        else if (tecla == 8) 
        {
            if (funcion.empty()) continue; 
            int len = funcion.length();
            
            // Borrado Múltiple
            const vector<int> longitudes = {6, 5, 4, 2, 1}; 
            bool borrado_multiple = false;
            
            for (int size : longitudes) {
                if (len >= size) {
                    string ultimo = funcion.substr(len - size, size);
                    if (terminaciones_insercion.count(ultimo)) {
                        funcion.erase(len - size);
                        borrado_multiple = true;
                        break;
                    }
                }
            }
            if (borrado_multiple) continue;
            
            funcion.pop_back(); // Borrado simple
            continue;
        }

        // 2. Manejo ESPECIAL de la tecla 'e' (Función 'exp', Notación Científica)
        if (tecla == 'e')
        {
            char siguiente = _getch(); 
            string pulsacion_doble = string(1, tecla) + siguiente;

            // Opción A: Función Exponencial (ex)
            if (pulsacion_doble == "ex") {
                funcion += mapeo_insercion.at("ex");
                continue;
            }
            // Opción B: Notación Científica (e- o e+)
            else if (siguiente == '-' || siguiente == '+') 
            {
                // VALIDACIÓN DE REGLA 6: Debe haber un número antes
                if (!funcion.empty() && isdigit(funcion.back())) 
                {
                    funcion += 'e';
                    funcion += siguiente;
                    continue;
                }
                // Si no hay un dígito antes, la pulsación se ignora.
            }
            // Si no fue 'ex' ni notación científica válida, la pulsación se ignora.
            continue; 
        }

        // 3. Bloque de Detección de Pulsaciones Dobles (s, c, a)
        if (tecla == 's' || tecla == 'c' || tecla == 'a')
        {
            char siguiente = _getch(); 
            string pulsacion_doble = string(1, tecla) + siguiente;

            if (mapeo_insercion.count(pulsacion_doble))
            {
                funcion += mapeo_insercion.at(pulsacion_doble);
                continue;
            }
            else
            {
                // Insertamos la forma simple de la primera tecla y el segundo carácter.
                if (mapeo_insercion.count(string(1, tecla))) {
                    funcion += mapeo_insercion.at(string(1, tecla));
                } else {
                    funcion += tecla;
                }
                funcion += siguiente;
                continue;
            }
        }

        // 4. Manejo de Pulsaciones Simples (L, r, t, d, i, u, l)
        string pulsacion_str(1, tecla);
        if (mapeo_insercion.count(pulsacion_str))
        {
            funcion += mapeo_insercion.at(pulsacion_str);
            continue;
        }

        // 5. Manejo de Caracteres Directos y Alias
        else if ((tecla >= '0' && tecla <= '9') || tecla == '.' || 
                 tecla == '+' || tecla == '-' || tecla == '*' || 
                 tecla == '/' || tecla == '^' || tecla == 'x' || tecla == ')') 
        {
            funcion += tecla;
        }
        else if (tecla == 'p' || tecla == 'P') 
        {
            funcion += '^'; // Alias para potencia
        }
    }

    system("cls");
    return funcion;
}