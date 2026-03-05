#include "simpsoncraft.h"
#include "utiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

const char MOVER_IZQ      = 'A';
const char MOVER_DER      = 'D';
const char MOVER_ABAJO    = 'S';
const char USAR_ESCALERA  = 'E';
const char USAR_SOPLETE   = 'O';

const int SIGUE_JUGANDO = 0;
const int JUEGO_GANADO  = 1;
const int JUEGO_PERDIDO = -1;

// Pre: recibe un caracter cualquiera
// Post: devuelve true si el caracter corresponde a un movimiento válido del juego
bool es_movimiento_valido(char movimiento) {
    bool valido = false;
    if (movimiento == MOVER_IZQ ||
        movimiento == MOVER_DER ||
        movimiento == MOVER_ABAJO ||
        movimiento == USAR_ESCALERA ||
        movimiento == USAR_SOPLETE) {
        valido = true;
    }
    return valido;
}
// Pre: no requiere condiciones iniciales
// Post: solicita al usuario un movimiento y lo devuelve asegurando que sea válido
char leer_movimiento_usuario() {
    char movimiento = ' ';
    bool movimiento_invalido = true;
    while (movimiento_invalido) {
        printf("Ingrese movimiento (A, D, S, E, O): ");
        scanf(" %c", &movimiento);
        movimiento_invalido = !es_movimiento_valido(movimiento);
        if (movimiento_invalido) {
            printf("Movimiento inválido. Intente nuevamente\n");
        }
    }
    return movimiento;
}
// Pre: -
// Post: muestra por pantalla un mensaje de bienvenida y las instrucciones básicas del juego
void imprimir_bienvenida() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║       ¡Bienvenido a Simpsoncraft!    ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
    printf("Objetivo: conseguir la receta secreta 'R' y llegar hasta Moe 'M'\n");
    printf("sin quedarte sin movimientos. ¡Suerte!\n\n");
}
// Pre: -
// Post: pide un movimiento al usuario, ejecuta la jugada y actualiza el estado del juego en pantalla
void ejecutar_turno(juego_t *juego) {
    char movimiento = leer_movimiento_usuario();
    system("clear");
    realizar_jugada(juego, movimiento);
    mostrar_juego(*juego);
}
// Pre: recibe el juego finalizado (ganado o perdido)
// Post: muestra por pantalla el mensaje correspondiente al resultado de la partida
void mostrar_resultado_final(juego_t juego) {
    if (estado_juego(juego) == JUEGO_GANADO) {
        printf("\n🏆 ¡Ganaste! Homero llegó a Moe con la receta\n");
    } else if (estado_juego(juego) == JUEGO_PERDIDO) {
        printf("\n💀 ¡Perdiste! Te quedaste sin movimientos\n");
    }
}
int main() {
    srand((unsigned) time(NULL));
    juego_t juego;
    inicializar_juego(&juego);
    imprimir_bienvenida();
    mostrar_juego(juego);
    while (estado_juego(juego) == SIGUE_JUGANDO) {
        ejecutar_turno(&juego);
    }
    mostrar_resultado_final(juego);
    return 0;
}


