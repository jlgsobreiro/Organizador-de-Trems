#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "parser.h"
#define limite 10000

int main() {
int fora=0;
    while (1) {
        fflush(stdin);
        system("cls");
        int numero_de_vagas;
        int numero_trilhos;
        int numero_de_vagoes;
        int cheio = 0;
        char *string = calloc(limite, sizeof(char));
        char **lista;
        int etapas = 1;

        printf("\nDigite a sequencia de Vagoes (separados por \" \", \".\", \",\" ou \";\" ex:[ t a,a2 a3. a5;aa] ou digite \"-\" para valores aleatorios)\n\n");
        fgets(string, limite, stdin);

        //cria lista com nome dos vagoes
        formatador_de_texto(string, limite);
        numero_de_vagoes = contador_de_palavras(string, limite);
        separador_de_palavras(string, limite);
        lista = lista_de_palavras(string, limite);

        //pega valores do arquivo Valores.txt a partir de um indice aleatorio e coloca valores na lista
        if (strcmp(lista[0], "-") == 0) {
            printf("\nDigite quantos valores deseja\n");
            int v;
            char *string_aux = calloc(limite, sizeof(char));

            scanf("%d", &v);

            FILE *fp;
            fp = fopen("C:\\Users\\JLGS\\CLionProjects\\Organizador de Trems\\Valores.txt", "r");
            fscanf(fp, "%[^\0]", string_aux);
            fclose(fp);

            string = string_aux;
            numero_de_vagoes = v;

            formatador_de_texto(string, limite);
            int sep = contador_de_palavras(string, limite);
            separador_de_palavras(string, limite);
            lista = lista_de_palavras(string, limite);

            char *str_aux = calloc(limite, sizeof(char));
            int i;
            for (i = 0; i < numero_de_vagoes; ++i) {
                char *c = lista[rand() % sep + 1];
                strcat(c, " ");
                strcat(str_aux, c);
            }
            formatador_de_texto(str_aux, limite);
            separador_de_palavras(str_aux, limite);
            lista = lista_de_palavras(str_aux, limite);

        }
        //lista da posicao dos vagoes
        int *pInt = calloc(numero_de_vagoes, sizeof(int));

        printf("\nDigite o numero de trilhos de espera\n");
        scanf("%d", &numero_trilhos);

        printf("\nDigite a quantidade de vagas nos trilhos de espera\n");
        scanf("%d", &numero_de_vagas);

        int posicao = 1;
        int rel_trilhos = numero_trilhos;
        //compara as palavras para determinar qual a posicao dela na sequencia dos vagoes
        int j;
        for (j = 0; j < numero_de_vagoes; j++) {
            posicao = 1;
            int i;
            for (i = 0; i < numero_de_vagoes; ++i) {
                int tam = strlen(lista[j]) < strlen(lista[i]) ? strlen(lista[i]) : strlen(lista[j]);
                if (strncmp(lista[j], lista[i], tam) > 0) {
                    posicao++;
                } else if (strncmp(lista[j], lista[i], tam) == 0 && i > j) {
                    posicao++;
                }
            }
            pInt[j] = posicao;
        }

        //cria vagoes na fila de entrada
        Vagao *entrada = cria_vagoes_na_entrada(pInt, lista, numero_de_vagoes);
        Vagao *saida = NULL;
        exibe_vagoes(entrada);

        //cria pilhas de espera
        Vagao **espera = cria_trilhos_de_espera(numero_trilhos);

        int vagao_final = 1;
        int i, melhor_trilho = 0;

        while (conta_vagoes(entrada) > 0) {

            //verifica se a primeira posicao da fila de entrada é o primeiro vagao da fila de saida
            if (entrada->posicao == vagao_final) {
                etapas++;
                vagao_final++;
                if (saida == NULL) {
                    saida = entrada;
                    entrada = entrada->anterior;
                    saida->anterior = NULL;
                    printf("\nVagao movido da entrada para a saida\n");
                    exibe_vagoes(ultimo_vagao(saida));
                } else {
                    Vagao *aux;
                    aux = entrada;
                    entrada = entrada->anterior;
                    aux->anterior = NULL;
                    ultimo_vagao(saida)->anterior = aux;
                    printf("\nVagao movido da entrada para a saida\n");
                    exibe_vagoes(ultimo_vagao(saida));
                }

            }
            if (entrada == NULL) {
                printf("Todos os vagoes movidos da entrada para a saida");
                break;
            }

            melhor_trilho = escolhe_melhor_trilho(espera,numero_trilhos,numero_de_vagas,entrada);

            //verifica se a pilha de espera escolhida tem espaco para armazenar o vagao da entrada e move vagao da entrada para as pilhas de espera
            if (melhor_trilho != -1) {
                etapas++;
                cheio = 0;
                if (ultimo_vagao(espera[melhor_trilho]) == NULL) {

                    espera[melhor_trilho] = entrada;
                    entrada = entrada->anterior;
                    espera[melhor_trilho]->anterior = NULL;
                    printf("\nVagao movido da entrada para o trilho de espera N: %i\n", melhor_trilho + 1);
                    exibe_vagoes(ultimo_vagao(espera[melhor_trilho]));

                } else {

                    ultimo_vagao(espera[melhor_trilho])->anterior = entrada;
                    Vagao *aux;
                    aux = entrada->anterior;
                    entrada->anterior = NULL;
                    entrada = aux;

                    printf("\nVagao movido da entrada para o trilho de espera N: %i\n", melhor_trilho + 1);
                    exibe_vagoes(ultimo_vagao(espera[melhor_trilho]));

                }
            } else {
                etapas++;
                //expande o numero de pilhas de espera
                numero_trilhos++;
                espera = incrementa_trilho(espera, numero_trilhos);
                printf("\nCriado trilho de espera N: %i\n", numero_trilhos);
            }

        }

        //imprime pilhas de espera
        for (i = 0; i < numero_trilhos; i++) {
            printf("\n\nLinha espera N: %i Numero de Vagoes: %i\n", i + 1, conta_vagoes(espera[i]));
            exibe_vagoes(espera[i]);
        }

        //organizador
        int fim = 0;
        int tr = 0;
        for (;;) {

            //procura pilha de espera com o proximo valor da saida
            fim = retorna_trilho_do_vagao(numero_trilhos, vagao_final, espera);

            //termina quando nao ha valores compativeis dentro das pilhas
            if (fim == -1) {
                printf("\nNao ha mais vagoes para serem movidos a saida\n");
                break;
            }
            //procura pilha com espaço disponivel com indice maior que a pilha do proximo valor que deve ir para saida
            tr = retorna_proximo_trilho_vago(numero_trilhos, numero_de_vagas, fim, espera);

            //desempilha ate achar o vagao que deve ir para a saida
            while (ultimo_vagao(espera[fim])->posicao != vagao_final) {
                etapas++;
                //cria pílha caso nao tenha disponivel
                if (tr == -1) {
                    numero_trilhos++;
                    espera = incrementa_trilho(espera, numero_trilhos);
                    tr = retorna_proximo_trilho_vago(numero_trilhos, numero_de_vagas, fim, espera);
                    printf("\nCriado trilho de espera N: %i\n", numero_trilhos);
                }
                //retira da pilha o vagao nao final
                if (espera[tr] == NULL) {
                    espera[tr] = retira_ultimo(espera[fim]);

                    printf("\nVagao movido do trilho de espera N: %i para o trilho de espera N: %i", fim + 1, tr + 1);
                    exibe_vagoes(ultimo_vagao(espera[tr]));
                } else if (tr == fim) {
                    tr = retorna_proximo_trilho_vago(numero_trilhos, numero_de_vagas, (fim + 1), espera);
                } else {
                    ultimo_vagao(espera[tr])->anterior = retira_ultimo(espera[fim]);
                    printf("\nVagao movido do trilho de espera N: %i para o trilho de espera N: %i", fim + 1, tr + 1);
                    exibe_vagoes(ultimo_vagao(espera[tr]));
                }

            }

            //desempilha a pilha de espera ate que nao tenha um vagao compativel com a saida
            while (ultimo_vagao(espera[fim])->posicao == vagao_final) {
                ultimo_vagao(saida)->anterior = retira_ultimo(espera[fim]);
                printf("\nVagao transferido do trilho de espera %i para a saida\n", fim + 1);
                exibe_vagoes(ultimo_vagao(saida));
                vagao_final++;
                etapas++;
                if (espera[fim] == ultimo_vagao(saida)) {
                    espera[fim] = NULL;
                    break;
                }
            }

        }

        //exibe fila de saida
        printf("\nLinha saida\n");
        exibe_vagoes(saida);

        printf("\n\nForam criados %i trilhos de espera alem dos %i iniciais \n Foram %i passos para fazer a reordenacao dos vagoes",
               (numero_trilhos - rel_trilhos), (rel_trilhos), etapas);

        printf("\n\nDigite \"-1\" para fechar o programa ou qualquer valor para reiniciar a simulcao.\n\n");
        scanf("%d", &fora);
        if(fora==-1){
            return 0;
        }
    }

}
