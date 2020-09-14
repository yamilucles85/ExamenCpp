#include <iostream>
#include <concepts>
#include <string>

template<typename T>
concept concepto = requires(T variable)
{
    { std::is_class<T>::value };
    { variable.leer() }              -> std::convertible_to<decltype(variable)>;
    { variable.validar(variable) }   -> std::convertible_to<bool>;
    { variable.escribir(variable) }  -> std::convertible_to<void>;
};

struct EstructuraValida
{
    int32_t numero;

    EstructuraValida leer(){
        EstructuraValida estructura_valida;
        int32_t numero;
        std::cin >>  numero;
        estructura_valida.numero = numero;
        return  estructura_valida;
    }

    bool validar(EstructuraValida estructura_valida){
        return estructura_valida.numero > 10 ? true : false;
    }

    void escribir(EstructuraValida estructura_valida){
        std::cout << estructura_valida.numero << std::endl;
    }
};

struct EstructuraInvalida
{
    std::string cadena;

    EstructuraInvalida leer(){
        EstructuraInvalida estructura_invalida;
        std::string cadena;
        std::getline(std::cin, cadena);
        estructura_invalida.cadena = cadena;
        return  estructura_invalida;
    }

    bool validar(EstructuraInvalida estructura_invalida){
        return estructura_invalida.cadena.size() > 10 ? true : false;
    }

    // Esta estructura no tiene el método escribir
};

template<concepto T>
void metodo(const T variable) {};

int32_t main()
{
    using namespace std;

//    EstructuraValida estructura_valida;
//    EstructuraValida estructura_valida_1 = estructura_valida.leer();
//    cout << boolalpha << estructura_valida_1.validar(estructura_valida_1) << endl;
//    estructura_valida_1.escribir(estructura_valida_1);

    EstructuraValida estructura_valida;
    EstructuraInvalida estructura_invalida;
    int32_t numero = 8;
    string cadena = "Cadena";

    metodo(estructura_valida);
//    metodo(estructura_invalida); // Estructura invalida, lanza error en el compilador
//    metodo(numero); // El concepto no acepta enteros
//    metodo(cadena); // Tipo de dato incorrecto es una cadena no una estructura

    return EXIT_SUCCESS;
}

// g++ Ejercicio3.cpp -o Ejercicio3 -std=gnu++20 -fconcepts