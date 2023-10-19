/* Trabalho de SI201 - Estrutura de Dados da FT - Unicamp
Integrantes do Grupo:
Gabriel José Pereira 247454
Gabriel Ferreira Souza 247453
Ricardo Soares Costa Junior 247592
Thiago Armando Paracampos Correa 254366
*/

#include <stdio.h>
#include <stdlib.h>

// Variaveis globais para controle dos IDs
int ID_PEDIDOS = 0;
int ID_PRODUTOS = 0;

// Cada nó dessa lista equivale a um item adicionado a um pedido
typedef struct no_lista_itens{
    int id_produto;
    int quantidade;
    struct no_lista_itens *prox;
}Tno_listaI;

// Cada nó da fila equivale a um pedido
typedef struct no_fila{
    int id;
    Tno_listaI *inicio_lista;
    struct no_fila *prox;
}Tno_fila;

// Cada nó da pilha contém um produto, que contém um tamanho, e o ponteiro para o próximo produto desse mesmo tipo
typedef struct pilha_itens{
    char tamanho;
    struct pilha_itens *prox;
}Tno_pilha;


// Cada nó da lista do estoque contém um ID para o tipo de produto, a sua quantidade em estoque, o ponteiro para o início da pilha de itens em estoque, e o ponteiro para o próximo ID de produto
typedef struct no_lista_estoque{
    int id;
    int quant_estoque;
    Tno_pilha *inicio_pilha;
    struct no_lista_estoque *prox;
}Tno_listaE;

// Funções da Pilha de Itens

void Push(Tno_pilha **inicio, char tamanho){
    Tno_pilha *novo = (Tno_pilha*)malloc(sizeof(Tno_pilha));
    novo->tamanho = tamanho;
    if(*inicio == NULL){
        novo->prox = NULL;
        *inicio = novo;
    }else{
        novo->prox = *inicio;
        *inicio = novo;
    }
}

void Pop(Tno_pilha **inicio){
    if(*inicio == NULL){
        printf("Nao ha elementos na pilha de itens.\n\n");
        return;
    }else{
        Tno_pilha *aux;
        aux = *inicio;
        *inicio = (*inicio)->prox;
        free(aux);
    }
}

void AtualizarQuantidadeEstoque(Tno_pilha *inicio, int *quant){
    (*quant) = 0;
    if(inicio == NULL){
        (*quant) = 0;
    }else{
        while(inicio != NULL){
            (*quant)++;
            inicio = inicio->prox;
        }
    }
}

void InserirQuantidadeProduto(Tno_listaE *inicio, int id, int quantidade, char tamanho){
    if(quantidade <= 0){
        printf("Quantidade invalida.\n\n");
        return;
    }
    if(inicio == NULL){
        printf("Nao ha nenhum produto cadastrado no estoque.\n\n");
        return;
    }else{
        while(inicio != NULL && inicio->id != id){
            inicio = inicio->prox;
        }
        if(inicio->id == id){
            for(int i = 0; i < quantidade; i++){
            Push(&inicio->inicio_pilha, tamanho);
            AtualizarQuantidadeEstoque(inicio->inicio_pilha, &inicio->quant_estoque);
        }
        }else{
            printf("Nao foi encontrado o item de ID %d em estoque.\n\n", id);
            return;
        }
        printf("Quantidade do produto de ID %d atualizada.\n\n", id);
    }
}

void RemoverQuantidadeProduto(Tno_listaE *inicio, int id, int quantidade){
        if(quantidade <= 0){
        printf("Quantidade invalida.\n\n");
        return;
    }
        if(inicio == NULL){
        printf("Nao ha nenhum produto cadastrado no estoque.\n\n");
        return;
    }else{
        while(inicio != NULL && inicio->id != id){
            inicio = inicio->prox;
        }
        if(inicio->id == id){
            if(inicio->quant_estoque < quantidade){
                printf("Quantidade invalida: nao ha produtos suficiente em estoque para a remocao.\n\n");
                return;
            }
            for(int i = 0; i < quantidade; i++){
            Pop(&inicio->inicio_pilha);
            AtualizarQuantidadeEstoque(inicio->inicio_pilha, &inicio->quant_estoque);
        }
        }else{
            printf("Nao foi encontrado o item de ID %d em estoque.\n\n", id);
            return;
        }
        printf("Quantidade do produto de ID %d atualizada.\n\n", id);
    }
}

// **********FUNÇÕES DE ESTOQUE - LISTA**********



void VisualizarEstoque(Tno_listaE *inicio){
    if(inicio == NULL){
        printf("Nao ha nenhum item cadastrado em estoque.\n\n");
    }else{
        printf("\n*****STATUS DO ESTOQUE*****\n\n");
        while(inicio != NULL){
            printf("***PRODUTO ID %d***\n", inicio->id);
            printf("Quant. em Estoque: %d\n\n", inicio->quant_estoque);
            inicio = inicio->prox;
        }
    }
}

// Responsável por gerar um novo ID de produto. Inicia um novo ID sem nenhum item em estoque

void InserirNovoProduto(Tno_listaE **inicio){
    Tno_listaE *novo = (Tno_listaE*)malloc(sizeof(Tno_listaE));
    novo->id = ID_PRODUTOS;
    novo->quant_estoque = 0;
    novo->inicio_pilha = NULL;
    novo->prox = NULL;
    if(*inicio == NULL){
        *inicio = novo;
    }else{
        Tno_listaE *percorre;
        percorre = *inicio;
        while(percorre->prox != NULL){
            percorre = percorre->prox;
        }
        percorre->prox = novo;
    }
    printf("O novo produto foi cadastrado com o ID %d.\n\n", novo->id);
    ID_PRODUTOS++;
    VisualizarEstoque(*inicio);
}

// Apaga o cadastro do produto com o ID fornecido

void CancelarCadastroProduto(Tno_listaE **inicio, int id){
    Tno_listaE *percorre;
    if(*inicio == NULL){
        printf("Nao ha produtos cadastrados no estoque.\n\n");
        return;
    }else{
        if((*inicio)->id == id){
            Tno_listaE *aux;
            aux = *inicio;
            *inicio = aux->prox;
            if(aux->quant_estoque > 0){
                RemoverQuantidadeProduto(*inicio, id, aux->quant_estoque);
            }
            printf("Produto de ID %d removido do cadastro do estoque.\n\n", aux->id);
            free(aux);
        }else{
            percorre = *inicio;
            while(percorre->prox != NULL && percorre->prox->id != id){
                percorre = percorre->prox;
            }
            if(percorre->prox->id == id){
                Tno_listaE *aux;
                aux = percorre->prox;
                percorre->prox = aux->prox;
                if(aux->quant_estoque > 0){
                    RemoverQuantidadeProduto(*inicio, id, aux->quant_estoque);
                }
                printf("Produto de ID %d removido do cadastro do estoque.\n\n", aux->id);
                free(aux);
            }else{
                printf("Nao foi encontrado o item de ID %d em estoque.\n\n", id);
                return;
            }
        }
    }
}

int VerificarQuantidadeEstoque(int id_produto, int quantidade, Tno_listaE *inicio){
    if(inicio == NULL){
        printf("Nao ha nenhum produto cadastrado no estoque.\n\n");
        return 0;
    }else{
        while(inicio != NULL && inicio->id != id_produto){
            inicio = inicio->prox;
        }
        if(inicio == NULL){
            printf("Nao foi encontrado o item de ID %d em estoque.\n\n", id_produto);
            return 0;            
        }
        if(inicio->id == id_produto){
            if(quantidade <= inicio->quant_estoque){
                return 1;
            }else{
                printf("Nao ha quantidade suficiente desse produto em estoque.\n\n");
                return 0;
            }
        }else{
            printf("Nao foi encontrado o item de ID %d em estoque.\n\n", id_produto);
            return 0;
        }
    }
}

// **********FUNÇÕES DA LISTA DE ITENS**********

void InserirItem(Tno_listaI **inicio, int index_produto, int quantidade, Tno_listaE *inicio_estoque){
    if(quantidade <= 0){
        printf("Quantidade invalida.\n\n");
        return;
    }
    if(VerificarQuantidadeEstoque(index_produto, quantidade, inicio_estoque)){
    Tno_listaI *novo = (Tno_listaI*)malloc(sizeof(Tno_listaI));
    novo->id_produto = index_produto;
    novo->quantidade = quantidade;
    novo->prox = NULL;
    if(*inicio == NULL){
        *inicio = novo;       
    }else{
        Tno_listaI *percorre;
        percorre = *inicio;
        if(percorre->id_produto == index_produto){
            if(VerificarQuantidadeEstoque(index_produto, (percorre->quantidade + quantidade), inicio_estoque)){
                percorre->quantidade += quantidade;
                printf("Como o item de ID %d ja existia no pedido, sua quantidade foi alterada.\n\n", index_produto);
                free(novo);
                return;
            }else{
                return;
            }
        }
        while(percorre->prox != NULL){
            if(percorre->id_produto == index_produto){
                if(VerificarQuantidadeEstoque(index_produto, (percorre->quantidade + quantidade), inicio_estoque)){
                percorre->quantidade += quantidade;
                printf("Como o item de ID %d ja existia no pedido, sua quantidade foi alterada.\n\n", index_produto);
                free(novo);
                return;
                }else{
                    return;
                }
            }
            percorre = percorre->prox;
        }
        if(percorre->id_produto == index_produto){
            if(VerificarQuantidadeEstoque(index_produto, (percorre->quantidade + quantidade), inicio_estoque)){
                percorre->quantidade += quantidade;
                printf("Como o item de ID %d ja existia no pedido, sua quantidade foi alterada.\n\n", index_produto);
                free(novo);
                return;
            }
            else{
                return;
            }
        }
        percorre->prox = novo;
    }
    printf("Item inserido.\n");
    }else{
        return;
    }
}

void CancelarItem(Tno_listaI **inicio, int index_produto){
    Tno_listaI *aux, *percorre;
    if(*inicio == NULL){
        printf("Nao ha itens no pedido.\n");
    }else{
        percorre = *inicio;
        if(percorre->id_produto == index_produto){
            aux = percorre;
            *inicio = aux->prox;
            printf("O item de ID %d foi removido do pedido.\n\n", aux->id_produto);
            free(aux);
        }else{
            if(percorre->prox == NULL){
                printf("Nao foi encontrado o item de ID %d no pedido.\n\n", index_produto);
                return;
            }
            while(percorre->prox->id_produto != index_produto && percorre->prox != NULL){
                percorre = percorre->prox;
            }
            if(percorre->prox->id_produto == index_produto){
                aux = percorre->prox;
                percorre->prox = aux->prox;
                printf("O item de ID %d foi removido do pedido.\n\n", aux->id_produto);
                free(aux);
            }else{
                printf("Nao foi encontrado o item de ID %d no pedido.\n\n", index_produto);
            }
        }
    }
}

void AlterarItens(Tno_listaI *inicio, int id, int quantidade, Tno_listaE *inicio_estoque){
    if(inicio == NULL){
            printf("Nao ha itens na lista.\n\n");
    }else{
            while(inicio != NULL && inicio->id_produto != id){
                inicio = inicio->prox;
            }
            if(inicio->id_produto == id){
                if(VerificarQuantidadeEstoque(id, quantidade, inicio_estoque)){
                inicio->quantidade = quantidade;
                }else{
                    return;
                }
            }else{
                printf("Nao foi encontrado o item de ID %d no pedido.\n\n", id);
            }
        }
}


void ConsultarItens(Tno_listaI *inicio, int id_pedido_fila){
    if(inicio == NULL){
        printf("Nao ha itens no pedido.\n\n");
    }else{
        printf("***PEDIDO -- ID %d***\n", id_pedido_fila);
        while(inicio != NULL){
            printf("ID do Item: %d -- Quant.: %d\n", inicio->id_produto, inicio->quantidade);
            inicio = inicio->prox;
        }
        printf("\n");
    }
}

// **********FUNÇÕES DA FILA DE PEDIDOS*********

void InserirPedido(Tno_fila **inicio, Tno_listaE *inicio_estoque){
    Tno_fila *novo = (Tno_fila*)malloc(sizeof(Tno_fila));
    novo->id = ID_PEDIDOS;
    novo->inicio_lista = NULL;
    novo->prox = NULL;
    int opcao, id_item, quantidade;
    do{
        printf("\n*****NOVO PEDIDO*****\n");
        printf("1 - Inserir Novo Item\n2 - Cancelar Item\n3 - Alterar Item\n4 - Consultar Itens\n0 - Finalizar\n");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
            printf("Qual o ID do produto a ser inserido? ");
            scanf("%d", &id_item);
            printf("Qual a quantidade do produto a ser comprada? ");
            scanf("%d", &quantidade);
            InserirItem(&novo->inicio_lista, id_item, quantidade, inicio_estoque);
            break;
            case 2:
            printf("Qual o ID do produto a ser removido? ");
            scanf("%d", &id_item);
            CancelarItem(&novo->inicio_lista, id_item);
            break;
            case 3:
            printf("Qual o ID do item a ser alterado? ");
            scanf("%d", &id_item);
            printf("Digite a nova quantidade: ");
            scanf("%d", &quantidade);
            AlterarItens(novo->inicio_lista, id_item, quantidade, inicio_estoque);
            break;
            case 4:
            ConsultarItens(novo->inicio_lista, novo->id);
            break;
            case 0:
            printf("Registrando pedido...\n");
            break;
            default:
            printf("Opcao Invalida\n");
        }
    }while(opcao != 0);

    if(novo->inicio_lista == NULL){
        free(novo);
        printf("Como o pedido estava vazio, o seu ID foi cancelado.\n\n");
    }else{
        Tno_fila *percorre;
        if(*inicio == NULL){
            *inicio = novo;
        }else{
            percorre = *inicio;
            while(percorre->prox != NULL){
                percorre = percorre->prox;
            }
            percorre->prox = novo;
        }
        printf("Seu pedido foi registrado com sucesso. ID do pedido gerado: %d.\n\n", novo->id);
        ID_PEDIDOS++;
    }
}

void ConsultarPedidos(Tno_fila *inicio){
    Tno_fila *percorre = inicio;
    if(percorre == NULL){
        printf("Nao ha pedidos na fila.\n\n");
    }else{
        printf("*****STATUS DA FILA DE PEDIDOS*****\n\n");
        while(percorre != NULL){
            ConsultarItens(percorre->inicio_lista, percorre->id);
            percorre = percorre->prox;
        }
    }
}

void AlterarPedido(Tno_fila *inicio, int id, Tno_listaE *inicio_estoque){
    Tno_fila *pedido;
    if(inicio == NULL){
        printf("Nao existe o pedido de ID %d na fila.\n\n");
        return;
    }else{
        while(inicio != NULL && inicio->id != id){
            inicio = inicio->prox;
        }
        if(inicio->id == id){
            pedido = inicio;
        }else{
            printf("Nao existe o pedido de ID %d na fila.\n\n");
            return;
        }
    }

    int opcao, id_pedido_scan, quantidade;    
    do{
        printf("\n*****PEDIDO DE ID %d*****\n", id);
        printf("1 - Inserir Novo Item\n2 - Cancelar Item\n3 - Alterar Item\n4 - Consultar Itens\n0 - Finalizar\n");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
            printf("Qual o ID do produto a ser inserido? ");
            scanf("%d", &id_pedido_scan);
            printf("Qual a quantidade do produto a ser comprada? ");
            scanf("%d", &quantidade);
            InserirItem(&pedido->inicio_lista, id_pedido_scan, quantidade, inicio_estoque);
            break;
            case 2:
            printf("Qual o ID do produto a ser removido? ");
            scanf("%d", &id_pedido_scan);
            CancelarItem(&pedido->inicio_lista, id_pedido_scan);
            break;
            case 3:
            printf("Qual o ID do item a ser alterado? ");
            scanf("%d", &id_pedido_scan);
            printf("Digite a nova quantidade: ");
            scanf("%d", &quantidade);
            AlterarItens(pedido->inicio_lista, id_pedido_scan, quantidade, inicio_estoque);
            break;
            case 4:
            ConsultarItens(pedido->inicio_lista, pedido->id);
            break;
            case 0:
            printf("Registrando pedido...\n");
            break;
            default:
            printf("Opcao Invalida\n");
        }
    }while(opcao != 0);
}

void ConcluirPedido(Tno_fila **inicio, Tno_listaE *inicio_estoque){
    Tno_fila *aux;
    aux = *inicio;
    if(aux != NULL){
        *inicio = (*inicio)->prox;
        printf("\nO pedido de ID %d foi concluido.\n\nDescricao do pedido encerrado:\n", aux->id);
        ConsultarItens(aux->inicio_lista, aux->id);
        Tno_listaI *percorre;
        percorre = aux->inicio_lista;
        while(percorre != NULL){
            RemoverQuantidadeProduto(inicio_estoque, percorre->id_produto, percorre->quantidade);
            percorre = percorre->prox;            
        }
        // Libera o espaço
        Tno_listaI *aux2, *aux3;
        aux2 = aux->inicio_lista;
        while(aux2 != NULL){
            aux3 = aux2->prox;
            free(aux2);
            aux2 = aux3;
        }
        free(aux);
    }else{
        printf("Nao ha pedidos na fila.\n\n");
    }
}

int main(){
    Tno_fila *inicio_fila;
    Tno_listaE *inicio_estoque;
    inicio_fila = NULL;
    inicio_estoque = NULL;
    int opcao, id_pedido_scan, quantidade;
    char tam;
    do{
        printf("\n*****TRABALHO DE ESTRUTURA DE DADOS*****\n");
        printf("1 - Inserir Pedido na Fila\n2 - Concluir Pedido\n3 - Alterar Pedido\n4 - Consultar Fila de Pedidos\n\n5 - Cadastrar Novo Produto\n6 - Inserir Quantidade de um Produto\n7 - Remover Quantidade de Um Produto\n8 - Excluir Cadastro de Produto\n9 - Visualizar Estoque\n\n0 - Finalizar Programa\n");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
            InserirPedido(&inicio_fila, inicio_estoque);
            break;
            case 2:
            ConcluirPedido(&inicio_fila, inicio_estoque);
            break;
            case 3:
            printf("Qual o ID do pedido a ser alterado? ");
            scanf("%d", &id_pedido_scan);
            AlterarPedido(inicio_fila, id_pedido_scan, inicio_estoque);
            break;
            case 4:
            ConsultarPedidos(inicio_fila);
            break;
            case 5:
            InserirNovoProduto(&inicio_estoque);
            break;
            case 6:
            printf("Qual o ID do produto? ");
            scanf("%d", &id_pedido_scan);
            printf("Qual a quantidade a ser inserida no estoque? ");
            scanf("%d", &quantidade);
            printf("Qual o tamanho dos produtos[P/M/G]? ");
            fflush(stdin);
            scanf("%c", &tam);
            InserirQuantidadeProduto(inicio_estoque, id_pedido_scan, quantidade, tam);
            break;
            case 7:
            printf("Qual o ID do produto? ");
            scanf("%d", &id_pedido_scan);
            printf("Qual a quantidade a ser removida do estoque? ");
            scanf("%d", &quantidade);
            RemoverQuantidadeProduto(inicio_estoque, id_pedido_scan, quantidade);
            break;
            case 8:
            printf("Qual o ID do produto? ");
            scanf("%d", &id_pedido_scan);
            CancelarCadastroProduto(&inicio_estoque, id_pedido_scan);
            break;            
            case 9:
            VisualizarEstoque(inicio_estoque);
            break;
            case 0:
            printf("Finalizando...");
            break;
            default:
            printf("Opcao Invalida\n");
        }
    }while(opcao != 0);

    return 0;
}