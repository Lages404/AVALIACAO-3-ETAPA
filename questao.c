#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> 
struct conta
{
    int nconta;
    char titular[50];
    float saldo;
};

int main()
{
    int opcao;
    struct conta c;
    int i;
    int achado;
    float sd;
    char randa[10];
    int buscar;
    int co, cd;
    float v;
    int ao, ad;
    float so, sd2;
    char no[50], nd[50];
    int senhaadm = 1234;
    int t;

    FILE *arquivo;
    srand(time(NULL));

    do{
        printf("\n            MENU          \n");
        printf("|1 - Criar conta-----------|\n");
        printf("|2 - Extrato---------------|\n");
        printf("|3 - Deposito--------------|\n");
        printf("|4 - Saque-----------------|\n");
        printf("|5 - Transferencia---------|\n");
        printf("|6 - Listar todas as contas|\n");
        printf("|7 - Sair------------------|\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1){
            printf("Informe o nome do titular: ");
            scanf("%s", c.titular);

            for (i = 0; i < 9; i++){
                randa[i] = (rand() % 10) + '0';
            }
            randa[9] = '\0';

            c.nconta = atoi(randa);
            c.saldo = 0;

            arquivo = fopen("contas.dat", "ab");
            if (arquivo == NULL){
                printf("Erro ao abrir o arquivo!\n");
            }
            else{
                fwrite(&c, sizeof(struct conta), 1, arquivo);
                fclose(arquivo);

                printf("\nConta criada com sucesso!\n");
                printf("Titular: %s\n", c.titular);
                printf("Numero da conta: %d\n", c.nconta);
                printf("Saldo atual: R$ %.2f\n", c.saldo);
            }
        }
        else if (opcao == 2) {
            achado = 0;
            printf("Informe o numero da conta: ");
            scanf("%d", &buscar);

            arquivo = fopen("contas.dat", "rb");
            if (arquivo == NULL){
                printf("Erro ao abrir o arquivo!\n");
            }
            else{
                while (fread(&c, sizeof(struct conta), 1, arquivo)){
                    if (c.nconta == buscar){
                        achado = 1;
                        printf("\nExtrato da conta %d\n", c.nconta);
                        printf("Titular: %s\n", c.titular);
                        printf("Saldo: R$ %.2f\n", c.saldo);
                        break;
                    }
                }

                if (!achado){
                    printf("Conta nao encontrada.\n");
                }
                fclose(arquivo);
            }
        }
        else if (opcao == 3) {
            printf("Informe o numero da conta: ");
            scanf("%d", &buscar);

            achado = 0;
            arquivo = fopen("contas.dat", "rb+");

            if (arquivo == NULL){
                printf("Erro ao abrir o arquivo\n");
            }
            else{
                while(fread(&c, sizeof(struct conta), 1, arquivo)){
                    if(c.nconta == buscar){
                        achado = 1;

                        printf("Saldo atual: R$ %.2f\n", c.saldo);
                        printf("Informe o valor do deposito: ");
                        scanf("%f", &sd);

                        c.saldo = c.saldo + sd;

                        fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                        fwrite(&c, sizeof(struct conta), 1, arquivo);

                        printf("Deposito realizado! Novo saldo: R$ %.2f\n", c.saldo);
                        break;
                    }
                }

                if (!achado){
                    printf("Conta nao encontrada!\n");
                }

                fclose(arquivo);
            }
        }
        else if(opcao == 4){
            printf("Informe o numero da conta: ");
            scanf("%d", &buscar);

            achado = 0;
            arquivo = fopen("contas.dat", "rb+"); 

            if (arquivo == NULL){
                printf("Erro ao abrir o arquivo ou nenhuma conta existente.\n");
            }
            else{
                while(fread(&c, sizeof(struct conta), 1, arquivo)){
                    if(c.nconta == buscar){
                        achado = 1;

                        printf("Saldo atual: R$ %.2f\n", c.saldo);
                        printf("Informe o valor do saque: ");
                        scanf("%f", &sd);

                        if(sd > c.saldo){
                            printf("Saldo insuficiente para saque!\n");
                        }
                        else{
                            c.saldo = c.saldo - sd;

                            fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                            fwrite(&c, sizeof(struct conta), 1, arquivo);
                            printf("Saque realizado! Novo saldo: R$ %.2f\n", c.saldo);
                        }
                        break;
                    }
                }
                if(!achado) printf("Conta nao encontrada!\n");
                fclose(arquivo);
            }
        }
        else if (opcao == 5){
            arquivo = fopen("contas.dat", "rb+");
            if (arquivo == NULL){
                printf("Nenhuma conta cadastrada!\n");
            }
            else{
                printf("Informe o numero da conta de origem: ");
                scanf("%d", &co);
                printf("Informe o numero da conta de destino: ");
                scanf("%d", &cd);
                printf("Informe o valor da transferencia: ");
                scanf("%f", &v);

                if(co == cd){
                    printf("Nao e possivel transferir para a mesma conta!\n");
                }
                else{
                    ao = 0; 
                    ad = 0; 

                   
                    rewind(arquivo);
                    while(fread(&c, sizeof(struct conta), 1, arquivo)){
                        if(c.nconta == co){
                            if(v > c.saldo){
                                printf("Saldo insuficiente na conta de origem!\n");
                                ao = 0; 
                                break;
                            }
                            ao = 1;
                            so = c.saldo;
                            strcpy(no, c.titular);
                            break;
                        }
                    }

                    
                    if(ao == 1){
                        rewind(arquivo);
                        while(fread(&c, sizeof(struct conta), 1, arquivo))
                        {
                            if(c.nconta == cd){
                                ad = 1;
                                sd2 = c.saldo;
                                strcpy(nd, c.titular);
                                break;
                            }
                        }
                    }
                    else if (ao == 0 && v <= so){
                         printf("Conta de origem nao encontrada!\n");
                    }

                    if(ad == 0 && ao == 1){
                        printf("Conta de destino nao encontrada!\n");
                    }

                    if(ao == 1 && ad == 1){
                        
                        so = so - v;
                        sd2 = sd2 + v;

                        
                        rewind(arquivo);
                        while(fread(&c, sizeof(struct conta), 1, arquivo)){
                            if(c.nconta == co){
                                c.saldo = so;
                                fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                                fwrite(&c, sizeof(struct conta), 1, arquivo);
                                break; 
                            }
                        }

                        
                        rewind(arquivo); 
                        while(fread(&c, sizeof(struct conta), 1, arquivo)){
                            if(c.nconta == cd){
                                c.saldo = sd2;
                                fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                                fwrite(&c, sizeof(struct conta), 1, arquivo);
                                break; 
                            }
                        }

                        printf("Transferencia de R$ %.2f realizada com sucesso!\n", v);
                        printf("Novo saldo origem (%s): R$ %.2f\n", no, so);
                        printf("Novo saldo destino (%s): R$ %.2f\n", nd, sd2);
                    }
                }
                fclose(arquivo);
            }
        }
        else if (opcao == 6){
            printf("Informe a senha de administrador: ");
            scanf("%d", &t);

    if (t == senhaadm){
            arquivo = fopen("contas.dat", "rb");
            if (arquivo == NULL){
                printf("Nenhuma conta cadastrada!\n");
            }
            else{
                printf("\n--- LISTA DE CONTAS ---\n");
                while(fread(&c, sizeof(struct conta), 1, arquivo))
                {
                    printf("Conta: %d | Titular: %s | Saldo: R$ %.2f\n", c.nconta, c.titular, c.saldo);
                }
                printf("-----------------------\n");
                fclose(arquivo);
            }
        }


    }


            } while (opcao != 7);

    printf("Programa encerrado...\n");
    return 0;
}
