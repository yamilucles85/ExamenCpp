#include "cxx/cxx.hpp"
#include <iostream>
#include <concepts>
#include <string>
#include <string_view>
#include <utility>
#include <iterator>
#include <vector>
#include <array>
#include <fstream>
#include <iomanip>
#include <filesystem>

const static int BUF_SIZE = 4096;

// Definición de Estructura Persona
struct Persona {
    int32_t Id;
    std::array<char, 30> Nombre;
    std::array<char, 30> Apellido;
    std::array<char, 30> Telefono;
    bool EstaBorrado;
};

// Definición de métodos: Leer, Mostrar, Buscar, Insertar, Actualizar, Borrar

enum class MenuOpciones : int32_t {
    Registrar = 1,
    Mostrar,
    Buscar,
    Actualizar,
    Borrar,
    Salir
};

std::istream &operator>>(std::istream &in, Persona &persona);

void PersonaMostrar(std::fstream &base_de_datos);

void PersonaBuscar(std::fstream &base_de_datos, int32_t &id);

void PersonaInsertar(std::fstream &base_de_datos);

bool PersonaActualizar(std::fstream &base_de_datos, int32_t &id);

bool PersonaBorrar(std::fstream &base_de_datos, int32_t &id);

MenuOpciones LeerOpcion();

static const int32_t BaseDeDatosId = std::ios::xalloc();

int32_t main() {
    using namespace std;

    // Se va a escribir o leer en el archivo.
    std::fstream base_de_datos;
    int32_t id;

    std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary;

    if (!std::filesystem::exists("Comunidad.bin"))
        mode |= std::ios::trunc;

    base_de_datos.open("Comunidad.bin", mode);

    std::cin.pword(BaseDeDatosId) = &base_de_datos;

    MenuOpciones opcion;

    while ((opcion = LeerOpcion()) != MenuOpciones::Salir) {
        switch (opcion) {
            case MenuOpciones::Registrar: {
                PersonaInsertar(base_de_datos);
                break;
            }

            case MenuOpciones::Mostrar: {
                PersonaMostrar(base_de_datos);
                break;
            }

            case MenuOpciones::Buscar: {
                std::cout << "Ingrese el Id de la persona: ";
                std::cin >> id;
                std::cin.ignore(1, '\n');
                PersonaBuscar(base_de_datos, id);
                break;
            }

            case MenuOpciones::Actualizar: {
                std::cout << "Ingrese el Id de la persona: ";
                std::cin >> id;
                std::cin.ignore(1, '\n');
                PersonaActualizar(base_de_datos, id);
                break;
            }

            case MenuOpciones::Borrar: {
                std::cout << "Ingrese el Id de la persona: ";
                std::cin >> id;
                std::cin.ignore(1, '\n');
                PersonaBorrar(base_de_datos, id);
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

MenuOpciones LeerOpcion() {
    std::cout << "\n1) - Registrar\n"
                   "2) - Mostrar\n"
                   "3) - Buscar\n"
                   "4) - Actualizar\n"
                   "5) - Borrar\n"
                   "6) - Salir\n"
                   "-> ";
    cxx::read_result<int32_t> opcion;
    do {
        opcion = cxx::read<int32_t>();
        if (opcion.has_error) {
            std::clog << "La opción es requerida.\n";
            std::cin.clear(std::ios::goodbit);
        } else if (opcion.value < static_cast<std::underlying_type_t <MenuOpciones>>(MenuOpciones::Registrar)
                   || opcion.value > static_cast<std::underlying_type_t <MenuOpciones>>(MenuOpciones::Salir)) {
            opcion.has_error = true;
            std::clog << "La opción no es válida\n";
        }
    } while (opcion.has_error);

    return static_cast<MenuOpciones>(opcion.value);
}

int32_t GenerarId(std::istream &in) {
    std::fstream *const base_de_datos = static_cast<std::fstream *>(in.pword(BaseDeDatosId));
    base_de_datos->seekp(0, std::ios::end);

    const size_t total_de_bytes = base_de_datos->tellp();
    const size_t numero_de_registros = total_de_bytes / sizeof(Persona);

    if (numero_de_registros == 0) {
        return 1;
    } else {
        Persona persona;
        base_de_datos->seekp(-sizeof(Persona), std::ios::end);
        base_de_datos->read(reinterpret_cast<char *>(&persona), sizeof(Persona));
        return persona.Id + 1;
    }
}

// Ingreso de información de la Persona
std::istream &operator>>(std::istream &in, Persona &persona) {
    persona.Id = GenerarId(in);

    std::cout << "Ingrese el nombre de la persona: \n-> ";
    do {
        in.getline(persona.Nombre.data(), persona.Nombre.size());

        if (in.fail() || in.eof())
            in.ignore(std::numeric_limits<int32_t>::max(), '\n');

        if (std::char_traits<char>::length(persona.Nombre.data()) == 0) {
            std::clog << "El nombre es obligatorio. \n-> ";
        }

        in.clear(std::ios::goodbit);
    } while (std::char_traits<char>::length(persona.Nombre.data()) == 0);

    std::cout << "Ingrese el apellido de la persona: \n-> ";
    do {
        in.getline(persona.Apellido.data(), persona.Apellido.size());

        if (in.fail() || in.eof())
            in.ignore(std::numeric_limits<int32_t>::max(), '\n');

        if (std::char_traits<char>::length(persona.Apellido.data()) == 0) {
            std::clog << "El apellido es obligatorio. \n-> ";
        }

        in.clear(std::ios::goodbit);
    } while (std::char_traits<char>::length(persona.Apellido.data()) == 0);

    std::cout << "Ingrese el teléfono de la persona: \n-> ";
    do {
        in.getline(persona.Telefono.data(), persona.Telefono.size());

        if (in.fail() || in.eof())
            in.ignore(std::numeric_limits<int32_t>::max(), '\n');

        if (std::char_traits<char>::length(persona.Telefono.data()) == 0) {
            std::clog << "El teléfono es obligatorio. \n-> ";
        }

        in.clear(std::ios::goodbit);
    } while (std::char_traits<char>::length(persona.Telefono.data()) == 0);

    return in;
}

// Definición del método Insertar.
void PersonaInsertar(std::fstream &base_de_datos) {
    Persona persona;
    std::cin >> persona;

    base_de_datos.seekp(0, std::ios::end);
    base_de_datos.write(reinterpret_cast<const char *>(&persona), sizeof(Persona));
}

void PersonaMostrar(std::fstream &base_de_datos) {
    base_de_datos.seekg(0, std::ios::end);
    const size_t total_de_bytes = base_de_datos.tellg();
    size_t numero_de_registros = total_de_bytes / sizeof(Persona);

    if (numero_de_registros == 0) {
        std::cout << "No hay datos registrados.\n";
        return;
    }

    base_de_datos.seekg(0, std::ios::beg);
    base_de_datos.clear(std::ios::goodbit);
    Persona persona;

    // Id        -> ancho de 10
    // Nombre    -> ancho de 30
    // Apellido  -> ancho de 30
    // Dirección -> ancho de 30
    // ----------------------------------------------------------------------------
    // |   Id    |      Nombre      |         Apellido          |    Teléfono    |
    // ----------------------------------------------------------------------------
    //                              Contenido
    // ----------------------------------------------------------------------------

    constexpr int32_t AnchoId = 10;
    constexpr int32_t AnchoNombre = 30;
    constexpr int32_t AnchoApellido = 30;
    constexpr int32_t AnchoTelefono = 30;
    constexpr int32_t PaddingIzquierdo = 1;
    constexpr int32_t PaddingDerecho = 1;
    constexpr int32_t NumeroDeSeparadores = 5;

    std::cout << std::left;

    std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                             (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

    std::cout
            << '|' << std::setw(AnchoId + PaddingIzquierdo + PaddingDerecho) << " Id"
            << '|' << std::setw(AnchoNombre + PaddingIzquierdo + PaddingDerecho) << " Nombre"
            << '|' << std::setw(AnchoApellido + PaddingIzquierdo + PaddingDerecho) << " Apellido"
            << '|' << std::setw(AnchoTelefono + PaddingIzquierdo + PaddingDerecho) << " Telefono"
            << '|' << std::endl;

    std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                             (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

    while (numero_de_registros--) {
        base_de_datos.read(reinterpret_cast<char *>(&persona), sizeof(Persona));

        std::cout
                << "| " << std::setw(AnchoId + PaddingDerecho) << persona.Id
                << "| " << std::setw(AnchoNombre + PaddingDerecho) << persona.Nombre.data()
                << "| " << std::setw(AnchoApellido + PaddingDerecho) << persona.Apellido.data()
                << "| " << std::setw(AnchoTelefono + PaddingDerecho) << persona.Telefono.data()
                << "| " << std::endl;
    }

    std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                             (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

    std::cout << std::right;
    base_de_datos.clear(std::ios::goodbit);
}

void PersonaBuscar(std::fstream &base_de_datos, int32_t &id) {
    base_de_datos.seekg(0, std::ios::end);
    const size_t total_de_bytes = base_de_datos.tellg();
    size_t numero_de_registros = total_de_bytes / sizeof(Persona);

    if (numero_de_registros == 0) {
        std::cout << "No hay datos registrados.\n";
        return;
    }

    base_de_datos.seekg(0, std::ios::beg);
    base_de_datos.clear(std::ios::goodbit);

    Persona persona;

    constexpr int32_t AnchoId = 10;
    constexpr int32_t AnchoNombre = 30;
    constexpr int32_t AnchoApellido = 30;
    constexpr int32_t AnchoTelefono = 30;
    constexpr int32_t PaddingIzquierdo = 1;
    constexpr int32_t PaddingDerecho = 1;
    constexpr int32_t NumeroDeSeparadores = 5;

    std::cout << std::left;

    std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                             (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

    std::cout
            << '|' << std::setw(AnchoId + PaddingIzquierdo + PaddingDerecho) << " Id"
            << '|' << std::setw(AnchoNombre + PaddingIzquierdo + PaddingDerecho) << " Nombre"
            << '|' << std::setw(AnchoApellido + PaddingIzquierdo + PaddingDerecho) << " Apellido"
            << '|' << std::setw(AnchoTelefono + PaddingIzquierdo + PaddingDerecho) << " Telefono"
            << '|' << std::endl;

    std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                             (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

    while (numero_de_registros--) {
        base_de_datos.read(reinterpret_cast<char *>(&persona), sizeof(Persona));
        if (persona.Id == id) {

            std::cout
                    << "| " << std::setw(AnchoId + PaddingDerecho) << persona.Id
                    << "| " << std::setw(AnchoNombre + PaddingDerecho) << persona.Nombre.data()
                    << "| " << std::setw(AnchoApellido + PaddingDerecho) << persona.Apellido.data()
                    << "| " << std::setw(AnchoTelefono + PaddingDerecho) << persona.Telefono.data()
                    << "| " << std::endl;
            break;
        }
    }

    std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                             (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

    std::cout << std::right;
    base_de_datos.clear(std::ios::goodbit);
}

bool PersonaActualizar(std::fstream &base_de_datos, int32_t &id) {

    std::fstream archivo_temp;
    std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc;
    archivo_temp.open("temp.bin", mode);
    std::cin.pword(BaseDeDatosId) = &archivo_temp;

    base_de_datos.seekg(0, std::ios::end);
    const size_t total_de_bytes = base_de_datos.tellg();
    size_t numero_de_registros = total_de_bytes / sizeof(Persona);

    if (numero_de_registros == 0) {
        std::cout << "No hay datos registrados.\n";
        return false;
    }

    base_de_datos.seekg(0, std::ios::beg);
    base_de_datos.clear(std::ios::goodbit);

    Persona persona;
    Persona nueva_persona;
    size_t  capacidad_acumulada;

    constexpr int32_t AnchoId = 10;
    constexpr int32_t AnchoNombre = 30;
    constexpr int32_t AnchoApellido = 30;
    constexpr int32_t AnchoTelefono = 30;
    constexpr int32_t PaddingIzquierdo = 1;
    constexpr int32_t PaddingDerecho = 1;
    constexpr int32_t NumeroDeSeparadores = 5;


    while (numero_de_registros--) {

        base_de_datos.read(reinterpret_cast<char *>(&persona), sizeof(Persona));

        if (persona.Id == id) {

            std::cin >> nueva_persona;

            persona.Nombre   = nueva_persona.Nombre;
            persona.Apellido = nueva_persona.Apellido;
            persona.Telefono = nueva_persona.Telefono;

            std::cout << std::left;

            std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                                     (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

            std::cout
                    << '|' << std::setw(AnchoId + PaddingIzquierdo + PaddingDerecho) << " Id"
                    << '|' << std::setw(AnchoNombre + PaddingIzquierdo + PaddingDerecho) << " Nombre"
                    << '|' << std::setw(AnchoApellido + PaddingIzquierdo + PaddingDerecho) << " Apellido"
                    << '|' << std::setw(AnchoTelefono + PaddingIzquierdo + PaddingDerecho) << " Telefono"
                    << '|' << std::endl;

            std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                                     (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;

            std::cout
                    << "| " << std::setw(AnchoId + PaddingDerecho)       << persona.Id
                    << "| " << std::setw(AnchoNombre + PaddingDerecho)   << persona.Nombre.data()
                    << "| " << std::setw(AnchoApellido + PaddingDerecho) << persona.Apellido.data()
                    << "| " << std::setw(AnchoTelefono + PaddingDerecho) << persona.Telefono.data()
                    << "| " << std::endl;

            std::cout << std::string(AnchoId + AnchoNombre + AnchoApellido + AnchoTelefono +
                                     (PaddingIzquierdo * 4) + (PaddingDerecho * 4) + NumeroDeSeparadores, '-') << std::endl;
        }
        archivo_temp.seekp(0, std::ios::end);
        archivo_temp.write(reinterpret_cast<const char *>(&persona), sizeof(Persona));
    }

    std::ifstream in("temp.bin",
                     std::ios_base::in | std::ios_base::binary);
    std::ofstream out("Comunidad.bin",
                      std::ios_base::out | std::ios_base::binary);


    char buf[BUF_SIZE];

    do {
        in.read(&buf[0], BUF_SIZE);
        out.write(&buf[0], in.gcount());
    } while (in.gcount() > 0);


    in.close();
    out.close();

    std::cout << std::right;
    archivo_temp.clear(std::ios::goodbit);
    base_de_datos.clear(std::ios::goodbit);

    return true;
}

bool PersonaBorrar(std::fstream &base_de_datos, int32_t &id) {
    std::fstream archivo_temp;
    std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc;
    archivo_temp.open("temp.bin", mode);
    std::cin.pword(BaseDeDatosId) = &archivo_temp;


    base_de_datos.seekg(0, std::ios::end);
    const size_t total_de_bytes = base_de_datos.tellg();
    size_t numero_de_registros = total_de_bytes / sizeof(Persona);

    if (numero_de_registros == 0) {
        std::cout << "No hay datos registrados.\n";
        return false;
    }

    base_de_datos.seekg(0, std::ios::beg);
    base_de_datos.clear(std::ios::goodbit);

    Persona persona;

    while (numero_de_registros--) {
        base_de_datos.read(reinterpret_cast<char *>(&persona), sizeof(Persona));
        if (persona.Id == id) continue;
        archivo_temp.seekp(0, std::ios::end);
        archivo_temp.write(reinterpret_cast<const char *>(&persona), sizeof(Persona));
    }

    std::ifstream in("temp.bin",
                     std::ios_base::in | std::ios_base::binary);
    std::ofstream out("Comunidad.bin",
                      std::ios_base::out | std::ios_base::binary);

    char buf[BUF_SIZE];

    do {
        in.read(&buf[0], BUF_SIZE);
        out.write(&buf[0], in.gcount());
    } while (in.gcount() > 0);

    in.close();
    out.close();

    archivo_temp.clear(std::ios::goodbit);
    base_de_datos.clear(std::ios::goodbit);

    return true;
}

// g++ -g Ejercicio4.cpp -o Ejercicio4 -std=gnu++20 -fconcepts -fcoroutines -fpermissive -fms-extensions -fconcepts-diagnostics-depth=4
