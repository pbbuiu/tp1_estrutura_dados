#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define QUANTIDADE 1000

int main() {
    FILE *arquivo_binario;
    FILE *arquivo_texto;
    int numeros[QUANTIDADE];

    for (int i = 0; i < QUANTIDADE; i++) {
        numeros[i] = i + 1;
    }

    srand(time(NULL));

    for (int i = QUANTIDADE - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }

    arquivo_binario = fopen("numeros.bin", "wb");
    arquivo_texto = fopen("numeros.txt", "w");

    if (arquivo_binario == NULL || arquivo_texto == NULL) {
        printf("Erro ao abrir os arquivos.\n");

        if (arquivo_binario != NULL) {
            fclose(arquivo_binario);
        }

        if (arquivo_texto != NULL) {
            fclose(arquivo_texto);
        }

        return 1;
    }

    for (int i = 0; i < QUANTIDADE; i++) {
        fwrite(&numeros[i], sizeof(int), 1, arquivo_binario);
        fprintf(arquivo_texto, "%d\n", numeros[i]);
    }

    fclose(arquivo_binario);
    fclose(arquivo_texto);

    printf("Arquivos gerados com sucesso!\n");

    return 0;
}