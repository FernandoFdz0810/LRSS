#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define LONG_BUFFER 8
#define N_PETICIONES 5


//Creación del socket


int main(int argc, char *argv[])
{
    //Creacion de la estructura sockaddr_in inicializada a 0.

    struct sockaddr_in datos_servidor, datos_cliente;
    int long_serv = sizeof(datos_servidor);
    int long_cliente = sizeof(datos_cliente);
    char msg_recv[LONG_BUFFER];
    char msg_enviado[LONG_BUFFER];
    int recv = 0;

    // Se rellena la estructura de los datos del servidor con 0's.

    bzero((char*) &datos_servidor, sizeof(datos_servidor));
    bzero((char*) &datos_cliente, sizeof(datos_cliente));

    //Se guarda el puerto introducido por la linea de comandos y se convierte a entero para agregarlo posteriormente a la estructura sockaddr_in.

    char *puerto_char = argv[1];
    int puerto = atoi(puerto_char);

    if(argc > 2)
    {
        printf("El número de argumentos pasado debe ser 'nombre del programa' y 'puerto de escucha'");
        return 0;
    }

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

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        printf("Error creando el socket\n");
        return -1;
    }

    datos_servidor.sin_family = AF_INET;
    datos_servidor.sin_port = htons(puerto);
    datos_servidor.sin_addr.s_addr = inet_addr("172.22.58.104"); 

    // Se realiza la asociación de los datos con el socket. 

    int asociacion = bind(sockfd,(struct sockaddr *) &datos_servidor, long_serv);
    //getsockname(sockfd,(struct sockaddr *) &datos_servidor, &long_serv);

    if(asociacion < 0)
    {
        printf("Error en asociación 'bind'. Cerrando conexión...\n");
        // Se cierra la conexión
        close(sockfd);
        return -1;
    }

    else
    {
        printf("Asociación del socket creada.\n");
    }

    int list = listen(sockfd, N_PETICIONES);

    if(list < 0)
    {
        printf("Error en la espera de peticiones\n");
    }

    else
    {
        printf("Esperando peticiones\n");
    }

    int n_sockfd = accept(sockfd, (struct sockaddr *) &datos_cliente, &long_cliente);

    if (n_sockfd < 0)
    {
        printf("Error en la petición procedente de %s\n", inet_ntoa(datos_cliente.sin_addr));
    }

    else
    {
        printf("Aceptando conexión procedente de %s\n", inet_ntoa(datos_cliente.sin_addr));
    }

    close(sockfd);











    /*
    do
    {
        printf("\n\tEsperando datos a recibir...\n");

        int recv = recvfrom(sockfd, msg_recv, LONG_BUFFER, 0, (struct sockaddr *) &datos_cliente, &long_cliente);

        if(recv < 0)
        {
            printf("\tError recibiendo datos\n");
        }

        else
        {
            printf("\tRecibiendo datos desde: %s\n", inet_ntoa(datos_cliente.sin_addr));
        }

        if(sendto(sockfd, msg_enviado, recv, 0, (struct sockaddr *) &datos_cliente, long_cliente) != recv)
        {
            printf("Error\n");
        }

        else
        {
            printf("\tEnviando datos...\n");
        }
    } while(recv != -1);
    */
    
}