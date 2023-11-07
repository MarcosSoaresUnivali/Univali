//---------------------------------------------------------------------------------------------
//  Exemplo – Transformação “s” -> “z”
//
// *********************************************
//  fc =   1            a =     wc
//     -------              -----------
//      2.pi.RC              (Fl + wc)
//
//  wc = 2.pi.fc        b =  (wc - Fl)
//                          -----------
//                           (Fl + wc)
//---------------------------------------------------------------------------------------------
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

# define M_PI           3.14159265358979323846

int main()
{
    int TAM = 0;
    short entrada, saida;
    float y = 0, p0 = 1, p1 = 0.5, t = 0.0025;
    // 0.0025 = 20 amostras
    // 1 seg = Fs
    int Fs = 8000;
    int Fl = Fs * 2;
    int n1 = Fs * t;

    char file_inp[] = "sweep.pcm"; //"alo.pcm";
    char file_out[] = "out_Sweep.pcm"; //"resultado_eco.pcm";

    double      doispi  = M_PI * 2
            ,   fc      = 400
            ,   wc      = doispi * fc
            ,   a       = (wc / (Fl + wc))
            ,   b       = ((wc - Fl) / (Fl + wc));
    p0                  = a;
    p1                  = b;

    printf("\n  In_File = %s\n", file_inp);
    printf("\n  Ou_File = %s\n", file_out);
    printf("\n  ------------------- "); //return 0;
    printf("\n  Fs      = %d\n", Fs);
    printf("\n  Fl      = %d\n", Fl);
    printf("\n  2.pi    = %lf\n", doispi);
    printf("\n  fc      = %lf\n", fc);
    printf("\n  wc      = %lf\n", wc);
    printf("\n  a = p0  = %lf\n", p0);
    printf("\n  b = p1  = %lf\n", p1);
    printf("\n  ------------------- "); //return 0;
    return 0;

    FILE *in_file, *out_file;
    if ((in_file = fopen(file_inp, "rb")) == NULL) // Abrindo o arquivo de entrada
    {
        printf("\nErro: Nao abriu o arquivo de entrada\n");
        return 0;
    }

    if ((out_file = fopen(file_out, "wb")) == NULL) // Criando o arquivo de saida com o nome alterado
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
            data[i] = p0*entrada + p0*data[i-n1] - p1*data[i-n1];
        }
        
        saida = (short)data[i];

        fwrite(&saida, sizeof(short), 1, out_file);
        saida = 0;
    }

    fclose(out_file); // Fechando os arquivos
    fclose(in_file);  // Fechando os arquivos
 
    printf("\n  Gravado Ok -------- "); //return 0;
    printf("\n  Ou_File = %s\n", file_out);
    printf("\n  ------------------- "); //return 0;
    return 0;
}