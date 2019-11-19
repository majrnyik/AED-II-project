#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>


//  ######################### CÓDIGO PARA ESCREVER VIDEO.MP4 EM ARQUIVO #######################################################################################################
 /* #                                                                                                                                                                         #
	#		#include <stdio.h>                                                                                                                                                #                                                                                                                                            
    #                                                                                                                                                                         #
	#		// Video resolution                                                                                                                                               #
	#		#define W 1280                                                                                                                                                    #
	#		#define H 720                                                                                                                                                     #
	#                                                                                                                                                                         #
	#		// Allocate a buffer to store one frame                                                                                                                           #
	#		unsigned char frame[H][W][3] = {0};                                                                                                                               #           
	#                                                                                                                                                                         #
	#  	int main()                                                                                                                                                            #
	#	{                                                                                                                                                                     #
	#		int x, y, count;                                                                                                                                                  #
    #                                                                                                                                                                         #  
	#		// Open an input pipe from ffmpeg and an output pipe to a second instance of ffmpeg                                                                               #                                                            
	#		FILE *pipein = popen("ffmpeg -i teapot.mp4 -f image2pipe -vcodec rawvideo -pix_fmt rgb24 -", "r");                                                                #                                                                            
	#		FILE *pipeout = popen("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s 1280x720 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output.mp4", "w");            #
    #                                                                                                                                                                         #
	#		// Process video frames                                                                                                                                           #
	#		while(1)                                                                                                                                                          #
	#		{                                                                                                                                                                 # 
	#			// Read a frame from the input pipe into the buffer                                                                                                           #                                 
	#			count = fread(frame, 1, H*W*3, pipein);                                                                                                                       #                                                                                                                                                 
    #                                                                                                                                                                         # 
	#			// If we didn't get a frame of video, we're probably at the end                                                                                               #
	#			if (count != H*W*3) break;                                                                                                                                    #
    #                                                                                                                                                                         #
	#			// Process this frame                                                                                                                                         #
	#			for (y=0 ; y<H ; ++y) for (x=0 ; x<W ; ++x)                                                                                                                   #
	#			{                                                                                                                                                             #
	#				// Invert each colour component in every pixel                                                                                                            #
	#				frame[y][x][0] = 255 - frame[y][x][0]; // red                                                                                                             #
	#				frame[y][x][1] = 255 - frame[y][x][1]; // green                                                                                                           #
	#				frame[y][x][2] = 255 - frame[y][x][2]; // blue                                                                                                            #
	#			}                                                                                                                                                             #
	#			                                                                                                                                                              #
	#			// Write this frame to the output pipe                                                                                                                        #
	#			fwrite(frame, 1, H*W*3, pipeout);                                                                                                                             #
	#		}                                                                                                                                                                 #
    #                                                                                                                                                                         #
	#		// Flush and close input and output pipes                                                                                                                         #
	#		fflush(pipein);                                                                                                                                                   #
	#		pclose(pipein);                                                                                                                                                   #
	#		fflush(pipeout);                                                                                                                                                  #
	#		pclose(pipeout);                                                                                                                                                  #
	#		}                                                                                                                                                                 #
    ############################ CÓDIGO PARA ESCREVER VIDEO.MP4 EM ARQUIVO ####################################################################################################
*/

//unsigned char : 0 - 256 bytes
typedef unsigned char byte;

//estrutura da nossa árvore
typedef struct nodoArvore
{
    int                 frequencia; //quantidade de vezes que aparecera
    byte                c;//dado
    struct nodoArvore   *esquerda;//estrutura
    struct nodoArvore   *direita;//estrutura
} nodoArvore;//definicao

//fila de prioridade sera definida como uma lista ligada
typedef struct nodoLista
{
    nodoArvore          *n;
    struct nodoLista    *proximo;
} nodoLista;

typedef struct lista
{
    nodoLista   *head;
    int         elementos;
} lista;

/**
* A função strdup é dependente de implementação nas plataformas não POSIX (Windows, etc)
* Segue uma implementação desta função como solução para o problema.
*/

char *strdup(const char *s)
{
    char *p = malloc(strlen(s) + 1);
    if (p) strcpy(p, s);
    return p;
}

/** Alocação de memoria e trata os ponteiros soltos acerca de nós da lista encadeada.
* Obs: cada nó da lista encadeada é conectado a um nó 'raiz' de árvore.
*/

nodoLista *novonodoLista(nodoArvore *nArv)
{
    nodoLista *novo;
    if ( (novo = malloc(sizeof(*novo))) == NULL ) return NULL;

    // adiciona o valor do nodo a arvore
    novo->n = nArv;

    novo->proximo = NULL;

    return novo;
}

// Função que faz alocação de memória e trata os ponteiros soltos acerca de nós da árvore
nodoArvore *novonodoArvore(byte c, int frequencia, nodoArvore *esquerda, nodoArvore *direita)
{
    nodoArvore *novo;

    if ((novo = malloc(sizeof(*novo)))== NULL) 
		return NULL;

    //senao atribui os valorers
    novo->c = c;
    novo->frequencia = frequencia;
    novo->esquerda = esquerda;
    novo->direita = direita;

    return novo;
}


void insereLista(nodoLista *n, lista *l)
{
    if (!l->head)
    {
        l->head = n;
    }

    // Se o campo 'frequência' do 'nó' parâmetro for menor que o campo 'frequência' do primeiro item (head)
    // da lista, incluir o novo nó como head, e colocar o head antigo como next desse novo

    else if (n->n->frequencia < l->head->n->frequencia)
    {
		//faz troca
        n->proximo = l->head;
        l->head = n;
    }
    else
    {
        // aponta para o primeiro nodo
        nodoLista *aux = l->head->proximo;
        // aponta pro primeiro nodo
        nodoLista *aux2 = l->head;

        // Laço que percorre a lista e insere o nó na posição certa de acordo com sua frequência.
        while (aux && aux->n->frequencia <= n->n->frequencia)
        {
            aux2 = aux;
            aux = aux2->proximo;
        }

        // insere
        aux2->proximo = n;
        n->proximo = aux;
    }

    // ++quantidade de elementos
    l->elementos++;
}

//funcao que larga o nodo de menor frequencia
nodoArvore *popMinLista(lista *l)
{

    // primeirro nodo
    nodoLista *aux = l->head;

    // ponteiro pra arvore do primeiro nodo
    nodoArvore *aux2 = aux->n;

    // swap head pro segundo elemento
    l->head = aux->proximo;

    free(aux);
    aux = NULL;

    // --quantidade de elmentos
    l->elementos--;

    return aux2;
}

//conta frequencia dos bytes
void getByteFrequency(FILE *entrada, unsigned int *listaBytes)
{
    byte c;

	//fread( array de frames , tamanho de cada elemento, quantos elementos, arquivo de entrada )
    //fread retorna a quantidade de blocos lidos com sucesso
    // Faz a leitura de 1 bloco de tamanho 1 byte a partir do arquivo 'entrada'
    // e salva no espaco de memoria de 'c'.
    // Converte esse byte num valor decimal, acessa o bucket correspondente e incrementa o valor (frequência).

    while (fread(&c, 1, 1, entrada) >= 1)
    {
        listaBytes[(byte)c]++;
    }
    rewind(entrada); //volta para o inicio do arquivo

}


//  Obtem o código começando no nodo n, utilizando o byte salvo em 'c', preenchendo 'buffer', desde o bucket 'tamanho'
// Função recursiva que percorre uma árvore de huffman e para ao encontrar o byte procurado (c)


bool pegaCodigo(nodoArvore *n, byte c, char *buffer, int tamanho)
{
    // Se o nó for folha e o seu valor for o buscado, colocar o caractere terminal no buffer e encerrar

    if (!(n->esquerda || n->direita) && n->c == c) //encontra
    {
        buffer[tamanho] = '\0';
        return true;
    }
    else
    {
        bool encontrado = false;

        // Se existir um nó à esquerda
        if (n->esquerda)
        {
            // Adicione '0' no bucket do buffer correspondente ao 'tamanho' nodeAtual
            buffer[tamanho] = '0';

            // fazer recursão no nó esquerdo
            encontrado = pegaCodigo(n->esquerda, c, buffer, tamanho + 1);
        }

        if (!encontrado && n->direita)
        {
            buffer[tamanho] = '1';
            encontrado = pegaCodigo(n->direita, c, buffer, tamanho + 1);
        }
        if (!encontrado)
        {
            buffer[tamanho] = '\0';
        }
        return encontrado;
    }

}

// Algoritmo para construir a árvore de huffman, inspirado no seguinte pseudocódigo:
// http://www.cs.gettysburg.edu/~ilinkin/courses/Spring-2014/cs216/assignments/a8.html
//
// procedure BUILD-TREE(frequencies):
//     pq ← make empty priority queue
//     for each (symbol, key) in frequencies:
//         h ← make a leaf node for the (symbol, key) pair
//         INSERT(pq, h)
//
//     n ← size[pq]
//     for i = 1 to n-1:
//         h1 ← POP-MIN(pq)
//         h2 ← POP-MIN(pq)
//         h3 ← MAKE-NODE(h1, h2)
//         INSERT(pq, h3)
//
//     return POP-MIN(pq)

nodoArvore *BuildHuffmanTree(unsigned *listaBytes)
{
    // lista com head null e 0 elementos
    lista l = {NULL, 0};

    // enche usando a array 'listaBytes' (que contém as frequências) uma lista encadeada de nós.
    // cada nodo contém uma árvore.
    for (int i = 0; i < 256; i++)
    {
        if (listaBytes[i]) // Se existe ocorrência do byte
        {
            // Insere na lista 'l' um nodos referente ao byte i e sua respectiva frequência (guardada em listaBytes[i]).
            // faz os nodos esquerdo e direito das árvores apontarem para NULL;
            insereLista(novonodoLista(novonodoArvore(i, listaBytes[i], NULL, NULL)), &l); //novonodoLista(novonodoArvore(i, listaBytes[i], NULL, NULL)) representa o nodo da lista
        }
    }

    while (l.elementos > 1) //enquanto o número de elementos da lista for maior que 1
    {
        // Nó esquerdo chama a função popMinLista() que vai na lista e pega a árvore fixada no primeiro nó
        // (que é a que contém a menor frequência)
        nodoArvore *nodoEsquerdo = popMinLista(&l);

        // Pega o outro nó que tem menor frequência
        nodoArvore *nodoDireito = popMinLista(&l);

        // Preenche com '#' o campo de caractere do nó da árvore.
        // Preenche o campo 'frequência' com a soma das frequências de 'nodoEsquerdo' e 'nodoDireito'.
        // Aponta o nó esquerdo para nodoEsquerdo e o nó direito para nodoDireito
        nodoArvore *soma = novonodoArvore('#',nodoEsquerdo->frequencia + nodoDireito->frequencia,nodoEsquerdo,nodoDireito); //novonodoArvore(byte c, int frequencia, nodoArvore *esquerda, nodoArvore *direita)
        // Reinsere o nó 'soma' na lista l
        insereLista(novonodoLista(soma), &l);
    }

    return popMinLista(&l);
}


void FreeHuffmanTree(nodoArvore *n)
{
    if (!n) return;
    else
    {
        nodoArvore *esquerda = n->esquerda;
        nodoArvore *direita = n->direita;
        free(n);
        FreeHuffmanTree(esquerda);
        FreeHuffmanTree(direita);
    }
}

int geraBit(FILE *entrada, int posicao, byte *aux )
{
    // se puder, lê
    (posicao % 8 == 0) ? fread(aux, 1, 1, entrada) : NULL == NULL ;

    // Exclamação dupla converte para '1' (inteiro) se não for 0. Caso contrário, deixa como está (0)
    // Joga '1' na casa binária 'posicao' e vê se "bate" com o byte salvo em *aux do momento
    // Isso é usado para percorrer a árvore (esquerda e direita)
    return !!((*aux) & (1 << (posicao % 8)));
}

void erroArquivo()
{
    printf("Arquivo nao encontrado\n");
    exit(0);
}

//compressao do arquivo
void CompressFile(const char *arquivoEntrada, const char *arquivoSaida)
{

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    // Abre arquivo do parâmetro arquivoEntrada no modo leitura de binário
	// FILE *pipeout = popen("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 -s 1280x720 -r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output.mp4", "w")
	//fopen no arquivo gerado pelo ffmpeg

    FILE *entrada = fopen(arquivoEntrada, "rb");
    (!entrada) ? erroArquivo() : NULL == NULL ;

    // Abre arquivo do parâmetro arquivoSaida no modo escrita de binário
    FILE *saida = fopen(arquivoSaida, "wb");
    (!saida) ? erroArquivo() : NULL == NULL ;
	//frequencia dos frames contidos em vetores de Char gerados pelo algoritmo comentado no inicio do código
    getByteFrequency(entrada, listaBytes);

    // enche arvore com a lista de frequência de bytes
    nodoArvore *raiz = BuildHuffmanTree(listaBytes);

    // Grava a lista de frequência nos primeiros 256 bytes do arquivo
    fwrite(listaBytes, 256, sizeof(listaBytes[0]), saida);

    // Move o ponteiro do stream 'saida' para a posição posterior ao tamanho de um unsigned int
    // É aqui que posteriormente será salvo o valor da variável 'tamanho'
    fseek(saida, sizeof(unsigned int), SEEK_CUR); //seek cur move o ponteiro do arquivo para a posicao desejada

    byte c;
    unsigned tamanho = 0;
    byte aux = 0;

	//fread( array/bloco de memoria , tamanho de cada elemento, quantos elementos, arquivo de entrada )
    // fread retorna a quantidade de bytes lidos com sucesso
    // Faz a leitura de 1 bloco de tamanho 1 byte a partir do arquivo 'entrada'
    // e salva no espaco de memoria de 'c'.
    while (fread(&c, 1, 1, entrada) >= 1)
    {

        // Cria um buffer vazio
        char buffer[1024] = {0};

        // Busca o código começando no nó 'raiz', utilizando o byte salvo em 'c', preenchendo 'buffer', desde o bucket deste último
        pegaCodigo(raiz, c, buffer, 0);

        // Laço que percorre o buffer
        for (char *i = buffer; *i; i++)
        {
            // Se o char na posição nodeAtual for '1'
            if (*i == '1')
            {
                // 2 elevado ao resto da divisão de 'tamanho' por 8
                // que é o mesmo que jogar um '1' na posição denotada por 'tamanho % 8'
                //aux = aux + pow(2, tamanho % 8);
                aux = aux | (1 << (tamanho % 8));
            }

            tamanho++;

            // Já formou um byte, é hora de escrevê-lo no arquivo
            if (tamanho % 8 == 0)
            {
                fwrite(&aux, 1, 1, saida);
                // Zera a variável auxiliar
                aux = 0;
            }
        }
    }

    // Escreve no arquivo o que sobrou
    fwrite(&aux, 1, 1, saida);

    // Move o ponteiro do stream para 256 vezes o tamanho de um unsigned int, a partir do início dele (SEEK_SET)
    fseek(saida, 256 * sizeof(unsigned int), SEEK_SET);

    // Salva o valor da variável 'tamanho' no arquivo saida
    fwrite(&tamanho, 1, sizeof(unsigned), saida);

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    // Calcula tamanho dos arquivos
    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    FreeHuffmanTree(raiz);

    printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)\nTempo gasto: %gs\n", arquivoEntrada, tamanhoEntrada / 1000, arquivoSaida, tamanhoSaida / 1000, tempoGasto);
    printf("Taxa de compressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    fclose(entrada);
    fclose(saida);

}


//funcao para descompressao
void DecompressFile(const char *arquivoEntrada, const char *arquivoSaida)
{

    clock_t inicio, final;
    double tempoGasto;
    inicio = clock();

    unsigned listaBytes[256] = {0};

    // Abre arquivo do parâmetro arquivoEntrada no modo leitura de binário
    FILE *entrada = fopen(arquivoEntrada, "rb");
    (!entrada) ? erroArquivo() : NULL == NULL ;

    // Abre arquivo do parâmetro arquivoSaida no modo escrita de binário
    FILE *saida = fopen(arquivoSaida, "wb");
    (!saida) ? erroArquivo() : NULL == NULL ;

    // Lê a lista de frequência que encontra-se nos primeiros 256 bytes do arquivo
    fread(listaBytes, 256, sizeof(listaBytes[0]), entrada);

    // Constrói árvore
    nodoArvore *raiz = BuildHuffmanTree(listaBytes);

    // Lê o valor dessa posição do stream para dentro da variável tamanho
    unsigned tamanho;
    fread(&tamanho, 1, sizeof(tamanho), entrada);

    unsigned posicao = 0;
    byte aux = 0;

    // Enquanto a posicao for menor que tamanho
    while (posicao < tamanho)
    {
        // Salvando o nodoArvore que encontramos
        nodoArvore *nodeAtual = raiz;

        // Enquanto nodeAtual não for folha
        while ( nodeAtual->esquerda || nodeAtual->direita )
        {
            nodeAtual = geraBit(entrada, posicao++, &aux) ? nodeAtual->direita : nodeAtual->esquerda;
        }

        fwrite(&(nodeAtual->c), 1, 1, saida);
    }

    FreeHuffmanTree(raiz);

    final = clock();
    tempoGasto = (double)(final - inicio) / CLOCKS_PER_SEC;

    fseek(entrada, 0L, SEEK_END);
    double tamanhoEntrada = ftell(entrada);

    fseek(saida, 0L, SEEK_END);
    double tamanhoSaida = ftell(saida);

    printf("Arquivo de entrada: %s (%g bytes)\nArquivo de saida: %s (%g bytes)\nTempo gasto: %gs\n", arquivoEntrada, tamanhoEntrada / 1000, arquivoSaida, tamanhoSaida / 1000, tempoGasto);
    printf("Taxa de descompressao: %d%%\n", (int)((100 * tamanhoSaida) / tamanhoEntrada));

    fclose(saida);
    fclose(entrada);
}


int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Uso: huffman [OPCAO] [ARQUIVO] [ARQUIVO]\n\n");
        printf("Opcoes:\n");
        printf("\t-c\tComprime\n");
        printf("\t-x\tDescomprime\n");
        printf("\nExemplo: ./huffman -c comprima.isso nisso.hx\n");
        return 0;
    }

    if (strcmp("-c", argv[1]) == 0)
    {
        if (strstr(argv[3], ".hx"))
        {
            CompressFile(argv[2], argv[3]);
        }
        else
        {
            printf("O arquivo resultante da compressao deve ter a extensao '.hx'.\n");
            printf("Exemplo: \n\t./huffman -c comprima.isso nisso.hx\n");
            return 0;
        }
    }
    else if (strcmp("-x", argv[1]) == 0)
    {
        if (strstr(argv[2], ".hx"))
        {
            DecompressFile(argv[2], argv[3]);
        }
        else
        {
            printf("O arquivo a ser descomprimido deve ter a extensao '.hx'.\n");
            printf("Exemplo: \n\t./huffman -x descomprime.hx nisso.extensao\n");
            return 0;
        }
    }
    else
    {
        printf("Uso: huffman [OPCAO] [ARQUIVO] [ARQUIVO]\n\n");
        printf("Opcoes:\n");
        printf("\t-c\tComprime\n");
        printf("\t-x\tDescomprime\n");
        printf("\nExemplo: ./huffman -c comprima.isso nisso.hx\n");
        return 0;
    }

    //CompressFile("meslo.ttf", "meslo.hx");
    //DecompressFile("meslo.hx", "meslo_copy.ttf");

    return 0;
}