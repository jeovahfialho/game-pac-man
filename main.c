/*
Autor: Jeovah Fialho
Cursando Bacharelado em Ciencia da Computacao - Universidade de Brasilia
E-mail: jeovahfialho@gmail.com

Versao do compilador: gcc (Ubuntu 4.3.2-1ubuntu11) 4.3.2

Programa testado no GCC GNU e no Windows. Padrao ANSI

Descricao do Problema: Dada uma planta baixa inicial (composta por uma parte superior, uma parte
inferior e um corredor entre as duas partes), que sera armazenada em uma matriz 23x75 do
tipo char, construir um programa que permita ao usuario dividir a planta inicial em
Partes inserindo as divisorias dos Partes (das partes superior e inferior) e as posicoes
das portas de cada comodo. Alem disso, o programa deve solicitar ao usuario o numero de
frutas as serem dispostas em cada comodo, bem como as posicoees (linha e coluna) de cada
fruta inserida. O jogo consiste em um elemento, denominado Come-Come (representado
pelo caracter 'C'), que deve se deslocar de uma posicao inicial (especificada abaixo),
percorrendo o corredor central da planta e entrando em cada comodo em busca das frutas
inseridas. O Come-Come deve se deslocar no corredor, da esquerda para a direita, entrando
sempre na porta mais proxima (seja para um comodo superior ou inferior). Alem disso, o
Come-Come deve iniciar a busca sempre pela fruta mais proxima dentro do comodo, e
assim por diante, ate que todas as frutas tenham sido comidas. Caso nco hajam frutas
dentro do comodo o Come-Come deve apenas entrar uma posicao dentro do comodo e
voltar ao corredor. Apos percorrer todos os Partes o Come-Come deve parar na saida
(posicao no final do corredor, oposta a posicao inicial de largada). Deve ser mostrado ao
usuario o numero total de passos que o Come-Come percorreu ate parar na saida. Dentro de
um comodo o Come-Come deve se locomover primeiro na horizontal, e depois na vertical. */


# include <stdio.h>
# include <stdlib.h> /* Para utilizar a funcao system() e a funcao sleep() */
# include <string.h> /* Para utiliza a funcao sctcmp */
# include <unistd.h>

# define LIN 23 /* Numero de Linhas da Matriz */
# define COL 75 /* Numero de Colunas da Matriz */
# define CS LIN/2 - 1 /* Linha que delimita o corredor por cima */
# define CI LIN/2 + 1 /* Linha que delimita o corredor por baixo */
# define MAXDIV ((COL - 1)/4 - 1) +2 /* Numero maximo de divisorias por partes + 2 (Mais dois pra incluir as paredes que delimitam o campo)
Simplesmente pra evitar reservar um espaco maior na memoria */
# define PACMAN 'C' /* Caracter que ira representar o PAC-MAN */
# define FRUTA '*' /* Caracter que ira representar o objeto que o PAC-MAN ira pegar */
# define TIME 0.5 /* Define o tamanho de espera da funcao sleep. Funciona diferente para Windows de linux, quando mudar o sistema alterar aqui.
Normalmente, para ficar boa a visualização 10 para Windows e 1 para Linux. Inicialmente usaremos o valor para Windows, 10 */


/**
@brief Desenhar as colunas e as linhas que serao padrao do campo;
Receberao (-) na Horizontal e (|) na Vertical;
Nas arestas (Considerando a matriz com se fosse um quadrado) recebera o caracter (+);
O restante das posicoes ira receber um espaco em branco (' ').
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@return Nenhum
*/
void DesenhaCampo(char Campo[][COL]){

    int linhas,colunas;/* Variaveis auxiliares para caminhar pela da matriz */

    /* Todos os as posicoes da matriz que nao fazem parte da aresta imaginaria
    considerando a matriz como um quadrado vao receber (' ') - Um espaco em branco.
    Pra quando for imprimir, nao aparecer nenhum lixo de memoria */
    for (linhas=1;linhas<LIN - 1;linhas++)
        for (colunas=1;colunas<COL - 1;colunas++)
            Campo[linhas][colunas] = ' ';

    /* Todas as colunas das linhas 0, CS, CI e LIN-1 (Onde LIN eh o numero de
    posicoes declaradas na matriz) vao receber o caracter (-) */
    for (colunas=1;colunas<COL - 1;colunas++){
        Campo[0][colunas] = '-';
        Campo[CS][colunas] = '-';/* Posicao da linha acima do corredor do PACMAN */
        Campo[CI][colunas] = '-';/* Posicao da linha abaixo do corredor do PACMAN */
        Campo[LIN - 1][colunas] = '-';
    }

    /* Todas as linhas das colunas 0 e LIN - 1 vao receber o caracter (|) */
    for (linhas=1;linhas<LIN - 1;linhas++){
        Campo[linhas][0] = '|';
        Campo[linhas][COL - 1] = '|';
    }

    /* Posicao onde ficara o PAC-MAN */
    Campo[LIN/2][0] = PACMAN;

    /* Colocando o simbolo (+) nas arestas */
    for (colunas=0;colunas<COL;colunas = colunas + COL - 1){
        Campo[0][colunas] = '+';
        Campo[CS][colunas] = '+';
        Campo[CI][colunas] = '+';
        Campo[LIN - 1][colunas] = '+';
    }
}

/**
@brief Apagar a Matriz anterior atraves do comando system(...). Pausar o processamento
atraves da funcao sleep(...). Desenhar a matriz do modo como ela esta. Ou seja, imprimi-la com as ultimas mudancas.
@param Campo[][COL] Unica matriz que seraimpressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@return Nenhum
*/
void AtualizaCampo(char Campo[][COL]){

    int Linhas,Colunas; /* Variaveis auxiliares para caminhar pela da matriz */

    /* system ("cls || clear") serve para Limpar a tela. Funcao sleep(TIME) para aguardar instantes de mostrar novamente */
    system("cls || clear"); /* "cls || clear". Pode receber "cls" ou "clear" - Vai depender do sistema operacional. No Windows "cls" no Linux "clear" */
    sleep(TIME); /* TIME - Pode trabalhar em milisegundos ou segundos. No Windows milisegundos no Linux em segundos. Esta definida como constante no defines.
    Logo acima da primeira funcao. =D*/

    /* Loop para imprimir a matriz Campo toda. Simplesmente andar do 0 ao 22, 74 vezes =D */
    for (Linhas=0;Linhas<LIN;Linhas++){
        for(Colunas=0;Colunas<COL;Colunas++)
            printf("%c",Campo[Linhas][Colunas]);
        printf("\n");
    }
}

/**
@brief Solicita ao usuario a posicao das divisorias e das portas, tanto na parte de cima como na parte de baixo
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param PosicaoPortas[][MAXDIV + 1] Matriz que armazena a posicao das portas
@param NumPortasComodo[] Vetor que armazena o numero de portas em cada comodo. Necessaria para futuramente servir como
delimitar do numero de posicoes que devemos acessar, ou seja, para nao correr o risco de acessarmos um lugar na memoria
que nao tenho sido armazenada nada (contenha, talvez lixo de memoria) ou ate um local que nao existe
@param PosicaoDiv[][MAXDIV] Matriz que armazena a posicao das divisorias
@return Nenhum
*/
void ColunasePortas(char Campo[][COL], int PosicaoPortas[][MAXDIV + 1], int *NumPortasComodo, int PosicaoDiv[][MAXDIV]){
    int UltimaDivisoria, Linhas, Partes, Divisoria, Paredes, InicioDivisoria = 1, Porta, LinhaDaPorta = CS;
    char TituloComodo[2][10] = {"SUPERIOR", "INFERIOR"};

    for (Partes = 0; Partes < 2; Partes++) {

        system("cls || clear"); /* Funcao para Limpar a tela. Pode receber "cls" ou "clear".
        Vai depender do sistema operacional. No Windows "cls" no Linux "clear" */
        UltimaDivisoria = 0; // Começa sem divisórias
        Paredes = 1; // Inicia a contagem de paredes/divisórias
        NumPortasComodo[Partes] = 1; // Inicia a contagem de portas para cada parte

        Divisoria = 0; /* Pra poder entrar no loop */

        while ((COL - 1 - UltimaDivisoria > 8) && (Divisoria != -1)){
            // Calcula a posição da próxima divisória de forma aleatória
            // Garante que a divisória esteja pelo menos a 4 posições de distância da última
            Divisoria = UltimaDivisoria + 4 + 1 + rand() % (COL - 3 - UltimaDivisoria + 4);

            while (((Divisoria < UltimaDivisoria +5) && (Divisoria != -1)) || (Divisoria > COL -4)){
                printf("ERRO! A posicao da divisoria deve estar entre %d e %d (Digite -1 para sair):",UltimaDivisoria +4,COL -3);
                Divisoria = UltimaDivisoria + 4 + 1 + rand() % (COL - 3 - UltimaDivisoria + 4);
            }

            if (Divisoria != -1){

                printf("Informe a posicao da porta no %do comodo (1-Esq; 2-Meio; 3-Dir):",NumPortasComodo[Partes]);
                Porta = 1 + rand() % 3;
                while ((Porta != 1) && (Porta != 2) && (Porta != 3)){/* Validando o valor da porta. Deve ser 1 ou 2 ou 3 */
                    printf("ERRO! Informe a posicao da porta no %do comodo (1-Esq; 2-Meio; 3-Dir):",NumPortasComodo[Partes]);
                    Porta = 1 + rand() % 3;
                }

                printf("\n"); /* Para melhorar a visualizacao dos dados de saida. Quebra linha apos receber a posicao da porta */

                /* Verifica onde a porta vai ficar, imprime um espaco em branco ' ' no lugar dela. Por ultimo vai salvar a posicao dela numa matriz
                A matriz PosicaoPortas sera identificada pelo numero do Partes, e o Numero da Porta */
                if (Porta == 1){
                    /* A posicao da porta, caso seja na esquerda, sera um espaco a mais que a Ultima Divisoria informada. */
                    PosicaoPortas[Partes][NumPortasComodo[Partes] -1] = UltimaDivisoria + 1;
                    Campo[LinhaDaPorta][UltimaDivisoria + 1] = ' ';
                }else if (Porta == 2){
                    /* A posicao da porta, caso seja no meio, sera a uma distancia de (Divisoria - UltimaDivisoria)/2 a frente da ultima divisoria informada */
                    PosicaoPortas[Partes][NumPortasComodo[Partes] -1] = (Divisoria - UltimaDivisoria)/2 + UltimaDivisoria;
                    Campo[LinhaDaPorta][(Divisoria - UltimaDivisoria)/2 + UltimaDivisoria] = ' ';
                }else{
                    /* A posicao da porta, caso seja na direita, sera dois espaco a menos da Atual divisoria informada. */
                    /* Obs.: Criamos no inicio um vetor de 75 colunas.
                    Para o compilador as posicoes variam de 0 a 74. Para o usuario de 1 a 75. O fato da diferente ser de 2 e nao de 1, se deve a isso */
                    PosicaoPortas[Partes][NumPortasComodo[Partes] -1] = Divisoria - 2;
                    Campo[LinhaDaPorta][Divisoria - 2] = ' ';
                }

                /* Como o numero de Quartos/Portas eh um a mais que o numero de divisorias, ja podemos somar mais um comodo */
                NumPortasComodo[Partes]++;

                /* Desenhando as divisorias e preenchendo com as extremidades com o simbolo (+) */

                /* Colocando o caracter '|' em todas as linhas na posicao da divisoria que o usuario escolheu */
                for (Linhas=InicioDivisoria;Linhas < InicioDivisoria + 10;Linhas++)
                    Campo[Linhas][Divisoria - 1] = '|';

                /* Colocando o caracter '+' nas extremidades da parede */
                for (Linhas=InicioDivisoria -1;Linhas < InicioDivisoria + 11;Linhas = Linhas + 10)
                    Campo[Linhas][Divisoria - 1] = '+';

                /* Apos validar a Divisoria e caso esta seja diferente de -1. Vamos salvar num vetor */
                PosicaoDiv[Partes][Paredes] = Divisoria - 1;

                /* Somando a quantidade de Paredes inserida pelo usuario. Atribuindo a variavel UltimaDivisoria, o valor da ultima divisoria digitada */
                Paredes++;
                UltimaDivisoria = Divisoria - 1;

            }
            /* Antes de sair do loop a parede da esquerda sera considerada como se fosse a ultima divisoria, pra poder servir como delimitadora */
            PosicaoDiv[Partes][Paredes] = COL -1;
        } /* END WHILE */
        
        /* Apos sair do loop, ou pelo fato de nao caber mais divisiorias ou pelo fato de o ususario ter digitado -1
        temos que pedir a posicao da porta do ultimo comodo que foi formado entre a Ultima divisoria que o usuario
        digitou e a parade final da matriz. A unica diferente dese loop para o outro, e que o parametreo Divisoria, vai ter o valor de COL.
        Pois o ultimo comodo vai estar entre a Ultima Divisoria e a Ultima parede da matriz */

        // Escolhe a posição da porta aleatoriamente dentro do comodo criado
        // Assumindo 1: Esquerda, 2: Meio, 3: Direita
        Porta = 1 + rand() % 3;

        /* Verifica onde a porta vai ficar, imprime um espaco em branco ' ' no lugar dela. Por ultimo vai salvar a posicao dela numa matriz
        A matriz PosicaoPortas sera identificada pelo numero da Parte, e o Numero da Porta */
        if (Porta == 1){
            /* A posicao da porta, caso seja na esquerda, sera um espaco a mais que a Ultima Divisoria informada. */
            PosicaoPortas[Partes][NumPortasComodo[Partes] -1] = UltimaDivisoria + 1;
            Campo[LinhaDaPorta][UltimaDivisoria + 1] = ' ';
        }else if (Porta == 2){
            /* A posicao da porta, caso seja no meio, sera a uma distancia de (COL - UltimaDivisoria)/2 a frente da ultima divisoria informada */
            PosicaoPortas[Partes][NumPortasComodo[Partes] -1] = (COL - UltimaDivisoria)/2 + UltimaDivisoria;
            Campo[LinhaDaPorta][(COL - UltimaDivisoria)/2 + UltimaDivisoria] = ' ';
        }else{
            /* A posicao da porta, caso seja na direita, sera dois espaco a menos da parede da matriz. */
            /* Obs.: Criamos no inicio um vetor de 75 colunas.
            Para o compilador as posicoes variam de 0 a 74. Para o usuario de 1 a 75. O fato da diferente ser de 2 e nao de 1, se deve a isso */
            PosicaoPortas[Partes][NumPortasComodo[Partes] -1] = COL - 2;
            Campo[LinhaDaPorta][COL - 2] = ' ';
        }

        /* Como o loop serve para pedir as divisorias e portas da parte 0 e da parte 1, temos que fazer algumas alteracoes */
        InicioDivisoria = InicioDivisoria + 12; /* Vai come�car abaixo do corredor principal */
        LinhaDaPorta = CI; /* Vai ficar em cima do corredor inferior */

        AtualizaCampo(Campo);

        getchar();/* Funcao que limpa o buffer e aguarda um tecla a ser lida. Utilizada para segurar a tela
        antes de passar pra outra etapa do programa. Ou seja, nada da atual tela vai sumir ate o que usuario tecle ENTER */
        printf("Tecle ENTER para continuar...");
        getchar();
    } /* End FOR */

}

/**
@brief Solicita do usuario a quantidade de frutas dentro de cada Quarto e a posicao delas. Eh claro, todos esses dados validados!
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param PosicaoPortas[][MAXDIV + 1] Matriz que armazena a posicao das portas
@param NumPortasComodo[] Vetor que armazena o numero de portas em cada comodo. Necessaria para futuramente servir como
delimitar do numero de posicoes que devemos acessar, ou seja, para nao correr o risco de acessarmos um lugar na memoria
que nao tenho sido armazenada nada (contenha, talvez lixo de memoria) ou ate um local que nao existe
@param PosicaoDiv[][MAXDIV] Matriz que armazena a posicao das divisorias
@param PosFrutaX[][50] Armazena a posicao da Fruta no eixo X, ou seja, a coluna da fruta
@param PosFrutaY[][50] Armazena a posicao da Fruta no eixo Y, ou seja, a linha da fruta
@param NumFrutas[][MAXDIV + 1] Armazena o numero de frutas em cada quarto.
@return Nenhum
*/

void ColocaFrutas(char Campo[][COL],int PosicaoDiv[][MAXDIV],int PosFrutaX[][50],int PosFrutaY[][50],int NumFrutas[][MAXDIV + 1],int *NumPortasComodo){

    int LinhaBase=1,Partes,Quartos,Frutas,ContDiv=0;
    char TituloComodo[2][10]={"SUPERIOR","INFERIOR"};

    /*
    Explicacao das variaveis:
    - LinhaBase: Variavel que representa a menor posicao de linha que uma fruta pode ficar. Ou seja, deve estar acima da posicao 1
    que por sua vez e onde esta a parede superior que delita o Campo, quando o loop for receber as frutas da parte inferior
    esta variavel recebe + 12 que sera por sua vez o novo parametro da parte inferior, ou seja, as frutas tem que estar numa posicao
    maior que LinhaBase
    - Partes: Variavel de controle do Loop Maior For, varia de Zero(0) a 1. Duas partes, parte inferior e parte superior.
    - Quartos: Variavel de Controle do Loop For, que ira solicitar a quantidade de frutas de cada Quarto
    - Frutas: Variavel de controle do Loop For, no qual sera solicitado do usuario a posicao das frutas. Ex.: fruta 1, fruta 2, fruta 3...
    - ContDiv: Variavel auxiliar que ira contar o numero de divisorias. Ira acessar as posicoes das "linhas" nas Matrizes:
    PosFrutaX[][50], PosFrutaY[][50]. Ex.: PosFrutaX[ContDiv][aux], PosFrutaX[ContDiv][aux]
    - Titulo[][] Utilizada para diferenciar a parte de cima da parte de baixo. Como as duas tem nomes diferentes, esta eh
    uma das forma de fazer essa alteracao. Acessaremos um dos titulos em Titulo[0] e o outro em Titulo[1] */

    /* Loop de repeticao FOR que ira ser repetido duas vezes. Uma para a Parte Superior e outra para a Inferior */
    for (Partes=0;Partes<2;Partes++){

        system("cls || clear"); /* Funcao para Limpar a tela. Pode receber "cls" ou "clear".
        Vai depender do sistema operacional. No Windows "cls" no Linux "clear" */

        /* O Titulo tem uma variavel que pode ser "INFERIOR" ou "SUPERIOR" */
        printf("*** PARTE %s ***\n",TituloComodo[Partes]);

        /* Loop de repiticao que ira de quarto por quarto/comodo solicitando o numero de frutas dentro de cada um */
        for (Quartos=0;Quartos<NumPortasComodo[Partes];Quartos++){
            
            NumFrutas[Partes][Quartos] = 1 + rand() % 5; // Entre 1 e 5 frutas

            /* Loop de validacao
            Testes:
            - O quantidade total de frutas deve ser menor que as colunas -1 X linhas -1 do atual comodo.
            - A quantidade de frutas deve ser maior que Zero(0)*/
            while ((NumFrutas[Partes][Quartos] < 0) || (NumFrutas[Partes][Quartos] > ( 9* (PosicaoDiv[Partes][Quartos + 1] - PosicaoDiv[Partes][Quartos] -1)))){
                NumFrutas[Partes][Quartos] = 1 + rand() % 5; // Entre 1 e 5 frutas
            }
            printf("\n");

            /* Loop para receber de cada Fruta a posicao x (coluna) e a posicao y (linha) */
            for (Frutas=0;Frutas<NumFrutas[Partes][Quartos];Frutas++){

                PosFrutaY[ContDiv][Frutas] = LinhaBase + 1 + rand() % (LinhaBase + 10 - LinhaBase);

                /* Como as linhas sao fixas, serao Sempre maior que a LinhaBase e Menor ou Igaul que LinhaBase + 9 */
                while ((PosFrutaY[ContDiv][Frutas] < LinhaBase + 1) || (PosFrutaY[ContDiv][Frutas] > LinhaBase + 9)){
                    PosFrutaY[ContDiv][Frutas] = LinhaBase + 1 + rand() % (LinhaBase + 10 - LinhaBase);
                }
                PosFrutaX[ContDiv][Frutas] = PosicaoDiv[Partes][Quartos] + 1 + 1 + rand() % (PosicaoDiv[Partes][Quartos + 1] +1 - PosicaoDiv[Partes][Quartos] + 1);

                /* As colunas nao sao fixas, logo, para validar as entradas usaremos a variavel que guardou a posicao das divisorias
                Olharemos pra uma divisoria X e seguinte divisoria. */
                while ((PosFrutaX[ContDiv][Frutas] < PosicaoDiv[Partes][Quartos] + 2) || (PosFrutaX[ContDiv][Frutas] > PosicaoDiv[Partes][Quartos + 1])){
                    PosFrutaX[ContDiv][Frutas] = PosicaoDiv[Partes][Quartos] + 1 + 1 + rand() % (PosicaoDiv[Partes][Quartos + 1] +1 - PosicaoDiv[Partes][Quartos] + 1);

                }

                /* Vai desenhar no Campo a fruta */
                Campo[PosFrutaY[ContDiv][Frutas] -1][PosFrutaX[ContDiv][Frutas] -1] = FRUTA;
            }
            /* Somando um no contador que passa pela Matriz PosFrutaY e PosFrutaX */
            ContDiv++;
        }
        AtualizaCampo(Campo);
        getchar();
        printf("Tecle ENTER para continuar...");
        getchar();

        /* Na parte de cima e na parte de baixo as referencias sao diferentes, Logo, somaremos +12 para a refencia das linhas trabalhar
        corretamente pra parte inferior */
        LinhaBase = LinhaBase + 12;
    }
}

/**
@brief Faz o Pac-Man andar uma posicao acima da atual
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param LinhaPac A posicao da linha atual do Pac-Man
@param ColunaPac A posicao da coluna atual do Pac-Man
@param ContPassos O contador de passos do PAc-man
@return Nenhum
*/
void AndaCima(char Campo[][COL],int *LinhaPac, int *ColunaPac, int *ContPassos){

    Campo[*LinhaPac][*ColunaPac] = ' '; /* Atribui Um espaco em branco pra atual posicao do Pac-man */
    (*LinhaPac)--; /* Move a linha atual dele pra cima */
    Campo[*LinhaPac][*ColunaPac] = PACMAN; /* Imprimi na atual posicao o PACMAN */
    (*ContPassos)++; /* Soma +1 na quantidade de passos dada */
    AtualizaCampo(Campo);

    /* Os comentarios dessa funcao sao semelhantes aos das tres funcoes seguintes */

}

/**
@brief Faz o Pac-Man andar uma posicao pra baixo da atual
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param LinhaPac A posicao da linha atual do Pac-Man
@param ColunaPac A posicao da coluna atual do Pac-Man
@param ContPassos O contador de passos do PAc-man
@return Nenhum
*/
void AndaBaixo(char Campo[][COL],int *LinhaPac, int *ColunaPac, int *ContPassos){

    Campo[*LinhaPac][*ColunaPac] = ' ';
    (*LinhaPac)++;
    Campo[*LinhaPac][*ColunaPac] = PACMAN;
    (*ContPassos)++;
    AtualizaCampo(Campo);

}

/**
@brief Faz o Pac-Man andar uma posicao pra direita da atual
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param LinhaPac A posicao da linha atual do Pac-Man
@param ColunaPac A posicao da coluna atual do Pac-Man
@param ContPassos O contador de passos do PAc-man
@return Nenhum
*/
void AndaDireita(char Campo[][COL],int *LinhaPac, int *ColunaPac, int *ContPassos){

    Campo[*LinhaPac][*ColunaPac] = ' ';
    (*ColunaPac)++;
    Campo[*LinhaPac][*ColunaPac] = PACMAN;
    (*ContPassos)++;
    AtualizaCampo(Campo);
}

/**
@brief Faz o Pac-Man andar uma posicao pra esquerda da atual
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param LinhaPac A posicao da linha atual do Pac-Man
@param ColunaPac A posicao da coluna atual do Pac-Man
@param ContPassos O contador de passos do PAc-man
@return Nenhum
*/
void AndaEsquerda(char Campo[][COL],int *LinhaPac, int *ColunaPac, int *ContPassos){

    Campo[*LinhaPac][*ColunaPac] = ' ';
    (*ColunaPac)--;
    Campo[*LinhaPac][*ColunaPac] = PACMAN;
    (*ContPassos)++;
    AtualizaCampo(Campo);
}

/**
@brief Caminha da Porta ate a fruta identificada como mais proxima, e caso haja mais de uma vai atras sempre da mais perto
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param LinhaPac A posicao da linha atual do Pac-Man
@param ColunaPac A posicao da coluna atual do Pac-Man
@param ContPassos O contador de passos do PAc-man
@param PosFrutaX[][50] Armazena a posicao da Fruta no eixo X, ou seja, a coluna da fruta
@param PosFrutaY[][50] Armazena a posicao da Fruta no eixo Y, ou seja, a linha da fruta
@param frutaperto Identifica pra funcao qual eh a fruta mais proxima, de acordo com a ordem de entrada. Ex.: fruta 1, ou fruta 10
@param quartoperto Identifica pra funcao qual a posicao do quarto do qual esta sendo analizado as frutas
@return Nenhum
*/
void PegaFruta(char Campo[][75], int frutaperto, int quartoperto, int *ColunaPac, int *LinhaPac,int PosFrutaY[][50], int PosFrutaX[][50],int *ContPassos){

    /* Andando na horizontal. Tendo apenas o cuidado de analisar se eh pra direita ou pra esquerda.
    Enquanto nao chegar na coluna desejada o Pac-Man anda! */
    if (PosFrutaX[quartoperto][frutaperto] < *ColunaPac + 1)
        while (PosFrutaX[quartoperto][frutaperto] < *ColunaPac + 1)
            AndaEsquerda(Campo,LinhaPac,ColunaPac,ContPassos);
    else
        while (PosFrutaX[quartoperto][frutaperto] > *ColunaPac + 1)
            AndaDireita(Campo,LinhaPac,ColunaPac,ContPassos);


    /* Andando na certical. Tendo apenas o cuidado de analisar se eh pra cima ou pra baixo.
    Enquanto nao chegar na linha desejada o Pac-Man anda! */
    if (PosFrutaY[quartoperto][frutaperto] < *LinhaPac + 1)
        while (PosFrutaY[quartoperto][frutaperto] < *LinhaPac + 1)
            AndaCima(Campo,LinhaPac,ColunaPac,ContPassos);

    else
        while (PosFrutaY[quartoperto][frutaperto] > *LinhaPac + 1)
            AndaBaixo(Campo,LinhaPac,ColunaPac,ContPassos);
}

/**
@brief Faz uma analise das distancias das frutas em relacao ao Pac-Man e encontra a fruta mais proxima dele.
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param LinhaPac A posicao da linha atual do Pac-Man
@param ColunaPac A posicao da coluna atual do Pac-Man
@param ContPassos O contador de passos do PAc-man
@param PosFrutaX[][50] Armazena a posicao da Fruta no eixo X, ou seja, a coluna da fruta
@param PosFrutaY[][50] Armazena a posicao da Fruta no eixo Y, ou seja, a linha da fruta
@param NumFrutas[][MAXDIV + 1] Armazena o numero de frutas em cada quarto.
@param parte Informa o numero correspondente a parte que esta sendo analizada, parte inferior ou superior
@param quartos Identifica quais os quartos estao sendo analisados, os quartos de cima ou de baixo.
@param atualquarto Identifica o atual quarto em analise
@return Nenhum
*/
void EncontraFrutaProxima(char Campo[][75], int PosFrutaY[][50], int PosFrutaX[][50], int NumFrutas[][MAXDIV + 1], int *LinhaPac, int *ColunaPac, int parte, int quartos, int atualquarto,int *ContPassos){

    int aux=0,aux2,menordistancia,quartoperto,frutaperto,posicaox,posicaoy;

    quartoperto = quartos; /* Identifica quais os quartos estao sendo analisados, os quartos de cima ou de baixo. */

    /* Loop While que passa analizando todas as frutas */
    while (aux < NumFrutas[parte][atualquarto]){
            menordistancia = 100; /* Colocando um valor um pouco alto pra servir como criterio de comparacao. Nunhuma distancia sera maior que essa. */

        /* Loop de repeticao FOR. Passa por todas as Frutas analizando as distancia delas para o Pac-Man */
        for (aux2=0;aux2<NumFrutas[parte][atualquarto];aux2++)

            /* So precisa analizar a fruta caso ela ainda exista, ou seja, caso nao tenho sido comida pelo Pac-Man */
            if (Campo[PosFrutaY[quartos][aux2]-1][PosFrutaX[quartos][aux2]-1] == FRUTA){

                /* Uma especie de modulo. Pra deixar os valores positivos */
                if (PosFrutaY[quartos][aux2] - (*LinhaPac +1) < 0)
                    posicaoy = -(PosFrutaY[quartos][aux2] - (*LinhaPac +1));
                else
                    posicaoy = PosFrutaY[quartos][aux2] - (*LinhaPac +1);

                /* Uma especie de modulo. Pra deixar os valores positivos */
                if (PosFrutaX[quartos][aux2] - (*ColunaPac +1) < 0)
                    posicaox = -(PosFrutaX[quartos][aux2] - (*ColunaPac +1));
                else
                    posicaox = PosFrutaX[quartos][aux2] - (*ColunaPac +1);

                /* Verificando se distancia da Fruta analizada eh menor que a da menor distancia ja marcada */
                if ( posicaoy + posicaox < menordistancia){

                    menordistancia = posicaoy + posicaox;
                    frutaperto = aux2; /* Identifica a fruta */
                }
            }

        PegaFruta(Campo,frutaperto,quartoperto,ColunaPac,LinhaPac,PosFrutaY,PosFrutaX,ContPassos);
        aux++; /* Variavel de controle do Loop While. Deve ser incrementada pra nao dar loop infinito */
    }
}

/**
@brief Faz o Pac-Man andar pelo corredor e entrar ate a porta de cada Quarto/Comodo
@param Campo[][COL] Unica matriz que sera impressa ao usuario do programa. Contem as paredes,
as portas, divisao dos Partes, posicao das frutas e posicao do Pac-Man
@param LinhaPac A posicao da linha atual do Pac-Man
@param ColunaPac A posicao da coluna atual do Pac-Man
@param ContPassos O contador de passos do PAc-man
@param PosFrutaX[][50] Armazena a posicao da Fruta no eixo X, ou seja, a coluna da fruta
@param PosFrutaY[][50] Armazena a posicao da Fruta no eixo Y, ou seja, a linha da fruta
@param NumFrutas[][MAXDIV + 1] Armazena o numero de frutas em cada quarto.
@param PosicaoPortas[][MAXDIV + 1] Matriz que armazena a posicao das portas
@param NumPortasComodo[] Vetor que armazena o numero de portas em cada comodo. Necessaria para futuramente servir como
delimitar do numero de posicoes que devemos acessar, ou seja, para nao correr o risco de acessarmos um lugar na memoria
que nao tenho sido armazenada nada (contenha, talvez lixo de memoria) ou ate um local que nao existe
@return Nenhum
*/
void EntraSaiPorta(char Campo[][COL], int PosicaoPortas[][MAXDIV + 1], int *NumPortasComodo, int *LinhaPac, int *ColunaPac, int *ContPassos, int NumFrutas[][MAXDIV + 1], int PosFrutaY[][50], int PosFrutaX[][50]){

    int aux,aux2=0,aux3=0;
    int quartossuperior = 0; /* A contagem dos quartos superior se inicia no Zero(0)*/
    int quartosinferior = NumPortasComodo[0]; /* A contagem dos quartos inferiores se inicia apos a dos quartos superiores */

    for(aux=0;aux<NumPortasComodo[0] + NumPortasComodo[1];aux++){

        /* Verifica quais das portas (inferior ou superior) vem primeiro.
        Se as da parte superior vierem primeiro entra no IF Senao entre no ELSE */
        if(((PosicaoPortas[0][aux2] <= PosicaoPortas[1][aux3]) && (aux2 < NumPortasComodo[0])) ||(aux3 == NumPortasComodo[1])) {
            while(*ColunaPac < PosicaoPortas[0][aux2])
                AndaDireita(Campo,LinhaPac,ColunaPac,ContPassos); /* Anda Na direcao oposta ao lado que inicio ate chegar numa porta */

            AndaCima(Campo,LinhaPac,ColunaPac,ContPassos); /* Chega na mesma linha da porta, porem sem entrar */


            if (NumFrutas[0][aux2] > 0){ /* Se houver frutas */
                AndaCima(Campo,LinhaPac,ColunaPac,ContPassos); /* Sobe mais uma posicao */

                /* Encontra a fruta mais proxima */
                EncontraFrutaProxima(Campo,PosFrutaY,PosFrutaX,NumFrutas,LinhaPac,ColunaPac,0,quartossuperior,aux2,ContPassos);

                /* Depois de pegar todas as frutas, eh hora de retornar pra porta.*/
                if (PosicaoPortas[0][aux2] < *ColunaPac)
                    while (PosicaoPortas[0][aux2] < *ColunaPac)
                        AndaEsquerda(Campo,LinhaPac,ColunaPac,ContPassos);

                else
                    while (PosicaoPortas[0][aux2] > *ColunaPac)
                        AndaDireita(Campo,LinhaPac,ColunaPac,ContPassos);

                while (9 > *LinhaPac)
                        AndaBaixo(Campo,LinhaPac,ColunaPac,ContPassos);

                AndaBaixo(Campo,LinhaPac,ColunaPac,ContPassos);
            }

            AndaBaixo(Campo,LinhaPac,ColunaPac,ContPassos);

            aux2++; /* Varia as portas da parte superior, ou seja, se por acaso a porta 1 for a mais perta do Pac-Man
            quando o loop retornar vai analizar a porta 2 da parte de cima */
            quartossuperior++;

        /* Caso a primeira seja da parte inferioro */
        }else{

            while(*ColunaPac < PosicaoPortas[1][aux3])
                AndaDireita(Campo,LinhaPac,ColunaPac,ContPassos);

            AndaBaixo(Campo,LinhaPac,ColunaPac,ContPassos);

            if (NumFrutas[1][aux3] > 0){
                AndaBaixo(Campo,LinhaPac,ColunaPac,ContPassos);

                EncontraFrutaProxima(Campo,PosFrutaY,PosFrutaX,NumFrutas,LinhaPac,ColunaPac,1,quartosinferior,aux3,ContPassos);

                if (PosicaoPortas[1][aux3] < *ColunaPac)
                    while (PosicaoPortas[1][aux3] < *ColunaPac)
                        AndaEsquerda(Campo,LinhaPac,ColunaPac,ContPassos);

                else
                    while (PosicaoPortas[1][aux3] > *ColunaPac)
                        AndaDireita(Campo,LinhaPac,ColunaPac,ContPassos);

                while (13 < *LinhaPac)
                        AndaCima(Campo,LinhaPac,ColunaPac,ContPassos);

                AndaCima(Campo,LinhaPac,ColunaPac,ContPassos);
            }

            AndaCima(Campo,LinhaPac,ColunaPac,ContPassos);

            aux3++;
            quartosinferior++;
        }
    }

    while(*ColunaPac < COL - 1)
        AndaDireita(Campo,LinhaPac,ColunaPac,ContPassos); /* Terminadas as portas, caminhar ate a parede oposta da que ele(PAC-MAN) saiu */
}

int main(){

    char Campo[LIN][COL];
    int PosicaoPortas[2][MAXDIV + 1],NumPortasComodo[2],PosicaoDiv[2][MAXDIV];
    int PosFrutaX[2*(MAXDIV + 1)][50],PosFrutaY[2*(MAXDIV + 1)][50],NumFrutas[2][MAXDIV + 1];
    int LinhaPac=CS+1,ColunaPac = 0,ContPassos=0;

    PosicaoDiv[0][0] = 0;
    PosicaoDiv[1][0] = 0;

    DesenhaCampo(Campo);

    AtualizaCampo(Campo);

    printf("Tecle ENTER para inserir as divisorias automaticamente...");
    getchar();

    ColunasePortas(Campo,PosicaoPortas,NumPortasComodo,PosicaoDiv);

    AtualizaCampo(Campo);

    ColocaFrutas(Campo,PosicaoDiv,PosFrutaX,PosFrutaY,NumFrutas,NumPortasComodo);

    EntraSaiPorta(Campo,PosicaoPortas,NumPortasComodo,&LinhaPac,&ColunaPac,&ContPassos,NumFrutas,PosFrutaY,PosFrutaX);

    printf("\nTotal de Passos: %d\n",ContPassos);
    getchar();
    getchar();
    return 0;
}
