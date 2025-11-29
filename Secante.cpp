#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>
#include <map>
#include <vector>
#include "exprtk/exprtk.hpp"

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>     expression_t;
typedef exprtk::parser<double>         parser_t;

using namespace std;

void LimpiarPantalla(void);
bool AntiCaracter(int& selector);
bool RangoEntrada(int selector, int min_val, int max_val);
int Menu(void);
void Sintaxis(void);
void mostrar_funcion(const string& funcion);
string obtener_funcion();
std::string validar_sintaxis_exprtk(const std::string& funcion_str);

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
    bool sintaxis_valida = false;
    //Mapeo interno para el borrado
    const map<string, int> terminaciones_insercion = 
    {
        {"log10(", 6}, {"asin(", 5}, {"acos(", 5}, {"atan(", 5}, {"sqrt(", 5}, 
        {"sin(", 4}, {"cos(", 4}, {"tan(", 4}, {"log(", 4}, {"exp(", 4}, 
        {"abs(", 4}, {"csc(", 4}, {"sec(", 4}, {"cot(", 4}, {"pi", 2}, {"e", 1}
    };
    // Mapeo interno para la insercion
    const map<string, string> mapeo_insercion = 
    {
        {"s", "sin("},   {"c", "cos("},   {"t", "tan("},   {"l", "log("}, 
        {"L", "log10("}, {"r", "sqrt("},  {"d", "abs("},   {"i", "pi"},
        {"u", "e"}, 
        {"cs", "csc("},  {"sc", "sec("},  {"ct", "cot("},  {"as", "asin("},
        {"ac", "acos("}, {"at", "atan("}, {"ex", "exp("} 
    };

    while (!sintaxis_valida)
    {
        mostrar_funcion(funcion);
        tecla = _getch(); 

        if (tecla == 13) // Enter: Intentar salir del modo edición
        {
            if (!funcion.empty()) 
            {
                LimpiarPantalla();
                cout << "Funcion ingresada: f(x) = " << funcion << endl;
                cout << "------------------------------------------" << endl;
                string error_msg = validar_sintaxis_exprtk(funcion);
                
                if (error_msg.empty())
                {
                    cout << "Sintaxis valida. ¿Desea continuar? (S/N): ";
                    char confirm = _getch();        
                    if (confirm == 'S' || confirm == 's') 
                    {
                        LimpiarPantalla();
                        return funcion; // ¡Salida exitosa!
                    } 
                    // Si presiona 'N', continúa el bucle para seguir editando
                }
                else
                {
                    // Error de compilación detectado por ExprTK
                    cout << "==========================================" << endl;
                    cout << "      ERROR DE SINTAXIS DETECTADO         " << endl;
                    cout << "==========================================" << endl;
                    cout << "Mensaje de ExprTK: " << error_msg << endl;
                    cout << "Presione cualquier tecla para editar de nuevo..." << endl;
                    _getch();
                }
            }
            continue;
        }
        else if (tecla == 8) // Backspace
        {
            int len = funcion.length();
            if (len > 0) 
            {
                const vector<int> longitudes = {6, 5, 4, 2, 1}; 
                bool borrado_multiple = false;
                for (int size : longitudes) 
                {
                    if (len >= size)
                    {
                        string ultimo = funcion.substr(len - size, size);
                        if (terminaciones_insercion.count(ultimo)) 
                        {
                            funcion.erase(len - size);
                            borrado_multiple = true;
                            break;
                        }
                    }
                }
                    
                if (!borrado_multiple) funcion.pop_back(); 
            }
            continue;
        }
        else if (tecla == 'e')
        {
            char siguiente = _getch(); 
            string pulsacion_doble = string(1, tecla) + siguiente;

            if (pulsacion_doble == "ex") 
            {
                funcion += mapeo_insercion.at("ex");
                continue;
            }
            else if (siguiente == '-' || siguiente == '+') 
            {
                if (!funcion.empty() && isdigit(funcion.back())) 
                {
                    funcion += 'e';
                    funcion += siguiente;
                    continue;
                }
            }
            continue; 
        }
        else if (tecla == 's' || tecla == 'c' || tecla == 'a')
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
                if (mapeo_insercion.count(string(1, tecla))) 
                {
                    funcion += mapeo_insercion.at(string(1, tecla));
                } 
                else
                {
                    funcion += tecla;
                }
                funcion += siguiente;
                continue;
            }
        }
        else
        {
            string pulsacion_str(1, tecla);
            if (mapeo_insercion.count(pulsacion_str))
            {
                funcion += mapeo_insercion.at(pulsacion_str);
                continue;
            }
            else if ((tecla >= '0' && tecla <= '9') || tecla == '.' || 
                     tecla == '+' || tecla == '-' || tecla == '*' || 
                     tecla == '/' || tecla == '^' || tecla == 'x' || tecla == ')') 
            {
                funcion += tecla;
            }
            else if (tecla == 'p' || tecla == 'P') 
            {
                funcion += '^';
            }
        }
    }
    
    // Este return nunca debería alcanzarse debido a la lógica del while
    return funcion;
}

std::string validar_sintaxis_exprtk(const std::string& funcion_str)
{
    double x_valor = 0.0;
    
    symbol_table_t symbol_table;
    symbol_table.add_variable("x", x_valor); 
    symbol_table.add_constants(); // Agrega pi, e, etc.
    
    expression_t expression;
    expression.register_symbol_table(symbol_table);
    
    parser_t parser;
    
    // Intenta compilar. Si falla, obtiene los errores
    if (!parser.compile(funcion_str, expression)) 
    {
        std::string error_str = "";
        for (std::size_t i = 0; i < parser.error_count(); ++i)
        {
            typedef exprtk::parser_error::type error_t;
            error_t error = parser.get_error(i);
            error_str += error.diagnostic;
            if (i < parser.error_count() - 1) 
            {
                error_str += "\n";
            }
        }
        return error_str;
    }
    
    return ""; // Cadena vacía indica éxito
}