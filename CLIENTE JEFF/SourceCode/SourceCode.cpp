#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <cstring>

//Macros
#ifdef _WIN32
#define CLEAR "cls"
#elif defined(unix)||defined(_unix_)||defined(__unix)||defined(_APPLE_)||defined(_MACH_)
#define CLEAR "clear"
#else
#error "S.O no soportado para la limpieza de pantalla"
#endif

#ifdef _WIN32
#define PAUSE "pause"
#elif defined(unix)||defined(_unix_)||defined(__unix)||defined(_APPLE_)||defined(_MACH_)
#define PAUSE "read -p 'ENTER para continuar...' var"
#else
#error "S.O no soportado para la limpieza de pantalla"
#endif
//Fin de macros

using namespace std;

//Nodo de la lista
struct NodoLista{
  int codigo;
  string nombre;
  double existencia;
  NodoLista *siguiente;
};

//Nodo de la cola
struct NodoCola{
  int codigoProducto;
  string nombrePiloto;
  int cantidad;
  int operacion;
  NodoCola *siguiente;
};

//Prototipos de funciones
void insertarLista(NodoLista *&,int ,string ,double );
void insertarCola(NodoCola *&, NodoCola *&, int, string, int, int);
bool cola_vacia(NodoCola *);
void menu(NodoLista *&,NodoCola *&,NodoCola *&);
void cargaDeProductosLista(NodoLista *&);
void realizarOperacionCamiones(NodoLista *&, NodoCola *&,NodoCola *&);
void buscarProductoLista(NodoLista *&, int);



int main(){
  //Creación de la lista
  NodoLista *lista = NULL;

  //Creación de la cola
  NodoCola *frenteCola = NULL;
  NodoCola *finCola = NULL;

  menu(lista, frenteCola, finCola);

  return 0;
}

void menu(NodoLista *& lista,NodoCola *& frenteCola, NodoCola *& finCola){
  int answer = 7;
  int flag = 0;
  while(answer != 4){
    system(CLEAR);
    cout<<"\t\t[Menú de sistema de inventarios]"<<endl<<endl;
    cout<<"\t1. Cargar productos a la lista."<<endl;
    cout<<"\t2. Realizar operación de carga o descarga"<<endl;
    cout<<"\t3. Consultar elementos de la lista."<<endl;
    cout<<"\t4. Salir."<<endl;
    cout<<"\tElección: ";
    cin>>answer;
    while(answer<1 || answer>4){
      cout<<"[Error] ¡Respuesta no válida!"<<endl;
      system(PAUSE);
      system(CLEAR);
      cout<<"\t\t[Menú de sistema de inventarios]"<<endl<<endl;
      cout<<"\t1. Cargar productos a la lista."<<endl;
      cout<<"\t2. Realizar operación de carga o descarga"<<endl;
      cout<<"\t3. Consultar elementos de la lista."<<endl;
      cout<<"\t4. Salir."<<endl;
      cout<<"\tElección: ";
      cin>>answer;
    }
    if(answer == 1){
      cargaDeProductosLista(lista);
      flag = 1;
    }
    else if(answer == 2){
      if(flag == 0){
        system(CLEAR);
        cout<<"Aún no ha cargado la lista de productos, proceda a realizar la carga"<<endl;
        system(PAUSE);
      }
      else{
        realizarOperacionCamiones(lista, frenteCola, finCola);
      }
    }
    else if(answer == 3){
      int tmpCod;
      cout<<"Por favor, digite el código del producto en la lista: ";
      cin>>tmpCod;
      buscarProductoLista(lista, tmpCod);
    }
    else{
      exit(0);
    }
  }

}

void realizarOperacionCamiones(NodoLista *& lista , NodoCola *& frenteCola, NodoCola *& finCola){
  //Creación de copia local de la lista a la cual apuntamos
  NodoLista *actual = new NodoLista();
  actual = lista;

  //Declaración de variables locales al método
  int cantOperaciones, codProducto, cantidad, operacion;
  string nomPiloto;


  //Sección de inserciones de productos
  cout<<"Por favor, indique la cantidad de operaciones a efectuar: ";
  cin>>cantOperaciones;

  for(int i=0; i < cantOperaciones;i++){
    FILE *sucursalUno = fopen("operaciones.txt","a");
    if (sucursalUno == NULL){
      cout<<"Error de apertura"<<endl;
    }
    system(CLEAR);
    cout<<"[Operación número "+to_string(i+1)+"]"<<endl;
    cout<<"Código Producto: ";
    cin>>codProducto;
    cout<<"Nombre del piloto: ";
    cin.ignore();
    getline(cin, nomPiloto);
    cout<<"Cantidad: ";
    cin>>cantidad;
    cout<<"Tipo de operación[1. carga - 2. Descarga]: ";
    cin>>operacion;
    while(operacion<1 || operacion>2){
      cout<<"¡Tipo de operación no válida!"<<endl;
      cout<<"Tipo de operación[1. carga - 2. Descarga]: ";
      cin>>operacion;
    }

    if(operacion == 1){
      while(actual != NULL){
        if(actual->codigo == codProducto){
          actual->existencia += cantidad;
          string str= "El piloto "+nomPiloto+" realizó la operación exitosa de Carga de "+actual->nombre+" por un total de "
          +to_string(actual->existencia)+" Kilos\n";
          int str_size = str.length();
          char Pinto[str_size];
          strcpy(Pinto, str.c_str());
          for(int i = 0;i<strlen(Pinto);i++){
            putc(Pinto[i],sucursalUno);
          }
          insertarCola(frenteCola,finCola,codProducto,nomPiloto,cantidad, operacion);
          fclose(sucursalUno);
          actual = actual->siguiente;
        }
        else{
          actual = actual->siguiente;
        }
      }
    }
    else{
      while(actual != NULL){
        if(actual->codigo == codProducto){
          actual->existencia -= cantidad;
          string str= "El piloto "+nomPiloto+" realizó la operación exitosa de descarga de "+actual->nombre+" por un total de "
          +to_string(actual->existencia)+" Kilos\n";
          int str_size = str.length();
          char Pinto[str_size];
          strcpy(Pinto, str.c_str());
          for(int i = 0;i<strlen(Pinto);i++){
            putc(Pinto[i],sucursalUno);
          }
          insertarCola(frenteCola, finCola, codProducto, nomPiloto, cantidad, operacion);
          fclose(sucursalUno);
          actual = actual->siguiente;
        }
        else{
          actual = actual->siguiente;
        }
      }
    }
  }
}

void buscarProductoLista(NodoLista *& lista, int tmpCod){
  NodoLista *actual = new NodoLista();
  actual = lista;
  if( actual != NULL ){
    int flag = 0;
    while(flag == 0){
      if(actual->codigo == tmpCod){
        system(CLEAR);
        cout<<"¡Producto encontrado!"<<endl<<endl;
        cout<<"Codigo: "+to_string(actual->codigo)<<endl;
        cout<<"Nombre producto: "+actual->nombre<<endl;
        cout<<"Existencia: "+to_string(actual->existencia)<<endl;
        flag = 1;
      }
      else{
        actual = actual->siguiente;
        if(actual == NULL){
          cout<<"¡Código de producto no encontrado!";
          flag = 1;
        }
      }
    }
  }
  else{
    cout<<"no se puede ejecutar la búsqueda [Lista Vacía]"<<endl;
  }
  cout<<endl;
  cin.ignore();
  system(PAUSE);
}

void cargaDeProductosLista(NodoLista *& lista){
  //Declaración de variables locales al método
  int cantidadProductos,codProducto;
  string nomProducto;
  double existencia;

  //Sección de inserciones de productos
  system(CLEAR);
  cout<<"Por favor, indique la cantidad de productos a insertar: ";
  cin>>cantidadProductos;

  for(int i=0;i<cantidadProductos;i++){
    system(CLEAR);
    cin.ignore();
    cout<<"[Producto número "+to_string(i+1)+"]"<<endl;
    cout<<"Código Producto: ";
    cin>>codProducto;
    cout<<"Nombre producto: ";
    cin>>nomProducto;
    cout<<"Existencia: ";
    cin>>existencia;
    insertarLista(lista,codProducto,nomProducto,existencia);
  }
  cout<<endl<<"<[Carga exitosa de productos a la lista]>"<<endl;
  cin.ignore();
  system(PAUSE);
}

bool cola_vacia(NodoCola *frente){
  return (frente == NULL)? true : false;
}

void insertarCola(NodoCola *& frente, NodoCola *& fin, int a, string b, int c,int d){
  NodoCola *nuevo_nodo = new NodoCola();

  nuevo_nodo->codigoProducto = a;
  nuevo_nodo->nombrePiloto = b;
  nuevo_nodo->cantidad = c;
  nuevo_nodo->operacion = d;
  nuevo_nodo->siguiente = NULL;
  if(cola_vacia(frente)){
    frente = nuevo_nodo;
  }
  else{
    fin->siguiente = nuevo_nodo;
  }
  cout<<"Elemento codigoProducto "<<a<<" insertado correctamente"<<endl;
}

void insertarLista(NodoLista *&lista,int n,string m,double o){
  NodoLista *nuevo_nodo = new NodoLista();
  nuevo_nodo->codigo = n;
  nuevo_nodo->nombre = m;
  nuevo_nodo->existencia = o;

  NodoLista *aux1 = lista;
  NodoLista *aux2;

  while((aux1 != NULL) && (aux1->codigo < n)){
    aux2 = aux1;
    aux1 = aux1->siguiente;
  }
  if(lista == aux1){
    lista = nuevo_nodo;
  }
  else{
    aux2->siguiente = nuevo_nodo;
  }
  nuevo_nodo->siguiente = aux1;
}
