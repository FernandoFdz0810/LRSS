import sys
import socket

port = int(sys.argv[1]) #Almacena en la variable port el parametro que se le pasa por teclado
#ip_address = "192.168.0.30"
#ip= socket.inet_aton(ip_address) #Me convierte la IP en una IP que puede entender el SO
server_address = ('192.168.0.32', port)

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=2):    
    print('El numero de parametros que me has pasado no ha sido el correcto')
    sys.exit() #Se sale del programa

#Comprobacion del puerto adecuado
#else:
    #if(puerto < 1023 || puerto > 5000):
       # print('El número de puerto del servidor escogido deber ser 1024-5000\n")
        #sys.exit()

#Creacion del socket
s=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print('Creacion del socket')

#Asociacion del socket
s.bind(server_address)
print('\nAsociacion del socket')

#i=0
print('\nEsperando escuchas\n')

while True:

    data, address = s.recvfrom(1024)
    print("Se ha conectado al servidor el ", address)
    print("\nEl cliente ha enviado",data.decode())

    mensaje= "Te has conectado en el servidor, mensaje para que aprezca en el terminal cliente"
    s.sendto(mensaje.encode(),address)
    print("\nMensaje enviado")
    
    #i=i+1

s.close()