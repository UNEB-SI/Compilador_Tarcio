#include "analex.h"

//Varíavel Global para contar linha
int linhas = 1;

//Tabela de Cadeias Literais
char tabLiteral[TAM_TABLITERAL][TAM_TABLITERAL];

//Função para adcionar string à tabela literal
int addTabelaLiteral(char adcionar[]){
    int pos = 0;

    //Encontrar o ultimo espaço vazio
    while(strcmp(tabLiteral[pos], "") != 0){
        pos++;
    }

    strcpy(tabLiteral[pos], adcionar);

    return pos;
}

int comparaPalavraReservada(char palavra[]){
    int i = 0;

    while(strcmp(PAL_RESERV[i], "") > 0) //enquanto houver strings no vetor
    {
        if(strcmp(palavra, PAL_RESERV[i]) == 0){
            return i; //retorna o indice se tiver na tabela de palavras reservadas
        }
        i++;
    }

    return -1; //se não estiver na tabela de palabras reservadas
}

void erroAnalisadorLexico(char descricao[]){
    printf("ERRO: %s na Linha %d\n", descricao, linhas);
    system("pause");
    exit(1);
}


token analisadorLexico(FILE *arquivo){
    /*DECLARAÇÕES*/
    token tk;
    int estado = 0;
    char ch, armazenarCaracter;
    int contadorLexema, contadorDigito, contadorLT;
    char digitos[TAM_DIGITOS], lexema[TAM_LEXEMA], stringLT[TAM_STRINGLT];

    /*ASSUME ESSES VALORES DO TOKEN, CASO NÃO MUDAREM, DEU ERRO*/
    tk.categoria = FIMARQ;
    tk.cod = 0;

    /*PEGA O PRIMEIRO CARACTERE*/
    ch = fgetc(arquivo);

    /*CASO SEJA FIM DO ARQUIVO*/
    if(feof(arquivo)){
        return tk;
    }

    /*LOOP-PRINCIPAL*/
    while(1){
        switch(estado){
        case 0: /*ESTADO INICIAL*/
            /*CASO SEJA BRANCO, \T OU \N*/
            if((ch == ' ') || (ch == '\t') || (ch == '\n')){
                estado = 0;

                /*CONTAR LINHAS*/
                if(ch == '\n'){
                    linhas++;
                }
            }
            /*CASO SEJA DIGITO*/
            else if(isdigit(ch)){
                estado = 3;
                contadorDigito = 0; //inicia o contador em 0
                digitos[contadorDigito] = ch; //armazena o digito na posição 0
            }
            /*CASO SEJA LETRA*/
            else if(isalpha(ch)){
                estado = 1;
                contadorLexema = 0; //inicia o contador em 0
                lexema[contadorLexema] = ch; //armazena a letra na posição 0
            }
            /*CASO SEJA PT_VIRG*/
            else if(ch == ';'){
                estado = 8;
                tk.categoria = SN;
                tk.cod = PT_VIRG;
                return tk;
            }
            /*CASO SEJA VIRGULA*/
            else if(ch == ','){
                estado = 11;
                tk.categoria = SN;
                tk.cod = VIRG;
                return tk;
            }
            /*CASO SEJA SOMA*/
            else if(ch == '+'){
                estado = 12;
                tk.categoria = SN;
                tk.cod = SOMA;
                return tk;
            }
            /*CASO SEJA SUBTRAÇÃO*/
            else if(ch == '-'){
                estado = 13;
                tk.categoria = SN;
                tk.cod = SUB;
                return tk;
            }
            /*CASO SEJA MULTIPLICAÇÃO*/
            else if(ch == '*'){
                estado = 14;
                tk.categoria = SN;
                tk.cod = SOMA;
                return tk;
            }
            /*CASO SEJA BARRA*/
            else if(ch == '/'){ estado = 15; }
            /*CASO SEJA ABRE PARENTESIS*/
            else if(ch == '('){
                estado = 19;
                tk.categoria = SN;
                tk.cod = PARENTESIS_ABRE;
                return tk;
            }
            /*CASO SEJA FECHA PARENTESIS*/
            else if(ch == ')'){
                estado = 20;
                tk.categoria = SN;
                tk.cod = PARENTESIS_FECHA;
                return tk;
            }
            /*CASO SEJA ABRE CHAVES*/
            else if(ch == '{'){
                estado = 21;
                tk.categoria = SN;
                tk.cod = CHAVES_ABRE;
                return tk;
            }
            /*CASO SEJA FECHA CHAVES*/
            else if(ch == '}'){
                estado = 22;
                tk.categoria = SN;
                tk.cod = CHAVES_FECHA;
                return tk;
            }
            /*CASO SEJA & */
            else if(ch == '&'){ estado = 23; }
            /*CASO SEJA | */
            else if(ch == '|'){ estado = 25; }
            /*CASO SEJA > */
            else if(ch == '>'){ estado = 27; }
            /*CASO SEJA < */
            else if(ch == '<'){ estado = 30; }
            /*CASO SEJA ! */
            else if(ch == '!'){ estado = 33; }
            /*CASO SEJA = */
            else if(ch == '='){ estado = 36; }
            /*CASO SEJA ABRE ASPAS DUPLAs " */
            else if(ch == '"'){
                estado = 39;
                contadorLT = -1;
            }
            /*CASO SEJA ABRE ASPAS SIMPLES ' */
            else if(ch == '\''){ estado = 42; }
            else{
                /*CASO SEJA FIM DO ARQUIVO*/
                if(feof(arquivo)){
                    return tk;
                }
                erroAnalisadorLexico("Caractere não valido");

            }

            break; /*BREAK ESTADO 0*/

        case 1: /*ID OU PR*/

            /*CASO SEJA _ OU DIGITO OU LETRA CONTINUA NO ESTADO 1*/
            if((ch=='_') || (isdigit(ch)) || (isalpha(ch))){
                estado = 1;
                lexema[++contadorLexema] = ch;
            }else{
                estado = 2; //vai pro estado 2
                ungetc(ch, arquivo);
                lexema[++contadorLexema] = '\0'; //fecha a string
                int testar = comparaPalavraReservada(lexema); //compara com as palavras reservadas

                //SE ESTIVER NA LISTA DE TABELA RESERVADA
                if(testar>=0){
                    tk.cod = testar; //indice da palavra reservada
                    tk.categoria = PR;
                }else{
                    //Se não for palavra reservada, é ID
                    tk.categoria = ID;
                    strcpy(tk.lexema, lexema);
                }

                return tk;

            }

            break; /*BREAK ESTADO 1 */

        case 3: /*CT_I, CT_R*/

            /*SE FOR DIGITO */
            if(isdigit(ch)){
                estado = 3;
                digitos[++contadorDigito] = ch;

            /*SE FOR PONTO */
            }else if(ch == '.'){
                estado = 4;
                digitos[++contadorDigito] = ch;

            /* SE FOR OUTRO ENTÃO É CT_I* */
            }else{
                estado = 5;
                ungetc(ch, arquivo);
                digitos[++contadorDigito] = '\0'; //fecha a string
                tk.categoria = CT_I;
                tk.valor_int = atoi(digitos);
                return tk;
            }
            break; /*BREAK ESTADO 3 */

        case 4: /*SAIU DO 3 LENDO PONTO */

            /*SE LER DIGITO */
            if(isdigit(ch)){
                estado = 6;
                digitos[++contadorDigito] = ch;
            }
            break; /*BREAK ESTADO 4 */

        case 6: /* SAIU DO 4 E LEU DIGITO */

            /*SE LER DIGITO */
            if(isdigit(ch)){
                estado = 6;
                digitos[++contadorDigito] = ch;
            /*SE FOR OUTRO */
            }else{
                estado=7;
                ungetc(ch, arquivo);
                digitos[++contadorDigito]='\0'; //fecha a string
                tk.categoria=CT_R; tk.valor_real=atof(digitos);
                return tk;
            }
            break; /*BREAK ESTADO 6 */

        case 15: /* LEU BARRA / */

            /*SE LER ASTERISCO* */
            if(ch == '*'){
                estado = 16;
            /*SE LER OUTRO ENTÃO É DIVISÃO */
            }else{
                estado = 18;
                ungetc(ch, arquivo);
                tk.categoria = SN;
                tk.cod = DIV;
                return tk;
            }
            break; /*BREAK ESTADO 15 */

        /*COMENTÁRIO*/
        case 16: /* SAI DO 15 E LI ASTERISCO * */

            /*SE LER ASTERISCO* */
            if(ch == '*'){
                estado = 17;
            /*SE LER OUTRO CONTINUA NO ESTADO 16 */
            }else if(isprint(ch) || iscntrl(ch)){
                estado = 16;
                /*CONTA LINHA*/
                if(ch == '\n'){
                    linhas++;
                }
                /*SE FIM DO ARQUIVO*/
                if(feof(arquivo)){
                    return tk;
                }
            }else{
                erroAnalisadorLexico("Erro no comentário!");
            }

            //se não contar, adcionar else if contando linha

            break; /*BREAK ESTADO 16 */

        case 17: /* SAI DO 16 E LEU ASTERISCO * */
            /*SE LER ASTERISCO* */
            if(ch == '*'){
                estado = 17;
            /*SE LER BARRA VAI PRO ESTADO 0 */
            }else if(ch == '/'){
                estado = 0;
            /*SE LER OUTRO VOLTA PRO ANTERIOR 16 */
            }else if(isprint(ch) || iscntrl(ch)){
                estado = 16;
                if(ch == '\n'){
                    linhas++;
                }
            }else{
                erroAnalisadorLexico("Erro no comentário!");
            }
            break; /*BREAK ESTADO 17 */

        case 23: /* LEU & E COMERCIAL */
            /*SE LER OUTRO &*/
            if(ch == '&'){
                estado = 24;
                tk.categoria = SN;
                tk.cod = AND;
                return tk;
            }
            break; /*BREAK ESTADO 23 */

        case 25: /* LEU | BARRA VERTICAL */
            /*SE LER OUTRO |*/
            if(ch == '|'){
                estado = 26;
                tk.categoria = SN;
                tk.cod = OR;
                return tk;
            }else{
                erroAnalisadorLexico("Erro!");
            }
            break; /*BREAK ESTADO 25 */

        case 27: /* LEU > */
            /*SE LER = É MAIOR_IG*/
            if(ch == '='){
                estado = 29;
                tk.categoria = SN;
                tk.cod = MAIOR_IG;
                return tk;
            /*SE LER OUTRO* É MAIOR_Q */
            }else{
                estado = 28;
                tk.categoria = SN;
                tk.cod = MAIOR_Q;
                return tk;
            }
            break; /*BREAK ESTADO 27 */

        case 30: /* LEU < */
            /*SE LER = É MENOR_IG*/
            if(ch == '='){
                estado = 31;
                tk.categoria = SN;
                tk.cod = MENOR_IG;
                return tk;
            /*SE LER OUTRO* É MENOR_Q */
            }else{
                estado = 32;
                tk.categoria = SN;
                tk.cod = MENOR_Q;
                return tk;
            }
            break; /*BREAK ESTADO 30 */

        case 33: /* LEU ! */
            /*SE LER = É DIFERENTE*/
            if(ch == '='){
                estado = 34;
                tk.categoria = SN;
                tk.cod = DIFERENTE;
                return tk;
            /*SE LER OUTRO* É NEGAÇÃO */
            }else{
                estado = 35;
                tk.categoria = SN;
                tk.cod = NEGACAO;
                return tk;
            }
            break; /*BREAK ESTADO 33 */

        case 36: /* LEU = */
            /*SE LER = É COMPARAÇÃO*/
            if(ch == '='){
                estado = 37;
                tk.categoria = SN;
                tk.cod = COMPARA;
                return tk;
            /*SE LER OUTRO* É NEGAÇÃO */
            }else{
                estado = 38;
                tk.categoria = SN;
                tk.cod = ATRIB;
                return tk;
            }
            break; /*BREAK ESTADO 36 */

        case 39: /* LEU " */
            /*SE LER " É PQ FECHOU STRING */
            if(ch == '"'){
                estado = 40;
                stringLT[++contadorLT] = '\0'; //fecha a string
                int aux = addTabelaLiteral(stringLT);
                tk.categoria = CT_LT;
                tk.posicao = aux;
                return tk;
            /*SE LER OUTRO* CONTINUA NO 39 ATÉ */
            }else{
                estado = 39;
                stringLT[++contadorLT] = ch;
                if(ch == '\n'){
                    linhas++;
                }
                if(feof(arquivo)){
                    erroAnalisadorLexico("Cadeia mal formada");
                }
            }
            break; /*BREAK ESTADO 39 */

        case 42: /* LEU ' */
            /*SE LER \ */
            if (ch == '\\'){
                estado=45;
            /*SE LER OUTRO* */
            }else{
                estado=43;
                if(isprint(ch)){
                    armazenarCaracter = ch;
                }else{
                    erroAnalisadorLexico("Caracter mal formada");
                }
            }
            break; /*BREAK ESTADO 42 */

        case 43: /* LEU OUTRO* */
            /*SE LER ' */
            if (ch == '\''){
                estado=44;
                tk.categoria = CT_C;
                tk.valor_ch = armazenarCaracter;
                return tk;
            /*SE LER OUTRO* */
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 43 */

        case 45: /* LEU \ */
            /* SE LEU 0 */
            if(ch == '0'){
                estado = 47;
            /* SE LEU n */
            }else if(ch == 'n'){
                estado = 46;
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 45 */

        case 47: /* LEU 0 */
            /* SE LEU ' */
            if (ch == '\'') {
                estado = 49;
                tk.categoria = CT_C;
                tk.valor_ch = '\0';
                return tk;
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 47 */

        case 46: /* LEU n */
            /* SE LEU ' */
            if (ch == '\'') {
                //ao invés de retornar token, aumenta a linha
                //estado = 48;
                //tk.categoria = CT_C;
                //tk.valor_ch = '\n';
                estado = 0;
                linhas++;
            }else{
                erroAnalisadorLexico("Caracter mal formada");
            }
            break; /*BREAK ESTADO 46 */

        default:
            printf("Estado: %d ", estado);
            erroAnalisadorLexico("Estado nao encontrado.");

        }//fim-switch estado


        ch=fgetc(arquivo);
    }//fim-loop principal

}//fim-função analisador lexico


int main(){
    token tk;
    char nomeArquivo[100];
    FILE *arquivo;

    printf("\n\tDigite o nome do arquivo: "); scanf("%s", nomeArquivo);

    /*ABRE O ARQUIVO*/
	if ( (arquivo = fopen(nomeArquivo,"r")) != NULL ){
        printf("\n\tArquivo aberto com sucesso!\n");
    }
    else{
        printf("\n\tNão foi possível abrir o arquivo %s\n", nomeArquivo);
        system("pause");
        exit(1);
    }
    /*FIM-ABRE O ARQUIVO*/

    while(1){
        tk = analisadorLexico(arquivo);

        printf("\t\n<");

        if(tk.categoria == PR){
            printf("PR, %s> ", PAL_RESERV[tk.cod]);
        }
        if(tk.categoria == ID){
            printf("ID, %s> ", tk.lexema);
        }
        if(tk.categoria == SN){
            printf("SN, %s> ", TABELASINAIS[tk.cod]);
        }
        if(tk.categoria == CT_I){
            printf("CT_I, %d> ", tk.valor_int);
        }
        if(tk.categoria == CT_R){
            printf("CT_R, %8.2f> ", tk.valor_real);
        }
        if(tk.categoria == CT_C){
            CT_C: printf("CT_C, %c> ", tk.valor_ch);
        }
        if(tk.categoria == CT_LT){
            CT_LT: printf("CT_LT, %s> ", tabLiteral[tk.posicao]);
        }
        if(tk.categoria == FIMARQ){
            printf("FIMARQ, 0>");
        }

        if(tk.categoria==FIMARQ){
            break;
        }
    }

    fclose(arquivo);

    return 0;
}
