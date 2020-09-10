cmake_minimum_required(VERSION 3.16)

project(Comunidad)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Comunidad
        main.cpp
        Entidades/Persona.hpp
        Iteradores/Persona.hpp
        Repositorios/Persona.hpp)