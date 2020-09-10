#include <iostream>

template<typename T>
concept concepto = requires(T tipo)
{

};

struct nombre_objeto
{
    nombre_objeto leer();
    bool escribir(nombre_objeto);
    void validar(nombre_objeto);
};

void metodo( <concept> variable)
{

}

int32_t main()
{
    return EXIT_SUCCESS;
}