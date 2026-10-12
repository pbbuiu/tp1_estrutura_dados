#include "BASE.H"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define QUANTIDADE 1000

int main() {
    FILE *arquivo_binario;
    FILE *arquivo_texto;
    int numeros[QUANTIDADE];
    Registros registro;

    for (int i = 0; i < QUANTIDADE; i++) {
        numeros[i] = i + 1;
    }

    srand(time(NULL));

    /* Embaralha as chaves para obter uma ordem aleatória */
    for (int i = QUANTIDADE - 1; i > 0; i--) {
        int j = rand() % (i + 1);

        int temp = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = temp;
    }

    arquivo_binario = fopen("registros.bin", "wb");
    arquivo_texto = fopen("registros.txt", "w");

    if (arquivo_binario == NULL || arquivo_texto == NULL) {
        printf("Erro ao abrir os arquivos.\n");

        if (arquivo_binario != NULL)
            fclose(arquivo_binario);

        if (arquivo_texto != NULL)
            fclose(arquivo_texto);

        return 1;
    }

    for (int i = 0; i < QUANTIDADE; i++) {
        registro.chave = numeros[i];
        registro.dado1 = rand() % 10000;

        for (int j = 0; j < TAM_DADO2 - 1; j++) {
            registro.dado2[j] = 'A' + rand() % 26;
        }

        registro.dado2[TAM_DADO2 - 1] = '\0';

        if (fwrite(&registro, sizeof(Registros), 1, arquivo_binario) != 1) {
            printf("Erro ao gravar registro binario.\n");
            fclose(arquivo_binario);
            fclose(arquivo_texto);
            return 1;
        }

        fprintf(arquivo_texto, "%d \n",
                registro.chave);
    }

    fclose(arquivo_binario);
    fclose(arquivo_texto);

    printf("Arquivos gerados com sucesso!\n");

    return 0;
}