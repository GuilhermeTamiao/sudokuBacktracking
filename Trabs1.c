#include <stdio.h>
#include <conio.h>
#include <locale.h>



int ehValido(int numero, int sudoku[][9], int linha, int coluna);
int sudokuBacktracking(int sudoku[][9], int linha, int coluna, int Nchamada);
void imprimeSudoku(int sudoku[][9]);

int qtdChamada = 0;

int main(int argc, char** argv)
{
    int sudoku1[9][9];
    char* nomeArquivo;
    FILE* sudokuArq;
    int i, j, nChamada;

	setlocale(LC_ALL, "Portuguese");
	
    /* Dados são lidos de arquivo de texto ou do console? */
    if (argc > 1)
	{
        nomeArquivo = argv[1];
        sudokuArq = fopen(nomeArquivo, "r");
    } else
	{
        sudokuArq = stdin;
    }

    if (sudokuArq == NULL)
	{
        puts("Erro de leitura de arquivo!");
        return 1;
    }

    /* Lê dados de arquivo ou stdin */
    for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			fscanf(sudokuArq, "%d", &sudoku1[i][j]);
			
	}

	
	
	

    /* Mostra problema inicial */
    puts("\nConfiguração Inicial:");

    imprimeSudoku(sudoku1);

    puts("\n-------------------------------------------------");
	puts("Resolvendo o Sudoku por backtracking recursivo...");
    puts("");

    if (sudokuBacktracking(sudoku1, 0, 0,0))
	{
		printf("\nA funcao sudokuBacktracking é chamada : %d vezes\n\n", qtdChamada);
        puts("Sudoku Resolvido!\n");
        imprimeSudoku(sudoku1);
    }
	else
	{
        puts("Não foi possível encontrar solução!");
    }
    
    return 0;
}

/*
 * Função recursiva que implementa o backtracking quando necessário.
 * Valor de retorno:
 *	0: não é possível encontrar solução válida; backtracking necessário
 *  1: solução parcial válida encontrada
 */
int sudokuBacktracking(int sudoku[][9], int linha, int coluna, int Nchamada) {
    int proximoNumero;
    
    qtdChamada++;
    

    Nchamada++;
    printf("Chamada %d: sudokuBaktracking(sudoku, %d, %d)\n", Nchamada,linha,coluna);

    
	/*
     * Se todas as linhas já foram completadas, o jogo foi resolvido!
     */
    if (linha == 9)
	{
        return 1;
    }

    /*
     * Se a célula já estiver preenchida, passa para a próxima (chamada recursiva)
     */
    if (sudoku[linha][coluna])
	{
		/* Última coluna? */
        if (coluna == 8)
		{
            if (sudokuBacktracking(sudoku, linha + 1, 0, Nchamada))
				return 1;
        }
		else
		{
            if (sudokuBacktracking(sudoku, linha, coluna + 1, Nchamada))
				return 1;
        }
        /* A chamada recursiva falhou, ou seja, não foi possível avançar na solução */
        printf("Retornando de sudokuBacktracking(sudoku, %d, %d): %d \n",linha,coluna,sudoku[linha][coluna]);
        return 0;
    }

    /*
     * Aqui a célula necessariamente ainda não foi preenchida. Testa os números em
	 * sequência e, para cada solução parcial válida, faz uma chamada recursiva,
	 * aprofundando-se na árvore de soluções.
     */
    for (proximoNumero = 1; proximoNumero < 10; proximoNumero++)
	{
        if(ehValido(proximoNumero, sudoku, linha, coluna))
		{
            sudoku[linha][coluna] = proximoNumero;
            if (coluna == 8)
			{
                if (sudokuBacktracking(sudoku, linha + 1, 0, Nchamada))
					return 1;
            }
			else
			{
                if (sudokuBacktracking(sudoku, linha, coluna + 1, Nchamada))
					return 1;
            }
            
            /* A chamada recursiva falhou, então é necessário testar o próximo número */
            sudoku[linha][coluna] = 0;
        }
    }
    
    /* Todos números foram testados e não se encontrou solução parcial válida */
    printf("Retornando de sudokuBacktracking(sudoku, %d, %d): %d \n",linha,coluna,sudoku[linha][coluna]);
    return 0;
}

/*
 * Verifica se um número é válido numa dada posição.
 */
int ehValido(int numero, int sudoku[][9], int linha, int coluna)
{
    int i = 0;
    
    /* Linha e coluna base do quadrado 3x3 a que pertence a célula testada */
    int linhaBase = (linha / 3) * 3;
    int colunaBase = (coluna / 3) * 3;
    
    /* Coordenadas dos elementos que sobram para testar depois de verificar
     * a violação da linha e da coluna */
    int linha1  = linhaBase + (linha + 1) % 3;
    int linha2  = linhaBase + (linha + 2) % 3;
    int coluna1 = colunaBase + (coluna + 1) % 3;
    int coluna2 = colunaBase + (coluna + 2) % 3;

    /* Verifica se o número viola regra da linha ou da coluna */
    for (i = 0; i < 9; i++)
	{
		/* Viola regra da linha ou da coluna? */
        if ((sudoku[i][coluna] == numero) || (sudoku[linha][i] == numero))
			return 0;
    }
    
    /* Verifica os 4 elementos restantes no quadrado 3x3 */
    if((sudoku[linha1][coluna1] == numero) || (sudoku[linha1][coluna2] == numero)) 
		return 0;
    if((sudoku[linha2][coluna1] == numero) || (sudoku[linha2][coluna2] == numero))
		return 0;
	
	/* Nenhuma regra violada! */
    return 1;
}

/*
 * Imprime o sudoku
 */
void imprimeSudoku(int sudoku[][9])
{
    int i, j;
    char c;
    
    for (i = 0; i < 9; i++)
	{
        for (j = 0; j < 9; j++)
		{
			if (sudoku[i][j])
				c = '0' + sudoku[i][j];
			else
				c = ' ';
            if (j == 2 || j == 5)
			{
                printf("%c | ", c);
            } else if (j == 8)
			{
                printf("%c\n", c);
            } else
			{
                printf("%c ", c);
            }
        }
        if (i == 2 || i == 5) {
            puts("------+-------+------");
        }
    }
}
