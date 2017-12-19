/*
   --------------------------------------- CRÉDITOS ----------------------------------------
   -                                                                                       -
   -                          Este Programa foi Desenvolvido por:                          -
   -                                                                                       -
   -                 > HEUDMANN Osmidio Lima       - heudmann@gmail.com                    -
   -                                                                                       -
   -----------------------------------------------------------------------------------------
   - Manaus/AM                           [H]x Systems                           23.06.2017 -
   -----------------------------------------------------------------------------------------
*/

//#include <windows.h> //Caso utilize keybd_event para maximizar
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define COLOR "COLOR 0F"

//Struct e Variáveis Globais para ajuste das rotinas do Programa
struct folha
{
	int reg;
	char nome[35];
	char turno;
	char categoria;
	float horas_trab;
	float salario;
	float valor_hora;
	float valor_aux;
	float salario_final;
};
	struct folha func;
	char op,po;
	bool lexit;
	int cd,num,nreg,limite_cad;
	float sal_min,sal_maior;
	FILE *folhapag,*auxiliar,*relatorio;


//Configurações Iniciais do Programa: Título, Idioma, Tamanho/Cores da Tela, Valores dos Salários, Limite de Registro
void config()
{
	system("title Programa RH - Folha de Funcionários");
	system("mode con: cols=145 line=40");
	system(COLOR);
	/*/Maximiza Tela no Prompt (CMD)
	keybd_event ( VK_MENU, 0x38, 0, 0 );
	keybd_event ( VK_SPACE, 0x39, 0, 0 );
	keybd_event (0x58,0,0,0);
	keybd_event ( VK_MENU, 0x38, KEYEVENTF_KEYUP, 0 );
	keybd_event ( VK_SPACE, 0x39, KEYEVENTF_KEYUP, 0 );
	keybd_event (0x58,0,KEYEVENTF_KEYUP,0); //*/
	setlocale(LC_ALL, "Portuguese");
	sal_min=900;
	sal_maior=1500;
	limite_cad=10000;
}


//Cabeçalhos e Rodapés utilizados nas telas do Programa
void header(){
	printf("\n\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\tEMPRESA XYZ LTDA     -	   (RH) REGISTRO DE FUNCIONÁRIOS\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------\n");
}

void header_incluir(){
	system("CLS");
	printf("\n\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\tEMPRESA XYZ LTDA     -	   (RH) REGISTRO DE FUNCIONÁRIOS\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------\n");
	printf("\n\t------------------------------- CADASTRO DE FUNCIONÁRIOS --------------------------------");
	printf("\n\n\t----------------------------------- INCLUIR REGISTRO ------------------------------------\n");
}

void header_altera1(){
	system("CLS");
	header();
	printf("\n\t-------------------------------- ALTERAR/EXCLUIR REGISTROS ------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t----------------------------------- Alterando REGISTRO: ---------------------------------");
}

void header_altera2(){
	printf("\n\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t-\t\t\t          - REGISTRO ALTERADO! -\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------\n");
	printf("\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t-\t\t\t       |Aperte qualquer tecla para|\t\t\t\t-");	
	printf("\n\t-\t\t\t       | voltar ao MENU Principal |\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t\t\t\t        Opção: ");
}

void header_limitecad(){
	printf("\n\t------------------------------------ LIMITE ATINGIDO! -----------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
	printf("\n\t-\t\t   OBS: Limite MÁXIMO de %d Funcionários Atingido! \t\t\t-",limite_cad);
	printf("\n\t-\t\t        Não é possível Cadastrar mais Funcionários \t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
	printf("\n\t-\t\tVocê pode EXCLUIR funcionários a partir do MENU Principal. \t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\t\t    [1] - VOLTAR ao MENU Principal \t\t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------");
}


//Contador de Registros/Cadastros
int contarRegistro(){
	int qtdReg=0;
	folhapag = fopen ("folhareg.dat","rb");
	if (folhapag == NULL){
		qtdReg=0;
	}
	else{
		fread(&func,sizeof(struct folha),1,folhapag);
		while(!feof(folhapag)){
			fread(&func,sizeof(struct folha),1,folhapag);
			qtdReg++;
		}
		fclose(folhapag);
	}
	return qtdReg;
}


//Cálculo Geral da Folha de Pagamento
void calcular(){
	//Hora Trabalhada - func.valor_hora
	if(func.categoria=='G' && func.turno=='N')
		func.valor_hora=(0.18*sal_min);
	else if(func.categoria=='G' && (func.turno=='M' || func.turno=='V'))
		func.valor_hora=(0.15*sal_min);
	else if (func.categoria=='O' && func.turno=='N')
		func.valor_hora=(0.13*sal_min);
	else if(func.categoria=='O' && (func.turno=='M' || func.turno=='V'))
		func.valor_hora=(0.10*sal_min);
	
	//Salário Inicial - func.salario
	func.salario=(func.valor_hora*func.horas_trab);

	//Auxílio Alimentação - func.valor_aux
	if(func.salario<=sal_min)
		func.valor_aux=(0.20*func.salario);
	else if(func.salario>sal_min && func.salario<sal_maior)
		func.valor_aux=(0.15*func.salario);
	else if(func.salario>=sal_maior)
		func.valor_aux=(0.05*func.salario);

	//Salário Final - func.salario_fina
	func.salario_final=(func.salario+func.valor_aux);
}


//Incluir Novos Cadastros
void incluir(){
	po='1';
	op='1';
	if (contarRegistro()>=limite_cad)
	{
		system("CLS");
		header();
		header_limitecad();
		printf("\n\t\t\t\t    Opção: ");
		op=getche();
		switch(op)
		{
			case '1': break;
			default: break;
		}
	}
	else
	{
		while(op=='1')
		{
			nreg=contarRegistro()+1;
			if (po=='1')
			{
				header_incluir();
				printf("\n\t\t\t\t-----------------------------------------");
				printf("\n\t\t\t\t-\t[1] - NOVO Registro n.[0%d]\t-",nreg);
				printf("\n\t\t\t\t-\t[2] - VOLTAR ao MENU\t\t-");
				printf("\n\t\t\t\t-----------------------------------------");
				printf("\n\n\t\t\t\t\tOpção: ");
				po=getche();
			}
			if (po!='1' && po!='n')
				break;
			else
			{
				header_incluir();
				printf("\n\t\t\t\t\t  - Registro n.[00%d] - \n",nreg);
				func.reg=nreg;
				printf("\n\t\t  > Nome do Funcionário: ");

				//Nome SEM espaços vazios \0 e SEM quebra de linha \n:
				fgets(func.nome, 35, stdin);
				func.nome[strlen(func.nome)-1]='\0';
				while(func.nome[strlen(func.nome)-1]==' ')
					func.nome[strlen(func.nome)-1]='\0';

				while(strcmp("\0",func.nome)==0){
					header_incluir();
					printf("\n\t\t\t    [ NOME Deve ser Preenchido! - Digite Novamente! ] \n");
					printf("\n\t\t  > Nome do Funcionário: ");

					fgets(func.nome, 35, stdin);
					func.nome[strlen(func.nome)-1]='\0';
					while(func.nome[strlen(func.nome)-1]==' ')
						func.nome[strlen(func.nome)-1]='\0';
				}
				fflush(stdin);
				//------------

				header_incluir();
				printf("\n\t\t\t\t\t  - Registro n.[00%d] - \n",nreg);
				printf("\n\t\t  > Nome do Funcionário: %s",func.nome);
				printf("\n\t\t  > Turno [M]at [V]esp [N]oturno: ");
				scanf("%c",&func.turno);
				fflush(stdin);

				while (func.turno !='M' && func.turno !='V' && func.turno !='N')
				{
					header_incluir();
					printf("\n\t\t\t       [ TURNO INEXISTENTE! - Digite Novamente! ] \n");
					printf("\n\t\t  > Nome do Funcionário: %s ",func.nome);
					printf("\n\t\t  > Turno [M]at [V]esp [N]oturno: ");
					scanf("%c",&func.turno);
					fflush(stdin);
				}
				fflush(stdin);

				header_incluir();
				printf("\n\t\t\t\t\t  - Registro n.[00%d] - \n",nreg);
				printf("\n\t\t  > Nome do Funcionário: %s ",func.nome);
				printf("\n");
				fflush(stdin);
				
				if(func.turno=='M')
					printf("\t\t  > Turno [M]at [V]esp [N]oturno: Matutino \n");
				else if(func.turno=='V')
					printf("\t\t  > Turno [M]at [V]esp [N]oturno: Vespertino \n");
				else if(func.turno=='N')
					printf("\t\t  > Turno [M]at [V]esp [N]oturno: Noturno \n");
				fflush(stdin);
				
				printf("\t\t  > Categoria [O]perário [G]erente: ");
				scanf("%c",&func.categoria);
				fflush(stdin);

				while (func.categoria !='O' && func.categoria !='G')
				{
					header_incluir();
					printf("\n\t\t\t     [ CATEGORIA INEXISTENTE! - Digite Novamente! ] \n");
					printf("\n\t\t  > Nome do Funcionário: %s ",func.nome);
					printf("\n");
					if(func.turno=='M')
						printf("\t\t  > Turno [M]at [V]esp [N]oturno: Matutino");
					else if(func.turno=='V')
						printf("\t\t  > Turno [M]at [V]esp [N]oturno: Vespertino");
					else if(func.turno=='N')
						printf("\t\t  > Turno [M]at [V]esp [N]oturno: Noturno");
					printf("\n\t\t  > Categoria [O]perário [G]erente: ");	
					scanf("%c",&func.categoria);
					fflush(stdin);
				}
				header_incluir();
				printf("\n\t\t\t\t\t  - Registro n.[00%d] - \n",nreg);
				printf("\n\t\t  > Nome do Funcionário: %s ",func.nome);
			
				if(func.turno=='M')
					printf("\n\t\t  > Turno [M]at [V]esp [N]oturno: Matutino \n");
				else if(func.turno=='V')
					printf("\n\t\t  > Turno [M]at [V]esp [N]oturno: Vespertino \n");
				else if(func.turno=='N')
					printf("\n\t\t  > Turno [M]at [V]esp [N]oturno: Noturno \n");
				if(func.categoria=='O')
					printf("\t\t  > Categoria [O]perário [G]erente: Operário \n");	
				else if(func.categoria=='G')
					printf("\t\t  > Categoria [O]perário [G]erente: Gerente \n");	

				printf("\t\t  > Horas Trabalhadas no Mês: ");	
				scanf("%f",&func.horas_trab);
				fflush(stdin);

				calcular();

				folhapag = fopen ("folhareg.dat","ab");
				fwrite(&func,sizeof(struct folha),1,folhapag);
				fclose(folhapag);

				if (contarRegistro()>=limite_cad)
				{
					printf("\n\t---------------------------------- REGISTRO REALIZADO! ----------------------------------");
					printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
					printf("\n\t-\t\t   OBS: Limite MÁXIMO de %d Funcionários Atingido! \t\t\t-",limite_cad);
					printf("\n\t-\t\t        Não é possível Cadastrar mais Funcionários \t\t\t-");
					printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
					printf("\n\t-\t\tVocê pode EXCLUIR funcionários a partir do MENU Principal. \t\t-");
					printf("\n\t-\t\t\t      [1]-VOLTAR ao MENU Principal \t\t\t\t-");
					printf("\n\t-----------------------------------------------------------------------------------------");
					printf("\n\t\t\t\t       Opção: ");
					getche();
					break;
				}
				else
				{
					printf("\n\t---------------------------------- REGISTRO REALIZADO! ----------------------------------");
					printf("\n\t-\t\t\t[1] - Cadastrar Novo Funcionário\t\t\t\t-");	
					printf("\n\t-\t\t\t[2] - MENU Principal \t\t\t\t\t\t-");
					printf("\n\t-----------------------------------------------------------------------------------------");
					printf("\n\n\t\t\t\tOpção: ");
					op=getche();
					if (op=='1')
					po='n';
				}
			}
		}
	}
}


//Cadastrar Funcionários
void cadastra(){
	if (contarRegistro()>=limite_cad){
		system("CLS");
		header();
		header_limitecad();
		printf("\n\t\t\t\t    Opção: ");
		op=getche();
		switch(op){
			case '1': break;
			default: break;
		}
	}
	else
	{
		system("CLS");
		header();
		printf("\n\t------------------------------- CADASTRO DE FUNCIONÁRIOS --------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		folhapag = fopen ("folhareg.dat","rb");
		if (folhapag == NULL)
		{
			printf("\n\t-\tOBS: Não foi encontrado o Arquivo de Registro 'FOLHAREG.DAT' na pasta. \t\t-");
			printf("\n\t-\t     Se esta for a 1a vez que irá Cadastrar Funcionários, escolha [1].\t\t-");
			printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
			printf("\n\t-\t\t\tEscolha uma das Opções abaixo: \t\t\t\t\t-");
			printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
			printf("\n\t-\t\t\t[1] - CADASTRAR Funcionários \t\t\t\t\t-");	
			printf("\n\t-\t\t\t[2] - ABRIR pasta de Origem \t\t\t\t\t-");	
			printf("\n\t-\t\t\t[3] - ATUALIZAR \t\t\t\t\t\t-");
			printf("\n\t-\t\t\t[4] - MENU Principal \t\t\t\t\t\t-");
			printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t\t\t\tOpção: ");
			op=getche();
			switch(op){
				case '1': incluir(); break;
				case '2': system("start ."); cadastra(); break;
				case '4': break;
				default: cadastra(); break;
			}
		}
		else
		{
			fclose(folhapag);
			incluir();
		}
	}
}


//Pesquisar - Busca por Nome
void busca_nome(){
	char name[35];
	int cont,i,j,w;
	op='1';
	num=0;
	while(op=='1')
	{
		system("CLS");
		header();
		printf("\n\t--------------------------------- PESQUISA DE REGISTROS ---------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t---------------------------- Digite o NOME a ser pesquisado: ----------------------------");
		printf("\n\n\t\t\t\t   >  ");


		fgets(name, 35, stdin);
		fflush(stdin);
		name[strlen(name)-1]='\0';
		while(name[strlen(name)-1]==' ')
			name[strlen(name)-1]='\0';

		printf("\n\t\t\t\t\t   -- [Resultados] --\n");
		
		cont=0;
		folhapag = fopen ("folhareg.dat","rb");
		fread(&func,sizeof(struct folha),1,folhapag);
		while(!feof(folhapag)){
	

//entrei aqui
		for (i=0; i<strlen(func.nome); i++)
			for (j=0; j<strlen(func.nome); j++)
			{
				//Remove repetições
				for (w=i+1; w<strlen(func.nome); w++)
					if ( tolower(name[i]) == tolower(name[w]))
						name[w] = ' ';
			
				//Compara letra a letra
				if ( (name[i] != ' ') && (tolower(name[i]) == tolower(func.nome[j])) )
				{
					cont++;
				}
			}
//ate aqui


//	orig	if (strcmp(name,func.nome)==0){
//	orig		cont++;

			if (cont>1){
	

				if (cont>1)
					printf("\n\n\t>---------------------------------------------------------------------------------------<\n");
				printf("\n\t\t\t\t   > Registro nº.....: %d",func.reg);
				printf("\n\t\t\t\t   > Nome............: %s",func.nome);
				if(func.turno=='M')
					printf("\n\t\t\t\t   > Turno...........: Matutino");
				else if(func.turno=='V')
					printf("\n\t\t\t\t   > Turno...........: Vespertino");
				else if(func.turno=='N')
					printf("\n\t\t\t\t   > Turno...........: Noturno");
				if(func.categoria=='O')
					printf("\n\t\t\t\t   > Categoria.......: Operário");
				else if(func.categoria=='G')
					printf("\n\t\t\t\t   > Categoria.......: Gerente");				
				printf("\n\t\t\t\t   > Horas Trab......: %.f",func.horas_trab);
				printf(" Horas");
				printf("\n\t\t\t\t   > Salário Inicial.: R$ %.2f",func.salario);
				printf("\n\t\t\t\t   > Valor Hora......: R$ %.2f",func.valor_hora);
				printf("\n\t\t\t\t   > Valor Auxílio...: R$ %.2f",func.valor_aux);
				printf("\n\t\t\t\t   > Salário Final...: R$ %.2f",func.salario_final);
			}
			fread(&func,sizeof(struct folha),1,folhapag);
		}
		fclose(folhapag);

		if (cont==0){
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t-\t\t\t     - NENHUM RESULTADO ENCONTRADO! -\t\t\t\t-");
			printf("\n\t-----------------------------------------------------------------------------------------\n");			
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t-\t\t\t      [1] - BUSCAR outro NOME \t\t\t\t\t-");	
			printf("\n\t-\t\t\t      [2] - Voltar ao MENU Principal \t\t\t\t-");
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t\t\t\t      Opção: ");
			op=getche();
		}
		else{
			printf("\n\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t-\t\t\t      - RESULTADOS ENCONTRADOS: 0%d -\t\t\t\t-",cont);
			printf("\n\t-\t\t    [1]-BUSCAR outro NOME   |   [2]-VOLTAR ao MENU  \t\t\t-");
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t\t\t\t      Opção: ");
			op=getche();
		}
	}
}


//Pesquisar - Busca por n. de Registro
void busca_reg(){
	int registro;
	int cont;
	op='1';
	num=0;
	while(op=='1')
	{
		system("CLS");
		header();
		printf("\n\t--------------------------------- PESQUISA DE REGISTROS ---------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t--------------------------- Digite o REGISTRO a ser pesquisado: -------------------------");
		printf("\n\n\t\t\t\t   >  ");
		scanf("%d",&registro);
		fflush(stdin);
		printf("\n\t\t\t\t\t   -- [Resultados] --\n");
		
		cont=0;
		folhapag = fopen ("folhareg.dat","rb");
		fread(&func,sizeof(struct folha),1,folhapag);
		while(!feof(folhapag)){
			if (registro == func.reg){
				cont++;	
				printf("\n\t\t\t\t   > Registro nº.....: %d",func.reg);
				printf("\n\t\t\t\t   > Nome............: %s",func.nome);
				if(func.turno=='M')
					printf("\n\t\t\t\t   > Turno...........: Matutino");
				else if(func.turno=='V')
					printf("\n\t\t\t\t   > Turno...........: Vespertino");
				else if(func.turno=='N')
					printf("\n\t\t\t\t   > Turno...........: Noturno");
				if(func.categoria=='O')
					printf("\n\t\t\t\t   > Categoria.......: Operário");
				else if(func.categoria=='G')
					printf("\n\t\t\t\t   > Categoria.......: Gerente");				
				printf("\n\t\t\t\t   > Horas Trab......: %.f",func.horas_trab);
				printf(" Horas");
				printf("\n\t\t\t\t   > Salário Inicial.: R$ %.2f",func.salario);
				printf("\n\t\t\t\t   > Valor Hora......: R$ %.2f",func.valor_hora);
				printf("\n\t\t\t\t   > Valor Auxílio...: R$ %.2f",func.valor_aux);
				printf("\n\t\t\t\t   > Salário Final...: R$ %.2f",func.salario_final);
			}
			fread(&func,sizeof(struct folha),1,folhapag);
		}
		fclose(folhapag);

		if (cont==0){
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t-\t\t\t     - REGISTRO NÃO CADASTRADO! -\t\t\t\t-");
			printf("\n\t-----------------------------------------------------------------------------------------\n");
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t-\t\t\t      [1] - BUSCAR outro REGISTRO \t\t\t\t-");	
			printf("\n\t-\t\t\t      [2] - Voltar ao MENU Principal \t\t\t\t-");
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t\t\t\t      Opção: ");
			op=getche();
		}
		else
		{
			printf("\n\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t-\t\t [1]-BUSCAR outro REGISTRO   |   [2]-VOLTAR ao MENU  \t\t\t-");
			printf("\n\t-----------------------------------------------------------------------------------------");
			printf("\n\t\t\t\t      Opção: ");
			op=getche();
		}
	}
}


//Função para Pesquisar Nomes e Registros
void pesquisa(){
	system("CLS");
	header();
	printf("\n\t--------------------------------- PESQUISA DE REGISTROS ---------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	folhapag = fopen ("folhareg.dat","rb");
	if (folhapag == NULL){
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
		printf("\n\t-\t\t   OBS: NÃO HÁ REGISTROS de Funcionários Cadastrados. \t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
		printf("\n\t-\t\t\tEscolha uma das Opções abaixo: \t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t[1] - CADASTRAR Funcionários \t\t\t\t\t-");	
		printf("\n\t-\t\t\t[2] - MENU Principal \t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-----------------------------------------------------------------------------------------");
		printf("\n\t\t\t\tOpção: ");
		op=getche();
		switch(op){
			case '1': incluir(); break;
			default: break;
		}
	}
	else
	{
		fclose(folhapag);
		system("CLS");
		header();
		printf("\n\t--------------------------------- PESQUISA DE REGISTROS ---------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t    > FUNCIONÁRIOS Cadastrados: [0%d]\t\t\t\t-",contarRegistro());
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-----------------------------------------------------------------------------------------\n");
		printf("\n\t-------------------------------------- BUSCAR POR: --------------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t  [1]-NOME do Funcionário   |   [2]-NÚMERO do Registro  \t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-----------------------------------------------------------------------------------------");
		printf("\n\t-\t\t\t     [3] - VOLTAR ao MENU Principal  \t\t\t\t-");
		printf("\n\t-----------------------------------------------------------------------------------------\n");
		printf("\t\t\t\t     Opção: ");
		op=getche();
		switch(op){
			case '1': busca_nome(); break;
			case '2': busca_reg(); break;
			default: break;
		}
	}
}


//Função GERAL para ALTERAR Dados / EXCLUIR Cadastros de Funcionários
void alterar(){
	int areg;
	char anome[35];
	char aturno;
	char acategoria;
	float ahoras_trab;
	float asalario;
	float avalor_hora;
	float avalor_aux;
	float asalario_final;
	int registro,cont,cont1;
	op='5';
	int num=0;

	if(contarRegistro()==0)
	{
		system("CLS");
		header();
		printf("\n\t-------------------------------- ALTERAR/EXCLUIR REGISTROS ------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
		printf("\n\t-\t\t   OBS: NÃO HÁ REGISTROS de Funcionários Cadastrados. \t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
		printf("\n\t-\t\t\tEscolha uma das Opções abaixo: \t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t[1] - CADASTRAR Funcionários \t\t\t\t\t-");	
		printf("\n\t-\t\t\t[2] - MENU Principal \t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-----------------------------------------------------------------------------------------");
		printf("\n\t\t\t\tOpção: ");
		op=getche();
		switch(op){
			case '1': incluir(); break;
			default: break;
		}
	}
	else
	{
		while(op=='5')
		{
			system("CLS");
			header();
			printf("\n\t-------------------------------- ALTERAR/EXCLUIR REGISTROS ------------------------------");
			printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
			printf("\n\t------------------------------- Digite o Número do REGISTRO: ----------------------------");
			printf("\n\n\t\t\t\t   >  ");
			scanf("%d",&registro);
			fflush(stdin);
			
			cont=0;
			cont1=0;
			folhapag = fopen ("folhareg.dat","rb");
			fread(&func,sizeof(struct folha),1,folhapag);
			
			while(!feof(folhapag))
			{
				if (registro == func.reg)
				{
					cont++;
					printf("\n\t\t\t\t  ->[1] - Nome............: %s",func.nome);
					if(func.turno=='M')
						printf("\n\t\t\t\t  ->[2] - Turno...........: Matutino");
					else if(func.turno=='V')
						printf("\n\t\t\t\t  ->[2] - Turno...........: Vespertino");
					else if(func.turno=='N')
						printf("\n\t\t\t\t  ->[2] - Turno...........: Noturno");

					if(func.categoria=='O')
						printf("\n\t\t\t\t  ->[3] - Categoria.......: Operário");
					else if(func.categoria=='G')
						printf("\n\t\t\t\t  ->[3] - Categoria.......: Gerente");				
					
					printf("\n\t\t\t\t  ->[4] - Horas Trab......: %.f",func.horas_trab);
					printf(" Horas");
					areg=func.reg;
					strcpy(anome,func.nome);
					aturno=func.turno;
					acategoria=func.categoria;
					ahoras_trab=func.horas_trab;
				}
				fread(&func,sizeof(struct folha),1,folhapag);
			}
			fclose(folhapag);

			if (cont==0){
				printf("\n\t-----------------------------------------------------------------------------------------");
				printf("\n\t-\t\t\t     - REGISTRO NÃO CADASTRADO! -\t\t\t\t-");
				printf("\n\t-----------------------------------------------------------------------------------------\n");
				printf("\n\t-----------------------------------------------------------------------------------------");
				printf("\n\t-\t\t\t      [1] - BUSCAR outro REGISTRO \t\t\t\t-");	
				printf("\n\t-\t\t\t      [2] - Voltar ao MENU Principal \t\t\t\t-");
				printf("\n\t-----------------------------------------------------------------------------------------");
				printf("\n\t\t\t\t      Opção: ");
				op=getche();
				if (op=='1')
					op='5';
			}
			else
			{
				printf("\n\n\t-----------------------------------------------------------------------------------------");
				printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
				printf("\n\t-\t       Digite o Número correspondente [1-4] ao que deseja [ALTERAR]   \t\t-");
				printf("\n\t-\t\t       - Para [EXCLUIR] o cadastro, digite [0] -\t\t\t-");
				printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
				printf("\n\t-----------------------------------------------------------------------------------------");
				printf("\n\t-\t\t [5]-ALTERAR outro REGISTRO   |   [6]-VOLTAR ao MENU  \t\t\t-");
				printf("\n\t-----------------------------------------------------------------------------------------");
				printf("\n\t\t\t\t      Opção: ");
				op=getche();
				switch(op)
				{
					case'1':
					{
						//Alterando NOME:
						header_altera1();
						folhapag = fopen ("folhareg.dat","rb");
						fread(&func,sizeof(struct folha),1,folhapag);
						while(!feof(folhapag))
						{
							if (registro == func.reg){
								strcpy(anome,func.nome);
								aturno=func.turno;
								acategoria=func.categoria;
								ahoras_trab=func.horas_trab;
							}
						fread(&func,sizeof(struct folha),1,folhapag);
						}
						fclose(folhapag);

						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						printf("\n\n\t\t\t        Atual ----> Nome............: %s",anome);
						printf("\n\t\t\t        Digite NOVO Nome............: ");

						//Nome SEM espaços e SEM enter:
						fgets(func.nome, 35, stdin);
						func.nome[strlen(func.nome)-1]='\0';
						while(func.nome[strlen(func.nome)-1]==' ')
							func.nome[strlen(func.nome)-1]='\0';
		
						while(strcmp("\0",func.nome)==0){
							header_altera1();
							printf("\n\n\t\t\t    [ NOME Deve ser Preenchido! - Digite Novamente! ]");
							printf("\n\n\t\t\t        Atual ----> Nome............: %s",anome);
							printf("\n\t\t\t        Digite NOVO Nome............: ");
							fgets(func.nome, 35, stdin);
							func.nome[strlen(func.nome)-1]='\0';
							while(func.nome[strlen(func.nome)-1]==' ')
								func.nome[strlen(func.nome)-1]='\0';
						}
						fflush(stdin);
						//------------

						header_altera1();
						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						printf("\n\n\t\t\t        Antes ----> Nome............: %s",anome);
						printf("\n\t\t\t               NOVO Nome............: %s",func.nome);

						func.reg=areg;
						//func.nome - ok
						func.turno=aturno;
						func.categoria=acategoria;
						func.horas_trab=ahoras_trab;
						calcular();

						fflush(stdin);
						folhapag = fopen ("folhareg.dat","r+b");
						fseek(folhapag,(areg-1)*(sizeof (struct folha)),SEEK_SET);
						fwrite(&func,sizeof(struct folha),1,folhapag);
						fclose(folhapag);

						cont1=0;
						header_altera2();
						getch();
						break;
					}
					case'2':
					{
						//Alterando TURNO:
						header_altera1();
						folhapag = fopen ("folhareg.dat","rb");
						fread(&func,sizeof(struct folha),1,folhapag);
						while(!feof(folhapag)){
							if (registro == func.reg){
								strcpy(anome,func.nome);
								aturno=func.turno;
								acategoria=func.categoria;
								ahoras_trab=func.horas_trab;
							}
							fread(&func,sizeof(struct folha),1,folhapag);
						}
						fclose(folhapag);

						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						if(aturno=='M')
							printf("\n\n\t\t\t        Atual ----> Turno...........: Matutino");
						else if(aturno=='V')
							printf("\n\n\t\t\t        Atual ----> Turno...........: Vespertino");
						else if(aturno=='N')
							printf("\n\n\t\t\t        Atual ----> Turno...........: Noturno");
						printf("\n\n\t\t\t        Digite NOVO Turno: \n\t\t\t        [M]at [V]esp [N]oturno......: ");
						scanf("%c",&func.turno);
						fflush(stdin);

						while (func.turno !='M' && func.turno !='V' && func.turno !='N')
						{
							header_altera1();
							printf("\n\n\t\t\t       [ TURNO INEXISTENTE! - Digite Novamente! ]");
							if(aturno=='M')
								printf("\n\n\t\t\t        Atual ----> Turno...........: Matutino");
							else if(aturno=='V')
								printf("\n\n\t\t\t        Atual ----> Turno...........: Vespertino");
							else if(aturno=='N')
								printf("\n\n\t\t\t        Atual ----> Turno...........: Noturno");
							printf("\n\n\t\t\t        Digite NOVO Turno: \n\t\t\t        [M]at [V]esp [N]oturno......: ");
							scanf("%c",&func.turno);
							fflush(stdin);
						}
						header_altera1();
						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						if(aturno=='M')
							printf("\n\n\t\t\t        Antes ----> Turno...........: Matutino");
						else if(aturno=='V')
							printf("\n\n\t\t\t        Antes ----> Turno...........: Vespertino");
						else if(aturno=='N')
							printf("\n\n\t\t\t        Antes ----> Turno...........: Noturno");

						if(func.turno=='M')					
							printf("\n\n\t\t\t               NOVO Turno: \n\t\t\t        [M]at [V]esp [N]oturno......: Matutino");
						else if(func.turno=='V')
							printf("\n\n\t\t\t               NOVO Turno: \n\t\t\t        [M]at [V]esp [N]oturno......: Vespertino");
						else if(func.turno=='N')
							printf("\n\n\t\t\t               NOVO Turno: \n\t\t\t        [M]at [V]esp [N]oturno......: Noturno");

						func.reg=areg;
						strcpy(func.nome,anome);
						//func.turno - ok
						func.categoria=acategoria;
						func.horas_trab=ahoras_trab;
						calcular();

						folhapag = fopen ("folhareg.dat","r+b");
						fseek(folhapag,(areg-1)*(sizeof (struct folha)),SEEK_SET);
						fwrite(&func,sizeof(struct folha),1,folhapag);
						fclose(folhapag);

						cont1=0;
						header_altera2();
						getch();
						break;
					}
					case'3':
					{
						//Alterando CATEGORIA:
						header_altera1();
						folhapag = fopen ("folhareg.dat","rb");
						fread(&func,sizeof(struct folha),1,folhapag);
						while(!feof(folhapag)){
							if (registro == func.reg){
								strcpy(anome,func.nome);
								aturno=func.turno;
								acategoria=func.categoria;
								ahoras_trab=func.horas_trab;
							}
						fread(&func,sizeof(struct folha),1,folhapag);
						}
						fclose(folhapag);

						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						if(acategoria=='G')
							printf("\n\n\t\t\t         Atual ---> Categoria.......: Gerente");
						else if(acategoria=='O')
							printf("\n\n\t\t\t         Atual ---> Categoria.......: Operário");
						printf("\n\n\t\t\t         Digite NOVA Categoria: \n\t\t\t         [O]perário [G]erente.......: ");
						scanf("%c",&func.categoria);
						fflush(stdin);		
			
						while (func.categoria !='O' && func.categoria !='G')
						{
							header_altera1();
							printf("\n\n\t\t\t     [ CATEGORIA INEXISTENTE! - Digite Novamente! ] \n");
							if(acategoria=='G')
								printf("\n\t\t\t         Atual ---> Categoria.......: Gerente");
							else if(acategoria=='O')
								printf("\n\t\t\t         Atual ---> Categoria.......: Operário");
							printf("\n\n\t\t\t         Digite NOVA Categoria: \n\t\t\t         [O]perário [G]erente......: ");
							scanf("%c",&func.categoria);
							fflush(stdin);		
						}
						header_altera1();
						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						if(acategoria=='G')
							printf("\n\n\t\t\t         Antes ---> Categoria.......: Gerente");
						else if(acategoria=='O')
							printf("\n\n\t\t\t         Antes ---> Categoria.......: Operário");

						if(func.categoria=='G')
							printf("\n\n\t\t\t               NOVA Categoria: \n\t\t\t         [O]perário [G]erente.......: Gerente");
						else if(func.categoria=='O')
							printf("\n\n\t\t\t               NOVA Categoria: \n\t\t\t         [O]perário [G]erente.......: Operário");

						func.reg=areg;
						strcpy(func.nome,anome);
						func.turno=aturno;
						//func.categoria - ok
						func.horas_trab=ahoras_trab;
	
						calcular();

						folhapag = fopen ("folhareg.dat","r+b");
						fseek(folhapag,(registro-1)*(sizeof (struct folha)),SEEK_SET);
						fwrite(&func,sizeof(struct folha),1,folhapag);
						fclose(folhapag);

						cont1=0;
						header_altera2();
						getch();
						break;
					}
					case'4':
					{
						//Alterando HORAS TRABALHADAS:
						header_altera1();
						folhapag = fopen ("folhareg.dat","rb");
						fread(&func,sizeof(struct folha),1,folhapag);
						while(!feof(folhapag)){
							if (registro == func.reg){
								strcpy(anome,func.nome);
								aturno=func.turno;
								acategoria=func.categoria;
								ahoras_trab=func.horas_trab;
							}
							fread(&func,sizeof(struct folha),1,folhapag);
						}
						fclose(folhapag);

						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						printf("\n\n\t\t\t        Atual ----> Horas Trab......: %.f",ahoras_trab);
						printf("\n\t\t\t       Digite NOVAS Horas Trab......: ");
						scanf("%f",&func.horas_trab);

						header_altera1();
						printf("\n\n\t\t\t\t          > Registro nº.....: %d",areg);
						printf("\n\n\t\t\t        Antes ----> Horas Trab......: %.f",ahoras_trab);
						printf("\n\t\t\t              NOVAS Horas Trab......: %.f",func.horas_trab);

						func.reg=areg;
						strcpy(func.nome,anome);
						func.turno=aturno;
						func.categoria=acategoria;
						//func.horas_trab - ok

						calcular();

						folhapag = fopen ("folhareg.dat","r+b");
						fseek(folhapag,(areg-1)*(sizeof (struct folha)),SEEK_SET);
						fwrite(&func,sizeof(struct folha),1,folhapag);
						fclose(folhapag);

						cont1=0;
						header_altera2();
						getch();
						break;
					}
					case'0':
					{
						//EXCLUIR REGISTRO:
						system("CLS");
						header();
						printf("\n\t-------------------------------- ALTERAR/EXCLUIR REGISTROS ------------------------------");
						printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
						printf("\n\t------------------------------------ EXCLUIR REGISTRO -----------------------------------\n\n");
						printf("\n\t-----------------------------------------------------------------------------------------");
						printf("\n\t-\t\t      Tem CERTEZA que deseja EXCLUIR o Registro n.[%d]?   \t\t-",areg);	
						printf("\n\t-\t\t\t\t     [1]-SIM / [2]-NÃO \t\t\t\t\t-");
						printf("\n\t-----------------------------------------------------------------------------------------");
						printf("\n\t\t\t\t\t     Opção: ");
						op=getche();
						switch(op)
						{
							case'1':
							{
								//Confirma Excluir
								cont1=0;
								folhapag=fopen("folhareg.dat","rb");
								auxiliar=fopen("auxiliar.dat","wb");
	
								fread(&func,sizeof(struct folha),1,folhapag);
								while(!feof(folhapag)){
									if(func.reg!=registro){
										cont1++;
										func.reg=cont1;
										fwrite(&func,sizeof(struct folha),1,auxiliar);
									}
								fread(&func,sizeof(struct folha),1,folhapag);
								}

								fclose(auxiliar);
								sleep(1);
								fclose(folhapag);
								remove("folhareg.dat");
								rename("auxiliar.dat","folhareg.dat");

								//Deleta folhareg.dat caso se exclua todos os registros:
								if (contarRegistro()==0)
									remove("folhareg.dat");
								//Fim Excluir

								printf("\n\n\t-----------------------------------------------------------------------------------------");
								printf("\n\t-\t\t\t          > REGISTRO EXCLUÍDO! <\t\t\t\t-");
								printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
								printf("\n\t-\t\t\t       [1]-VOLTAR ao MENU Principal  \t\t\t\t-");
								printf("\n\t-----------------------------------------------------------------------------------------\n");
								printf("\t\t\t\t                   ");
								getch();
								break;
							}
							default: break;
						}
					}
					default: break;
				}
			}
		}
	}
}


//Exporta a Folha de Pagamento de todos os funcionários em Arquivo .dat
void exporta(){
	int contt;
	system("CLS");
	header();
	printf("\n\t------------------------------------ FOLHA DE PAGAMENTO ---------------------------------\n");

	relatorio = fopen ("FOLHA_DE_PAGAMENTO.txt","w");
	fprintf(relatorio,"\n\t-----------------------------------------------------------------------------------------");
	fprintf(relatorio,"\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	fprintf(relatorio,"\n\t-\t\tEMPRESA XYZ LTDA     -	   (RH) REGISTRO DE FUNCIONÁRIOS\t\t-");
	fprintf(relatorio,"\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	fprintf(relatorio,"\n\t-----------------------------------------------------------------------------------------");
	fprintf(relatorio,"\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	fprintf(relatorio,"\n\t---------------------------- RELATÓRIO DA FOLHA DE PAGAMENTO ----------------------------\n");
	
	contt=0;
	folhapag = fopen ("folhareg.dat","rb");
	fread(&func,sizeof(struct folha),1,folhapag);
	
	while(!feof(folhapag))
	{
		contt++;
		if (contt>1)
			fprintf(relatorio,"\n\n\t>---------------------------------------------------------------------------------------<\n");
		
		fprintf(relatorio,"\n\t\t\t\t   > Registro nº.....: %d",func.reg);
		fprintf(relatorio,"\n\t\t\t\t   > Nome............: %s",func.nome);
	
		if(func.turno=='M')
			fprintf(relatorio,"\n\t\t\t\t   > Turno...........: Matutino");
		else if(func.turno=='V')
			fprintf(relatorio,"\n\t\t\t\t   > Turno...........: Vespertino");
		else if(func.turno=='N')
			fprintf(relatorio,"\n\t\t\t\t   > Turno...........: Noturno");
		if(func.categoria=='O')
			fprintf(relatorio,"\n\t\t\t\t   > Categoria.......: Operário");
		else if(func.categoria=='G')
			fprintf(relatorio,"\n\t\t\t\t   > Categoria.......: Gerente");				
	
		fprintf(relatorio,"\n\t\t\t\t   > Horas Trab......: %.f",func.horas_trab);
		fprintf(relatorio," Horas");
		fprintf(relatorio,"\n\t\t\t\t   > Salário Inicial.: R$ %.2f",func.salario);
		fprintf(relatorio,"\n\t\t\t\t   > Valor Hora......: R$ %.2f",func.valor_hora);
		fprintf(relatorio,"\n\t\t\t\t   > Valor Auxílio...: R$ %.2f",func.valor_aux);
		fprintf(relatorio,"\n\t\t\t\t   > Salário Final...: R$ %.2f",func.salario_final);
		fread(&func,sizeof(struct folha),1,folhapag);
	}

	fclose(folhapag);
	fprintf(relatorio,"\n\n\t-----------------------------------------------------------------------------------------");
	fprintf(relatorio,"\n\t-\t\t\t       - TOTAL DE FUNCIONÁRIOS: 0%d -\t\t\t\t-",contt);
	fprintf(relatorio,"\n\t-----------------------------------------------------------------------------------------");
	fprintf(relatorio,"\n\t- Manaus/AM                           [H]x Systems                                 2017 -");
	fprintf(relatorio,"\n\t-----------------------------------------------------------------------------------------\t\n");
	fprintf(relatorio,"\t\t\t    Relatório Gerado no dia: %s, às %s\n",__DATE__,__TIME__);
	fclose(relatorio);

	printf("\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\tRelatório Gerado com Sucesso em %s, às %s \t\t-",__DATE__,__TIME__);
	printf("\n\t-\t\t     Armazenado no Arquivo 'FOLHA_DE_PAGAMENTO.TXT'\t\t\t-",contt);
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------");
	printf("\n\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t-\t\t\t       - TOTAL DE FUNCIONÁRIOS: 0%d -\t\t\t\t-",contt);
	printf("\n\t-\t\t                     [VOLTAR ao MENU]    \t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------");
	printf("\n\n\t\t\t\t             Opção: ");
	system("start FOLHA_DE_PAGAMENTO.txt");
	op=getche();
}


//Exibe a Folha de Pagamento de todos os Funcionários em TELA
void folha_lista(){
	int contt;
	system("CLS");
	header();
	printf("\n\t------------------------------------ FOLHA DE PAGAMENTO ---------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t------------------------------------ TODOS os Cadastros ---------------------------------\n");

	contt=0;
	folhapag = fopen ("folhareg.dat","rb");
	fread(&func,sizeof(struct folha),1,folhapag);
	while(!feof(folhapag))
	{
		contt++;
		if (contt>1)
			printf("\n\n\t>---------------------------------------------------------------------------------------<\n");
		
		printf("\n\t\t\t\t   > Registro nº.....: %d",func.reg);
		printf("\n\t\t\t\t   > Nome............: %s",func.nome);
		
		if(func.turno=='M')
			printf("\n\t\t\t\t   > Turno...........: Matutino");
		else if(func.turno=='V')
			printf("\n\t\t\t\t   > Turno...........: Vespertino");
		else if(func.turno=='N')
			printf("\n\t\t\t\t   > Turno...........: Noturno");
		if(func.categoria=='O')
			printf("\n\t\t\t\t   > Categoria.......: Operário");
		else if(func.categoria=='G')
			printf("\n\t\t\t\t   > Categoria.......: Gerente");				
		
		printf("\n\t\t\t\t   > Horas Trab......: %.f",func.horas_trab);
		printf(" Horas");
		printf("\n\t\t\t\t   > Salário Inicial.: R$ %.2f",func.salario);
		printf("\n\t\t\t\t   > Valor Hora......: R$ %.2f",func.valor_hora);
		printf("\n\t\t\t\t   > Valor Auxílio...: R$ %.2f",func.valor_aux);
		printf("\n\t\t\t\t   > Salário Final...: R$ %.2f",func.salario_final);
		fread(&func,sizeof(struct folha),1,folhapag);
	}
	fclose(folhapag);
	printf("\n\n\t-----------------------------------------------------------------------------------------");
	printf("\n\t-\t\t\t     - FUNCIONÁRIOS CADASTRADOS: 0%d -\t\t\t\t-",contt);
	printf("\n\t-\t\t                     [VOLTAR ao MENU]    \t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------");
	printf("\n\n\t\t\t\t             Opção: ");
	op=getche();
}


//Função Geral para Mostrar / Exportar Folha de Pagamento
void folha(){
	folhapag = fopen ("folhareg.dat","rb");
	if (folhapag == NULL)
	{
		system("CLS");
		header();
		printf("\n\t------------------------------------ FOLHA DE PAGAMENTO ---------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
		printf("\n\t-\t\t   OBS: NÃO HÁ REGISTROS de Funcionários Cadastrados. \t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
		printf("\n\t-\t\t\tEscolha uma das Opções abaixo: \t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t[1] - CADASTRAR Funcionários \t\t\t\t\t-");	
		printf("\n\t-\t\t\t[2] - MENU Principal \t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-----------------------------------------------------------------------------------------");
		printf("\n\t\t\t\tOpção: ");
		op=getche();
		switch(op){
			case '1': incluir(); break;
			default: break;
		}
	}
	else
	{
		fclose(folhapag);
		system("CLS");
		header();
		printf("\n\t ----------------------------------- FOLHA DE PAGAMENTO --------------------------------");
		printf("\n\t|                                           |                                           |");
		printf("\n\t|                                           |                                           |");
		printf("\n\t|            Para Mostrar na tela           |           Para EXPORTAR a lista           |");
		printf("\n\t|           TODOS os Cadastros dos          |             como Relatório em             |");
		printf("\n\t|          Funcionários Registrados         |              ARQUIVO de Texto             |");
		printf("\n\t|                                           |                                           |");
		printf("\n\t|                 Escolha [1]               |                 Escolha [2]               |");
		printf("\n\t|                                           |                                           |");
		printf("\n\t|                                           |                                           |");
		printf("\n\t ---------------------------------------------------------------------------------------");
		printf("\n\t|                                   [3]-MENU Principal                                  |");
		printf("\n\t ---------------------------------------------------------------------------------------");
		printf("\n\n\t\t\t\t\t\t  Opção: ");
		op=getche();
		switch(op){
			case '1': folha_lista(); break;
			case '2': exporta(); break;
			default: break;
		}
	}
}


//Informações sobre os Cálculos da Folha de Pagamento que o Programa Efetua
void infoCalculo(){
	system("CLS");
	header();
	printf("\n\t-------------------------- CÁLCULO SOBRE A FOLHA DE PAGAMENTO ---------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t   --------------------------------------------------------------------  \t-");
	printf("\n\t-\t   -  CATEGORIA/CARGO  |         TURNO          |    VALOR DA HORA    -  \t-");
	printf("\n\t-\t   --------------------------------------------------------------------  \t-");
	printf("\n\t-\t   -      GERENTE      |        Noturno         | 18%% Do Sal. Mínino  -  \t-");
	printf("\n\t-\t   -      GERENTE      | Matutino OU Vespertino | 15%% Do Sal. Mínino  -  \t-");
	printf("\n\t-\t   -      OPERÁRIO     |        Noturno         | 13%% Do Sal. Mínino  -  \t-");
	printf("\n\t-\t   -      OPERÁRIO     | Matutino OU Vespertino | 10%% Do Sal. Mínino  -  \t-");
	printf("\n\t-\t   --------------------------------------------------------------------  \t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t   --------------------------------------------------------------------  \t-");
	printf("\n\t-\t   -            SALÁRIO INICIAL           |    AUXÍLIO-ALIMENTAÇÃO    -  \t-");
	printf("\n\t-\t   --------------------------------------------------------------------  \t-");
	printf("\n\t-\t   -      MENOR ou IGUAL a R$900,00       |  20%% do Salário Inicial   -  \t-");
	printf("\n\t-\t   - MAIOR que R$900 e MENOR que R$1.500  |  15%% do Salário Inicial   -  \t-");
	printf("\n\t-\t   -      MAIOR ou IGUAL a R$1.500        |   5%% do Salário Inicial   -  \t-");
	printf("\n\t-\t   --------------------------------------------------------------------  \t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------\t");
	printf("\n\t\t\t\t      [ENTER para voltar ao MENU]");
	getch();
}


//Créditos sobre o Desenvolvimento do Programa
void creditos()
{
	system("color 0A");
	system("CLS");
	header();
	printf("\n\t--------------------------------------- CRÉDITOS ----------------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-                          Este Programa foi Desenvolvido por:                          -");	
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-                               > HEUDMANN Osmidio Lima                                 -");	
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\t\t\t- Contato: (92) 99403-9117  \t\t\t\t-");
	printf("\n\t-\t\t\t\t- E-mail : heudmann@gmail.com \t\t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------\t");
	printf("\n\t- Manaus/AM                           [H]x Systems                                 2017 -");
	printf("\n\t-----------------------------------------------------------------------------------------\t");
	printf("\n\n\t\t\t\t      [ENTER para voltar ao MENU]");
	getch();
	system(COLOR);
}


//Sair do Programa
sair(){
	system("CLS");
	header();
	printf("\n\t------------------------------------ SAIR DO PROGRAMA ------------------------------------");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");	
	printf("\n\t-\t\t\t  Tem certeza que deseja SAIR do Programa? \t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-\t\t\t      [1] - SAIR \t\t\t\t\t\t-");	
	printf("\n\t-\t\t\t      [2] - Voltar ao MENU Principal \t\t\t\t-");
	printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
	printf("\n\t-----------------------------------------------------------------------------------------");
	printf("\n\n\t\t\t\t      Opção: ");
	op=getche();
	switch(op){
		case '1':{
			lexit=true;
			break;
		}
		default: break;
	}
}


//MENU Declaração
void menu();


//Funcão Principal - MAIN()
int main()
{
	config();
	menu();
}

//MENU Principal do Programa
void menu()
{
	lexit=false;
	int cd;
	op='0';
	while (op!='7' && !lexit)
	{
		system("cls");
		header();
		printf("\n\t------------------------------------ MENU DE OPÇÕES -------------------------------------");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t[1] - CADASTRAR Funcionários \t\t\t\t\t-");	
		printf("\n\t-\t\t\t[2] - PESQUISAR Cadastros \t\t\t\t\t-");	
		printf("\n\t-\t\t\t[3] - ALTERAR/EXCLUIR Cadastros \t\t\t\t-");	
		printf("\n\t-\t\t\t[4] - EXIBIR/EXPORTAR Folha de Pagamento \t\t\t-");	
		printf("\n\t-\t\t\t[5] - CÁLCULO da Folha de Pagamento \t\t\t\t-");
		printf("\n\t-\t\t\t[6] - SOBRE o Programa \t\t\t\t\t\t-");
		printf("\n\t-\t\t\t[7] - SAIR \t\t\t\t\t\t\t-");
		printf("\n\t-\t\t\t\t\t\t\t\t\t\t\t-");
		printf("\n\t-----------------------------------------------------------------------------------------\t");
		printf("\n\t- Manaus/AM                           [H]x Systems                                 2017 -");
		printf("\n\t-----------------------------------------------------------------------------------------\t");
		printf("\n\t\t\t\tOpção: ");
		op=getche();
		switch(op)
		{
			case '1': cd=1; cadastra(); break;
			case '2': pesquisa(); break;
			case '3': alterar(); break;
			case '4': folha(); break;
			case '5': infoCalculo(); break;
			case '6': creditos(); break;
			case '7': sair(); break;
			default: break;
		}
	}
	system("CLS");
	system("EXIT");
}
