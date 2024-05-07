# Plantilla de Proyecto CMake para SFML de UTN-INSPT

Esta plantilla de repositorio debería permitir un inicio rápido y sin complicaciones de tu próximo proyecto de SFML utilizando CMake. Gracias a la [naturaleza de plantillas de GitHub](https://docs.github.com/es/repositories/creating-and-managing-repositories/creating-a-repository-from-a-template), puedes bifurcar este repositorio sin heredar su historial de Git.

La plantilla comienza siendo muy básica, pero podría recibir características adicionales con el tiempo:

- Script básico de CMake para construir tu proyecto y enlazar SFML en cualquier sistema operativo.
- Script básico de [GitHub Actions](https://github.com/features/actions) para todas las plataformas principales.

## Cómo Usar

1. Sigue las instrucciones anteriores sobre cómo usar la función de plantilla de proyectos de GitHub para crear tu propio proyecto.
2. Abre [CMakeLists.txt](CMakeLists.txt). Renombra el proyecto y el ejecutable con el nombre que desees. Los nombres del proyecto y del ejecutable no tienen que coincidir.
3. Si usas Linux, instala las dependencias de SFML usando el gestor de paquetes de tu sistema. En Ubuntu y otras distribuciones basadas en Debian, puedes usar los siguientes comandos:
    ```
    sudo apt update
    sudo apt install \
        libxrandr-dev \
        libxcursor-dev \
        libudev-dev \
        libfreetype-dev \
        libopenal-dev \
        libflac-dev \
        libvorbis-dev \
        libgl1-mesa-dev \
        libegl1-mesa-dev
    ```
4. Configura y construye tu proyecto. La mayoría de los IDE populares admiten proyectos de CMake con muy poco esfuerzo de tu parte.
    - [VS Code](https://code.visualstudio.com) a través de la [extensión CMake](https://code.visualstudio.com/docs/cpp/cmake-linux)
    - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
    - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
    - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

    Usar CMake desde la línea de comandos también es directo.

    Para un generador de configuración única (típicamente el caso en Linux y macOS):
    ```
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    ```

    Para un generador de configuración múltiple (típicamente el caso en Windows):
    ```
    cmake -S . -B build
    cmake --build build --config Release
    ```
5. ¡Disfruta!

## Actualización de SFML

SFML se encuentra a través del módulo [FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) de CMake. FetchContent descarga automáticamente SFML desde GitHub y lo construye junto con tu propio código. Más allá de la conveniencia de no tener que instalar SFML tú mismo, esto garantiza la compatibilidad ABI y simplifica cosas como especificar bibliotecas estáticas versus compartidas.

Modificar la versión de SFML que deseas es tan fácil como cambiar el argumento [`GIT_TAG`](CMakeLists.txt#L7). Actualmente utiliza la última versión en desarrollo de SFML 2 a través de la etiqueta `2.6.x`. Si te sientes aventurero y quieres probar SFML 3, usa la etiqueta `master`. ¡Ten cuidado, esto requiere cambiar tu código para adaptarlo a la API modificada! Las amables personas en la [comunidad de SFML](https://github.com/SFML/SFML#community) pueden ayudarte con esa transición y los errores que puedas encontrar en el camino.

## Pero yo quiero...

Modificar opciones de CMake agregándolas como parámetros de configuración (con una bandera `-D`) o modificando el contenido de CMakeCache.txt y reconstruyendo.

### Usar Bibliotecas Estáticas

Por defecto, SFML construye bibliotecas compartidas y este valor por defecto se hereda en tu proyecto. La opción [`BUILD_SHARED_LIBS`](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html) de CMake te permite elegir bibliotecas estáticas o compartidas para todo el proyecto.

### Cambiar Compiladores

Consulta la variedad de opciones [`CMAKE_<LANG>_COMPILER`](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER.html). En particular, querrás modificar `CMAKE_CXX_COMPILER` para que apunte al compilador de C++ que deseas utilizar.

### Cambiar Optimizaciones del Compilador

CMake abstrae los flags de optimización específicos a través de la opción [`CMAKE_BUILD_TYPE`](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html). Por defecto, este proyecto recomienda compilaciones `Release` que habilitan optimizaciones. Otros tipos de compilación incluyen compilaciones `Debug` que habilitan símbolos de depuración pero deshabilitan las optimizaciones. Si estás utilizando un generador de configuración múltiple (como suele ser el caso en Windows), puedes modificar la opción [`CMAKE_CONFIGURATION_TYPES`](https://cmake.org/cmake/help/latest/variable/CMAKE_CONFIGURATION_TYPES.html#variable:CMAKE_CONFIGURATION_TYPES).

### Cambiar Generadores

Si bien CMake intentará seleccionar un generador adecuado por defecto, algunos sistemas ofrecen una variedad de generadores para elegir. Ubuntu, por ejemplo, ofrece Makefiles y Ninja como dos opciones potenciales. Para obtener una lista de generadores, haz clic [aquí](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html). Para modificar el generador que estás utilizando, debes reconfigurar tu proyecto proporcionando una bandera `-G` con un valor correspondiente al generador que deseas. No puedes simplemente modificar una entrada en el archivo CMakeCache.txt a diferencia de las opciones anteriores. Luego puedes reconstruir tu proyecto con este nuevo generador.

## Más Lectura

Aquí tienes algunos recursos útiles si deseas aprender más sobre CMake:

- [Cómo Usar CMake Sin el Dolor Agonizante - Parte 1](https://alexreinking.com/blog/how-to-use-cmake-without-the-agonizing-pain-part-1.html)
- [Cómo Usar CMake Sin el Dolor Agon

izante - Parte 2](https://alexreinking.com/blog/how-to-use-cmake-without-the-agonizing-pain-part-2.html)
- [Serie de YouTube Mejor CMake por Jefferon Amstutz](https://www.youtube.com/playlist?list=PL8i3OhJb4FNV10aIZ8oF0AA46HgA2ed8g)

## Licencia

El código fuente tiene licencia dual bajo Dominio Público y MIT, elige la que prefieras.
