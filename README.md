![ICESI University Logo](https://www.icesi.edu.co/launiversidad/images/La_universidad/logo_icesi.png)

# Proyecto MDS-UPDRS Nivel 3: Prueba de Taconeo y Zapateo

Este repositorio contiene el código desarrollado para la **prueba MDS-UPDRS nivel 3**, específicamente la prueba de **taconeo y zapateo**, utilizando una **ESP32** y un sensor **MPU 6050**. Los datos capturados durante la prueba se envían a un servidor mediante **HTTP**, y la activación de las pruebas se realiza a través de un **broker MQTT** en **HiveMQ**.

---

## Integrantes

- **Dylan Bermúdez**
- **Santiago Escobar**
- **Kevin Nieto**
- **Ricardo Urbina**

### Curso
**Proyecto Integrador I - Ingeniería Telemática**

### Docente
**Domiciano Rincón**

---

## Descripción del Proyecto

La **prueba MDS-UPDRS (Movement Disorder Society - Unified Parkinson's Disease Rating Scale)** nivel 3 evalúa los síntomas motores de pacientes con enfermedad de Parkinson. Este proyecto se enfoca en las pruebas de **taconeo y zapateo**, donde se utilizan dispositivos de hardware para registrar el movimiento y evaluar la capacidad del paciente.

### Componentes del Proyecto

1. **ESP32**: Microcontrolador principal encargado de gestionar la conexión a internet (WiFi), la suscripción al tópico MQTT, la recolección de datos del sensor MPU 6050 y el envío de los datos mediante HTTP.
   
2. **MPU 6050**: Sensor inercial que mide la aceleración y la velocidad angular en los tres ejes (X, Y, Z), usado para registrar los movimientos del pie durante las pruebas de taconeo y zapateo.

3. **Broker MQTT (HiveMQ)**: Sistema de mensajería utilizado para activar el inicio de la prueba desde un dispositivo externo. Se utiliza un cluster gratuito de **HiveMQ Cloud**.

4. **Servidor HTTP**: Los datos recolectados por el sensor MPU 6050 se envían a un servidor para su almacenamiento y posterior análisis.

---

## Funcionalidades

- **Conexión a WiFi**: La ESP32 se conecta a una red WiFi para enviar datos al servidor y suscribirse al broker MQTT.
  
- **Suscripción a MQTT**: La ESP32 se suscribe a un tópico en el broker HiveMQ. Cuando recibe el mensaje de inicio (por ejemplo, `"start"`), comienza la recolección de datos.

- **Recolección de Datos del MPU 6050**: El sensor MPU 6050 mide la aceleración y la velocidad angular en los tres ejes y convierte los datos en formato JSON.

- **Envío de Datos por HTTP**: Una vez que finaliza la prueba, los datos se envían a un servidor HTTP en formato JSON para ser procesados o almacenados.

---

## Instalación y Uso

### Requisitos

- **Hardware**:
  - ESP32
  - Sensor MPU 6050
  - Botones y buzzer para la interfaz física de la prueba
  - Conexión WiFi
  
- **Software**:
  - [Arduino IDE](https://www.arduino.cc/en/software) o [PlatformIO](https://platformio.org/) para la programación de la ESP32
  - Broker MQTT (Cluster gratuito de HiveMQ)
  - Servidor HTTP para recibir los datos (opcionalmente un servidor local o en la nube)

### Dependencias

El proyecto requiere las siguientes bibliotecas instaladas en el entorno de desarrollo de Arduino:

```bash
PubSubClient.h
WiFi.h
HTTPClient.h
Wire.h
I2Cdev.h
MPU6050.h
```

### Configuración del MQTT (HiveMQ)

El proyecto está configurado para utilizar un broker MQTT gratuito de **HiveMQ Cloud**. Para suscribirse a un tópico:

1. Dirígete a [HiveMQ Cloud](https://www.hivemq.com/mqtt-cloud-broker/).
2. Inicia sesión y crea un **cluster** gratuito.
3. Utiliza las credenciales y el URL proporcionado por HiveMQ para configurar el broker en el código del ESP32.

```cpp
const char* broker = "url_broker";
const int port = 8883;
const char* topic = "nombre/prueba";
```

### Configuración del WiFi

Antes de compilar y subir el código a la ESP32, configura tu red WiFi:

```cpp
const char* ssid = "TuSSID";  // Cambia esto por el nombre de tu red WiFi
const char* password = "TuPassword";  // Cambia esto por la contraseña de tu WiFi
```

### Ejecución

1. **Conexión WiFi**: La ESP32 se conectará automáticamente a la red configurada.
2. **Suscripción MQTT**: La ESP32 se suscribe al tópico definido. Cuando recibe un mensaje (por ejemplo, `"start"`), comenzará la captura de datos.
3. **Recolección de Datos**: El sensor MPU 6050 capturará los datos de movimiento (aceleración y giroscopio).
4. **Envío de Datos**: Al finalizar la prueba, los datos se enviarán automáticamente a un servidor mediante una petición HTTP en formato JSON.

---

## Ejemplo de Datos Recibidos

```json
{
  "ax": -324,
  "ay": 110,
  "az": 1024,
  "gx": 512,
  "gy": -256,
  "gz": 0
}
```

---

## Contribuciones

Para contribuir a este proyecto, por favor abre un issue o envía un pull request. Toda ayuda es bienvenida.