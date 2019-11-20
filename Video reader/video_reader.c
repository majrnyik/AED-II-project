//

// Video processing example using FFmpeg

// Written by Ted Burke - last updated 12-2-2017

//



#include <stdio.h>
#include <stdlib.h>


// Video resolution

#define W 1280

#define H 720

#define MAX_COLOR 256

#define colorType unsigned int


colorType ***Alocar_matriz_int (int i, int j, int k)
{
  colorType ***v;  /* ponteiro para a matriz */
  int   m, n;    /* variavel auxiliar      */
  if (i < 1 || j < 1 || k < 1) { /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (NULL);
     }
  /* aloca as linhas da matriz */
  v = (colorType ***) calloc (i, sizeof(colorType *));	/* Um vetor de m ponteiros para int */
  if (v == NULL) {
     printf ("** Erro: Memoria Insuficiente **");
     return (NULL);
     }
  /* aloca as colunas da matriz */
  for ( m=0; m < i; m++ ) {
      v[m] = (colorType **) calloc (j, sizeof(colorType *));	/* m vetores de n floats */
      if (v[m] == NULL) {
         printf ("** Erro: Memoria Insuficiente **");
         return (NULL);
         }
  }

  for(m=0; m<i; m++){
      for(n=0; n<j; n++){
          v[m][n] = (colorType *) calloc(k, sizeof(colorType));
          if(v[m][n] == NULL){
            printf ("** Erro: Memoria Insuficiente **");
            return (NULL);
          }
      }
  }

  return (v); /* retorna o ponteiro para a matriz */
}

colorType ***Liberar_matriz_int (int i, int j, int k, colorType ***v)
{
  int  m, n;  /* variavel auxiliar */
  if (v == NULL) return (NULL);
  if (i < 1 || j < 1 || k < 1) {  /* verifica parametros recebidos */
     printf ("** Erro: Parametro invalido **\n");
     return (v);
  }
  for (m=0; m<i; m++){
      for(n=0; n<j; n++){
        free (v[m][n]); /* libera as linhas da matriz */
      }
  } 
  free (v);      /* libera a matriz (vetor de ponteiros) */
  return (NULL); /* retorna um ponteiro nulo */
}

// Allocate a buffer to store one frame

unsigned char frame[H][W][3] = {0};



void main()

{
    int x, y, count;

    // Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg

    FILE *pipein = popen("ffmpeg -i teapot.mp4 -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");

    // Open arq where colors counting will be stored
    FILE *pontArq = fopen("video1.txt", "w");

    unsigned char r, g, b;
    colorType ***colors_frame = Alocar_matriz_int(MAX_COLOR, MAX_COLOR, MAX_COLOR);

    // Process video frames

    while(1)

    {
        // Read a frame from the input pipe into the buffer

        count = fread(frame, 1, H*W*3, pipein);

        // If we didn't get a frame of video, we're probably at the end

        if (count != H*W*3) break;

        // Process this frame

        for (y=0 ; y<H ; ++y) for (x=0 ; x<W ; ++x)

        {
            r = frame[y][x][0]; // red
            g = frame[y][x][1]; // green
            b = frame[y][x][2]; // blue

            colors_frame[r][g][b] = colors_frame[r][g][b]+1;
        }

    }

    colorType numPixels = 0;

    for(int red=0; red<MAX_COLOR; red++){
        for(int green=0; green<MAX_COLOR; green++){
            for(int blue=0; blue<MAX_COLOR; blue++){
                fprintf(pontArq, "%i\n", colors_frame[red][green][blue]);
                numPixels += colors_frame[red][green][blue];
            }
        }
    }

    printf("Number of counted pixels: %i\n", numPixels);

    
    // Flush and close input and output pipes

    fflush(pipein);

    pclose(pipein);

    fclose(pontArq);

    colors_frame = Liberar_matriz_int(MAX_COLOR, MAX_COLOR, MAX_COLOR, colors_frame);


}
