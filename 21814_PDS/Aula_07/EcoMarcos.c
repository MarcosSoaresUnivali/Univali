#include <stdio.h>
#include <fcntl.h>
#include <io.h>

int main()
{
    int TAM = 0;
    short entrada, saida;
    float p0 = 1, p1 = 0.5, t = 0.25; //0.0025;
    // 0.0025 = 20 amostras
    // 1 seg = Fs
    int Fs = 8000;
    int n1 = Fs*t;

    FILE *in_file, *out_file;
    if ((in_file = fopen("alo.pcm", "rb")) == NULL) // Abrindo o arquivo de entrada
    {
        printf("\nErro: Nao abriu o arquivo de entrada\n");
        return 0;
    }

    if ((out_file = fopen("resultado_eco_alo.pcm", "wb")) == NULL) // Criando o arquivo de saida com o nome alterado
    {
        printf("\nErro: Nao abriu o arquivo de saida\n");
        return 0;
    }

    while (fread(&entrada, sizeof(short), 1, in_file) > 0) // Calculando o tamanho do arquivo
    {
        TAM++;
    }
    rewind(in_file); // Enviando cursor para o inicio do arquivo
    short data[TAM];

    for(int i=0;i<TAM;i++)
        data[i]=0;

    //short Value = 0;
    for (int i = 0; i < TAM; i++)
    {
        fread(&entrada, sizeof(short), 1, in_file); // entrada recebe dado na posicao do cursor
        if((i-n1) < 0){
            data[i] = p0*entrada;
        }else{
            data[i] = p0*entrada + p1*data[i-n1];
        }
        
        saida = (short)data[i];

        fwrite(&saida, sizeof(short), 1, out_file);
        saida = 0;
    }

    printf("Arquivo gravado com sucesso!");
    fclose(out_file); // Fechando os arquivos
    fclose(in_file);  // Fechando os arquivos
    return 0;
}
