
/*********************************************************************
 * PROMPT
 *********************************************************************/

void pantallaBienvenida()
{
        printf("\n-------------------------------------------------\n");
        printf("\tBienvenido a mi shell V. %s\n", SHELL_VERSION);
        printf("\tProcess id: %d\n", (int) SHELL_PID);
        printf("-------------------------------------------------\n");
        printf("\n\n");
}

/**
 *	despliega el prompt de nuestro shell
 */
void shellPrompt()
{
        printf("mishell# ");
}

void init()
{
        SHELL_PID = getpid();    // retorna el pid del shell
        SHELL_TERMINAL = STDIN_FILENO;     // terminal = STDIN
        SHELL_ES_INTERACTIVO = isatty(SHELL_TERMINAL); // el shell es interactivo si STDIN es el terminal
        //isatty - testea si el descriptor de archivo refiere a el terminal
        /*STDIN_FILENO es una macro definida en <unistd.h>.
         * Al menos para sistemas compatibles con POSIX,  es requerido que sea 0
        /STDIN_FILENO es el descriptor de archivo de entrada estandar por defecto el cual es 0
         * es esencialmente una directiva definida como cero*/
        if (SHELL_ES_INTERACTIVO) {     // es el shell interactivo?
        } else {
                printf("No es posible hacer el shell interactivo. Saliendo..\n");
                exit(EXIT_FAILURE);
        }
}

/*********************************************************************
 * FUNCIONES DE ENTRADA DE USUARIO
 *********************************************************************/

/**
 *	obtiene una linea de texto del teclado
 */
void getLineaTexto()
{
		destruirComando();    // limpia el buffer y la linea de comandos
        while ((entradaUsuario != '\n') && (bufferChars < BUFFER_MAX_LONG)) {
                buffer[bufferChars++] = entradaUsuario;
                entradaUsuario = getchar();
        }
        buffer[bufferChars] = 0x00;    // null para terminar el buffer
        rellenarComando();
}

/**
 * rellena el array comandoArgv con todas las palabras
 * de la linea de comandos (un espacio  " " es usado como separador)
 */
void rellenarComando()
{
        char* pbuffer;            // un puntero al buffer
        pbuffer = strtok(buffer, " ");
        /*La funcion de la libreria de C char *strtok(char *str, const char *delim)
        * divide la cadena str en una serie de tokens usando el delimitador delim*/

        while (pbuffer != NULL) {   // mientras el puntero no sea NULL
                comandoArgv[comandoArgc] = pbuffer;   // rellenar el array comandoArgv
                pbuffer = strtok(NULL, " "); // Posteriores llamadas
                comandoArgc++;
        }
}

/**
 *	destruye el array de chars, dejando que el usuario introduzca otra linea de comandos
 */
void destruirComando()
{
        while (comandoArgc != 0) {
                comandoArgv[comandoArgc] = NULL;  // elimina el puntero a la cadena
                comandoArgc--;        // decrementa el numero de las palabras en la linea
        }
        bufferChars = 0;              // reinicia el conteo de chars en el buffer
}

/*********************************************************************
 * FUNCIONES DEL SHELL
 *********************************************************************/

/**
 * ejecucion generica void
 */
void manejarComandoUsuario()
{
	if ((verificarComandosBuiltIn(comandoArgv[0])) == 0) {
        crearProceso(comandoArgv);
	}
}

/**
 * comandos incorporados:
 * retorna 1 si un comando built-in es ejecutado de lo contrario 0
 */
int verificarComandosBuiltIn()
{

        if (strcmp("cd", comandoArgv[0]) == 0) {
                cambiarDirectorio();
                return 1;
        }
        return 0;
}

/**
 * cambia el directorio actual
 */
void cambiarDirectorio()
{
	//chdir -> Cambia el directorio actual de trabajo
	//getenv -> Retorna una cadena conteniendo el valor de la variable de entorno cuyo nombre es
	//especificado como argumento
        if (comandoArgv[1] == NULL) {
                chdir(getenv("HOME"));            // simula un comando "cd"
        } else {
                if (chdir(comandoArgv[1]) == -1) {     // ir al nuevo directorio
                        printf(" %s: no such directory\n", comandoArgv[1]);
                }
        }
}
/**
 * ejecuta un programa
 */
void ejecutarComando(char *comando[])
{
        if (execvp(*comando, comando) == -1)
                perror("error en execvp");
}

/**
 * crear un proceso con fork y ejecuta un programa hijo
 */
void crearProceso(char *comando[])
{
        pid_t pid;
        pid = fork();
        switch (pid) {
        case -1:
                perror("error en fork");
                exit(EXIT_FAILURE);
                break;
        case 0:
                ejecutarComando(comando);
                exit(EXIT_SUCCESS);
                break;
        default:
                break;
        }
}



