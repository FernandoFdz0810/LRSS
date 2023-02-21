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

#define LONG_PAQUETE 32
#define PUERTO 2000


//Creación del socket


int main(int argc, char *argv[])
{
    //Creacion de la estructura sockaddr_in inicializada a 0.

    struct sockaddr_in datos_servidor, datos_cliente;

    
    // Se rellena la estructura de los datos del servidor  y del cliente con 0's.

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
    paquetes. 

    */


    int long_serv = sizeof(datos_servidor);
    int long_cliente = sizeof(datos_cliente);
    char* msg_enviado[LONG_PAQUETE];
    int lg_msg_enviado = sizeof(msg_enviado)/8;
    char* msg_recibido[LONG_PAQUETE];
    int lg_msg_recibido = sizeof(msg_recibido)/8;
    double t_ms[4];
    double t_min = 0.0, t_max = 0.0, t_medio = 0.0;
    int paquetes_enviados = 0, paquetes_recibidos = 0, paquetes_perdidos = 0;

    /*
    En las siguientes lineas de código se almacena el valor del 'segundo' parámetro pasado por la linea de ordenes. 
    Se compara que el dato introducido sea 'ping' tanto minúsculas como mayúsculas. En caso contrario se solicita 
    al usuario que vuelva a introducir los datos. 
    */

    char* solicitud = argv[1];
    
    if(strcmp(argv[1], "ping") != 0 && strcmp(argv[1], "PING") != 0)
    {
        printf("El primer argumento que se debe de pasar debe de ser 'ping' (MAYUS o MINUS)\n");
        return -1;
    }

    /*
    Se crean más variables locales de tipo puntero a char para almacenar la dirección IP y el puerto
    en la que se encuentra escuchando el servidor. Se hace uso de la función atoi() para pasar el valor del puerto
    a entero. 

    Respecto a los datos del servidor, se utiliza la familita AF_INET especificando así que se va a utilizar
    TCP/IP. Se agrega a la estructura el valor del puerto y también la dirección IP.
    */
    
    char *nombre_direccion = argv[2];
    char *puerto_char = argv[3];
    int puerto = atoi(puerto_char);

    datos_servidor.sin_family = AF_INET;
    datos_servidor.sin_port = htons(puerto);
    datos_servidor.sin_addr.s_addr = inet_addr(nombre_direccion); 

    /*
    Respecto a los datos del cliente, es decir, nosotros mismos, especificamos de nuevo que se va a utilizar 
    TCP/IP. Y se agrega el puerto que va a utilizar el cliente definido por defecto mediante 
    #define PUERTO 2000.
    */

    datos_cliente.sin_family = AF_INET;
    datos_cliente.sin_port = htons(PUERTO);

    /*
    Se verifica que el numero de argumentos pasado por la linea de ordenes sea menor que 4. 
    En caso de que se introduzca de forma correcta, se verifica que el puerto este comprendido entre
    los rangos 1025-5000. 
    */

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
    Se utiliza connect para establecer conexión con el servidor. 
    Como argumentos que se le pasan, primero el descriptor del socket, creado anteriormente. 
    El socket se configura para establecer la conexión en los datos del servidor que hayamos 
    introducido al inicio del programa. 

    En caso de error devuelve -1.  
    */

    int conexion = connect(sockfd,(struct sockaddr *) &datos_servidor, long_serv);

    if (conexion < 0)
    {
        printf("Cliente: no se puede conectar con el servidor\n");
    }

    else
    {
        printf("\nConexión creada con el servidor.\n\n");
        
    }

    printf("Enviando datos...\n\n");
    printf("\nHaciendo ping con %s enviando una cantidad de %d bytes:\n\n", nombre_direccion, lg_msg_enviado);

    /*
    Se hace uso de un bucle for para enviar un total de 4 mensajes al servidor por defecto. 
    */

    for(int i = 0; i < 4; i++)
    {
        /*
        Una vez establecida la conexión con el servidor, y aceptada por el mismo se comienzan 
        a enviar mensajes con la función write().

        Esta toma como argumentos el descriptor del socket creado anteriormente. 
        Un puntero al buffer de datos (&msg_enviado), y la cantidad de bytes del buffer de datos.
        */
        int wr = write(sockfd, &msg_enviado, lg_msg_enviado);

        /*
        Se comienza un contador de tiempo en formato 'CLOCKS' (tiempos de reloj)
        mediante la funcion clock definida en la libreria'time.h'. 
        */
        clock_t comienzo = clock();

        /*
        La función write devuelve el número de bytes que se han escrito con exito. Por tanto, 
        si se devuelve un número diferente, es que ha ocurrido un error a la hora de enviar
        el mensaje, lo que quiere decir que el páquete que se ha tratado de enviar se ha perdido. 
        Así, se añade un contador a la variable creada anteriormente como paquete_perdido.
        */

        if(wr != lg_msg_enviado)
        {
            printf("Error al enviar mensaje con write\n");
            paquetes_perdidos += 1;
        }

        else
        {   
            /*
            En caso de que se envie de forma correcta, se añadirá un contador a la cantidad de 
            paquetes_enviados. 
            */
            paquetes_enviados += 1;
        }

        /*
         Añadimos una parada entre el envio y la recepción de mensajes mediante read() para que 
         la recepción sea un poco más 'realista'.
        */ 
        

        sleep(1.25);

        /*
        Una vez el paquete ha sido enviado, se pretende recibir la respuesta por parte del servidor,
        para ello se emplea la función read() que recibe como argumentos el descriptor del socket, 
        un puntero al buffer de destino (&msg_recibido), y la cantidad de bytes desde el socket 
        del servidor.  

        En caso de error devuelve '-1' y se añadiría un contador al número de paquetes_perdidos, 
        Devuelve 0 si el servidor ha cerrado la conexión. Por tanto, se cerraría el socket mediante 
        la funcion close()
        */

        int rd = read(sockfd, &msg_recibido, lg_msg_recibido);

        if (rd == -1)
        {
            printf("Error recibiendo datos\n");
            paquetes_perdidos += 1;
        }

        if(rd == 0)
        {
            printf("El servidor ha cerrado la conexión\n");
            close(sockfd);
        }

        else
        {

            /*
                En caso contrario de que se empiecen a recibir paquetes de forma correcta, 
                se añade un contador al número de paquetes_recibidos y se emplea de nuevo la función
                clock() para a posteriori calcular el tiempo en segundos transcurrido entre el envio
                del paquete al servidor y la recepción. 

                Estos tiempos se almacenan en un array de datos para cada uno de los paquetes enviados. Se realiza
                el calculo en milisegundos mediante la función pow().

                Se realizan una serie de comparaciones con condicionales if para verificar cual 
                es el paquete con menor y mayor tiempo de envio y recepción. 

            */

            paquetes_recibidos += 1;
            clock_t final = clock();
            double tiempo_transcurrido = 0.0;
            tiempo_transcurrido += (double)(final - comienzo) / CLOCKS_PER_SEC;
            t_ms[i] = tiempo_transcurrido / pow(10,-3);
            t_min = t_ms[0];
            if(t_ms[i] > t_max)
            {
                t_max = t_ms[i];
            }
            if(t_ms[i] < t_min)
            {
                t_min = t_ms[i];
            }

            /*
                Finalmente se muestran por pantalla para cada uno de los paquetes recibidos
                los siguientes datos por pantalla. IP del servidor, número de bytes recibidos,
                tiempo transcurrido y TTL(Time-To-Life)
            */

            printf("Respuesta desde: %s bytes: %d tiempo: %.3fms TTL = 115 \n", inet_ntoa(datos_servidor.sin_addr),lg_msg_recibido, t_ms[i]);
        }

    }

    /*
        Se realiza un bucle for para sumar en cada iteración el valor de cada uno de 
        los tiempos obtenidos entre el envio y la recepción del mensaje, para finalmente dividirlo
        entre el número total de mensajes enviados y obtener el tiempo medio.

        Se realiza también el cálculo de porcentaje de perdida. 

        Finalmente, se muestran los datos de las estadísticas por pantalla y por último se cierra
        la conexión del socket.  
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

    
    close(sockfd);
}