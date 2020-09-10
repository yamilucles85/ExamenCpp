#include <iostream>

int32_t main()
{
    int (*(&(*(*var)())(const int&))[10])[10];
    return EXIT_SUCCESS;
}

//int (*(&(*(*var)())(const int&))[10])[10]
//
//using puntero_a_array_de_10_elementos            = int (*)[10];  // int a[10][10];
//using referencia_a_puntero_array_de_10_elementos = puntero_a_array_de_10_elementos&  // int (*&)[10];
//using referencia_a_array_de_10_elementos_a_un_punteros_array_10_elementos = puntero_a_array_de_10_elementos(&)[10]
//using funcion2 = referencia_a_array_de_10_elementos_a_un_punteros_array_10_elementos(*)(const int&);
//using funcion1 = funcion2(*)();
//funcion1 var = var_funcion;
//
//funcion2 var_funcion()
//{
//
//}