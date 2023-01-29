#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>


//Creación del socket


int main(int argc, char *argv[])
{
    //Creacion de la estructura sockaddr_in inicializada a 0.

    struct sockaddr_in direccion_servidor;

    if(argc > 2)
    {
        printf("El numero de argumentos pasado debe ser 'nombre del programa' y 'puerto de escucha'");
    }

    else
    {
        printf("El servidor se encuentra escuchando por el puerto %s\n", argv[1]);
    }

    char* puerto = argv[1];
    
    // Creación de un socket AF_INET(DOMINIO INTERNET TCP/IP), en modo no conectivo SOCK_DGRAM (UDP)
    //En caso de fallo devuelve -1, si todo va bien devuelve un valor entero bajo.

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_port = puerto;

    printf("%d\n", direccion_servidor.sin_port);
    


}






