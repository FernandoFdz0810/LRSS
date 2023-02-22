import sys
import socket

port = int(sys.argv[1]) #Almacena en la variable port el parametro que se le pasa por teclado
server_address = ('192.168.0.36', port)

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=2):    
    print('\nEl numero de parametros que me has pasado no ha sido el correcto')
    sys.exit() #Se sale del programa

#Comprobacion del puerto adecuado
#else:
    #if(puerto < 1023 || puerto > 5000):
       # print('El número de puerto del servidor escogido deber ser 1024-5000\n")
        #sys.exit()

#Creacion del socket. AF_INET indica uso de direcciones IPv4. SOCK_STREAM para que sea orientado a conexion
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('\nCreacion del socket')

#Asociamos el socket a una dir IP y puerto especificos. Pasandole la cadena server_address que almacena dir IP y puerto del servidor  
s.bind(server_address)
print('\nAsociacion del socket')

#i=0
print('\nEsperando escuchas\n')

s.listen(5) #Indicamos que permita hasta 5 conexiones en espera/cola

while True:
    #Accept acepta una conexion entrante. Devuelve un socket nuevo. Y la dir IP y puerto del cliente que se ha conectado, que almacenamos en client_address
    client_socket, client_address = s.accept() #Genera socket nuevo client_socket. client_address almacena la dir IP y puerto del cliente
    print('\nSe ha conectado el cliente ',client_address)

    data = client_socket.recv(1024) #Utilizamos recv para recibir mensajes del cliente. Permitimos recibir hasta 1024 Bytes de datos de una sola vez.
    print('\n',data.decode())

    mensaje = "Hola cliente, te hablo, soy el servidor. Encantado"
    client_socket.send(mensaje.encode())    #Codificamos y enviamos el mensaje
    print('\nMensaje enviado')