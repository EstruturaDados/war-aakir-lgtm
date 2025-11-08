#include <stdio.h>
#include <stdlib.h> // Para calloc, free, rand, srand
#include <string.h>
#include <time.h>   // Para time()

// Estrutura para armazenar os dados de um território
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

// Estrutura para armazenar os detalhes da missão do jogador
struct Missao {
    int id; // 1: Destruir cor, 2: Conquistar X territórios
    char descricao[100];
    char cor_alvo[10];       // Usado na missão 1
    int territorios_alvo; // Usado na missão 2
};

// --- Protótipos das Funções ---
void inicializar_territorios(struct Territorio *territorios, int num_territorios);
void exibir_mapa(const struct Territorio *territorios, int num_territorios);
void simular_ataque(struct Territorio *territorios, int num_territorios);
struct Missao sortear_missao();
void exibir_missao(const struct Missao *missao);
int verificar_missao(const struct Territorio *territorios, int num_territorios, const struct Missao *missao, const char* cor_jogador);


int main() {
    const int NUM_TERRITORIOS = 5;
    struct Territorio *territorios;
    struct Missao missao_jogador;
    char cor_jogador[10];
    int opcao;

    srand(time(NULL));

    territorios = (struct Territorio*) calloc(NUM_TERRITORIOS, sizeof(struct Territorio));
    if (territorios == NULL) {
        printf("Erro: Falha ao alocar memória.\n");
        return 1;
    }

    // --- Preparação do Jogo ---
    inicializar_territorios(territorios, NUM_TERRITORIOS);
    strcpy(cor_jogador, territorios[0].cor); // Assume que o jogador é o dono do primeiro território (cor "Azul")
    missao_jogador = sortear_missao();

    printf("--- Bem-vindo ao WAR Estruturado: Nível Mestre ---\
");
    printf("Você comanda o exército %s.\n", cor_jogador);
    exibir_missao(&missao_jogador);

    // --- Laço Principal do Jogo ---
    while (1) {
        exibir_mapa(territorios, NUM_TERRITORIOS);
        printf("\n--- Menu Principal ---\
");
        printf("1. Atacar\n");
        printf("2. Verificar Missão\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                simular_ataque(territorios, NUM_TERRITORIOS);
                break;
            case 2:
                if (verificar_missao(territorios, NUM_TERRITORIOS, &missao_jogador, cor_jogador)) {
                    printf("\n*************************************************\
");
                    printf("*** PARABÉNS! Missão Cumprida! Você venceu! ***\
");
                    printf("*************************************************\
");
                    free(territorios);
                    return 0; // Fim de jogo
                } else {
                    printf("\n>>> Missão ainda não cumprida. Continue a lutar!\
");
                }
                break;
            case 0:
                printf("\nJogo encerrado.\n");
                free(territorios);
                return 0;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    }
}

// Popula os territórios com dados pré-definidos.
void inicializar_territorios(struct Territorio *territorios, int num_territorios) {
    strcpy(territorios[0].nome, "Brasil");     strcpy(territorios[0].cor, "Azul");     territorios[0].tropas = 5;
    strcpy(territorios[1].nome, "Argentina");  strcpy(territorios[1].cor, "Verde");    territorios[1].tropas = 3;
    strcpy(territorios[2].nome, "Peru");       strcpy(territorios[2].cor, "Verde");    territorios[2].tropas = 2;
    strcpy(territorios[3].nome, "Chile");      strcpy(territorios[3].cor, "Vermelho"); territorios[3].tropas = 4;
    strcpy(territorios[4].nome, "Colombia");   strcpy(territorios[4].cor, "Amarelo");  territorios[4].tropas = 3;
}

// Exibe o mapa. Usa 'const' pois não modifica os dados.
void exibir_mapa(const struct Territorio *territorios, int num_territorios) {
    printf("\n\n--- Mapa de Territórios Atual ---\
");
    printf("ID | %-20s | %-15s | %s\n", "Território", "Cor do Exército", "Tropas");
    printf("------------------------------------------------------\
");
    for (int i = 0; i < num_territorios; i++) {
        printf("% -2d | %-20s | %-15s | %d\n", i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
    printf("------------------------------------------------------\
");
}

// Lógica de ataque, similar ao nível anterior, com mais validações.
void simular_ataque(struct Territorio *territorios, int num_territorios) {
    int id_atacante, id_defensor;
    printf("\nEscolha o território ATACANTE (ID): "); scanf("%d", &id_atacante);
    printf("Escolha o território DEFENSOR (ID): "); scanf("%d", &id_defensor);

    if (id_atacante < 1 || id_atacante > num_territorios || id_defensor < 1 || id_defensor > num_territorios || id_atacante == id_defensor) {
        printf("\nErro: Seleção de territórios inválida.\n"); return;
    }
    int idx_atacante = id_atacante - 1;
    int idx_defensor = id_defensor - 1;

    if (territorios[idx_atacante].tropas < 2) {
        printf("\nErro: O atacante precisa de pelo menos 2 tropas.\n"); return;
    }
    if (strcmp(territorios[idx_atacante].cor, territorios[idx_defensor].cor) == 0) {
        printf("\nErro: Você não pode atacar um território que já é seu.\n"); return;
    }

    printf("\n>>> Batalha: %s ataca %s!\n", territorios[idx_atacante].nome, territorios[idx_defensor].nome);
    int dado_ataque = (rand() % 6) + 1, dado_defesa = (rand() % 6) + 1;
    printf("... Dados -> Ataque: %d | Defesa: %d ...\n", dado_ataque, dado_defesa);

    if (dado_ataque >= dado_defesa) {
        printf("Resultado: Vitória do ATACANTE! Defensor perde 1 tropa.\n");
        territorios[idx_defensor].tropas--;
        if (territorios[idx_defensor].tropas == 0) {
            printf("!!! CONQUISTA !!! %s foi dominado por %s.\n", territorios[idx_defensor].nome, territorios[idx_atacante].cor);
            strcpy(territorios[idx_defensor].cor, territorios[idx_atacante].cor);
            territorios[idx_defensor].tropas = 1;
            territorios[idx_atacante].tropas--;
        }
    } else {
        printf("Resultado: Vitória do DEFENSOR! Ataque repelido.\n");
    }
}

// Sorteia uma das duas missões possíveis.
struct Missao sortear_missao() {
    struct Missao missao;
    if (rand() % 2 == 0) {
        missao.id = 1;
        strcpy(missao.descricao, "Destruir todos os exércitos da cor Verde.");
        strcpy(missao.cor_alvo, "Verde");
        missao.territorios_alvo = 0;
    } else {
        missao.id = 2;
        strcpy(missao.descricao, "Conquistar um total de 3 territórios.");
        strcpy(missao.cor_alvo, "");
        missao.territorios_alvo = 3;
    }
    return missao;
}

// Exibe a missão do jogador.
void exibir_missao(const struct Missao *missao) {
    printf("\n--- SUA MISSÃO ---\
");
    printf("%s\n", missao->descricao);
    printf("------------------\n");
}

// Verifica se a missão do jogador foi concluída.
int verificar_missao(const struct Territorio *territorios, int num_territorios, const struct Missao *missao, const char* cor_jogador) {
    if (missao->id == 1) { // Missão: Destruir a cor_alvo
        for (int i = 0; i < num_territorios; i++) {
            if (strcmp(territorios[i].cor, missao->cor_alvo) == 0) return 0; // Inimigo ainda existe
        }
        return 1; // Inimigo não encontrado, missão cumprida
    }
    else if (missao->id == 2) { // Missão: Conquistar um número de territórios
        int count = 0;
        for (int i = 0; i < num_territorios; i++) {
            if (strcmp(territorios[i].cor, cor_jogador) == 0) count++;
        }
        return count >= missao->territorios_alvo; // Retorna verdadeiro se o objetivo foi alcançado
    }
    return 0;
}
