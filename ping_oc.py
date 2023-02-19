import sys
import socket
import time
import numpy as np

ip_serv= sys.argv[1]
port_serv= int(sys.argv[2])
address_serv=(ip_serv,port_serv)
tiempo_total=0

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=3):    
    print('El numero de parametros que me has pasado no ha sido el correcto')
    sys.exit()

#Comprobacion del puerto adecuado


#Creacion del socket
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('\nCreacion del socket')

s.connect(address_serv) #Pasamos la direccionIP+Puerto del servidor al que quiere conectarse
print('\nConexion establecida')


# Enviar datos al servidor
message = "Hola, servidor!"
s.send(message.encode())

# Recibir datos del servidor
data = s.recv(1024).decode()
print("\nRespuesta del servidor:", data)

# Cerrar la conexión
s.close()
print("\nConexión cerrada.")