#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>



// --- Constantes Globais ---
#define CAPACIDADE_MOCHILA 10 
#define MAX_NOME 50           
#define MAX_TIPO 50

// --- Estrutura do Item ---
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
} Item;

// --- "Banco de Dados" Global ---
Item mochila[CAPACIDADE_MOCHILA];
int totalItens = 0;

// --- funções ---
void limparBuffer(); 
void adicionarItem();
void removerItem();
void listarItens();
void buscarItemPorNome(); // nova função adicionada em NIVEL AVENTUREIRO
void exibirMenu();


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
            case 4: // <-- NOVO
                buscarItemPorNome();
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
 * @brief Exibe o menu principal de opções. (ATUALIZADO)
 */
void exibirMenu() {
    printf("\n--- MOCHILA VIRTUAL (Itens: %d/%d) ---\n", totalItens, CAPACIDADE_MOCHILA);
    printf("| 1. Adicionar Item                   |\n");
    printf("| 2. Remover Item                     |\n");
    printf("| 3. Listar Itens                     |\n");
    printf("| 4. Buscar Item por Nome             |\n"); 
    printf("| 0. Sair                             |\n");
    printf("---------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Limpa o buffer de entrada (stdin).
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Adiciona um novo item à mochila.
 */
void adicionarItem() {
    printf("\n--- Adicionar Novo Item ---\n");

    if (totalItens >= CAPACIDADE_MOCHILA) {
        printf("Erro: A mochila esta cheia! Nao e possivel adicionar mais itens.\n");
        return; 
    }

    Item* novoItem = &mochila[totalItens];

    printf("Nome do item: ");
    fgets(novoItem->nome, MAX_NOME, stdin);
    novoItem->nome[strcspn(novoItem->nome, "\n")] = '\0';

    printf("Tipo do item (ex: Pocao, Arma, Comida): ");
    fgets(novoItem->tipo, MAX_TIPO, stdin);
    novoItem->tipo[strcspn(novoItem->tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem->quantidade);
    limparBuffer();

    totalItens++;

    printf("\nItem '%s' (x%d) adicionado com sucesso!\n", novoItem->nome, novoItem->quantidade);
}

/**
 * @brief Remove um item da mochila pelo nome.
 */
void removerItem() {
    printf("\n--- Remover Item ---\n");

    if (totalItens == 0) {
        printf("A mochila esta vazia. Nao ha itens para remover.\n");
        return;
    }

    char nomeParaRemover[MAX_NOME];
    printf("Digite o nome exato do item a ser removido: ");
    fgets(nomeParaRemover, MAX_NOME, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = '\0';

    int indiceEncontrado = -1; 

    // Busca sequencial para encontrar o índice
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeParaRemover) == 0) {
            indiceEncontrado = i;
            break; 
        }
    }

    if (indiceEncontrado == -1) {
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeParaRemover);
    } else {
        // Desloca os itens para cobrir o espaço
        for (int i = indiceEncontrado; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        totalItens--;
        printf("\nItem '%s' removido com sucesso!\n", nomeParaRemover);
    }
}

/**
 * @brief Lista todos os itens cadastrados na mochila em formato de tabela.
 */
void listarItens() {
    printf("\n--- Itens na Mochila (%d/%d) ---\n", totalItens, CAPACIDADE_MOCHILA);

    printf("----------------------------------------------------------\n");
    printf("%-20s | %-20s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------------\n");

    if (totalItens == 0) {
        printf("Nenhum item cadastrado.\n");
    } else {
        for (int i = 0; i < totalItens; i++) {
            printf("%-20s | %-20s | %d\n",
                   mochila[i].nome,
                   mochila[i].tipo,
                   mochila[i].quantidade);
        }
    }
    printf("----------------------------------------------------------\n");
}

/**
 * @brief Busca sequencialmente um item pelo nome e exibe seus detalhes.
 * (NOVA FUNÇÃO)
 */
void buscarItemPorNome() {
    printf("\n--- Buscar Item por Nome ---\n");

    // 1. Verifica se a mochila está vazia
    if (totalItens == 0) {
        printf("A mochila esta vazia. Nao ha itens para buscar.\n");
        return;
    }

    // 2. Solicita o nome
    char nomeParaBuscar[MAX_NOME];
    printf("Digite o nome exato do item a ser buscado: ");
    fgets(nomeParaBuscar, MAX_NOME, stdin);
    nomeParaBuscar[strcspn(nomeParaBuscar, "\n")] = '\0';

    // 3. Implementa a flag (usando um índice)
    // -1 significa "não encontrado"
    int indiceEncontrado = -1;

    // 4. Realiza a busca sequencial
    for (int i = 0; i < totalItens; i++) {
        // 'strcmp' retorna 0 se as strings forem idênticas
        if (strcmp(mochila[i].nome, nomeParaBuscar) == 0) {
            indiceEncontrado = i; // Armazena o índice onde o item foi achado
            break; // Para o loop assim que encontrar o item
        }
    }

    // 5. Exibe o resultado
    if (indiceEncontrado != -1) {
        // Item foi encontrado
        printf("\nItem encontrado!\n");
        printf("-------------------------------\n");
        printf("Nome:       %s\n", mochila[indiceEncontrado].nome);
        printf("Tipo:       %s\n", mochila[indiceEncontrado].tipo);
        printf("Quantidade: %d\n", mochila[indiceEncontrado].quantidade);
        printf("-------------------------------\n");
    } else {
        // Item não foi encontrado
        printf("\nErro: Item '%s' nao foi encontrado na mochila.\n", nomeParaBuscar);
    }
}