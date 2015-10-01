
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
        //isatty - test whether a file descriptor refers to a terminal
        /*STDIN_FILENO is a macro defined in <unistd.h>.
         * At least for a POSIX compliant system, it's not just "almost certainly 0";
         * it's required to be defined as 0.
        /STDIN_FILENO is the default standard input file descriptor number which is 0.
        It is essentially a defined directive for general use*/
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
 * rellena el array commandArgv con todas las palabras
 * de la linea de comandos (un espacio  " " es usado como separador)
 */
void rellenarComando()
{
        char* pbuffer;            // un puntero al buffer
        pbuffer = strtok(buffer, " ");
        /*The C library function char *strtok(char *str, const char *delim)
         * breaks string str into a series of tokens using the delimiter delim.*/

        while (pbuffer != NULL) {   // mientras el puntero no sea NULL
                comandoArgv[comandoArgc] = pbuffer;   // rellenar el array commandArgv
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
                comandoArgc--;                                                                       // decrement the number of words in the command line
        }
        bufferChars = 0;                                                                          // erase the number of chars in the buffer
}

/*********************************************************************
 * FUNCIONES DEL SHELL
 *********************************************************************/

/**
 * ejecucion generica void
 */
void manejarComandoUsuario()
{
	if ((checkBuiltInCommands(comandoArgv[0])) == 0) {
        crearProceso(comandoArgv);
	}
}

/**
 * comandos incorporados: exit, in, out, bg, fg, jobs, kill
 * returns 	1 if a built-in command is executed,
 * 			0 otherwise
 */
int checkBuiltInCommands()
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
 * crear un proceso con fork and ejecutar un programa hijo
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



