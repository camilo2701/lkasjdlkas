#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define maxBodegas 5
#define maxChar 1000

//agregar free a los nodos auxiliares
// arreglar fallas al veirificar que esten todas las bodegas ocupadas o disponibles
struct nodoProveedores{

    struct proveedor *datosProveedor;
    struct nodoProveedores *sig, *ant;
};

struct nodoVentas{

    struct venta *datosVenta;
    struct nodoVentas *sig;
};

struct bodegas{

    int idBodega;
    float capacidad;
    float disponibilidad;
    char *direccion;
    struct nodoProductos *ABP;
};

struct nodoProductos{

    struct producto *datosProducto;
    struct nodoProductos *izq, *der;
};

struct producto{

    int idProducto;
    int precio;
    int cantidad;
    float peso;
    char *nombre;
};

struct nodoCompras{

    struct producto *detalleCompra;
    int cantPorProduct;
    struct nodoCompras *sig, *ant;
};

struct proveedor{

    int idProveedor;
    char *nombre;
    struct nodoCompras *listaCompras;
};

struct venta{

    int idVenta;
    int total;
    char *fecha;
    char *direccion;
    char *nombreCliente;
    struct nodoVendido *listaVendido;
};

struct nodoVendido{

    struct producto detalleProducto;
    struct nodoVendido *sig;
};


struct MercaditoLibre{

    struct nodoProveedores *listaProveedores;
    struct nodoVentas *listaVentas;
    struct bodegas *Bodega[maxBodegas];
};

//inicializarMercaditoLibre: Como bien dice su nombre, inicializa la estructura principal dandole memoria e inicializando las listas que contiene en NULL;
void inicializarMercaditoLibre(struct MercaditoLibre **ML){

    int i;
    (*ML) = (struct MercaditoLibre *) malloc(sizeof(struct MercaditoLibre));
    (*ML)->listaProveedores = NULL;
    (*ML)->listaVentas = NULL;
    for(i = 0; i < maxBodegas; i++) (*ML)->Bodega[i] = NULL;
}

// valida que la opcion ingresada por el usuario sea valida, de lo contrario se le pedira que la vuelva a inrgresar
void validarOpcion(int *opcion, int intervalo){

    while(true){

        if((*opcion) < 0 || (*opcion) > intervalo) puts("OPCION INVALIDA, INTENTE NUEVAMENTE O INGRESE 0 (CERO) PARA SALIR.\n");
        else return;
        printf("Opcion :");
        scanf("%i", opcion); getchar();}
}

// buscarProveedor: Recibe la lista de proveedores y un id a buscar, si lo encuentra retorna 0, en el caso de no encontrarlo, retorna 1;
struct proveedor *buscarProveedor(struct nodoProveedores *lista, int id){

    if(lista != NULL){
        struct nodoProveedores *rec = lista;
        do{
            if(rec->datosProveedor->idProveedor == id) return rec->datosProveedor;
            rec = rec->sig;
        }while(rec != lista);
    }return NULL;
}

// crearInfoProveedores: Crea un nodo para el proveedor con la informacion recibida por consola
struct proveedor *crearInfoProveedores(struct MercaditoLibre *ML){
    struct proveedor *nuevo = NULL;
    int id;
    char buffer[maxChar];
    do {
        printf(" Ingresa el id del proveedor :");
        scanf("%i", &id);

        if(id == 0){
            puts("----------------------------------------");
            return NULL;
        }
        else if (!buscarProveedor(ML->listaProveedores, id)){

            nuevo = (struct proveedor *) malloc(sizeof(struct proveedor));

            printf(" Ingresa el nombre del proveedor :"); getchar();
            scanf(" %[^\n]s", buffer);

            nuevo->nombre = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
            strcpy(nuevo->nombre, buffer);

            nuevo->listaCompras = NULL;
            nuevo->idProveedor = id;
            return nuevo;
        }
        else puts("\nERROR: El proveedor que has ingresado ya existe, ingresa un proveedor nuevo o 0 (cero) para salir.\n");
    }while(true);
}

struct nodoProveedores *crearNodoProveedores(struct proveedor *datos){

    struct nodoProveedores *nuevo = (struct nodoProveedores *) malloc(sizeof(struct nodoProveedores));

    nuevo->datosProveedor = datos;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    return nuevo;
}

void agregarProveedor(struct nodoProveedores **lista, struct nodoProveedores *nodo){

    if((*lista) == NULL){
        (*lista) = nodo;
        (*lista)->sig = (*lista);
        (*lista)->ant = (*lista);
    }else{
        nodo->sig = (*lista);
        nodo->ant = (*lista)->ant;
        (*lista)->ant->sig = nodo;
        (*lista)->ant = nodo;
    }
}

struct bodegas *buscarBodega(struct bodegas *arreglo[maxBodegas], int id){

    int i;
    for(i = 0; i < maxBodegas; i++)

        if(arreglo[i] != NULL)
            if(arreglo[i]->idBodega == id) return arreglo[i];
    return NULL;
}

int verificarBodegas(struct bodegas *arreglo[maxBodegas]){

    int i;
    for (i = 0 ; i < maxBodegas ; i++){
        if (arreglo[i] != NULL) return -1;
    }
    return 0;
}

int buscarPosicionBodega(struct bodegas *arreglo[maxBodegas], int id){

    int i;
    for(i = 0; i < maxBodegas; i++)
        if(arreglo[i] != NULL)
            if(arreglo[i]->idBodega == id) return i;
    return -1;
}

struct bodegas *crearInfoBodegas(struct MercaditoLibre *ML, int *bodegasOcupadas){

    struct bodegas *nuevo = (struct bodegas *)malloc(sizeof(struct bodegas));
    int id;
    char buffer[maxChar];
    do{
        printf(" Ingresa el id de la bodega :");
        scanf("%i", &id); getchar();
        if(id == 0){
            puts("----------------------------------------");
            return NULL;
        }
        else if(!buscarBodega(ML->Bodega, id)){

            float almacenaje;
            printf(" Ingresa la cantidad de peso maxima de almacenaje (en kgs) :");
            scanf("%f", &almacenaje);getchar();

            printf(" Ingresa la direccion :"); getchar();
            scanf(" %[^\n]s", buffer);

            nuevo->direccion = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
            strcpy(nuevo->direccion, buffer);
            nuevo->capacidad = almacenaje;
            nuevo->disponibilidad = almacenaje;
            nuevo->idBodega = id;
            nuevo->ABP = NULL;
            return nuevo;
        }
        else puts("\nERROR: La bodega que has ingresado ya existe, ingresa una nueva bodega o 0 (cero) para salir\n");
    }while(true);
}

void opcionAgregar(struct MercaditoLibre *ML, int *bodegasOcupadas){

    int opcion;
    puts("----------------------------------------");
    puts("Que deseas agregar? (elige una opcion).\n");
    puts("1. Agregar proveedor\n2. Agregar bodega\n0. SALIR\n");
    printf("Opcion :");

    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 2);

    if(opcion == 1){

        struct proveedor *nodoP = crearInfoProveedores(ML);
        if(nodoP){
            struct nodoProveedores *nuevo = crearNodoProveedores(nodoP);
            agregarProveedor(&(ML->listaProveedores), nuevo);
            puts("\nProveedor agregado con exito!");
            puts("----------------------------------------");
        }
    }
    else if(opcion == 2){

        if((*bodegasOcupadas) == 5){
            puts("\nERROR: No quedan bodegas disponibles\n");
            puts("----------------------------------------");
        }
        else{
            struct bodegas *nodoB = crearInfoBodegas(ML, bodegasOcupadas);
            if(nodoB){
                (*bodegasOcupadas)++;
                ML->Bodega[(*bodegasOcupadas) - 1] = nodoB;
                puts("\nBodega agregada con exito!");
                puts("----------------------------------------");
            }
        }
    }else{
        puts("----------------------------------------");
        return;
    }
}

void imprimirTodosProveedores(struct nodoProveedores *lista){

    struct nodoProveedores *rec = lista;
    int cont = 1;
    do{
        printf("%i) ID: %4i  |   %s\n", cont,  rec->datosProveedor->idProveedor, rec->datosProveedor->nombre);
        cont++;
        rec = rec->sig;
    }while(rec != lista);
    puts("");
    puts("----------------------------------------");
}

void imprimirTodasBodegas(struct bodegas *arreglo[maxBodegas]){

    int i;
    for(i = 0; i < maxBodegas; i++)
        if(arreglo[i] != NULL)
            if(arreglo[i]->idBodega != 0) printf("%i) ID: %4i  |  CAPACIDAD: %8.2f Kg | DIRECCION: %s\n", i+1, arreglo[i]->idBodega, arreglo[i]->capacidad, arreglo[i]->direccion);
    puts("----------------------------------------");
}

void imprimirTodosProductos(struct nodoProductos *ABB){
    if (ABB){
        imprimirTodosProductos(ABB->izq);
        printf("ID: %i  |  NOMBRE: %s |  CANTIDAD: %i  |  PESO (kg): %.2f |  PRECIO: %i\n\n", ABB->datosProducto->idProducto, ABB->datosProducto->nombre, ABB->datosProducto->cantidad, ABB->datosProducto->peso, ABB->datosProducto->precio);
        imprimirTodosProductos(ABB->der);
        puts("----------------------------------------");
    }
}

void imprimirDetalleVenta(struct nodoVendido *lista){

    puts("\nDETALLE DE PRODUCTOS :\n");

    struct nodoVendido *rec = lista;
    while(rec != NULL){

        printf("ID: %4i  |  NOMBRE: %30s |  CANTIDAD: %5i  |  PRECIO: %5i |  TOTAL %i\n", rec->detalleProducto.idProducto, rec->detalleProducto.nombre, rec->detalleProducto.cantidad,
                                                                                                rec->detalleProducto.precio, (rec->detalleProducto.precio * rec->detalleProducto.cantidad));
        rec = rec->sig;
    }
}

int buscarBodegaDisponible(struct bodegas *arreglo[maxBodegas], float espacioRequerido){

    int i;
    for(i = 0; i < maxBodegas; i++)

        if(arreglo[i] != NULL)
            if(arreglo[i]->disponibilidad >= espacioRequerido) return i;
    return -1;
}

struct producto *buscarProductoEnABB(struct nodoProductos *ABB, int id){

    if(ABB == NULL) return NULL;

    else if(ABB->datosProducto->idProducto == id) return ABB->datosProducto;

    else if(ABB->datosProducto->idProducto > id) return buscarProductoEnABB(ABB->izq, id);

    else return buscarProductoEnABB(ABB->der, id);
}

struct producto *buscarProducto(struct bodegas *arreglo[maxBodegas], int id, int *bodegaProducto){

    int i;
    struct producto *product = NULL;
    for(i = 0; i < maxBodegas; i++){

        if(arreglo[i] != NULL){
            product = buscarProductoEnABB(arreglo[i]->ABP, id);
            if(product != NULL){
                (*bodegaProducto) = i;
                return product;
            }
        }
    }
    return product;
}

struct venta *buscarVenta(struct nodoVentas *lista, int id){

    struct nodoVentas *rec = lista;
    while(rec != NULL){

        if(rec->datosVenta->idVenta == id) return rec->datosVenta;
        rec = rec->sig;
    }
    return NULL;
}

void mostrarProveedor(struct nodoProveedores *lista, int opcion){

    int id;

    if(opcion == 1){
        do{
            printf("Ingrese id :");
            scanf("%i", &id); getchar();
            puts("");

            if(id == 0){
                puts("----------------------------------------"); return;
            }
            struct proveedor *datosProveedor = buscarProveedor(lista, id);
            if(datosProveedor) {
                printf("\nID: %i  | NOMBRE: %s", datosProveedor->idProveedor, datosProveedor->nombre);
                puts("----------------------------------------");
                return;

            }else puts("\nERROR: Proveedor no encontrado, reintente. (Ingrese 0 para salir).");

        }while(true);
    }
    else if(opcion == 2){

        if(lista == NULL){
            puts("\nERROR: No existen proveedores en sistema");
            puts("----------------------------------------");
        }
        else{
            puts("");
            imprimirTodosProveedores(lista);
        }
    }else{
        puts("----------------------------------------"); return;
    }
}

void mostrarBodegas(struct bodegas *arreglo[maxBodegas], int opcion){

    int id;

    if(opcion == 1){
        do{
            printf("Ingrese id :");
            scanf("%i", &id); getchar();

            if(id == 0){
                puts("----------------------------------------"); return;
            }
            struct bodegas *infoBodega = buscarBodega(arreglo, id);

            if(infoBodega) {
                printf("\nID: %4i  |  CAPACIDAD: %4.2f Kg | DIRECCION: %s\n", infoBodega->idBodega, infoBodega->capacidad, infoBodega->direccion);
                puts("----------------------------------------");
                return;
            }
            else puts("ERROR: Bodega no encontrada, reintente. (Ingrese 0 para salir).");

        }while(true);

    }else if(opcion == 2){

        if(verificarBodegas(arreglo) == 0){
            puts("\nERROR: No existen bodegas en sistema");
            puts("----------------------------------------");
        }else{
            puts("");
            imprimirTodasBodegas(arreglo);
        }
    }else {
        puts("----------------------------------------"); return;
    }
}

void mostrarProducto(struct bodegas *arreglo[maxBodegas], int opcion){

    int id;
    if (opcion == 1){
        do{
            printf("Ingrese id :");
            scanf("%i", &id); getchar();
            if(id == 0){
                puts("----------------------------------------"); return;
            }
            int bodegaTemp = -1;
            struct producto *datosProducto = buscarProducto(arreglo, id, &bodegaTemp);
            if (datosProducto){
                printf("ID: %i  |  NOMBRE: %s |  CANTIDAD: %i  |  PESO (kg): %.2f |  PRECIO: %i\n\n", datosProducto->idProducto, datosProducto->nombre, datosProducto->cantidad, datosProducto->peso, datosProducto->precio);
                puts("----------------------------------------");
                return;
            }
            else puts("ERROR: Producto no encontrado, reintente. (Ingrese 0 para salir).");
        } while(true);

    }
    else
    {
        if(verificarBodegas(arreglo) == 0){
            puts("\nERROR: No existen bodegas en sistema");
            puts("----------------------------------------");
            return;
        }
        else{
            printf("Ingrese id bodega :");
            scanf("%i", &id); getchar();
            if(id == 0){
                puts("----------------------------------------"); return;
            }
            struct bodegas *infoBodega = buscarBodega(arreglo, id);
            if (infoBodega){
                puts("");
                imprimirTodosProductos(infoBodega->ABP);
            }
            else puts("ERROR: Bodega no encontrada, reintente. (Ingrese 0 para salir).");
        }
    }
}

void mostrarVenta(struct nodoVentas *lista){

    int id;
    do{
        printf("Ingrese id :");
        scanf("%i", &id); getchar();

        if(id == 0){
            puts("----------------------------------------"); return;
        }
        struct venta *infoVenta = buscarVenta(lista, id);

        if(infoVenta) {
            puts("DETALLES DE LA VENTA :");
            printf("\nID: %i\nNOMBRE: %s\nFECHA: %s\nDIRECCION: %s\n", infoVenta->idVenta, infoVenta->nombreCliente, infoVenta->fecha, infoVenta->direccion);
            imprimirDetalleVenta(infoVenta->listaVendido);

            printf("\nTOTAL COMPRA : %i\n", infoVenta->total);
            puts("----------------------------------------");
            return;
        }
        else puts("ERROR: Venta no encontrada, reintente. (Ingrese 0 para salir).");

    }while(true);
}

void mostrarCompra(struct nodoProveedores *lista){

    int id, cont = 1;
    do{
        printf("Ingrese id del proveedor al que se compro (0 para salir) :");
        scanf("%i", &id); getchar();

        if(id == 0){ puts("----------------------------------------"); return;}
        struct proveedor *infoProveedor = buscarProveedor(lista, id);

        if(infoProveedor){

            puts("\nDETALLES DE LA COMPRA :\n");
            struct nodoCompras *rec = infoProveedor->listaCompras;
            while(rec != NULL){

                printf("%i) ID: %3i | NOMBRE: %5s | CANTIDAD: %i\n", cont, rec->detalleCompra->idProducto, rec->detalleCompra->nombre, rec->cantPorProduct);
                cont++;
                rec = rec->sig;
            }
            puts("\n----------------------------------------");
            return;
        }
        else puts("ERROR: Proveedor no encontrado, reintente. (Ingrese 0 para salir).");

    }while(true);
}

void opcionMostrar(struct MercaditoLibre *ML){

    int opcion, id;
    puts("----------------------------------------");
    puts("Que deseas mostrar? (elige una opcion).\n");
    puts("1. Mostrar proveedor\n2. Mostrar bodega\n3. Mostrar producto\n4. Mostrar venta\n5. Mostrar compra\n0. SALIR\n");
    printf("Opcion :");

    scanf("%i", &opcion); getchar();
    puts("----------------------------------------");
    validarOpcion(&opcion, 5);

    switch (opcion){

        case 1:
            puts("\n1. Buscar proveedor por su identidicador\n2. Ver todos los proveedores en el sistema\n0. SALIR\n");
            printf("Opcion :");
            scanf("%i", &opcion); getchar();
            validarOpcion(&opcion, 2);

            mostrarProveedor(ML->listaProveedores, opcion);
            break;

        case 2:
            puts("\n1. Buscar una bodega por su identidicador\n2. Ver todas las bodegas en el sistema\n0. SALIR\n");
            printf("Opcion :");
            scanf("%i", &opcion); getchar();
            validarOpcion(&opcion, 2);

            mostrarBodegas(ML->Bodega, opcion);
            break;

        case 3:
            puts("\n1. Buscar un producto por su identidicador\n2. Ver todos los productos de una bodega\n0. SALIR\n");
            printf("Opcion :");
            scanf("%i", &opcion); getchar();
            validarOpcion(&opcion, 2);

            mostrarProducto(ML->Bodega, opcion);
            break;

        case 4:
            mostrarVenta(ML->listaVentas);

        case 5:
            mostrarCompra(ML->listaProveedores);
        default: return;
    }
}

struct producto *crearInfoProducto(int id){

    struct producto *nuevo = (struct producto *) malloc(sizeof(struct producto));
    char buffer[maxChar];

    printf(" Ingresa el nombre del producto :");
    scanf(" %[^\n]s", buffer); getchar();

    int precio;
    printf(" Ingresa el precio del producto :");
    scanf("%i", &precio); getchar();

    float peso;
    printf(" Ingresa el peso del producto (en kilogramos) :");
    scanf("%f", &peso); getchar();

    nuevo->nombre = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(nuevo->nombre, buffer);
    nuevo->idProducto = id;
    nuevo->precio = precio;
    nuevo->peso = peso;
    nuevo->cantidad = 0;
    return nuevo;
}

struct nodoProductos *crearNodoProducto(struct producto *nodo){

    struct nodoProductos *nuevo = (struct nodoProductos *) malloc(sizeof(struct nodoProductos));
    nuevo->datosProducto = nodo;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

void agregarProducto(struct nodoProductos **ABB, struct nodoProductos *nuevo){

    if((*ABB) == NULL){
        (*ABB) = nuevo; return;
    }
    else if((*ABB)->datosProducto->idProducto > nuevo->datosProducto->idProducto) return agregarProducto(&(*ABB)->izq, nuevo);

    else return agregarProducto(&(*ABB)->der, nuevo);
}

void reemplazar(struct nodoProductos **ABB, struct nodoProductos **aux){

    if (!((*ABB)->der))
    {
        (*aux)->datosProducto = (*ABB)->datosProducto;
        *aux = (*ABB);
        *ABB = (*ABB)->izq;
    }
    else
        reemplazar(&(*ABB)->der, &(*aux));
}

void eliminarProducto(struct nodoProductos **ABB, int id){

    struct nodoProductos *aux = NULL;
    if (!(*ABB)) return;
    if ((*ABB)->datosProducto->idProducto < id)
        eliminarProducto(&(*ABB)->der, id);
    else
    {
        if ((*ABB)->datosProducto->idProducto > id)
            eliminarProducto(&(*ABB)->izq, id);
        else
        {
            if ((*ABB)->datosProducto->idProducto == id)
            {
                aux = (*ABB);
                if (!((*ABB)->izq))
                    *ABB = (*ABB)->der;
                else
                {
                    if (!((*ABB)->der))
                        *ABB = (*ABB)->izq;
                    else
                        reemplazar(&(*ABB)->izq, &aux);
                }
                free(aux);
            }
        }
    }
}

void eliminarProveedor(struct nodoProveedores **lista, int id){

    struct nodoProveedores *actual = *lista;
    if ((*lista)->datosProveedor->idProveedor == id){
        if ((*lista)->sig == *lista) *lista = NULL;
        else{
            (*lista)->ant->sig = (*lista)->sig;
            (*lista)->sig->ant = (*lista)->ant;
            *lista = (*lista)->sig;
        }
    }
    while (actual->sig != (*lista) && actual->sig->datosProveedor->idProveedor != id)
        actual = actual->sig;
    if (actual->sig->datosProveedor->idProveedor == id){
        actual->sig = actual->sig->sig;
        actual->sig->sig->ant = actual;
    }
}

void eliminarVenta(struct nodoVentas **lista, int id){

    struct nodoVentas *actual = (*lista);
    if ((*lista) != NULL)
    {
        if ((*lista)->datosVenta->idVenta == id){
            *lista = (*lista)->sig;
        }
        while (actual->sig != NULL){
            if (actual->sig->datosVenta->idVenta == id){
                actual->sig = actual->sig->sig;
                return;
            }
            actual = actual->sig;
        }
    }
}

struct venta *crearInfoVenta(struct nodoVentas *lista){

    int id;
    struct venta *nuevo = (struct venta *) malloc(sizeof(struct venta));

    do{
        printf("Ingresa el id de la venta :");
        scanf("%i", &id); getchar();

        if(id == 0){ puts("----------------------------------------"); return NULL;}

        else if(!buscarVenta(lista, id)){

            char fecha[maxChar];
            printf("Ingresa la fecha de venta (MM/DD/AAAA) :");
            scanf(" %[^\n]s", fecha); getchar();

            char cliente[maxChar];
            printf("Ingresa el nombre del cliente :");
            scanf(" %[^\n]s", cliente); getchar();

            char direccion[maxChar];
            printf("Ingresa la direccion de envio :");
            scanf(" %[^\n]s", direccion); getchar();

            nuevo->idVenta = id;
            nuevo->fecha = (char *) malloc(sizeof(char) * (strlen(fecha) + 1));
            strcpy(nuevo->fecha, fecha);

            nuevo->direccion = (char *) malloc(sizeof(char) * (strlen(direccion) + 1));
            strcpy(nuevo->direccion, direccion);

            nuevo->nombreCliente = (char *) malloc(sizeof(char) * (strlen(cliente) + 1));
            strcpy(nuevo->nombreCliente, cliente);
            nuevo->listaVendido = NULL;
            return nuevo;
        }
        else puts("\nERROR: El id que has ingresado ya existe, ingresa un id nuevo o 0 (cero) para salir\n");
    }while(true);
}

struct nodoVendido *crearNodoVendido(struct producto *datos, int unidades){

    struct nodoVendido *nuevo = (struct nodoVendido *) malloc(sizeof(struct nodoVendido));
    nuevo->detalleProducto = *datos;
    nuevo->detalleProducto.cantidad = unidades;
    nuevo->sig = NULL;
    return nuevo;
}

void agregarNodoVendido(struct nodoVendido **lista, struct nodoVendido *nodo){

    if((*lista) == NULL) (*lista) = nodo;
    else{
        struct nodoVendido *rec = (*lista);
        while(rec->sig != NULL) rec = rec->sig;

        rec->sig = nodo;
    }
}

void pedirProductosEnBodega(struct nodoVendido *lista, struct bodegas *arreglo[maxBodegas], int *totalVenta){

    int bodegaDelProducto;
    struct nodoVendido *rec = lista;
    struct producto *datoProducto = NULL;
    while(rec != NULL){

        datoProducto = buscarProducto(arreglo, rec->detalleProducto.idProducto, &bodegaDelProducto);
        arreglo[bodegaDelProducto]->disponibilidad += ((float)rec->detalleProducto.cantidad * rec->detalleProducto.peso);
        datoProducto->cantidad -= rec->detalleProducto.cantidad;

        (*totalVenta) += (rec->detalleProducto.cantidad * rec->detalleProducto.precio);
        rec = rec->sig;
    }
}

int confirmarVenta(struct venta *datos, struct bodegas *arreglo[maxBodegas]){

    int opcion, totalVenta;

    printf("Estas seguro que quieres realizar la venta?\n1. Si\n2. No\n\n");
    printf("Opcion :");
    scanf("%i", &opcion); getchar();
    puts("");
    validarOpcion(&opcion, 2);

    if(opcion == 1){

        pedirProductosEnBodega(datos->listaVendido, arreglo, &totalVenta);
        datos->total = totalVenta;
        return 0;
    }
    else return -1;
}

struct nodoVentas *crearNodoVenta(struct venta *nodo){

    struct nodoVentas *nuevo = (struct nodoVentas *) malloc(sizeof(struct nodoVentas));
    nuevo->datosVenta = nodo;
    nuevo->sig = NULL;
    return nuevo;
}

void agregarVenta(struct nodoVentas **lista, struct nodoVentas *nodo){

    if((*lista) == NULL) (*lista) = nodo;
    else{
        struct nodoVentas *rec = (*lista);
        while(rec->sig != NULL) rec = rec->sig;

        rec->sig = nodo;
    }
}

int verificarProducto(struct nodoVendido *lista, int id) {

    struct nodoVendido *rec = lista;
    while (rec != NULL) {

        if (rec->detalleProducto.idProducto == id) return -1;
        rec = rec->sig;
    }
    return 0;
}

void opcionVenta(struct MercaditoLibre *ML){

    int id, cont = 1, bodegaDelProducto, unidades;

    puts("----------------------------------------");
    puts("Bienvenido al portal de ventas.");
    puts("Ingresa los datos de la venta antes de continuar (0 para salir) : \n");

    struct venta *nodoVenta = crearInfoVenta(ML->listaVentas);
    if(nodoVenta){

        puts("\nAhora ingrese el id del/los producto/s a vender.");
        puts("Para finalizar la insercion de productos, ingrese 0 (cero).\n");
        do{

            printf("Producto %i : ", cont);
            scanf("%i", &id); getchar();

            if(id == 0){ puts("----------------------------------------"); break;}

            else if(verificarProducto(nodoVenta->listaVendido, id) == 0){

                printf("Ingresa la cantidad de unidades a vender : ");
                scanf("%i", &unidades);
                puts("");

                struct producto *datosProducto = buscarProducto(ML->Bodega, id, &bodegaDelProducto);

                if(datosProducto){

                    if(datosProducto->cantidad >= unidades){

                        struct nodoVendido *nodoVendido = crearNodoVendido(datosProducto, unidades);
                        agregarNodoVendido(&(nodoVenta->listaVendido), nodoVendido);
                        cont++;

                        if((datosProducto->cantidad - unidades) < 10)
                            printf("Alerta!: Quedan pocas unidades de este producto. Unidades restantes : %i\n\n", (datosProducto->cantidad - unidades));

                    }else{
                        puts("ERROR: No hay suficientes unidades.");
                        printf("Unidades disponibles : %i\n\n", datosProducto->cantidad);
                    }
                }else puts("ERROR: Producto no encontrado, reintente. (Ingrese 0 para salir).");
            }else puts("ERROR: No puedes repetir el producto, reintente. (Ingrese 0 para salir).");
        }while(true);

        if(nodoVenta->listaVendido != NULL && confirmarVenta(nodoVenta, ML->Bodega) != -1){

            struct nodoVentas *nodoV = crearNodoVenta(nodoVenta);
            agregarVenta(&(ML->listaVentas), nodoV);
            puts("La compra se ha realizado con exito!");
            puts("Puedes ver los detalles de la venta en la opcion 4 del menu con el id de venta.");
            puts("----------------------------------------");
        }
    }else return;
}

int contarOcurrencias(struct nodoCompras *lista, int id){

    int cont = 0;
    if (lista){
        while (lista != NULL){
            if (lista->detalleCompra->idProducto == id) cont++;
            lista = lista->sig;
        }
    }
    return cont;
}

void determinarOcurrencias(struct nodoProductos *ABB, struct nodoCompras *lista, struct producto *productoMayor, struct producto *productoMenor, int *ocurrenciasMayor, int *ocurrenciasMenor){

    int ocurrencias;
    if (ABB){
        determinarOcurrencias(ABB->izq, lista, productoMayor, productoMenor, ocurrenciasMayor, ocurrenciasMenor);
        ocurrencias = contarOcurrencias(lista, ABB->datosProducto->idProducto);
        if (ocurrencias < (*ocurrenciasMenor)){
            *productoMenor = *(ABB->datosProducto);
            *ocurrenciasMenor = ocurrencias;
        }
        else if (ocurrencias > (*ocurrenciasMayor)){
            *productoMayor = *(ABB->datosProducto);
            *ocurrenciasMayor = ocurrencias;
        }
        determinarOcurrencias(ABB->der, lista, productoMayor, productoMenor, ocurrenciasMayor, ocurrenciasMenor);
    }
}

void mostrarRotacionProducto(struct MercaditoLibre *ML){

    struct producto productoMayorRotacion, productoMenorRotacion;
    int mayorOcurrencia = -1, menorOcurrencia, i;
    for (i = 0 ; i < maxBodegas ; i++){
        if (ML->Bodega[i]){
            if (mayorOcurrencia == -1){
                productoMayorRotacion = *(ML->Bodega[i]->ABP->datosProducto);
                productoMenorRotacion = *(ML->Bodega[i]->ABP->datosProducto);
                menorOcurrencia = contarOcurrencias(ML->listaProveedores->datosProveedor->listaCompras, productoMenorRotacion.idProducto);
                mayorOcurrencia = menorOcurrencia;
            }
            determinarOcurrencias(ML->Bodega[i]->ABP, ML->listaProveedores->datosProveedor->listaCompras, &productoMayorRotacion, &productoMenorRotacion, &mayorOcurrencia, &menorOcurrencia);
        }
    }
    if (mayorOcurrencia != -1){
        printf("PRODUCTO CON MAYOR ROTACION:\n");
        printf("ID: %i  |  NOMBRE: %s |  OCURRENCIA: %i  |  PESO (kg): %.2f |  PRECIO: %i\n\n", productoMayorRotacion.idProducto, productoMayorRotacion.nombre, mayorOcurrencia, productoMayorRotacion.peso, productoMayorRotacion.precio);
        printf("PRODUCTO CON MENOR ROTACION:\n");
        printf("ID: %i  |  NOMBRE: %s |  OCURRENCIA: %i  |  PESO (kg): %.2f |  PRECIO: %i\n\n", productoMenorRotacion.idProducto, productoMenorRotacion.nombre, menorOcurrencia, productoMenorRotacion.peso, productoMenorRotacion.precio);
        puts("----------------------------------------");
    }
    else puts("\nERROR: No existen registros de ventas.");
}

int gestionarInvenCaso_uno(struct bodegas *arreglo[maxBodegas], struct producto *datosP, int cant, int bodegaProducto){

    float espacioAocupar;
    espacioAocupar = ((datosP->peso) * ((float)cant));

    if(arreglo[bodegaProducto]->disponibilidad >= espacioAocupar){
        arreglo[bodegaProducto]->disponibilidad -= espacioAocupar;
        datosP->cantidad += cant;

    }else{

        int bodegaCandidata;
        float espacioBuscado;

        espacioBuscado = espacioAocupar + ((float) datosP->cantidad * datosP->peso);
        bodegaCandidata = buscarBodegaDisponible(arreglo, espacioBuscado);

        if(bodegaCandidata != -1){

            eliminarProducto(&(arreglo[bodegaProducto]->ABP), datosP->idProducto);
            arreglo[bodegaProducto]->disponibilidad += (datosP->peso) * ((float) (datosP->cantidad));

            struct nodoProductos *nuevo = crearNodoProducto(datosP);
            agregarProducto(&(arreglo[bodegaCandidata]->ABP), nuevo);

            arreglo[bodegaCandidata]->disponibilidad -= espacioBuscado;
            datosP->cantidad += cant;
        }
        else{
            puts("\nERROR: No se ha podido agregar la cantidad ingresada a la bodega.");
            puts("No hay espacio suficiente en las bodegas disponibles.");
            puts("----------------------------------------");
            return -1;
        }
    }
    puts("\nSe ha agregado la cantidad comprada a la bodega con exito!");
    puts("----------------------------------------");
    return 0;
}

int gestionarInvenCaso_dos(struct bodegas *arreglo[maxBodegas], struct producto *datosP, int cant){

    int bodegaCandidata;
    float espacioAocupar;

    espacioAocupar = (datosP->peso) * ((float)cant);
    bodegaCandidata = buscarBodegaDisponible(arreglo, espacioAocupar);

    if(bodegaCandidata != -1){

        struct nodoProductos *nuevo = crearNodoProducto(datosP);
        nuevo->datosProducto->cantidad = cant;
        agregarProducto(&(arreglo[bodegaCandidata]->ABP), nuevo);

        arreglo[bodegaCandidata]->disponibilidad -= espacioAocupar;
        puts("\nEl producto ha sido agregado a la bodega con exito!");
        puts("----------------------------------------");
        return 0;

    }else {
        puts("\nERROR: No se ha podido agregar la cantidad ingresada a la bodega.");
        puts("No hay espacio suficiente en las bodegas disponibles.");
        puts("----------------------------------------");
        return -1;
    }
}

struct nodoCompras *crearNodoCompras(struct producto *nodo){

    struct nodoCompras *nuevo = (struct nodoCompras *) malloc(sizeof(struct nodoCompras));
    nuevo->detalleCompra = nodo;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    return nuevo;
}

void agregarNodoCompras(struct nodoCompras **lista, struct nodoCompras *nuevo){

    if(!(*lista)) (*lista) = nuevo;
    else{

        struct nodoCompras *rec = (*lista);
        while(rec->sig != NULL) rec = rec->sig;

        rec->sig = nuevo;
        nuevo->ant = rec;
    }
}

void opcionComprar(struct MercaditoLibre *ML){

    int id, cont = 1, bodegaDelProducto, unidades;

    puts("----------------------------------------");
    puts("Bienvenido al portal de compras.");

    do {
        printf("Ingresa el id del proveedor a comprar (0 para salir) :");
        scanf("%i", &id);
        getchar();

        if (id == 0) {
            puts("----------------------------------------");
            return;
        }
        struct proveedor *datosProveedor = buscarProveedor(ML->listaProveedores, id);
        if (datosProveedor) {

            puts("\nAhora ingrese el id del/los producto/s a comprar.");
            puts("Para finalizar la insercion de productos, ingrese 0 (cero).\n");

            do{
                printf("Producto %i : ", cont);
                scanf("%i", &id); getchar();

                if(id == 0){ puts("----------------------------------------"); return;}

                struct producto *datosProducto = buscarProducto(ML->Bodega, id, &bodegaDelProducto);
                if(datosProducto){

                    printf("Hemos encontrado el producto en nuestro sistema, ingresa la cantidad de unidades a comprar :");
                    scanf("%i", &unidades); getchar();

                    if(gestionarInvenCaso_uno(ML->Bodega, datosProducto, unidades, bodegaDelProducto) == 0){

                        struct nodoCompras *nodoC = crearNodoCompras(datosProducto);
                        nodoC->cantPorProduct = unidades;
                        agregarNodoCompras(&(datosProveedor->listaCompras), nodoC);
                    }else return;

                }else{

                    puts("\nEl producto ingresado no se encuentra en nuestro sistema.");
                    puts("Registra los datos del nuevo producto antes de continuar.\n");

                    struct producto *nodoPrto = crearInfoProducto(id);
                    printf("\nIngrese la cantidad de unidades a comprar :");
                    scanf("%i", &unidades); getchar();

                    if(gestionarInvenCaso_dos(ML->Bodega, nodoPrto, unidades) == 0){

                        struct nodoCompras *nodoC = crearNodoCompras(nodoPrto);
                        nodoC->cantPorProduct = unidades;
                        agregarNodoCompras(&(datosProveedor->listaCompras), nodoC);
                    }else return;
                }
                cont++;
            }while(true);

        }else puts("\nERROR: Proveedor no encontrado, reintente. (Ingrese 0 para salir).");

    }while(true);
}

void modificarProveedor(struct nodoProveedores *lista, struct proveedor *datos){

    int opcion, id;
    char buffer[maxChar];

    printf("DATOS DEL PROVEEDOR :\n");
    printf("1) ID : %i\n", datos->idProveedor);
    printf("2) NOMBRE : %s\n\n", datos->nombre);
    printf("Ingrese una opcion a modificar (0 para salir) :");
    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 2);

    if(opcion == 1){

        do {
            printf("Ingrese el nuevo id :");
            scanf("%i", &id); getchar();

            if(id == 0){
                puts("----------------------------------------"); return;
            }
            else if(!buscarProveedor(lista, id)){

                datos->idProveedor = id;
                puts("\nProveedor modificado con exito!");
                puts("----------------------------------------"); return;
            }else{
                puts("\nERROR: El id ingresado ya se encuentra registrado, reintente. (Ingrese 0 para salir).");
            }
        }while(true);
    }
    else if(opcion == 2){

        printf("Ingrese el nuevo nombre :");
        scanf(" %[^\n]s", buffer); getchar();

        free(datos->nombre);
        datos->nombre = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(datos->nombre, buffer);

        puts("\nProveedor modificado con exito!");
        puts("----------------------------------------"); return;
    }else return;
}

void modificarBodega(struct bodegas *arreglo[maxBodegas], struct bodegas *datos){

    int opcion, id;
    char buffer[maxChar];

    printf("DATOS DE LA BODEGA :\n");
    puts("(Solo puedes modificar la capacidad de la bodega si esta se encuentra vacia)\n");
    printf("1) ID : %i\n", datos->idBodega);
    printf("2) CAPACIDAD : %.2f Kg\n", datos->capacidad);
    printf("3) DIRECCION : %s\n\n", datos->direccion);
    printf("Ingrese una opcion a modificar (0 para salir) :");
    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 3);

    if(opcion == 1){

        do {
            printf("Ingrese el nuevo id :");
            scanf("%i", &id);getchar();

            if(id == 0){
                puts("----------------------------------------"); return;
            }
            else if(!buscarBodega(arreglo, id)){

                datos->idBodega = id;
                puts("\nBodega modificada con exito!");
                puts("----------------------------------------"); return;
            }else{
                puts("\nERROR: El id ingresado ya se encuentra registrado, reintente. (Ingrese 0 para salir).");
            }
        }while(true);
    }
    else if(opcion == 2){

        if(datos->ABP == NULL){

            float capacidad;
            printf("Ingrese la nueva capacidad :");
            scanf("%f", &capacidad);

            datos->capacidad = capacidad;
            datos->disponibilidad = capacidad;

            puts("\nBodega modificada con exito!");
            puts("----------------------------------------"); return;
        }
        else{ puts("ERROR: La bodega no se encuentra vacia"); return;}
    }
    else if(opcion == 3){

        printf("Ingrese la nueva direccion :");
        scanf(" %[^\n]s", buffer); getchar();

        free(datos->direccion);
        datos->direccion = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(datos->direccion, buffer);

        puts("\nBodega modificada con exito!");
        puts("----------------------------------------"); return;
    }else return;
}

void modificarProducto(struct bodegas *arreglo[maxBodegas], struct producto *datos){

    int opcion, id;
    int bodegaDelProducto;
    char buffer[maxChar];

    printf("DATOS DEL PRODUCTO:\n");
    puts("(No puedes modificar el peso del producto)\n");
    printf("1) ID : %i\n", datos->idProducto);
    printf("2) NOMBRE : %s\n", datos->nombre);
    printf("3) PRECIO : %i\n", datos->precio);
    printf("Ingrese una opcion a modificar (0 para salir) :");
    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 3);

    if(opcion == 1){

        do {
            printf("Ingrese el nuevo id :");
            scanf("%i", &id);getchar();

            if(id == 0){
                puts("----------------------------------------"); return;
            }
            else if(!buscarProducto(arreglo, id, &bodegaDelProducto)){

                datos->idProducto = id;
                puts("\nBodega modificada con exito!");
                puts("----------------------------------------"); return;
            }else{
                puts("\nERROR: El id ingresado ya se encuentra registrado, reintente. (Ingrese 0 para salir).");
            }
        }while(true);
    }
    else if(opcion == 2){

        printf("Ingrese el nuevo nombre :");
        scanf(" %[^\n]s", buffer); getchar();

        free(datos->nombre);
        datos->nombre = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(datos->nombre, buffer);

        puts("\nProducto modificado con exito!");
        puts("----------------------------------------"); return;

    }
    else if(opcion == 3){

        int precio;
        printf("Ingrese el nuevo precio :");
        scanf("%i", &precio); getchar();

        datos->precio = precio;
        puts("\nProducto modificado con exito!");
        puts("----------------------------------------"); return;
    }else return;
}

void modificarVenta(struct nodoVentas *lista, struct venta *datos){

    int opcion, id;
    int bodegaDelProducto;
    char buffer[maxChar];

    printf("DATOS DEL PRODUCTO:\n");
    puts("(No puedes modificar los productos de la venta)\n");
    printf("1) ID : %i\n", datos->idVenta);
    printf("2) NOMBRE : %s\n", datos->nombreCliente);
    printf("3) DIRECCION : %s\n", datos->direccion);
    printf("4) TOTAL (Por descuento o cupon) : %i\n", datos->total);

    printf("Ingrese una opcion a modificar (0 para salir) :");
    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 3);

    if(opcion == 1){

        do {
            printf("Ingrese el nuevo id :");
            scanf("%i", &id);getchar();

            if(id == 0){
                puts("----------------------------------------"); return;
            }
            else if(!buscarVenta(lista, id)){

                datos->idVenta = id;
                puts("\nVenta modificada con exito!");
                puts("----------------------------------------"); return;
            }else{
                puts("\nERROR: El id ingresado ya se encuentra registrado, reintente. (Ingrese 0 para salir).");
            }
        }while(true);
    }
    else if(opcion == 2){

        printf("Ingrese el nuevo nombre :");
        scanf(" %[^\n]s", buffer); getchar();

        free(datos->nombreCliente);
        datos->nombreCliente = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(datos->nombreCliente, buffer);

        puts("\nVenta modificada con exito!");
        puts("----------------------------------------"); return;

    }
    else if(opcion == 3) {

        printf("Ingrese la nueva direccion :");
        scanf(" %[^\n]s", buffer); getchar();

        free(datos->direccion);
        datos->direccion = (char *) malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(datos->direccion, buffer);

        puts("\nVenta modificada con exito!");
        puts("----------------------------------------");
        return;

    }else if(opcion == 4) {

        int total;
        printf("Ingrese el nuevo total de compra :");
        scanf(" %i", &total); getchar();

        datos->total = total;
        puts("\nVenta modificada con exito!");
        puts("----------------------------------------");
        return;
    }else return;
}

void opcionModificar(struct MercaditoLibre *ML){

    int opcion, id;

    puts("----------------------------------------");
    puts("Que deseas modificar? (elige una opcion).\n");
    puts("1. Modificar proveedor\n2. Modificar bodega\n3. Modificar producto\n4. Modificar venta\n0. SALIR\n");
    printf("Opcion :");

    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 4);

    switch(opcion){

        case 1:
            do{
                puts("----------------------------------------");
                printf(" Ingrese id del proveedor a modificar :");
                scanf("%i", &id); getchar();
                puts("");

                if(id == 0){
                    puts("----------------------------------------"); return;
                }
                struct proveedor *datosProveedor = buscarProveedor(ML->listaProveedores, id);

                if(datosProveedor){ modificarProveedor(ML->listaProveedores, datosProveedor); return;}

                else puts("\nERROR: Proveedor no encontrado, reintente. (Ingrese 0 para salir).");
            }while(true);

        case 2:
            do{
                puts("----------------------------------------");
                printf(" Ingrese id de la bodega a modificar :");
                scanf("%i", &id); getchar();
                puts("");

                if(id == 0){
                    puts("----------------------------------------"); return;
                }
                struct bodegas *datosBodega = buscarBodega(ML->Bodega, id);

                if(datosBodega){ modificarBodega(ML->Bodega, datosBodega); return;}

                else puts("\nERROR: Bodega no encontrada, reintente. (Ingrese 0 para salir).");
            }while(true);

        case 3:
            do{
                puts("----------------------------------------");
                printf(" Ingrese id del producto a modificar :");
                scanf("%i", &id); getchar();
                puts("");

                if(id == 0){
                    puts("----------------------------------------"); return;
                }
                int bodegaDelProducto;
                struct producto *datosProducto = buscarProducto(ML->Bodega, id, &bodegaDelProducto);

                if(datosProducto){ modificarProducto(ML->Bodega, datosProducto); return;}

                else puts("\nERROR: Producto no encontrado, reintente. (Ingrese 0 para salir).");
            }while(true);

        case 4:
            do{
                puts("----------------------------------------");
                printf(" Ingrese id de la venta a modificar :");
                scanf("%i", &id); getchar();
                puts("");

                if(id == 0){
                    puts("----------------------------------------"); return;
                }
                struct venta *datosVenta = buscarVenta(ML->listaVentas, id);

                if(datosVenta){ modificarVenta(ML->listaVentas, datosVenta); return;}

                else puts("\nERROR: Producto no encontrado, reintente. (Ingrese 0 para salir).");
            }while(true);
    }
}

void opcionEliminar(struct MercaditoLibre *ML){

    int opcion, id, bodega;

    puts("----------------------------------------");
    puts("Que deseas eliminar? (elige una opcion).\n");
    puts("1. Eliminar proveedor\n2. Eliminar bodega\n3. Eliminar producto\n4. Eliminar venta\n0. SALIR\n");
    printf("Opcion :");

    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 4);

    switch (opcion){

        case 1:
            do{
                printf(" Ingrese id del proveedor a eliminar :");
                scanf("%i", &id); getchar();
                if(id == 0){
                    puts("----------------------------------------");
                    return;
                }
                else if(buscarProveedor(ML->listaProveedores, id)){
                    eliminarProveedor(&(ML->listaProveedores), id);
                    puts("\nProveedor eliminado con exito!");
                    puts("----------------------------------------");
                    return;
                }
                else puts("\nERROR: Proveedor no encontrado, reintente. (Ingrese 0 para salir).");
            } while(true);

        case 2:
            do{
                printf(" Ingrese id de la bodega a eliminar :");
                scanf("%i", &id); getchar();
                if(id == 0){
                    puts("----------------------------------------");
                    return;
                }
                else if(buscarBodega(ML->Bodega, id)){
                    bodega = buscarPosicionBodega(ML->Bodega, id);

                    if (ML->Bodega[bodega]->ABP){
                        puts("\nERROR: No puede eliminar mientras existan productos en la bodega.");
                    }
                    else{
                        ML->Bodega[bodega] = NULL;
                        puts("\nBodega eliminada con exito!");
                        puts("----------------------------------------");
                    }
                    return;
                }
                else puts("\nERROR: Bodega no encontrada, reintente. (Ingrese 0 para salir).");
            } while(true);

        case 3:
            do{
                printf(" Ingrese id del producto a eliminar :");
                scanf("%i", &id); getchar();
                if(id == 0){
                    puts("----------------------------------------");
                    return;
                }
                else if(buscarProducto(ML->Bodega, id, &bodega)){
                    eliminarProducto(&(ML->Bodega[bodega]->ABP), id);
                    puts("\nProducto eliminado con exito!");
                    puts("----------------------------------------");
                    return;
                }
                else puts("\nERROR: Producto no encontrado, reintente. (Ingrese 0 para salir).");
            } while(true);

        case 4:
            do{
                printf(" Ingrese id de la venta a eliminar :");
                scanf("%i", &id); getchar();
                if(id == 0){
                    puts("----------------------------------------");
                    return;
                }
                else if(buscarVenta(ML->listaVentas, id)){
                    eliminarVenta(&(ML->listaVentas), id);
                    puts("\nVenta eliminada con exito!");
                    puts("----------------------------------------");
                    return;
                }
                else puts("\nERROR: Venta no encontrada, reintente. (Ingrese 0 para salir).");
            } while(true);
    }
}

int calcularTotalVendido(struct nodoVentas *lista, char *anio){

    int total = 0;

    struct nodoVentas *rec = lista;
    while(rec != NULL){

        char *ayio = &rec->datosVenta->fecha[strlen(rec->datosVenta->fecha) - 4];
        if(strcmp(ayio, anio) == 0) total += rec->datosVenta->total;

        rec = rec->sig;
    }
    return total;
}

void opcionMasOpciones(struct MercaditoLibre *ML){

    int opcion, total;
    char buffer[maxChar];

    puts("----------------------------------------");
    puts("Que deseas realizar? (elige una opcion).\n");
    puts("1. Mostrar producto mas vendido\n2. Total vendido en un anio\n3. Mostrar productos mayor y menor rotacion\n0. SALIR\n");
    printf("Opcion :");

    scanf("%i", &opcion); getchar();
    validarOpcion(&opcion, 3);

    switch(opcion){

        case 1:
            //mostrarProductoMasVendido(ML->listaVentas);
            break;

        case 2:

            printf("Ingresa el anio :");
            scanf(" %[^\n]s", buffer); getchar();

            total = calcularTotalVendido(ML->listaVentas, buffer);
            printf("Total recaudado en el anio %s : %i\n", buffer, total);
            puts("----------------------------------------");
            break;

        case 3:
            mostrarRotacionProducto(ML);
            break;

        default: return;
    }

}

// Rotacion productos.
// El producto con mas rotacion es aquel que mas reposiciones tuvo durante un periodo sin importar la cantidad en unidades que compre de un producto
int main(){

    struct MercaditoLibre *ML = NULL;
    inicializarMercaditoLibre(&ML);

    int opcion;
    int bodegasOcupadas = 0;
    puts("\n.:Bienvenido a Mercadito Libre!:.\n");

    do{
        puts("Que deseas hacer? (elige una opcion).\n");
        puts("1. Agregar proveedor / bodega.\n2. Comprar producto(s).\n3. Vender producto(s).\n4. Mostrar proveedor(es) / bodega(s) / producto(s) / venta(s) / compra(s)");
        puts("5. Modificar proveedor / venta / bodega / producto.\n6. Eliminar proveedor / bodega / producto / venta.\n7. Mas opciones.\n");
        printf("Opcion :");

        scanf("%i", &opcion); getchar();
        validarOpcion(&opcion, 7);

        switch(opcion){

            case 1: opcionAgregar(ML, &bodegasOcupadas); break;

            case 2: opcionComprar(ML); break;

            case 3: opcionVenta(ML); break;

            case 4: opcionMostrar(ML); break;

            case 5: opcionModificar(ML); break;

            case 6: opcionEliminar(ML); break;

            case 7: opcionMasOpciones(ML); break;
        }
    }while(opcion != 0);
    return 0;
}