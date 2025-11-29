#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <conio.h>
#include <map>
#include <vector>
#include <cmath>
#include <limits>
#include <iomanip>
#include "exprtk.hpp"

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>     expression_t;
typedef exprtk::parser<double>         parser_t;

using namespace std;

struct RangoRaiz {
    double inicio;
    double fin;
    double valor_inicio;
    double valor_fin;
};

struct ResultadoIteracion {
    int iteracion;
    double x_anterior;
    double x_actual;
    double f_x_actual;
    double error_absoluto;
    double error_relativo;
    int cifras_significativas;
};

void LimpiarPantalla(void);
bool AntiCaracter(int& selector);
bool RangoEntrada(int selector, int min_val, int max_val);
int Menu(void);
void Sintaxis(void);
void mostrar_funcion(const string& funcion);
string obtener_funcion();
std::string validar_sintaxis_exprtk(const std::string& funcion_str);
double evaluar_funcion(const string& funcion_str, double x_val);
int preguntar_tipo_busqueda();
double preguntar_valor_objetivo();
string construir_funcion_ajustada(const string& funcion_original, double objetivo);
void preguntar_rango(double& x_min, double& x_max);
vector<RangoRaiz> buscar_cambios_signo(const string& funcion_str, double x_min, double x_max, double paso = 1.0);
int menu_seleccion_raices(const vector<RangoRaiz>& raices);
int menu_criterio_paro();
int preguntar_num_iteraciones();
double preguntar_error_relativo();
int preguntar_cifras_significativas();
int calcular_cifras_significativas(double error_relativo);
ResultadoIteracion metodo_secante(const string& funcion_str, double x0, double x1, int max_iter, double tol_error, int cifras_objetivo, int criterio, bool& exito);
void mostrar_resultado(const ResultadoIteracion& resultado, int criterio);
void mostrar_resultados_multiples(const vector<pair<RangoRaiz, ResultadoIteracion>>& resultados, int criterio);
bool preguntar_continuar(const string& mensaje);

int main()
{
    while(true)
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
        
        // Obtener y validar función
        string funcion_ingresada = obtener_funcion();
        LimpiarPantalla();
        
        // 1. Preguntar tipo de búsqueda
        int tipo_busqueda = preguntar_tipo_busqueda();
        double valor_objetivo = 0.0;
        string funcion_trabajo = funcion_ingresada;
        
        if(tipo_busqueda == 2) // Acercarse a un valor específico
        {
            valor_objetivo = preguntar_valor_objetivo();
            funcion_trabajo = construir_funcion_ajustada(funcion_ingresada, valor_objetivo);
            LimpiarPantalla();
            cout << "Funcion ajustada: f(x) = " << funcion_trabajo << endl;
            cout << "Buscando donde la funcion original valga " << valor_objetivo << endl;
            system("pause");
        }
        
        // Bucle para permitir calcular múltiples raíces
        bool continuar_raices = true;

        // 2. Preguntar rango de búsqueda
        double x_min, x_max;
        preguntar_rango(x_min, x_max);
        
        while(continuar_raices)
        {

            
            // 3. Buscar cambios de signo en el rango
            LimpiarPantalla();
            cout << "Buscando raices en el rango [" << x_min << ", " << x_max << "]..." << endl;
            vector<RangoRaiz> raices_encontradas = buscar_cambios_signo(funcion_trabajo, x_min, x_max);
            
            if(raices_encontradas.empty())
            {
                cout << "\nNo se encontraron cambios de signo en el rango especificado." << endl;
                cout << "No se pueden garantizar raices en este intervalo." << endl;
                system("pause");
                continue;
            }
            
            // Mostrar raíces encontradas y permitir selección
            int seleccion_raiz = menu_seleccion_raices(raices_encontradas);
            
            if(seleccion_raiz == 0) // Volver al menú principal
            {
                continuar_raices = false;
                continue;
            }
            
            // Bucle para permitir diferentes criterios de paro
            bool continuar_criterios = true;
            while(continuar_criterios)
            {
                // 4. Preguntar criterio de paro
                int criterio = menu_criterio_paro();
                
                if(criterio == 4) // Volver a selección de raíces
                {
                    continuar_criterios = false;
                    continue;
                }
                
                int max_iter = 1000;
                double tol_error = 1e-10;
                int cifras_objetivo = 0;
                
                if(criterio == 1) // Por número de iteraciones
                {
                    max_iter = preguntar_num_iteraciones();
                }
                else if(criterio == 2) // Por error relativo
                {
                    tol_error = preguntar_error_relativo();
                    max_iter = 10000; // Límite de seguridad
                }
                else if(criterio == 3) // Por cifras significativas
                {
                    cifras_objetivo = preguntar_cifras_significativas();
                    max_iter = 10000; // Límite de seguridad
                }
                
                if(seleccion_raiz == raices_encontradas.size() + 1) // Calcular todas
                {
                    vector<pair<RangoRaiz, ResultadoIteracion>> resultados_multiples;
                    
                    for(size_t i = 0; i < raices_encontradas.size(); i++)
                    {
                        double x0 = raices_encontradas[i].inicio;
                        double x1 = raices_encontradas[i].fin;
                        bool exito = false;
                        
                        ResultadoIteracion resultado = metodo_secante(funcion_trabajo, x0, x1, max_iter, tol_error, cifras_objetivo, criterio, exito);
                        
                        if(exito)
                        {
                            resultados_multiples.push_back(make_pair(raices_encontradas[i], resultado));
                        }
                    }
                    
                    mostrar_resultados_multiples(resultados_multiples, criterio);
                }
                else // Calcular una específica
                {
                    int indice = seleccion_raiz - 1;
                    double x0 = raices_encontradas[indice].inicio;
                    double x1 = raices_encontradas[indice].fin;
                    bool exito = false;
                    
                    ResultadoIteracion resultado = metodo_secante(funcion_trabajo, x0, x1, max_iter, tol_error, cifras_objetivo, criterio, exito);
                    
                    if(exito)
                    {
                        mostrar_resultado(resultado, criterio);
                    }
                }
                
                system("pause");
                
                // 6. Preguntar si desea calcular con otro criterio
                continuar_criterios = preguntar_continuar("¿Desea calcular con otro criterio de paro? (S/N): ");
            }
            
            // Preguntar si desea calcular otra raíz
            continuar_raices = preguntar_continuar("¿Desea calcular otra raiz del rango? (S/N): ");
        }
    }
    
    return 0;
}

// Implementación de funciones existentes
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
    const map<string, int> terminaciones_insercion = 
    {
        {"log10(", 6}, {"asin(", 5}, {"acos(", 5}, {"atan(", 5}, {"sqrt(", 5}, 
        {"sin(", 4}, {"cos(", 4}, {"tan(", 4}, {"log(", 4}, {"exp(", 4}, 
        {"abs(", 4}, {"csc(", 4}, {"sec(", 4}, {"cot(", 4}, {"pi", 2}, {"e", 1}
    };
    const map<string, string> mapeo_insercion = 
    {
        {"s", "sin("},   {"c", "cos("},   {"t", "tan("},   {"l", "log("}, 
        {"L", "log10("}, {"r", "sqrt("},  {"d", "abs("},   {"i", "pi"},
        {"u", "e"}, 
        {"cs", "csc("},  {"sc", "sec("},  {"ct", "cot("},  {"as", "asin("},
        {"ac", "acos("}, {"at", "atan("}, {"ex", "exp("} 
    };

    while (true)
    {
        mostrar_funcion(funcion);
        tecla = _getch(); 
        if (tecla == 13)
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
                        return funcion;
                    } 
                }
                else
                {
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
        else if (tecla == 8)
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
                    tecla == '/' || tecla == '^' || tecla == 'x' || tecla == ')' || tecla == '(') 
            {
                funcion += tecla;
            }
            else if (tecla == 'p' || tecla == 'P') 
            {
                funcion += '^';
            }
        }
    }
}

std::string validar_sintaxis_exprtk(const std::string& funcion_str)
{
    double x_valor = 0.0;
    symbol_table_t symbol_table;
    symbol_table.add_variable("x", x_valor); 
    symbol_table.add_constants();
    expression_t expression;
    expression.register_symbol_table(symbol_table);
    parser_t parser;
    
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
    return "";
}

double evaluar_funcion(const string& funcion_str, double x_val)
{
    symbol_table_t symbol_table;
    symbol_table.add_variable("x", x_val);
    symbol_table.add_constants();
    
    expression_t expression;
    expression.register_symbol_table(symbol_table);
    
    parser_t parser;
    
    if(parser.compile(funcion_str, expression))
    {
        return expression.value();
    }
    
    return numeric_limits<double>::quiet_NaN();
}

int preguntar_tipo_busqueda()
{
    int selector;
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "         TIPO DE BUSQUEDA                 " << endl;
        cout << "==========================================" << endl;
        cout << "1. Buscar una raiz (donde f(x) = 0)" << endl;
        cout << "2. Acercarse a un valor especifico" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Seleccion: ";
        cin >> selector;
        
        if(AntiCaracter(selector)) continue;
        if(RangoEntrada(selector, 1, 2)) continue;
        break;
    }
    return selector;
}

double preguntar_valor_objetivo()
{
    double valor;
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "         VALOR OBJETIVO                   " << endl;
        cout << "==========================================" << endl;
        cout << "Ingrese el valor al que desea acercarse: ";
        cin >> valor;
        
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida. Intente de nuevo..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        break;
    }
    return valor;
}

string construir_funcion_ajustada(const string& funcion_original, double objetivo)
{
    string funcion_ajustada = "(" + funcion_original + ")";
    
    if(objetivo >= 0)
    {
        funcion_ajustada += "-" + to_string(objetivo);
    }
    else
    {
        funcion_ajustada += "+" + to_string(-objetivo);
    }
    
    return funcion_ajustada;
}

void preguntar_rango(double& x_min, double& x_max)
{
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "         RANGO DE BUSQUEDA                " << endl;
        cout << "==========================================" << endl;
        cout << "Ingrese el primer limite del rango: ";
        cin >> x_min;
        
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida. Intente de nuevo..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        
        cout << "Ingrese el segundo limite del rango: ";
        cin >> x_max;
        
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida. Intente de nuevo..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        
        if(x_min == x_max)
        {
            cout << "Los limites no pueden ser iguales. Intente de nuevo..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        
        if(x_min > x_max)
        {
            double temp = x_min;
            x_min = x_max;
            x_max = temp;
        }
        
        break;
    }
}

vector<RangoRaiz> buscar_cambios_signo(const string& funcion_str, double x_min, double x_max, double paso)
{
    vector<RangoRaiz> raices;
    
    double x_actual = x_min;
    double f_anterior = evaluar_funcion(funcion_str, x_actual);
    
    while(x_actual < x_max)
    {
        double x_siguiente = x_actual + paso;
        if(x_siguiente > x_max) x_siguiente = x_max;
        
        double f_actual = evaluar_funcion(funcion_str, x_siguiente);
        
        // Verificar si hay cambio de signo
        if(!isnan(f_anterior) && !isnan(f_actual))
        {
            if((f_anterior * f_actual) < 0 || f_anterior == 0 || f_actual == 0)
            {
                RangoRaiz rango;
                rango.inicio = x_actual;
                rango.fin = x_siguiente;
                rango.valor_inicio = f_anterior;
                rango.valor_fin = f_actual;
                raices.push_back(rango);
            }
        }
        
        x_actual = x_siguiente;
        f_anterior = f_actual;
    }
    
    return raices;
}

int menu_seleccion_raices(const vector<RangoRaiz>& raices)
{
    int selector;
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "       RAICES ENCONTRADAS                 " << endl;
        cout << "==========================================" << endl;
        cout << "Se encontraron " << raices.size() << " cambio(s) de signo:" << endl << endl;
        
        for(size_t i = 0; i < raices.size(); i++)
        {
            cout << (i+1) << ". Entre x = " << fixed << setprecision(6) << raices[i].inicio 
                 << " y x = " << raices[i].fin << endl;
            cout << "   f(" << raices[i].inicio << ") = " << raices[i].valor_inicio << endl;
            cout << "   f(" << raices[i].fin << ") = " << raices[i].valor_fin << endl << endl;
        }
        
        cout << (raices.size() + 1) << ". Calcular TODAS las raices" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Seleccion: ";
        cin >> selector;
        
        if(AntiCaracter(selector)) continue;
        if(RangoEntrada(selector, 0, raices.size() + 1)) continue;
        break;
    }
    return selector;
}

int menu_criterio_paro()
{
    int selector;
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "         CRITERIO DE PARO                 " << endl;
        cout << "==========================================" << endl;
        cout << "1. Por numero de iteraciones" << endl;
        cout << "2. Por error relativo" << endl;
        cout << "3. Por cifras significativas" << endl;
        cout << "4. Volver a seleccion de raices" << endl;
        cout << "------------------------------------------" << endl;
        cout << "Seleccion: ";
        cin >> selector;
        
        if(AntiCaracter(selector)) continue;
        if(RangoEntrada(selector, 1, 4)) continue;
        break;
    }
    return selector;
}

int preguntar_num_iteraciones()
{
    int num_iter;
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "       NUMERO DE ITERACIONES              " << endl;
        cout << "==========================================" << endl;
        cout << "Ingrese el numero de iteraciones (1-10000): ";
        cin >> num_iter;
        
        if(AntiCaracter(num_iter)) continue;
        if(RangoEntrada(num_iter, 1, 10000)) continue;
        break;
    }
    return num_iter;
}

double preguntar_error_relativo()
{
    double error;
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "         ERROR RELATIVO OBJETIVO          " << endl;
        cout << "==========================================" << endl;
        cout << "Ingrese el error relativo deseado (0.0000000001 - 0.5): ";
        cin >> error;
        
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida. Intente de nuevo..." << endl;
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        
        if(error < 0.0000000001 || error > 0.5)
        {
            cout << "Error fuera de rango. Debe estar entre 0.0000000001 y 0.5" << endl;
            this_thread::sleep_for(chrono::seconds(2));
            continue;
        }
        
        break;
    }
    return error;
}

int preguntar_cifras_significativas()
{
    int cifras;
    while(true)
    {
        LimpiarPantalla();
        cout << "==========================================" << endl;
        cout << "       CIFRAS SIGNIFICATIVAS              " << endl;
        cout << "==========================================" << endl;
        cout << "Ingrese el numero de cifras significativas (1-15): ";
        cin >> cifras;
        
        if(AntiCaracter(cifras)) continue;
        if(RangoEntrada(cifras, 1, 15)) continue;
        break;
    }
    return cifras;
}

int calcular_cifras_significativas(double error_relativo)
{
    if (error_relativo <= 0) return 0;

    double abs_error = std::abs(error_relativo);
    int posicion_m = static_cast<int>(std::ceil(std::abs(std::log10(abs_error))));
    double error_normalizado = abs_error * std::pow(10, posicion_m);
    int primera_cifra_c = static_cast<int>(std::round(error_normalizado));
    int cifras_n;
    if (primera_cifra_c > 5) 
    {
        cifras_n = posicion_m - 2; 
    } 
    else 
    {
        cifras_n = posicion_m - 1;
    }
    return std::max(0, cifras_n);
}

ResultadoIteracion metodo_secante(const string& funcion_str, double x0, double x1, int max_iter, 
                                   double tol_error, int cifras_objetivo, int criterio, bool& exito)
{
ResultadoIteracion resultado;
resultado.iteracion = 0;
resultado.x_anterior = x0;
resultado.x_actual = x1;
exito = false;

double f_x0 = evaluar_funcion(funcion_str, x0);
double f_x1 = evaluar_funcion(funcion_str, x1);

for(int i = 1; i <= max_iter; i++)
{
    if(fabs(f_x1 - f_x0) < 1e-15)
    {
        cout << "\nAdvertencia: Division por cero inminente. Deteniendo..." << endl;
        break;
    }
    
    double x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);
    
    if(!isfinite(x2))
    {
        cout << "\nAdvertencia: Valor fuera de rango. Deteniendo..." << endl;
        break;
    }
    
    double f_x2 = evaluar_funcion(funcion_str, x2);
    
    if(isnan(f_x2))
    {
        cout << "\nAdvertencia: Funcion no evaluable en x = " << x2 << endl;
        break;
    }
    
    resultado.iteracion = i;
    resultado.x_anterior = x1;
    resultado.x_actual = x2;
    resultado.f_x_actual = f_x2;
    
    if(x2 != 0)
    {
        resultado.error_absoluto = fabs(x2 - x1);
        resultado.error_relativo = fabs((x2 - x1) / x2);
        resultado.cifras_significativas = calcular_cifras_significativas(resultado.error_relativo);
    }
    else
    {
        resultado.error_absoluto = fabs(x2 - x1);
        resultado.error_relativo = 0;
        resultado.cifras_significativas = 15;
    }
    
    // Verificar criterios de paro
    bool detener = false;
    
    if(criterio == 1) // Por iteraciones
    {
        if(i >= max_iter) detener = true;
    }
    else if(criterio == 2) // Por error relativo
    {
        if(resultado.error_relativo < tol_error) detener = true;
    }
    else if(criterio == 3) // Por cifras significativas
    {
        if(resultado.cifras_significativas >= cifras_objetivo) detener = true;
    }
    
    // Verificar convergencia (mismo resultado)
    if(fabs(x2 - x1) < 1e-15)
    {
        cout << "\nConvergencia alcanzada (sin cambios entre iteraciones)." << endl;
        detener = true;
    }
    
    if(detener)
    {
        exito = true;
        break;
    }
    
    // Actualizar para siguiente iteración
    x0 = x1;
    f_x0 = f_x1;
    x1 = x2;
    f_x1 = f_x2;
}

if(!exito && resultado.iteracion == max_iter)
{
    exito = true; // Se alcanzó el máximo de iteraciones
}

return resultado;
}

void mostrar_resultado(const ResultadoIteracion& resultado, int criterio)
{
LimpiarPantalla();
cout << "==========================================" << endl;
cout << "             RESULTADO                    " << endl;
cout << "==========================================" << endl;
cout << fixed << setprecision(10);
cout << "Iteraciones realizadas: " << resultado.iteracion << endl;
cout << "Valor aproximado de x: " << resultado.x_actual << endl;
cout << "f(x) = " << resultado.f_x_actual << endl;
cout << "Error absoluto: " << resultado.error_absoluto << endl;
cout << "Error relativo: " << resultado.error_relativo << endl;
cout << "Cifras significativas: " << resultado.cifras_significativas << endl;
cout << "==========================================" << endl;
}

void mostrar_resultados_multiples(const vector<pair<RangoRaiz, ResultadoIteracion>>& resultados, int criterio)
{
LimpiarPantalla();
cout << "==========================================" << endl;
cout << "         RESULTADOS MULTIPLES             " << endl;
cout << "==========================================" << endl;
cout << fixed << setprecision(10);

for(size_t i = 0; i < resultados.size(); i++)
{
    cout << "\nRaiz " << (i+1) << " (rango inicial: ["
         << resultados[i].first.inicio << ", " << resultados[i].first.fin << "]):" << endl;
    cout << "  Iteraciones: " << resultados[i].second.iteracion << endl;
    cout << "  x = " << resultados[i].second.x_actual << endl;
    cout << "  f(x) = " << resultados[i].second.f_x_actual << endl;
    cout << "  Error relativo: " << resultados[i].second.error_relativo << endl;
    cout << "  Cifras significativas: " << resultados[i].second.cifras_significativas << endl;
}
cout << "\n==========================================" << endl;
}

bool preguntar_continuar(const string& mensaje)
{
LimpiarPantalla();
cout << mensaje;
char respuesta = _getch();
return (respuesta == 'S' || respuesta == 's');
}