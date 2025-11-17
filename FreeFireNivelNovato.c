#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h> 
#include <string.h> 

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

// --- "Banco de Dados"  ---

Item mochila[CAPACIDADE_MOCHILA];
// Variável para controlar quantos itens realmente estão na mochila
int totalItens = 0;

// --- Funções ---
void limparBuffer();
void adicionarItem();
void removerItem();
void listarItens();
void exibirMenu();


int main() {
    int opcao;

    do {
        exibirMenu();
        
        // Lê a opção do usuário
        scanf("%d", &opcao);
        
        // Limpa o '\n' do buffer para evitar problemas com 'fgets'
        limparBuffer(); 

        // opções com switch
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
            case 0:
                printf("\nSaindo da mochila... Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida! Pressione Enter para tentar novamente...\n");
                // Aguarda o usuário pressionar Enter
                limparBuffer(); 
        }

    } while (opcao != 0); // Continua o loop enquanto a opção não for 0 (Sair)

    return 0;
}

/*
 * =======================
 * FUNÇÕES AUXILIARES
 * =======================
 */

/**
 * @brief Exibe o menu principal de opções.
 */
void exibirMenu() {
    printf("\n--- MOCHILA VIRTUAL (Itens: %d/%d) ---\n", totalItens, CAPACIDADE_MOCHILA);
    printf("| 1. Adicionar Item                   |\n");
    printf("| 2. Remover Item                     |\n");
    printf("| 3. Listar Itens                     |\n");
    printf("| 0. Sair                             |\n");
    printf("---------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Limpa o buffer de entrada .
  */
void limparBuffer() {
    int c;
    // Lê e descarta caracteres do buffer até encontrar um '\n' ou o fim do arquivo 
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Adiciona um novo item à mochila.
 */
void adicionarItem() {
    printf("\n--- Adicionar Novo Item ---\n");

    // 1. Verifica se a mochila está cheia
    if (totalItens >= CAPACIDADE_MOCHILA) {
        printf("Erro: A mochila esta cheia! Nao e possivel adicionar mais itens.\n");
        return; 
    }

    // 2. Coleta dados do novo item
    // Usa a posição 'totalItens' como o índice do novo item
    Item* novoItem = &mochila[totalItens];

    printf("Nome do item: ");
    fgets(novoItem->nome, MAX_NOME, stdin);
    novoItem->nome[strcspn(novoItem->nome, "\n")] = '\0';

    printf("Tipo do item (ex: Pocao, Arma, Comida): ");
    fgets(novoItem->tipo, MAX_TIPO, stdin);
    novoItem->tipo[strcspn(novoItem->tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem->quantidade);
    // Limpa o buffer após o scanf de número
    limparBuffer();

    // 3. Incrementa o contador de itens
    totalItens++;

    printf("\nItem '%s' (x%d) adicionado com sucesso!\n", novoItem->nome, novoItem->quantidade);
}

/**
 * @brief Remove um item da mochila pelo nome.
 */
void removerItem() {
    printf("\n--- Remover Item ---\n");

    // 1. Verifica se a mochila está vazia
    if (totalItens == 0) {
        printf("A mochila esta vazia. Nao ha itens para remover.\n");
        return;
    }

    char nomeParaRemover[MAX_NOME];
    printf("Digite o nome exato do item a ser removido: ");
    fgets(nomeParaRemover, MAX_NOME, stdin);
    nomeParaRemover[strcspn(nomeParaRemover, "\n")] = '\0';

    int indiceEncontrado = -1; // -1 significa "não encontrado"

    // 2. Encontra o índice do item
    for (int i = 0; i < totalItens; i++) {
        // 'strcmp' retorna 0 se as strings forem idênticas
        if (strcmp(mochila[i].nome, nomeParaRemover) == 0) {
            indiceEncontrado = i;
            break; // Para o loop assim que encontrar o item
        }
    }

    // 3. Processa a remoção
    if (indiceEncontrado == -1) {
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeParaRemover);
    } else {
        // 4. Remove o item deslocando todos os itens subsequentes para a esquerda
       
        for (int i = indiceEncontrado; i < totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }

        // 5. Decrementa o contador total de itens
        totalItens--;

        printf("\nItem '%s' removido com sucesso!\n", nomeParaRemover);
    }
}

/**
 * @brief Lista todos os itens cadastrados na mochila em formato de tabela.
 */
/**
 * @brief Lista todos os itens cadastrados na mochila em formato de tabela.
 * (VERSÃO ATUALIZADA)
 */
void listarItens() {
    printf("\n--- Itens na Mochila (%d/%d) ---\n", totalItens, CAPACIDADE_MOCHILA);

    // 1. Imprime o cabeçalho da tabela (SEMPRE)
    printf("----------------------------------------------------------\n");
    printf("%-20s | %-20s | %s\n", "NOME", "TIPO", "QUANTIDADE");
    printf("----------------------------------------------------------\n");

    // 2. verifica se está vazia
    if (totalItens == 0) {
        // Imprime a mensagem vazia
        printf("Nenhum item cadastrado.\n");
    } else {
        // 3. Itera e imprime cada item (se houver)
        for (int i = 0; i < totalItens; i++) {
            printf("%-20s | %-20s | %d\n",
                   mochila[i].nome,
                   mochila[i].tipo,
                   mochila[i].quantidade);
        }
    }

    printf("----------------------------------------------------------\n");
}