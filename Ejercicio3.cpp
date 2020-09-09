/*
    Ejercicio #3
*/
#include <iterator>
#include <iostream>
#include <iso646.h>

template<typename T>
concept Interfaz = requires(T obj)
{
    { obj.begin() } -> Leer;
};




struct variable
{
    variable leer();
    bool validar(variable);
    void escribir(variable);
};

void Method( Interfaz variable)
{

}

variable leer(){
    return variable;
}