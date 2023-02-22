import sys
import socket

port = int(sys.argv[1]) #Almacena en la variable port el parametro que se le pasa por teclado
server_address = ('192.168.2.3', port)

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=2):    
    print('\nEl numero de parametros que me has pasado no ha sido el correcto')
    sys.exit() #Se sale del programa

#Comprobacion del puerto adecuado
if(port < 1024 or port > 5000):
    print("El puerto que se debe elegir debe estar comprendido en el rango 1024-5000")
    sys.exit()

#Creacion del socket. AF_INET indica uso de direcciones IPv4. SOCK_STREAM para que sea orientado a conexion
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('Creacion del socket')

#Asociamos el socket a una dir IP y puerto especificos. Pasandole la cadena server_address que almacena dir IP y puerto del servidor  
s.bind(server_address)
print('Asociacion del socket')

print('Esperando peticiones...\n')

s.listen(4) #Indicamos que permita hasta 4 conexiones en espera/cola

while True:
    #Accept acepta una conexion entrante. Devuelve un socket nuevo. Y la dir IP y puerto del cliente que se ha conectado, que almacenamos en client_address
    n_socket, direccion_cliente = s.accept() #Genera socket nuevo client_socket. client_address almacena la dir IP y puerto del cliente
    print('\nSe ha conectado el cliente ', direccion_cliente, '\n')

    while True:
        datos = n_socket.recv(32) #Utilizamos recv para recibir mensajes del cliente. Permitimos recibir hasta 32 Bytes de datos de una sola vez.

        mensaje = "32"
        
        if datos:
            n_socket.send(mensaje.encode())    #Codificamos y enviamos el mensaje
            print('Mensaje enviado\n')
        else:
            print("Ningun dato más recibido por parte del cliente:", direccion_cliente)
            print("Se cierra conexión TCP. A la espera de más peticiones...")

            break

s.close()