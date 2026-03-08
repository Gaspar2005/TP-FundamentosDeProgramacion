# SimpsonCraft 🧱🔥
Trabajo práctico del curso **Fundamentos de Programación**  
FIUBA – **[Cátedra Méndez](https://fundamendez.com.ar/)**
## ¿Qué es?

SimpsonCraft es un juego desarrollado en C como trabajo práctico del curso
Fundamentos de Programación (FIUBA).

El objetivo es ayudar a Homero a recuperar su receta secreta y llegar hasta Moe
antes de quedarse sin movimientos.

## Demo


https://github.com/user-attachments/assets/f13059ef-33c2-493b-8532-ccbb443f8c40




## 📜 Instrucciones de juego

### 🎯 Objetivo
Encontrar la **receta secreta (R)** y llegar hasta **Moe (M)** antes de quedarte sin movimientos.

### 🎮 Controles

- **A** → mover a la izquierda  
- **D** → mover a la derecha  
- **S** → romper bloque debajo  
- **E** → crear escalera  
- **O** → usar soplete  

### 🧱 Bloques

- **T** → tierra (se rompe en 2 golpes)
- **A** → telaraña (se rompe en 1 golpe)
- **V** → viga (indestructible)
- **E** → escalera

### 🧰 Objetos

- **W** → madera (para construir escaleras)
- **S** → soplete (rompe bloques a distancia)

### ⚠️ Obstáculos

- **C** → cables (perdés movimientos)
- **B** → murciélagos (perdés madera)

### 🏆 Ganar

Recolectar la receta **y llegar a Moe** antes de quedarte sin movimientos.

## Compilación

gcc *.c utiles.o -o juego -std=c99 -Wall -Wconversion -Werror -lm

## Ejecución

./juego

### ⚠️ _Not Windows friendly._ (Únicamente soporta Linux y macOS.)
