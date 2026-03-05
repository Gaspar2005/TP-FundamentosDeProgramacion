#include "simpsoncraft.h"
#include "utiles.h" 
#include <stdio.h>     
#include <stdlib.h>
#include <time.h>   
#include <stdbool.h>

const char LETRA_HOMERO = 'H';
const int POS_INICIAL_FILA = 0;
const int POS_INICIAL_COL = 0;
const int MADERAS_INICIALES = 15;
const int SOPLETES_INICIALES = 2;
const bool RECOLECTO_RECETA_INICIAL = false;

const char TIPO_VIGA = 'V';
const int RESISTENCIA_VIGA = -1;

const char TIPO_TIERRA = 'T';
const int RESISTENCIA_TIERRA = 2;
const int CANTIDAD_DE_TIERRAS = 80;

const char TIPO_TELARANA = 'A';
const int RESISTENCIA_TELARANA = 1;
const int CANTIDAD_DE_TELARANAS = 100;

const char TIPO_MADERA = 'W';
const int CANTIDAD_DE_MADERA = 20;
const char TIPO_SOPLETE = 'S';
const int CANTIDAD_DE_SOPLETES = 10;

const char TIPO_CABLES = 'C';
const int CANTIDAD_DE_CABLES = 25;
const char TIPO_MURCIELAGOS = 'B';
const int CANTIDAD_DE_MURCIELAGOS = 20;

const int MOVIMIENTOS_INICIALES = 100;

const char MOVIMIENTO_ABAJO     = 'S';
const char MOVIMIENTO_IZQUIERDA = 'A';
const char MOVIMIENTO_DERECHA   = 'D';
const char TECLA_ESCALERA = 'E';
const char TECLA_SOPLETE = 'O';
const int MADERA_RECOLECTADA = 2;
const char TIPO_ESCALERA = 'E';
const int COSTO_ESCALERA = 3;

const int EFECTO_CABLES = 5;
const int EFECTO_MURCIELAGOS = 3;

const int RANGO_MANHATTAN = 2;
const char LETRA_RECETA = 'R';
const char LETRA_MOE = 'M';

const int GANASTE = 1;
const int PERDISTE = -1;
// PRE: recibe un puntero a una coordenada válida
// POST: genera una posición aleatoria dentro de los límites del terreno
void generar_posicion_random(coordenada_t* posicion) {
    posicion->fil = rand() % MAX_FILAS;
    posicion->col = rand() % MAX_COLUMNAS;
}
// PRE:-
// POST: devuelve true si la posición dada coincide con la posición actual de Homero
bool posicion_ocupada_homero(juego_t* juego, coordenada_t nueva_posicion_homero) {
    bool respuesta_homero = false;
    if (juego->homero.posicion.fil == nueva_posicion_homero.fil && juego->homero.posicion.col == nueva_posicion_homero.col) {
        respuesta_homero = true;
    }
    return respuesta_homero;
}
// PRE:-
// Post: devuelve true si la posición dada coincide con la posición de Moe
bool posicion_ocupada_moe(juego_t* juego, coordenada_t nueva_posicion_moe) {
    bool respuesta_moe = false;
    if (juego->posicion_moe.fil == nueva_posicion_moe.fil && juego->posicion_moe.col == nueva_posicion_moe.col) {
        respuesta_moe = true;
    }
    return respuesta_moe;
}
// PRE: la posición está dentro del terreno
// Post: devuelve true si hay un bloque en esa posición
bool posicion_ocupada_bloques(juego_t* juego, coordenada_t nueva_posicion) {
    int i = 0;
    bool respuesta = false;
    while (!respuesta && i < juego->tope_bloques) {
        if (juego->bloques[i].posicion.fil == nueva_posicion.fil &&
            juego->bloques[i].posicion.col == nueva_posicion.col) {
            respuesta = true;
        }
        i++;
    }
    return respuesta;
}
// PRE:-
// Post: devuelve true si hay un bloque rompible (tierra o telaraña) en esa posición
bool tiene_bloque_rompible(juego_t* juego, coordenada_t posicion) { 
    bool ocupada = false;
    for (int i = 0 ; i < juego->tope_bloques && !ocupada ; i++) {
        if (juego->bloques[i].posicion.fil == posicion.fil && 
            juego->bloques[i].posicion.col == posicion.col &&
            (juego ->bloques[i].tipo==TIPO_TIERRA || juego->bloques[i].tipo == TIPO_TELARANA)){
                ocupada = true;
        }
    }
    return ocupada;
}
// PRE:-
// Post: devuelve true si ya existe una herramienta en esa posición
bool posicion_repetida_herramientas(const juego_t* juego, coordenada_t posicion) {
    bool herramientas_repetidas = false;
    for (int j = 0; j < juego->tope_herramientas && !herramientas_repetidas; j++) {
        if (juego->herramientas[j].posicion.fil == posicion.fil &&
            juego->herramientas[j].posicion.col == posicion.col) {
            herramientas_repetidas = true;
        }
    }
    return herramientas_repetidas;
}
// Pre: -
// Post: devuelve true si ya existe un obstáculo en esa posición
bool posicion_repetida_obstaculos(juego_t* juego, coordenada_t posicion) { 
    bool obstaculos_repetidos = false;
    for (int k = 0; k < juego->tope_obstaculos && !obstaculos_repetidos; k++) {
        if (juego->obstaculos[k].posicion.fil == posicion.fil &&
            juego->obstaculos[k].posicion.col == posicion.col) {
            obstaculos_repetidos = true;
        }
    }
    return obstaculos_repetidos;
}
// Pre: recibe un puntero a un juego válido
// Post: inicializa los valores iniciales del personaje Homero
void inicializar_homero(juego_t *juego) {
        juego->homero.tipo = LETRA_HOMERO;
        juego->homero.posicion.fil = POS_INICIAL_FILA;
        juego->homero.posicion.col = POS_INICIAL_COL;
        juego->homero.cantidad_madera = MADERAS_INICIALES;
        juego->homero.cantidad_sopletes = SOPLETES_INICIALES;
        juego->homero.recolecto_receta = RECOLECTO_RECETA_INICIAL;
}
// Pre: el tope_bloques en 0
// Post: agrega todas las vigas del mapa (ya con sus posiciones puestas por la biblioteca utiles.h) al arreglo de bloques
void inicializar_vigas(juego_t *juego) {
        coordenada_t vigas[MAX_VIGAS];
        int tope_vigas = 0;
        obtener_mapa(vigas, &tope_vigas);

        for (int i=0; i<tope_vigas;i++){
            juego ->bloques[juego->tope_bloques].tipo = TIPO_VIGA;
            juego ->bloques[juego->tope_bloques].posicion = vigas[i];
            juego ->bloques[juego->tope_bloques].resistencia = RESISTENCIA_VIGA;
            juego->tope_bloques++;
        }
}
// Pre: -
// Post: ubica a Moe en la última fila en una posición libre
void inicializar_moe(juego_t *juego) {
    coordenada_t posicion;
    bool posicion_encontrada_para_moe = false;
    while (!posicion_encontrada_para_moe) {
        generar_posicion_random(&posicion);
        posicion.fil = MAX_FILAS - 1;

        if (!posicion_ocupada_bloques(juego, posicion) && !posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego, posicion)) {
            posicion_encontrada_para_moe = true;
        }
    }
    juego->posicion_moe = posicion;
}
// Pre: -
// Post: coloca la cantidad definida de bloques de tierra en posiciones válidas dentro del terreno
void inicializar_tierras(juego_t *juego) {
    for (int i = 0; i < CANTIDAD_DE_TIERRAS; ) {
        coordenada_t posicion;
        generar_posicion_random(&posicion);
        if (!posicion_ocupada_bloques(juego, posicion) && !posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego, posicion)) {
            juego->bloques[juego->tope_bloques].tipo=TIPO_TIERRA;
            juego->bloques[juego->tope_bloques].posicion=posicion;
            juego->bloques[juego->tope_bloques].resistencia=RESISTENCIA_TIERRA;
            juego->tope_bloques++;
            i++;
        }
    }
}
// Pre: -
// Post: coloca la cantidad definida de bloques de telarañas en posiciones válidas dentro del terreno
void inicializar_telaranas(juego_t *juego) {
    for (int i = 0; i < CANTIDAD_DE_TELARANAS; ) {
        coordenada_t posicion;
        generar_posicion_random(&posicion);
        if (!posicion_ocupada_bloques(juego, posicion) && !posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego, posicion)) {
            juego->bloques[juego->tope_bloques].tipo=TIPO_TELARANA;
            juego->bloques[juego->tope_bloques].posicion=posicion;
            juego->bloques[juego->tope_bloques].resistencia=RESISTENCIA_TELARANA;
            juego->tope_bloques++;
            i++;
        }
    }
}
// Pre: deben existir bloques rompibles en el terreno
// Post: ubica las maderas sobre bloques rompibles en posiciones válidas dentro del terreno
void inicializar_maderas(juego_t *juego) {
    for (int i = 0; i < CANTIDAD_DE_MADERA; ) {
        coordenada_t posicion;
        generar_posicion_random(&posicion); // puede haber una funcion para recortar el if para herramientas
        if (tiene_bloque_rompible(juego, posicion) && !posicion_repetida_herramientas(juego, posicion) && !posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego, posicion)) {
            juego->herramientas[juego->tope_herramientas].tipo = TIPO_MADERA;
            juego->herramientas[juego->tope_herramientas].posicion = posicion;
            juego->tope_herramientas++;
            i++;
        }
    }
}
// Pre:deben existir bloques rompibles en el terreno
// Post: ubica los sopletes sobre bloques rompibles en posiciones válidas dentro del terreno
void inicializar_sopletes(juego_t *juego) {
    for (int i = 0; i < CANTIDAD_DE_SOPLETES; ) {
        coordenada_t posicion;
        generar_posicion_random(&posicion);
        if (tiene_bloque_rompible(juego, posicion) && !posicion_repetida_herramientas(juego, posicion) && !posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego, posicion)) {
            juego->herramientas[juego->tope_herramientas].tipo = TIPO_SOPLETE;
            juego->herramientas[juego->tope_herramientas].posicion = posicion;
            juego->tope_herramientas++;
            i++;
        }
    }
}
// Pre: -
// Post: inicializa todas las herramientas del juego llamando a las funciones correspondientes
void inicializar_herramientas(juego_t *juego) {
    inicializar_maderas(juego);
    inicializar_sopletes(juego);
}
// Pre:deben existir bloques rompibles en el terreno
// Post: ubica los cables en posiciones válidas dentro del terreno, evitando superposiciones con otros elementos
void inicializar_cables(juego_t *juego) {
    for (int i = 0; i < CANTIDAD_DE_CABLES; ) {
        coordenada_t posicion;
        generar_posicion_random(&posicion);
        if (tiene_bloque_rompible(juego, posicion) && !posicion_repetida_obstaculos(juego, posicion) && !posicion_repetida_herramientas(juego, posicion) && !posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego, posicion)) {
            juego->obstaculos[juego->tope_obstaculos].tipo = TIPO_CABLES;
            juego->obstaculos[juego->tope_obstaculos].posicion = posicion;
            juego->tope_obstaculos++;
            i++;
        }
    }
}
// Pre:deben existir bloques rompibles en el terreno
// Post: ubica los murciélagos en posiciones válidas dentro del terreno, evitando superposiciones con otros elementos
void inicializar_murcielagos(juego_t *juego) {
    for (int i = 0; i < CANTIDAD_DE_MURCIELAGOS; ) {
        coordenada_t posicion;
        generar_posicion_random(&posicion);
        if (tiene_bloque_rompible(juego, posicion) && !posicion_repetida_obstaculos(juego, posicion) && !posicion_repetida_herramientas(juego, posicion) && !posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego, posicion)) {
            juego->obstaculos[juego->tope_obstaculos].tipo = TIPO_MURCIELAGOS;
            juego->obstaculos[juego->tope_obstaculos].posicion = posicion;
            juego->tope_obstaculos++;
            i++;
        }
    }
}
// Pre: -
// Post: inicializa todos los obstáculos del juego llamando a las funciones correspondientes
void inicializar_obstaculos(juego_t *juego) {
    inicializar_cables(juego);
    inicializar_murcielagos(juego);
}
// Pre:-
// Post: coloca la receta en una posición válida del terreno que no se superponga con otros elementos
void inicializar_receta(juego_t *juego){
    bool receta_colocada = false;
    coordenada_t posicion;
    while(!receta_colocada){
        generar_posicion_random(&posicion);
        if (!posicion_ocupada_homero(juego, posicion) && !posicion_ocupada_moe(juego ,posicion) && !posicion_ocupada_bloques(juego,posicion) && !posicion_repetida_herramientas(juego , posicion) && !posicion_repetida_obstaculos(juego, posicion)){
            juego->posicion_receta = posicion;
            receta_colocada = true;
        }
    }
}
//   Pre condiciones: -
//  Post condiciones: Inicializará el juego, cargando toda la información inicial de Homero, Moe, los bloques, las herramientas y los obstáculos.
void inicializar_juego(juego_t *juego) {
        juego->tope_bloques = 0;
        juego->tope_herramientas = 0;
        juego->tope_obstaculos = 0;
        juego->movimientos_restantes = MOVIMIENTOS_INICIALES;
        inicializar_homero(juego);
        inicializar_vigas(juego);
        inicializar_moe(juego);
        inicializar_tierras(juego);
        inicializar_telaranas(juego);
        inicializar_herramientas(juego);
        inicializar_obstaculos(juego);
        inicializar_receta(juego);
}
// Pre:El juego debe estar inicializado previamente con `inicializar_juego`
// Post: devuelve true si las dos coordenadas representan la misma posición
bool en_misma_pos(coordenada_t a, coordenada_t b) {
    return (a.fil == b.fil) && (a.col == b.col);
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: devuelve true si hay una viga en la posición indicada
bool hay_viga_en_posicion(juego_t *juego, coordenada_t pos) {
    int i = 0;
    bool hay_viga = false;
    while (i < juego->tope_bloques && !hay_viga) {
        if (juego->bloques[i].tipo == TIPO_VIGA &&
            juego->bloques[i].posicion.fil == pos.fil &&
            juego->bloques[i].posicion.col == pos.col) {
            hay_viga = true;
        }
        i++;
    }
    return hay_viga;
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: devuelve true si hay una escalera en la posición indicada
bool tiene_escalera (juego_t *juego, coordenada_t posicion) { 
    bool hay_escalera = false;
    for (int i = 0; i < juego->tope_bloques && !hay_escalera; i++){
        if (juego->bloques[i].tipo == TIPO_ESCALERA && juego->bloques[i].posicion.fil == posicion.fil && juego->bloques[i].posicion.col == posicion.col){
            hay_escalera = true;
        }
    }
    return hay_escalera;
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: genera una nueva telaraña en una posición libre y válida
void regenerar_telarana(juego_t *juego) {
    coordenada_t nueva_posicion;
    bool posicion_valida = false;
    while (!posicion_valida) {
        generar_posicion_random(&nueva_posicion);
        if (!posicion_ocupada_bloques(juego, nueva_posicion) &&
            !posicion_ocupada_homero(juego, nueva_posicion) &&
            !posicion_ocupada_moe(juego, nueva_posicion) &&
            (juego->posicion_receta.fil != nueva_posicion.fil || juego->posicion_receta.col != nueva_posicion.col)) {
            posicion_valida = true;
        }
    }
    if (juego->tope_bloques < MAX_BLOQUES) {
        juego->bloques[juego->tope_bloques].tipo = TIPO_TELARANA;
        juego->bloques[juego->tope_bloques].posicion = nueva_posicion;
        juego->bloques[juego->tope_bloques].resistencia = RESISTENCIA_TELARANA;
        juego->tope_bloques++;
    }
}
// Pre: recibe un bloque válido que se destruye
// Post: elimina el bloque y aplica los efectos correspondientes
void procesar_bloque_destruido(juego_t *juego, int i) {
    if (juego->bloques[i].tipo == TIPO_TELARANA)
        regenerar_telarana(juego);
    else if (juego->bloques[i].tipo == TIPO_ESCALERA)
        juego->homero.cantidad_madera += MADERA_RECOLECTADA;
    juego->bloques[i] = juego->bloques[juego->tope_bloques - 1];
    juego->tope_bloques--;
}
// Pre: el índice pertenece a un bloque del juego
// Post: reduce la resistencia del bloque y lo elimina si se rompe
void dañar_bloque_si_es_rompible(juego_t *juego, int i) {
    if ( juego->bloques[i].tipo == TIPO_TIERRA || juego->bloques[i].tipo == TIPO_TELARANA) {
        juego->bloques[i].resistencia--;
        if (juego->bloques[i].resistencia <= 0)
            procesar_bloque_destruido(juego, i);
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego` y el movimiento recibido es lateral 
// Post: mueve a Homero una columna hacia la dirección indicada si está dentro del terreno
void mover_homero(juego_t* juego, char movimiento){
    if (movimiento == MOVIMIENTO_IZQUIERDA && juego->homero.posicion.col > 0){
        juego->homero.posicion.col--;
    }
    else if (movimiento == MOVIMIENTO_DERECHA && juego->homero.posicion.col < MAX_COLUMNAS - 1){
        juego->homero.posicion.col++;
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego` y la posición anterior de Homero es válida
// Post: si Homero choca con un bloque lateral, le resta resistencia y vuelve a su posición anterior
void interactuar_con_bloques_lateral(juego_t *juego, coordenada_t anterior) {
    for (int i = 0; i < juego->tope_bloques; i++) {
        if (juego->bloques[i].posicion.fil == juego->homero.posicion.fil && juego->bloques[i].posicion.col == juego->homero.posicion.col) {
            if (juego->bloques[i].tipo == TIPO_TIERRA || juego->bloques[i].tipo == TIPO_TELARANA) {
                dañar_bloque_si_es_rompible(juego, i);
                juego->homero.posicion = anterior;
            }
        }
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: resta de resistencia el bloque ubicado justo debajo de Homero si puede romperse
void interactuar_con_bloques_abajo(juego_t *juego) {
    int fila_abajo = juego->homero.posicion.fil + 1;
    int columna = juego->homero.posicion.col;
    for (int i = 0; i < juego->tope_bloques; i++) {
        if (juego->bloques[i].posicion.fil == fila_abajo &&
            juego->bloques[i].posicion.col == columna) {
            dañar_bloque_si_es_rompible(juego, i);
            if (juego->bloques[i].tipo == TIPO_ESCALERA){
                procesar_bloque_destruido(juego,i);
            }
        }
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: devuelve true si Homero puede moverse lateralmente sin chocar con una viga
bool puede_moverse_lateralmente(juego_t* juego, char movimiento) {
    coordenada_t destino = juego->homero.posicion;
    if (movimiento == MOVIMIENTO_IZQUIERDA && destino.col > 0) {
        destino.col--;
    }
    else if (movimiento == MOVIMIENTO_DERECHA && destino.col < MAX_COLUMNAS - 1) {
        destino.col++;
    }
    else {
        return false; 
    }
    return !hay_viga_en_posicion(juego,destino);
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: mueve a Homero una fila hacia abajo si no hay bloque o viga debajo
bool puede_moverse_hacia_abajo(juego_t *juego) {
    if (juego->homero.posicion.fil >= MAX_FILAS - 1){
        return false;
    }
    coordenada_t debajo = juego->homero.posicion;
    debajo.fil = debajo.fil + 1;
    debajo.col = debajo.col;
    if (hay_viga_en_posicion(juego, debajo)) {
        return false;
    }
    if (tiene_bloque_rompible(juego, debajo) || tiene_escalera(juego, debajo)) {
        interactuar_con_bloques_abajo(juego);
        juego->movimientos_restantes--;
        return false;
    }
    juego->homero.posicion.fil++;
    return true;
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: crea una escalera en la posición actual si Homero tiene madera suficiente y el espacio lo permite
void crear_escalera(juego_t *juego) {
    bool puede_crear = true;
    int fila = juego->homero.posicion.fil;
    int col  = juego->homero.posicion.col;
    if (fila == 0 || juego->homero.cantidad_madera < COSTO_ESCALERA || juego->tope_bloques >= MAX_BLOQUES) {
        puede_crear = false;
    }
    coordenada_t arriba;
    arriba.fil = fila - 1;
    arriba.col = col;
    if (posicion_ocupada_bloques(juego, arriba))
        puede_crear = false;
    if (puede_crear) {
        juego->bloques[juego->tope_bloques].tipo = TIPO_ESCALERA;
        juego->bloques[juego->tope_bloques].posicion.fil = fila;
        juego->bloques[juego->tope_bloques].posicion.col = col;
        juego->bloques[juego->tope_bloques].resistencia = 1;
        juego->tope_bloques++;
        juego->homero.posicion.fil--;
        juego->homero.cantidad_madera -= COSTO_ESCALERA;
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: si Homero se encuentra sobre una herramienta, la recolecta y se elimina del terreno
void recolectar_herramientas_si_corresponde(juego_t* juego) {
    int i = 0;
    while (i < juego->tope_herramientas) {
        if (en_misma_pos(juego->herramientas[i].posicion, juego->homero.posicion)) {
            if (juego->herramientas[i].tipo == TIPO_MADERA) juego->homero.cantidad_madera++;
            else if (juego->herramientas[i].tipo == TIPO_SOPLETE) juego->homero.cantidad_sopletes++;
            juego->herramientas[i] = juego->herramientas[juego->tope_herramientas - 1];
            juego->tope_herramientas--;
        } else {
            i++;
        }
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: aplica el efecto del obstáculo en la posición de Homero y lo elimina del terreno
void aplicar_obstaculos_si_corresponde(juego_t* juego) {
    int i = 0;
    while (i < juego->tope_obstaculos) {
        if (en_misma_pos(juego->obstaculos[i].posicion, juego->homero.posicion)) {
            if (juego->obstaculos[i].tipo == TIPO_CABLES) {
                if (juego->movimientos_restantes >= EFECTO_CABLES){
                    juego->movimientos_restantes -= EFECTO_CABLES;
                }
                else{
                    juego->movimientos_restantes = 0;
                }
            }
        else if (juego->obstaculos[i].tipo == TIPO_MURCIELAGOS) {
                if (juego->homero.cantidad_madera >= EFECTO_MURCIELAGOS) juego->homero.cantidad_madera -= EFECTO_MURCIELAGOS;
                else juego->homero.cantidad_madera = 0;
            }
            juego->obstaculos[i] = juego->obstaculos[juego->tope_obstaculos - 1];
            juego->tope_obstaculos--;
        } else {
            i++;
        }
    }
}
// Pre: recibe dos coordenadas válidas
// Post: devuelve la distancia manhattan entre ambas posiciones
int distancia_manhattan (coordenada_t a, coordenada_t b){
    return abs(a.fil - b.fil) + abs(a.col - b.col);
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: si Homero no tiene bloques debajo, cae hasta encontrar uno o llegar al fondo
void aplicar_gravedad(juego_t *juego){
    bool seguir_bajando = true;
    while (juego->homero.posicion.fil < MAX_FILAS - 1 && seguir_bajando){
        int fila_abajo = juego->homero.posicion.fil +1;
        int columna_abajo = juego->homero.posicion.col;
        bool hay_bloque_abajo = false;
        for (int i = 0; i < juego->tope_bloques && !hay_bloque_abajo; i++) {
            if (juego->bloques[i].posicion.fil == fila_abajo && juego->bloques[i].posicion.col == columna_abajo){
                hay_bloque_abajo = true;
            }
        }
        if (!hay_bloque_abajo){
            juego->homero.posicion.fil++;
            recolectar_herramientas_si_corresponde(juego);
            aplicar_obstaculos_si_corresponde(juego);
        }
        else {
            seguir_bajando = false;
        }
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego` y Homero tiene al menos un soplete
// Post: destruye los bloques en un radio de distancia 2 sin afectar vigas ni personajes
void usar_soplete(juego_t *juego) {
    if (juego->homero.cantidad_sopletes > 0) {
        for (int i = 0; i < juego->tope_bloques; i++) {
            if ((juego->bloques[i].tipo == TIPO_TIERRA || juego->bloques[i].tipo == TIPO_TELARANA || juego->bloques[i].tipo == TIPO_ESCALERA) &&
                distancia_manhattan(juego->bloques[i].posicion, juego->homero.posicion)<=RANGO_MANHATTAN){
                if (juego->bloques[i].tipo == TIPO_TELARANA) {
                    regenerar_telarana(juego);
                }
                if (juego->bloques[i].tipo == TIPO_ESCALERA){
                    juego->homero.cantidad_madera +=MADERA_RECOLECTADA;
                }
                
                juego->bloques[i] = juego->bloques[juego->tope_bloques - 1];
                juego->tope_bloques--;
                i--;
            }
        }
        juego->homero.cantidad_sopletes--;
        aplicar_gravedad(juego);
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: marca la receta como recolectada si Homero pasa por encima
void recolectar_receta_si_corresponde(juego_t *juego){
    if (!juego->homero.recolecto_receta && en_misma_pos(juego->homero.posicion, juego->posicion_receta)){
        juego->homero.recolecto_receta = true;
    }
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego` y el movimiento recibido es válido
// Post: ejecuta la acción correspondiente según el movimiento ingresado
void procesar_accion(juego_t *juego, char movimiento) {
    coordenada_t anterior = juego->homero.posicion;
    if (movimiento == MOVIMIENTO_DERECHA || movimiento == MOVIMIENTO_IZQUIERDA) {
        if(puede_moverse_lateralmente(juego,movimiento)){
            mover_homero(juego, movimiento);
            interactuar_con_bloques_lateral(juego, anterior);
            juego->movimientos_restantes--;
        }
    }
    else if (movimiento == MOVIMIENTO_ABAJO) {
        if(puede_moverse_hacia_abajo(juego)){
            interactuar_con_bloques_abajo(juego);
            juego->movimientos_restantes--;
        }
    }
    else if (movimiento == TECLA_ESCALERA) {
        crear_escalera(juego);
    }
    else if (movimiento == TECLA_SOPLETE) {
        usar_soplete(juego); 
    }
}
//  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción debe ser válida.
//  Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
void realizar_jugada(juego_t *juego, char movimiento) {
    procesar_accion(juego, movimiento);
    aplicar_gravedad(juego);
    recolectar_herramientas_si_corresponde(juego);
    aplicar_obstaculos_si_corresponde(juego);
    recolectar_receta_si_corresponde(juego);
}
// Pre: recibe un juego válido y una coordenada dentro del terreno
// Post: devuelve el tipo de bloque presente o '.' si no hay ninguno
char bloque_en_posicion(juego_t juego, coordenada_t posicion) {
    char tipo_bloque = '.';
    bool bloque_encontrado = false;
    for (int i = 0; i < juego.tope_bloques && !bloque_encontrado; i++) {
        if (en_misma_pos(juego.bloques[i].posicion, posicion)) {
            tipo_bloque = juego.bloques[i].tipo;
            bloque_encontrado = true;
        }
    }
    return tipo_bloque;
}
// Pre: recibe un juego válido y una posición dentro del terreno
// Post: devuelve el tipo de herramienta en esa posición o '.' si no hay
char herramienta_en_posicion(juego_t juego, coordenada_t posicion) {
    char tipo_herramienta = '.';
    bool herramienta_encontrada = false;
    for (int i = 0; i < juego.tope_herramientas && !herramienta_encontrada; i++) {
        if (en_misma_pos(juego.herramientas[i].posicion, posicion)) {
            tipo_herramienta = juego.herramientas[i].tipo;
            herramienta_encontrada = true;
        }
    }
    return tipo_herramienta;
}
// Pre: recibe un juego válido y una posición dentro del terreno
// Post: devuelve true si existe un bloque en esa posición
bool hay_bloque_en(juego_t juego, coordenada_t posicion) {
    bool hay_bloque = false;
    for (int i = 0; i < juego.tope_bloques && !hay_bloque; i++) {
        if (en_misma_pos(juego.bloques[i].posicion, posicion)) {
            hay_bloque = true;
        }
    }
    return hay_bloque;
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego` y la coordenada pertenece al terreno
// Post: devuelve el caracter que representa el contenido visible de esa celda
char caracter_en_posicion(juego_t juego, coordenada_t posicion) {
    char caracter = bloque_en_posicion(juego, posicion);
    if (caracter == '.' && !hay_bloque_en(juego, posicion)) {
        caracter = herramienta_en_posicion(juego, posicion);
    }
    if (!juego.homero.recolecto_receta && en_misma_pos(juego.posicion_receta, posicion)) {
        caracter = LETRA_RECETA;
    }
    if (en_misma_pos(juego.posicion_moe, posicion)) {
        caracter = LETRA_MOE;
    }
    if (en_misma_pos(juego.homero.posicion, posicion)) {
        caracter = LETRA_HOMERO;
    }
    return caracter;
}
// Pre: recibe un juego válido y un número de fila dentro del terreno
// Post: imprime en pantalla los caracteres de esa fila
void imprimir_fila(juego_t juego, int fila) {
    coordenada_t posicion;
    posicion.fil = fila;
    for (int col = 0; col < MAX_COLUMNAS; col++) {
        posicion.col = col; // columna actual
        printf("%c ", caracter_en_posicion(juego, posicion));
    }
    printf("\n");
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: imprime la información del estado actual del jugador
void imprimir_menu(juego_t juego) {
    printf("Movimientos restantes: %d\n", juego.movimientos_restantes);
    printf("Maderas: %d\n", juego.homero.cantidad_madera);
    printf("Sopletes: %d\n", juego.homero.cantidad_sopletes);
    printf("Receta: %s\n", juego.homero.recolecto_receta ? "✅ Recolectada" : "❌ No recolectada");
}
//  Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego `.
//  Post condiciones: Imprime el juego por pantalla.
void mostrar_juego(juego_t juego) {
    for (int i = 0; i < MAX_FILAS; i++) {
        imprimir_fila(juego, i);
    }
    imprimir_menu(juego);
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: devuelve true si Homero está en la misma posición que Moe y tiene la receta
bool homero_gano(juego_t juego) {
    bool gano = false;
    if (juego.homero.recolecto_receta &&
        juego.homero.posicion.fil == juego.posicion_moe.fil &&
        juego.homero.posicion.col == juego.posicion_moe.col) {
        gano = true;
    }
    return gano;
}
// Pre: El juego debe estar inicializado previamente con `inicializar_juego`
// Post: devuelve true si Homero se quedó sin movimientos
bool homero_perdio(juego_t juego) {
    bool perdio = false;
    if (juego.movimientos_restantes <= 0) {
        perdio = true;
    }
    return perdio;
}
//  Pre condiciones: El juego deberá estar inicializado previamente con `inicializar_juego `
//  Post condiciones: Devuelve:
//  --> 1 si es ganado
//  --> -1 si es perdido
//  --> 0 si se sigue jugando
//  El juego se dará por ganado cuando Homero recolecta su receta secreta y llega a Moe.
//  Se dará por perdido si se le terminan los movimientos antes de recolectar la receta y llegar a Moe.
int estado_juego(juego_t juego) {
    int estado = 0;
    if (homero_perdio(juego)) {
        estado = PERDISTE; 
    } 
    else if (homero_gano(juego)) {
        estado = GANASTE;
    }
    return estado;
}
