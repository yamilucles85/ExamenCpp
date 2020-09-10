#ifndef __ITERADORES_PERSONA_HPP__
#define __ITERADORES_PERSONA_HPP__

#include <fstream>
#include "../Entidades/Persona.hpp"
#include <iterator>

namespace Comunidad {
    namespace Iteradores {
        template<typename T>
        class Iterador;

        template<>
        class Iterador<Entidades::Persona> {
        public:
            using difference_type = ptrdiff_t;
            using value_type = Entidades::Persona;
            using pointer = Entidades::Persona *;
            using const_pointer = const Entidades::Persona *;
            using reference = Entidades::Persona &;
            using const_reference = const Entidades::Persona &;
            using iterator_category = std::input_iterator_tag;

            Iterador(std::fstream &repositorio, const size_t indice) noexcept
                    : m_indice{indice}, m_repositorio{&repositorio} {
            }

            Iterador(Iterador &&it)
                    : m_indice{std::exchange(it.m_indice, 0)}, m_repositorio{std::exchange(it.m_repositorio, nullptr)} {
            }

            Iterador &operator=(Iterador &&it) {
                this->m_indice = std::exchange(it.m_indice, 0);
                this->m_repositorio = std::exchange(it.m_repositorio, nullptr);
                return *this;
            }

            Iterador(const Iterador &) = delete;

            Iterador &operator=(const Iterador &) = delete;

            ~Iterador() = default;

            reference operator*() noexcept {
                m_repositorio->seekg(m_indice * sizeof(value_type), std::ios::beg);
                m_repositorio->read(reinterpret_cast<char *>(&m_persona), sizeof(value_type));
                return m_persona;
            }

            pointer operator->() noexcept {
                return &this->operator*();
            }

            const_reference operator*() const noexcept {
                m_repositorio->seekg(m_indice * sizeof(value_type), std::ios::beg);
                m_repositorio->read(reinterpret_cast<char *>(&m_persona), sizeof(value_type));
                return m_persona;
            }

            const_pointer operator->() const noexcept {
                return &this->operator*();
            }

            Iterador &operator++() noexcept {
                ++m_indice;
                return *this;
            }

            const Iterador &operator++() const noexcept {
                ++m_indice;
                return *this;
            }

            Iterador operator++(int) noexcept {
                return {*m_repositorio, m_indice++};
            }

            const Iterador operator++(int) const noexcept {
                return {*m_repositorio, m_indice++};
            }

            Iterador &operator--() noexcept {
                --m_indice;
                return *this;
            }

            const Iterador &operator--() const noexcept {
                --m_indice;
                return *this;
            }

            Iterador operator--(int) noexcept {
                return {*m_repositorio, m_indice--};
            }

            const Iterador operator--(int) const noexcept {
                return {*m_repositorio, m_indice--};
            }

            value_type operator[](const int32_t index) const noexcept {
                m_repositorio->seekg(index * sizeof(value_type), std::ios::beg);
                m_repositorio->read(reinterpret_cast<char *>(&m_persona), sizeof(value_type));
                return m_persona;
            }

            bool operator!=(const Iterador &it) const noexcept {
                return this->m_indice != it.m_indice;
            }

            friend Iterador operator+(const Iterador &it, const int32_t index);

            friend Iterador operator+(const int32_t index, const Iterador &it);

            friend Iterador operator-(const Iterador &it, const int32_t index);

            friend Iterador operator-(const int32_t index, const Iterador &it);

        private:
            mutable size_t m_indice;
            std::fstream *m_repositorio;
            mutable value_type m_persona;
        };

        Iterador<Entidades::Persona> operator+(const Iterador<Entidades::Persona> &it, const int32_t index) {
            return {*it.m_repositorio, it.m_indice + index};
        }

        Iterador<Entidades::Persona> operator+(const int32_t index, const Iterador<Entidades::Persona> &it) {
            return {*it.m_repositorio, it.m_indice + index};
        }

        Iterador<Entidades::Persona> operator-(const Iterador<Entidades::Persona> &it, const int32_t index) {
            return {*it.m_repositorio, it.m_indice - index};
        }

        Iterador<Entidades::Persona> operator-(const int32_t index, const Iterador<Entidades::Persona> &it) {
            return {*it.m_repositorio, it.m_indice - index};
        }
    }
}

#endif // !__ITERADORES_PERSONA_HPP__