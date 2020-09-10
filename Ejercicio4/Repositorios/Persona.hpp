#ifndef __REPOSITORIOS_PERSONA_HPP__
#define __REPOSITORIOS_PERSONA_HPP__

#include <fstream>
#include <filesystem>
#include <cstddef>
#include "../Iteradores/Persona.hpp"

namespace Comunidad {
    namespace Repositorios {

        template<typename T>
        class Repositorio;

        template<>
        class Repositorio<Entidades::Persona> {
        public:

            using value_type = Entidades::Persona;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using reference = Entidades::Persona &;
            using const_reference = const Entidades::Persona &;
            using pointer = Entidades::Persona *;
            using const_pointer = const Entidades::Persona *;
            using iterator = Iteradores::Iterador<Entidades::Persona>;
            using const_iterator = const Iteradores::Iterador<Entidades::Persona>;

            Repositorio() {
                std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary;

                if (!std::filesystem::exists("PersonaRepositorio.bin"))
                    mode |= std::ios::trunc;

                m_repositorio.open("PersonaRepositorio.bin", mode);
            }


            bool push_back(const_reference cliente) noexcept {
                m_repositorio.seekp(0, std::ios::end);

                // Escribiendo datos del repositorio en binario.
                m_repositorio.write(reinterpret_cast<const char *>(&cliente), sizeof(value_type));

                // Verificar que no haya errores al guardar.
                if (m_repositorio.good()) {
                    m_repositorio.flush();
                    return true;
                }

                // Si hubo error, poner Repositorio en un estado "sin error"
                m_repositorio.clear(std::ios::goodbit);

                // Retornar False si hubo error.
                return false;
            }

            iterator begin() noexcept {
                return {m_repositorio, 0};
            }

            iterator end() noexcept {
                return {m_repositorio, size()};
            }

            const_iterator cbegin() const noexcept {
                return {m_repositorio, 0};
            }

            const_iterator cend() const noexcept {
                return {m_repositorio, size()};
            }

            size_t size() const noexcept {
                return std::filesystem::file_size("PersonaRepositorio.bin") / sizeof(value_type);
            }

        private:
            mutable std::fstream m_repositorio;
        };
    }
}

#endif // !__REPOSITORIOS_PERSONA_HPP__