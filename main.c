#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define TRUE 1

int main(void) {
  while (TRUE) {
    // Printar no prompt
    printf("$ - ");
    char *input = malloc(sizeof(char) * 100);
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = 0;

    // Verificação de comando exit para encerrar o shell
    if (strcmp(input, "exit") == 0) {
      puts("Parando o shell...");
      break;
    }
    
    char **args = malloc(90);
    char *token;
    int num_args = 0;
    // da um split no input para ter o comando e os argumentos
    token = strtok(input, " "); 
    while (token != NULL) {
      // vai colocando numa lista o comando e seus argumentos
      args[num_args] = token;
      num_args++;
      // meio que lembra da string e passa para o proximo token
      token = strtok(NULL, " ");
    }
    args[num_args] = NULL; // Termina os args com NULL (Default)

    pid_t child_pid = fork(); // Cria fork

    if (child_pid < 0) {
      puts("error na criação do processo filho");
    } else if (child_pid == 0) {
      // Executa o command + args e verifica se o comando deu -1
      if (execvp(args[0], args) == -1) {
        perror("Erro ao executar o comando");
      }
    } else {
      // O pai aqui espera  pelo processo filho antes de encerrar
      int status;
      waitpid(child_pid, &status, 0);
      printf("Filho com o PID %d terminou com status %d\n", child_pid, status);
    }
  }

  return 0;
}
