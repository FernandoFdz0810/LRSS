import sys
import socket

port = int(sys.argv[1]) #Almacena en la variable port el parametro que se le pasa por teclado
server_address = ('192.168.2.3', port)

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=2):    
    print('El numero de parametros que me has pasado no ha sido el correcto')
    sys.exit()

#Comprobacion del puerto adecuado
if(port < 1024 or port > 5000):
    print("El puerto que se debe elegir debe estar comprendido en el rango 1024-5000")
    sys.exit()

#Creacion del socket. AF_INET indica uso de direcciones IPv4. SOCK_STREAM para que sea orientado a conexion
s=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print('Creacion del socket')

#Asociacion del socket a la dirección del servidor.
s.bind(server_address)
print('\nAsociacion del socket')

print('\nEsperando escuchas...\n')

while True:
    """
        Con recvfrom recibimos datos del servidor. Permitimos recibir hasta
        32 bytes de datos de una sola vez. 
    """
    data, address = s.recvfrom(32)
    print("Se ha conectado al servidor el ", address)
    print("\nEl cliente ha enviado",data.decode())

    #Con Sendto codificamos y enviamos el mensaje. Los parámetros que se le pasan
    #serán el mensaje que se va a enviar. Y la dirección del cliente que se ha conectado
    #que será la dirección IP donde se van a enviar los datos.
    mensaje= data.decode()
    s.sendto(mensaje.encode(),address)
    print("\nMensaje enviado")
    

s.close()