#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

// Estrutura para armazenar dados do usuário
typedef struct {
    char nome[100];
    int idade;
    int cpf;
    int cep;
    char nacionalidade[30];
    char email[100];
    int status;
} Usuario;

// Estrutura para armazenar dados do pedido
typedef struct {
    int numero_pedido;
    int preco;
    int usuario_cpf;
} Pedido;

// Declaração da função de comparação
int compararPedidos(const void *a, const void *b);

// Função para validar CPF
int validarCPF(int cpf) {
    return cpf > 0;
}

// Função para comparar pedidos
int compararPedidos(const void *a, const void *b) {
    const Pedido *pedidoA = (const Pedido *)a;
    const Pedido *pedidoB = (const Pedido *)b;
    return pedidoA->numero_pedido - pedidoB->numero_pedido;
}

// Função para buscar pedido por número
Pedido *buscarPedidoPorNumero(Pedido *pedidos, int quantidade, int numero_pedido) {
    Pedido chave = {.numero_pedido = numero_pedido};
    return bsearch(&chave, pedidos, quantidade, sizeof(Pedido), compararPedidos);
}

// Função para ordenar pedidos
void ordenarPedidos(Pedido *pedidos, int quantidade) {
    qsort(pedidos, quantidade, sizeof(Pedido), compararPedidos);
}

// Função para escrever dados do usuário em um arquivo
void escreverUsuarioNoArquivo(Usuario usuario) {
    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(arquivo, "%s,%d,%d,%d,%s,%s,%d\n", usuario.nome, usuario.idade, usuario.cpf, usuario.cep, usuario.nacionalidade, usuario.email, usuario.status);
    fclose(arquivo);
}

// Função para cadastrar usuário
void cadastrarUsuario() {
    Usuario usuario;
    
    printf("Qual seu nome? ");
    scanf("%99s", usuario.nome);
    usuario.status = 1; // Ativo por padrão
    
    printf("Qual sua idade? ");
    scanf("%d", &usuario.idade);
    
    printf("Qual seu CPF? ");
    scanf("%d", &usuario.cpf);
    
    if (!validarCPF(usuario.cpf)) {
        printf("CPF invalido.\n");
        return;
    }
    
    printf("Qual seu CEP? ");
    scanf("%d", &usuario.cep);
    
    printf("Qual sua nacionalidade? ");
    scanf("%29s", usuario.nacionalidade);
    
    printf("Qual seu email? ");
    scanf("%99s", usuario.email);
    
    escreverUsuarioNoArquivo(usuario);
    
    printf("Cadastro realizado com sucesso!\n");
}

// Função para consultar usuário por CPF
void consultarUsuarioPorCPF(int cpf) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    Usuario usuario;
    int encontrado = 0;
    while (fscanf(arquivo, "%99[^,],%d,%d,%d,%29[^,],%99[^\n]\n", usuario.nome, &usuario.idade, &usuario.cpf, &usuario.cep, usuario.nacionalidade, usuario.email) != EOF) {
        if (usuario.cpf == cpf) {
            printf("Nome: %s\nIdade: %d\nCPF: %d\nCEP: %d\nNacionalidade: %s\nEmail: %s\n", usuario.nome, usuario.idade, usuario.cpf, usuario.cep, usuario.nacionalidade, usuario.email);
            encontrado = 1;
            break;
        }
    }
    
    if (!encontrado) {
        printf("Usuario com CPF %d nao encontrado.\n", cpf);
    }
    
    fclose(arquivo);
}

int consultarUsuarioPorCPF2(int cpf) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }
    
    Usuario usuario;
    while (fscanf(arquivo, "%99[^,],%d,%d,%d,%29[^,],%99[^\n]\n", usuario.nome, &usuario.idade, &usuario.cpf, &usuario.cep, usuario.nacionalidade, usuario.email) != EOF) {
        if (usuario.cpf == cpf) {
            fclose(arquivo);
            return 0; // Usuário encontrado
        }
    }
    
    fclose(arquivo);
    return -1; // Usuário não encontrado
}

// Função para consultar usuário por nome
void consultarUsuarioPorNome(char *nomeConsulta) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    
    Usuario usuario;
    int encontrado = 0;
    while (fscanf(arquivo, "%99[^,],%d,%d,%d,%29[^,],%99[^\n]\n", usuario.nome, &usuario.idade, &usuario.cpf, &usuario.cep, usuario.nacionalidade, usuario.email) != EOF) {
        if (strcmp(usuario.nome, nomeConsulta) == 0) {
            printf("Nome: %s\nIdade: %d\nCPF: %d\nCEP: %d\nNacionalidade: %s\nEmail: %s\n", usuario.nome, usuario.idade, usuario.cpf, usuario.cep, usuario.nacionalidade, usuario.email);
            encontrado = 1;
        }
    }
    
    if (!encontrado) {
        printf("Usuario com nome %s nao encontrado.\n", nomeConsulta);
    }
    
    fclose(arquivo);
}

// Função para consultar usuário
void consultarUsuario() {
    int escolha;
    printf("1. Pesquisar por CPF\n2. Pesquisar por nome\nEscolha uma opção: ");
    scanf("%d", &escolha);

    if (escolha == 1) {
        int cpfConsulta;
        printf("Digite o CPF para consultar: ");
        scanf("%d", &cpfConsulta);
        consultarUsuarioPorCPF(cpfConsulta);
    } else if (escolha == 2) {
        char nomeConsulta[100];
        printf("Digite o nome para consultar: ");
        scanf("%99s", nomeConsulta);
        consultarUsuarioPorNome(nomeConsulta);
    } else {
        printf("Opção inválida.\n");
    }
}

// Função para escrever pedido no arquivo
void escreverPedidoNoArquivo(Pedido pedido) {
    FILE *arquivo = fopen("pedidos.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    fprintf(arquivo, "%d,%d,%d\n", pedido.numero_pedido, pedido.preco, pedido.usuario_cpf);
    fclose(arquivo);
}

// Função para pesquisar pedido por número
void pesquisarPedidoPorNumero(int numero_pedido) {
    FILE *arquivo = fopen("pedidos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Pedido pedidos[100];
    int quantidade = 0;
    while (fscanf(arquivo, "%d,%d,%d\n", &pedidos[quantidade].numero_pedido, &pedidos[quantidade].preco, &pedidos[quantidade].usuario_cpf) != EOF) {
        quantidade++;
    }
    fclose(arquivo);

    ordenarPedidos(pedidos, quantidade);

    Pedido *resultado = buscarPedidoPorNumero(pedidos, quantidade, numero_pedido);

    if (resultado != NULL) {
        printf("Pedido encontrado: Número: %d, Preço: %d, CPF do Usuário: %d\n", resultado->numero_pedido, resultado->preco, resultado->usuario_cpf);
    } else {
        printf("Pedido não encontrado.\n");
    }
}

// Função para deletar pedido do arquivo
void deletarPedidoDoArquivo(int numero_pedido) {
    FILE *arquivo = fopen("pedidos.txt", "r");
    FILE *arquivoTemp = fopen("temp.txt", "w");
    if (arquivo == NULL || arquivoTemp == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Pedido pedido;
    while (fscanf(arquivo, "%d,%d,%d\n", &pedido.numero_pedido, &pedido.preco, &pedido.usuario_cpf) != EOF) {
        if (pedido.numero_pedido != numero_pedido) {
            fprintf(arquivoTemp, "%d,%d,%d\n", pedido.numero_pedido, pedido.preco, pedido.usuario_cpf);
        }
    }

    fclose(arquivo);
    fclose(arquivoTemp);
    remove("pedidos.txt");
    rename("temp.txt", "pedidos.txt");
}

// Função para fazer pedido
void fazerPedido() {
    int cpf_alvo;
    printf("Qual CPF do usuário para fazer pedido: ");
    scanf("%d", &cpf_alvo);
    if(consultarUsuarioPorCPF2(cpf_alvo)==-1){
        printf("error");
        return;
    }

    Pedido pedido;
    printf("Numero do pedido: ");
    scanf("%d", &pedido.numero_pedido);
    printf("Preco: ");
    scanf("%d", &pedido.preco);
    pedido.usuario_cpf = cpf_alvo;
    escreverPedidoNoArquivo(pedido);
}

// Função para menu de pedidos
void menuPedidos() {
    int escolha;
    printf("1. Fazer pedido\n2. Pesquisar pedido\n3. Cancelar pedido\nEscolha uma opção: ");
    scanf("%d", &escolha);

    if (escolha == 1) {
        fazerPedido();
     
    } else if (escolha == 2) {
        int numero_pedido;
        printf("Numero do pedido para pesquisar: ");
        scanf("%d", &numero_pedido);
        pesquisarPedidoPorNumero(numero_pedido);
        
    } else if (escolha == 3) {
        int numero_pedido;
        printf("Numero do pedido para cancelar: ");
        scanf("%d", &numero_pedido);
        deletarPedidoDoArquivo(numero_pedido);
       
    } else {
        printf("Opcao invalida.\n");
        
    }
}

// Função para excluir usuário
int excluirUsuario(int cpf_alvo) {
    FILE *arquivo = fopen("usuarios.txt", "r");
    FILE *arquivoTemp = fopen("temp_usuarios.txt", "w");
    if (arquivo == NULL || arquivoTemp == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    Usuario usuario;
    int encontrado = 0;

    while (fscanf(arquivo, "%99[^,],%d,%d,%d,%29[^,],%99[^\n]\n", usuario.nome, &usuario.idade, &usuario.cpf, &usuario.cep, usuario.nacionalidade, usuario.email) != EOF) {
        if (usuario.cpf == cpf_alvo) {
            encontrado = 1;
            continue; // Pular a escrita deste usuário no arquivo temporário
        }
        fprintf(arquivoTemp, "%s,%d,%d,%d,%s,%s,%d\n", usuario.nome, usuario.idade, usuario.cpf, usuario.cep, usuario.nacionalidade, usuario.email, usuario.status);
    }

    fclose(arquivo);
    fclose(arquivoTemp);

    if (!encontrado) {
        printf("Usuario com CPF %d não encontrado.\n", cpf_alvo);
        remove("temp_usuarios.txt");
        return -1;
    }

    // Substituir o arquivo original pelo arquivo temporário
    remove("usuarios.txt");
    rename("temp_usuarios.txt", "usuarios.txt");

    printf("Usuario excluido com sucesso.\n");
    return 0;
}

// Função para desativar usuário
int desativarUsuario(int cpf_alvo) {
    FILE *arquivo = fopen("usuarios.txt", "r+");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    Usuario usuario;
    int encontrado = 0;
    long posicao;

    while (fscanf(arquivo, "%99[^,],%d,%d,%d,%29[^,],%99[^\n]\n", usuario.nome, &usuario.idade, &usuario.cpf, &usuario.cep, usuario.nacionalidade, usuario.email) != EOF) {
        if (usuario.cpf == cpf_alvo) {
            usuario.status = 0; // Definir status como inativo
            posicao = ftell(arquivo) - sizeof(Usuario);
            fseek(arquivo, posicao, SEEK_SET);
            printf("\n-------------------------------------------------\n");
            fprintf(arquivo, "\n%s,%d,%d,%d,%s,%s,%d\n", usuario.nome, usuario.idade, usuario.cpf, usuario.cep, usuario.nacionalidade, usuario.email, usuario.status);
            printf("\n-------------------------------------------------\n");
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);

    if (!encontrado) {
        printf("Usuario com CPF %d não encontrado.\n", cpf_alvo);
        return -1;
    }

    printf("Status do usuario atualizado com sucesso.\n");
    return 0;
}

// Função para editar usuário
void editarUsuario() {
    int escolha;
    int cpf_alvo;

    printf("\nNúmero de CPF para editar: ");
    scanf("%d", &cpf_alvo);
    consultarUsuarioPorCPF(cpf_alvo);

    printf("Qual alteração deseja fazer?\n");
    printf("\n1. Desativar\n2. Excluir\n");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            if (desativarUsuario(cpf_alvo) == 0) {
                printf("Usuário desativado com sucesso.\n");
            }
            break;
        case 2:
            excluirUsuario(cpf_alvo);
            break;
        default:
            printf("Opção inválida.\n");
            break;
    }
}

// Função para menu de usuários
void menuUsuarios() {
    int escolha;
    printf("\n-------------------------------------------------\n");
    printf("\n1. Cadastrar usuario\n2. Pesquisar/Imprimir usuário\n3. Desativar/Excluir usuario\nEscolha uma opcao: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            cadastrarUsuario();
        
            break;
        case 2:
            consultarUsuario();
          
            break;
        case 3:
            editarUsuario();
     
            break;
        default:
            printf("Opção invalida.\n");
            break;
    }
    printf("\n-------------------------------------------------\n");
}

// Função principal
int main() {
    setlocale(LC_ALL, "Portuguese");
    int escolha;
    printf("\n-------------------------------------------------\n");
    printf("1. Usuarios\n2. Pedidos\n3. Sair\nEscolha uma opcao: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1:
            menuUsuarios();
            break;
        case 2:
            menuPedidos();
            break;
        case 3:
            printf("\nSaindo.....\n");
            break;
        default:
            printf("Opçao invalida.\n");
            break;
    }
    printf("\n-------------------------------------------------\n");

    return 0;
}