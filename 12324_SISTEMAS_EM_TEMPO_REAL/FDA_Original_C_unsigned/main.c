/* =================================================================================
    FDA (Filtro de Difusão Anisotrópica)
    Professor:    Felipe Viel
    Curso:        Engenharia de Computação
    Disciplina:   Sistemas de Tempo Real
    Aluno:        Marcos Paulo Soares
    
    Avaliação 1 – Avaliação de Processos Críticos - Análise temporal 
    
    gcc -o fda pgmtolist.c pgmfiles.c diff2d.c main.c -lm

   ================================================================================= */

#include  <stdio.h>
#include  <stdlib.h>
#include  <string.h>
#include  <math.h>
#include  "pgmfiles.h"
#include  "diff2d.h"
#include  <time.h>
#include  <pthread.h>

#define   NTHREADS    1
#define   ARRAYSIZE   256
#define   ITERATIONS  ARRAYSIZE / NTHREADS

#define HI(num)	(((num) & 0x0000FF00) << 8)
#define LO(num)	((num) & 0x000000FF)

float             **matrix;
eightBitPGMImage  *PGMImage;
eightBitPGMImage  *PGMImage_N;
long              imax;
float             lambda;
int               result;
clock_t           start_time1, start_time2, start_time3, start_time4, start_time5,
                  start_time6, start_time7, end_time1, end_time2, end_time3, 
                  end_time4, end_time5, end_time6, end_time7, begin_time, 
                  finish_time, elapsed_time;


/* ---------------------------------------------------------------------------------
    Função crítica (critical_function) para a otimização dos tempos criticos
   --------------------------------------------------------------------------------- */
void *critical_function(void *tid) 
{
  /* Initialize my part of the global array and keep local sum */
  PGMImage_N = (eightBitPGMImage *) malloc(sizeof(eightBitPGMImage));
  int     i, j, start, *mytid, end;
  mytid = (int *) tid;
  start = (*mytid * ITERATIONS);
  end = start + ITERATIONS;

  /* MPS matrix copy N part */
  float   **c;  /* work copy of matrix */

  /* ---- allocate storage for c ---- */
  c = (float **) malloc ((ITERATIONS+2) * sizeof(float *));
  if (c == NULL)
    {
      printf("\n[Thread %5d] Not enough storage available\n",*mytid);
      exit(1);
    }
    for (i=0; i<=ITERATIONS+1; i++)
      {
        c[i] = (float *) malloc ((PGMImage->y+2) * sizeof(float));
        if (c[i] == NULL)
          {
            printf("\n[Thread %5d] Not enough storage available\n",*mytid);
            exit(1);
          }
      }


  /* ---- copy matrix part into c ---- */
  for (i=1; i<=ITERATIONS; i++)
    for (j=1; j<=PGMImage->y; j++)
      c[i][j] = matrix[start+i-1][j-1];

  printf ("\n[Thread %5d] Doing iterations \t%10d to \t %10d \n",*mytid,start,end-1); 

/*
  for (i=0; i<PGMImage_N->x; i++)
    for (j=0; j<PGMImage_N->y; j++)
      *(PGMImage_N->imageData + i*PGMImage_N->y + j) = (char) c[i][j];
*/

  for (i=1; i<=imax; i++)
    {
      printf("\n[Thread %5d] Iteration number: %5d \n",*mytid, i);
      diff2d (0.5, lambda, ITERATIONS, PGMImage->y, c); 
    }

  free(c);

/*
  for (i=1; i <=ITERATIONS; i++)
    {
      for (j=1; j<=2; j++)
        {                                   //PGMImage->y
          printf("matrix_c[%d][%d] -> %.2f ", i, j, c[i][j] );
        }
      printf("\n");
    }
  printf("\n");
  printf("*************\n");

*/

  //printf ("\n[Thread %5d] Sum %e",*mytid,mysum); 
  pthread_exit(NULL);

}


/* ---------------------------------------------------------------------------------
    Função Principal
   --------------------------------------------------------------------------------- */
void main (int argc, char **argv)
{

  char              row[80];
  //float           **matrix;
  int               i, j, start, tids[NTHREADS];
  FILE              *inimage, *outimage;
  //long              imax;
  //float             lambda;
  //int               result;
  //eightBitPGMImage  *PGMImage;
  
/* ---------------------------------------------------------------------------------
    0. Inicio begin time (Tempo Total)
   --------------------------------------------------------------------------------- */ 
  printf("\n[MAIN] FDA Start running... \n");
  begin_time = clock();


/* ---------------------------------------------------------------------------------
    1. Inicio Leitura da Imagem (lena_noise.pgm)
   --------------------------------------------------------------------------------- */
  start_time1 = clock();
  PGMImage = (eightBitPGMImage *) malloc(sizeof(eightBitPGMImage));
	
  if (!argv[1])
    {
	    printf("\n[MAIN] Name of input PGM image file (with extender): ");
      scanf("%s", PGMImage->fileName);
    }
  else
    {
      strcpy(PGMImage->fileName, argv[1]);
    }

  result = read8bitPGM(PGMImage);

  if(result < 0) 
    {
      printPGMFileError(result);
      exit(result);
    }

  end_time1 = clock();
  elapsed_time = end_time1 - start_time1;
  printf("\n[MAIN] Elapsed time 1 (read PGM in file): %lu clock ticks\n", elapsed_time);


/* ---------------------------------------------------------------------------------
    2. Inicio alocação para a matriz
   --------------------------------------------------------------------------------- */
  start_time2 = clock();

  matrix = (float **) malloc (PGMImage->x * sizeof(float *));
  if (matrix == NULL)
    { 
      printf("\n[MAIN] Not enough storage available\n");
      exit(1);
    } 
  for (i=0; i<PGMImage->x; i++)
    {
      matrix[i] = (float *) malloc (PGMImage->y * sizeof(float));
      if (matrix[i] == NULL)
        { 
	        printf("\n[MAIN] Not enough storage available\n");
	        exit(1);
        }
    }

  end_time2 = clock();
  elapsed_time = end_time2 - start_time2;
  printf("\n[MAIN] Elapsed time 2 (alloc matrix): %lu clock ticks\n", elapsed_time);


/* ---------------------------------------------------------------------------------
    3. Inicio Le a imagem e salva na matriz
   --------------------------------------------------------------------------------- */
  start_time3 = clock();

  for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      matrix[i][j] = (float) *(PGMImage->imageData + (i*PGMImage->y) + j);

  end_time3 = clock();
  elapsed_time = end_time3 - start_time3;
  printf("\n[MAIN] Elapsed time 3 (store matrix): %lu clock ticks\n", elapsed_time);


/* ---------------------------------------------------------------------------------
    4. Inicio Processa a imagem e executa o FDA
   --------------------------------------------------------------------------------- */
  printf("\n[MAIN] Contrast paramter lambda (>0): ");
  scanf("%f", &lambda);

  printf("\n[MAIN] Number of iterations: ");
  scanf("%ld", &imax);

  if (NTHREADS == 1)
    {
      start_time4 = clock();

      for (i=1; i<=imax; i++)
        {
          printf("\n[MAIN] Iteration number: %5d \n", i);
          diff2d (0.5, lambda, PGMImage->x, PGMImage->y, matrix); 
        }

      end_time4 = clock();
      elapsed_time = end_time4 - start_time4;
      printf("\n[MAIN] Elapsed time 4 (diff2d function): %lu clock ticks\n", elapsed_time);
    }
  else
    {
      start_time4 = clock();

      pthread_t threads[NTHREADS];
      pthread_attr_t attr; 
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
      for (i=0; i<NTHREADS; i++)
      {
        tids[i] = i;
        pthread_create(&threads[i], &attr, critical_function, (void *) &tids[i]);
      }

      /* Wait for all threads to complete... */ 
      for (i=0; i<NTHREADS; i++)
      {
        pthread_join(threads[i], NULL);
      }  

      end_time4 = clock();
      elapsed_time = end_time4 - start_time4;
      printf("\n[MAIN] Elapsed time 4 (diff2d function): %lu clock ticks\n", elapsed_time);

      /* Clean up and exit */
      pthread_attr_destroy(&attr);  
      pthread_exit (NULL);
    }

/* ---------------------------------------------------------------------------------
    5. Inicio Copia a imagem filtrada para a estrutura de PGM
   --------------------------------------------------------------------------------- */
  start_time5 = clock();

  for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      *(PGMImage->imageData + i*PGMImage->y + j) = (char) matrix[i][j];

  end_time5 = clock();
  elapsed_time = end_time5 - start_time5;
  printf("\n[MAIN] Elapsed time 5 (copy 2 PGM): %lu clock ticks\n", elapsed_time);


/* ---------------------------------------------------------------------------------
    6. Grava a imagem de saida para PGM
   --------------------------------------------------------------------------------- */
  start_time6 = clock();
  
  if (!argv[2])
  {
    printf("\n[MAIN] Name of output PGM image file (with extender): ");
    scanf("%s", PGMImage->fileName);
  }
  else
  {
    strcpy(PGMImage->fileName, argv[2]);
  }

  write8bitPGM(PGMImage);
  
  end_time6 = clock();
  elapsed_time = end_time6 - start_time6;
  printf("\n[MAIN] Elapsed time 6 (save PGM): %lu clock ticks\n", elapsed_time);


/* ---------------------------------------------------------------------------------
    7. Desaloca variaveis
   --------------------------------------------------------------------------------- */
  start_time7 = clock();
  
  for (i=0; i<PGMImage->x; i++)
    free(matrix[i]);

  free(matrix);
  free(PGMImage->imageData);
  free(PGMImage);

  end_time7 = clock();
  finish_time = clock();
  elapsed_time = end_time7 - start_time7;
  printf("\n[MAIN] Elapsed time 7 (unalloc): %lu clock ticks\n", elapsed_time);
  elapsed_time = finish_time - begin_time;
  printf("\n[MAIN] Total Elapsed time: %lu clock ticks\n", elapsed_time);

}