#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#define LONG_BUFFER 4
#define PUERTO 2000


//Creación del socket


int main(int argc, char *argv[])
{
    //Creacion de la estructura sockaddr_in inicializada a 0.

    struct sockaddr_in datos_servidor, datos_cliente;
    int long_serv = sizeof(datos_servidor);
    int long_cliente = sizeof(datos_cliente);
    char* msg_enviado[LONG_BUFFER];
    int lg_msg_enviado = sizeof(msg_enviado);
    char* msg_recibido[LONG_BUFFER];
    char* solicitud = argv[1];
    double t_ns[3];
    double t_min = 0.0, t_max = 0.0, t_medio[3];
    int paquetes_enviados = 0, paquetes_recibidos = 0, paquetes_perdidos = 0; //Número de mensajes enviados

    // Se rellena la estructura de los datos del servidor  y del cliente con 0's.

    bzero((char*) &datos_servidor, sizeof(datos_servidor));
    bzero((char*) &datos_cliente, sizeof(datos_cliente));
    
    if(strcmp(argv[1], "ping") != 0 && strcmp(argv[1], "PING") != 0)
    {
        printf("El primer argumento que se debe de pasar debe de ser 'ping' (MAYUS o MINUS)\n");
        return -1;
    }
    
    char *nombre_direccion = argv[2];
    char *puerto_char = argv[3];
    int puerto = atoi(puerto_char);

    datos_servidor.sin_family = AF_INET;
    datos_servidor.sin_port = htons(puerto);
    datos_servidor.sin_addr.s_addr = inet_addr(nombre_direccion); 

    if(argc > 4)
    {
        printf("El número de argumentos pasado debe ser 'nombre del programa', 'direccion IP o nombre de host' y 'puerto de escucha'");
        return 0;
    }

    else
    {
        if(puerto < 1023 || puerto > 5000)
        {
            printf("El número de puerto del servidor escogido deber ser 1024-5000\n");
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

    datos_cliente.sin_family = AF_INET;
    datos_cliente.sin_port = htons(PUERTO);

    // Se realiza la asociación de los datos con el socket. 

    int conexion = connect(sockfd,(struct sockaddr *) &datos_servidor, long_serv);

    if (conexion < 0)
    {
        printf("Cliente: no se puede conectar\n");
    }

    else
    {
        printf("\nConexión creada con el servidor\n\n");
    }

    for(int i = 0; i < 4; i++)
    {
        int wr = write(sockfd, msg_enviado, lg_msg_enviado);

        if(wr != lg_msg_enviado)
        {
            printf("Error al enviar mensaje con write\n");
            close(sockfd);
        }

        else
        {
            printf("Enviando datos...\n\n");
        }

        int rd = read(sockfd, msg_recibido, sizeof(msg_recibido));

        if (rd == -1)
        {
            printf("Error recibiendo datos\n");
        }

        if(rd == 0)
        {
            printf("El servidor ha cerrado la conexión\n");
            close(sockfd);
        }

        else
        {
            printf("Recibiendo datos...\n\n");
        }

    }

    
    close(sockfd);




}