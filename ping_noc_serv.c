#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define LONG_BUFFER 32
#define TTL 115


//Creación del socket


int main(int argc, char *argv[])
{
    struct sockaddr_in datos_servidor, datos_cliente;

    /*
    Se calculan el tamaño de los datos del cliente y del servidor que van a ser utilizados a posteriori
    bind() y sendto()
    */

    int long_serv = sizeof(datos_servidor);
    int long_cliente = sizeof(datos_cliente);

    /*
    Se crean variables de tipo puntero a char que van a ser del tamaño 32 bytes, es decir, del tamaño del mensaje 
    que se va a enviar mediante el socket
    */

    char msg_recv[LONG_BUFFER];
    char msg_enviado[LONG_BUFFER];
    int recv = 0;

    // Se rellena la estructura de los datos del servidor con 0's.

    bzero((char*) &datos_servidor, sizeof(datos_servidor));

    /*
    Se guarda el puerto introducido por la linea de comandos y se convierte a entero
    para agregarlo posteriormente a la estructura sockaddr_in.
    */ 

    char *puerto_char = argv[1];
    int puerto = atoi(puerto_char);

    /*
    En la siguientes lineas, la estructura sockaddr_in que hemos creado anteriormente y que hemos nombrado datos_servidor
    va a almacenar los datos del servidor con el que nos queremos comunicar y cuyos datos hemos pasado como parámetros al 
    iniciar el programa. Utilizamos la función htons() para traducir el orden de los bytes del host al orden de los bytes de la red.
    También, la función inet_addr se utiliza para traducir una dirección IP en formato de cadena de caracteres en un formato de 32 bits 
    a un formato adecuado para su uso en red. 
    */

    datos_servidor.sin_family = AF_INET;
    datos_servidor.sin_port = htons(puerto);
    datos_servidor.sin_addr.s_addr = inet_addr("172.22.58.104"); 


    if(argc > 2)
    {
        printf("El número de argumentos pasado debe ser 'nombre del programa' y 'puerto de escucha'");
        return 0;
    }

     /*
    En caso de que se cumpla que haya menos de 2 argumentos. Se verifica que el puerto que se ha introducido
    por la linea de argumentos este comprendido en el rango de 1024-5000, ya que es el rango de puertos 
    que es utilizado para servidores como viene comentado en el libro.
    */

    else
    {
        if(puerto < 1023 || puerto > 5000)
        {
            printf("El número de puerto escogido deber ser 1024-5000\n");
            return -1;
        }
    }
    
    // Creación de un socket AF_INET(DOMINIO INTERNET TCP/IP), en modo no conectivo SOCK_DGRAM (UDP)
    //En caso de fallo devuelve -1, si todo va bien devuelve un valor entero bajo.

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0)
    {
        printf("Error creando el socket\n");
        return -1;
    }

    /*
    Se realiza la asociación una dirección local con el socket que hemos creado anteriormente. 
    El segundo parámetro contiene la dirección a la que se quiere asociar el socket. 
    El tercer parámetro especifica el tamaño de la estructura en bytes (calculado anteriormente).
    Provoca un error si el valor devuelto en 'asociación' es '-1'. 
    */

    int asociacion = bind(sockfd,(struct sockaddr *) &datos_servidor, long_serv);

    if(asociacion < 0)
    {
        printf("Error en asociación 'bind'. Cerrando conexión...\n");
        // Se cierra la conexión
        close(sockfd);
        return -1;
    }

    else
    {
        /*
        Se muestran datos por pantalla especificando la dirección IP y puerto mediante la cual el servidor va
        a escuhar las peticiones. 
        */

        printf("Asociación del socket creada.\n\tEscuchando por el puerto: %d\n", ntohs(datos_servidor.sin_port));
        printf("\tDirección IP: %s\n", inet_ntoa(datos_servidor.sin_addr));
    }



    do
    {
        /*
        Se hace uso del bucle do... while para que el servidor se mantenga escuchando las peticiones
        excepto cuando haya algun error a la hora de recibir los datos. 
        */

        printf("\n\tEsperando datos a recibir...\n");

        /*
        A continuación, el servidor espera a recibir el envio por parte del cliente, a través del decriptor del socket. 
        Esta respuesta va a ser almacenada en la variable 'msg_recv' y de la LONG_PAQUETE especificada.
        En caso de error, recvfrom devuelve '-1'
        */


        int recv = recvfrom(sockfd, msg_recv, LONG_BUFFER, 0, (struct sockaddr *) &datos_cliente, &long_cliente);

        if(recv < 0)
        {
            printf("\tError recibiendo datos\n");
        }

        else
        {
            printf("\tRecibiendo datos desde: %s\n", inet_ntoa(datos_cliente.sin_addr));
        }
        
        /*
        Mediante la función send() se envian datos mediante el socket creado. 
        Se toman como argumentos el descriptor del socket creado, un puntero al buffer que contiene
        los datos que se van a enviar, además de su tamaño en bytes. 
        Además, también se indican los datos del cliente que se está comunicando con nosotros. 

        En caso de dar error, se devuelve un '-1'.
        */

        if(sendto(sockfd, msg_enviado, recv, 0, (struct sockaddr *) &datos_cliente, long_cliente) != recv)
        {
            printf("Error\n");
        }

        else
        {
            printf("\tEnviando datos...\n");
        }
        
    } while(recv != -1);
    
}






