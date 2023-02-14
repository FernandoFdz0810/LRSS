import sys
import socket

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

mensaje= "Haciendo ping"
s.sendto(mensaje.encode(),address_serv)
print("Mensaje enviado")

#Continuara