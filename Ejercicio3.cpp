#include <iostream>
#include <concepts>

template<typename T>
concept concepto = std::is_class<T>::value;
//        requires(T estructura)
//{
//    std:is_struct<estructura>::value;
////    estructura.leer() -> estructura;
////    validar(estructura) - > bool;
////    void escribir(estructura);
//};

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
    int32_t number;
};

template<concepto T>
void metodo(const T&& estructura) {};

int32_t main()
{
//    EstructuraValida estructura_valida;
//    EstructuraValida estructura_valida_1 = estructura_valida.leer();
//    std::cout << std::boolalpha << estructura_valida_1.validar(estructura_valida_1) << std::endl;
//    estructura_valida_1.escribir(estructura_valida_1);

    metodo(EstructuraValida);
    metodo(EstructuraInvalida);

    return EXIT_SUCCESS;
}