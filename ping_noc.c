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
#include <ctype.h>
#include <time.h>
#include <math.h>

#define LONG_PAQUETE 32
#define PUERTO 2000


//Creación del socket


int main(int argc, char *argv[])
{
    /*
    Se crean una serie de variables que se van a utilizar para el cálculo del tiempo entre el envio y la recepción de
    los mensajes. También se crean otras variables para calcular del número de paquetes enviados, cuantos se reciben
    y cuantos se pierden.
    */

    double t_ms[3];
    double t_min = 0.0, t_max = 0.0, t_medio = 0.0;
    int paquetes_enviados = 0, paquetes_recibidos = 0, paquetes_perdidos = 0; //Número de mensajes enviados


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

    char* msg_enviado[LONG_PAQUETE];
    memset(msg_enviado,0,LONG_PAQUETE);
    char* msg_recibido[LONG_PAQUETE];

    // En solicitud se guarda el "segundo" parámetro que se pasa por argumento. 

    char* solicitud = argv[1];
    

    // Se rellena la estructura de los datos del servidor  y del cliente con 0's.

    bzero((char*) &datos_servidor, sizeof(datos_servidor));
    bzero((char*) &datos_cliente, sizeof(datos_cliente));
    
    /*
    Con este if se pretende comprobar que el "segundo" parametro que se pasa por argumento
    al llamar al programa sea "ping" tanto minúsculas como mayúsculas.
    */

    if(strcmp(argv[1], "ping") != 0 && strcmp(argv[1], "PING") != 0)
    {
        printf("El primer argumento que se debe de pasar debe de ser 'ping' (MAYUS o MINUS)\n");
        return -1;
    }
    
    /*
    En la siguientes variables se almacenan la 'IP' pasada por argumento a la que se encuentra el servidor escuchando
    y su puerto. Como los argumentos que se pasan por la linea de comandos van por defecto en formato CHAR, para guardar el puerto
    como entero utilizamos la funcion atoi().
    */

    char *nombre_direccion = argv[2];
    char *puerto_char = argv[3];
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
    datos_servidor.sin_addr.s_addr = inet_addr(nombre_direccion); 

    /*
    Para el siguiente condicional if, se verifica que el numero de argumentos no sea mayor a 4. 
    */

    if(argc > 4)
    {
        printf("El número de argumentos pasado debe ser 'nombre del programa', 'direccion IP o nombre de host' y 'puerto de escucha'");
        return 0;
    }

    /*
    En caso de que se cumpla que haya 4 o menos argumentos. Se verifica que el puerto que se ha introducido
    por la linea de argumentos este comprendido en el rango de 1024-5000, ya que es el rango de puertos 
    que es utilizado para servidores como viene comentado en el libro.
    */

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

    /*
    Para la estructura del cliente se añade el tipo de familia que va a utilizar, en este caso
    AF_INET(TCP/IP) y se le agrega el puerto que va a utilizar definido por defecto el 2000, 
    */

    datos_cliente.sin_family = AF_INET;
    datos_cliente.sin_port = htons(PUERTO);

    /*
    Definimos una variable llamada ttl con valor 64. Con la función setsockopt() se establece
    el valor del campo TTL de los paquetes IP enviados. TTL permite establecer el numero de saltos de red
    que puede atravesar un paquete IP a través de los routers. 

    IPPROTO_IP sirve para especificar que se esta configurando a nivel IP. IP_TTL especifica la opción de TTL.
    &ttl es un puntero a la variable que contiene el valor del TTL deseado, por último sizeof(ttl) calcula el 
    tamaño de la variable ttl en bytes.     
    */

    int ttl = 64;
    int ttl_check = setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    if (ttl_check < 0)
    {
        printf("Error asociando TTL\n");
        return -1;
    }

    /*
    Se realiza la asociación una dirección local con el socket que hemos creado anteriormente. 
    El segundo parámetro contiene la dirección a la que se quiere asociar el socket. 
    El tercer parámetro especifica el tamaño de la estructura en bytes (calculado anteriormente).
    Provoca un error si el valor devuelto en 'asociación' es '-1'. 
    */

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
        /*
        Se muestran datos por pantalla especificando la dirección IP y puerto a la que se van a enviar los datos,
        para verificar que no ha habido ningun error. 
        */

        printf("Asociación del socket creada.\n\tPreparado para enviar por el puerto: %d\n", ntohs(datos_cliente.sin_port));
        printf("\tEnviando a la dirección IP: %s\n", inet_ntoa(datos_servidor.sin_addr));
    }

    printf("Enviando datos...\n\n");
    printf("\nHaciendo ping con %s: Enviando una cantidad de datos de %ld bytes\n\n", nombre_direccion, sizeof(msg_enviado)/8);

   // Se realiza un bucle for para que por defecto se realicen 4 envios.
   
   for(int i = 0; i < 4; i++)
    {
        /*
        Mediante la función send() se envian datos mediante el socket creado. 
        Se toman como argumentos el descriptor del socket creado, un puntero al buffer que contiene
        los datos que se van a enviar, además de su tamaño en bytes. 
        Además, también se indican los datos del servidor con el que nos queremos comunicar. 

        En caso de dar error, se devuelve un '-1'.
        */

        int send = sendto(sockfd, msg_enviado, LONG_PAQUETE, 0, (struct sockaddr *) &datos_servidor, long_serv);

        /*
        Se hace uso de la función clock() definida en la libreria 'time.h' para iniciar un contador de tiempo cuando
        se realiza el envio del mensaje. 
        */

        clock_t comienzo = clock();

        if(send < 0)
        {
            printf("Error enviando datos\n");
            // En caso de que ocurra un error, el contador de paquetes perdidos se incrementa en 1.
            paquetes_perdidos += 1;
        }

        else
        {
            // En caso de que se envie correctamente, el contador de paquetes enviados se incrementa en 1. 
            paquetes_enviados += 1;
        }

        /*
        A continuación, el cliente espera a recibir la respuesta por parte del servidor, a través del decriptor del socket. 
        Esta respuesta va a ser almacenada en la variable 'msg_recibido' y de la LONG_PAQUETE especificada.
        En caso de error, recvfrom devuelve '-1'
        */

        int n = recvfrom(sockfd, msg_recibido, LONG_PAQUETE, 0, (struct sockaddr *) 0, (int *) 0);
        // Se hace uso de la función sleep para que los datos aparezcan por pantalla de forma 'más realista'. 
        sleep(1.5);

        if(n < 0)
        {
            /*
            Si ocurre un error el contador de paquetes perdidos se incrementa en 1. 
            */
            printf("Error\n");
            paquetes_perdidos += 1;

        }

        else
        {
            /*
            En caso contrario, de haber recibido el paquete de forma correcta, el contador
            de paquetes recibidos se incrementa en 1. 
            En el instante de recibir el paquete, se inicia otro contador de tiempo denominado como 'final'
            */
            paquetes_recibidos += 1;
            clock_t final = clock();
            double tiempo_transcurrido = 0.0;

            /*
            Se calcula el tiempo transcurrido desde el envio del paquete hasta la recepción del mismo
            calculando su diferencia. El resultado dado sera por tiempos de reloj. Se divide por 'CLOCKS_PER_SEC'
            para obtener el tiempo en segundos. 
            Finalmente, se hace uso de la función pow() para pasar el tiempo a milisegundos.
            */

            tiempo_transcurrido += (double)(final - comienzo) / CLOCKS_PER_SEC;
            t_ms[i] = tiempo_transcurrido / pow(10,-3);
            t_min = t_ms[0];

            /*
            Se realizan una serie de condicionales para verificar cual es el tiempo 
            mínimo y máximo del envio de cada uno de los paquetes. 
            */

            if(t_ms[i] > t_max)
            {
                t_max = t_ms[i];
            }
            if(t_ms[i] < t_min)
            {
                t_min = t_ms[i];
            }

            // Finalmente se muestra por pantalla los datos relacionados con la recepción de los mensajes. 

            printf("Respuesta desde: %s bytes: %ld tiempo: %.3fms TTL = %d \n", inet_ntoa(datos_servidor.sin_addr),sizeof(msg_recibido)/8, t_ms[i], ttl);
        }

    }

    /*
    Por último se realizan una serie de cálculos para mostrar por pantalla las estadísticas
    de ping para la dirección IP del servidor con la que hemos solicitado la conexión. 
    */
    

    for(int i = 0; i < 4; i++)
    {
        t_medio += t_ms[i];
    }
    double perc_perdida = (paquetes_perdidos * 100)/paquetes_recibidos;
    double media = t_medio/4;


    printf("\nEstadísticas de ping para: %s:\n", inet_ntoa(datos_servidor.sin_addr));
    printf("\tPaquetes: Enviados = %d, Recibidos = %d, Perdidos = %d (%.2f%% perdida),\n", paquetes_enviados, paquetes_recibidos, paquetes_perdidos, perc_perdida);
    printf("Tiempo aproximado de vuelta en mili-segundos:\n");
    printf("\tMinimo = %.3fms, Máximo = %.3fms, Media = %.3fms\n\n", t_min, t_max, media);
    
}


