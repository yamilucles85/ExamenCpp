#ifndef __ENTIDADES_PERSONA_HPP__
#define __ENTIDADES_PERSONA_HPP__

#include <iostream>

namespace Comunidad {
    namespace Entidades {
        struct Persona {
            int32_t Id;
            std::array<char, 30> Nombre;
            std::array<char, 30> Apellido;
            std::array<char, 30> Telefono;
        };
    }
}

#endif // !__ENTIDADES_PERSONA_HPP__