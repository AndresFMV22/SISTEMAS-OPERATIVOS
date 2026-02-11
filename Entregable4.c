#include <stdio.h>    // Para printf
#include <stdlib.h>   // Para exit
#include <unistd.h>   // Para fork(), getpid(), getppid()
#include <sys/wait.h> // Para wait()

int main()
{

    int i;
    int numero = 10;   // Aca en la variable ponemos el numero base para el cálculo
    int acumulado = 0; // Esta variable es donde el padre sumará los resultados
    pid_t pid;

    /*
        Este ciclo se ejecuta 3 veces para poder crear
        los tres procesos hijos.
    */
    for (i = 0; i < 3; i++)
    {

        pid = fork(); // Aqui fork crea una copia del proceso actual

        /*
            Si fork falla, devuelve un valor negativo
            En ese caso se termina el programa con un mensaje de error.
        */
        if (pid < 0)
        {
            perror("Error en fork");
            exit(1);
        }

        /*
            Cuando pid == 0 estamos dentro del proceso hijo.
            Cada hijo ejecutará este bloque
        */
        if (pid == 0)
        {

            printf("hijo[%d] -> pid = %d y ppid = %d\n",
                   i, getpid(), getppid());

            /*
                Cada hijo hace una parte del cálculo.
                En este caso suma el número base más su índice.
            */
            int parcial = numero + i;

            printf("hijo[%d] valor parcial: %d\n", i, parcial);

            /*
                El hijo termina aquí.
                Se usa exit para enviar el resultado al padre.
            */
            exit(parcial);
        }
    }

    /*
        Este código solo lo ejecuta el padre
        Aquí el padre espera a que cada hijo termine y recoge su resultado para acumularlo.
    */
    int status;
    int resultado;

    for (i = 0; i < 3; i++)
    {

        wait(&status); // El padre se bloquea hasta que un hijo finaliza

        /*
            Se verifica que el hijo terminó correctamente
            Luego se obtiene el valor que envió con exit()
        */
        if (WIFEXITED(status))
        {
            resultado = WEXITSTATUS(status);
            acumulado += resultado;
        }
    }
    /*  WIFEXITED permite verificar si un proceso hijo terminó normalmente mediante
        exit() antes de recuperar su código de salida con WEXITSTATUS().*/

    for (i = 0; i < 5; i++)
    {
        printf("rutina del proceso padre valor de i:%d\n", i);
    }

    /*
        Finalmente el padre muestra el acumulado total
        calculado con ayuda de los hijos
    */
    printf("\nAcumulado total: %d\n", acumulado);

    return 0;
}