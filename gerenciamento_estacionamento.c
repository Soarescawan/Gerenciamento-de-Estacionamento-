/* Código Cawan Soares e Árthur Gandra - Gerenciamento de Estacionamento 
    Versão: 1.0
    Data: 20/12/2025 OBS: Data quando o codigo foi iniciado
    Descrição: Sistema de gerenciamento de estacionamento com cadastro, remoção, listagem e busca de veículos.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h> 




typedef struct Usuario{
    char *nome;
    char *cpf;
    char *telefone;
    char *email;
    char *endereco;
    int veiculos_cadastrados;// essa parte que to implemnentando pra contar quantos veiculos o proprietario tem cadastrado//
    
    
    struct Usuario *prox1;
}Usuario;

// Lista encadeada para armazenar veiculos//
typedef struct Veiculo {
    char *vaga_do_veiculo;
    char *placa; 
    char *modelo;
    char *cor;
    char *tipo_vaga;
    char *hora_entrada;
    char *Data_de_pagamento;
    char *tempo_permanencia;
    char *forma_pagamento;
    char *valor_pago;
    
    Usuario *condutor;
    struct Veiculo *prox; //Ponteiro para o proximo Veiculo que sera adicionado importante pois esse é o ponteiro que aponta para o endereço do proximo veiculo//
} Veiculo;


Usuario *buscar_proprietario(Usuario *lista1, char *cpf);


Usuario *cadastrarCondutor(Usuario *lista);
//busca veiculo pela placa, ponteiro percorre na lista pra achar a placa

void somar_veiculos(Usuario *lista1, char *cpf);


void buscar_veiculo(Veiculo *lista, char *placa);

//função criada para ler umas string ja com a alocação de memoria propria pra ela
char *ler_string();

//reescreve os arquivos "Apaga o arquivo " quando o carro é removido da lista 
void reescrever_arquivo_veiculos(Veiculo *lista);

//faz uma comparação na lista pra ver se algum veiculo ja contem a placa
int Id_existe(Veiculo *lista, char *id_vaga);

//salva os dados no arquivo txt//
void dados_do_veiculo(Veiculo *v);

//Função para limpar buffer do teclado após usar scanf e fgets só cuida pra nao usar depois de ler strings pois fica esperando um enter//
void limpar_buffer();

//Menu do Sistema//
void menu();

int verificar_cpf(Usuario *lista1, char *cpf);

//Busca horario direto do sistema//
char *hora_atual_sistema();

//Busca data direto do sistema//
char *data_do_sistema();

//Seleciona o tempo que o condutor deseja ficar no estacionamento//
char *tempo_permanencia_sistema(); 

//Seleciona a forma de pagamento//
char *forma_pagamento();

//inserir Veiculos na Lista encadeada//
Veiculo *inserir_veiculo(Veiculo *lista,Usuario *lista1);

//Listar Veiculos na Lista encadeada//
void listar_veiculos(Veiculo *lista,Usuario *lista1);

//Remover veiculos da lista 
Veiculo* remover_veiculo(Veiculo *lista, char *placa); 

void reescrever_arquivo_prorpietario(Usuario *lista1);

Usuario *remover_Proprietario(Usuario *lista1, char *cpf);

void dados_do_condutor(Usuario *proprietario);

int main() {

    setlocale(LC_ALL, "PORTUGUESE");// meu notbook tem problema com acento ai tem que colocar essa função//

    Veiculo *lista = NULL;//Ponteiro incializado como nulo, este ponteiro aponta para o nó da lista aa e nao esquece que o ponteiro guarda um endereço de memoria
    Usuario *lista1= NULL;
    int opcao;
    char *placa = NULL;
    char *confirma =NULL;
    char *cpf= NULL;

    do {
        menu();
        printf("\n==== Escolha a opcao desejada ==== \n");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {

            case 1:
            printf(" \n===== Cadastrar Proprietários =====");
            lista1 = cadastrarCondutor(lista1);
                        break;

            case 2:
                printf("\n ======  Cadatrar Veiculos ======");
                lista = inserir_veiculo(lista,lista1);//lista recebe a função inserir veiculo, esta função é chamda para o adicionamento de um veiculo, e lista recebe o endereço de memoria do primeiro veiculo
                break;

            case 3:
                printf("\n===== Digite a placa  =====: ");
                 placa = ler_string();

                

                printf("\nVoce realmente deseja remover este veiculo da Placa %s ? (s/n): ",placa);
                 confirma = ler_string();
                

                      
                if (strcmp(confirma,"n") == 0 || strcmp(confirma, "N") == 0 || strcmp(confirma,"Nao") == 0 || strcmp(confirma ,"NAO") == 0 ) {
                    printf("\nRemocao cancelada.\n");
                     break;

                 }else{
                     printf("\nRemovendo veiculo com placa: %s", placa);
                        lista = remover_veiculo(lista, placa);
                             break;
                        }

            case 4:
            printf("\n ======= Veiculos no Estacionamento ======= \n");
                listar_veiculos(lista,lista1);
                break;

            case 5:
                printf("\n ===== Digite a Placa ===== \n");
                     placa = ler_string();
                         buscar_veiculo(lista, placa);
                break;
            
            
        
        case 6:

            printf("\n ===== Digite o CPF ===== \n");
                cpf = ler_string();
                buscar_proprietario(lista1,cpf);

            break;


        case 7:
            printf("\nInsira o CPF para a exclusao do cadastro do proprietario:");
                 cpf = ler_string();

                

                printf("\nVoce realmente deseja remover este Proprietario? (s/n): ");
                 confirma = ler_string();

                      
                if (strcmp(confirma, "N") == 0 || strcmp(confirma, "n") == 0 || strcmp(confirma, "Nao") == 0 || strcmp(confirma ,"NAO") == 0) {
                    printf("\nRemocao cancelada.\n");
                     break;

                 }else{
                     printf("\nRemovendo Proprietario: \n");
                        lista1 = remover_Proprietario(lista1, cpf);
                    printf("\n--------- Proprietario Removido com sucesso ----------\n");
                             break;
                        }


            case 8:
                  printf("\nEncerrando sistema...\n");
                  break;
    
             default:
                 printf("\nOpcao invalida!\n");

                    }
    
        
    } while (opcao != 8);
    
    return 0;
}

    
char *forma_pagamento() {
    char *metodo_pagamento = NULL;
    int opcao;

    printf("\nEscolha a forma de pagamento:\n");
    printf("1 - Dinheiro\n");
    printf("2 - Cartao de Credito\n");
    printf("3 - Cartao de Debito\n");
    printf("4 - Pix\n");
    printf("Opcao: ");

    scanf("%d", &opcao);
    limpar_buffer();

    switch (opcao) {
        case 1:
           metodo_pagamento = realloc(metodo_pagamento,strlen("Dinheiro")+1);
            strcpy(metodo_pagamento, "Dinheiro");

            break;
        case 2:
            metodo_pagamento = realloc(metodo_pagamento,strlen("credito")+1); 
            strcpy(metodo_pagamento, "Credito");
            break;
        case 3:
             metodo_pagamento = realloc(metodo_pagamento,strlen("Debito")+1);
            strcpy(metodo_pagamento, "Debito");
            break;
        case 4:
             metodo_pagamento = realloc(metodo_pagamento,strlen("Pix")+1);
            strcpy(metodo_pagamento, "Pix");
            break;
        default:
            printf("Opcao invalida. Definindo como 'Nao especificado'.\n");
            printf("\nTente novamente.\n");
            forma_pagamento();
            break;
    }
   

    return metodo_pagamento;
}


void buscar_veiculo(Veiculo *lista, char *placa) { 

        while (lista != NULL) { 
            if (strcmp(lista->placa, placa) == 0) {
                 printf("\n--- Veiculo Encontrado ---\n"); 
                 printf("\nVaga do veiculo: %s",lista->vaga_do_veiculo);
                 printf("\nPlaca: %s", lista->placa);
                 printf("\nModelo: %s", lista->modelo); 
                 printf("\nCor: %s", lista->cor); 
                 printf("\nEntrada: %s", lista->hora_entrada);
                 printf("\nData de Pagamento: %s", lista->Data_de_pagamento);
                 printf("\nTempo de Permanencia: %s", lista->tempo_permanencia);
                 printf("\nForma de Pagamento: %s", lista->forma_pagamento);
                 printf("\nValor pago: %s ", lista->valor_pago);
                 printf("\nTipo de Vaga: %s", lista->tipo_vaga);
                 printf("\nProprietario: %s", lista->condutor->nome);// descobri que podemos acessar os campos da estrutura Usuario atraves do ponteiro condutor que esta dentro da estrutura veiculo//
                 printf("\n-------------------------------\n");
                 


                 return; 
                }
                 
                 lista = lista->prox;
         } 

                 printf("\nVeiculo nao encontrado.\n"); 
    }


Veiculo* remover_veiculo(Veiculo *lista, char *placa) {

    Veiculo *atual = lista;
    Veiculo *anterior = NULL;

    while (atual != NULL) {

        if (strcmp(atual->placa, placa) == 0) {

            if (anterior == NULL)
                lista = atual->prox;
            else
                anterior->prox = atual->prox;

            free(atual->placa);
            free(atual->modelo);
            free(atual->cor);
            free(atual->hora_entrada);
            free(atual->Data_de_pagamento);
            free(atual->tempo_permanencia);
            free(atual->forma_pagamento);
            free(atual->valor_pago);
            free(atual->tipo_vaga);
            free(atual->vaga_do_veiculo);
            atual->condutor->veiculos_cadastrados--;//decrementa o numero de veiculos cadastrados do proprietario//
            free(atual);
            reescrever_arquivo_veiculos(lista);

            printf("\nVeiculo removido com sucesso!\n");
            return lista;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("\nVeiculo nao encontrado.\n");
    return lista;
}

Veiculo *inserir_veiculo(Veiculo *lista,Usuario *lista1) {

    //novo é um ponteiro que aponta para o novo veiculo que sera adicionado na lista encadeada//

    Veiculo *novo = malloc(sizeof(Veiculo));
    
         

        char *cpf = malloc(12);// aloca memoria para o cpf que o usuario vai digitar//

        printf("\nInsira o cpf do condutor: ");
        cpf = ler_string();

        Usuario *dono = buscar_proprietario(lista1,cpf);
        
        

     
        if(dono == NULL){
            printf("\nEste Proprietario nao existe\n");
            
            free(cpf);
            free(novo);
            return lista;
        }
        

        somar_veiculos(lista1,cpf);
        novo->condutor = dono;

      
        free(cpf);
    
    
            do{

                printf("\n Tipo de Vaga (Carro/Moto): ");
                    novo->tipo_vaga = ler_string();



                if((strcmp(novo->tipo_vaga,"Carro")== 0) || (strcmp(novo->tipo_vaga,"carro")==0)){
                    
                    printf("Vaga de Carro selecionada.\n");
            
                    break;
                    }

                else if((strcmp(novo->tipo_vaga,"Moto")== 0)|| (strcmp(novo->tipo_vaga,"moto")==0) || (strcmp(novo->tipo_vaga,"Motocicleta")==0) || (strcmp(novo->tipo_vaga,"motocicleta")==0) ){
                    printf("Vaga de Motocicleta selecionada.\n");
                    break;
                    }
                    
                    else{


                    printf("Tipo de Vaga invalido. Por favor, digite 'Carro' ou 'Moto'.\n");
                    free(novo->tipo_vaga);
                        free(novo);

                
            }
            }while(1);
        

        do{
            int vaga_valida ;

            printf("\nvaga do Veiculo: ");
            novo->vaga_do_veiculo = ler_string();

            if(novo->vaga_do_veiculo == NULL){
                printf("\nErro ao ler vaga do veiculo.\n");
                free(novo->vaga_do_veiculo);
                continue;
            }

            vaga_valida = Id_existe(lista, novo->vaga_do_veiculo);

            if(vaga_valida == 1){
                free(novo->vaga_do_veiculo);
                   printf("\nErro:  Vaga do veiculo já esta em uso.\n");
                   printf("\nPor favor, insira uma vaga diferente.\n");
                continue;
            }else{
                printf("\nVaga do veiculo cadastrada com sucesso.\n");
                break;
            }
        

        }while(1);


            do{
                                 printf("\nPlaca: ");
                    novo->placa = ler_string();

                if(strlen(novo->placa) < 7 || strlen(novo->placa) > 8 || novo->placa == NULL ){
                        printf("\nErro: Placa deve conter entre 0 a 7 caracteres.\n");

                            if(novo->placa != NULL)
                                free(novo->placa);
                }

            }while(strlen(novo->placa) < 7 || strlen(novo->placa) > 8 || novo->placa == NULL);
        


            printf("\nModelo: ");
            novo->modelo = ler_string();

            printf("\nCor: ");
            novo->cor = ler_string();

            novo->hora_entrada = hora_atual_sistema();
            novo->Data_de_pagamento = data_do_sistema();


        
            
            novo->valor_pago = tempo_permanencia_sistema();
        

            while (1)
            {
                
                novo->tempo_permanencia = NULL;
            
            if(strcmp(novo->valor_pago, "R$50,00") == 0){
                    printf("\nDiaria selecionada.\n");

                        novo->tempo_permanencia = realloc(novo->tempo_permanencia,strlen( "Diaria")+1);

                        strcpy(novo->tempo_permanencia, "Diaria");
                        break;

                }else if(strcmp(novo->valor_pago, "R$150,00") == 0){
                    printf("\nMensal selecionada.\n");
                        novo->tempo_permanencia = realloc(novo->tempo_permanencia,strlen("Mensal")+1);
                        strcpy(novo->tempo_permanencia, "Mensal");
                        break;

                }else if(strcmp(novo->valor_pago, "R$30,00") == 0){
                    printf("\nPernoite selecionada.\n");
                        novo->tempo_permanencia =  realloc(novo->tempo_permanencia,strlen("Pernoite")+1);
                        strcpy(novo->tempo_permanencia, "Pernoite");
                        break;
                }
                break;
            }
            

            printf("\nForma de Pagamento:\n ");
            novo->forma_pagamento = forma_pagamento();
        
        
        
            dados_do_veiculo(novo);
        

            /* Insere no início da lista */
            novo->prox = lista;// essa parte é importante pq ela ta fazendo o ponteiro do novo veiculo apontar para o inicio da lista encadeada//
            lista = novo;

            printf("\nVeiculo cadastrado com sucesso!\n");
            return lista;
        }

void listar_veiculos(Veiculo *lista,Usuario *lista1) {

        if (lista == NULL) {
            printf("\nNenhum veiculo cadastrado.\n");
            return;
        }

    printf("\n--- Veiculos no Estacionamento ---\n");

    while (lista != NULL && lista1 !=NULL) {
        printf("\nVaga do Veiculo: %s", lista->vaga_do_veiculo);
        printf("\nPlaca: %s", lista->placa);
        printf("\nModelo: %s", lista->modelo);
        printf("\nCor: %s", lista->cor);
        printf("\nEntrada: %s", lista->hora_entrada);
        printf("\nData de Pagamento: %s", lista->Data_de_pagamento);
        printf("\nTempo de Permanencia: %s", lista->tempo_permanencia);
        printf("\nForma de Pagamento: %s", lista->forma_pagamento);
        printf("\nValor pago: %s ", lista->valor_pago);
        printf("\nTipo de Vaga: %s", lista->tipo_vaga);
        printf("\nProprietario: %s", lista1->nome);
        printf("\n-------------------------------\n");
        lista = lista->prox;
        lista1=lista1->prox1;
    }

    while (lista != NULL) {
    printf("\nNO: %p", (void*)lista);
    printf("\nPROX: %p", (void*)lista->prox);

    printf("\nPlaca: %s", lista->placa);
    printf("\n--------------------------\n");

    lista = lista->prox;
     lista1=lista1->prox1;
}

}

void dados_do_veiculo(Veiculo *v) {

    FILE *file = fopen("Estacionamento.txt", "a");
    if (!file) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fprintf(file, "\n---- Dados do Veiculo ----\n");
    fprintf(file, "ID da Vaga: %s\n", v->vaga_do_veiculo);
    fprintf(file, "Placa: %s\n", v->placa);
    fprintf(file, "Modelo: %s\n", v->modelo);
    fprintf(file, "Cor: %s\n", v->cor);
    fprintf(file, "Tipo de Vaga: %s\n", v->tipo_vaga);
    fprintf(file, "Hora Entrada: %s\n", v->hora_entrada);
    fprintf(file, "Data do Pagamento: %s\n", v->Data_de_pagamento);
    fprintf(file, "Tempo Permanencia: %s\n", v->tempo_permanencia);
    fprintf(file, "Forma Pagamento: %s\n", v->forma_pagamento);
    fprintf(file, "Valor Pago: %s\n", v->valor_pago);
    fprintf(file, "Proprietario: %s\n", v->condutor->nome);

   
   // fprintf(file, "CPF: %s\n", v->cpf);
    fprintf(file, "--------------------------\n");
    printf("\n");

    fclose(file);
}

char *tempo_permanencia_sistema() {
    

        int opcao;
        char *valor = NULL;

    while(1){
        printf("\n====== Escolha o tempo de permanencia ======\n");
        printf("1  - Diaria: 24 horas Valor: R$50,00\n");
        printf("2  - Mensal: 30 dias Valor: R$150,00\n");
        printf("3  - Pernoite: 12 horas Valor: R$30,00\n");

        printf("\nInsira o tempo de permanencia desejado: ");
        scanf("%d", &opcao);
        
        limpar_buffer();

        switch (opcao)
        {
        case 1:
        printf("\nDiaria selecionada.\n");
        printf("\nO valor da diaria e R$ 50,00\n");
        valor = realloc(valor,strlen("R$50,00")+1);
        strcpy(valor, "R$50,00");
    
        return  valor;
            
           
        case 2:
        printf("\nMensal selecionada.\n");
        printf("\nO valor do mensal e R$ 150,00\n");
            valor = realloc(valor,strlen("R$150,00")+1);
            strcpy(valor, "R$150,00");
            return valor;
           
        case 3:
        printf("\nPernoite selecionada.\n");
        printf("\nO valor do pernoite e R$ 30,00\n");
        
            valor = realloc(valor,strlen("R$30,00")+1);
            strcpy(valor, "R$30,00");
            return valor;
           
        
        default:
        printf("\nTempo invalido\n");
        printf("\n Tente novamente.\n");
            
        }
    }
    
}

char *data_do_sistema() {

    // essa função é igual a da hora mudando somente a saida
    time_t agora;
    struct tm *tempo;
    char *data = malloc(20); // Dia/Mes/Ano + '\0' nao esquece que o /0 é o que indica uma string em C//

    time(&agora);
    tempo = localtime(&agora);

    sprintf(data, "%02d/%02d/%04d", tempo->tm_mday, tempo->tm_mon + 1, tempo->tm_year + 1900);

    return data;
}

char *hora_atual_sistema() {

    // essa funcao pega a hora do sistema e retorna uma string com o formato HH:MM //
    time_t agora;// tipo de dado que armazena o tempo em segundos desde 1 de janeiro de 1970 cara isso é tipo um long long int é muito grande//
    struct tm *tempo;// estrutura que armazena a data e hora local do proprio pc ja vem na função time//
    char *hora = malloc(20); // HH:MM + '\0' aqui ja aloca direto pois sabemos o que vamos usar
    

    time(&agora);// função que pega o tempo atual do sistema e armazena na variavel agora//
    tempo = localtime(&agora);// tempo recebe a hora local convertida da variavel agora, tempo tem que ser um ponteiro por que a localtime retorna um ponteiro//

    sprintf(hora, "%02d:%02d", tempo->tm_hour, tempo->tm_min);

    return hora;
}

int Id_existe(Veiculo *lista, char *id_vaga) {

    Veiculo *atual = lista;//atual ponteiro auxiliar que percorrem a lista//

    while (atual != NULL) {
        if (strcmp(atual->vaga_do_veiculo, id_vaga) == 0) { //compara o id da vaga do veiculo atual com o id que o usuario quer cadastrar//
        
            return 1; // ID existe
        }
        atual = atual->prox;

    }
    return 0; // ID não existe
}

void reescrever_arquivo_veiculos(Veiculo *lista) {
    FILE *file = fopen("Estacionamento.txt", "w");// esse w apaga o arquivo antigo e cria um novo com os campos vazios//
    if (!file) {
        printf("Erro ao reescrever arquivo.\n");
        return;
    }

    while (lista != NULL) {
        fprintf(file, "\n---- Dados do Veiculo ----\n");
        fprintf(file, "Vaga do Veiculo: %s\n", lista->vaga_do_veiculo);
        fprintf(file, "Placa: %s\n", lista->placa);
        fprintf(file, "Modelo: %s\n", lista->modelo);
        fprintf(file, "Cor: %s\n", lista->cor);
        fprintf(file, "Tipo de Vaga: %s\n", lista->tipo_vaga);
        fprintf(file, "Hora Entrada: %s\n", lista->hora_entrada);
        fprintf(file, "Data do Pagamento: %s\n", lista->Data_de_pagamento);
        fprintf(file, "Tempo Permanencia: %s\n", lista->tempo_permanencia);
        fprintf(file, "Forma Pagamento: %s\n", lista->forma_pagamento);
        fprintf(file, "Valor Pago: %s\n", lista->valor_pago);

       
       
        fprintf(file, "--------------------------\n");

        lista = lista->prox;
    }

    fclose(file);
}

char *ler_string() {
    char memoria[256];// criei um buffer temporario para ler a entrada do usuario, nao tem por que criar uma alocação dinamica pois o buffer é excluido apos o fim da função//
    
    if (fgets(memoria, sizeof(memoria), stdin) == NULL){//se o usuario nao digitar nada retorna NULL//
        return NULL;}
        
        
        memoria[strcspn(memoria, "\n")] = '\0'; // remove o ENTER e coloca o caractere de fim de string '\0'//
        char *palavra = malloc(strlen(memoria) + 1);// aloca memoria dinamicamente para a string final//
     // a função strcspn retorna o índice do primeiro caractere encontrado na segunda string que está presente na primeira string. Nesse caso, ela encontra a posição do caractere de nova linha '\n' na string buffer.//
    if (palavra == NULL)
        return NULL;

    strcpy(palavra, memoria);// copia o conteudo do buffer para a string alocada dinamicamente//

    return palavra;// retorna ja a palavra digitada pelo usuario com a memoria certinho//

    //AA te explicano o resto do lixo que ficou morre junto com a função//
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    /*explicando essa funcao cara andei pesquisando e o getchar() limpa somente 1 elemento ja esse enquanto tiverno buffer
    ela limpa so para quadno acha um \n ou fim do arquivo*/
}


Usuario *cadastrarCondutor(Usuario *lista1){

     Usuario *novo = malloc(sizeof(Usuario)); // Alocação dinamica de memoria para o novo proprietario(ou nó da lista)//

     novo->veiculos_cadastrados = 0; // inicializa o contador de veiculos cadastrados para o novo proprietario//
        int contador =0;

     int chances =3;
     printf("\nInsira o nome do Condutor:  ");
     novo->nome = ler_string();
     //novo é um ponteiro que aponta para a lista usuarios e o nome é um campo da estrutura//
        do {
        printf("\nInsira o CPF do Proprietario: ");
        novo->cpf = ler_string();

        if(contador == 2){
            printf("\nNumero maximo de tentativas atingido. Retornando ao menu principal.\n");
            free(novo->cpf);
            free(novo);
            return lista1;
        }

        if (novo->cpf == NULL) {
            printf("Erro ao ler CPF.\n");
            continue;
        }

        if (strlen(novo->cpf) != 11) {
            printf("Erro: CPF deve conter exatamente 11 numeros.\n");
            free(novo->cpf);
            novo->cpf = NULL;
            continue;
        }
        
        if (verificar_cpf(lista1, novo->cpf)) {
            printf("Erro: CPF ja cadastrado \n");
            printf("Voce tera %d chances para digitar um CPF valido.\n",chances -1);
            free(novo->cpf);
            novo->cpf = NULL;
            chances--;
            contador++;
            continue;
        }
        
        break; // CPF valido e unico

    } while (1);

    printf("\nInsira o telefone do Condutor: ");
        novo->telefone = ler_string();

                
               
                    
    printf("\nInsira seu email: ");
        novo->email = ler_string();

    printf("\nInsira seu Endereço: ");
        novo->endereco = ler_string();

     dados_do_condutor(novo);




    novo->prox1 = lista1;

     return novo;


}

        

void menu() {
    
    printf(" \033[32m ==== Gerenciamento de Estacionamento ====\n");//Arthur coloquei essa cor amarela pra ficar mais bonito /033[0m pra cancelar ela dentro do printf//
    printf("1 - Cadastrar Condutor\n");
    printf("2 - Cadastrar Veiculo\n");
    printf("3 - Remover Veiculo\n");
    printf("4 - Listar Veiculos\n");
    printf("5 - Buscar Veiculo pela Placa\n");
    printf("6 - Buscar Dados do Condutor\n");
    printf("7 - Remover Proprietario\n");
    printf("8 - Sair\n");

}

void dados_do_condutor(Usuario *proprietario) {
   // lista passada no parametro da função //
    FILE *file = fopen("Cadastro_de_usuario.txt", "a"); //cria um txt se nao existir//
    if (!file) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    // aqui no crei ponteiro auxilia por que da pra usar o proprio ponteiro passado no parametro//
    fprintf(file, "\n---- Dados do Condutor ----\n");
    fprintf(file, "Nome do Condutor: %s\n", proprietario->nome);
    fprintf(file, "Telefone: %s\n", proprietario->telefone);
    fprintf(file, "Email: %s\n", proprietario->email);
    fprintf(file, "Endereço: %s\n", proprietario->endereco);
    fprintf(file, "Cpf: %s\n", proprietario->cpf);
  
    fprintf(file, "--------------------------\n");
    printf("\n");

    fclose(file);
}


Usuario *buscar_proprietario(Usuario *lista1, char *cpf) { 
 

    //lembrando se quer procurar um usuario vc deve passar tambem o ponteiro que inicia a lista encadeadan na funcao//

        while (lista1 != NULL) { 
            if (strcmp(lista1->cpf, cpf) == 0 && lista1 != NULL) { // faz um teste comparando o cpf do proprietario com o cpf que o usuario quer buscar//
                 printf("\n--- Proprietario Encontrado ---\n"); 
                     printf( "---- Dados do Condutor ----\n");
                     printf( "Nome do Condutor: %s\n", lista1->nome);
                     printf( "Telefone: %s\n", lista1->telefone);
                     printf( "Email: %s\n", lista1->email);
                     printf( "Endereço: %s\n", lista1->endereco);
                     printf( "Cpf: %s\n", lista1->cpf);
                     printf( "Veiculos Cadastrados: %d\n", lista1->veiculos_cadastrados);
                     printf( "--------------------------\n");
                    
              


                 return lista1;//retorna o no encontrado//

                }
                 
                 lista1 = lista1->prox1;//se ele nao for o no procurado ele faz o ponteiro andar na lista encadeada//
         } 

                 printf("\nProprietario nao encontrado.\n"); 
                 return NULL;
}





void reescrever_arquivo_prorpietario(Usuario *lista1) {
            //Pra voce entender essa função ela apaga o arquivo antigo e cria um novo com os dados vazios isso so reescreve //
        
        FILE *file = fopen("Cadastro_de_usuario.txt", "w");
        if (!file) {
            printf("Erro ao abrir arquivo.\n");
            return;
        }
        while(lista1 != NULL){
                fprintf(file, "\n---- Dados do Condutor ----\n");
                fprintf(file, "Nome do Condutor: %s\n", lista1->nome);
                fprintf(file, "Telefone: %s\n", lista1->telefone);
                fprintf(file, "Email: %s\n", lista1->email);
                fprintf(file, "Endereço: %s\n", lista1->endereco);
                fprintf(file, "Cpf: %s\n",lista1->cpf);
            
                fprintf(file, "\n--------------------------\n");
                printf("\n");
                lista1 = lista1->prox1;// aqui ele percorre a lista encadeada esse ponteiro lista1 recebe o endereço do proximo nó//
                //essa interação de ponteiros é o que faz a lista encadeada funcionar//
            }
        fclose(file);
}

 Usuario *remover_Proprietario(Usuario *lista1, char *cpf){
     Usuario *atual = lista1;//ponteiro que percorre a lista ele comeca apontando pro primeiro proprietario da lista//
     Usuario *anterior = NULL;// ponteiro que aponta pro proprietario anterior ao atual//

   // sao ponteiros auxiliares para ajudar na remoção do nó (usuario) correto da lista encadeada//

    while (atual != NULL) {// aqui resumindo ele percorre a lista enquanto o ponteiro atual nao for nulo//

        if (strcmp(atual->cpf, cpf) == 0) {// faz comparacão do cpf do proprietario atual com o cpf que o usuario quer remover//

            if (anterior == NULL)
                lista1 = atual->prox1;// se o proprietario a ser removido for o primeiro da lista, a lista aponta pro proximo proprietario//
            else
                anterior->prox1 = atual->prox1;//aqui ele faz o ponteiro do proprietario anterior apontar pro proximo proprietario do atual//

            free(atual->cpf);
            free(atual->email);
            free(atual->endereco);
            free(atual->telefone);
            free(atual->nome);
            
            free(atual);
             
            reescrever_arquivo_prorpietario(lista1);

            printf("\nProprietario removido com sucesso!\n");
            return lista1;// retorna o ponteiro da lista atualizada//
        }

        anterior = atual;// faz o ponteiro anterior apontar pro proprietario atual//
        atual = atual->prox1;// faz o ponteiro atual andar na lista encadeada e procurar nos proximos proprietarios(ou nós)//
    }

    printf("\nProprietario nao encontrado.\n");
    return lista1;
}


int verificar_cpf(Usuario *lista1, char *cpf) {
    Usuario *atual = lista1;//aponta pro endereço do primeiro usuario da lista//

    // caso nao entender me pergunte//

    while (atual != NULL) { // percorre a lista enquanto o ponteiro atual nao for nulo//
        if (strcmp(atual->cpf, cpf) == 0) {
            return 1; // CPF existe ele retorna 1 coloquei um teste la na funcao cadastrar condutor pra saber se o cpf ja existe//
        }
        atual = atual->prox1;//faz o ponteiro andar na lista encadeada e procura nos proximos usuarios(ou nós)//
    }
    return 0; // cpf não existe
}

void somar_veiculos(Usuario *lista1, char *cpf) {
    while (lista1 != NULL) {
        if (strcmp(lista1->cpf, cpf) == 0) {
            lista1->veiculos_cadastrados++;
            return;
        }
        lista1 = lista1->prox1;
    }
}

   
/* Dificuldades existentes cawan

pensar em como um estacionamento funciona na vida real
umas da maxima difuculdades foi a data e hora que vem direto do sistema
conseguir fazer a lista encadeada funcionar corretamente
integrar duas listas encadeadas (veiculos e proprietarios)
por que integrar duas listas encadeadas? porque um veiculo tem um proprietario e um proprietario pode ter varios veiculos
gerenciar a memoria dinamica corretamente para evitar vazamentos
tratar buffers
gerenciar o codigo grande 
trabalhar usando ponteiros para estruturas

conforme for ajustado eu vou atualizando aqui



*/
