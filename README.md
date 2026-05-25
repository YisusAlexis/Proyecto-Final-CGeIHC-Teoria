# Proyecto Final: Entorno 3D Interactivo en OpenGL

¡Bienvenido al repositorio de nuestro proyecto final de **Computación Gráfica**! Este es un entorno tridimensional interactivo desarrollado desde cero utilizando el pipeline moderno de OpenGL. El proyecto integra animaciones jerárquicas, simulaciones físicas, iluminación dinámica y escenarios temáticos totalmente explorables.

---

## ⚠️ NOTA IMPORTANTE (Descarga de Recursos)

> 📢 **¡ATENCIÓN!** Debido al tamaño y peso de los archivos de geometría y mapas de bits, los modelos 3D (archivos `.obj`) y sus respectivas texturas **no se encuentran incluidos directamente en este repositorio** para mantenerlo ligero.
>
> Para poder compilar y visualizar la escena correctamente, debes descargar los recursos desde el siguiente enlace de Google Drive y colocarlos dentro de la carpeta correspondiente del proyecto (ej. `assets/` o `models/`):
>
> 🔗 **Enlace de descarga en Google Drive:** [PEGA_AQUÍ_TU_LIGA_DE_DRIVE]

---

## 🚀 Características Principales

* **Renderizado Eficiente:** Uso de shaders modernos (Vertex y Fragment Shaders) con carga dinámica de modelos en formato `.obj`.
* **Iluminación Avanzada:** Implementación de múltiples fuentes de luz en tiempo real, incluyendo luces ambientales, direccionales, luces posicionales (Point Lights) y reflectores (Spotlights) dinámicos.
* **Físicas e Interactividad:** Simulación de movimiento parabólico (ej. lanzamiento de catapulta) y colisiones básicas.
* **Animación Jerárquica:** Modelos complejos articulados con transformaciones matemáticas en cascada (como helicópteros y vehículos en movimiento).
* **Cámaras Dinámicas:** Sistema de cámaras múltiples (cámara libre en primera persona y cámaras fijas/cinemáticas sincronizadas con el movimiento).

---

## 🛠️ Tecnologías y Herramientas

El proyecto fue desarrollado utilizando el siguiente stack técnico:

* **Lenguaje:** C++
* **API Gráfica:** OpenGL (Modern Pipeline / Core Profile)
* **Matemáticas Gráficas:** [GLM (OpenGL Mathematics)](https://github.com/g-truc/glm) para matrices de transformación, vectores y proyecciones.
* **Entorno de Desarrollo (IDE):** Visual Studio Community (2022 o superior).
* **Formatos Soportados:** Modelos 3D en formato `.obj` y texturas procesadas mediante software de edición (Blender / GIMP).

---

## 🎮 Controles del Sistema

Para interactuar con el entorno y explorar la escena, utiliza los siguientes comandos del teclado y mouse:

### Movimiento de Cámara
| Tecla / Acción | Función |
| :--- | :--- |
| `W`, `A`, `S`, `D` | Desplazamiento de la cámara (Adelante, Izquierda, Atrás, Derecha) |
| `Movimiento del Mouse` | Orientación de la vista (Pitch y Yaw) |

### Iluminación e Interacciones
| Tecla / Acción | Función |
| :--- | :--- |
| `L` | Alternar encendido/apagado de las luces generales de la escena |
| `F` | Activar/desactivar linterna (Spotlight) acoplada a la cámara/vehículo |
| `1`, `2`, `3` | Cambiar entre los diferentes modos de cámara disponibles |

*(Nota: Los controles de animación de vehículos como el helicóptero adaptan automáticamente sus luces frontales y traseras según la dirección de la marcha).*

---

## 📦 Requisitos e Instalación

Para ejecutar este proyecto de forma local en **Visual Studio Community**, sigue estos pasos:

1.  **Clonar el repositorio:**
    ```bash
    git clone [https://github.com/tu-usuario/tu-repositorio.git](https://github.com/tu-usuario/tu-repositorio.git)
    ```
2.  **Descargar los assets:** Descarga los modelos y texturas desde la nota importante del inicio y colócalos en la ruta interna estructurada del proyecto.
3.  **Abrir el proyecto:**
    * Abre Visual Studio Community.
    * Selecciona `Archivo` > `Abrir` > `Proyecto o Solución` y busca el archivo `.sln` del proyecto.
4.  **Configuración de Dependencias:**
    * Asegúrate de que las rutas de inclusión (`Include`) y librerías (`Lib`) para **GLFW**, **GLEW/GLAD** y **GLM** estén correctamente enlazadas en las propiedades del proyecto de Visual Studio.
    * Configura la arquitectura de compilación en **x86** o **x64** según corresponda a tus binarios enlazados.
5.  **Compilar y Ejecutar:**
    * Presiona `F5` o haz clic en el botón de **Iniciar Depuración**.

---

## 👥 Desarrollado por

* **Alexis Perez** 

---
