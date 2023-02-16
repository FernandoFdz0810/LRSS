import sys
import socket
import time

ip_serv= sys.argv[1]
port_serv= int(sys.argv[2])
address_serv=(ip_serv,port_serv)

#Comprobamos que se le han pasado el numero adecuado de argumentos
if(len(sys.argv)!=3):    
    print('El numero de parametros que me has pasado no ha sido el correcto')
    sys.exit()

#Comprobacion del puerto adecuado


#Creacion del socket
s=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print('Creacion del socket')

i=0
while i<4:  #Ejecuta veces el bucle
    mensaje= "Haciendo ping"
    s.sendto(mensaje.encode(),address_serv)
    print("Mensaje enviado")
    i= i+1

    data, address = s.recvfrom(1024)
    print("Se ha recibido mensaje del servidor ", address)
    print("El servidor ha enviado",data.decode())
    time.sleep(2)#Añade un retraso de 2s

s.close()