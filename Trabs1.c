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
	
    /* Dados s�o lidos de arquivo de texto ou do console? */
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

    /* L� dados de arquivo ou stdin */
    for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			fscanf(sudokuArq, "%d", &sudoku1[i][j]);
			
	}

	
	
	

    /* Mostra problema inicial */
    puts("\nConfigura��o Inicial:");

    imprimeSudoku(sudoku1);

    puts("\n-------------------------------------------------");
	puts("Resolvendo o Sudoku por backtracking recursivo...");
    puts("");

    if (sudokuBacktracking(sudoku1, 0, 0,0))
	{
		printf("\nA funcao sudokuBacktracking � chamada : %d vezes\n\n", qtdChamada);
        puts("Sudoku Resolvido!\n");
        imprimeSudoku(sudoku1);
    }
	else
	{
        puts("N�o foi poss�vel encontrar solu��o!");
    }
    
    return 0;
}

/*
 * Fun��o recursiva que implementa o backtracking quando necess�rio.
 * Valor de retorno:
 *	0: n�o � poss�vel encontrar solu��o v�lida; backtracking necess�rio
 *  1: solu��o parcial v�lida encontrada
 */
int sudokuBacktracking(int sudoku[][9], int linha, int coluna, int Nchamada) {
    int proximoNumero;
    
    qtdChamada++;
    

    Nchamada++;
    printf("Chamada %d: sudokuBaktracking(sudoku, %d, %d)\n", Nchamada,linha,coluna);

    
	/*
     * Se todas as linhas j� foram completadas, o jogo foi resolvido!
     */
    if (linha == 9)
	{
        return 1;
    }

    /*
     * Se a c�lula j� estiver preenchida, passa para a pr�xima (chamada recursiva)
     */
    if (sudoku[linha][coluna])
	{
		/* �ltima coluna? */
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
        /* A chamada recursiva falhou, ou seja, n�o foi poss�vel avan�ar na solu��o */
        printf("Retornando de sudokuBacktracking(sudoku, %d, %d): %d \n",linha,coluna,sudoku[linha][coluna]);
        return 0;
    }

    /*
     * Aqui a c�lula necessariamente ainda n�o foi preenchida. Testa os n�meros em
	 * sequ�ncia e, para cada solu��o parcial v�lida, faz uma chamada recursiva,
	 * aprofundando-se na �rvore de solu��es.
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
            
            /* A chamada recursiva falhou, ent�o � necess�rio testar o pr�ximo n�mero */
            sudoku[linha][coluna] = 0;
        }
    }
    
    /* Todos n�meros foram testados e n�o se encontrou solu��o parcial v�lida */
    printf("Retornando de sudokuBacktracking(sudoku, %d, %d): %d \n",linha,coluna,sudoku[linha][coluna]);
    return 0;
}

/*
 * Verifica se um n�mero � v�lido numa dada posi��o.
 */
int ehValido(int numero, int sudoku[][9], int linha, int coluna)
{
    int i = 0;
    
    /* Linha e coluna base do quadrado 3x3 a que pertence a c�lula testada */
    int linhaBase = (linha / 3) * 3;
    int colunaBase = (coluna / 3) * 3;
    
    /* Coordenadas dos elementos que sobram para testar depois de verificar
     * a viola��o da linha e da coluna */
    int linha1  = linhaBase + (linha + 1) % 3;
    int linha2  = linhaBase + (linha + 2) % 3;
    int coluna1 = colunaBase + (coluna + 1) % 3;
    int coluna2 = colunaBase + (coluna + 2) % 3;

    /* Verifica se o n�mero viola regra da linha ou da coluna */
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
