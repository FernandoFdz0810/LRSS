import sys
import socket
import time
import numpy as np

ip_serv= sys.argv[1] #Almacenamos en ip_serv el segundo parametro que pasamos al ejecutar el programa, que será la dir IP. (El primero es el nombre del programa)
port_serv= int(sys.argv[2]) #Almacenamos en port_serv el tercer parametro que pasamos al ejecutar el programa, que será el puerto.
address_serv=(ip_serv,port_serv) #Metemos en address_serv Dir IP y puerto en una sola cadena que usaremos para la funcion connect 
tiempo_total=0

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=3):    
    print('El numero de parametros que me has pasado no ha sido el correcto')
    sys.exit()

#Comprobacion del puerto adecuado


#Creacion del socket. AF_INET indica uso de direcciones IPv4. SOCK_STREAM para que sea orientado a conexion
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('\nCreacion del socket')

s.connect(address_serv) #A traves de address_serv indicamos a que dir IP y puerto queremos que se conecte
print('\nConexion establecida')


# Enviar datos al servidor
message = "Hola, servidor!"
s.send(message.encode()) #Codificamos y enviamos el mensaje

# Recibir datos del servidor
data = s.recv(1024).decode() #Utilizamos recv para recibir mensajes del servidor. Permitimos recibir hasta 1024 Bytes de datos de una sola vez.
print("\nRespuesta del servidor:", data)

# Cerrar la conexión
s.close()
print("\nConexión cerrada.")