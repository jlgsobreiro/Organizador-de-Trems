#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

Vagao* cria_vagao(int posicao, char* valor){
    Vagao* vagao = calloc(sizeof(Vagao),1);

    vagao->posicao = posicao;
    vagao->valor = valor;
    vagao->anterior = NULL;

    return vagao;
}

Vagao* cria_vagoes_na_entrada (int* sequencia,char** lista,int numero_vagoes){
    int i = 0;
    if(sequencia!=NULL){
        Vagao* vagao = cria_vagao(sequencia[i],lista[i]);
        Vagao* aux = vagao;
        for (i=1;i<=numero_vagoes-1;i++){
            aux->anterior = cria_vagao(sequencia[i],lista[i]);
            aux = aux->anterior;
        }
        return vagao;
    } else {
        return NULL;
    }

}

void exibe_vagoes(Vagao* entrada){
    Vagao* vagao;
    int i = 0;
    for (vagao = entrada;vagao != NULL;vagao = vagao->anterior){
        i++;
        printf("Vagao %i // \n",i);
        printf("Posicao %i // \n",vagao->posicao);
        printf("Valor %s // \n",vagao->valor);
    }
    if(i==0){
        printf("Vagao Vazio \n");
    }
}

Vagao* ultimo_vagao(Vagao* vagao){
    if(vagao == NULL ){
        return vagao;
    }
    for (; vagao->anterior != NULL ; vagao = vagao->anterior);
    return vagao;
}

Vagao** cria_trilhos_de_espera(int numero_de_trilhos){

    Vagao** lista_de_trilhos = calloc((size_t) numero_de_trilhos, sizeof(Vagao*));


    return lista_de_trilhos;
}

int conta_vagoes (Vagao* vagao){
    int i;
    for (i=0; vagao != NULL ; vagao = vagao->anterior){
        i++;
    }

    return i;
}

int retorna_trilho_do_vagao (int numero_de_trilhos, int posicao_do_vagao,Vagao** vagao){

    Vagao** aux1 = vagao;
    int i;
    for (i = 0; i < numero_de_trilhos; ++i) {
        for (Vagao* aux = aux1[i]; aux != NULL; aux = aux->anterior) {
            if (aux->posicao == posicao_do_vagao){
                return i;
            }
        }
    }
    return -1;
}

int retorna_proximo_trilho_vago (int numero_de_trilhos, int numero_de_vagas,int trilho,Vagao** vagao){

    int i;
    for (i = trilho; i <= numero_de_trilhos-1; i++) {
        if(conta_vagoes(vagao[i])< numero_de_vagas){
            return i;
        }
    }
    return -1;
}

Vagao* retira_ultimo (Vagao* vagao){
    Vagao* aux;
    Vagao* aux2;
    if(vagao == NULL){
        return vagao;
    } else if(vagao->anterior == NULL){
        aux = vagao;//Item removido
        vagao = NULL;
        return aux;
    } else {

        for (aux = vagao; aux->anterior->anterior != NULL; aux = aux->anterior) ;

        aux2 = aux->anterior;
        aux->anterior = NULL;

        return aux2;
    }

}

Vagao** incrementa_trilho(Vagao** espera, int numero_de_trilhos){
    espera = realloc(espera, numero_de_trilhos * sizeof(Vagao *));
    espera[numero_de_trilhos - 1] = NULL;
    printf("\nNova Linha de espera criada");
    return espera;
}

int trilho_cheio(Vagao* espera, int numero_de_vagas){
    if(conta_vagoes(espera) == numero_de_vagas){
        return 1;
    }
    if(conta_vagoes(espera) == 0){
        return -1;
    }
    return 0;
}

int escolhe_melhor_trilho (Vagao** espera, int numero_de_trilhos, int numero_de_vagas, Vagao* entrada){
    int i,melhor_trilho=0,cheio=0;
    int posicao_entrada = entrada->posicao;
    for (i = 0; i < numero_de_trilhos; ++i) {
        switch (trilho_cheio(espera[i], numero_de_vagas)) {
            case 1:
                cheio++;
                break;

            case 0:
                melhor_trilho = ultimo_vagao(espera[i])->posicao > posicao_entrada ? i : -1;
                break;

            case -1:
                melhor_trilho = i;
            break;
        }
    }
    if(cheio == numero_de_trilhos){
        melhor_trilho = -1;
    }
    return melhor_trilho;
}