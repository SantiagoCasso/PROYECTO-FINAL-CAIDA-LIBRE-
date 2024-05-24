import math
import serial
import time

# Definición de la función para calcular el tiempo necesario para una distancia dada
def calcular_tiempo_para_distancia(distancia, gravedad=9.8):
    return math.sqrt((2 * distancia) / gravedad)

# Definición de la función para ejecutar el temporizador
def ejecutar_temporizador():
    # Configura el puerto serial (ajusta 'COM3' al puerto que usa tu Arduino)
    puerto_serial = serial.Serial('COM3', 9600, timeout=1)
    time.sleep(2)  # Espera a que la conexión se establezca

    # Inicialización de variables
    tiempo_promedio=0
    tiempo_transcurrido = 0
    gravedad = 9.8

    try:
        # Bucle principal del temporizador
        while True:
            # Verifica si hay datos disponibles en el puerto serial
            if puerto_serial.in_waiting > 0:
                # Lee los datos del puerto serial y los decodifica
                lectura_serial = puerto_serial.readline().decode().strip()
                print(lectura_serial)

                # Comprueba si se ha presionado el botón para iniciar el temporizador
                if lectura_serial == "LED Encendido":
                    
                    tiempo_inicio = time.time()

                # Comprueba si se ha detectado un sonido para finalizar el temporizador
                elif lectura_serial.startswith("Tiempo transcurrido"):

                    tiempo_transcurrido = (float(lectura_serial.split(":")[1].strip())) # Convierte el tiempo a segundos
                    tiempo_promedio=tiempo_transcurrido-0.039
                    print("tiempo transcurrido ajustado: "+ str(tiempo_promedio))
                      
                    break

    except KeyboardInterrupt:
        # Manejo de interrupción de teclado (Ctrl+C)
        print("Programa terminado")
    finally:
        # Cierra el puerto serial al finalizar el programa
        puerto_serial.close()

    # Cálculo de la distancia y velocidad usando el tiempo medido
    dis_medida = (gravedad * ((tiempo_promedio )** 2)) / 2
    velocidad_medida = math.sqrt(2 * gravedad * dis_medida)
    gravedad_formula = (2 * dis_medida) / ((tiempo_promedio) ** 2)  # Calcular gravedad experimental
    
    print("Datos medidos por el sensor:")
    print("La velocidad final es: {:.3f} m/s".format(velocidad_medida))
    print("La distancia a la que fue soltado el objeto fue: {:.3f} metros".format(dis_medida))
    print("Gravedad experimental: {:.3f} m/s^2".format(gravedad_formula))

    return tiempo_promedio, dis_medida, velocidad_medida

# Función principal
def main():
    while True:
        # Ejecuta el temporizador y obtiene los datos medidos
        tiempo_promedio, dis_medida, velocidad_medida = ejecutar_temporizador()

        # Solicita la distancia objetivo al usuario
        distancia_objetivo = float(input("Ingrese la distancia original en metros: "))

        # Calcula el tiempo correcto para la distancia objetivo
        tiempo_correcto = calcular_tiempo_para_distancia(distancia_objetivo)

        # Cálculo de la distancia y velocidad usando el tiempo ajustado
        dis_correcta = (9.8 * (tiempo_correcto ** 2)) / 2
        velocidad_correcta = math.sqrt(2 * 9.8 * dis_correcta)
        gravedad_formulaC = (2 * dis_correcta) / (tiempo_correcto ** 2)  # Calcular gravedad experimental para la distancia correcta
        
        print("\nDatos reales:")
        print("tiempo real de caida: {:.3f} segundos".format(tiempo_correcto))
        print("La velocidad final es: {:.3f} m/s".format(velocidad_correcta))
        print("La distancia a la que fue soltado el objeto fue: {:.3f} metros".format(dis_correcta))
        print("Gravedad experimental (para la distancia correcta): {:.3f} m/s^2".format(gravedad_formulaC))

        # Pregunta al usuario si desea ejecutar el código nuevamente
        reiniciar = input("\n¿Desea ejecutar el código de nuevo? (S/N): ").strip().lower()
        if reiniciar != 's':
            break

# Punto de entrada al programa
if __name__ == "__main__": 
    main()

