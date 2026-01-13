/* Código Cawan Soares e Árthur Gandra - Gerenciamento de Estacionamento 
    Versão: 1.0
    Data: 20/12/2025 OBS: Data quando o codigo foi iniciado
    Descrição: Sistema de gerenciamento de estacionamento com cadastro, remoção, listagem e busca de veículos.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>// Usamos para colocar acento nas palavras em portugues pois o linux mint tem problema com acento//
#include <time.h> // Usamos função para buscar hora e data do sistema 



// Lista encadeada para Proprietario dos veiculos//
typedef struct Usuario{
    char *nome;
    char *cpf;
    char *telefone;
    char *email;
    char *endereco;
    
    
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

int verificar_cpf(Usuario *lista, char *cpf);

//Busca horario direto do sistema//
char *hora_atual_sistema();

//Busca data direto do sistema//
char *data_do_sistema();

//Seleciona o tempo que o condutor deseja ficar no estacionamento//
char *tempo_permanencia_sistema(); 

//Seleciona a forma de pagamento//
char *dados_de_pagamento(Veiculo *v);

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
                

                      
                if (strcmp(confirma,"s") == 0 || strcmp(confirma, "S") == 0 || strcmp(confirma,"Sim") == 0 || strcmp(confirma ,"SIM") == 0 ) {
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
                     printf("\nRemovendo Proprietario: ");
                        lista1 = remover_Proprietario(lista1, cpf);
                    printf("--------- Proprietario Removido com sucesso ----------");
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

    
char *dados_de_pagamento(Veiculo *v) {
    char *metodo = NULL;
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
            metodo = malloc(9);
            strcpy(metodo, "Dinheiro");
            
            break;
        case 2:
            metodo = malloc(18);
            strcpy(metodo, "Credito");
            break;
        case 3:
            metodo = malloc(16);
            strcpy(metodo, "Debito");
            break;
        case 4:
            metodo = malloc(4);
            strcpy(metodo, "Pix");
            break;
        default:
            printf("Opcao invalida. Definindo como 'Nao especificado'.\n");
            metodo = malloc(18);
            strcpy(metodo, "Nao especificado");
            break;
    }
   

    return metodo;
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
    //Alocaçao dinamica de memoria para o novo veiculo, aqui eu aloquei pra toda a estrutura //
    Veiculo *novo = malloc(sizeof(Veiculo));
    Usuario *dono = NULL;

    char *cpf;
     printf("\nInsira O cpf do condutor: ");
    cpf = ler_string();

   dono = buscar_proprietario(lista1,cpf);

   if(dono == NULL){
    printf("\nEste Proprietario nao existe");

    free(cpf);
    free(novo);
    return lista;
   }

    printf("\nProprietario encontrado");

    novo->condutor = dono;

   
    free(cpf);
   
   






    do{

        printf("\n Tipo de Vaga (Carro/Motocicleta): ");
             novo->tipo_vaga = ler_string();



        if((!strcmp(novo->tipo_vaga,"Carro")) || (!strcmp(novo->tipo_vaga,"carro")) || ((novo->tipo_vaga != NULL))){
            printf("Vaga de Carro selecionada.\n");
      
             break;
            }

        else if((!strcmp(novo->tipo_vaga,"Moto"))|| (!strcmp(novo->tipo_vaga,"moto")) || (!strcmp(novo->tipo_vaga,"Motocicleta")) || (!strcmp(novo->tipo_vaga,"motocicleta")) || (novo->tipo_vaga != NULL)){
            printf("Vaga de Motocicleta selecionada.\n");
             break;
            }
            
            else{


              printf("Tipo de Vaga invalido. Por favor, digite 'Carro' ou 'Motocicleta'.\n");
              free(novo->tipo_vaga);
                 free(novo);

        
    }
}while(1);
  

do{
    printf("vaga do Veiculo: ");
    novo->vaga_do_veiculo = ler_string();
   

}while(Id_existe(lista, novo->vaga_do_veiculo));


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


   
    printf("\nPara sabaer o valor a pagar, escolha o tempo de permanencia:\n");
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
    novo->forma_pagamento = dados_de_pagamento(novo);
  
   
   
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

   
   // fprintf(file, "CPF: %s\n", v->cpf);
    fprintf(file, "--------------------------\n");
    printf("\n");

    fclose(file);
}

char *tempo_permanencia_sistema() {
    char *tempo = malloc(20);

    int opcao;

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
        strcpy(tempo, "R$50,00");
        return tempo;
        
        break;
    case 2:
    printf("\nMensal selecionada.\n");
    printf("\nO valor do mensal e R$ 150,00\n");
        strcpy(tempo, "R$150,00");
        return tempo;
        break;
    case 3:
    printf("\nPernoite selecionada.\n");
    printf("\nO valor do pernoite e R$ 30,00\n");
        strcpy(tempo, "R$30,00");
        return tempo;
        break;
    
    default:
     printf("\nTempo invalido\n");
     printf("\n Tente novamente.\n");

        continue;
       
    }
}

char *data_do_sistema() {
    time_t agora;
    struct tm *tempo;
    char *data = malloc(11); // DD/MM/YYYY + '\0'
    

    time(&agora);
    tempo = localtime(&agora);

    sprintf(data, "%02d/%02d/%04d", tempo->tm_mday, tempo->tm_mon + 1, tempo->tm_year + 1900);

    return data;
}

char *hora_atual_sistema() {
    time_t agora;
    struct tm *tempo;
    char *hora = malloc(6); // HH:MM + '\0'
    

    time(&agora);
    tempo = localtime(&agora);

    sprintf(hora, "%02d:%02d", tempo->tm_hour, tempo->tm_min);

    return hora;
}

int Id_existe(Veiculo *lista, char *id_vaga) {
    Veiculo *atual = lista;
        while (atual != NULL) {
            if (strcmp(atual->vaga_do_veiculo, id_vaga) == 0) {
                return 1; // ID existe
            }
            atual = atual->prox;
        }
    return 0; // ID não existe
}

void reescrever_arquivo_veiculos(Veiculo *lista) {
    FILE *file = fopen("Estacionamento.txt", "w");
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
    char buffer[256];// criei um buffer temporario para ler a entrada do usuario//

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)//se o usuario nao digitar nada retorna NULL//
        return NULL;

    buffer[strcspn(buffer, "\n")] = '\0'; // remove o ENTER e coloca o caractere de fim de string '\0'//
     // a função strcspn retorna o índice do primeiro caractere encontrado na segunda string que está presente na primeira string. Nesse caso, ela encontra a posição do caractere de nova linha '\n' na string buffer.//
    char *str = malloc(strlen(buffer) + 1);// aloca memoria dinamicamente para a string final//
    if (str == NULL)
        return NULL;

    strcpy(str, buffer);// copia o conteudo do buffer para a string alocada dinamicamente//
    return str;
}

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
}


Usuario *cadastrarCondutor(Usuario *lista){

    Usuario *novo = malloc(sizeof(Usuario));
    
    printf("\nInsira o nome do Condutor:  ");
    novo->nome = ler_string();

    
    do {
        //Nessa parte o codigo faz uma verificação para garantir que o CPF tenha exatamente 11 números//
        printf("\nInsira o CPF do Proprietario: ");
        novo->cpf = ler_string();
        int verificar = verificar_cpf(lista, novo->cpf);
        //strlen conta a quantidade de caracteres na string e faz o teste se o usuario apertar enter tambem da erro//
                    if(verificar == 1){
                        printf("Erro: CPF já cadastrado.\n");
                        printf("Por favor, insira um CPF diferente ou verifique em buscar proprietario.\n");
                        
                        int opcao;
                        printf("Deseja tentar novamente? (1 - Sim / 2 - Nao): ");
                        scanf("%d", &opcao);
                        limpar_buffer();    
                                switch(opcao){
                                    case 1:
                                    printf("Tente novamente.\n");
                                    free(novo->cpf);

                                        break;
                                    case 2:
                                        free(novo->cpf);
                                        free(novo);
                                        return lista;
                                    default:
                                        printf("Opcao invalida. Cancelando cadastro.\n");
                                        free(novo->cpf);
                                        free(novo);
                                        return lista;
                                }
                        free(novo->cpf);
                        continue;
                    }

        if(verificar == 0){
            if (strlen(novo->cpf) != 11 || novo->cpf == NULL ){
                 printf("Erro: CPF deve conter exatamente 11 números.\n");

            if(novo->cpf != NULL)
            free(novo->cpf);
            
            }
        }
     } while (strlen(novo->cpf) != 11 || novo->cpf == NULL);



     do{
        printf("\nInsira o numero do telefone: ");
            novo->telefone =ler_string();

            if(strlen(novo->telefone )!= 11|| novo->telefone == NULL){
            printf("Erro: Celular deve conter exatamente 9 números + 2 numeros do DDD.\n");

            if(novo->telefone != NULL)
                    free(novo->telefone);

                }
                
                
            }while(strlen(novo->telefone) !=11 || novo->telefone == NULL );
            
        
        printf("\nInsira seu email: ");
        novo->email = ler_string();

        printf("\nInsira seu Endereço: ");
        novo->endereco = ler_string();

        dados_do_condutor(novo);




    novo->prox1 = lista;

    return novo;


}

void menu() {
    
    printf(" ==== Gerenciamento de Estacionamento ====\n");
    printf("1 - Cadastrar Condutor\n");
    printf("2 - Cadastrar Veiculo\n");
    printf("3 - Remover Veiculo\n");
    printf("4 - Listar Veiculos\n");
    printf("5 - Buscar Veiculo pela Placa\n");
    printf("6 - Buscar Dados do Condutor\n");
    printf("7 - Remover Proprietario\n");

    printf("8- Sair\n");
}

void dados_do_condutor(Usuario *proprietario) {

    FILE *file = fopen("Cadastro_de_usuario.txt", "a");
    if (!file) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

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


    

        while (lista1 != NULL) { 
            if (strcmp(lista1->cpf, cpf) == 0 && lista1 != NULL) {
                 printf("\n--- Proprietario Encontrado ---\n"); 
                     printf( "\n---- Dados do Condutor ----\n");
                     printf( "\nNome do Condutor: %s\n", lista1->nome);
                     printf( "\nTelefone: %s\n", lista1->telefone);
                     printf( "\nEmail: %s\n", lista1->email);
                     printf( "\nEndereço: %s\n", lista1->endereco);
                     printf( "\nCpf: %s\n", lista1->cpf);
  
                    printf( "\n--------------------------\n");
                    printf("\n");

                


                 return lista1;
; 
                }
                 
                 lista1 = lista1->prox1;
         } 

                 printf("\nProprietario nao encontrado.\n"); 
                 return NULL;
    }




    void reescrever_arquivo_prorpietario(Usuario *lista1) {
    
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
            lista1 = lista1->prox1;
    }
    fclose(file);
}

 Usuario *remover_Proprietario(Usuario *lista1, char *cpf){
     Usuario *atual = lista1;
     Usuario *anterior = NULL;

    while (atual != NULL) {

        if (strcmp(atual->cpf, cpf) == 0) {

            if (anterior == NULL)
                lista1 = atual->prox1;
            else
                anterior->prox1 = atual->prox1;

            free(atual->cpf);
            free(atual->email);
            free(atual->endereco);
            free(atual->telefone);
            free(atual->nome);
            
            free(atual);
            reescrever_arquivo_prorpietario(lista1);

            printf("\nProprietario removido com sucesso!\n");
            return lista1;
        }

        anterior = atual;
        atual = atual->prox1;
    }

    printf("\nProprietario nao encontrado.\n");
    return lista1;
}

 
int verificar_cpf(Usuario *lista, char *cpf) {
    Usuario *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->cpf, cpf) == 0) {
            return 1; // CPF existe
        }
        atual = atual->prox1;
    }
    return 0; // cpf não existe
}
