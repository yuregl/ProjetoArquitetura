#include "LeitorDeArquivos.h"
#include "MensagemErro.h"
#include <string.h>
#include <stdlib.h>

//DEFINES
#define SIZE_REGISTRADORES 10
#define EXIT_PROGRAM 0
#define RET 0
#define ADD 1
#define COMP 2
#define ESCREVA 3
#define GETINT 4
#define JE 5
#define JG 6
#define JL 7
#define JUMP 8
#define LOAD 9
#define MOVE 10
#define PRINT 11
#define MUL 12
#define SUB 13
#define STORE 14
#define REGISTRADOR 'r'

// MACROS
#define CHAR_TO_INT(r) (r - '0');
#define SEARCH_SPACE(instrucao) strchr(instrucao, ' ');
#define STRING_TO_INT(r) ((int) atoi(r));

//VARIAVEIS GLOBAIS
int registrador[10] = {0,0,0,0,0,0,0,0,0,0};
int reg1, reg2, cmp = -2;
char *re1, *re2;
int ciclos = 0;

//FUNCOES

char *fetchInstrucao(int IR){
    return getInstrucao(IR, &ciclos);
}

void funcaoADD(char *instrucao){

    //BUSCANDO PRIMEIRO REGISTRADOR
    re1 = SEARCH_SPACE(instrucao);
    re1++;

    /*
        REG1 RECEBERA O VALOR DO REGISTRADOR
        ONDE O RESULTADO DA OPERACAO SERA
        SALVO
    */
    ++re1;
    reg1 = CHAR_TO_INT(*re1);

    //BURSCANDO SEGUNDO REGISTRADOR
    //OU ENDERECO DE MEMORIA
    re2 = strchr(instrucao,',');
    re2++;

    if (*re2 == REGISTRADOR){   /**CASO SEJAM 2 REGISTRADORES*/

        /*
            PEGANDO VALOR DO REGISTRADOR
            DEPOIS DA LETRA 'R' E CONVERTENDO
            PARA INTEIRO

            EX: R1 ==> (reg1 = 1)
        */

        ++re2;
        reg2 = CHAR_TO_INT(*re2);

        /*
            SOMANDO OS VALORES DOS REGISTRADORES E
            ARMAZENANDO O RESULTADO NO REG1
        */

        registrador[reg1] = registrador[reg1] + registrador[reg2];

    }else if(*re2 == 'c'){  /**CASO SEJA CONSTANTE*/

        //REG2 SERA UM INTEIRO COM VALOR CONSTANTE
        ++re2;
        reg2 = STRING_TO_INT(re2);

        registrador[reg1] = registrador[reg1] + reg2;

    } else if(*re2 - '0' >= 0 && *re2 - '0' <= 9){  /**CASO SEJA UM INTEIRO*/

        reg2 = CHAR_TO_INT(*re2);

        registrador[reg1] = registrador[reg1] + getPosicaoMemoria(reg2, &ciclos);

    }else{
        mostraErro(ERRO_FUNCAO_ADD);
    }
}

void funcaoCOMP(char *instrucao){

    re1 = SEARCH_SPACE(instrucao);
    re1++;

    re2 = strchr(instrucao, ',');
    re2++;

    /*
        VERIFICANDO SE AMBOS OS PARAMETROS
        SAO REGISTRADORES
    */

    if(*re1 == REGISTRADOR && *re2 == REGISTRADOR){
        re1++;
        reg1 = CHAR_TO_INT(*re1);

        re2++;
        reg2 = CHAR_TO_INT(*re2);

        if(registrador[reg1] > registrador[reg2])
            cmp = 1;
        else if(registrador[reg1] == registrador[reg2])
            cmp = 0;
        else
            cmp = -1;
    }else if(*re1 == REGISTRADOR && *re2 == 'c'){
        re1++;
        reg1 = CHAR_TO_INT(*re1);

        re2++;
        reg2 = CHAR_TO_INT(*re2);

        if(registrador[reg1] > reg2)
            cmp = 1;
        else if(registrador[reg1] == reg2)
            cmp = 0;
        else
            cmp = -1;

    }else{
        mostraErro(ERRO_FUNCAO_CMP);
    }

}

void funcaoGetInt(char *instrucao){
    re1 = strchr(instrucao, REGISTRADOR);
    re1++;
    reg1 = CHAR_TO_INT(*re1);

    scanf("%d", &registrador[reg1]);
}

void funcaoPrint(char *instrucao){

    re1 = SEARCH_SPACE(instrucao);
    re1++;

    printf("%s", re1);

}

void funcaoSub(char *instrucao){
    //BUSCANDO PRIMEIRO REGISTRADOR
    re1 = SEARCH_SPACE(instrucao);
    re1++;

    /*
        REG1 RECEBERA O VALOR DO REGISTRADOR
        ONDE O RESULTADO DA OPERACAO SERA
        SALVO
    */
    ++re1;
    reg1 = CHAR_TO_INT(*re1);

    //BURSCANDO SEGUNDO REGISTRADOR
    //OU ENDERECO DE MEMORIA
    re2 = strchr(instrucao,',');
    re2++;

    if (*re2 == REGISTRADOR){   /**CASO SEJAM 2 REGISTRADORES*/

        /*
            PEGANDO VALOR DO REGISTRADOR
            DEPOIS DA LETRA 'R' E CONVERTENDO
            PARA INTEIRO

            EX: R1 ==> (reg1 = 1)
        */

        ++re2;
        reg2 = CHAR_TO_INT(*re2);

        /*
            SOMANDO OS VALORES DOS REGISTRADORES E
            ARMAZENANDO O RESULTADO NO REG1
        */

        registrador[reg1] = registrador[reg1] - registrador[reg2];

    }else if(*re2 == 'c'){  /**CASO SEJA CONSTANTE*/

        //REG2 SERA UM INTEIRO COM VALOR CONSTANTE
        ++re2;
        reg2 = STRING_TO_INT(re2);

        registrador[reg1] = registrador[reg1] - reg2;

    } else if(*re2 - '0' >= 0 && *re2 - '0' <= 9){  /**CASO SEJA UM INTEIRO*/

        reg2 = CHAR_TO_INT(*re2);

        registrador[reg1] = registrador[reg1] - getPosicaoMemoria(reg2, &ciclos);

    }else{
        mostraErro(ERRO_FUNCAO_SUB);
    }

}

void funcaoStore(char *instrucao){

    re1 = SEARCH_SPACE(instrucao);
    re1++;

    re2 = strchr(instrucao, ',');
    re2++;

    if(*re1 == REGISTRADOR){

        re1++;
        reg1 = CHAR_TO_INT(*re1);

        reg2 = STRING_TO_INT(re2);

        if(reg2 <= (getSizeMemoria() - 1)  && reg2 >= 0){
            setPosicaoMemoria(reg2, registrador[reg1]);
        }

        else
            mostraErro(ERRO_STORE);

    }

}

void funcaoJG(char *instrucao, int *PC){

    if(cmp == 1){
        re1 = SEARCH_SPACE(instrucao);
        re1++;

        reg1 = STRING_TO_INT(re1);

        *PC = reg1;
    }


}

void funcaoJE(char *instrucao, int *PC){
    if(cmp == 0){
        re1 = SEARCH_SPACE(instrucao);
        re1++;

        reg1 = STRING_TO_INT(re1);

        *PC = reg1;
    }
}

void funcaoJL(char *instrucao, int *PC){
    if(cmp == -1){
        re1 = SEARCH_SPACE(instrucao);
        re1++;

        reg1 = STRING_TO_INT(re1);

        *PC = reg1;
    }
}

void funcaoJump(char *instrucao, int *PC){
    re1 = SEARCH_SPACE(instrucao);
    re1++;

    reg1 = STRING_TO_INT(re1);

    *PC = reg1;
}

void funcaoMult(char *instrucao){
    //BUSCANDO PRIMEIRO REGISTRADOR
    re1 = SEARCH_SPACE(instrucao);
    re1++;

    /*
        REG1 RECEBERA O VALOR DO REGISTRADOR
        ONDE O RESULTADO DA OPERACAO SERA
        SALVO
    */
    ++re1;
    reg1 = CHAR_TO_INT(*re1);

    //BURSCANDO SEGUNDO REGISTRADOR
    //OU ENDERECO DE MEMORIA
    re2 = strchr(instrucao,',');
    re2++;

    if (*re2 == REGISTRADOR){   /**CASO SEJAM 2 REGISTRADORES*/

        /*
            PEGANDO VALOR DO REGISTRADOR
            DEPOIS DA LETRA 'R' E CONVERTENDO
            PARA INTEIRO

            EX: R1 ==> (reg1 = 1)
        */

        ++re2;
        reg2 = CHAR_TO_INT(*re2);

        /*
            SOMANDO OS VALORES DOS REGISTRADORES E
            ARMAZENANDO O RESULTADO NO REG1
        */

        registrador[reg1] = registrador[reg1] * registrador[reg2];

    }else if(*re2 == 'c'){  /**CASO SEJA CONSTANTE*/

        //REG2 SERA UM INTEIRO COM VALOR CONSTANTE
        ++re2;
        reg2 = STRING_TO_INT(re2);

        registrador[reg1] = registrador[reg1] * reg2;

    } else if(*re2 - '0' >= 0 && *re2 - '0' <= 9){  /**CASO SEJA UMA POSICAO DE MEMORIA*/

        reg2 = CHAR_TO_INT(*re2);

        //printf("Fetch de Operando: %d\n\n", ciclos);
        registrador[reg1] = registrador[reg1] * getPosicaoMemoria(reg2, &ciclos);

    }else{
        mostraErro(ERRO_FUNCAO_MULT);
    }
}

void funcaoEscreva(char *instrucao){
    re1 = SEARCH_SPACE(instrucao);
    re1 += 2;

    reg1 = STRING_TO_INT(re1);

    printf("%d\n", registrador[reg1]);
}

void funcaoLoad(char *instrucao){
    re1 = strchr(instrucao, REGISTRADOR);
    re1++;

    reg1 = CHAR_TO_INT(*re1);

    re2 = strchr(instrucao, ',');
    re2++;

    // SE FOR LOAD DE CONSTANTE
    if(*re2 == 'c'){
        re2++;
        reg2 = STRING_TO_INT(re2);

        registrador[reg1] = reg2;

    }else{  // SE FOR LOAD DA MEMORIA

        re2 = strchr(instrucao, ',');
        re2++;

        reg2 = STRING_TO_INT(re2);

        if(reg2 >= 0 && reg2 <= getSizeMemoria()){
            registrador[reg1] = getPosicaoMemoria(reg2, &ciclos);
        }else{
            mostraErro(ERRO_FUNCAO_LOAD);
        }

    }



}

void funcaoMove(char *instrucao){

    re1 = SEARCH_SPACE(instrucao);
    re1++;

    re2 = strchr(instrucao, ',');
    re2++;

    if(*re1 == REGISTRADOR && *re2 == REGISTRADOR){

        re1++; re2++;

        reg1 = CHAR_TO_INT(*re1);
        reg2 = CHAR_TO_INT(*re2);

        /*
            Movendo valor do registrador 2 para
            registrador 1
        */
        registrador[reg1] = registrador[reg2];

    }else{
        mostraErro(ERRO_FUNCAO_MOVE);
    }

}

void funcaoRet(int *PC){

    *PC = -1;
    printf("\n\nNumero de ciclos necessarios: %d\n\n", ciclos);

}

int decodificaInstrucao(char *instrucao){

    switch(instrucao[0]){

    case 'A':
        return ADD;

    case 'C':
        if(instrucao[1] == 'O')
            return COMP;

    case 'E':
        return ESCREVA;

    case 'G':
        return GETINT;

    case 'J':

        if(instrucao[1] == 'G')
            return JG;
        else if(instrucao[1] == 'E')
            return JE;
        else if(instrucao[1] == 'L')
            return JL;
        else if(instrucao[1] == 'U')
            return JUMP;
        else
            mostraErro(ERRO_JUMP_J_EGL);
        break;

    case 'L':
        return LOAD;

    case 'M':
        if(instrucao[1] == 'O')
            return MOVE;
        else
            return MUL;

    case 'P':
        return PRINT;

    case 'R':
        return RET;

    case 'S':
        if(instrucao[1] == 'U')
            return SUB;
        else if(instrucao[1] == 'T')
            return STORE;
        else
            mostraErro(ERRO_SUB_STORE);

        break;
    }
    return -1;

}

void executaInstrucao(char *instrucao, int *PC, int execInstrucao){

    switch(execInstrucao){

    case RET:
        funcaoRet(PC);
        break;

    case ADD:
        funcaoADD(instrucao);
        break;

    case COMP:
        funcaoCOMP(instrucao);
        break;

    case ESCREVA:
        funcaoEscreva(instrucao);
        break;

    case GETINT:
        funcaoGetInt(instrucao);
        break;

    case JE:
        funcaoJE(instrucao, PC);
        break;

    case JG:
        funcaoJG(instrucao, PC);
        break;

    case JL:
        funcaoJL(instrucao, PC);
        break;

    case JUMP:
        funcaoJump(instrucao, PC);
        break;

    case LOAD:
        funcaoLoad(instrucao);
        break;

    case MOVE:
        funcaoMove(instrucao);
        break;

    case PRINT:
        funcaoPrint(instrucao);
        break;

    case MUL:
        funcaoMult(instrucao);
        break;

    case SUB:
        funcaoSub(instrucao);
        break;

    case STORE:
        funcaoStore(instrucao);
        break;

    }
    ciclos++;
    //printf("Executou instrucao: %d\n\n", ciclos);
}

