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

#define LONG_BUFFER 8
#define PUERTO 2000


//Creación del socket


int main(int argc, char *argv[])
{
    //Creacion de la estructura sockaddr_in inicializada a 0.

    struct sockaddr_in datos_servidor, datos_cliente;
    int long_serv = sizeof(datos_servidor);
    int long_cliente = sizeof(datos_cliente);
    char* msg_enviado[LONG_BUFFER];
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

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0)
    {
        printf("Error creando el socket\n");
        return -1;
    }

    datos_cliente.sin_family = AF_INET;
    datos_cliente.sin_port = htons(PUERTO);

    // Se realiza la asociación de los datos con el socket. 

    int asociacion = bind(sockfd,(struct sockaddr *) &datos_cliente, long_cliente);

    if(asociacion < 0)
    {
        printf("Error en asociación 'bind'. Cerrando conexión...\n");
        // Se cierra la conexión
        close(sockfd);
        return -1;
    }

    else
    {
        printf("Asociación del socket creada.\n\tPreparado para enviar por el puerto: %d\n", ntohs(datos_cliente.sin_port));
        printf("\tEnviando a la dirección IP: %s\n", inet_ntoa(datos_servidor.sin_addr));
    }

    //printf("Escriba el mensaje que desea escribir: \n");
    *msg_enviado = "ping";
    int log_msg = sizeof(msg_enviado);

    printf("Enviando datos...\n\n");
    printf("\nHaciendo ping con %s:\n\n", nombre_direccion);

    for(int i = 0; i < 4; i++)
    {
        int send = sendto(sockfd, msg_enviado, log_msg, 0, (struct sockaddr *) &datos_servidor, long_serv);
        clock_t comienzo = clock();

        if(send < 0)
        {
            printf("Error enviando datos\n");
            paquetes_perdidos += 1;
        }

        else
        {
            paquetes_enviados += 1;
        }

        int n = recvfrom(sockfd, msg_recibido, LONG_BUFFER, 0, (struct sockaddr *) 0, (int *) 0);
        sleep(1.5);

        if(n < 0)
        {
            printf("Error\n");
            paquetes_perdidos += 1;

        }

        else
        {
            paquetes_recibidos += 1;
            clock_t final = clock();
            double tiempo_transcurrido = 0.0;
            tiempo_transcurrido += (double)(final - comienzo) / CLOCKS_PER_SEC;
            t_ns[i] = tiempo_transcurrido / pow(10,-3);
            t_min = t_ns[0];
            if(t_ns[i] > t_max)
            {
                t_max = t_ns[i];
            }
            if(t_ns[i] < t_min)
            {
                t_min = t_ns[i];
            }
            printf("Respuesta desde: %s bytes: %ld tiempo: %.3fms TTL = 115\n", inet_ntoa(datos_servidor.sin_addr),sizeof(msg_recibido)/8, t_ns[i]);
        }

    }

    t_medio[0] = 0.0;
    for(int i = 0; i < 4; i++)
    {
        t_medio[i] += t_ns[i];
    }
    double perc_perdida = (paquetes_perdidos * 100)/paquetes_recibidos;
    double media = t_medio[3]/4;


    printf("\nEstadísticas de ping para: %s:\n", inet_ntoa(datos_servidor.sin_addr));
    printf("\tPaquetes: Enviados = %d, Recibidos = %d, Perdidos = %d (%.2f%% perdida),\n", paquetes_enviados, paquetes_recibidos, paquetes_perdidos, perc_perdida);
    printf("Tiempo aproximado de vuelta en mili-segundos:\n");
    printf("\tMinimo = %.3fms, Máximo = %.3fms, Media = %.3fms\n\n", t_min, t_max, media);
    
    //REVISAR TIEMPO MEDIO 
    //REVISAR TTL
}


