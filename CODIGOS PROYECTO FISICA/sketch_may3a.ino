#include <Servo.h> // Incluimos la librería Servo para controlar el servomotor

int buttonPin = 12; // Pin conectado al pulsador
int sensorPin = 9; // Pin conectado a la salida digital del módulo de sensor de sonido KY-038
int ledPin = 13; // Pin conectado al positivo del LED
int servoPin = 8; // Pin de control del servo de rotación continua
int servoPin2 = 10; // Pin de control del servo estático
boolean is_on = false; // Para determinar/seguir si el LED está encendido o apagado
boolean timer_running = false; // Para determinar si el temporizador está en marcha
unsigned long start_time; // Para almacenar el tiempo de inicio
unsigned long elapsed_time; // Para almacenar el tiempo transcurrido
Servo myServo; // Creamos un objeto de tipo Servo para controlar el servomotor de rotación continua
Servo myServo2; // Creamos un objeto de tipo Servo para controlar el servomotor estático

int buttonState = 0; // Variable para almacenar el estado actual del pulsador
int lastButtonState = HIGH; // Variable para almacenar el último estado del pulsador
unsigned long lastDebounceTime = 0; // Última vez que el pin de entrada fue cambiado
unsigned long debounceDelay = 50; // Tiempo de rebote del botón
unsigned long moveDuration = 200; // Duración del movimiento en milisegundos (ajusta según sea necesario)
bool isMoving = false; // Variable para controlar si el servo está en movimiento

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Configura el pin del botón como entrada con resistencia pull-up
  pinMode(sensorPin, INPUT); // Configura el pin del sensor de sonido como entrada
  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  myServo.attach(servoPin); // Asociamos el objeto Servo al pin de control del servomotor de rotación continua
  myServo2.attach(servoPin2); // Asociamos el objeto Servo al pin de control del servomotor estático
  myServo.write(90); // Detenemos el servomotor de rotación continua (posición neutral)
  Serial.begin(9600); // Inicializa el puerto serial a 9600 baudios
}

void loop() {
  int reading = digitalRead(buttonPin); // Lee el estado del pulsador
  int soundData = digitalRead(sensorPin); // Lee datos del sensor de sonido

  // Verifica si la lectura del botón ha cambiado (debido a ruido o una pulsación real)
  if (reading != lastButtonState) {
    // Resetea el temporizador de debounce
    lastDebounceTime = millis();
  }

  // Verifica si el tiempo desde el último cambio es mayor que el tiempo de debounce
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Si la lectura ha cambiado después del tiempo de debounce, considera el cambio como válido
    if (reading != buttonState) {
      buttonState = reading;

      // Solo realiza la acción si el nuevo estado del botón es presionado
      if (buttonState == LOW && !timer_running) {
        start_time = millis(); // Almacena el tiempo actual como tiempo de inicio
        timer_running = true; // Marca que el temporizador está en marcha

        // Movimiento del servomotor estático
        if (!is_on) { // Si el LED está apagado, enciéndelo y mueve el servo
          digitalWrite(ledPin, HIGH);
          myServo2.write(0); // Mueve el servomotor estático a la posición 0 grados
          is_on = true;
          Serial.println("LED Encendido"); // Envía mensaje de LED encendido por puerto serial
        }
      }

      // Movimiento del servomotor de rotación continua
      if (buttonState == LOW && !isMoving) {
        myServo.write(135); // Gira el servomotor en una dirección (valor ajustado para velocidad deseada)
        delay(moveDuration); // Espera durante el tiempo especificado para simular el movimiento
        myServo.write(90); // Detiene el servomotor
        isMoving = true; // Indica que el servo ha realizado el primer movimiento
      }
    }
  }

  if (timer_running) {
    if (!is_on) { // Si el LED está apagado, enciéndelo y mueve el servo estático
      digitalWrite(ledPin, HIGH);
      myServo2.write(0); // Mueve el servomotor estático a la posición 0 grados
      is_on = true;
      Serial.println("LED Encendido"); // Envía mensaje de LED encendido por puerto serial
    }
  } else { // Si el temporizador no está en marcha, apaga el LED y mueve el servo estático a 90 grados
    digitalWrite(ledPin, LOW);
    myServo2.write(25); // Mueve el servomotor estático a la posición 45 grados
    is_on = false;
  }

  // Verifica si el sensor de sonido detecta un sonido y el temporizador está en marcha
  if (soundData == HIGH && timer_running) {
    elapsed_time = millis() - start_time; // Calcula el tiempo transcurrido
    Serial.print("Tiempo transcurrido con delay: ");
    Serial.println((float)elapsed_time/1000,3);
    timer_running = false; // Marca que el temporizador ya no está en marcha
    

    // Movimiento del servomotor de rotación continua
    if (isMoving) {
      myServo.write(25); // Gira el servomotor en la dirección opuesta (valor ajustado para velocidad deseada)
      delay(moveDuration); // Espera durante el tiempo especificado para simular el movimiento de regreso
      myServo.write(90); // Detiene el servomotor
      isMoving = false; // Resetea la variable de movimiento
    }
  }

  // Guarda el estado de la lectura actual como el último estado
  lastButtonState = reading;
}
