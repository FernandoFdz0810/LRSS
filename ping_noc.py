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
s=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print('\nCreacion del socket')

i=0
while i<4:  #Ejecuta veces el bucle
    mensaje= "Haciendo ping"
    s.sendto(mensaje.encode(),address_serv)
    inicio=time.time()
    print("\nMensaje enviado")
    i= i+1

    data, address = s.recvfrom(1024)
    fin=time.time()
    diferencia = fin - inicio
    diferencia = diferencia / 0.001
    
    np.array.append(diferencia)
    tiempo_total += np.array([i])
    print("\nSe ha recibido mensaje del servidor ", address)
    print('\nTiempo transcurrido total', diferencia)
    print("\nEl servidor ha enviado",data.decode())
    time.sleep(2)#Añade un retraso de 2s

tiempo_medio = tiempo_total / 4
print('\nTiempo medio ', tiempo_medio)
s.close()