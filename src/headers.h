#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/select.h>

#define TRUE 1
#define FALSE !TRUE

static const char* SHELL_VERSION = "1.0";

/* definicion de las estructuras de datos de entrada de usuario */
#define BUFFER_MAX_LONG 50              // tamaño maximo del buffer (50 chars)
static char entradaUsuario = '\0';
static char buffer[BUFFER_MAX_LONG];    // buffer para la entrada del usuario
static int bufferChars = 0;

static char *comandoArgv[5];       // vector de cadenas que componen el comando dado
static int comandoArgc = 0;        // contador de las palabras del comando dado

/* shell pid, default terminal */
static pid_t SHELL_PID;
static int SHELL_TERMINAL, SHELL_ES_INTERACTIVO;

/*********************************************************************
 * PROMPT
 *********************************************************************/

/* despliega la pantalla de bienvenida */
void pantallaBienvenida();
/* despliega el prompt de nuestra terminal */
void shellPrompt();

/*********************************************************************
 * FUNCIONES DE ENTRADA DE USUARIO
 *********************************************************************/

/* obtiene una linea de texto digitada por el usuario */
void getLineaTexto();
/* rellena el arreglo comandoArgv[] con cada palabra que compone la linea de comandos */
void rellenarComando();
/* libera el buffer y comandoArgv[] */
void destruirComando();

/*********************************************************************
 * FUNCIONES DEL SHELL
 *********************************************************************/

/* maneja el comando dado: es un comando incorporado o un programa externo? */
void manejarComandoUsuario();
/* ejecuta un comando de sistema */
void ejecutarComando(char *comando[]);
/* crear un procesos hijo */
void crearProceso(char *comando[]);
/* inicializa el shell */
void init();
