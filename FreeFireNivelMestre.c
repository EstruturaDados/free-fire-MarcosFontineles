#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


// --- Constantes Globais ---
#define CAPACIDADE_MOCHILA 10
#define MAX_NOME 50
#define MAX_TIPO 50

// --- Enum para Critérios de Ordenação 
typedef enum {
    NOME,
    TIPO,
    PRIORIDADE,
    DESORDENADO // Estado padrão ou após modificações
} CriterioOrdenacao;

// --- Estrutura do Item (Atualizada) ---
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; // Novo campo (1=Max, 5=Min)
} Item;

// --- "Banco de Dados" e Estado Global ---
Item mochila[CAPACIDADE_MOCHILA];
int totalItens = 0;

CriterioOrdenacao estadoOrdenacao = DESORDENADO;

// --- Protótipos das Funções ---
void limparBuffer();
void adicionarItem();
void removerItem();
void listarItens();
void exibirMenu();

// funções (Nível Mestre)
const char* getNomeEstadoOrdenacao(); // Helper para exibir o estado
void ordenarMochila();
void insertionSort(CriterioOrdenacao criterio);
void buscarComBuscaBinaria();


int main() {
    int opcao;

    do {
        exibirMenu();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                adicionarItem();
                break;
            case 2:
                removerItem();
                break;
            case 3:
                listarItens();
                break;
            case 4:
                ordenarMochila(); // NOVO: Submenu de ordenação
                break;
            case 5:
                buscarComBuscaBinaria(); // NOVO: Busca Binária
                break;
            case 0:
                printf("\nSaindo da mochila... Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter para tentar novamente...\n");
                limparBuffer();
        }

    } while (opcao != 0);

    return 0;
}


/**
 * @brief Helper que retorna o nome do estado atual de ordenação.
 */
const char* getNomeEstadoOrdenacao() {
    switch (estadoOrdenacao) {
        case NOME:       return "Ordenado por Nome";
        case TIPO:       return "Ordenado por Tipo";
        case PRIORIDADE: return "Ordenado por Prioridade";
        default:         return "Desordenado";
    }
}

/**
 * @brief Exibe o menu principal de opções. (ATUALIZADO)
 */
void exibirMenu() {
    printf("\n--- MOCHILA MESTRE (Itens: %d/%d) ---\n", totalItens, CAPACIDADE_MOCHILA);
    printf("| Estado Atual: %s\n", getNomeEstadoOrdenacao());
    printf("---------------------------------------\n");
    printf("| 1. Adicionar Item                   |\n");
    printf("| 2. Remover Item                     |\n");
    printf("| 3. Listar Itens                     |\n");
    printf("| 4. Ordenar Mochila                  |\n"); // NOVO
    printf("| 5. Buscar (Busca Binaria) por Nome  |\n"); // NOVO
    printf("| 0. Sair                             |\n");
    printf("---------------------------------------\n");
    printf("Escolha uma opcao: ");
}


void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void adicionarItem() {
    printf("\n--- Adicionar Novo Item ---\n");

    if (totalItens >= CAPACIDADE_MOCHILA) {
        printf("Erro: A mochila esta cheia!\n");
        return;
    }

    Item* novoItem = &mochila[totalItens];

    printf("Nome do item: ");
    fgets(novoItem->nome, MAX_NOME, stdin);
    novoItem->nome[strcspn(novoItem->nome, "\n")] = '\0';

    printf("Tipo do item: ");
    fgets(novoItem->tipo, MAX_TIPO, stdin);
    novoItem->tipo[strcspn(novoItem->tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem->quantidade);

    // Validação do novo campo 'prioridade'
    do {
        printf("Prioridade (1=Alta, 5=Baixa): ");
        scanf("%d", &novoItem->prioridade);
        if (novoItem->prioridade < 1 || novoItem->prioridade > 5) {
            printf("Valor invalido. Digite um numero entre 1 e 5.\n");
        }
    } while (novoItem->prioridade < 1 || novoItem->prioridade > 5);

    limparBuffer();

    totalItens++;
    // se Adicionar um item quebra a ordenação
    estadoOrdenacao = DESORDENADO;

    printf("\nItem '%s' (Prioridade %d) adicionado com sucesso!\n", novoItem->nome, novoItem->prioridade);
}

/**
 * @brief Remove um item da mochila pelo nome
 */
void removerItem() {
    printf("\n--- Remover Item ---\n");

    if (totalItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    char nomeParaRemover[MAX_NOME];
    printf("Digite o nome exato do item a ser removido: ");
    fgets(nomeParaRemover, MAX_NOME, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = '\0';

    int indiceEncontrado = -1;
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeParaRemover) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) {
        printf("\nItem '%s' nao encontrado.\n", nomeParaRemover);
    } else {
        for (int i = indiceEncontrado; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        totalItens--;
        // e se remover um item também quebra a ordenação
        estadoOrdenacao = DESORDENADO;
        printf("\nItem '%s' removido com sucesso!\n", nomeParaRemover);
    }
}

/**
 * @brief Lista todos os itens cadastrados. 
 */
void listarItens() {
    printf("\n--- Itens na Mochila (%d/%d) ---\n", totalItens, CAPACIDADE_MOCHILA);
    printf("| Estado: %s\n", getNomeEstadoOrdenacao());

    // Cabeçalho da tabela atualizado
    printf("---------------------------------------------------------------------\n");
    printf("%-20s | %-15s | %-10s | %s\n", "NOME", "TIPO", "PRIORIDADE", "QTD");
    printf("---------------------------------------------------------------------\n");

    if (totalItens == 0) {
        printf("Nenhum item cadastrado.\n");
    } else {
        // Loop para imprimir itens
        for (int i = 0; i < totalItens; i++) {
            printf("%-20s | %-15s | %-10d | %d\n",
                   mochila[i].nome,
                   mochila[i].tipo,
                   mochila[i].prioridade,
                   mochila[i].quantidade);
        }
    }
    printf("---------------------------------------------------------------------\n");
}



/**
 * @brief Exibe um submenu para o usuário escolher como ordenar.
 */
void ordenarMochila() {
    if (totalItens < 2) {
        printf("\nNecessario ao menos 2 itens na mochila para ordenar.\n");
        return;
    }

    int opcao;
    printf("\n--- Criterio de Ordenacao ---\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo (A-Z)\n");
    printf("3. Por Prioridade (1-5, Alta primeiro)\n");
    printf("0. Cancelar\n");
    printf("Escolha: ");

    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1:
            insertionSort(NOME);
            break;
        case 2:
            insertionSort(TIPO);
            break;
        case 3:
            insertionSort(PRIORIDADE);
            break;
        case 0:
            printf("Ordenacao cancelada.\n");
            break;
        default:
            printf("Opcao invalida.\n");
    }
}


void insertionSort(CriterioOrdenacao criterio) {
    long long comparacoes = 0;                  // Contador de desempenho
    Item chave;
    int j;

    // Loop principal do Insertion Sort
    for (int i = 1; i < totalItens; i++) {
        chave = mochila[i]; // O item a ser posicionado
        j = i - 1;

        bool deveTrocar = false;

        // Loop interno para encontrar a posição correta
      
        while (j >= 0) {
            comparacoes++; // CONTA A COMPARAÇÃO
            
            
            switch (criterio) {
                case NOME:
                    // Se o item[j] vem *depois* da chave (alfabeticamente)
                    deveTrocar = (strcmp(mochila[j].nome, chave.nome) > 0);
                    break;
                case TIPO:
                    deveTrocar = (strcmp(mochila[j].tipo, chave.tipo) > 0);
                    break;
                case PRIORIDADE:
                    // Se a prioridade[j] é *maior* que a da chave (ex: 3 > 1)
                    deveTrocar = (mochila[j].prioridade > chave.prioridade);
                    break;
                default: // Segurança, não deve acontecer
                     deveTrocar = false;
            }

            if (deveTrocar) {
                // "Empurra" o item[j] para a direita
                mochila[j + 1] = mochila[j];
                j--; // Move o índice para a esquerda
            } else {
                // Posição correta encontrada
                break;
            }
        }
        // Insere a chave na sua posição correta
        mochila[j + 1] = chave;
    }

    // Atualiza o estado 
    estadoOrdenacao = criterio;
    printf("\nMochila ordenada por '%s' com %lld comparacoes.\n", getNomeEstadoOrdenacao(), comparacoes);
}

/**
 * @brief Busca um item usando Busca Binária (Requer ordenação por nome).
 */
void buscarComBuscaBinaria() {
    printf("\n--- Buscar Item (Busca Binaria) ---\n");

    // 1. Verificação de Pré-requisito 
    if (estadoOrdenacao != NOME) {
        printf("ERRO FATAL: A Busca Binaria so funciona se a mochila\n");
        printf("estiver ordenada por NOME. Use a Opcao 4 primeiro.\n");
        return;
    }

    if (totalItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    // 2. Solicita o nome
    char nomeParaBuscar[MAX_NOME];
    printf("Digite o nome exato do item a ser buscado: ");
    fgets(nomeParaBuscar, MAX_NOME, stdin);
    nomeParaBuscar[strcspn(nomeParaBuscar, "\n")] = '\0';

    // 3. Lógica da Busca Binária
    int inicio = 0;
    int fim = totalItens - 1;
    int indiceEncontrado = -1;
    long long comparacoes = 0;

    while (inicio <= fim) {
        comparacoes++;
        // Calcula o meio (evitando overflow de (inicio+fim))
        int meio = inicio + (fim - inicio) / 2;
        
        // Compara a string do meio com a buscada
        int cmp = strcmp(mochila[meio].nome, nomeParaBuscar);

        if (cmp == 0) {
            // 0 = Strings são iguais 
            indiceEncontrado = meio;
            break;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    
    // 4. Exibe o resultado
    printf("Busca binaria concluida com %lld comparacoes.\n", comparacoes);
    if (indiceEncontrado != -1) {
        printf("\nItem encontrado!\n");
        printf("-------------------------------\n");
        printf("Nome:       %s\n", mochila[indiceEncontrado].nome);
        printf("Tipo:       %s\n", mochila[indiceEncontrado].tipo);
        printf("Quantidade: %d\n", mochila[indiceEncontrado].quantidade);
        printf("Prioridade: %d\n", mochila[indiceEncontrado].prioridade);
        printf("-------------------------------\n");
    } else {
        printf("\nErro: Item '%s' nao foi encontrado na mochila.\n", nomeParaBuscar);
    }
}