#include <iostream>
#include <iomanip>
#include "Entidades/Persona.hpp"
#include "Repositorios/Persona.hpp"

using namespace Comunidad;
using namespace Entidades;
using namespace Repositorios;

int32_t main()
{
    using ctraits = std::char_traits<char>;
    Repositorio<Persona> repositorio;

    Persona persona;
    ctraits::copy(persona.Nombre.data(), "Israel", 7);
    ctraits::copy(persona.Apellido.data(), "Teneda", 7);
    ctraits::copy(persona.Telefono.data(), "0995691344", 11);

    repositorio.push_back(persona);

    for (const Persona &p : repositorio)
    {
        std::cout << "Nombre:   " << p.Nombre.data()   << "\n";
        std::cout << "Apellido: " << p.Apellido.data() << "\n";
        std::cout << "Telefono: " << p.Telefono.data() << "\n";
        std::cout << "------------------------------------\n\n";
    }

    return EXIT_SUCCESS;
}

