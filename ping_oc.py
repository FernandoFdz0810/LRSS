import sys
import socket
import time
import numpy as np

ip_serv= sys.argv[1] #Almacenamos en ip_serv el segundo parametro que pasamos al ejecutar el programa, que será la dir IP. (El primero es el nombre del programa)
port_serv= int(sys.argv[2]) #Almacenamos en port_serv el tercer parametro que pasamos al ejecutar el programa, que será el puerto.
address_serv=(ip_serv,port_serv) #Metemos en address_serv Dir IP y puerto en una sola cadena que usaremos para la funcion connect 
tiempo_total=0
paquetes_enviados=0
paquetes_recibidos=0
paquetes_perdidos=0
tiempos = []
t_max = 0
t_min = 0
i = 0

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=3):    
    print('El numero de parametros que me has pasado no ha sido el correcto')
    sys.exit()

#Comprobacion del puerto adecuado
if(port_serv < 1024 or port_serv > 5000):
    print("El puerto que se debe elegir debe estar comprendido en el rango 1024-5000")
    sys.exit()

#Creacion del socket. AF_INET indica uso de direcciones IPv4. SOCK_STREAM para que sea orientado a conexion
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('\nCreacion del socket')

s.connect(address_serv) #A traves de address_serv indicamos a que dir IP y puerto queremos que se conecte
print("Haciendo ping con", ip_serv, "con 32 bytes de datos:\n")

while i<4:

    # Enviar datos al servidor
    mensaje = "32"
    try:
        s.send(mensaje.encode()) #Codificamos y enviamos el mensaje
        inicio=time.time()
        paquetes_enviados += 1
    except:
        paquetes_perdidos += 1

    try:
        # Recibir datos del servidor
        data = s.recv(32).decode() #Utilizamos recv para recibir mensajes del servidor. Permitimos recibir hasta 1024 Bytes de datos de una sola vez.
        fin=time.time()
        paquetes_recibidos += 1
    except:
        paquetes_recibidos += 1

    diferencia = fin - inicio
    milisegundos = pow(10, 3)
    diferencia = diferencia * milisegundos 

    
    tiempo_total += diferencia
    tiempos.append(diferencia)

    t_min = tiempos[0]

    if (tiempos[i] > t_max):
        t_max = tiempos[i]
    if (tiempos[i] < t_min):
        t_min = tiempos[i]


    print("Respuesta por parte de", ip_serv,": bytes=32 tiempo={:.3f}ms".format(diferencia), "TTL= 115")
    time.sleep(1.25)#Añade un retraso de 2s

    i = i + 1

tiempo_medio = tiempo_total / 4
Prc_pp = (paquetes_perdidos * 100)/paquetes_recibidos
print("\nEstadísticas de ping para",ip_serv)
print("\tPaquetes: Enviados =", paquetes_enviados, "Recibidos:", paquetes_recibidos, "Perdidos:", paquetes_perdidos, "(", Prc_pp,"% perdidos),\n")
print("Tiempo aproximado de vuelta en mili-segundos")
print("\tMinimo={:.3f}ms".format(t_min), "Máximo={:.3f}ms".format(t_max), "Media:{:.3f}ms".format(tiempo_medio))

# Cerrar la conexión
s.close()
print("\nConexión cerrada.")