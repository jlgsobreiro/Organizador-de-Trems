
typedef struct Vagao{
    char* valor;
    int posicao;
    struct Vagao* anterior;
}Vagao;

Vagao* cria_vagao(int posicao, char* valor);
Vagao* cria_vagoes_na_entrada (int* sequencia,char** lista ,int numero_vagoes);
void exibe_vagoes(Vagao* entrada);
Vagao* ultimo_vagao(Vagao* vagao);
Vagao** cria_trilhos_de_espera(int numero_de_trilhos);
int conta_vagoes (Vagao* vagao);
int retorna_trilho_do_vagao (int numero_de_trilhos, int posicao_do_vagao,Vagao** vagao);
int retorna_proximo_trilho_vago (int numero_de_trilhos, int numero_de_vagas,int trilho,Vagao** vagao);
Vagao* retira_ultimo (Vagao* vagao);
Vagao** incrementa_trilho(Vagao** espera, int numero_de_trilhos);
int trilho_cheio(Vagao* espera, int numero_de_vagas);
int escolhe_melhor_trilho (Vagao** espera, int numero_de_trilhos, int numero_de_vagas, Vagao* entrada);

