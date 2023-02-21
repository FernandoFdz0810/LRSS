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

#define LONG_PAQUETE 32
#define N_PETICIONES 4


//Creación del socket


int main(int argc, char *argv[])
{
    //Creacion de la estructura sockaddr_in inicializada a 0.

    struct sockaddr_in datos_servidor, datos_cliente;

    // Se rellena la estructura de los datos del servidor con 0's.

    bzero((char*) &datos_servidor, sizeof(datos_servidor));
    bzero((char*) &datos_cliente, sizeof(datos_cliente));

    /*
    En la siguientes lineas se crean una serie de variables que calculan la longitud 
    de la estructura datos_servidor y datos_cliente. 
    También se crean variables para el envio y la recepción de los mensajes de un tamaño 
    definido previamente en #define LONG_PAQUETE. Además de las correspondientes variables para tener 
    almacenado el tamaño correspondiente de cada mensaje en bytes (dividido entre 8).

    También se crean variables para almacenar en un array los tiempo entre envio y recepción del paquete.

    Por último se crean variables locales para el cálculo de las estadísticas del envio de los
    paquetes, etnre otras funciones. 

    */

    int long_serv = sizeof(datos_servidor);
    int long_cliente = sizeof(datos_cliente);
    char msg_recv[LONG_PAQUETE];
    char msg_enviado[LONG_PAQUETE];
    memset(msg_enviado,0,LONG_PAQUETE);
    int rd;
    int n_sockfd = 0;

    /*
    Se guarda el puerto introducido por la linea de comandos y
    se convierte a entero meidante atoi() para agregarlo posteriormente
    a la estructura sockaddr_in declarada como datos_servidor.
    */

    char *puerto_char = argv[1];
    int puerto = atoi(puerto_char);

    /*
    Respecto a los datos del servidor, se utiliza la familita AF_INET especificando así que se va a utilizar
    TCP/IP. Se agrega a la estructura el valor del puerto y también la dirección IP.
    */

    datos_servidor.sin_family = AF_INET;
    datos_servidor.sin_port = htons(puerto);
    datos_servidor.sin_addr.s_addr = inet_addr("192.168.2.3"); 

    /*
    Se verifica que el numero de argumentos pasado por la linea de ordenes sea menor que 2,
    es decir, el nombre del programa y el puerto donde se desea que escuche el servidor. 
    En caso de que se introduzca de forma correcta, se verifica que el puerto este comprendido entre
    los rangos 1025-5000. 
    */

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

    // Se realiza la asociación de los datos del servidor con el socket. 
    // En caso de error devuelve '-1'.

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
        printf("Asociación del socket creada.\n");
        printf("Escuchando por la dirección IP  : %s\n", inet_ntoa(datos_servidor.sin_addr));
        printf("Por el puerto: %d\n\n", puerto);
    } 

    /*
    La función listen se utlizar para aceptar conexiones entrante. 
    Toma como argumentos el descriptor del socket, y el número máximo de conexiones 
    en cola que se permiten establecer para ese socket. Por defecto se han establecido 
    un máximo de 4 peticiones.

    Devuelve un valor negativo en caso de error. 
    */

    int list = listen(sockfd, N_PETICIONES);

    if(list < 0)
    {
        printf("Error en la espera de peticiones\n");
        close(sockfd);
    }

    /*
    A continucación se emplea un bucle do...while para que el servidor
    se encuentre escuchando excepto si se produce algun error en la función accept. 

    La función accept lo que permite es aceptar las conexiones que esta escuchando 
    el servidor. Esta función recibe como argumentos el descriptor del socket que esta 
    esperando las conexiones, un puntero a una estructura de datos que almacena los datos 
    del cliente que se conecta al socket. Es una función bloqueante, es decir, se mantiene
    a la espera hasta que recibe peticiones. 

    Cuando se recibe una conexión entrante devuelve un nuevo descriptor de socket 
    y que es la que indica el establecimiento de la conexión con el cliente. 
    Este nuevo descriptor de socket es el que se utilizara para la recepción y el envio de
    los datos.
    */

    do
    {
        printf("\n\nEsperando peticiones...\n\n");

        n_sockfd = accept(sockfd, (struct sockaddr *) &datos_cliente, &long_cliente);

        if (n_sockfd < 0)
        {
            printf("Error en la petición procedente de %s\n", inet_ntoa(datos_cliente.sin_addr));
            break;
        }

        else
        {
            printf("\tAceptando conexión procedente de %s\n\n", inet_ntoa(datos_cliente.sin_addr));
        }

        /*
        A continuación se emplea otro bucle do...while por si ocurriera algun error 
        en la recepción de los datos mediante read(). En caso de que ocurriera algun error, se cerraría la
        conexión con el NUEVO descriptor del socket, manteniendose abierto el descriptor 
        que almacena las peticiones de conexión. 
        */

        do
        { 
            rd = read(n_sockfd, &msg_recv, sizeof(msg_recv));
        
            if (rd <= 0)
            {
                printf("\tCerrando conexión TCP con el cliente ubicado en %s\n", inet_ntoa(datos_cliente.sin_addr));
                close(n_sockfd);
                break;
            }

            else
            {
                printf("\t\nLeyendo datos de %s\n", inet_ntoa(datos_cliente.sin_addr));
            }

            /*
            Una vez se han leido los mensajes enviados por el cliente se procede
            al envio de los datos del servidor al cliente. 
            La función write() recibe como argumentos el nuevo descriptor de socket, 
            un puntero al buffer de datos que se van a enviar, y la cantidad de datos
            que se van a enviar a través del buffer de datos.
            */

            int wr = write(n_sockfd, &msg_enviado, LONG_PAQUETE);

            if(wr < 0)
            {
                printf("\nError al enviar el mensaje al cliente ubicado en %s\n\n", inet_ntoa(datos_cliente.sin_addr));
            }

            else
            {
                printf("\n\tEnviando el mensaje de tamaño %ld bytes al cliente ubicado en %s\n", sizeof(msg_enviado), inet_ntoa(datos_cliente.sin_addr));
            }

        } while (rd != -1);
    } while (n_sockfd != -1);   
}