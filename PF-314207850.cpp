
//----------------------------------------------------------------------|
//FACULTAD DE INGENIERIA UNAM
//COMPUTACION GRAFICA E INTERACCION HUMANO COMPUTADORA - Teoria
//2026-2
//PROYECTO FINAL
//25-mayo-2026
//PROFESOR: José Roque Román Guadarrama
//INTEGRANTES:
//------------Pérez León Jesús Alexis
//----------------------------------------------------------------------|
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Skybox.h"
#include "Model.h"

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

#include <iostream>
#include <direct.h>  // Para _getcwd en Windows
// ==========================================
// para OpenAl Audio
// ==========================================
#include <AL/al.h>
#include <AL/alc.h>

#include <fstream>
#include <cstring>
// =================================================================
// VARIABLES GLOBALES PARA EL CONTROL DE AUDIO (OPENAL)
// =================================================================
ALCdevice* audioDevice = nullptr; // Representa tu tarjeta de sonido/audífonos
ALCcontext* audioContext = nullptr; // Representa el entorno acústico del juego

//// Identificadores para los buffers y bocinas de tu tarea
ALuint bufferSoundtrack;
ALuint fuenteSoundtrack;





const float toRadians = 3.14159265f / 180.0f;

//---------------------------------------------------------------------------
//variables animacion humo mascara aku
float rotHumoAku = 0;
float creceHumoAltoAku = 0;
float creceHumoAnchoAku = 0.0f;
//VARIABLES PARA EL CONTROL DE LA PISTA Y LOS VEHICULOS QUE LA RECORREN
//variables animacion humo carro1 (auto donde viaja crash y grunt)
//tipo escape de carro
float creceHumoCar1 = 0.0f;
float rotHumoCar1 = 0.0f;
//movimiento auto y llantas
float rotLlantaCar1 = 0.0f;
// offset´s carro1 (auto donde viaja crash y grunt)
float offsetCentroPistaCar1 = -2.3f;
float offsetAlturaPistaCar1 = 0.0f;
float velocidadHumoCar1;

//---------------------------------------------------------------------------
//variables animacion humo carro2 (auto donde viaja masterchief y coco)
//tipo escape de carro
float creceHumoCar2 = 0.0f;
float rotHumoCar2 = 0.0f;
//movimiento auto y llantas
float rotLlantaCar2 = 0.0f;
// offset´s carro2 (auto donde viaja masterchief y coco)
float offsetCentroPistaCar2 = 1.9f;
float offsetAlturaPistaCar2 = -0.3f;
//---------------------------------------------------------------------------
//variables animacion humo carro3 (moto donde viaja harry potter)
//tipo escape de carro
float creceHumoCar3 = 0.0f;
float rotHumoCar3 = 0.0f;
//movimiento auto y llantas
float rotLlantaCar3 = 0.0f;
// offset´s carro2 (moto donde viaja harry potter)
float offsetCentroPistaCar3 = 0.1f;
float offsetAlturaPistaCar3 = 2.0f;

// Datos para manejo de puntos de la pista y movimiento del auto
int i = 0;           // Índice del punto actual
float t = 0.0f;      // Progreso entre punto i e i+1 (0.0 a 1.0)
float velocidad = 0.5f;
float distancia;
float velGral = 0.0f;
// Variables para el Translate y Rotate del auto
glm::vec3 posCars = glm::vec3(-41.6996f, 0.0f, 0.0f);
float angleY = 0.0f; // YAW (giro horizontal)
float angleX = 0.0f; // PITCH (si sube o baja una rampa)
//float angleZ = 0.0f; // ROLL (giro lateral)

bool animPista = false;

//VARIABLES ANIMACION CRASH
//variavles animacion tornado crash
bool girandoTornado = false;
float rotacionTornado = 0.0f;
float velocidadGiro = 1000.0f; // Ajusta qué tan rápido gira
int crashEstadoAnimacion = 0;

// --- SISTEMA DE ESTADOS ---
enum EstadoAnimacion { REPOSO, TORNADO_CRASH, ROCA_RODANDO };
EstadoAnimacion estadoActual = REPOSO;

// --- POSICIONES X DE LAS CAJAS ---
// Las alineamos en el eje X con un espacio de 4 unidades entre cada una
float inicioX = -40.0f;
float paso = 10.0f;

// Variables para el movimiento libre de la cámara aérea (Opción 2)
//float aeroCamX = 45.0f;       // Posición inicial X (puedes ajustarla al centro de tu mapa)
//float aeroCamZ = -45.0f;      // Posición inicial Z
//float aeroCamAltura = 200.0f;  // Altura fija desde la que mirará hacia abajo
//float aeroCamVel = 25.0f;     // Velocidad de desplazamiento XZ de la cámara aérea

glm::vec3 posCajaAku = glm::vec3(inicioX, -2.0f, 80.0f);
glm::vec3 posCajaCaraCrash = glm::vec3(inicioX + paso, -2.0f, 80.0f);
glm::vec3 posCajaCheckPoint = glm::vec3(inicioX + paso * 2, -2.0f, 80.0f);
glm::vec3 posCajaFlechaArriba = glm::vec3(inicioX + paso * 3, -2.0f, 80.0f);
glm::vec3 posCajaMetalExcl = glm::vec3(inicioX + paso * 4, -2.0f, 80.0f);
glm::vec3 posCajaNitro = glm::vec3(inicioX + paso * 5, -2.0f, 80.0f);
glm::vec3 posCajaNitroExcl = glm::vec3(inicioX + paso * 6, -2.0f, 80.0f);
glm::vec3 posCajaNormal = glm::vec3(inicioX + paso * 7, -2.0f, 80.0f);
glm::vec3 posCajaQuestion = glm::vec3(inicioX + paso * 8, -2.0f, 80.0f);
glm::vec3 posCajaTNT = glm::vec3(inicioX + paso * 9, -2.0f, 80.0f);
glm::vec3 posTotem1Crash = glm::vec3(inicioX + paso * 11, -2.0f, 80.0f);

// --- ESCALAS EN Y (Dinámicas) ---
float escAku = 5.0f, escCara = 5.0f, escCheck = 5.0f, escFlecha = 5.0f, escMetal = 5.0f;
float escNitro = 5.0f, escNitroExcl = 5.0f, escNormal = 5.0f, escQuest = 5.0f, escTNT = 5.0f;

// --- VARIABLES DE LA ROCA ---
glm::vec3 posBolaRocaInicial = glm::vec3(-55.0f, 4.0f, 80.0f); // Empieza antes que las cajas
glm::vec3 posBolaRoca = posBolaRocaInicial;
float rotacionRocaZ = 0.0f;
float velocidadRoca = 8.0f;

// //----------------------------------------------------------------------------------------------------------
//VARIABLES PARA RUEDAS EXPRESO HOWARTS
float angleAccumulator = 0.0f;
float wheelSpeed = 0.0f; // Velocidad de giro (radianes o grados por segundo)
float rotRuedaCabinaHowarts = 0.0f;

//VARIABLES HUMO TREN HOWARTS
float rotHumoTren = 0.0f;
float creceHumoAltoTren = 1.0f;

//VARIABLES ANIMACIO EXPRESSO DE HOWARTS POR KEYFRAMES
#include <vector>

// 1. Keyframes del Camino Mágico (Vértices del cuadrado flotante en tu escena)
// Modifiqué ligeramente las alturas (Y) para que suba y baje suavemente al volar
std::vector<glm::vec3> keyframesCamino = {
    // === Tramo Esquina 1 (Abajo Izquierda) ===
    glm::vec3(-160.0f, 50.0f,  160.0f), // Punto base de la Esquina 1
    glm::vec3(0.0f,  67.5f,  160.0f), // Punto medio hacia Esquina 2 (Suaviza la recta frontal)

    // === Tramo Esquina 2 (Abajo Derecha) ===
    glm::vec3(160.0f, 85.0f,  160.0f), // Punto base de la Esquina 2
    glm::vec3(160.0f, 57.5f,    0.0f), // Punto medio hacia Esquina 3 (Suaviza la caída lateral)

    // === Tramo Esquina 3 (Arriba Derecha) ===
    glm::vec3(160.0f,  30.0f, -160.0f), // Punto base de la Esquina 3
    glm::vec3(0.0f, 65.0f, -160.0f), // Punto medio hacia Esquina 4 (Suaviza la subida trasera)

    // === Tramo Esquina 4 (Arriba Izquierda) ===
    glm::vec3(-160.0f, 100.0f, -160.0f), // Punto base de la Esquina 4
    glm::vec3(-160.0f, 75.0f,    0.0f)  // Punto medio hacia Esquina 1 (Cierra el ciclo de manera fluida)
};

// 2. Variables de control de tiempo para la animación
float tiempoGlobalTren = 0.0f;
float velocidadTren = 0.05f;       // Qué tan rápido recorre el circuito completo
float desfaseEntreVagones = 0.03f; // Distancia de separación temporal en la curva

// 3. Función matemática Spline Catmull-Rom para suavizar las esquinas del cuadrado
glm::vec3 InterpolacionCatmullRom(const std::vector<glm::vec3>& puntos, float t) {
    int numPuntos = puntos.size();

    // Mapeamos t global (0.0 a 1.0) al tramo de la curva correspondiente
    float t_escalado = t * numPuntos;
    int i = (int)t_escalado;
    float local_t = t_escalado - i;

    // Buscamos los 4 puntos vecinos con comportamiento circular (wrap-around)
    glm::vec3 p0 = puntos[(i - 1 + numPuntos) % numPuntos];
    glm::vec3 p1 = puntos[i % numPuntos];
    glm::vec3 p2 = puntos[(i + 1) % numPuntos];
    glm::vec3 p3 = puntos[(i + 2) % numPuntos];

    // Fórmula cúbica estándar de Catmull-Rom
    return 0.5f * (
        (2.0f * p1) +
        (-p0 + p2) * local_t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * local_t * local_t +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * local_t * local_t * local_t
        );
}
//----------------------------------------------------------------------------------------------------------

//VARIABLES ENCENDIDO Y APAGADO DE LAS SPOTLIGHTS (ESTATUA ESCALERAS GIRATORIAS, CARRITOS EXCIBICION Y CRISTAL CRASH) CON TECLA "G"
bool fenixLightOn = true;  // Estado de la luz (empieza encendida)
bool gKeyWasPressed = false; // Bandera para rastrear el estado anterior de la tecla
int carritoIndex = 0;          // 0 = Coco, 1 = Crash, 2 = Cortex

//------------------------------------------------------------------------------------
// --- Variables del Ciclo Solar Automático ---
float tiempoCicloTotal = 120.0f; // El ciclo completo (Día + Noche + Transiciones) dura 2 minutos (120s)
float horaSimulada = 6.0f;       // Empezamos a las 6:00 AM (Amanecer)
float velocidadTiempo = 1.0f;    // Multiplicador de velocidad (1.0 = normal)

bool prevTeclaT = false;         // Tu tecla de depuración rápida

// ============================================
// DECLARACIÓN DE VARIABLES PARA CÁMARAS ESTÁTICAS (N)
// ============================================
std::vector<glm::vec3> hpCamEstaticaPos;    // Lista de posiciones de las cámaras fijas
std::vector<glm::vec3> hpCamEstaticaTarget; // Lista de objetivos a los que miran
int puntoInteresActual = 0;                 // Índice para saber qué cámara fija usar (0, 1 o 2)
bool prevTeclaN = false;                    // Flag para evitar que la tecla N se buclee al dejarla presionada

// Offset de toda la escena de Harry Potter en el mundo
glm::vec3 hpOffsetEscena = glm::vec3(55.0f, 0.0f, 25.0f);

std::uint8_t* CargarWav(const std::string& nombreArchivo, ALenum& formato, ALsizei& tamano, ALsizei& frecuencia) {
    // Intentar abrir el archivo de forma local directa
    std::ifstream archivo(nombreArchivo, std::ios::binary);

    // Si falla, intentar buscarlo con doble extensión por si Windows la duplicó
    if (!archivo.is_open()) {
        std::string rutaAlternativa = nombreArchivo + ".wav";
        archivo.open(rutaAlternativa, std::ios::binary);
        if (!archivo.is_open()) {
            std::cerr << "Error de lectura: No se pudo abrir el archivo: " << nombreArchivo << std::endl;
            return nullptr;
        }
        std::cout << "-> [Aviso] Se detecto doble extension en Windows. Cargando archivo alterno." << std::endl;
    }

    char idRiff[4];
    archivo.read(idRiff, 4);
    if (std::strncmp(idRiff, "RIFF", 4) != 0) {
        std::cerr << "Error de formato: El archivo no es un WAV RIFF valido." << std::endl;
        return nullptr;
    }

    archivo.seekg(8); // Saltar tamaño del archivo
    char idWave[4];
    archivo.read(idWave, 4);

    char idChunk[4];
    std::uint32_t tamanoChunk;
    short canales = 0;
    short bitsPorMuestra = 0;
    std::uint8_t* datosAudio = nullptr;

    // Escaneo dinámico de bloques en el archivo real
    while (archivo.read(idChunk, 4)) {
        archivo.read(reinterpret_cast<char*>(&tamanoChunk), 4);

        if (std::strncmp(idChunk, "fmt ", 4) == 0) {
            short formatoAudio;
            archivo.read(reinterpret_cast<char*>(&formatoAudio), 2);
            archivo.read(reinterpret_cast<char*>(&canales), 2);
            archivo.read(reinterpret_cast<char*>(&frecuencia), 4);
            archivo.seekg(6, std::ios::cur);
            archivo.read(reinterpret_cast<char*>(&bitsPorMuestra), 2);
            if (tamanoChunk > 16) archivo.seekg(tamanoChunk - 16, std::ios::cur);
        }
        else if (std::strncmp(idChunk, "data", 4) == 0) {
            tamano = tamanoChunk;
            datosAudio = new std::uint8_t[tamano];
            archivo.read(reinterpret_cast<char*>(datosAudio), tamano);
            break;
        }
        else {
            archivo.seekg(tamanoChunk, std::ios::cur);
        }
    }
    archivo.close();

    if (!datosAudio) {
        std::cerr << "Error: No se encontraron datos de audio validos." << std::endl;
        return nullptr;
    }

    if (canales == 1) {
        formato = (bitsPorMuestra == 8) ? AL_FORMAT_MONO8 : AL_FORMAT_MONO16;
    }
    else {
        formato = (bitsPorMuestra == 8) ? AL_FORMAT_STEREO8 : AL_FORMAT_STEREO16;
    }

    std::cout << "-> ¡WAV EXTERNO CARGADO! Canales: " << canales << " | Frecuencia: " << frecuencia << " Hz" << std::endl;
    return datosAudio;
}

// Murallas / paredes
float      escMurallaL = 1.0f;
glm::vec3  scaleMurallaL = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3  posMurallaL_1 = glm::vec3(-200.0f, -2.0f, -150.0f);
glm::vec3  rotMurallaL_1 = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3  posMurallaL_2 = glm::vec3(200.0f, -2.0f, 150.0f);
glm::vec3  rotMurallaL_2 = glm::vec3(0.0f, 180.0f, 0.0f);

// ======================================================
// SISTEMA: Window, shaders, camara, texturas, skybox
// ======================================================
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture pisoTexture;
Texture humoTexture;
Skybox skyboxDia;
Skybox skyboxNoche;

// ======================================================
// MODELOS - ESCENARIO
// ======================================================
Model Pared_M;          // Muralla/pared del escenario
Model PaloFarola_M;     // Poste de la farola
Model FocoDer_M;        // Foco derecho de la farola
Model FocoIzq_M;        // Foco izquierdo de la farola
Model Banca_M;          // Banca del parque
Model BloquePasto_M;    // Bloque de pasto decorativo
Model BasePlanetas_M;   // Base del reloj de planetas
Model RelojBase_M;      // Cuerpo principal del reloj
Model RelojEngrane1_M;  // Engrane 1 del reloj (orbita Y-)
Model RelojEngrane2_M;  // Engrane 2 del reloj (orbita Y+)
Model RelojEngrane3_M;  // Engrane 3 del reloj (orbita Y-)
Model RelojEngrane4_M;  // Engrane 4 del reloj (orbita Y+)
Model RelojGalRedE_M;   // Galaxia/red del reloj (orbita Y-)
Model RelojPlanetas_M;  // Planetas del reloj (orbita Y-)

// ======================================================
// MODELOS - AVATARES
// ======================================================

// --- Avatar BioShock: Big Daddy ---
Model BigDaddyCuerpo_M;   // Torso principal del Big Daddy
Model BigDaddyTaladro_M;  // Taladro del brazo derecho
Model BigDaddyBrazoIzq_M; // Brazo izquierdo
Model BigDaddyBrazoDer_M; // Brazo derecho
Model BigDaddyPiernaIzq_M;// Pierna izquierda
Model BigDaddyPiernaDer_M;// Pierna derecha

// --- Avatar Harry Potter: Hermione ---
Model Hermione_HP_M;   // Cuerpo completo de Hermione
Model BrazoDer_HP_M;   // Brazo derecho de Hermione
Model BrazoIzq_HP_M;   // Brazo izquierdo de Hermione
Model PiernaDer_HP_M;  // Pierna derecha de Hermione
Model PiernaIzq_HP_M;  // Pierna izquierda de Hermione

// --- Avatar Crash Bandicoot: Crash ---
Model Crash_Cuerpo_M;    // Cuerpo principal de Crash
Model Crash_BrazoDer_M;  // Brazo derecho de Crash
Model Crash_BrazoIzq_M;  // Brazo izquierdo de Crash
Model Crash_PiernaDer_M; // Pierna derecha de Crash
Model Crash_PiernaIzq_M; // Pierna izquierda de Crash

// ======================================================
// MODELOS - OBJETOS
// ======================================================

// --- Objetos BioShock ---
Model BolsaDer_M;   // Tapa derecha del medkit
Model BolsaIzq_M;   // Tapa izquierda del medkit
Model BolsaMid_M;   // Cuerpo central del medkit
Model Plasmido_M;   // Frasco de plasmido (flota y gira)

// --- Objetos Harry Potter ---
Model Carro_HP_M;        // Carro volador de Harry Potter
Model LlantaFD_HP_M;     // Llanta frontal derecha del carro HP
Model LlantaFI_HP_M;     // Llanta frontal izquierda del carro HP
Model LlantaAD_HP_M;     // Llanta trasera derecha del carro HP
Model LlantaAI_HP_M;     // Llanta trasera izquierda del carro HP
Model Bolsa_HP_M;        // Bolsa de Hermione
Model Giratiempo_HP_M;   // Giratiempo (time-turner)
Model LibroHechizos_HP_M;// Libro de hechizos
Model Snitch_HP_M;       // Snitch dorada (animacion de vuelo)
Model Copa_HP_M;         // Copa de los tres magos
Model Maletas_HP_M;      //maletas equipaje HP
Model EstatuaAjedrez_HP_M;  //estatua de pieza ajedrez pelicula
Model EstatuaEscaleraGiratoria_HP_M; //estatua (ave) escalera giratoria pelicula 
Model Farola_M;         //farola publica para iluminar la escena de noce

// --- Objetos Harry Potter: Expreso de Hogwarts ---
Model ExpresoHowarts_M;         // Cabina principal del Expreso de Hogwarts
Model LlantaSteamCar3_M;       // Llanta del Expreso (steamcar3)
Model SteamCar3_M;             // Modelo completo Expreso/bicicleta Harry
Model CabinaExpresoHowarts;
Model EjeDelanteroExpresoHowarts;
Model EjeTraseroExpresoHowarts;
Model EjeVagonExpresoHowarts;
Model VagonExpresoHowarts;
Model PistonIzquierdoExpresoHowarts;
Model BielaIzquierdaExpresoHowarts;
Model BielaMotrizIzquierdaExpresoHowarts;
Model PistonDerechoExpresoHowarts;
Model BielaDerechaExpresoHowarts;
Model BielaMotrizDerechaExpresoHowarts;

// --- Objetos Crash Bandicoot ---
Model Crash_GoKart_M;      // Go-kart de Crash
Model LlantaFD_Crash_M;    // Llanta frontal derecha del kart
Model LlantaFI_Crash_M;    // Llanta frontal izquierda del kart
Model LlantaAD_Crash_M;    // Llanta trasera derecha del kart
Model LlantaAI_Crash_M;    // Llanta trasera izquierda del kart
Model SteamCar_Crash_M;    // Steam car de Crash (decorativo)

// --- Objetos Crash Bandicoot: props del universo ---
Model ArbolWumpa_M;            // Arbol de fruta Wumpa
Model BaseCilindro_M;          // Base cilindrica decorativa
Model BolaRoca_M;              // Bola de roca
Model CajaAku_M;               // Caja con cara de Aku Aku
Model CajaCaraCrash_M;         // Caja con cara de Crash
Model CajaCheckPoint_M;        // Caja de checkpoint
Model CajaFlechaArriba_M;      // Caja con flecha hacia arriba
Model CajaMetalExclamation_M;  // Caja metalica con signo !
Model CajaNitro_M;             // Caja de nitro
Model CajaNitroExclamation_M;  // Caja nitro con signo !
Model CajaNormalCrash_M;       // Caja normal de Crash
Model CajaQuestion_M;          // Caja con signo ?
Model CajaTNT_M;               // Caja TNT
Model CocoOnGoKart_M;          // Coco Bandicoot en su kart
Model CortexOnGoKart_M;        // Dr. Cortex en su kart
Model CrystalCrash_M;          // Cristal del universo Crash
Model CrystalHielo_M;          // Cristal de hielo
Model FrutaWumpa_M;            // Fruta Wumpa
Model JoyasCrash_M;            // Joyas del universo Crash
Model MascaraAku_M;            // Mascara de Aku Aku
Model MascaraAkuNoche_M;            // Mascara de Aku Aku
Model MascaraUka_M;            // Mascara de Uka Uka
Model Pista_M;                 // Pista de carreras
Model SteamCar1_M;             // Steam Car 1 (Crash & Grunt)
Model SteamCar2_M;             // Steam Car 2 (Chief Coco)
Model Totem1Crash_M;           // Totem decorativo 1
Model Totem2Crash_M;           // Totem decorativo 2
Model RuedaSteamCar2_M;        // Rueda del Steam Car 2

// ======================================================
// MATERIALES
// ======================================================
Material Material_brillante; // Specular alto: metales, vidrio
Material Material_opaco;     // Specular bajo: madera, tela, pasto

// ======================================================
// TIEMPO / FPS
// ======================================================
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// ======================================================
// LUCES
// ======================================================
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight  spotLights[MAX_SPOT_LIGHTS];

// ======================================================
// SHADERS
// ======================================================
static const char* vShader = "shaders/shader_light.vert";
static const char* fShader = "shaders/shader_light.frag";

// ======================================================
// CAPTURA DE WAYPOINTS (C = toggle, P = guardar, L = limpiar)
// ======================================================
bool modoCaptura = false;
int  capturaContador = 0;
std::vector<glm::vec3> waypointsCapturados;

bool teclaCAnterior = false;
bool teclaPAnterior = false;
bool teclaLAnterior = false;

// ======================================================
// HELPER: aplica Translate * RotX * RotY * RotZ * Scale
// ======================================================
glm::mat4 ApplyTRS(glm::mat4 base, glm::vec3 t, glm::vec3 r, glm::vec3 s)
{
    base = glm::translate(base, t);
    base = glm::rotate(base, r.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    base = glm::rotate(base, r.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    base = glm::rotate(base, r.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    base = glm::scale(base, s);
    return base;
}

glm::vec3 SafeNormalize(const glm::vec3& v)
{
    float len = glm::length(v);
    if (len <= 0.0001f) return glm::vec3(0.0f, 0.0f, 0.0f);
    return v / len;
}

void UpdatePathFollower(
    glm::vec3& pos,
    float& rotY,
    int& currentIndex,
    const std::vector<glm::vec3>& path,
    float speed,
    float dt,
    float threshold = 0.60f)
{
    if (path.empty()) return;

    glm::vec3 target = path[currentIndex];
    glm::vec3 delta = target - pos;
    float dist = glm::length(delta);

    if (dist < threshold) {
        currentIndex = (currentIndex + 1) % path.size();
        target = path[currentIndex];
        delta = target - pos;
        dist = glm::length(delta);
    }

    if (dist > 0.0001f) {
        glm::vec3 dir = delta / dist;
        float step = speed * dt;
        if (step > dist) step = dist;
        pos += dir * step;

        rotY = atan2(dir.x, dir.z) / toRadians;
    }
}
// ======================================================
// HELPER: mueve una posicion hacia el origen en X y Z
// ======================================================
glm::vec3 MoveTowardsOriginXZ(glm::vec3 p, float step)
{
    if (p.x > 0.0f)      p.x -= step;
    else if (p.x < 0.0f) p.x += step;
    if (p.z > 0.0f)      p.z -= step;
    else if (p.z < 0.0f) p.z += step;
    return p;
}

// ======================================================
// GEOMETRIA BASE (piso como mesh)
// ======================================================
void CreateObjects()
{
    unsigned int floorIndices[] = { 0, 2, 1, 1, 2, 3 };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f,  0.0f,   0.0f, 1.0f, 0.0f,
         10.0f, 0.0f, -10.0f,  10.0f,  0.0f,   0.0f, 1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,   0.0f, 10.0f,   0.0f, 1.0f, 0.0f,
         10.0f, 0.0f,  10.0f,  10.0f, 10.0f,   0.0f, 1.0f, 0.0f
    };
    //PLANOS INTERSECTADOS PARA SIMULAR HUMO
    unsigned int humo1Indices[] = {
       0, 1, 2,
       0, 2, 3,
       4,5,6,
       4,6,7
    };
    GLfloat humo1Vertices[] = {
        // Primer Plano (XY) - Centrado en X, base en Y=0
        // Posiciones          // Textura      // Normales
        -0.5f,  0.0f, 0.0f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
         0.5f,  0.0f, 0.0f,    1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
         0.5f,  1.0f, 0.0f,    1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
        -0.5f,  1.0f, 0.0f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f,

        // Segundo Plano (ZY) - Centrado en Z, base en Y=0
         0.0f,  0.0f, -0.5f,   0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.0f,  0.0f,  0.5f,   1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.5f,   1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
         0.0f,  1.0f, -0.5f,   0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
    };

    Mesh* floor = new Mesh();
    floor->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(floor);

    //humo1 planos intersecados
    Mesh* humo = new Mesh();
    humo->CreateMesh(humo1Vertices, humo1Indices, 64, 12);
    meshList.push_back(humo);
}

// ======================================================
// SHADERS
// ======================================================
void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

std::vector<glm::vec3> puntosPista = {
    glm::vec3(-41.6996f, 0.6028f, 0.0000f),
    glm::vec3(-41.7042f, 1.6028f, 0.0000f),
    glm::vec3(-41.6725f, 3.2178f, 0.0000f),
    glm::vec3(-41.6772f, 5.3804f, 0.0000f),
    glm::vec3(-41.6772f, 25.8454f, 0.0000f),
    glm::vec3(-41.5318f, 31.5867f, 0.0000f),
    glm::vec3(-40.3448f, 35.1235f, 0.0000f),
    glm::vec3(-38.2130f, 38.0547f, 0.0000f),
    glm::vec3(-35.1850f, 40.2349f, 0.0000f),
    glm::vec3(-31.7208f, 41.3250f, 0.0000f),
    glm::vec3(-25.9796f, 41.4461f, 0.0000f),
    glm::vec3(-25.9796f, 41.4461f, 0.0000f),
    glm::vec3(28.3157f, 41.4742f, 0.0000f),
    glm::vec3(32.0447f, 41.2919f, 0.0000f),
    glm::vec3(35.5354f, 40.1284f, 0.0000f),
    glm::vec3(38.5214f, 37.9694f, 0.0000f),
    glm::vec3(40.6411f, 34.9616f, 0.0000f),
    glm::vec3(41.7918f, 31.4288f, 0.0000f),
    glm::vec3(41.9129f, 27.7749f, 0.0000f),
    glm::vec3(41.9371f, -16.3140f, 0.0000f),
    glm::vec3(41.7918f, -20.1415f, 0.0000f),
    glm::vec3(40.5805f, -23.6541f, 0.0000f),
    glm::vec3(38.3599f, -26.5981f, 0.0000f),
    glm::vec3(35.3655f, -28.7682f, 0.0000f),
    glm::vec3(31.8664f, -29.8953f, 0.0000f),
    glm::vec3(28.0981f, -30.0131f, 0.0000f),
    glm::vec3(28.0981f, -30.0131f, 0.0000f),
    glm::vec3(-30.0008f, -30.0131f, 0.0000f),
    glm::vec3(-31.8176f, -29.8516f, 0.0000f),
    glm::vec3(-35.3302f, -28.6807f, 0.0000f),
    glm::vec3(-38.2775f, -26.5005f, 0.0000f),
    glm::vec3(-38.2775f, -26.5005f, 0.0000f),
    glm::vec3(-40.4376f, -23.5128f, 0.0000f),
    glm::vec3(-41.1441f, -21.7363f, 0.0000f),
    glm::vec3(-41.5479f, -20.0002f, 0.0000f),
    glm::vec3(-41.5479f, -20.0002f, 0.0000f),
    glm::vec3(-41.6690f, -16.2050f, 0.0000f),
    glm::vec3(-41.6690f, -0.0889f, 0.0000f),
};

void recorrerPista(float deltaTime) {
    if (puntosPista.empty()) return;
    // 1. Definir puntos de origen y destino con cierre de circuito
    glm::vec3 p0 = puntosPista[i];
    // El operador % asegura que si i es el último, el siguiente sea 0
    int siguienteIndice = (i + 1) % puntosPista.size();
    glm::vec3 p1 = puntosPista[siguienteIndice];
    // 2. Control de progreso (t)
    distancia = glm::distance(p0, p1);
    // Evitar errores si hay puntos duplicados
    if (distancia > 0.0001f) {
        t += (velocidad * deltaTime) / distancia;
    }
    else {
        t = 1.0f;
    }
    // 3. Si llegamos al destino, avanzamos el índice
    if (t >= 1.0f) {
        t = 0.0f;
        i = (i + 1) % puntosPista.size(); // Reinicia i a 0 automáticamente al terminar
        return;
    }
    // --- POSICIÓN INTERPOLADA ---
    glm::vec3 posInterp = p0 + t * (p1 - p0);
    posCars.x = posInterp.x;
    posCars.y = posInterp.z; // Mapeo de ejes Blender -> OpenGL
    posCars.z = posInterp.y;
    // --- CÁLCULO DE DIRECCIÓN ---
    glm::vec3 direccion;
    direccion.x = p1.x - p0.x;
    direccion.y = p1.z - p0.z;
    direccion.z = p1.y - p0.y;

    if (glm::length(direccion) > 0.0001f) {
        direccion = glm::normalize(direccion);

        // --- CÁLCULO DE ÁNGULOS ---
        // angleY: Giro horizontal (Yaw)
        angleY = glm::degrees(atan2(direccion.x, direccion.z));
        // angleX: Inclinación de rampa (Pitch)
        angleX = glm::degrees(-asin(direccion.y));
    }
}
void animacionAplastaCajas(float deltaTime) {
    switch (estadoActual) {

    case REPOSO:
        if (mainWindow.getsKeys()[GLFW_KEY_1]) {
            // --- RESET ANTES DE EMPEZAR ---
            posBolaRoca = posBolaRocaInicial;
            rotacionRocaZ = 0.0f;
            // Devolvemos las cajas a su tamaño original (3.0f según tu código anterior)
            escAku = escCara = escCheck = escFlecha = escMetal = 5.0f;
            escNitro = escNitroExcl = escNormal = escQuest = escTNT = 5.0f;

            // Iniciamos la secuencia
            estadoActual = TORNADO_CRASH;
            rotacionTornado = 0.0f;
        }
        break;

    case TORNADO_CRASH:
        rotacionTornado += (deltaTime * 10) * velocidadGiro;
        if (rotacionTornado >= 7200.0f) {
            rotacionTornado = 0.0f;
            estadoActual = ROCA_RODANDO;
        }
        break;

    case ROCA_RODANDO:
        // 1. Mover y Rotar
        posBolaRoca.x += deltaTime * velocidadRoca;
        rotacionRocaZ -= deltaTime * 100.0f;

        // 2. Detección de aplastamiento (se mantiene igual)
        if (posBolaRoca.x > posCajaAku.x)         escAku = 0.5f;
        if (posBolaRoca.x > posCajaCaraCrash.x)   escCara = 0.5f;
        if (posBolaRoca.x > posCajaCheckPoint.x)  escCheck = 0.5f;
        if (posBolaRoca.x > posCajaFlechaArriba.x) escFlecha = 0.5f;
        if (posBolaRoca.x > posCajaMetalExcl.x)   escMetal = 0.5f;
        if (posBolaRoca.x > posCajaNitro.x)       escNitro = 0.5f;
        if (posBolaRoca.x > posCajaNitroExcl.x)   escNitroExcl = 0.5f;
        if (posBolaRoca.x > posCajaNormal.x)      escNormal = 0.5f;
        if (posBolaRoca.x > posCajaQuestion.x)    escQuest = 0.5f;
        if (posBolaRoca.x > posCajaTNT.x)         escTNT = 0.5f;

        // 3. DETENCIÓN POR TÓTEM
        // Si la roca alcanza la posición del tótem (menos un pequeño margen por el radio de la bola)
        if (posBolaRoca.x >= (posTotem1Crash.x - 9.0f)) {
            // Mantenemos la posición X justo ahí
            posBolaRoca.x = posTotem1Crash.x - 9.0f;
            // Pasamos a REPOSO pero SIN resetear los valores
            estadoActual = REPOSO;
        }
        break;
    }
}


void InicializarAudio() {

   
    std::cout << "-> Inicializando OpenAL Soft..." << std::endl;

    audioDevice = alcOpenDevice(NULL);
    if (!audioDevice) return;

    audioContext = alcCreateContext(audioDevice, NULL);
    if (!audioContext) { alcCloseDevice(audioDevice); return; }
    alcMakeContextCurrent(audioContext);

    // Generar la bocina del soundtrack
    alGenSources(1, &fuenteSoundtrack);
    alGenBuffers(1, &bufferSoundtrack);

    ALenum formato;
    ALsizei tamano, frecuencia;

    // LEER TU ARCHIVO REAL DE LA CARPETA DETECTADA
    std::uint8_t* datosMúsica = CargarWav("soundtrack", formato, tamano, frecuencia);

    if (datosMúsica != nullptr) {
        alBufferData(bufferSoundtrack, formato, datosMúsica, tamano, frecuencia);
        delete[] datosMúsica; // Liberar RAM local

        alSourcei(fuenteSoundtrack, AL_BUFFER, bufferSoundtrack);
        alSourcei(fuenteSoundtrack, AL_LOOPING, AL_TRUE); // Bucle infinito
        alSourcef(fuenteSoundtrack, AL_GAIN, 0.25f);       // Volumen al 25%

        // Audio de fondo relativo a la cabeza (2D uniforme)
        alSourcei(fuenteSoundtrack, AL_SOURCE_RELATIVE, AL_TRUE);
        alSource3f(fuenteSoundtrack, AL_POSITION, 0.0f, 0.0f, 0.0f);

        alSourcePlay(fuenteSoundtrack);
        std::cout << "-> ¡El archivo real soundtrack.wav esta sonando de fondo!" << std::endl;
    }
    else {
        std::cerr << "CRÍTICO: El archivo WAV real fallo al cargarse a OpenAL." << std::endl;
    }
}

void LimpiarAudio() {
    // Apagar y borrar soundtrack
    alSourceStop(fuenteSoundtrack);
    alDeleteSources(1, &fuenteSoundtrack);
    alDeleteBuffers(1, &bufferSoundtrack);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(audioContext);
    alcCloseDevice(audioDevice);
}

// ======================================================
// MAIN
// ======================================================
int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    mainWindow = Window(1366, 768);
    mainWindow.Initialise();
    CreateObjects();
    CreateShaders();
    InicializarAudio();

    camera = Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -60.0f, 0.0f, 20.0f, 0.3f
    );

    // ------------------------------------------------
    // TEXTURAS Y SKYBOX
    // ------------------------------------------------
    pisoTexture = Texture("Textures/piso2.jpg");
    pisoTexture.LoadTextureA();
    humoTexture = Texture("Textures/humo1.png");
    humoTexture.LoadTextureA();

    // Skybox dia
    std::vector<std::string> skyboxFacesDia;
    skyboxFacesDia.push_back("Textures/Skybox/_px.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_nx.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_ny.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_py.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_pz.jpg");
    skyboxFacesDia.push_back("Textures/Skybox/_nz.jpg");
    skyboxDia = Skybox(skyboxFacesDia);

    // Skybox noche
    std::vector<std::string> skyboxFacesNoche;
    skyboxFacesNoche.push_back("Textures/Skybox/_pxnight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_nxnight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_nynight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_pynight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_pznight.jpg");
    skyboxFacesNoche.push_back("Textures/Skybox/_nznight.jpg");
    skyboxNoche = Skybox(skyboxFacesNoche);

    // ================================================
    // CARGA DE MODELOS - ESCENARIO
    // ================================================
    printf("\n================ ESCENARIO ================\n");

    printf("Cargando Pared_M -> Models/pared.obj ...\n");
    Pared_M = Model(); Pared_M.LoadModel("Models/pared.obj");
    printf("OK -> Pared_M\n");

    printf("Cargando PaloFarola_M -> Models/Palofarola.obj ...\n");
    PaloFarola_M = Model(); PaloFarola_M.LoadModel("Models/Palofarola.obj");
    printf("OK -> PaloFarola_M\n");

    printf("Cargando FocoDer_M -> Models/Focoder.obj ...\n");
    FocoDer_M = Model(); FocoDer_M.LoadModel("Models/Focoder.obj");
    printf("OK -> FocoDer_M\n");

    printf("Cargando FocoIzq_M -> Models/Focoizq.obj ...\n");
    FocoIzq_M = Model(); FocoIzq_M.LoadModel("Models/Focoizq.obj");
    printf("OK -> FocoIzq_M\n");

    printf("Cargando Banca_M -> Models/Banca.obj ...\n");
    Banca_M = Model(); Banca_M.LoadModel("Models/Banca.obj");
    printf("OK -> Banca_M\n");

    printf("Cargando BloquePasto_M -> Models/Bloquepasto.obj ...\n");
    BloquePasto_M = Model(); BloquePasto_M.LoadModel("Models/Bloquepasto.obj");
    printf("OK -> BloquePasto_M\n");

    printf("Cargando BasePlanetas_M -> Models/Baseplanetas.obj ...\n");
    BasePlanetas_M = Model(); BasePlanetas_M.LoadModel("Models/Baseplanetas.obj");
    printf("OK -> BasePlanetas_M\n");

    printf("Cargando RelojBase_M -> Models/relojbase.obj ...\n");
    RelojBase_M = Model(); RelojBase_M.LoadModel("Models/relojbase.obj");
    printf("OK -> RelojBase_M\n");

    printf("Cargando RelojEngrane1_M -> Models/relojengrane1.obj ...\n");
    RelojEngrane1_M = Model(); RelojEngrane1_M.LoadModel("Models/relojengrane1.obj");
    printf("OK -> RelojEngrane1_M\n");

    printf("Cargando RelojEngrane2_M -> Models/relojengrane2.obj ...\n");
    RelojEngrane2_M = Model(); RelojEngrane2_M.LoadModel("Models/relojengrane2.obj");
    printf("OK -> RelojEngrane2_M\n");

    printf("Cargando RelojEngrane3_M -> Models/relojengrane3.obj ...\n");
    RelojEngrane3_M = Model(); RelojEngrane3_M.LoadModel("Models/relojengrane3.obj");
    printf("OK -> RelojEngrane3_M\n");

    printf("Cargando RelojEngrane4_M -> Models/relojengrane4.obj ...\n");
    RelojEngrane4_M = Model(); RelojEngrane4_M.LoadModel("Models/relojengrane4.obj");
    printf("OK -> RelojEngrane4_M\n");

    printf("Cargando RelojGalRedE_M -> Models/relojgalrede.obj ...\n");
    RelojGalRedE_M = Model(); RelojGalRedE_M.LoadModel("Models/relojgalrede.obj");
    printf("OK -> RelojGalRedE_M\n");

    printf("Cargando RelojPlanetas_M -> Models/relojplanetas.obj ...\n");
    RelojPlanetas_M = Model(); RelojPlanetas_M.LoadModel("Models/relojplanetas.obj");
    printf("OK -> RelojPlanetas_M\n");

    // ================================================
    // CARGA DE MODELOS - AVATARES
    // ================================================

    // --- Avatar BioShock ---
    printf("\n================ AVATARES - BIOSHOCK ================\n");

    printf("Cargando BigDaddyCuerpo_M -> Models/Cuerpobig.obj ...\n");
    BigDaddyCuerpo_M = Model(); BigDaddyCuerpo_M.LoadModel("Models/Cuerpobig.obj");
    printf("OK -> BigDaddyCuerpo_M\n");

    printf("Cargando BigDaddyTaladro_M -> Models/taladro.obj ...\n");
    BigDaddyTaladro_M = Model(); BigDaddyTaladro_M.LoadModel("Models/taladro.obj");
    printf("OK -> BigDaddyTaladro_M\n");

    printf("Cargando BigDaddyBrazoIzq_M -> Models/brazoizq.obj ...\n");
    BigDaddyBrazoIzq_M = Model(); BigDaddyBrazoIzq_M.LoadModel("Models/brazoizq.obj");
    printf("OK -> BigDaddyBrazoIzq_M\n");

    printf("Cargando BigDaddyBrazoDer_M -> Models/brazoder.obj ...\n");
    BigDaddyBrazoDer_M = Model(); BigDaddyBrazoDer_M.LoadModel("Models/brazoder.obj");
    printf("OK -> BigDaddyBrazoDer_M\n");

    printf("Cargando BigDaddyPiernaIzq_M -> Models/piernaizq.obj ...\n");
    BigDaddyPiernaIzq_M = Model(); BigDaddyPiernaIzq_M.LoadModel("Models/piernaizq.obj");
    printf("OK -> BigDaddyPiernaIzq_M\n");

    printf("Cargando BigDaddyPiernaDer_M -> Models/piernader.obj ...\n");
    BigDaddyPiernaDer_M = Model(); BigDaddyPiernaDer_M.LoadModel("Models/piernader.obj");
    printf("OK -> BigDaddyPiernaDer_M\n");

    // --- Avatar Harry Potter ---
    printf("\n================ AVATARES - HARRY POTTER ================\n");

    printf("Cargando Hermione_HP_M -> Models/HermioneCompleta.obj ...\n");
    Hermione_HP_M = Model(); Hermione_HP_M.LoadModel("Models/HermioneCompleta.obj");
    printf("OK -> Hermione_HP_M\n");

    printf("Cargando BrazoDer_HP_M -> Models/brazoderhermione.obj ...\n");
    BrazoDer_HP_M = Model(); BrazoDer_HP_M.LoadModel("Models/brazoderhermione.obj");
    printf("OK -> BrazoDer_HP_M\n");

    printf("Cargando BrazoIzq_HP_M -> Models/brazoizqhermione.obj ...\n");
    BrazoIzq_HP_M = Model(); BrazoIzq_HP_M.LoadModel("Models/brazoizqhermione.obj");
    printf("OK -> BrazoIzq_HP_M\n");

    printf("Cargando PiernaDer_HP_M -> Models/piernader.obj ...\n");
    PiernaDer_HP_M = Model(); PiernaDer_HP_M.LoadModel("Models/piernaderHermione.obj");
    printf("OK -> PiernaDer_HP_M\n");

    printf("Cargando PiernaIzq_HP_M -> Models/piernaizq.obj ...\n");
    PiernaIzq_HP_M = Model(); PiernaIzq_HP_M.LoadModel("Models/piernaizqHermione.obj");
    printf("OK -> PiernaIzq_HP_M\n");

    //expreso howards
    CabinaExpresoHowarts.LoadModel("Models/cabinaExpresoHowarts.obj");
    EjeDelanteroExpresoHowarts = Model();
    EjeDelanteroExpresoHowarts.LoadModel("Models/ejeDelanteroExpresoHowarts.obj");
    EjeTraseroExpresoHowarts = Model();
    EjeTraseroExpresoHowarts.LoadModel("Models/ejeTraseroExpresoHowarts.obj");
    EjeVagonExpresoHowarts = Model();
    EjeVagonExpresoHowarts.LoadModel("Models/ejeVagonExpresoHowarts.obj");
    VagonExpresoHowarts = Model();
    VagonExpresoHowarts.LoadModel("Models/vagonExpresoHowarts.obj");
    PistonIzquierdoExpresoHowarts = Model();
    PistonIzquierdoExpresoHowarts.LoadModel("Models/pistonIzquierdoExpresoHowarts.obj");
    BielaIzquierdaExpresoHowarts = Model();
    BielaIzquierdaExpresoHowarts.LoadModel("Models/bielaIzquierdaExpresoHowarts.obj");
    BielaMotrizIzquierdaExpresoHowarts = Model();
    BielaMotrizIzquierdaExpresoHowarts.LoadModel("Models/bielaMotrizIzquierdaExpresoHowarts.obj");
    PistonDerechoExpresoHowarts = Model();
    PistonDerechoExpresoHowarts.LoadModel("Models/pistonDerechoExpresoHowarts.obj");
    BielaDerechaExpresoHowarts = Model();
    BielaDerechaExpresoHowarts.LoadModel("Models/bielaDerechaExpresoHowarts.obj");
    BielaMotrizDerechaExpresoHowarts = Model();
    BielaMotrizDerechaExpresoHowarts.LoadModel("Models/bielaMotrizDerechaExpresoHowarts.obj");

    // --- Avatar Crash Bandicoot ---
    printf("\n================ AVATARES - CRASH ================\n");

    printf("Cargando Crash_Cuerpo_M -> Models/crashCharacterCuerpo.obj ...\n");
    Crash_Cuerpo_M = Model(); Crash_Cuerpo_M.LoadModel("Models/crashCharacterCuerpo.obj");
    printf("OK -> Crash_Cuerpo_M\n");

    printf("Cargando Crash_BrazoDer_M -> Models/crashCharacterBrazoDerecho.obj ...\n");
    Crash_BrazoDer_M = Model(); Crash_BrazoDer_M.LoadModel("Models/crashCharacterBrazoDerecho.obj");
    printf("OK -> Crash_BrazoDer_M\n");

    printf("Cargando Crash_BrazoIzq_M -> Models/crashCharacterBrazoIzquierdo.obj ...\n");
    Crash_BrazoIzq_M = Model(); Crash_BrazoIzq_M.LoadModel("Models/crashCharacterBrazoIzquierdo.obj");
    printf("OK -> Crash_BrazoIzq_M\n");

    printf("Cargando Crash_PiernaDer_M -> Models/crashCharacterPiernaDerecha.obj ...\n");
    Crash_PiernaDer_M = Model(); Crash_PiernaDer_M.LoadModel("Models/crashCharacterPiernaDerecha.obj");
    printf("OK -> Crash_PiernaDer_M\n");

    printf("Cargando Crash_PiernaIzq_M -> Models/crashCharacterPiernaIzquierda.obj ...\n");
    Crash_PiernaIzq_M = Model(); Crash_PiernaIzq_M.LoadModel("Models/crashCharacterPiernaIzquierda.obj");
    printf("OK -> Crash_PiernaIzq_M\n");

    // ================================================
    // CARGA DE MODELOS - OBJETOS
    // ================================================

    // --- Objetos BioShock ---
    printf("\n================ OBJETOS - BIOSHOCK ================\n");

    printf("Cargando BolsaDer_M -> Models/bolsader.obj ...\n");
    BolsaDer_M = Model(); BolsaDer_M.LoadModel("Models/bolsader.obj");
    printf("OK -> BolsaDer_M\n");

    printf("Cargando BolsaIzq_M -> Models/bolsaizq.obj ...\n");
    BolsaIzq_M = Model(); BolsaIzq_M.LoadModel("Models/bolsaizq.obj");
    printf("OK -> BolsaIzq_M\n");

    printf("Cargando BolsaMid_M -> Models/bolsamid.obj ...\n");
    BolsaMid_M = Model(); BolsaMid_M.LoadModel("Models/bolsamid.obj");
    printf("OK -> BolsaMid_M\n");

    printf("Cargando Plasmido_M -> Models/plasmido.obj ...\n");
    Plasmido_M = Model(); Plasmido_M.LoadModel("Models/plasmido.obj");
    printf("OK -> Plasmido_M\n");

    // --- Objetos Harry Potter ---
    printf("\n================ OBJETOS - HARRY POTTER ================\n");

    printf("Cargando Carro_HP_M -> Models/CarroCompleto.obj ...\n");
    Carro_HP_M = Model(); Carro_HP_M.LoadModel("Models/CarroCompleto.obj");
    printf("OK -> Carro_HP_M\n");

    printf("Cargando LlantaFD_HP_M -> Models/llSuperiorDer.obj ...\n");
    LlantaFD_HP_M = Model(); LlantaFD_HP_M.LoadModel("Models/llSuperiorDer.obj");
    printf("OK -> LlantaFD_HP_M\n");

    printf("Cargando LlantaFI_HP_M -> Models/llSuperiorIzq.obj ...\n");
    LlantaFI_HP_M = Model(); LlantaFI_HP_M.LoadModel("Models/llSuperiorIzq.obj");
    printf("OK -> LlantaFI_HP_M\n");

    printf("Cargando LlantaAD_HP_M -> Models/llAtrasDer.obj ...\n");
    LlantaAD_HP_M = Model(); LlantaAD_HP_M.LoadModel("Models/llAtrasDer.obj");
    printf("OK -> LlantaAD_HP_M\n");

    printf("Cargando LlantaAI_HP_M -> Models/llAtrasIzq.obj ...\n");
    LlantaAI_HP_M = Model(); LlantaAI_HP_M.LoadModel("Models/llAtrasIzq.obj");
    printf("OK -> LlantaAI_HP_M\n");

    printf("Cargando Bolsa_HP_M -> Models/bolsa.obj ...\n");
    Bolsa_HP_M = Model(); Bolsa_HP_M.LoadModel("Models/bolsa.obj");
    printf("OK -> Bolsa_HP_M\n");

    printf("Cargando Giratiempo_HP_M -> Models/giratiempo.obj ...\n");
    Giratiempo_HP_M = Model(); Giratiempo_HP_M.LoadModel("Models/giratiempo.obj");
    printf("OK -> Giratiempo_HP_M\n");

    printf("Cargando LibroHechizos_HP_M -> Models/libroHechizos.obj ...\n");
    LibroHechizos_HP_M = Model(); LibroHechizos_HP_M.LoadModel("Models/libroHechizos.obj");
    printf("OK -> LibroHechizos_HP_M\n");

    printf("Cargando Snitch_HP_M -> Models/snitch.obj ...\n");
    Snitch_HP_M = Model(); Snitch_HP_M.LoadModel("Models/snitch.obj");
    printf("OK -> Snitch_HP_M\n");

    printf("Cargando Copa_HP_M -> Models/copa.obj ...\n");
    Copa_HP_M = Model(); Copa_HP_M.LoadModel("Models/copa.obj");
    printf("OK -> Copa_HP_M\n");

    Maletas_HP_M = Model(); Maletas_HP_M.LoadModel("Models/maletasHP.obj");

    EstatuaAjedrez_HP_M = Model(); EstatuaAjedrez_HP_M.LoadModel("Models/estatuaAjedrez.obj");

    EstatuaEscaleraGiratoria_HP_M = Model(); EstatuaEscaleraGiratoria_HP_M.LoadModel("Models/estatuaEscaleraGiratoria.obj");

    Farola_M = Model();  Farola_M.LoadModel("Models/farola2.obj");

    // Expreso de Hogwarts
    printf("Cargando ExpresoHowarts_M -> Models/cabinaExpresoHowarts.obj ...\n");
    ExpresoHowarts_M = Model(); ExpresoHowarts_M.LoadModel("Models/expressHowartsCompleto.obj");
    printf("OK -> ExpresoHowarts_M\n");

    printf("Cargando LlantaSteamCar3_M -> Models/llantaSteamCar3BikeHarry.obj ...\n");
    LlantaSteamCar3_M = Model(); LlantaSteamCar3_M.LoadModel("Models/llantaSteamCar3BikeHarry.obj");
    printf("OK -> LlantaSteamCar3_M\n");

    printf("Cargando SteamCar3_M -> Models/steamCar3BikeHarry.obj ...\n");
    SteamCar3_M = Model(); SteamCar3_M.LoadModel("Models/steamCar3BikeHarry.obj");
    printf("OK -> SteamCar3_M\n");

    // --- Objetos Crash Bandicoot ---
    printf("\n================ OBJETOS - CRASH ================\n");

    printf("Cargando Crash_GoKart_M -> Models/crashOnGoKart.obj ...\n");
    Crash_GoKart_M = Model(); Crash_GoKart_M.LoadModel("Models/crashOnGoKart.obj");
    printf("OK -> Crash_GoKart_M\n");

    printf("Cargando LlantaFD_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaFD_Crash_M = Model(); LlantaFD_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaFD_Crash_M\n");

    printf("Cargando LlantaFI_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaFI_Crash_M = Model(); LlantaFI_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaFI_Crash_M\n");

    printf("Cargando LlantaAD_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaAD_Crash_M = Model(); LlantaAD_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaAD_Crash_M\n");

    printf("Cargando LlantaAI_Crash_M -> Models/ruedaSteamCar1CrashGrunt.obj ...\n");
    LlantaAI_Crash_M = Model(); LlantaAI_Crash_M.LoadModel("Models/ruedaSteamCar1CrashGrunt.obj");
    printf("OK -> LlantaAI_Crash_M\n");

    printf("Cargando SteamCar_Crash_M -> Models/steamCar.obj ...\n");
    SteamCar_Crash_M = Model();
    SteamCar_Crash_M.LoadModel("Models/steamCar.obj");
    printf("OK -> SteamCar_Crash_M\n");

    // Props del universo Crash
    printf("Cargando ArbolWumpa_M -> Models/arbolWumpa.obj ...\n");
    ArbolWumpa_M = Model(); ArbolWumpa_M.LoadModel("Models/arbolWumpa.obj");
    printf("OK -> ArbolWumpa_M\n");

    printf("Cargando BaseCilindro_M -> Models/baseCilindroCrash.obj ...\n");
    BaseCilindro_M = Model(); BaseCilindro_M.LoadModel("Models/baseCilindroCrash.obj");
    printf("OK -> BaseCilindro_M\n");

    printf("Cargando BolaRoca_M -> Models/bolaRocaCrash.obj ...\n");
    BolaRoca_M = Model(); BolaRoca_M.LoadModel("Models/bolaRocaCrash.obj");
    printf("OK -> BolaRoca_M\n");

    printf("Cargando CajaAku_M -> Models/cajaAku.obj ...\n");
    CajaAku_M = Model(); CajaAku_M.LoadModel("Models/cajaAku.obj");
    printf("OK -> CajaAku_M\n");

    printf("Cargando CajaCaraCrash_M -> Models/cajaCaraCrash.obj ...\n");
    CajaCaraCrash_M = Model(); CajaCaraCrash_M.LoadModel("Models/cajaCaraCrash.obj");
    printf("OK -> CajaCaraCrash_M\n");

    printf("Cargando CajaCheckPoint_M -> Models/cajaCheckPoint.obj ...\n");
    CajaCheckPoint_M = Model(); CajaCheckPoint_M.LoadModel("Models/cajaCheckPoint.obj");
    printf("OK -> CajaCheckPoint_M\n");

    printf("Cargando CajaFlechaArriba_M -> Models/cajaFlechaArriba.obj ...\n");
    CajaFlechaArriba_M = Model(); CajaFlechaArriba_M.LoadModel("Models/cajaFlechaArriba.obj");
    printf("OK -> CajaFlechaArriba_M\n");

    printf("Cargando CajaMetalExclamation_M -> Models/cajaMetalExclamation.obj ...\n");
    CajaMetalExclamation_M = Model(); CajaMetalExclamation_M.LoadModel("Models/cajaMetalExclamation.obj");
    printf("OK -> CajaMetalExclamation_M\n");

    printf("Cargando CajaNitro_M -> Models/cajaNitro.obj ...\n");
    CajaNitro_M = Model(); CajaNitro_M.LoadModel("Models/cajaNitro.obj");
    printf("OK -> CajaNitro_M\n");

    printf("Cargando CajaNitroExclamation_M -> Models/cajaNitroExclamation.obj ...\n");
    CajaNitroExclamation_M = Model(); CajaNitroExclamation_M.LoadModel("Models/cajaNitroExclamation.obj");
    printf("OK -> CajaNitroExclamation_M\n");

    printf("Cargando CajaNormalCrash_M -> Models/cajaNormalCrash.obj ...\n");
    CajaNormalCrash_M = Model(); CajaNormalCrash_M.LoadModel("Models/cajaNormalCrash.obj");
    printf("OK -> CajaNormalCrash_M\n");

    printf("Cargando CajaQuestion_M -> Models/cajaQuestion.obj ...\n");
    CajaQuestion_M = Model(); CajaQuestion_M.LoadModel("Models/cajaQuestion.obj");
    printf("OK -> CajaQuestion_M\n");

    printf("Cargando CajaTNT_M -> Models/cajaTNT.obj ...\n");
    CajaTNT_M = Model(); CajaTNT_M.LoadModel("Models/cajaTNT.obj");
    printf("OK -> CajaTNT_M\n");

    printf("Cargando CocoOnGoKart_M -> Models/cocoOnGoKart.obj ...\n");
    CocoOnGoKart_M = Model(); CocoOnGoKart_M.LoadModel("Models/cocoOnGoKart.obj");
    printf("OK -> CocoOnGoKart_M\n");

    printf("Cargando CortexOnGoKart_M -> Models/cortexOnGoKart.obj ...\n");
    CortexOnGoKart_M = Model(); CortexOnGoKart_M.LoadModel("Models/cortexOnGoKart.obj");
    printf("OK -> CortexOnGoKart_M\n");

    printf("Cargando CrystalCrash_M -> Models/crystalCrash.obj ...\n");
    CrystalCrash_M = Model(); CrystalCrash_M.LoadModel("Models/crystalCrash.obj");
    printf("OK -> CrystalCrash_M\n");

    printf("Cargando CrystalHielo_M -> Models/crystalHielo.obj ...\n");
    CrystalHielo_M = Model(); CrystalHielo_M.LoadModel("Models/crystalHielo.obj");
    printf("OK -> CrystalHielo_M\n");

    printf("Cargando FrutaWumpa_M -> Models/frutaWumpa.obj ...\n");
    FrutaWumpa_M = Model(); FrutaWumpa_M.LoadModel("Models/frutaWumpa.obj");
    printf("OK -> FrutaWumpa_M\n");

    printf("Cargando JoyasCrash_M -> Models/joyasCrash.obj ...\n");
    JoyasCrash_M = Model(); JoyasCrash_M.LoadModel("Models/joyasCrash.obj");
    printf("OK -> JoyasCrash_M\n");

    printf("Cargando MascaraAku_M -> Models/mascaraAku.obj ...\n");
    MascaraAku_M = Model(); MascaraAku_M.LoadModel("Models/mascaraAku.obj");
    printf("OK -> MascaraAku_M\n");

    printf("Cargando MascaraAkuNoche_M -> Models/mascaraAkuNoche.obj ...\n");
    MascaraAkuNoche_M = Model(); MascaraAkuNoche_M.LoadModel("Models/mascaraAkuNoche.obj");
    printf("OK -> MascaraAkuNoche_M\n");

    printf("Cargando MascaraUka_M -> Models/mascaraUka.obj ...\n");
    MascaraUka_M = Model(); MascaraUka_M.LoadModel("Models/mascaraUka.obj");
    printf("OK -> MascaraUka_M\n");

    printf("Cargando Pista_M -> Models/pista.obj ...\n");
    Pista_M = Model(); Pista_M.LoadModel("Models/pista.obj");
    printf("OK -> Pista_M\n");

    printf("Cargando SteamCar1_M -> Models/steamCar1CrashandGrunt.obj ...\n");
    SteamCar1_M = Model(); SteamCar1_M.LoadModel("Models/steamCar1CrashandGrunt.obj");
    printf("OK -> SteamCar1_M\n");

    printf("Cargando SteamCar2_M -> Models/steamCar2ChiefCoco.obj ...\n");
    SteamCar2_M = Model(); SteamCar2_M.LoadModel("Models/steamCar2ChiefCoco.obj");
    printf("OK -> SteamCar2_M\n");

    printf("Cargando Totem1Crash_M -> Models/totem1Crash.obj ...\n");
    Totem1Crash_M = Model(); Totem1Crash_M.LoadModel("Models/totem1Crash.obj");
    printf("OK -> Totem1Crash_M\n");

    printf("Cargando Totem2Crash_M -> Models/totem2Crash.obj ...\n");
    Totem2Crash_M = Model(); Totem2Crash_M.LoadModel("Models/totem2Crash.obj");
    printf("OK -> Totem2Crash_M\n");

    printf("Cargando RuedaSteamCar2_M -> Models/ruedaSteamCar2ChiefCoco.obj ...\n");
    RuedaSteamCar2_M = Model(); RuedaSteamCar2_M.LoadModel("Models/ruedaSteamCar2ChiefCoco.obj");
    printf("OK -> RuedaSteamCar2_M\n");

    printf("================ FIN CARGA DE MODELOS ================\n\n");

    // ================================================
    // MATERIALES
    // ================================================
    Material_brillante = Material(4.0f, 256); // Metales, vidrio, plasmido
    Material_opaco = Material(0.3f, 4);   // Madera, tela, pasto, pared

    // ================================================
    // LUCES
    // ================================================

    // Luz direccional (sol / luna): se recalcula cada frame segun ciclo dia/noche
    mainLight = DirectionalLight(
        1.0f, 1.0f, 1.0f,
        0.3f, 0.6f,
        0.0f, -0.5f, 1.0f
    );


    unsigned int pointLightCount = 0;

    // PointLight 0: luz roja del plasmido (posicion se actualiza cada frame)
    pointLights[0] = PointLight(
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f,
        -10.0f, -2.0f, 0.0f,
        0.5f, 0.3f, 0.2f
    );
    pointLightCount++;

    // PointLight 1: luz copa torneo 3 magos
    pointLights[1] = PointLight(
        0.53f, 0.81f, 0.98f,
        0.0f, 1.2f,
        62.0f, -1.0f, 18.0f,
        0.3f, 0.2f, 0.1f
    );
    pointLightCount++;

    // PointLight 2: luzfarola 1
    pointLights[2] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        -50.0f, 12.3f, -110.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    // PointLight 3: luzfarola 2
    pointLights[3] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        50.0f, 12.3f, -110.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    // PointLight 4: luzfarola 3
    pointLights[4] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        -50.0f, 12.3f, 110.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    // PointLight 5: luzfarola 4
    pointLights[5] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        50.0f, 12.3f, 110.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    // PointLight 6: luzfarola 5
    pointLights[6] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        105.0f, 12.3f, 50.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    // PointLight 7: luzfarola 6
    pointLights[7] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        105.0f, 12.3f, -50.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    // PointLight 8: luzfarola 7
    pointLights[8] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        -105.0f, 12.3f, 50.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    // PointLight 9: luzfarola 8
    pointLights[9] = PointLight(
        1.0f, 0.75f, 0.35f,    // Color dorado cálido
        0.0f, 0.0f,            // INICIA APAGADA: Ambiental (0.0f) y Difusa (0.0f) para el arranque de día
        -105.0f, 12.3f, -50.0f,     // POSICIÓN CORREGIDA: X=50, Z=0 (igual que el modelo) y Y=7.5 (altura de la bombilla)
        0.3f, 0.01f, 0.00025f  // Tu atenuación perfecta de doble alcance
    );
    pointLightCount++;

    
    unsigned int spotLightCount = 0;

    // SpotLight 0: linterna de la camara (flash, se actualiza cada frame)
    spotLights[0] = SpotLight(
        1.0f, 1.0f, 1.0f,
        0.0f, 2.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        5.0f
    );
    spotLightCount++;

    // SpotLight 1: luz azul estatica de la escena
    spotLights[1] = SpotLight(
        0.0f, 0.0f, 1.0f,
        1.0f, 2.0f,
        5.0f, 10.0f, 0.0f,
        0.0f, -5.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        15.0f
    );
    spotLightCount++;

    // SpotLight 2: ilumina los carritos de crash bandicoot uno por uno por tecla
    spotLights[2] = SpotLight(
        0.9f, 0.95f, 1.0f,      // Color: Blanco frío de alta tecnología
            2.5f, 0.6f,             // Intensidad ambiental y difusa (un poco más brillante)
            -160.0f, 12.0f, 0.0f,   // POSICIÓN: Sobre el carrito de Coco (Y elevado a 12)
            0.0f, -1.0f, 0.0f,      // DIRECCIÓN: Recto hacia abajo
            1.0f, 0.007f, 0.0008f,
            30.0f                   // Ángulo ligeramente más abierto para cubrir el coche completo
        );
    spotLightCount++;

    spotLights[3] = SpotLight(
        0.0f, 0.85f, 1.0f,      // COLOR: Cian / Azul Eléctrico Neón (Contrasta con el rosa y se ve mágico)
        3.0f, 0.8f,             // INTENSIDADES: Ambiental (3.0f) y Difusa (0.8f)
        160.0f, 14.0f, 0.0f,    // POSICIÓN: Justo arriba del cristal
        0.0f, -1.0f, 0.0f,      // DIRECCIÓN: Recto de arriba hacia abajo
        1.0f, 0.007f, 0.0008f,  // ATENUACIÓN: Rango optimizado
        25.0f                   // ÁNGULO: Cono cerrado para enfocar la gema
    );
    spotLightCount++;

    // SpotLight 4: Faro del Tren Howarts
    spotLights[4] = SpotLight(
        1.0f, 0.85f, 0.5f,      // Color: Blanco cálido (igual)
        3.0f, 3.0f,             // ↑↑ Ambiente y Difusa: ambas a 3.0f para más potencia
        0.0f, 0.0f, 0.0f,       // Posición dinámica
        0.0f, 0.0f, -1.0f,      // Dirección dinámica
        1.0f, 0.003f, 0.0003f,  // ↑ Atenuación: Reducida (lineal y cuadrática más bajas)
        25.0f                   // Ángulo (igual)
    );
    spotLightCount++;

    // SpotLight 5: Ilumina por teclado a las escultura de ave de la pelicula Harry potter escalera giratoria
    spotLights[5] = SpotLight(
        1.0f, 0.85f, 0.5f,
        2.5f, 0.5f,
        0.0f, 0.0f, -120.0f,  // POSICIÓN: Justo encima de la estatua (Y = 7.0f)
        0.0f, 1.0f, 0.0f,      // DIRECCIÓN: Recto hacia abajo
        1.0f, 0.007f, 0.0008f,
        25.0f
    );
    spotLightCount++;


    // ================================================
    // UNIFORMS DEL SHADER
    // ================================================
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
    GLuint uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
    GLuint uniformTextureOffset = 0, uniformColor = 0;

    // Matriz de proyeccion perspectiva (se calcula una sola vez)
    glm::mat4 projection = glm::perspective(
        45.0f,
        (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
        0.1f,
        1000.0f
    );

    lastTime = glfwGetTime();

    // ================================================
    // VARIABLES - ESCENARIO
    // ================================================

    // Piso (mesh plano escalado)
    // Se usa meshList[0], no tiene variables propias de posicion

    

    // Farola: posicion y rotacion del objeto padre
    float      escFarola = 1.0f;
    glm::vec3  posFarola = glm::vec3(6.0f, -2.0f, -4.0f);
    glm::vec3  rotFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    // Offsets locales de cada pieza de la farola (relativas al padre)
    glm::vec3  offsetPaloFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotPaloFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scalePaloFarola = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetFocoDerFarola = glm::vec3(0.0f, 8.38f, 3.05f);
    glm::vec3  rotFocoDerFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleFocoDerFarola = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetFocoIzqFarola = glm::vec3(0.0f, 8.38f, -3.0f);
    glm::vec3  rotFocoIzqFarola = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleFocoIzqFarola = glm::vec3(1.0f, 1.0f, 1.0f);
    // Posiciones locales de la luz dentro de cada foco (para SpotLight)
    glm::vec3  luzLocalFocoDer = glm::vec3(0.0f, 8.10f, 3.05f);
    glm::vec3  luzLocalFocoIzq = glm::vec3(0.0f, 8.10f, -3.0f);
    glm::vec3  dirLocalFocoDer = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
    glm::vec3  dirLocalFocoIzq = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));

    // Banca del parque
    float      escBanca = 1.0f;
    glm::vec3  posBanca = glm::vec3(2.5f, -2.0f, 6.0f);
    glm::vec3  rotBanca = glm::vec3(0.0f, 180.0f, 0.0f);
    glm::vec3  scaleBanca = glm::vec3(1.0f, 1.0f, 1.0f);

    // Bloques de pasto (8 instancias distribuidas en anillos alrededor de la escena)
    float      escBloquePasto = 1.0f;
    glm::vec3  scaleBloquePasto = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 pastoBasePos[4] = {
        glm::vec3(160.0f, -2.0f, 115.0f),
        glm::vec3(-110.0f, -2.0f, 170.0f),
        glm::vec3(108.0f, -2.0f, -173.0f),
        glm::vec3(-160.0f, -2.0f, -120.0f)
    };

    float pastoRotY[4] = { 0.0f, -90.0f, 90.0f, 180.0f };
    float desplazamientoHaciaOrigen = 25.0f;

    // CORRECCIÓN: Ahora son solo 8 elementos en total (4 exteriores + 4 interiores)
    glm::vec3 pastoPos[8];
    glm::vec3 pastoRot[8];

    // Anillo exterior (Índices 0 al 3)
    for (int i = 0; i < 4; i++) {
        pastoPos[i] = pastoBasePos[i];
        pastoRot[i] = glm::vec3(0.0f, pastoRotY[i], 0.0f);
    }

    // Anillo interior (Índices 4 al 7)
    // CORRECCIÓN: Se guardan consecutivamente usando [i + 4] para no dejar huecos vacíos
    for (int i = 0; i < 4; i++) {
        pastoPos[i + 4] = MoveTowardsOriginXZ(pastoBasePos[i], desplazamientoHaciaOrigen * 2.0f);
        pastoRot[i + 4] = glm::vec3(0.0f, pastoRotY[i], 0.0f);
    }

    // Reloj de planetas (objeto padre + piezas jerarquicas)
    float      escReloj = 4.0f;
    glm::vec3  posReloj = glm::vec3(0.0f, -2.0f, -20.0f);
    glm::vec3  rotReloj = glm::vec3(0.0f, 0.0f, 0.0f);
    // Offsets y escalas locales de cada pieza del reloj
    glm::vec3  offsetBasePlanetas = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotBasePlanetas = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleBasePlanetas = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojBase = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotRelojBase = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojBase = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane1 = glm::vec3(0.0f, 0.3f, 0.3f);
    glm::vec3  rotRelojEngrane1 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane1 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane2 = glm::vec3(0.0f, 0.3f, 0.0f);
    glm::vec3  rotRelojEngrane2 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane2 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane3 = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3  rotRelojEngrane3 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane3 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojEngrane4 = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3  rotRelojEngrane4 = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojEngrane4 = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojGalRedE = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  rotRelojGalRedE = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojGalRedE = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3  offsetRelojPlanetas = glm::vec3(0.1f, 2.2f, -0.1f);
    glm::vec3  rotRelojPlanetas = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  scaleRelojPlanetas = glm::vec3(1.0f, 1.0f, 1.0f);
    // Velocidades de orbita de cada engrane del reloj (grados/segundo)
    float velRelojEngrane1 = 15.0f;
    float velRelojEngrane2 = 6.0f;
    float velRelojEngrane3 = 6.0f;
    float velRelojEngrane4 = 6.0f;
    float velRelojGalRedE = 6.0f;
    float velRelojPlanetas = 6.0f;
    // Angulos de orbita acumulados (se actualizan cada frame)
    float angOrbitaRelojEngrane1 = 0.0f;
    float angOrbitaRelojEngrane2 = 0.0f;
    float angOrbitaRelojEngrane3 = 0.0f;
    float angOrbitaRelojEngrane4 = 0.0f;
    float angOrbitaRelojGalRedE = 0.0f;
    float angOrbitaRelojPlanetas = 0.0f;

   

    // Debug: timer para imprimir posicion de camara en consola
    float debugTimer = 0.0f;
   
    // ============================================
    // CAMARAS - HARRY POTTER / HERMIONE
    // ============================================

    int modoCamaraHP = 0;
    bool prevF1Cam = false;
    bool prevF2Cam = false;
    bool prevF3Cam = false;
    bool hpCamInit = false;

    // Camara de tercera persona ligada a Hermione
    float hpCamDistTP = 6.0f;
    float hpCamAlturaTP = 3.0f;
    float hpCamLookAhead = 1.5f;
    float hpCamLookY = 1.2f;

    // Camara aerea sobre plano XZ
    glm::vec3 hpCamAereaPos(0.0f, 0.0f, 0.0f);
    float hpCamAereaVel = 60.0f;
    float hpCamAereaAltura = 200.0f;

    // Camara de recorrido de interes
    std::vector<glm::vec3> hpCamInteresPos;
    std::vector<glm::vec3> hpCamInteresTarget;
    float hpCamInteresTimer = 0.0f;
    float hpCamInteresDuracion = 4.0f;


    // ================================================
    // VARIABLES - AVATAR BIOSHOCK: Big Daddy
    // ================================================

    // Escala global del Big Daddy
    float      escBigDaddy = 2.0f;
    // Posicion del objeto padre del Big Daddy en la escena
    glm::vec3  posBigDaddy = glm::vec3(0.0f, -1.0f, 0.0f);
    // Offsets locales de cada parte del Big Daddy (relativas al padre)
    glm::vec3  offsetCuerpo = glm::vec3(-0.9f, 4.0f, 1.0f);
    glm::vec3  rotCuerpo = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetBrazoIzq = glm::vec3(-2.0f, 4.2f, 0.0f);
    glm::vec3  rotBrazoIzq = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetBrazoDer = glm::vec3(0.2f, 4.2f, 0.0f);
    glm::vec3  rotBrazoDer = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetPiernaIzq = glm::vec3(-1.5f, 2.7f, 0.0f);
    glm::vec3  rotPiernaIzq = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetPiernaDer = glm::vec3(-0.15f, 2.7f, 0.0f);
    glm::vec3  rotPiernaDer = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3  offsetTaladro = glm::vec3(1.6f, 0.7f, 0.1f);
    glm::vec3  rotTaladro = glm::vec3(0.0f, 0.0f, 45.0f);
    // Animacion del taladro (tecla 1)
    float      taladroRot = 1.0f;
    bool       taladroGirando = false;
    bool       prevTecla1 = false;
    // Animacion de caminata del Big Daddy automatica por trayectoria cerrada
    float caminataTime = 0.0f;
    float caminataSpeed = 3.0f;
    float caminataAmp = 25.0f;         // amplitud piernas
    float caminataBrazoAmp = 10.0f;    // amplitud brazos
    bool bigDaddyCaminando = true;

    // Posicion y orientacion real del Big Daddy sobre el mundo
    glm::vec3 bigDaddyPosActual = glm::vec3(-100.31f, posBigDaddy.y, 108.96f);
    float bigDaddyRotY = 0.0f;
    float bigDaddyVel = 9.0f;

    // Ruta cerrada: se ignora Y de tus capturas y se conserva la Y actual del Big Daddy
    std::vector<glm::vec3> rutaBigDaddy = {
        glm::vec3(-100.31f, posBigDaddy.y, 108.96f),
        glm::vec3(-101.90f, posBigDaddy.y, -113.97f),
        glm::vec3(-94.94f,  posBigDaddy.y, -113.87f),
        glm::vec3(8.21f,  posBigDaddy.y, -113.42f),
        glm::vec3(1.95f,  posBigDaddy.y,  -62.86f),
        glm::vec3(25.05f,  posBigDaddy.y,  -66.81f),
        glm::vec3(102.11f,  posBigDaddy.y, -117.19f),
        glm::vec3(104.51f,  posBigDaddy.y, -103.92f),
        glm::vec3(102.80f,  posBigDaddy.y,   -6.70f),
        glm::vec3(71.21f,  posBigDaddy.y,    7.65f),
        glm::vec3(98.84f,  posBigDaddy.y,   64.48f),
        glm::vec3(104.10f,  posBigDaddy.y,  108.58f),
        glm::vec3(61.31f,  posBigDaddy.y,  106.53f),
        glm::vec3(-17.02f,  posBigDaddy.y,  109.65f),
        glm::vec3(-17.15f,  posBigDaddy.y,   65.64f),
        glm::vec3(-49.46f,  posBigDaddy.y,    3.16f),
        glm::vec3(56.64f,  posBigDaddy.y,   -0.67f),
        glm::vec3(-25.46f,  posBigDaddy.y,   70.07f),
        glm::vec3(-101.35f, posBigDaddy.y,  112.95f)
    };

    int bigDaddyNodoActual = 1;
    // Efecto de vibracion de camara al acercarse al Big Daddy
    float      vibracionAmp = 0.05f;
    float      vibracionRangoMax = 15.0f;

    // ================================================
    // VARIABLES - OBJETOS BIOSHOCK
    // ================================================

    // Medkit (bolsa): escala y posicion del padre, offsets de tapas
    float      escMedkit = 0.25f;
    glm::vec3  posMedkit = glm::vec3(10.0f, -1.0f, 0.0f);
    glm::vec3  offsetBolsaDer = glm::vec3(0.0f, 0.2f, -0.13f);
    glm::vec3  offsetBolsaIzq = glm::vec3(0.0f, 0.0f, 1.1f);
    // Animacion de apertura del medkit (se activa al acercarse)
    float      puertaAngulo = 0.0f;
    float      puertaTarget = 0.0f;
    float      puertaSpeed = 3.0f;
    float      distActivacionMedkit = 5.0f;

    // Plasmido: escala base, posicion y animaciones de flotacion/latido/rotacion
    float      escPlasmido = 3.0f;
    glm::vec3  posPlasmido = glm::vec3(-10.0f, -1.0f, 0.0f);
    float      plasTime = 0.0f;
    float      plasFloatSpeed = 0.25f;  // velocidad del seno de flotacion
    float      plasFloatAmp = 0.04f;  // amplitud de flotacion en Y
    float      plasRotSpeed = 3.0f;   // velocidad de rotacion en Y
    float      plasRotY = 0.0f;   // angulo de rotacion acumulado
    float      plasHeartSpeed = 0.4f;   // velocidad del "latido" de escala
    float      plasHeartAmp = 0.25f;  // amplitud del latido de escala


    //copa animacion
    float copaTime = 0.0f;
    float copaFloatSpeed = 0.25f;
    float copaFloatAmp = 0.04f;
    float copaRotSpeed = 3.0f;
    float copaRotY = 0.0f;
    float copaHeartSpeed = 0.4f;
    float copaHeartAmp = 0.20f;

    // ================================================
    // VARIABLES - AVATAR HARRY POTTER: Hermione
    // ================================================

    

    // Posicion y rotacion de Hermione (movimiento con FLECHAS)
    float hpPosX = 0.0f;
    float hpPosZ = 0.0f;
    float hpRotPersonaje = 0.0f;
    float hpVelocidad = 30.0f;

    // Angulos de animacion de extremidades de Hermione
    float hpRotBrazoDer = 0.0f;
    float hpRotBrazoIzq = 0.0f;
    float hpRotPiernaDer = 0.0f;
    float hpRotPiernaIzq = 0.0f;


    // ================================================
  // VARIABLES - OBJETOS HARRY POTTER
  // ================================================

  // Carro volador: ruta automatica con tecla 1
    std::vector<glm::vec3> hpCarroRuta = {
        glm::vec3(-125.04f, 4.45f, -129.69f),
        glm::vec3(-87.08f,  5.65f, -158.69f),
        glm::vec3(114.75f,  2.43f, -153.83f),
        glm::vec3(129.78f,  1.60f, -102.01f),
        glm::vec3(129.45f,  1.62f,  95.31f),
        glm::vec3(103.59f,  1.86f, 130.33f),
        glm::vec3(-84.61f,  1.87f, 135.58f),
        glm::vec3(-123.09f, 1.18f, 110.57f),
        glm::vec3(-129.73f, 1.57f, -93.24f),
        glm::vec3(-118.50f, 1.81f, -149.37f),
        glm::vec3(-84.57f,  2.61f, -165.86f)
    };

    float hpCarroYFijo = 1.60f;
    glm::vec3 hpCarroPos = glm::vec3(hpCarroRuta[0].x, hpCarroYFijo, hpCarroRuta[0].z);
    float hpRotCarro = 0.0f;
    float hpRotLlantas = 0.0f;
    float hpVelocidadCarro = 10.0f;
    bool hpCarroAutoActivo = false;
    bool prevTeclaAuto1 = false;
    int hpCarroWPActual = 1;

    // Props de Harry Potter
    glm::vec3 hpPosBolsa = hpOffsetEscena + glm::vec3(-8.0f, -1.0f, -4.0f);
    glm::vec3 hpPosGiratiempo = hpOffsetEscena + glm::vec3(-10.0f, 0.0f, -7.0f);
    glm::vec3 hpPosLibro = hpOffsetEscena + glm::vec3(3.0f, -1.0f, 3.0f);
    glm::vec3 hpPosCopa = hpOffsetEscena + glm::vec3(7.0f, -1.0f, -7.0f);

    // Snitch: ruta automatica con tecla 3
    std::vector<glm::vec3> hpSnitchRuta = {
        glm::vec3(99.43f,   5.88f, -32.88f),
        glm::vec3(49.79f,   3.06f,  47.08f),
        glm::vec3(31.90f,   5.82f, 102.71f),
        glm::vec3(-11.96f,  2.07f,  19.60f),
        glm::vec3(-11.74f,  3.85f, -27.56f),
        glm::vec3(17.69f,   4.55f, -103.31f),
        glm::vec3(-97.00f,  3.90f, -107.43f),
        glm::vec3(-98.47f,  6.74f,  -5.85f),
        glm::vec3(-99.56f,  4.43f,  85.56f),
        glm::vec3(33.56f,   4.25f,   4.02f)
    };

    glm::vec3 hpPosSnitch = hpSnitchRuta[0];
    bool hpSnitchVisible = true;
    bool hpSnitchAnimacionActiva = false;
    bool prevTeclaAuto3 = false;
    int hpSnitchWPActual = 1;
    float hpSnitchVelocidadRuta = 15.0f;

 


    // ================================================
    // VARIABLES - AVATAR CRASH BANDICOOT: Crash
    // ================================================

    glm::vec3 crashOffsetEscena = glm::vec3(-55.0f, 0.0f, 25.0f);

    float crashPosX = 0.0f;
    float crashPosZ = 0.0f;
    float crashRotY = 0.0f;
    float crashVelocidad = 2.0f;

    float crashRotBrazoDer = 0.0f;
    float crashRotBrazoIzq = 0.0f;
    float crashRotPiernaDer = 0.0f;
    float crashRotPiernaIzq = 0.0f;


    // ================================================
    // VARIABLES - OBJETOS CRASH BANDICOOT
    // ================================================

    // Go-kart de Crash: ruta automatica con tecla 2
    std::vector<glm::vec3> kartRuta = {
        glm::vec3(-53.65f,  2.59f, -151.59f),
        glm::vec3(-79.39f,  2.78f, -152.69f),
        glm::vec3(-116.51f, 1.83f, -129.59f),
        glm::vec3(-120.70f, 2.37f,   76.67f),
        glm::vec3(-103.70f, 2.25f,  117.08f),
        glm::vec3(90.09f,   2.16f,  121.69f),
        glm::vec3(119.34f,  2.59f,   96.96f),
        glm::vec3(120.59f,  2.58f, -113.85f),
        glm::vec3(95.87f,   2.86f, -148.83f),
        glm::vec3(-35.04f,  2.35f, -150.78f)
    };

    glm::vec3 kartPos = kartRuta[0];
    float kartRotY = 0.0f;
    float kartVelocidad = 8.0f;
    float kartRotLlantas = 0.0f;
    bool kartAutoActivo = false;
    bool prevTeclaAuto2 = false;
    int kartWPActual = 1;



    // Posiciones de los props del universo Crash (linea de display en Z+15)
    glm::vec3  posArbolWumpa = crashOffsetEscena + glm::vec3(0.0f, -2.0f, 15.0f);
    glm::vec3  posBaseCilindro = crashOffsetEscena + glm::vec3(5.0f, -2.0f, 15.0f);
    glm::vec3  posCocoOnGoKart = crashOffsetEscena + glm::vec3(65.0f, -2.0f, 15.0f);
    glm::vec3  posCortexOnGoKart = crashOffsetEscena + glm::vec3(70.0f, -2.0f, 15.0f);
    //glm::vec3  posCrystalCrash = crashOffsetEscena + glm::vec3(75.0f, -2.0f, 15.0f);
    glm::vec3  posCrystalHielo = crashOffsetEscena + glm::vec3(80.0f, -2.0f, 15.0f);
    glm::vec3  posFrutaWumpa = crashOffsetEscena + glm::vec3(85.0f, -2.0f, 15.0f);
    glm::vec3  posJoyasCrash = crashOffsetEscena + glm::vec3(90.0f, -2.0f, 15.0f);
    glm::vec3  posMascaraAku = crashOffsetEscena + glm::vec3(95.0f, -2.0f, 15.0f);
    glm::vec3  posMascaraUka = crashOffsetEscena + glm::vec3(100.0f, -2.0f, 15.0f);
    glm::vec3 posPista = glm::vec3(0.0f, -1.5f, 20.0f);
    glm::vec3  posSteamCar1 = crashOffsetEscena + glm::vec3(110.0f, -2.0f, 15.0f);
    glm::vec3  posSteamCar2 = crashOffsetEscena + glm::vec3(115.0f, -2.0f, 15.0f);
    glm::vec3  posTotem2Crash = crashOffsetEscena + glm::vec3(125.0f, -2.0f, 15.0f);
    glm::vec3  posRuedaSteamCar2 = crashOffsetEscena + glm::vec3(130.0f, -2.0f, 15.0f);


    // ================================================
    // VARIABLES - SISTEMA DE CAMARAS
    // ================================================
    int modoCamara = 1;           // 1=3raPersona, 2=Aerea, 3=Interes
    bool prevTeclaVirgula = false; // tecla ,
    bool prevTeclaPunto = false;   // tecla .
    bool prevTeclaMenos = false;   // tecla -
    int puntoInteresActual = 0;
    bool prevTeclaV = false;


    // ================================================
    // LOOP PRINCIPAL
    // ================================================
    while (!mainWindow.getShouldClose())
    {
        // --- Delta time ---
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        if (deltaTime > 0.05f) deltaTime = 0.05f; // cap a 20fps minimo
        lastTime = now;






        glfwPollEvents();
        // El mouse siempre debe actualizar la orientación de la cámara tanto en Tercera Persona (1) como en Libre (3)
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        if (modoCamara == 3)
        {
            // El movimiento WASD del espacio libre solo afecta a la cámara si estamos en modo libre
            camera.keyControl(mainWindow.getsKeys(), deltaTime);
        }
     

        // ============================================
        // LOGICA DE ANIMACIONES Y CONTROLES
        // ============================================

        //copa animacion como plasmido
        copaTime += deltaTime;
        copaRotY += copaRotSpeed * deltaTime;
        if (copaRotY > 360.0f) copaRotY -= 360.0f;

        float copaOffsetY = copaFloatAmp * sinf(copaTime * copaFloatSpeed);
        float copaHeartbeat = fabsf(sinf(copaTime * copaHeartSpeed));
        float copaEscActual = 1.0f + copaHeartAmp * copaHeartbeat;

        // --- Taladro del Big Daddy automatico mientras camina ---
        if (bigDaddyCaminando)
        {
            taladroRot += 150.0f * deltaTime;
            if (taladroRot > 360.0f)
                taladroRot -= 360.0f;
        }

        // --- Caminata automatica del Big Daddy por ruta cerrada ---
        float pasoSeno = 0.0f;
        float anguloPiernaIzq = 0.0f, anguloPiernaDer = 0.0f;
        float anguloBrazoIzq = 0.0f, anguloBrazoDer = 0.0f;

        if (rutaBigDaddy.size() >= 2)
        {
            glm::vec3 destino = rutaBigDaddy[bigDaddyNodoActual];
            glm::vec3 direccion = destino - bigDaddyPosActual;
            direccion.y = 0.0f;

            float distancia = glm::length(direccion);

            if (distancia > 0.001f)
            {
                bigDaddyCaminando = true;

                glm::vec3 dirNorm = glm::normalize(direccion);
                float paso = bigDaddyVel * deltaTime;

                if (paso >= distancia)
                {
                    bigDaddyPosActual = destino;
                    bigDaddyNodoActual = (bigDaddyNodoActual + 1) % rutaBigDaddy.size();
                }
                else
                {
                    bigDaddyPosActual += dirNorm * paso;
                }

                bigDaddyPosActual.y = posBigDaddy.y;

                // Ajuste para que el modelo mire hacia donde avanza
                bigDaddyRotY = -glm::degrees(atan2(dirNorm.x, dirNorm.z)) + 180.0f;

                // Reutiliza la animacion que ya tenias en brazos y piernas
                caminataTime += deltaTime;
                pasoSeno = sinf(caminataTime * caminataSpeed);

                anguloPiernaIzq = pasoSeno * caminataAmp;
                anguloPiernaDer = -pasoSeno * caminataAmp;
                anguloBrazoIzq = -pasoSeno * caminataBrazoAmp;
                anguloBrazoDer = pasoSeno * caminataBrazoAmp;
            }
            else
            {
                bigDaddyPosActual = destino;
                bigDaddyNodoActual = (bigDaddyNodoActual + 1) % rutaBigDaddy.size();
            }
        }
        else
        {
            bigDaddyCaminando = false;
            bigDaddyPosActual = posBigDaddy;
            bigDaddyRotY = 0.0f;
        }

        // --- Apertura del medkit al acercarse ---
        float distCamMK = glm::length(camera.getCameraPosition() - posMedkit);
        puertaTarget = (distCamMK <= distActivacionMedkit) ? 45.0f : 0.0f;
        if (puertaAngulo < puertaTarget) {
            puertaAngulo += puertaSpeed * deltaTime;
            if (puertaAngulo > puertaTarget) puertaAngulo = puertaTarget;
        }
        else if (puertaAngulo > puertaTarget) {
            puertaAngulo -= puertaSpeed * deltaTime;
            if (puertaAngulo < puertaTarget) puertaAngulo = puertaTarget;
        }


        bool* keys = mainWindow.getsKeys();


        //================================================================
        //CONTROL SPOTLIGHTS
        //================================================================
        //CONTROL DEL SPOTLIGHT PARA LA ESTATUA ESCALERAS GIRATORIAS
       // Control de la Spotlight del fénix con la tecla G
        if (keys[GLFW_KEY_G]) {
            if (!gKeyWasPressed) {
                // 1. Alternamos el encendido de la luz del Fénix (Spotlight 5)
                fenixLightOn = !fenixLightOn;

                // 2. Avanzamos la posición de la luz de los carritos (Spotlight 2)
                // Al usar (carritoIndex + 1) % 3, el número cicla siempre entre 0, 1 y 2
                carritoIndex = (carritoIndex + 1) % 3;

                gKeyWasPressed = true; // Bloqueamos hasta soltar la tecla
            }
        }
        else {
            gKeyWasPressed = false;    // Se libera al soltar la tecla
        }

        // --- 1. CONTROL DE TECLA "T" (AVANCE RÁPIDO PARA PRUEBAS) ---
        bool currTeclaT = mainWindow.getsKeys()[GLFW_KEY_T];
        if (currTeclaT) {
            // Mientras mantengas presionada la T, el tiempo vuela (avanza 10 veces más rápido)
            horaSimulada += deltaTime * 5.0f;
        }
        else {
            // Flujo normal del tiempo automático
            horaSimulada += deltaTime * (24.0f / tiempoCicloTotal); // Mapea 120 segundos a 24 horas
        }

        // Reiniciar el día al pasar las 24 horas
        if (horaSimulada >= 24.0f) horaSimulada -= 24.0f;


        // --- 2. CÁLCULO DE LA TRASLACIÓN DEL SOL/LUNA (Movimiento de la Luz) ---
        // Convertimos la hora actual a radianes para mover la luz en un círculo (Eje Y y Eje Z)
        // 6:00 AM = Amanecer, 12:00 PM = Mediodía (Sol arriba), 18:00 PM = Atardecer, 24:00 = Medianoche
        float anguloSolar = glm::radians((horaSimulada - 6.0f) * (360.0f / 24.0f));

        float sunY = -sinf(anguloSolar); // Controla la altura del sol
        float sunZ = cosf(anguloSolar);  // Controla la profundidad del sol
        glm::vec3 direccionLuzDir = glm::normalize(glm::vec3(0.0f, sunY, sunZ));


        // --- 3. CÁLCULO DE FACTORES DE ILUMINACIÓN Y COLOR ---
        float factorDia = 0.0f;
        glm::vec3 colorLuz = glm::vec3(1.0f, 1.0f, 1.0f);

        // Determinar intensidades y tonos según la hora (Fases lumínicas)
        if (horaSimulada >= 6.0f && horaSimulada < 18.0f) {
            // --- ES DE DÍA ---
            // El factor sube a 1.0 al mediodía y baja a 0.0 al atardecer
            factorDia = sinf(glm::radians((horaSimulada - 6.0f) * (180.0f / 12.0f)));

            // Tono ligeramente más cálido/amarillo al amanecer/atardecer
            colorLuz = glm::vec3(1.0f, 0.9f + (0.1f * factorDia), 0.7f + (0.3f * factorDia));
        }
        else {
            // --- ES DE NOCHE ---
            factorDia = 0.0f;
            // Tono azulado/frío místico para la Luna
            colorLuz = glm::vec3(0.6f, 0.7f, 1.0f);
        }

        // Interpolación de intensidades
        float ambientDia = 0.3f, diffuseDia = 0.8f;
        float ambientNoche = 0.04f, diffuseNoche = 0.15f; // Ajustada la noche para que no sea oscuridad absoluta

        float ambientActual = ambientNoche + (ambientDia - ambientNoche) * factorDia;
        float diffuseActual = diffuseNoche + (diffuseDia - diffuseNoche) * factorDia;


        // --- 4. ACTUALIZACIÓN DE LA LUZ DIRECCIONAL ---
        mainLight = DirectionalLight(
            colorLuz.x, colorLuz.y, colorLuz.z,
            ambientActual, diffuseActual,
            direccionLuzDir.x, direccionLuzDir.y, direccionLuzDir.z
        );


     

        // --- Animacion del plasmido (flotacion + latido + rotacion) ---
        plasTime += deltaTime;
        plasRotY += plasRotSpeed * deltaTime;
        if (plasRotY >= 360.0f) plasRotY -= 360.0f;
        float plasOffsetY = plasFloatAmp * sinf(plasTime * plasFloatSpeed);
        float heartbeat = fabsf(sinf(plasTime * plasHeartSpeed));
        float plasEscActual = escPlasmido + plasHeartAmp * heartbeat;

        pointLights[0] = PointLight(
            1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            posPlasmido.x, posPlasmido.y + plasOffsetY, posPlasmido.z,
            0.5f, 0.3f, 0.2f
        );

        pointLights[1] = PointLight(
            0.53f, 0.81f, 0.98f,
            0.0f, 1.2f,
            hpPosCopa.x, hpPosCopa.y + copaOffsetY, hpPosCopa.z,
            0.3f, 0.2f, 0.1f
        );


        // ========================================================
         // INITIALIZACIÓN DE POSICIONES (UNA SOLA VEZ)
         // ========================================================
        if (!hpCamInit)
        {
            // Inicializar cámara aérea con los valores de tus variables globales
            hpCamAereaPos = hpOffsetEscena + glm::vec3(0.0f, hpCamAereaAltura, 18.0f);

            // Las 3 vistas estáticas (Modo 4) solicitadas: ajusta las coordenadas según desees
            hpCamEstaticaPos = {
                hpOffsetEscena + glm::vec3(-35.0f, 12.0f, 35.0f),  // Vista de esquina 1
                hpOffsetEscena + glm::vec3(0.0f, 15.0f, -40.0f),   // Vista panorámica norte
                hpOffsetEscena + glm::vec3(35.0f, 10.0f, 35.0f)    // Vista de esquina 2
            };

            hpCamEstaticaTarget = {
                hpOffsetEscena + glm::vec3(0.0f, 2.0f, 0.0f),      // Mirando al centro
                hpOffsetEscena + glm::vec3(0.0f, 1.0f, 10.0f),
                hpOffsetEscena + glm::vec3(-5.0f, 2.0f, -5.0f)
            };

            // Aseguramos que el índice empiece en la primera vista
            puntoInteresActual = 0;
            hpCamInit = true;
        }

        // ========================================================
        // CONTROLES - SELECCIÓN DE SISTEMA DE CÁMARAS (4 MODOS)
        // ========================================================
        bool currVirgula = keys[GLFW_KEY_COMMA];
        bool currPunto = keys[GLFW_KEY_PERIOD];
        bool currM = keys[GLFW_KEY_M];
        bool currN = keys[GLFW_KEY_N];

        if (currVirgula && !prevTeclaVirgula) {
            modoCamara = 1;
            printf("Camara: 1 - Tercera Persona (Hermione - Flechas + Mouse)\n");
        }
        if (currPunto && !prevTeclaPunto) {
            modoCamara = 2;
            printf("Camara: 2 - Vista Aerea en Plano XZ (Controlada con WASD)\n");
        }
        if (currM && !prevTeclaMenos) {
            modoCamara = 3;
            printf("Camara: 3 - Camara Libre (WASD + Mouse libre)\n");
        }
        if (currN && !prevTeclaN) {
            modoCamara = 4;
            // Si ya estábamos en modo 4, avanzamos secuencialmente a la siguiente sub-vista (0, 1, 2)
            if (modoCamara == 4) {
                puntoInteresActual = (puntoInteresActual + 1) % 3;
            }
            else {
                modoCamara = 4;
            }
            printf("Camara: 4 - Vista Estatica fija. Sub-indice activo: %d\n", puntoInteresActual);
        }
        prevTeclaVirgula = currVirgula;
        prevTeclaPunto = currPunto;
        prevTeclaMenos = currM;
        prevTeclaN = currN;

        // Mantener compatibilidad con tu variable modoCamaraHP
        modoCamaraHP = modoCamara - 1;
        if (modoCamaraHP < 0) modoCamaraHP = 0;

        // Actualizar control de mouse en la clase base siempre
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // ========================================================
        // LÓGICA DE MOVIMIENTO DE LA CÁMARA AÉREA INDEPENDIENTE (MODO 2)
        // ========================================================
        if (modoCamara == 2) {
            if (keys[GLFW_KEY_W]) hpCamAereaPos.z -= hpCamAereaVel * deltaTime;
            if (keys[GLFW_KEY_S]) hpCamAereaPos.z += hpCamAereaVel * deltaTime;
            if (keys[GLFW_KEY_A]) hpCamAereaPos.x -= hpCamAereaVel * deltaTime;
            if (keys[GLFW_KEY_D]) hpCamAereaPos.x += hpCamAereaVel * deltaTime;
            hpCamAereaPos.y = hpOffsetEscena.y + hpCamAereaAltura;
        }

        // ========================================================
// MOVIMIENTO FLUIDO DEL PERSONAJE (PROTEGIDO CONTRA NaN)
// ========================================================
// Extraemos la dirección de visión horizontal del mouse mediante la clase cámara
        glm::vec3 camDirection = camera.getCameraDirection();
        glm::vec3 camForwardXZ(0.0f, 0.0f, -1.0f); // Valor por defecto seguro (Norte)

        // PROTECCIÓN: Solo normalizamos si la cámara no está mirando perfectamente hacia arriba o hacia abajo
        float longitudHorizontal = sqrtf(camDirection.x * camDirection.x + camDirection.z * camDirection.z);
        if (longitudHorizontal > 0.001f)
        {
            camForwardXZ = glm::normalize(glm::vec3(camDirection.x, 0.0f, camDirection.z));
        }

        glm::vec3 camRightXZ = glm::normalize(glm::cross(camForwardXZ, glm::vec3(0.0f, 1.0f, 0.0f)));

        bool hpMoving = false;
        glm::vec3 hpDireccionMovimiento(0.0f);

        // Mapeo con teclas de FLECHAS relativo a la orientación del mouse
        if (keys[GLFW_KEY_UP]) {
            hpDireccionMovimiento += camForwardXZ;
            hpMoving = true;
        }
        if (keys[GLFW_KEY_DOWN]) {
            hpDireccionMovimiento -= camForwardXZ;
            hpMoving = true;
        }
        if (keys[GLFW_KEY_LEFT]) {
            hpDireccionMovimiento -= camRightXZ;
            hpMoving = true;
        }
        if (keys[GLFW_KEY_RIGHT]) {
            hpDireccionMovimiento += camRightXZ;
            hpMoving = true;
        }

        if (hpMoving) {
            // PROTECCIÓN: Solo normalizamos la dirección si realmente hay teclas pulsadas
            if (glm::length(hpDireccionMovimiento) > 0.001f)
            {
                hpDireccionMovimiento = glm::normalize(hpDireccionMovimiento);

                // Modificar posición global del avatar en el mapa
                hpPosX += hpDireccionMovimiento.x * hpVelocidad * deltaTime;
                hpPosZ += hpDireccionMovimiento.z * hpVelocidad * deltaTime;

                // Rotación matemática precisa hacia la dirección real del recorrido
                hpRotPersonaje = (atan2f(-hpDireccionMovimiento.x, -hpDireccionMovimiento.z) / toRadians) + 180.0f;
            }

            // Animación cíclica de las articulaciones de Hermione
            float hpAnim = sinf(glfwGetTime() * 6.5f) * 35.0f;
            hpRotBrazoDer = hpAnim;   hpRotBrazoIzq = -hpAnim;
            hpRotPiernaDer = -hpAnim; hpRotPiernaIzq = hpAnim;
        }
        else {
            // Si está quieta en tercera persona, se alinea elegantemente con el centro de la pantalla
            // PROTECCIÓN: Solo calculamos la rotación estática en Modo 1 para evitar romperla en vistas aéreas
            if (modoCamara == 1 && longitudHorizontal > 0.001f) {
                hpRotPersonaje = (atan2f(-camForwardXZ.x, -camForwardXZ.z) / toRadians) + 180.0f;
            }
            hpRotBrazoDer = hpRotBrazoIzq = hpRotPiernaDer = hpRotPiernaIzq = 0.0f;
        }

        // Posición real del personaje en coordenadas de mundo (dejamos el valor de Y ajustado)
        glm::vec3 hpWorldPos = hpOffsetEscena + glm::vec3(hpPosX, 1.0f, hpPosZ);

        // ========================================================
        // ASIGNACIÓN DE POSICIONES Y OBJETIVOS DE CÁMARA (VIEW MATRIX)
        // ========================================================
        glm::vec3 camPosActiva(0.0f, 0.0f, 0.0f);
        glm::vec3 camTargetActiva(0.0f, 0.0f, 0.0f);
        glm::vec3 upVector(0.0f, 1.0f, 0.0f); // Up vector estándar por defecto en Y

        if (modoCamara == 1)
        {
            // --- MODO 1: TERCERA PERSONA DIRECTA ORBITAL DESDE MOUSE ---
            float distanciaDetras = 15.0f;
            float alturaCamara = 6.0f;

            camPosActiva = hpWorldPos - (camForwardXZ * distanciaDetras) + glm::vec3(0.0f, alturaCamara, 0.0f);
            camTargetActiva = hpWorldPos + glm::vec3(0.0f, 2.0f, 0.0f);
        }
        else if (modoCamara == 2)
        {
            // --- MODO 2: VISTA AÉREA SATELITAL EN PLANO XZ ---
            camPosActiva = hpCamAereaPos;
            camTargetActiva = glm::vec3(hpCamAereaPos.x, 0.0f, hpCamAereaPos.z);

            // Reorientación de vectores para evitar Gimbal Lock al mirar hacia abajo
            upVector = glm::vec3(0.0f, 0.0f, -1.0f);
        }
        else if (modoCamara == 3)
        {
            // --- MODO 3: CÁMARA LIBRE TOTAL (VUELA CON WASD) ---
            camera.keyControl(keys, deltaTime * 5.0f);

            camPosActiva = camera.getCameraPosition();
            camTargetActiva = camPosActiva + camera.getCameraDirection() * 5.0f;
        }
        else
        {
            // --- MODO 4: LAS 3 VISTAS CINEMÁTICAS ESTÁTICAS INTERCAMBIABLES CON LA TECLA "N" ---
            int idx = puntoInteresActual;
            if (idx < 0 || idx >= 3) idx = 0;

            camPosActiva = hpCamEstaticaPos[idx];
            camTargetActiva = hpCamEstaticaTarget[idx];
        }

        // ========================================================
        // SINCRONIZACIÓN Y EJECUCIÓN FINAL DE MATRICES
        // ========================================================
        if (modoCamara != 3)
        {
            camera.setPosition(camPosActiva);
            camera.lookAt(camTargetActiva);
        }
        else
        {
            camPosActiva = camera.getCameraPosition();
            camTargetActiva = camPosActiva + camera.getCameraDirection() * 5.0f;
        }

        glm::mat4 viewMatrix = glm::lookAt(camPosActiva, camTargetActiva, upVector);

        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniform3f(uniformEyePosition, camPosActiva.x, camPosActiva.y, camPosActiva.z);

     



        // --- Engranes del reloj (orbita continua cada frame) ---
        angOrbitaRelojEngrane1 -= velRelojEngrane1 * deltaTime;
        angOrbitaRelojEngrane2 += velRelojEngrane2 * deltaTime;
        angOrbitaRelojEngrane3 -= velRelojEngrane3 * deltaTime;
        angOrbitaRelojEngrane4 += velRelojEngrane4 * deltaTime;
        angOrbitaRelojGalRedE -= velRelojGalRedE * deltaTime;
        angOrbitaRelojPlanetas -= velRelojPlanetas * deltaTime;
        if (angOrbitaRelojEngrane1 <= -360.0f) angOrbitaRelojEngrane1 += 360.0f;
        if (angOrbitaRelojEngrane2 >= 360.0f) angOrbitaRelojEngrane2 -= 360.0f;
        if (angOrbitaRelojEngrane3 <= -360.0f) angOrbitaRelojEngrane3 += 360.0f;
        if (angOrbitaRelojEngrane4 >= 360.0f) angOrbitaRelojEngrane4 -= 360.0f;
        if (angOrbitaRelojGalRedE <= -360.0f) angOrbitaRelojGalRedE += 360.0f;
        if (angOrbitaRelojPlanetas <= -360.0f) angOrbitaRelojPlanetas += 360.0f;

        // --- Farola: calcular posicion mundial de los focos para SpotLights ---
        glm::mat4 baseFarola = glm::mat4(1.0f);
        baseFarola = glm::translate(baseFarola, posFarola);
        baseFarola = glm::rotate(baseFarola, rotFarola.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        baseFarola = glm::rotate(baseFarola, rotFarola.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        baseFarola = glm::rotate(baseFarola, rotFarola.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

     

        

        // ============================================
        // CONTROLES - OBJETOS HARRY POTTER
        // ============================================


        // Toggle snitch -> tecla 3
        bool currAuto3 = keys[GLFW_KEY_3];
        if (currAuto3 && !prevTeclaAuto3) {
            hpSnitchAnimacionActiva = !hpSnitchAnimacionActiva;
            hpSnitchVisible = true;
        }
        prevTeclaAuto3 = currAuto3;

       

        // Actualizacion snitch
        if (hpSnitchAnimacionActiva && hpSnitchVisible) {
            UpdatePathFollower(hpPosSnitch, hpRotCarro, hpSnitchWPActual, hpSnitchRuta, hpSnitchVelocidadRuta, deltaTime, 0.80f);
        }



        

        // ============================================
        // CONTROLES - ANIMACIONES CRASH BANDICOOT
        // ============================================
        //ANIMACION RECORRER PISTA (CARRERA)
        velGral = 0.02f;

        static bool teclaPresionada = false;
        if (keys[GLFW_KEY_2]) {

            if (!teclaPresionada) {
                animPista = !animPista;
                if (!animPista) {
                    i = 0;
                    t = 0.0f;
                    angleY = 0.0f;
                    posCars = glm::vec3(-41.6996f, 0.0f, 0.0000f);
                }
                teclaPresionada = true;
            }
        }
        else {
            teclaPresionada = false;

        }
        if (animPista) {
            recorrerPista(deltaTime / velGral);
        }

        //SE ACTIVA LA ANIMACION SI SE PRESIONO LA TECLA 3
        animacionAplastaCajas(deltaTime);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Skybox segun hora del dia
        if (factorDia >= 0.5f)
            skyboxDia.DrawSkybox(camera.calculateViewMatrix(), projection);
        else
            skyboxNoche.DrawSkybox(camera.calculateViewMatrix(), projection);

       
        

        shaderList[0].UseShader();

        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformColor = shaderList[0].getColorLocation();
        uniformTextureOffset = shaderList[0].getOffsetLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));


        // Uniforms de camara activa (ya configurada arriba con setPosition/lookAt)
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
        glUniform3f(uniformEyePosition, camPosActiva.x, camPosActiva.y, camPosActiva.z);


        


        /*glm::vec3 lowerLightRender = camPosActiva;
        lowerLightRender.y -= 0.3f;
        spotLights[0].SetFlash(lowerLightRender, camDirActiva);*/

        //SPOTLIGHTS CONTROL-------------------------------------------------------------------------------------------------- 
        // ACTUALIZACIÓN DE LA INTENSIDAD ANTES DE ENVIAR AL SHADER
        // --- 1. ACTUALIZAR POSICIÓN DE LA LUZ DE LOS CARRITOS (Spotlight 2) ---
        if (carritoIndex == 0) {
            // Sobre el carrito de Coco (Z = 0.0f)
            spotLights[2].SetPos(glm::vec3(-160.0f, 12.0f, 0.0f));
        }
        else if (carritoIndex == 1) {
            // Sobre el carrito de Crash (Z = 20.0f)
            spotLights[2].SetPos(glm::vec3(-160.0f, 12.0f, 20.0f));
        }
        else if (carritoIndex == 2) {
            // Sobre el carrito de Cortex (Z = -20.0f)
            spotLights[2].SetPos(glm::vec3(-160.0f, 12.0f, -20.0f));
        }


        // --- 2. CONTROLAR EL APAGADO DEL FÉNIX (Spotlight 5) ---
        if (fenixLightOn) {
            spotLights[5].SetIntensity(2.5f, 0.5f); // Encendido original
            spotLights[3].SetIntensity(3.0f, 0.8f); // Cristal (Crash Bandicoot) - ¡NUEVA!
        }
        else {
            spotLights[5].SetIntensity(0.0f, 0.0f); // Apagado
            spotLights[3].SetIntensity(0.0f, 0.0f); // Cristal (Crash Bandicoot) - ¡NUEVA!
        }

        //POINTLIGHTS CONTROL FAROLAS-----------------------------------------------------------------------------------------
        if (horaSimulada >= 16.0f || horaSimulada < 8.0f) {
            // ¡Es de noche! Encendemos la bombilla de la farola con su brillo máximo
            pointLights[2].SetIntensity(0.0f, 1.7f);
            pointLights[3].SetIntensity(0.0f, 1.7f);
            pointLights[4].SetIntensity(0.0f, 1.7f);
            pointLights[5].SetIntensity(0.0f, 1.7f);
            pointLights[6].SetIntensity(0.0f, 1.7f);
            pointLights[7].SetIntensity(0.0f, 1.7f);
            pointLights[8].SetIntensity(0.0f, 1.7f);
            pointLights[9].SetIntensity(0.0f, 1.7f);
        }
        else {
            // ¡Es de día! Apagamos la farola por completo para ahorrar recursos
            pointLights[2].SetIntensity(0.0f, 0.0f);
            pointLights[3].SetIntensity(0.0f, 0.0f);
            pointLights[4].SetIntensity(0.0f, 0.0f);
            pointLights[5].SetIntensity(0.0f, 0.0f);
            pointLights[6].SetIntensity(0.0f, 0.0f);
            pointLights[7].SetIntensity(0.0f, 0.0f);
            pointLights[8].SetIntensity(0.0f, 0.0f);
            pointLights[9].SetIntensity(0.0f, 0.0f);
        }


        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);

        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));




        glm::mat4 model = glm::mat4(1.0f);

        // ============================================
        // RENDER - ESCENARIO
        // ============================================

        // Piso (mesh escalado con textura)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[0]->RenderMesh();

        // Muralla 1 (lado negativo)
        model = glm::mat4(1.0f);
        model = ApplyTRS(model, posMurallaL_1, rotMurallaL_1, glm::vec3(escMurallaL) * scaleMurallaL);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Pared_M.RenderModel();

        // Muralla 2 (lado positivo, rotada 180)
        model = glm::mat4(1.0f);
        model = ApplyTRS(model, posMurallaL_2, rotMurallaL_2, glm::vec3(escMurallaL) * scaleMurallaL);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Pared_M.RenderModel();

        // Farola: poste (padre jerarquico)
        model = ApplyTRS(baseFarola, offsetPaloFarola, rotPaloFarola, glm::vec3(escFarola) * scalePaloFarola);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        PaloFarola_M.RenderModel();


        // Banca del parque
        model = glm::mat4(1.0f);
        model = ApplyTRS(model, posBanca, rotBanca, glm::vec3(escBanca) * scaleBanca);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Banca_M.RenderModel();

        // Bloques de pasto (8 instancias en anillo)
        for (int i = 0; i < 8; i++) { // <-- CAMBIADO DE 12 A 8
            model = glm::mat4(1.0f);
            model = ApplyTRS(model, pastoPos[i], pastoRot[i], glm::vec3(escBloquePasto) * scaleBloquePasto);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
            BloquePasto_M.RenderModel();
        }

        // Reloj de planetas: matriz base comun para todas las piezas
        glm::mat4 baseReloj = glm::mat4(1.0f);
        baseReloj = glm::translate(baseReloj, posReloj);
        baseReloj = glm::rotate(baseReloj, rotReloj.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        baseReloj = glm::rotate(baseReloj, rotReloj.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        baseReloj = glm::rotate(baseReloj, rotReloj.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

        // Reloj: base de planetas
        model = ApplyTRS(baseReloj, offsetBasePlanetas, rotBasePlanetas, glm::vec3(escReloj) * scaleBasePlanetas);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BasePlanetas_M.RenderModel();

        // Reloj: cuerpo base
        model = ApplyTRS(baseReloj, offsetRelojBase, rotRelojBase, glm::vec3(escReloj) * scaleRelojBase);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojBase_M.RenderModel();

        // Reloj: engrane 1 (orbita Y-, desplazado con offset)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane1);
        model = glm::rotate(model, rotRelojEngrane1.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane1.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane1.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane1);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane1_M.RenderModel();

        // Reloj: engrane 2 (orbita Y+)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane2);
        model = glm::rotate(model, rotRelojEngrane2.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane2.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane2.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane2);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane2_M.RenderModel();

        // Reloj: engrane 3 (orbita Y-)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane3);
        model = glm::rotate(model, rotRelojEngrane3.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane3.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane3.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane3);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane3_M.RenderModel();

        // Reloj: engrane 4 (orbita Y+)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojEngrane4 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojEngrane4);
        model = glm::rotate(model, rotRelojEngrane4.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane4.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojEngrane4.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojEngrane4);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojEngrane4_M.RenderModel();

        // Reloj: galaxia/red (orbita Y-)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojGalRedE * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojGalRedE);
        model = glm::rotate(model, rotRelojGalRedE.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojGalRedE.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojGalRedE.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojGalRedE);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojGalRedE_M.RenderModel();

        // Reloj: planetas (orbita Y-)
        model = baseReloj;
        model = glm::rotate(model, angOrbitaRelojPlanetas * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, offsetRelojPlanetas);
        model = glm::rotate(model, rotRelojPlanetas.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotRelojPlanetas.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotRelojPlanetas.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escReloj) * scaleRelojPlanetas);
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        RelojPlanetas_M.RenderModel();

        // ============================================
        // RENDER - AVATAR BIOSHOCK: Big Daddy
        // ============================================

        // Matriz base del Big Daddy padre jerarquico posicion + direccion real de la ruta
        glm::mat4 modelaux_BD = glm::mat4(1.0f);
        modelaux_BD = glm::translate(modelaux_BD, bigDaddyPosActual);
        modelaux_BD = glm::rotate(modelaux_BD, bigDaddyRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

        // Big Daddy: torso (hijo del padre)
        model = modelaux_BD;
        model = glm::translate(model, offsetCuerpo);
        model = glm::rotate(model, rotCuerpo.x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotCuerpo.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotCuerpo.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyCuerpo_M.RenderModel();

        // Big Daddy: brazo izquierdo (animado en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetBrazoIzq);
        model = glm::rotate(model, (rotBrazoIzq.x + anguloBrazoIzq) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotBrazoIzq.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotBrazoIzq.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyBrazoIzq_M.RenderModel();

        // Big Daddy: brazo derecho (animado en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetBrazoDer);
        model = glm::rotate(model, (rotBrazoDer.x + anguloBrazoDer) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotBrazoDer.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotBrazoDer.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyBrazoDer_M.RenderModel();

        // Big Daddy: pierna izquierda (animada en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetPiernaIzq);
        model = glm::rotate(model, (rotPiernaIzq.x + anguloPiernaIzq) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotPiernaIzq.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotPiernaIzq.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyPiernaIzq_M.RenderModel();

        // Big Daddy: pierna derecha (animada en caminata)
        model = modelaux_BD;
        model = glm::translate(model, offsetPiernaDer);
        model = glm::rotate(model, (rotPiernaDer.x + anguloPiernaDer) * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotPiernaDer.y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotPiernaDer.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BigDaddyPiernaDer_M.RenderModel();

        // Big Daddy: taladro (hijo del brazo der, escala -X para espejear el mesh)
        model = modelaux_BD;
        model = glm::translate(model, offsetTaladro * escBigDaddy);
        model = glm::rotate(model, -anguloBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotTaladro.z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, taladroRot * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-escBigDaddy, escBigDaddy, escBigDaddy));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BigDaddyTaladro_M.RenderModel();
        glCullFace(GL_BACK);

        // ============================================
        // RENDER - OBJETOS BIOSHOCK
        // ============================================

        // Medkit: matriz padre comun para las tres piezas
        glm::mat4 modelaux_MK = glm::mat4(1.0f);
        modelaux_MK = glm::translate(modelaux_MK, posMedkit);

        // Medkit: cuerpo central (escala -X para espejear)
        model = modelaux_MK;
        model = glm::scale(model, glm::vec3(-escMedkit, escMedkit, escMedkit));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BolsaMid_M.RenderModel();
        glCullFace(GL_BACK);

        // Medkit: tapa derecha (se abre al acercarse, rotacion +puertaAngulo en X)
        model = modelaux_MK;
        model = glm::translate(model, offsetBolsaDer);
        model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, puertaAngulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-escMedkit, escMedkit, escMedkit));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BolsaDer_M.RenderModel();
        glCullFace(GL_BACK);

        // Medkit: tapa izquierda (se abre al acercarse, rotacion -puertaAngulo en X)
        model = modelaux_MK;
        model = glm::translate(model, offsetBolsaIzq);
        model = glm::rotate(model, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, -puertaAngulo * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-escMedkit, escMedkit, escMedkit));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        glCullFace(GL_FRONT);
        BolsaIzq_M.RenderModel();
        glCullFace(GL_BACK);

        // Plasmido: flota, rota en Y y pulsa en escala (latido)
        glm::mat4 modelaux_PL = glm::mat4(1.0f);
        modelaux_PL = glm::translate(modelaux_PL, posPlasmido + glm::vec3(0.0f, plasOffsetY, 0.0f));
        modelaux_PL = glm::rotate(modelaux_PL, plasRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = modelaux_PL;
        model = glm::scale(model, glm::vec3(plasEscActual));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Plasmido_M.RenderModel();

        // ============================================
        // RENDER - AVATAR HARRY POTTER: Hermione
        // ============================================
        glm::mat4 modelaux;

        // Hermione: cuerpo completo (padre jerarquico)
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpOffsetEscena + glm::vec3(hpPosX, 2.0f, hpPosZ));
        model = glm::rotate(model, hpRotPersonaje * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Hermione_HP_M.RenderModel();

        // Hermione: brazo derecho (hijo del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(0.5f, 0.8f, 0.0f));
        modelaux = glm::rotate(modelaux, hpRotBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BrazoDer_HP_M.RenderModel();

        // Hermione: brazo izquierdo (hijo del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(-0.3f, 0.8f, 0.0f));
        modelaux = glm::rotate(modelaux, hpRotBrazoIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BrazoIzq_HP_M.RenderModel();
        // Hermione: pierna derecha (hija del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(-0.1f, -0.7f, 0.0f));
        //modelaux = glm::rotate(modelaux, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // giro fijo
        modelaux = glm::rotate(modelaux, hpRotPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // animación
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        PiernaDer_HP_M.RenderModel();

        // Hermione: pierna izquierda (hija del cuerpo)
        modelaux = model;
        modelaux = glm::translate(modelaux, glm::vec3(0.1f, -0.7f, 0.0f));
        //modelaux = glm::rotate(modelaux, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f)); // giro fijo
        modelaux = glm::rotate(modelaux, hpRotPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f)); // animación
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelaux));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        PiernaIzq_HP_M.RenderModel();

        // ============================================
        // RENDER - OBJETOS HARRY POTTER
        // ============================================

        // Bolsa de Hermione (posicion fija en la escena)
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosBolsa);
        model = glm::scale(model, glm::vec3(3.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Bolsa_HP_M.RenderModel();

        // Giratiempo (escala reducida a 0.5)
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosGiratiempo);
        model = glm::scale(model, glm::vec3(1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Giratiempo_HP_M.RenderModel();

        // Libro de hechizos
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosLibro);
        model = glm::scale(model, glm::vec3(2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        LibroHechizos_HP_M.RenderModel();

        // Snitch dorada: solo si es visible (animacion de vuelo senoidal)
        if (hpSnitchVisible)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, hpPosSnitch);
            model = glm::scale(model, glm::vec3(2.4f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            Snitch_HP_M.RenderModel();
        }

        // Copa de los tres magos
        model = glm::mat4(1.0f);
        model = glm::translate(model, hpPosCopa + glm::vec3(0.0f, copaOffsetY, 0.0f));
        model = glm::rotate(model, copaRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(copaEscActual));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Copa_HP_M.RenderModel();

        //maletas equipaje HP
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(50.0f, -2.0f, 50.0f));
        //model = glm::rotate(model, copaRotY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Maletas_HP_M.RenderModel();

        //ESTATUAS PIEZA AJEDREZ HP
        //Estatua 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-150.0f, -2.0f, 160.0f));
        model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EstatuaAjedrez_HP_M.RenderModel();

        //Estatua 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-150.0f, -2.0f, -160.0f));
        model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EstatuaAjedrez_HP_M.RenderModel();

        //Estatua 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(150.0f, -2.0f, 160.0f));
        model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EstatuaAjedrez_HP_M.RenderModel();

        //Estatua 4
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(150.0f, -2.0f, -160.0f));
        model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EstatuaAjedrez_HP_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -120.0f));
        model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(12.0f, 12.0f, 12.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        EstatuaEscaleraGiratoria_HP_M.RenderModel();
        
        // ============================================
        // RENDER - CRASH ANIMACION APLASTA CAJAS
        // ============================================
        
        //Jerarquia persibaje crash bandicoot
        //EL PADRE (Cuerpo/Torso)
        glm::mat4 modelCrash = glm::mat4(1.0f);
        modelCrash = glm::translate(modelCrash, glm::vec3(-63.0f, 2.0f, 80.0f));
        modelCrash = glm::rotate(modelCrash, (rotacionTornado) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCrash = glm::rotate(modelCrash, 180 *toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCrash = glm::scale(modelCrash, glm::vec3(3.0f));
        // Dibujar Cuerpo
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCrash));
        Crash_Cuerpo_M.RenderModel();

        // Brazo Derecho
        glm::mat4 modelauxCrash = modelCrash; // Copiamos la base del padre
        modelauxCrash = glm::translate(modelauxCrash, glm::vec3(-0.252f, 0.0049f, 0.0255f));
        modelauxCrash = glm::rotate(modelauxCrash, crashRotBrazoDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelauxCrash));
        Crash_BrazoDer_M.RenderModel();

        // Brazo Izquierdo
        modelauxCrash = modelCrash; // Reset a la base del padre
        modelauxCrash = glm::translate(modelauxCrash, glm::vec3(0.235f, -0.005f, 0.019f));
        modelauxCrash = glm::rotate(modelauxCrash, crashRotBrazoIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelauxCrash));
        Crash_BrazoIzq_M.RenderModel();

        // Pierna Derecha
        modelauxCrash = modelCrash;
        modelauxCrash = glm::translate(modelauxCrash, glm::vec3(0.0f, -0.589f, 0.008f));
        modelauxCrash = glm::rotate(modelauxCrash, crashRotPiernaDer * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelauxCrash));
        Crash_PiernaDer_M.RenderModel();

        // Pierna Izquierda
        modelauxCrash = modelCrash;
        modelauxCrash = glm::translate(modelauxCrash, glm::vec3(-0.005f, -0.587f, 0.008f));
        modelauxCrash = glm::rotate(modelauxCrash, crashRotPiernaIzq * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelauxCrash));
        Crash_PiernaIzq_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, posBolaRoca);
        model = glm::rotate(model, rotacionRocaZ * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(8.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        BolaRoca_M.RenderModel();

        // --- RENDER DE CAJAS APLASTABLES ---

        // Aku
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaAku);
        model = glm::scale(model, glm::vec3(5.0f, escAku, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaAku_M.RenderModel();

        // Cara Crash
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaCaraCrash);
        model = glm::scale(model, glm::vec3(5.0f, escCara, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaCaraCrash_M.RenderModel();

        // CheckPoint
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaCheckPoint);
        model = glm::scale(model, glm::vec3(5.0f, escCheck, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaCheckPoint_M.RenderModel();

        // Flecha Arriba
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaFlechaArriba);
        model = glm::scale(model, glm::vec3(5.0f, escFlecha, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaFlechaArriba_M.RenderModel();

        // Metal
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaMetalExcl);
        model = glm::scale(model, glm::vec3(5.0f, escMetal, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaMetalExclamation_M.RenderModel();

        // Nitro
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaNitro);
        model = glm::scale(model, glm::vec3(5.0f, escNitro, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaNitro_M.RenderModel();

        // Nitro
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaNitroExcl);
        model = glm::scale(model, glm::vec3(5.0f, escNitroExcl, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaNitroExclamation_M.RenderModel();

        // Normal
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaNormal);
        model = glm::scale(model, glm::vec3(5.0f, escNormal, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaNormalCrash_M.RenderModel();

        // Question
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaQuestion);
        model = glm::scale(model, glm::vec3(5.0f, escQuest, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaQuestion_M.RenderModel();

        // TNT
        model = glm::mat4(1.0f);
        model = glm::translate(model, posCajaTNT);
        model = glm::scale(model, glm::vec3(5.0f, escTNT, 5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        CajaTNT_M.RenderModel();

        // El tótem siempre se dibuja en su posición final
        model = glm::mat4(1.0f);
        model = glm::translate(model, posTotem1Crash);
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Totem1Crash_M.RenderModel();

        // ============================================
        // RENDER - OBJETOS CRASH
        // ============================================
        
        float tiempo = glfwGetTime();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-160.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BaseCilindro_M.RenderModel();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-160.0f, 4.0f, 0.0f));
        model = glm::scale(model, glm::vec3(4.0f));
        model = glm::rotate(model, glm::radians(tiempo * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Crash_GoKart_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-160.0f, -2.0f, 20.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BaseCilindro_M.RenderModel();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-160.0f, 4.0f, 20.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, glm::radians(tiempo * -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CocoOnGoKart_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-160.0f, -2.0f, -20.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        BaseCilindro_M.RenderModel();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-160.0f, 4.0f, -20.0f));
        model = glm::scale(model, glm::vec3(2.0f));
        model = glm::rotate(model, glm::radians(tiempo * -50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CortexOnGoKart_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(165.0f, -1.0f, 0.0f));
        model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        ArbolWumpa_M.RenderModel();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(165.0f, -1.0f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        ArbolWumpa_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(160.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Totem2Crash_M.RenderModel();

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(160.0f, 6.0f + glm::sin(tiempo * 2.0f) * 0.5f, 0.0f));
        model = glm::rotate(model, glm::radians(tiempo * 50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
        CrystalCrash_M.RenderModel();


        // --- Configuracion del camino ---
        float tamañoLado = 360.0f; // Qué tan grande es el cuadrado
        int frutasPorLado = 50;    // Cuántas frutas hay en cada línea
        float paso = tamañoLado / frutasPorLado;
        float tiempoGlobal = glfwGetTime();

        // Posición inicial del cuadrado
        float baseX = -180.0f;
        float baseZ = -180.0f;

        for (int i = 0; i < 4; i++) { // 4 lados del cuadrado
            for (int j = 0; j < frutasPorLado; j++) {
                float x = 0, z = 0;

                // Calcular posición según el lado actual
                if (i == 0) { x = j * paso; z = 0; }             // Lado norte
                if (i == 1) { x = tamañoLado; z = j * paso; }    // Lado este
                if (i == 2) { x = (frutasPorLado - j) * paso; z = tamañoLado; } // Lado sur
                if (i == 3) { x = 0; z = (frutasPorLado - j) * paso; } // Lado oeste

                model = glm::mat4(1.0f);

                // --- ANIMACIÓN INDIVIDUAL ---
                // Usamos 'j' e 'i' para que no todas brinquen al mismo tiempo (desfase)
                float desfase = (i * frutasPorLado + j) * 0.5f;
                float brincoY = glm::sin(tiempoGlobal * 3.0f + desfase) * 0.8f;
                float rotacion = tiempoGlobal * 100.0f;

                // --- TRANSFORMACIONES ---
                model = glm::translate(model, glm::vec3(baseX + x, 1.0f + brincoY, baseZ + z));
                model = glm::rotate(model, glm::radians(rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(1.0f));

                glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
                Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
                FrutaWumpa_M.RenderModel();
            }
        }

        //MASCARAS AKU CON ANIMACION DE HUMO
        //animacion humo
        rotHumoAku += 0.5f * (deltaTime * 50.0f);
        if (creceHumoAltoAku >= 20.0f) {
            creceHumoAltoAku = 0.0f;
            creceHumoAnchoAku = 0.0f;
        }
        creceHumoAltoAku += 0.1f * (deltaTime * 50.0f);
        if (creceHumoAnchoAku <= 7.0f)
            creceHumoAnchoAku += 0.1f * (deltaTime * 50.0f);

        //instancia mascara aku1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-100.0f, 2.0f, 100.0f));
        model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
        //model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        //selecicion de modelo enfuncion si es dia o noche luz ojos cocinada
        if (horaSimulada >= 16.0f || horaSimulada < 8.0f) {
            MascaraAkuNoche_M.RenderModel();
        }
        else {
            MascaraAku_M.RenderModel();
        }
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(creceHumoAnchoAku, creceHumoAltoAku, creceHumoAnchoAku));
        model = glm::rotate(model, rotHumoAku * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        //instancia mascara aku2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(100.0f, 2.0f, 100.0f));
        model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
        //model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        //selecicion de modelo enfuncion si es dia o noche luz ojos cocinada
        if (horaSimulada >= 16.0f || horaSimulada < 8.0f) {
            MascaraAkuNoche_M.RenderModel();
        }
        else {
            MascaraAku_M.RenderModel();
        }
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(creceHumoAnchoAku, creceHumoAltoAku, creceHumoAnchoAku));
        model = glm::rotate(model, rotHumoAku * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        //instancia mascara uka1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(100.0f, 2.0f, -100.0f));
        model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        //model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        MascaraUka_M.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(creceHumoAnchoAku, creceHumoAltoAku, creceHumoAnchoAku));
        model = glm::rotate(model, rotHumoAku * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        //instancia mascara uka2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-100.0f, 2.0f, -100.0f));
        model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelaux = model;
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        //model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        MascaraUka_M.RenderModel();
        model = modelaux;
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
        model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(creceHumoAnchoAku, creceHumoAltoAku, creceHumoAnchoAku));
        model = glm::rotate(model, rotHumoAku * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // ========================
        // PISTA (MODELO BASE)
        // ========================
        glm::mat4 modelPista = glm::mat4(1.0f);
        /*modelPista = glm::translate(modelPista, glm::vec3(0.0f, -0.7f, 0.0f));
        modelPista = glm::rotate(modelPista, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelPista = glm::scale(modelPista, glm::vec3(1.0f, 1.0f, 1.0f));*/
        modelPista = glm::translate(modelPista, posPista);
        modelPista = glm::scale(modelPista, glm::vec3(3.4f, 4.0f, 4.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelPista));
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        Pista_M.RenderModel();

        // ========================
        // CARRO 1 CRASH Y GRUNT
        // ========================
        glm::mat4 modelCarro1 = modelPista;
        modelCarro1 = glm::translate(modelCarro1, glm::vec3(posCars.x + offsetCentroPistaCar1, posCars.y + offsetAlturaPistaCar1, -posCars.z));
        modelCarro1 = glm::rotate(modelCarro1, glm::radians(-angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        modelCarro1 = glm::rotate(modelCarro1, -180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCarro1 = glm::rotate(modelCarro1, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        modelCarro1 = glm::scale(modelCarro1, glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat4 modelCarro1Aux = modelCarro1;  // Variable específica para el carro 1
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro1Aux));
        SteamCar1_M.RenderModel();


        // HUMO DEL CARRO 1
        rotHumoCar1 += 5.0f;
        if (creceHumoCar1 >= 5.0f) {
            creceHumoCar1 = 2.0f;
        }
        creceHumoCar1 += 0.5f * (deltaTime*50.0f);

        glm::mat4 modelHumoCar1 = modelCarro1Aux;
        modelHumoCar1 = glm::translate(modelHumoCar1, glm::vec3(0.0f, 0.3f, 0.0f));
        modelHumoCar1 = glm::rotate(modelHumoCar1, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        modelHumoCar1 = glm::scale(modelHumoCar1, glm::vec3(2.0f, creceHumoCar1, 1.0f));
        modelHumoCar1 = glm::rotate(modelHumoCar1, rotHumoCar1 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHumoCar1));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        // RUEDAS DEL CARRO 1
        glm::mat4 modelRuedaCar1;  // Variable específica para ruedas del carro 1
        //rotacion ruedas= w=v/r
        rotLlantaCar1 += ((deltaTime * 50.0f) / velGral) / 0.03f; //0.03=radio aprox ruedas respecto a la escala de opengl
        // Llanta delantera izquierda
        modelRuedaCar1 = modelCarro1Aux;
        modelRuedaCar1 = glm::translate(modelRuedaCar1, glm::vec3(0.67f, 0.38f, 1.22f));
        modelRuedaCar1 = glm::rotate(modelRuedaCar1, rotLlantaCar1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar1));
        LlantaFD_Crash_M.RenderModel();

        // Llanta delantera derecha
        modelRuedaCar1 = modelCarro1Aux;
        modelRuedaCar1 = glm::translate(modelRuedaCar1, glm::vec3(-0.67f, 0.38f, 1.22f));
        modelRuedaCar1 = glm::rotate(modelRuedaCar1, rotLlantaCar1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar1));
        LlantaFD_Crash_M.RenderModel();

        // Llanta trasera izquierda
        modelRuedaCar1 = modelCarro1Aux;
        modelRuedaCar1 = glm::translate(modelRuedaCar1, glm::vec3(0.67f, 0.38f, -1.06f));
        modelRuedaCar1 = glm::rotate(modelRuedaCar1, rotLlantaCar1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar1));
        LlantaFD_Crash_M.RenderModel();

        // Llanta trasera derecha
        modelRuedaCar1 = modelCarro1Aux;
        modelRuedaCar1 = glm::translate(modelRuedaCar1, glm::vec3(-0.67f, 0.38f, -1.06f));
        modelRuedaCar1 = glm::rotate(modelRuedaCar1, rotLlantaCar1 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar1));
        LlantaFD_Crash_M.RenderModel();



        // ========================
        // CARRO 2 MASTER CHIEF Y COCO
        // ========================
        glm::mat4 modelCarro2 = modelPista;  // Sigue la jerarquía de la pista
        modelCarro2 = glm::translate(modelCarro2, glm::vec3(posCars.x + offsetCentroPistaCar2, posCars.y + offsetAlturaPistaCar2, -posCars.z));
        modelCarro2 = glm::rotate(modelCarro2, glm::radians(-angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        //modelCarro2 = glm::rotate(modelCarro2, -180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCarro2 = glm::rotate(modelCarro2, glm::radians(-angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        modelCarro2 = glm::rotate(modelCarro2, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCarro2 = glm::scale(modelCarro2, glm::vec3(0.5f, 0.5f, 0.5f));
        glm::mat4 modelCarro2Aux = modelCarro2;  // Variable específica para el carro 2

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro2Aux));
        SteamCar2_M.RenderModel();

        // ACTUALIZAR ANIMACIONES DEL CARRO 2
        rotHumoCar2 += 0.5f;
        if (creceHumoCar2 >= 5.0f) {
            creceHumoCar2 = 3.0f;
        }
        creceHumoCar2 += 0.1f * (deltaTime * 50.0f);

        // HUMO DEL CARRO 2 (HIJO DEL CARRO 2)
        glm::mat4 modelHumoCar2 = modelCarro2Aux;
        modelHumoCar2 = glm::translate(modelHumoCar2, glm::vec3(-0.6f, 4.0f, -0.05f));
        modelHumoCar2 = glm::rotate(modelHumoCar2, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelHumoCar2 = glm::scale(modelHumoCar2, glm::vec3(1.0f, creceHumoCar2, 1.0f));
        modelHumoCar2 = glm::rotate(modelHumoCar2, rotHumoCar2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHumoCar2));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        // RUEDAS DEL CARRO 2
        glm::mat4 modelRuedaCar2;  // Variable específica para ruedas del carro 2
        //rotacion ruedas= w=v/r
        rotLlantaCar2 += ((deltaTime * 50.0f) / velGral) / 0.03f; //0.03=radio aprox ruedas respecto a la escala de opengl
        // Llanta delantera izquierda
        modelRuedaCar2 = modelCarro2Aux;
        modelRuedaCar2 = glm::translate(modelRuedaCar2, glm::vec3(-0.55f, 1.35f, 0.5f));
        modelRuedaCar2 = glm::rotate(modelRuedaCar2, rotLlantaCar2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar2));
        RuedaSteamCar2_M.RenderModel();

        // Llanta delantera derecha
        modelRuedaCar2 = modelCarro2Aux;
        modelRuedaCar2 = glm::translate(modelRuedaCar2, glm::vec3(-0.55f, 1.35f, -2.0f));
        modelRuedaCar2 = glm::rotate(modelRuedaCar2, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelRuedaCar2 = glm::rotate(modelRuedaCar2, -rotLlantaCar2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar2));
        RuedaSteamCar2_M.RenderModel();

        // Llanta trasera izquierda
        modelRuedaCar2 = modelCarro2Aux;
        modelRuedaCar2 = glm::scale(modelRuedaCar2, glm::vec3(1.7f, 1.7f, 1.7f));
        modelRuedaCar2 = glm::translate(modelRuedaCar2, glm::vec3(1.87f, 1.05f, 0.45f));
        modelRuedaCar2 = glm::rotate(modelRuedaCar2, rotLlantaCar2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar2));
        RuedaSteamCar2_M.RenderModel();

        // Llanta trasera derecha
        modelRuedaCar2 = modelCarro2Aux;
        modelRuedaCar2 = glm::scale(modelRuedaCar2, glm::vec3(1.7f, 1.7f, 1.7f));
        modelRuedaCar2 = glm::rotate(modelRuedaCar2, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelRuedaCar2 = glm::translate(modelRuedaCar2, glm::vec3(-1.87f, 1.05f, 1.3f));
        modelRuedaCar2 = glm::rotate(modelRuedaCar2, -rotLlantaCar2 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRuedaCar2));
        RuedaSteamCar2_M.RenderModel();

        // ========================
        // CARRO 3 HARRY POTTER EN MOTO
        // ========================
        glm::mat4 modelCarro3 = modelPista;  // Sigue la jerarquía de la pista
        //glm::mat4 modelCarro3 = glm::mat4(1.0f);

        modelCarro3 = glm::translate(modelCarro3, glm::vec3(0.0f, -2.0f, 0.0f));
        modelCarro3 = glm::translate(modelCarro3, glm::vec3(posCars.x + offsetCentroPistaCar3, posCars.y + offsetAlturaPistaCar3, -posCars.z));
        modelCarro3 = glm::rotate(modelCarro3, glm::radians(-angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        modelCarro3 = glm::rotate(modelCarro3, 180.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCarro3 = glm::rotate(modelCarro3, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        //modelCarro3 = glm::rotate(modelCarro3, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        modelCarro3 = glm::scale(modelCarro3, glm::vec3(1.5f, 1.5f, 1.5f));
        glm::mat4 modelCarro3Aux = modelCarro3;  // Variable específica para el carro 3
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro3Aux));
        SteamCar3_M.RenderModel();

        // HUMO DEL CARRO 3
        glm::mat4 modelHumoCar3 = modelCarro3Aux;
        rotHumoCar3 += 4.0f;
        if (creceHumoCar3 >= 5.0f) {
            creceHumoCar3 = 3.0f;
        }
        creceHumoCar3 += 0.5f * (deltaTime * 50.0f);
        modelHumoCar3 = glm::translate(modelHumoCar3, glm::vec3(0.0f, 0.7f, 0.5f));
        modelHumoCar3 = glm::rotate(modelHumoCar3, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        modelHumoCar3 = glm::scale(modelHumoCar3, glm::vec3(0.7f, creceHumoCar3, 0.7f));
        modelHumoCar3 = glm::rotate(modelHumoCar3, rotHumoCar3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHumoCar3));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        modelHumoCar3 = modelCarro3Aux;
        modelHumoCar3 = glm::translate(modelHumoCar3, glm::vec3(0.0f, 0.3f, 0.8f));
        modelHumoCar3 = glm::rotate(modelHumoCar3, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        modelHumoCar3 = glm::scale(modelHumoCar3, glm::vec3(0.6f, creceHumoCar3 - 3.0f, 0.6f));
        modelHumoCar3 = glm::rotate(modelHumoCar3, rotHumoCar3 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelHumoCar3));
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        humoTexture.UseTexture();
        Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
        meshList[1]->RenderMesh();
        glDisable(GL_BLEND);

        //Rueda central
        //rotacion ruedas= w=v/r
        rotLlantaCar3 += ((deltaTime * 50.0f) / velGral) / 0.03f; //0.03=radio aprox ruedas respecto a la escala de opengl
        modelCarro3 = modelCarro3Aux;
        modelCarro3 = glm::translate(modelCarro3, glm::vec3(0.0f, 0.25f, -0.45f));
        modelCarro3 = glm::rotate(modelCarro3, rotLlantaCar3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro3));
        LlantaSteamCar3_M.RenderModel();

        //Rueda Trasera
        modelCarro3 = modelCarro3Aux;
        modelCarro3 = glm::translate(modelCarro3, glm::vec3(0.0f, 0.25f, -0.97f));
        modelCarro3 = glm::rotate(modelCarro3, rotLlantaCar3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro3));
        LlantaSteamCar3_M.RenderModel();

        //Rueda delantera izquierda
        modelCarro3 = modelCarro3Aux;
        modelCarro3 = glm::scale(modelCarro3, glm::vec3(1.15f, 1.15f, 1.15f));
        modelCarro3 = glm::translate(modelCarro3, glm::vec3(0.23f, 0.255f, 0.89f));
        modelCarro3 = glm::rotate(modelCarro3, rotLlantaCar3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro3));
        LlantaSteamCar3_M.RenderModel();

        //Rueda delantera derecha
        modelCarro3 = modelCarro3Aux;
        modelCarro3 = glm::scale(modelCarro3, glm::vec3(1.15f, 1.15f, 1.15f));
        modelCarro3 = glm::translate(modelCarro3, glm::vec3(-0.23f, 0.255f, 0.89f));
        modelCarro3 = glm::rotate(modelCarro3, rotLlantaCar3 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCarro3));
        LlantaSteamCar3_M.RenderModel();
        //
        //FIN PISTA Y AUTOS
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //INICIO TREN HOWARTS Y ANIMACION
        

        // Avanzar el tiempo global del tren de forma infinita y circular
        tiempoGlobalTren += velocidadTren * deltaTime;
        if (tiempoGlobalTren > 1.0f) tiempoGlobalTren -= 1.0f;

        // Rotación autónoma de las ruedas
        wheelSpeed = 50.0f;
        rotRuedaCabinaHowarts += wheelSpeed * deltaTime;
        float anguloFinal = rotRuedaCabinaHowarts * 5.0f * toRadians;

        

        // Variables auxiliares compartidas para optimizar el cálculo de Roll
        glm::vec3 posFutura;
        glm::vec3 dirFutura;
        float deltaYaw;


        // =================================================================
        // =================== LOCOMOTORA (CABINA PRINCIPAL) =================
        // =================================================================
        {
            float t_cabina = tiempoGlobalTren;
            glm::vec3 posCabina = InterpolacionCatmullRom(keyframesCamino, t_cabina);
            glm::vec3 posAdelanteCabina = InterpolacionCatmullRom(keyframesCamino, t_cabina + 0.001f);
            glm::vec3 direccionCabina = glm::normalize(posAdelanteCabina - posCabina);

            glm::mat4 trainModel = glm::mat4(1.0f);
            trainModel = glm::translate(trainModel, posCabina);
            //colocamos el audio de tren siguiendo la posicion de la cabina

            // 1. YAW: Orientación en el plano XZ (Giro en Y)
            float anguloGiroCabina = atan2(direccionCabina.x, direccionCabina.z);
            trainModel = glm::rotate(trainModel, anguloGiroCabina, glm::vec3(0.0f, 1.0f, 0.0f));

            // 2. PITCH: Inclinación de subida/bajada (Eje X local)
            float pitchCabina = -asin(direccionCabina.y);
            trainModel = glm::rotate(trainModel, pitchCabina, glm::vec3(1.0f, 0.0f, 0.0f));

            // Efecto serpenteante extra acoplado de forma local
            float balanceoMagico = glm::sin(tiempoGlobalTren * 30.0f) * 0.05f;
            trainModel = glm::rotate(trainModel, balanceoMagico, glm::vec3(1.0f, 0.0f, 0.0f));

            // 3. ROLL: Peralte lateral automático en curvas (Eje Z local)
            posFutura = InterpolacionCatmullRom(keyframesCamino, t_cabina + 0.005f);
            dirFutura = glm::normalize(posFutura - posCabina);
            deltaYaw = atan2(dirFutura.x, dirFutura.z) - anguloGiroCabina;
            if (deltaYaw > glm::pi<float>()) deltaYaw -= glm::two_pi<float>();
            if (deltaYaw < -glm::pi<float>()) deltaYaw += glm::two_pi<float>();
            float rollCabina = deltaYaw * 4.0f;
            trainModel = glm::rotate(trainModel, rollCabina, glm::vec3(0.0f, 0.0f, 1.0f));

            trainModel = glm::scale(trainModel, glm::vec3(2.0f, 2.0f, 2.0f));
            glm::mat4 trainModelAux = trainModel; // Matriz base heredable con orientación 3D completa



            //FARO TREN DE HOWARTS
            // 1. Calculamos el desfase del faro respecto al centro de tu malla local.
    //    (Prueba con Z en 12.0f para llevarlo al frente de la caldera y Y en 1.8f para la altura)
            glm::vec4 posicionLocalFaro = glm::vec4(0.0f, -1.2f, 9.0f, 1.0f);
            glm::vec3 posicionMundoFaro = glm::vec3(trainModelAux * posicionLocalFaro);

            // 2. Extraemos el vector de dirección frontal del tren desde la matriz orientada.
            //    Si tu modelo camina hacia adelante en el eje -Z, usa la columna [2] invertida.
            //    Si camina hacia +Z de forma nativa, quita el signo menos.
            glm::vec3 direccionMundoFaro = glm::vec3(trainModelAux[2]);

            // 3. Aprovechamos el método SetFlash de tu SpotLight.cpp que actualiza y normaliza todo de golpe
            spotLights[4].SetFlash(posicionMundoFaro, direccionMundoFaro);

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            CabinaExpresoHowarts.RenderModel();

            rotHumoTren += 5.0f * (deltaTime * 50.0f);
            if (creceHumoAltoTren >= 20.0f) {
                creceHumoAltoTren = 10.0f;
            }
			creceHumoAltoTren += 0.5f * (deltaTime * 50.0f);
            //salida humo principal
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, 3.0f, 5.5f));
            trainModel = glm::scale(trainModel, glm::vec3(6.0f, creceHumoAltoTren, 6.0f));
            trainModel = glm::rotate(trainModel, rotHumoTren   * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            humoTexture.UseTexture();
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            meshList[1]->RenderMesh();
            //salida humo secundario
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, 3.0f, -3.5f));
            trainModel = glm::scale(trainModel, glm::vec3(3.0f, creceHumoAltoTren*0.5f, 3.0f));
            trainModel = glm::rotate(trainModel, -rotHumoTren * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            humoTexture.UseTexture();
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            meshList[1]->RenderMesh();
            glDisable(GL_BLEND);

            // --- EJE DELANTERO 1 ---
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, -3.5f, 7.5f));
            trainModel = glm::rotate(trainModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            EjeDelanteroExpresoHowarts.RenderModel();

            // --- EJE DELANTERO 2 ---
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, -3.5f, 3.5f));
            trainModel = glm::rotate(trainModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            EjeDelanteroExpresoHowarts.RenderModel();

            // --- EJE TRASERO 1 ---
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, -2.45f, -0.35f));
            trainModel = glm::rotate(trainModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            EjeTraseroExpresoHowarts.RenderModel();

            // --- EJE TRASERO 2 (CENTRAL) ---
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, -2.45f, -5.0f));
            trainModel = glm::rotate(trainModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            EjeTraseroExpresoHowarts.RenderModel();

            // --- EJE TRASERO 3 ---
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, -2.45f, -10.1f));
            trainModel = glm::rotate(trainModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            EjeTraseroExpresoHowarts.RenderModel();

            // =================================================================
            // --- MECÁNICA LADO IZQUIERDO ---
            // =================================================================
            float r = 0.8f;
            trainModel = trainModelAux;
            float offsetY = r * glm::sin(anguloFinal);
            float offsetZ = r * -glm::cos(anguloFinal);

            glm::vec3 posicionPerno = glm::vec3(-0.08f, -2.45f + offsetY, -5.0f + offsetZ);
            trainModel = glm::translate(trainModel, posicionPerno);
            glm::mat4 matrizPernoMovel = trainModel;
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            BielaMotrizIzquierdaExpresoHowarts.RenderModel();

            // Biela Cabeceo Izquierda
            trainModel = matrizPernoMovel;
            float Y_fijo_piston = -3.4f;
            float Z_fijo_piston = 1.0f;
            glm::vec3 puntoDestino = glm::vec3(-0.1f, Y_fijo_piston, Z_fijo_piston);
            glm::vec3 direccion = puntoDestino - posicionPerno;
            float anguloCompensacion = atan2(direccion.y, direccion.z);
            trainModel = glm::rotate(trainModel, -anguloCompensacion, glm::vec3(1.0f, 0.0f, 0.0f));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            BielaIzquierdaExpresoHowarts.RenderModel();

            // Pistón Izquierdo
            float longitudBielaReal = 4.5f;
            float posicionZ_Piston = posicionPerno.z + (longitudBielaReal * glm::cos(anguloCompensacion));
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(0.0f, -2.7f, posicionZ_Piston + 3.8f));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            PistonIzquierdoExpresoHowarts.RenderModel();

            // =================================================================
            // --- MECÁNICA LADO DERECHO ---
            // =================================================================
            float anguloDerecho = anguloFinal + glm::half_pi<float>(); // Desfase 90°

            float r_der = 0.8f;
            trainModel = trainModelAux;
            float offsetY_der = r_der * glm::sin(anguloDerecho);
            float offsetZ_der = r_der * -glm::cos(anguloDerecho);

            glm::vec3 posicionPernoDerecho = glm::vec3(-2.1f, -2.45f + offsetY_der, -5.0f + offsetZ_der);
            trainModel = glm::translate(trainModel, posicionPernoDerecho);
            glm::mat4 matrizPernoMovelDerecho = trainModel;
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            BielaMotrizDerechaExpresoHowarts.RenderModel();

            // Biela Cabeceo Derecha
            trainModel = matrizPernoMovelDerecho;
            float Y_fijo_piston_der = -3.5f;
            float Z_fijo_piston_der = 0.9f;
            glm::vec3 puntoDestinoDerecho = glm::vec3(0.0f, Y_fijo_piston_der, Z_fijo_piston_der);
            glm::vec3 direccionDerecha = puntoDestinoDerecho - posicionPernoDerecho;
            float anguloCompensacionDerecho = atan2(direccionDerecha.y, direccionDerecha.z);
            trainModel = glm::rotate(trainModel, -anguloCompensacionDerecho, glm::vec3(1.0f, 0.0f, 0.0f));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            BielaDerechaExpresoHowarts.RenderModel();

            // Pistón Derecho
            float longitudBielaRealDerecha = 4.5f;
            float posicionZ_PistonDerecho = posicionPernoDerecho.z + (longitudBielaRealDerecha * glm::cos(anguloCompensacionDerecho));
            trainModel = trainModelAux;
            trainModel = glm::translate(trainModel, glm::vec3(-2.4f, -2.7f, posicionZ_PistonDerecho + 3.8f));
            Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(trainModel));
            PistonDerechoExpresoHowarts.RenderModel();
        }

        // =================================================================
        // ==================== CONFIGURACIÓN DE VAGONES ===================
        // =================================================================
        // Habilitar mezcla para transparencias
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        float espacioCabinaAVagon = 0.05f;
        float espacioEntreVagones = 0.035f;

        // --- VAGÓN 1 ---
        float t_wagon1 = tiempoGlobalTren - espacioCabinaAVagon;
        if (t_wagon1 < 0.0f) t_wagon1 += 1.0f;
        {
            glm::vec3 posWagon1 = InterpolacionCatmullRom(keyframesCamino, t_wagon1);
            glm::vec3 posAdelanteWagon1 = InterpolacionCatmullRom(keyframesCamino, t_wagon1 + 0.001f);
            glm::vec3 direccionWagon1 = glm::normalize(posAdelanteWagon1 - posWagon1);

            glm::mat4 wagonModel = glm::mat4(1.0f);
            wagonModel = glm::translate(wagonModel, posWagon1);

            float yawWagon1 = atan2(direccionWagon1.x, direccionWagon1.z);
            wagonModel = glm::rotate(wagonModel, yawWagon1, glm::vec3(0.0f, 1.0f, 0.0f));
            wagonModel = glm::rotate(wagonModel, -asin(direccionWagon1.y), glm::vec3(1.0f, 0.0f, 0.0f));

            posFutura = InterpolacionCatmullRom(keyframesCamino, t_wagon1 + 0.005f);
            dirFutura = glm::normalize(posFutura - posWagon1);
            deltaYaw = atan2(dirFutura.x, dirFutura.z) - yawWagon1;
            if (deltaYaw > glm::pi<float>()) deltaYaw -= glm::two_pi<float>();
            if (deltaYaw < -glm::pi<float>()) deltaYaw += glm::two_pi<float>();
            wagonModel = glm::rotate(wagonModel, deltaYaw * 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

            wagonModel = glm::scale(wagonModel, glm::vec3(2.0f, 2.0f, 2.0f));
            glm::mat4 wagonModelAux = wagonModel;

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagonModel));
            VagonExpresoHowarts.RenderModel();

            // Ejes V1
            wagonModel = wagonModelAux; wagonModel = glm::translate(wagonModel, glm::vec3(0.0f, -2.6f, 6.8f)); wagonModel = glm::rotate(wagonModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagonModel)); EjeVagonExpresoHowarts.RenderModel();
            wagonModel = wagonModelAux; wagonModel = glm::translate(wagonModel, glm::vec3(0.0f, -2.6f, 1.8f)); wagonModel = glm::rotate(wagonModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagonModel)); EjeVagonExpresoHowarts.RenderModel();
            wagonModel = wagonModelAux; wagonModel = glm::translate(wagonModel, glm::vec3(0.0f, -2.6f, -3.0f)); wagonModel = glm::rotate(wagonModel, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagonModel)); EjeVagonExpresoHowarts.RenderModel();
        }

        // --- VAGÓN 2 ---
        float t_wagon2 = t_wagon1 - espacioEntreVagones;
        if (t_wagon2 < 0.0f) t_wagon2 += 1.0f;
        {
            glm::vec3 posWagon2 = InterpolacionCatmullRom(keyframesCamino, t_wagon2);
            glm::vec3 posAdelanteWagon2 = InterpolacionCatmullRom(keyframesCamino, t_wagon2 + 0.001f);
            glm::vec3 direccionWagon2 = glm::normalize(posAdelanteWagon2 - posWagon2);

            glm::mat4 wagon2Model = glm::mat4(1.0f);
            wagon2Model = glm::translate(wagon2Model, posWagon2);

            float yawWagon2 = atan2(direccionWagon2.x, direccionWagon2.z);
            wagon2Model = glm::rotate(wagon2Model, yawWagon2, glm::vec3(0.0f, 1.0f, 0.0f));
            wagon2Model = glm::rotate(wagon2Model, -asin(direccionWagon2.y), glm::vec3(1.0f, 0.0f, 0.0f));

            posFutura = InterpolacionCatmullRom(keyframesCamino, t_wagon2 + 0.005f);
            dirFutura = glm::normalize(posFutura - posWagon2);
            deltaYaw = atan2(dirFutura.x, dirFutura.z) - yawWagon2;
            if (deltaYaw > glm::pi<float>()) deltaYaw -= glm::two_pi<float>();
            if (deltaYaw < -glm::pi<float>()) deltaYaw += glm::two_pi<float>();
            wagon2Model = glm::rotate(wagon2Model, deltaYaw * 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

            wagon2Model = glm::scale(wagon2Model, glm::vec3(2.0f, 2.0f, 2.0f));
            glm::mat4 wagon2ModelAux = wagon2Model;

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon2Model));
            VagonExpresoHowarts.RenderModel();

            // Ejes V2
            wagon2Model = wagon2ModelAux; wagon2Model = glm::translate(wagon2Model, glm::vec3(0.0f, -2.6f, 6.8f)); wagon2Model = glm::rotate(wagon2Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon2Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon2Model = wagon2ModelAux; wagon2Model = glm::translate(wagon2Model, glm::vec3(0.0f, -2.6f, 1.8f)); wagon2Model = glm::rotate(wagon2Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon2Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon2Model = wagon2ModelAux; wagon2Model = glm::translate(wagon2Model, glm::vec3(0.0f, -2.6f, -3.0f)); wagon2Model = glm::rotate(wagon2Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon2Model)); EjeVagonExpresoHowarts.RenderModel();
        }

        // --- VAGÓN 3 ---
        float t_wagon3 = t_wagon2 - espacioEntreVagones;
        if (t_wagon3 < 0.0f) t_wagon3 += 1.0f;
        {
            glm::vec3 posWagon3 = InterpolacionCatmullRom(keyframesCamino, t_wagon3);
            glm::vec3 posAdelanteWagon3 = InterpolacionCatmullRom(keyframesCamino, t_wagon3 + 0.001f);
            glm::vec3 direccionWagon3 = glm::normalize(posAdelanteWagon3 - posWagon3);

            glm::mat4 wagon3Model = glm::mat4(1.0f);
            wagon3Model = glm::translate(wagon3Model, posWagon3);

            float yawWagon3 = atan2(direccionWagon3.x, direccionWagon3.z);
            wagon3Model = glm::rotate(wagon3Model, yawWagon3, glm::vec3(0.0f, 1.0f, 0.0f));
            wagon3Model = glm::rotate(wagon3Model, -asin(direccionWagon3.y), glm::vec3(1.0f, 0.0f, 0.0f));

            posFutura = InterpolacionCatmullRom(keyframesCamino, t_wagon3 + 0.005f);
            dirFutura = glm::normalize(posFutura - posWagon3);
            deltaYaw = atan2(dirFutura.x, dirFutura.z) - yawWagon3;
            if (deltaYaw > glm::pi<float>()) deltaYaw -= glm::two_pi<float>();
            if (deltaYaw < -glm::pi<float>()) deltaYaw += glm::two_pi<float>();
            wagon3Model = glm::rotate(wagon3Model, deltaYaw * 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

            wagon3Model = glm::scale(wagon3Model, glm::vec3(2.0f, 2.0f, 2.0f));
            glm::mat4 wagon3ModelAux = wagon3Model;

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon3Model));
            VagonExpresoHowarts.RenderModel();

            // Ejes V3
            wagon3Model = wagon3ModelAux; wagon3Model = glm::translate(wagon3Model, glm::vec3(0.0f, -2.6f, 6.8f)); wagon3Model = glm::rotate(wagon3Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon3Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon3Model = wagon3ModelAux; wagon3Model = glm::translate(wagon3Model, glm::vec3(0.0f, -2.6f, 1.8f)); wagon3Model = glm::rotate(wagon3Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon3Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon3Model = wagon3ModelAux; wagon3Model = glm::translate(wagon3Model, glm::vec3(0.0f, -2.6f, -3.0f)); wagon3Model = glm::rotate(wagon3Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon3Model)); EjeVagonExpresoHowarts.RenderModel();
        }

        // --- VAGÓN 4 ---
        float t_wagon4 = t_wagon3 - espacioEntreVagones;
        if (t_wagon4 < 0.0f) t_wagon4 += 1.0f;
        {
            glm::vec3 posWagon4 = InterpolacionCatmullRom(keyframesCamino, t_wagon4);
            glm::vec3 posAdelanteWagon4 = InterpolacionCatmullRom(keyframesCamino, t_wagon4 + 0.001f);
            glm::vec3 direccionWagon4 = glm::normalize(posAdelanteWagon4 - posWagon4);

            glm::mat4 wagon4Model = glm::mat4(1.0f);
            wagon4Model = glm::translate(wagon4Model, posWagon4);

            float yawWagon4 = atan2(direccionWagon4.x, direccionWagon4.z);
            wagon4Model = glm::rotate(wagon4Model, yawWagon4, glm::vec3(0.0f, 1.0f, 0.0f));
            wagon4Model = glm::rotate(wagon4Model, -asin(direccionWagon4.y), glm::vec3(1.0f, 0.0f, 0.0f));

            posFutura = InterpolacionCatmullRom(keyframesCamino, t_wagon4 + 0.005f);
            dirFutura = glm::normalize(posFutura - posWagon4);
            deltaYaw = atan2(dirFutura.x, dirFutura.z) - yawWagon4;
            if (deltaYaw > glm::pi<float>()) deltaYaw -= glm::two_pi<float>();
            if (deltaYaw < -glm::pi<float>()) deltaYaw += glm::two_pi<float>();
            wagon4Model = glm::rotate(wagon4Model, deltaYaw * 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

            wagon4Model = glm::scale(wagon4Model, glm::vec3(2.0f, 2.0f, 2.0f));
            glm::mat4 wagon4ModelAux = wagon4Model;

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon4Model));
            VagonExpresoHowarts.RenderModel();

            // Ejes V4
            wagon4Model = wagon4ModelAux; wagon4Model = glm::translate(wagon4Model, glm::vec3(0.0f, -2.6f, 6.8f)); wagon4Model = glm::rotate(wagon4Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon4Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon4Model = wagon4ModelAux; wagon4Model = glm::translate(wagon4Model, glm::vec3(0.0f, -2.6f, 1.8f)); wagon4Model = glm::rotate(wagon4Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon4Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon4Model = wagon4ModelAux; wagon4Model = glm::translate(wagon4Model, glm::vec3(0.0f, -2.6f, -3.0f)); wagon4Model = glm::rotate(wagon4Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon4Model)); EjeVagonExpresoHowarts.RenderModel();
        }

        // --- VAGÓN 5 ---
        float t_wagon5 = t_wagon4 - espacioEntreVagones;
        if (t_wagon5 < 0.0f) t_wagon5 += 1.0f;
        {
            glm::vec3 posWagon5 = InterpolacionCatmullRom(keyframesCamino, t_wagon5);
            glm::vec3 posAdelanteWagon5 = InterpolacionCatmullRom(keyframesCamino, t_wagon5 + 0.001f);
            glm::vec3 direccionWagon5 = glm::normalize(posAdelanteWagon5 - posWagon5);

            glm::mat4 wagon5Model = glm::mat4(1.0f);
            wagon5Model = glm::translate(wagon5Model, posWagon5);

            float yawWagon5 = atan2(direccionWagon5.x, direccionWagon5.z);
            wagon5Model = glm::rotate(wagon5Model, yawWagon5, glm::vec3(0.0f, 1.0f, 0.0f));
            wagon5Model = glm::rotate(wagon5Model, -asin(direccionWagon5.y), glm::vec3(1.0f, 0.0f, 0.0f));

            posFutura = InterpolacionCatmullRom(keyframesCamino, t_wagon5 + 0.005f);
            dirFutura = glm::normalize(posFutura - posWagon5);
            deltaYaw = atan2(dirFutura.x, dirFutura.z) - yawWagon5;
            if (deltaYaw > glm::pi<float>()) deltaYaw -= glm::two_pi<float>();
            if (deltaYaw < -glm::pi<float>()) deltaYaw += glm::two_pi<float>();
            wagon5Model = glm::rotate(wagon5Model, deltaYaw * 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));

            wagon5Model = glm::scale(wagon5Model, glm::vec3(2.0f, 2.0f, 2.0f));
            glm::mat4 wagon5ModelAux = wagon5Model;

            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon5Model));
            VagonExpresoHowarts.RenderModel();

            // Ejes V5
            wagon5Model = wagon5ModelAux; wagon5Model = glm::translate(wagon5Model, glm::vec3(0.0f, -2.6f, 6.8f)); wagon5Model = glm::rotate(wagon5Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon5Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon5Model = wagon5ModelAux; wagon5Model = glm::translate(wagon5Model, glm::vec3(0.0f, -2.6f, 1.8f)); wagon5Model = glm::rotate(wagon5Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon5Model)); EjeVagonExpresoHowarts.RenderModel();
            wagon5Model = wagon5ModelAux; wagon5Model = glm::translate(wagon5Model, glm::vec3(0.0f, -2.6f, -3.0f)); wagon5Model = glm::rotate(wagon5Model, anguloFinal, glm::vec3(1.0f, 0.0f, 0.0f)); glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wagon5Model)); EjeVagonExpresoHowarts.RenderModel();
        }

        // Deshabilitar mezcla al terminar el tren
        glDisable(GL_BLEND);
        //FIN TREN HOWARTS Y ANIMACIONES DEL MISMO
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        //--------------------------------------------------------------------------------------------------------------------
        //FAROLAS PUBLICAS QUE ILUMINAN LA ESCENA DE NOCHE 
        //farola 1
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-50.0f, -3.5f, -110.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();

        //farola 2
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(50.0f, -3.5f, -110.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();

        //farola 3
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(50.0f, -3.5f, 110.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();

        //farola 4
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-50.0f, -3.5f, 110.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();

        //farola 5
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(105.0f, -3.5f, 50.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();

        //farola 6
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(105.0f, -3.5f, -50.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();

        //farola 7
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-105.0f, -3.5f, 50.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();

        //farola 8
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-105.0f, -3.5f, -50.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        Farola_M.RenderModel();





        glUseProgram(0);
        mainWindow.swapBuffers();
    }

    LimpiarAudio();
    return 0;
}
