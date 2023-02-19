import sys
import socket

port = int(sys.argv[1]) #Almacena en la variable port el parametro que se le pasa por teclado
#ip_address = "192.168.43.187"
#ip= socket.inet_aton(ip_address) #Me convierte la IP en una IP que puede entender el SO
server_address = ('192.168.0.34', port)

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=2):    
    print('\nEl numero de parametros que me has pasado no ha sido el correcto')
    sys.exit() #Se sale del programa

#Comprobacion del puerto adecuado
#else:
    #if(puerto < 1023 || puerto > 5000):
       # print('El número de puerto del servidor escogido deber ser 1024-5000\n")
        #sys.exit()

#Creacion del socket
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print('\nCreacion del socket')

#Asociacion del socket
s.bind(server_address)
print('\nAsociacion del socket')

#i=0
print('\nEsperando escuchas\n')

s.listen(5) #Permite hasta 5 conexiones en espera

while True:
    client_socket, client_address = s.accept() #Genera socket nuevo client_socket. client_address almacena la dir IP y puerto del cliente
    print('\nSe ha conectado el cliente ',client_address)

    data = client_socket.recv(1024) #Recibe hasta 1024bytes de datos y los almacena en la variable data
    print('\n',data.decode())

    mensaje = "Hola cliente, te hablo, soy el servidor. Encantado"
    client_socket.send(mensaje.encode())
    print('\nMensaje enviado')