#include "sfml-utn-inspt.h"

/*
 * Constantes generales de la ventana
 */
const unsigned int ANCHO_VENT = 800;
const unsigned int ALTO_VENT = 600;
const unsigned int FRAMERATE = 60;

// lee los eventos (clicks, posición del mouse, redimensión de la ventana, etc)
void read_events(RenderWindow &window);

// Parametros del cuadrado que rebota en la ventana:
const float VELOCIDAD = 4.5f;
const float LADO_CUADRADO = 25.f;

// Tipo de dato custom, borde de la ventana, para detectar coliciones
enum borde_t { SUPERIOR, INFERIOR, IZQUIERDO, DERECHO };

/**
 * @brief Detecta la colición de un rectangulo con la ventana
 *
 * @param r Rectangulo a evaluar si coliciona
 * @param borde Con qué borde (borde_t) tiene que contrastar la colisión
 * @return true si ha colisionado el rectangulo `r` con el `borde` especificado
 * @return false si no ha colicionado o se pasa un `borde` inválido.
 */
bool colision_con_ventana(RectangleShape &r, borde_t borde);

int main() {
  // Se crea la ventana de ANCHO_VENT x ALTO_VENT con el nombre "UTN-INSPT SFML"
  RenderWindow window = RenderWindow{{ANCHO_VENT, ALTO_VENT}, "UTN-INSPT SFML"};
  // Se establece un limite de FRAMERATE x segundo
  window.setFramerateLimit(FRAMERATE);

  // Se define un cuadrado de LADO_CUADRADO de lado
  RectangleShape cuadrado = RectangleShape({LADO_CUADRADO, LADO_CUADRADO});
  // Color RGB (Rojo, Verde, Azul) entre 0 y 255
  cuadrado.setFillColor(Color{255, 0, 128});

  // Los diferenciales de movimiento (cuanto avanza el cuadrado) en los ejes x,y
  float diff_x, diff_y;

  /*
   * Loop principal del juego: mientras la ventana esté abierta...
   */
  while (window.isOpen()) {
    /**
     *  Lectura de los eventos y teclas por parte del usuario
     */
    read_events(window);

    /*
     *  Lógica del juego
     */
    // Cambia el sentido del movimiento en X si se detecta una colisión con el
    // borde de la ventana
    if (colision_con_ventana(cuadrado, IZQUIERDO)) {
      diff_x = VELOCIDAD;
    } else if (colision_con_ventana(cuadrado, DERECHO)) {
      diff_x = -VELOCIDAD;
    }
    // Idem anterior, pero en el eje Y
    if (colision_con_ventana(cuadrado, SUPERIOR)) {
      diff_y = VELOCIDAD;
    } else if (colision_con_ventana(cuadrado, INFERIOR)) {
      diff_y = -VELOCIDAD;
    }

    // Se desplaza el cuadrado en diff_x y diff_y
    cuadrado.move(diff_x, diff_y);

    /*
     *  Actualizar el dibujo de la ventana
     */
    // Siempre se borra la pantalla anterior
    window.clear();

    // Se dibujan los elementos:
    window.draw(cuadrado);

    // Se muestra la pantalla con los elemnetos dibujados sobre ella:
    window.display();
  }
}

void read_events(RenderWindow &window) {
  // Se "leen" los eventos y se guardan en event.
  Event event = Event();
  // Se recorren los eventos hasta que ya no hayan más
  while (window.pollEvent(event)) {
    if (event.type == Event::Closed) {
      // Si el usuario tocó la [X] para cerra la ventana:
      window.close();
    }
  }
}

bool colision_con_ventana(RectangleShape &r, borde_t borde) {
  switch (borde) {
  case SUPERIOR:
    return r.getGlobalBounds().top <= 0;
  case INFERIOR:
    return (r.getGlobalBounds().top + r.getGlobalBounds().height) >= ALTO_VENT;
  case IZQUIERDO:
    return r.getGlobalBounds().left <= 0;
  case DERECHO:
    return (r.getGlobalBounds().left + r.getGlobalBounds().width) >= ANCHO_VENT;
  }
  return false;
}
