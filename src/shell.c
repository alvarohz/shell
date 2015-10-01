#include "headers.h" // declaraciones y prototipos
#include "fshell.h"

int main(int argc, char **argv, char **envp)
{
        init();
        pantallaBienvenida();
        shellPrompt();    // muestra el prompt
        while (TRUE) {
                entradaUsuario = getchar();
                switch (entradaUsuario) {
                case '\n':   // si el usuario solo presiona enter
                        shellPrompt();   // no pasa nada
                        break;
                default:
                        getLineaTexto();     // almacena la entrada de usuario en el buffer
                        manejarComandoUsuario(); // ejecuta el comando de usuario
                        shellPrompt();  // muestra el prompt
                        break;
                }
        }
        printf("\n");

        return 0;

}
