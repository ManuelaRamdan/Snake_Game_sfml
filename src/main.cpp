#include "sfml-utn-inspt.h"
// constantes de la ventana
const unsigned int ANCHO_VENT = 800;
const unsigned int ALTO_VENT = 600;
const unsigned int FRAMERATE = 60;

//-------------------------------------------------------------------------
// creamos la variable direccion t cuanros cuadraditos se mueven
enum Direccion
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

const float MOVIMIENTO = 20.0f;
Texture texCuerpoHorizontal, texCuerpoVertical;

const float VELOCIDAD = 4.5f;
const float LADO_CUADRADO = 25.f;

const string PATH = "../recursos/Graphics/";
#define CANT_TEXT 4
Texture textCola[CANT_TEXT];
Texture textDoblar[CANT_TEXT];

Texture texturaCabeza, texturaCuerpo, texturaCola;

class rectacgleSnake : public sf::RectangleShape
{

public:
    rectacgleSnake()
    {
        setSize(Vector2f(20, 20));
    }
};

// cabeceras de las funciones
bool colisionConBordesForma(const CircleShape &forma, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

bool colisionConBordesSnake(const vector<rectacgleSnake> &snake, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

bool colisionConBordesRectangulo(const RectangleShape &bloque, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior);

void moverSnake(Direccion &direccion, vector<rectacgleSnake> &snake);

bool colisionArbustoCabezaSnake(const RectangleShape &arbusto, const rectacgleSnake &cabeza);

bool colisionConArbustoManzana(const RectangleShape &arbusto, const CircleShape &forma);

bool colisionArbustoSnake(const RectangleShape &arbusto, const vector<rectacgleSnake> &snake);
bool colisionManzanaSnake(const CircleShape &manzana, const vector<rectacgleSnake> &snake);

int posCola(Vector2f actual, Vector2f anterior);
int posCurva(Vector2f anterior, Vector2f actual, Vector2f siguiente);

bool colisionConCuerpoSnake(vector<rectacgleSnake> &snake);
void cargarTextura(Direccion &direccion, vector<rectacgleSnake> &snake);

void recolocarManzana(CircleShape &manzana, const RectangleShape &arbusto, const vector<rectacgleSnake> &snake, const RectangleShape &bordeIzq, const RectangleShape &bordeDer, const RectangleShape &bordeInf, const RectangleShape &bordeSup);

int main()
{
    rectacgleSnake rs;
    texCuerpoHorizontal.loadFromFile(PATH + "cuerpoHorizontal.png");
    texCuerpoVertical.loadFromFile(PATH + "cuerpoVertical.png");

    textCola[0].loadFromFile(PATH + "colaArriba.png");
    textCola[1].loadFromFile(PATH + "colaDerecha.png");
    textCola[2].loadFromFile(PATH + "colaAbajo.png");
    textCola[3].loadFromFile(PATH + "colaIzq.png");

    // creamos la ventana
    RenderWindow ventana = RenderWindow({ANCHO_VENT, ALTO_VENT}, "Unidad 1 - Taller de Videojuegos");
    ventana.setFramerateLimit(FRAMERATE);
    //-------------------------------------------------------------------------
    // sonido de cuando come la manzana
    SoundBuffer bufferComer;
    if (!bufferComer.loadFromFile("../recursos/comeManzana.mp3"))
    {
        std::cerr << "Error al cargar el buffer" << std::endl;
    }

    Sound sonidoCome;
    sonidoCome.setBuffer(bufferComer);
    sonidoCome.setVolume(100.0);
    //-------------------------------------------------------------------------
    // creamos la "manzana"
    Texture manzana;
    manzana.loadFromFile("../recursos/Graphics/manzana.png");
    CircleShape forma = CircleShape(15, 20);
    forma.setPosition((ANCHO_VENT / 2) + 20, (ALTO_VENT / 2));
    forma.setTexture(&manzana);

    //-------------------------------------------------------------------------
    // cabeceras de las funciones
    // Mover esta línea al inicio del archivo

    //-------------------------------------------------------------------------
    // creamos el bloque
    Texture arbustoText;
    arbustoText.loadFromFile("../recursos/Graphics/arbustro.png");
    RectangleShape arbusto(Vector2f(100, 100));
    arbusto.setPosition(100, 100);
    arbusto.setTexture(&arbustoText);

    Vector2f arbustoPosInicial = arbusto.getPosition();
    //-------------------------------------------------------------------------
    // cargar textura de la serpiente

    if (!texturaCabeza.loadFromFile(PATH + "cabezaDerecha.png") ||
        !texturaCuerpo.loadFromFile(PATH + "cuerpoHorizontal.png") ||
        !texturaCola.loadFromFile(PATH + "colaDerecha.png"))
    {
        return -1; // Termina si no se pueden cargar las texturas
    }

    //-------------------------------------------------------------------------
    // creamos  el "cuerpo" de la serpiente
    vector<rectacgleSnake> snake;
    int x = (ANCHO_VENT / 2);
    for (int i = 0; i < 4; i++)
    {

        // square.setFillColor(Color::Green);
        rs.setPosition(x - (i * 20), 180);

        if (i == 0)
        {
            rs.setTexture(&texturaCabeza); // Textura de la cabeza
        }
        else if (i == 2)
        {
            rs.setTexture(&texturaCola); // Textura de la cola
        }
        else
        {
            rs.setTexture(&texturaCuerpo); // Textura del cuerpo
        }

        snake.push_back(rs);
    }

    //-------------------------------------------------------------------------
    // creamos los bordes
    Color colorBorde(14, 129, 14);
    RectangleShape bordeIzquierdo(Vector2f(20, ALTO_VENT));
    bordeIzquierdo.setFillColor(colorBorde);
    bordeIzquierdo.setPosition(0, 0);

    RectangleShape bordeDerecho(Vector2f(20, ALTO_VENT));
    bordeDerecho.setFillColor(colorBorde);
    bordeDerecho.setPosition(ANCHO_VENT - 20, 0);

    RectangleShape bordeInferior(Vector2f(ANCHO_VENT, 20));
    bordeInferior.setFillColor(colorBorde);
    bordeInferior.setPosition(0, ALTO_VENT - 20);

    RectangleShape bordeSuperior(Vector2f(ANCHO_VENT, 20));
    bordeSuperior.setFillColor(colorBorde);
    bordeSuperior.setPosition(0, 0);

    //-------------------------------------------------------------------------
    // mostrar los puntos
    // Agregar un mensaje de puntos
    Font fuente;
    fuente.loadFromFile("../recursos/TT Octosquares Trial Regular.ttf");
    Text textoPuntos;
    textoPuntos.setFont(fuente);
    textoPuntos.setCharacterSize(20);
    textoPuntos.setFillColor(Color::White);
    textoPuntos.setPosition(10, 0);
    textoPuntos.setString("Puntos: 0");

    Text textoInicio;
    textoInicio.setFont(fuente);
    textoInicio.setCharacterSize(30);
    textoInicio.setFillColor(Color::Black);
    textoInicio.setString("Bienvenido a Snake!\nPresiona ESPACIO para jugar.");
    textoInicio.setPosition((ANCHO_VENT - textoInicio.getLocalBounds().width) / 2, (ALTO_VENT - textoInicio.getLocalBounds().height) / 2);

    Text textoPerdiste;
    textoPerdiste.setFont(fuente);
    textoPerdiste.setCharacterSize(30);
    textoPerdiste.setFillColor(Color::Red);
    textoPerdiste.setString("Perdiste!\nPresiona ESPACIO para reiniciar.");
    textoPerdiste.setPosition((ANCHO_VENT - textoPerdiste.getLocalBounds().width) / 2, (ALTO_VENT - textoPerdiste.getLocalBounds().height) / 2);
    //-------------------------------------------------------------------------
    // declaramos las variables
    Direccion direccion = RIGHT;
    int puntos = 0;
    bool seMovio = true, haMovido = false;
    bool juego = true;
    bool juegoIniciado = false;
    bool chocoConBorde = false;
    float xCuadrado = 1.0, yCuadrado = 1;
    bool derecha = true, abajo = true;
    bool mostrarCuadrado = false;
    //-------------------------------------------------------------------------

    // Los diferenciales de movimiento (cuanto avanza el cuadrado) en los ejes x,y
    Vector2 diff = {VELOCIDAD, VELOCIDAD};
    //-------------------------------------------------------------------------
    int contFrame = 0;
    //-------------------------------------------------------------------------
    Texture texture;
    !texture.loadFromFile(PATH + "f3.png") && cout << "error en carga de imagen desde disco";
    // Crea un sprite y le asigna la textura cargada
    Sprite sprite(texture);
    sprite.setTextureRect(IntRect(0, 0, ANCHO_VENT, ALTO_VENT));

    //-------------------------------------------------------------------------
    int cantFrame = 8;
    int delay = 6;
    //-------------------------------------------------------------------------
    bool terminoJuego = false;
    //-------------------------------------------------------------------------
    // se abre la ventana
    while (ventana.isOpen())
    {

        if (contFrame == 60)
        {
            contFrame = 0;
        }
        else
        {
            contFrame++;
        }

        if (terminoJuego)
        {
            ventana.close();
        }

        Event evento;
        while (ventana.pollEvent(evento))
        {
            if (evento.type == sf::Event::Closed)
            {
                ventana.close();
            }
            else if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Space)
            {
                if (!juego) // si está en "game over"
                {
                    // reset flags
                    juegoIniciado = true;
                    juego = true;
                    chocoConBorde = false;
                    direccion = RIGHT;
                    puntos = 0;

                    // reset snake a 3 segmentos
                    while (snake.size() > 4)
                        snake.pop_back();

                    int x = ANCHO_VENT / 2;
                    for (int i = 0; i < 4; i++)
                    {
                        snake[i].setPosition(x - (i * 20), 180);

                        if (i == 0)
                            snake[i].setTexture(&texturaCabeza);
                        else if (i == 3)
                            snake[i].setTexture(&texturaCola);
                        else
                            snake[i].setTexture(&texturaCuerpo);
                    }

                    // reset arbusto
                    arbusto.setPosition(arbustoPosInicial);
                    delay = 6;
                }
            }
        }

        if (!juego)
        {
            ventana.clear();
            ventana.draw(sprite);
            ventana.draw(textoPerdiste);
            ventana.display();
            continue;
        }

        // cuando el juego empieza, muestre la pantalla de inicio
        if (!juegoIniciado)
        {
            ventana.clear();
            ventana.draw(sprite);
            if (juego)
            {
                ventana.draw(textoInicio);
            }

            ventana.display();

            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                juegoIniciado = true;
                puntos = 0;
                juego = true;
                chocoConBorde = false;
            }
            continue;
        }

        //-------------------------------------------------------------------------
        // movimiento de la serpiente
        Direccion nuevaDireccion = direccion;

        if (Keyboard::isKeyPressed(Keyboard::Right) && direccion != LEFT)
        {
            nuevaDireccion = RIGHT;
            haMovido = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down) && direccion != UP)
        {
            nuevaDireccion = DOWN;
            haMovido = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left) && direccion != RIGHT)
        {
            nuevaDireccion = LEFT;
            haMovido = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up) && direccion != DOWN)
        {
            nuevaDireccion = UP;
            haMovido = true;
        }
        cargarTextura(nuevaDireccion, snake);

        if (nuevaDireccion != direccion)
        {
            direccion = nuevaDireccion;
        }

        if (haMovido && contFrame % delay == 0)
        {
            moverSnake(direccion, snake);
            // haMovido = false; // Resetear la bandera
        }

        //-------------------------------------------------------------------------
        // movimiento aleatorio de la "manzana"
        if (snake[0].getGlobalBounds().intersects(forma.getGlobalBounds()))
        {
            // Reposicionar manzana de forma segura
            recolocarManzana(forma, arbusto, snake, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior);

            // Crear nuevo segmento
            rectacgleSnake nuevoSegmento;

            // Asignar la textura de cola al nuevo segmento
            int posVecTextCola = posCola(snake[snake.size() - 2].getPosition(), snake[snake.size() - 3].getPosition());
            nuevoSegmento.setTexture(&textCola[posVecTextCola]);


            // Posicionar el nuevo segmento donde estaba el anterior (cola actual)
            nuevoSegmento.setPosition(snake[snake.size() - 1].getPosition());


            // Asignar la textura de cuerpo al segmento que era la cola
            snake[snake.size() -1].setTexture(&texturaCuerpo);

            // Agregar el nuevo segmento a la serpiente
            snake.push_back(nuevoSegmento);



            sonidoCome.play();
            // Incrementar los puntos
            puntos++;
            textoPuntos.setString("Puntos: " + to_string(puntos));

            delay = max(2, 6 - (puntos / 20));
            //cout << "Delay: " << delay << endl;

        }
        //-------------------------------------------------------------------------
        // que pasa si el snake choca con el borde
        if (colisionConBordesSnake(snake, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior) || colisionConCuerpoSnake(snake))
        {
            chocoConBorde = true;
            puntos = 0;
            textoPuntos.setString("Puntos: " + to_string(puntos));
        }

        if (chocoConBorde)
        {
            juego = false;
            juegoIniciado = false;
        }

        //-------------------------------------------------------------------------
        // si la snake choca con el arbusto
        if (colisionArbustoSnake(arbusto, snake))
        {
            puntos = 0;
            textoPuntos.setString("Puntos: " + to_string(puntos));

            for (int i = 4; i < snake.size(); i++)
            {
                snake.pop_back();
            }

            int x = (ANCHO_VENT / 2) - 20;
            for (int i = 0; i <= snake.size(); i++)
            {
                snake[i].setPosition(x, 180);
                x = x - 5;
            }
        }

        //-------------------------------------------------------------------------
        // movimiento del arbusto
        if (puntos % 5 == 0 && puntos > 0 && seMovio)
        {
            do
            {
                arbusto.setPosition(rand() % ANCHO_VENT, rand() % ALTO_VENT);
            } while (colisionConBordesRectangulo(arbusto, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior) ||
                     colisionArbustoSnake(arbusto, snake) ||
                     colisionConArbustoManzana(arbusto, forma));
            seMovio = false;
        }

        if (puntos % 5 != 0)
        {
            seMovio = true;
        }
        //-------------------------------------------------------------------------


        //-------------------------------------------------------------------------
        // que la "manzana" no este donde esta el arbusto
        if (forma.getGlobalBounds().intersects(arbusto.getGlobalBounds()) ||
            colisionConBordesForma(forma, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior) || colisionManzanaSnake(forma, snake))

        {
            // Cambiar la posición de la forma a una nueva posición aleatoria
            float x;
            float y;
            do
            {
                x = 20 + rand() % (ANCHO_VENT - 40); // evita los bordes izquierdo y derecho
                y = 20 + rand() % (ALTO_VENT - 40);  // evita los bordes superior e inferior
                forma.setPosition(x, y);
                forma.setPosition(rand() % ANCHO_VENT + 20, rand() % ALTO_VENT);

            } while (colisionConArbustoManzana(arbusto, forma) ||
                     colisionConBordesForma(forma, bordeIzquierdo, bordeDerecho, bordeInferior, bordeSuperior) || colisionManzanaSnake(forma, snake));
        }

        //-------------------------------------------------------------------------
        // dibuja todos los objetos necesarios
        ventana.clear();
        ventana.draw(sprite);
        ventana.draw(bordeIzquierdo);
        ventana.draw(bordeDerecho);
        ventana.draw(bordeInferior);
        ventana.draw(bordeSuperior);
        ventana.draw(arbusto);
        ventana.draw(textoPuntos);



        for (int i = 0; i < snake.size() - 1; i++)
        {
            ventana.draw(snake[i]);
        }
        ventana.draw(forma);
        ventana.display();
    }
}

bool colisionConBordesForma(const CircleShape &forma, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior)
{
    return forma.getGlobalBounds().intersects(bordeIzquierdo.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeInferior.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeDerecho.getGlobalBounds()) ||
           forma.getGlobalBounds().intersects(bordeSuperior.getGlobalBounds());
}

bool colisionConBordesSnake(const vector<rectacgleSnake> &snake, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior)
{
    return snake[0].getGlobalBounds().intersects(bordeIzquierdo.getGlobalBounds()) ||
           snake[0].getGlobalBounds().intersects(bordeDerecho.getGlobalBounds()) ||
           snake[0].getGlobalBounds().intersects(bordeInferior.getGlobalBounds()) ||
           snake[0].getGlobalBounds().intersects(bordeSuperior.getGlobalBounds());
}

bool colisionConBordesRectangulo(const RectangleShape &bloque, const RectangleShape &bordeIzquierdo, const RectangleShape &bordeDerecho, const RectangleShape &bordeInferior, const RectangleShape &bordeSuperior)
{
    return bloque.getGlobalBounds().intersects(bordeIzquierdo.getGlobalBounds()) ||
           bloque.getGlobalBounds().intersects(bordeDerecho.getGlobalBounds()) ||
           bloque.getGlobalBounds().intersects(bordeInferior.getGlobalBounds()) ||
           bloque.getGlobalBounds().intersects(bordeSuperior.getGlobalBounds());
}

void moverSnake(Direccion &direccion, vector<rectacgleSnake> &snake)
{
    // Guarda la posición actual de la cabeza de la serpiente
    Vector2f cabezaPos = snake[0].getPosition();

    // Mueve la cabeza en la dirección deseada
    switch (direccion)
    {
    case UP:
        cabezaPos.y -= MOVIMIENTO;

        break;
    case DOWN:
        cabezaPos.y += MOVIMIENTO;

        break;
    case LEFT:
        cabezaPos.x -= MOVIMIENTO;

        break;
    case RIGHT:
        cabezaPos.x += MOVIMIENTO;

        break;
    }

    for (int i = snake.size() - 1; i > 0; --i)
    {

        Vector2f posAnterior = snake[i - 1].getPosition();
        snake[i].setPosition(posAnterior);
    }

    snake[0].setPosition(cabezaPos);
    cargarTextura(direccion, snake);
}

bool colisionArbustoCabezaSnake(const RectangleShape &arbusto, const rectacgleSnake &cabeza)
{
    return cabeza.getGlobalBounds().intersects(arbusto.getGlobalBounds());
}

bool colisionConArbustoManzana(const RectangleShape &arbusto, const CircleShape &forma)
{
    return forma.getGlobalBounds().intersects(arbusto.getGlobalBounds());
}

int posCola(Vector2f actual, Vector2f anterior)
{
    int pos = -1;
    if (actual.x < anterior.x)
    {
        pos = 1;
    }
    else if (actual.x > anterior.x)
    {
        pos = 3;
    }
    else if (actual.y > anterior.y)
    {
        pos = 0;
    }
    else if (actual.y < anterior.y)
    {
        pos = 2;
    }

    return pos;
}

int posCurva(Vector2f anterior, Vector2f actual, Vector2f siguiente)
{
    int pos = -1;
    if (anterior.x < actual.x)
    { // derecha
        if (actual.y < siguiente.y)
        {
            // derecha abajo
            pos = 1;
        }
        else
        {
            // derecha arriba
            pos = 3;
        }
    }
    else if (anterior.x > actual.x) // iqz
    {
        if (actual.y < siguiente.y)
        {
            // izq abajo
            pos = 0;
        }
        else
        {
            // izq arriba
            pos = 2;
        }
    }
    else if (anterior.y < actual.y)
    { // abajo
        if (actual.x < siguiente.x)
        {
            // abajo derecha
            pos = 2;
        }
        else
        {
            // abajo izquierda
            pos = 3;
        }
    }
    else if (anterior.y > actual.y)
    {
        if (actual.x < siguiente.x)
        {
            // arriba derecha
            pos = 0;
        }
        else
        {
            // arriba izquierda
            pos = 1;
        }
    }

    return pos;
}

bool colisionConCuerpoSnake(vector<rectacgleSnake> &snake)
{
    bool choco = false;
    for (size_t i = 1; i < snake.size(); ++i)
    {
        if (snake[0].getGlobalBounds().intersects(snake[i].getGlobalBounds()))
        {
            choco = true;
        }
    }
    return choco;
}

void cargarTextura(Direccion &direccion, vector<rectacgleSnake> &snake)
{
    switch (direccion)
    {
    case UP:

        texturaCabeza.loadFromFile(PATH + "cabezaArriba.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    case DOWN:
        texturaCabeza.loadFromFile(PATH + "cabezaAbajo.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    case LEFT:
        texturaCabeza.loadFromFile(PATH + "cabezaIzq.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    case RIGHT:
        texturaCabeza.loadFromFile(PATH + "cabezaDerecha.png");
        snake[0].setTexture(&texturaCabeza);

        break;
    }

    textDoblar[0].loadFromFile(PATH + "cuerpo_upRight.png");
    textDoblar[1].loadFromFile(PATH + "cuerpo_upLeft.png");
    textDoblar[2].loadFromFile(PATH + "cuerpo_downRight.png");
    textDoblar[3].loadFromFile(PATH + "cuerpo_downLeft.png");

    for (int i = snake.size() - 1; i > 0; --i)
    {

        Vector2f posAnterior = snake[i - 1].getPosition();
        if ((posAnterior.x != snake[i].getPosition().x))
        {
            snake[i].setTexture(&texCuerpoHorizontal);
        }
        else if ((posAnterior.y != snake[i].getPosition().y))
        {
            snake[i].setTexture(&texCuerpoVertical);
        }
    }

    for (int i = snake.size() - 2; i > 0; --i)
    {

        Vector2f posAnterior = snake[i + 1].getPosition();
        Vector2f posActual = snake[i].getPosition();
        Vector2f posSiguiente = snake[i - 1].getPosition();
        if (posAnterior.x != posSiguiente.x && posAnterior.y != posSiguiente.y)
        {
            int curvaPos = posCurva(posAnterior, posActual, posSiguiente);
            if (curvaPos != -1)
            {
                snake[i].setTexture(&textDoblar[curvaPos]);
            }
        }
    }

    int posVecTextCola;
    posVecTextCola = posCola(snake[snake.size() - 2].getPosition(), snake[snake.size() - 3].getPosition());
    snake[snake.size() - 2].setTexture(&textCola[posVecTextCola]);
}

bool colisionArbustoSnake(const RectangleShape &arbusto, const vector<rectacgleSnake> &snake)
{
    for (const auto &segmento : snake)
    {
        if (segmento.getGlobalBounds().intersects(arbusto.getGlobalBounds()))
        {
            return true;
        }
    }
    return false;
}

bool colisionManzanaSnake(const CircleShape &manzana, const vector<rectacgleSnake> &snake)
{
    for (const auto &segmento : snake)
    {
        if (manzana.getGlobalBounds().intersects(segmento.getGlobalBounds()))
        {
            return true;
        }
    }
    return false;
}

void recolocarManzana(CircleShape &manzana,
                      const RectangleShape &arbusto,
                      const vector<rectacgleSnake> &snake,
                      const RectangleShape &bordeIzq,
                      const RectangleShape &bordeDer,
                      const RectangleShape &bordeInf,
                      const RectangleShape &bordeSup)
{
    float x, y;
    float ancho = manzana.getRadius() * 2;
    float alto = manzana.getRadius() * 2;

    do
    {
        x = 20 + rand() % int(ANCHO_VENT - ancho - 40);
        y = 20 + rand() % int(ALTO_VENT - alto - 40);
        manzana.setPosition(x, y);
    } while (
        colisionConBordesForma(manzana, bordeIzq, bordeDer, bordeInf, bordeSup) ||
        colisionManzanaSnake(manzana, snake) ||
        colisionConArbustoManzana(arbusto, manzana));
}
