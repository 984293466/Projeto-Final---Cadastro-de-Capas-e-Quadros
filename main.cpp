#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>

struct reg
{
	char model[30];
	char mat[30];
	char esp[30];
	char status;
};

int tamanho(FILE *);
void cadastrar(FILE *);
void consultar(FILE *);
void geraarqtxt(FILE *);
void excluir(FILE *);

int main()
{
	int op;
	FILE *arq;

	if ((arq = fopen("dados1.dat", "rb+")) == NULL)
	{
		if ((arq = fopen("dados1.dat", "wb+")) == NULL)
		{
			printf("Falha ao abrir o arquivo!\n");
			system("pause");
		}
	}

	//existentes no arquivo agenda1.dat
	do
	{
		system("CLS");
		printf("\n======= PEDIDOS ======= \n");
		printf("1.Cadastrar pedido\n");
		printf("2.Consultar pedido por código\n");
		printf("3.Gerar arquivo\n");
		printf("4.Excluir registro\n");
		printf("5.Sair\n");
		printf("===========Pedidos:%d=\n", tamanho(arq));
		printf("Opcao:");
		scanf("%d", &op);
		switch (op)
		{
		case 1:
			cadastrar(arq);
			break;
		case 2:
			consultar(arq);
			break;
		case 3:
		     geraarqtxt(arq);
			break;
		case 4:
			excluir(arq); 
			break;

		case 5: fclose(arq);
		}
	} while (op != 5);
}

void cadastrar(FILE *arq)
{
	reg contato;
	char confirma;
	contato.status = ' ';
	fflush(stdin);

	printf("Cadastrando novo registro:\n");
	printf("\nNumero do registro:%d\n", tamanho(arq) + 1);
	printf("Modelo da capa.................:");
	//getchar();
	gets(contato.model);
	printf("Material da capa...............:");
	gets(contato.mat);
	printf("Específica ou Universal........:");
	gets(contato.esp);
	printf("\nConfirma <s/n>:");
	fflush(stdin);
	scanf("%c", &confirma);

	if (toupper(confirma) == 'S', 's')
	{
		printf("\ngravando...\n\n");
		fseek(arq, 0, SEEK_END);
		fwrite(&contato, sizeof(reg), 1, arq);

	}
	system("pause");
}

void consultar(FILE *arq)
{
	reg contato;
	int nr;
	printf("\nConsulta pelo codigo\n");
	printf("\nInforme o Codigo...:");
	scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0))
	{
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&contato, sizeof(reg), 1, arq);
		if (contato.status == ' ')
		{
			printf("\nModelo da capa.............:%s", contato.model);
			printf("\nMaterial da capa...........:%s", contato.mat);
			printf("\nEspecífica ou Universal....:%s\n\n", contato.esp);
		}

		else
			printf("\nRegistro excluido! \n");

	}
	else
	{
		printf("\nNumero de registro invalido!\n");
	}
	system("pause");
}

void geraarqtxt(FILE *arq)
{
	char nomearq[20];
	printf("Nome do arquivo texto:");
	scanf("%s", nomearq);
	strcat(nomearq, ".txt");
	FILE *arqtxt = fopen(nomearq, "w");
	if (!arqtxt)
	{
		printf("Nao foi possivel criar esse arquivo!\n");
		system("pause");
		//return;
	}
	fprintf(arqtxt, "Modelo                Material            E/U               Status\n");
	fprintf(arqtxt, "==================================================================\n");
	int nr;
	reg contato;
	for (nr = 0; nr<tamanho(arq); nr++)
	{
		fseek(arq, nr * sizeof(reg), SEEK_SET);
		fread(&contato, sizeof(reg), 1, arq);
		fprintf(arqtxt, "%-20s%-12s%-25s- %c\n", contato.model, contato.mat, contato.esp, contato.status);
	}
	fprintf(arqtxt, "================================================================\n");
	fclose(arqtxt);
}

void excluir(FILE *arq)
{
	reg contato;
	char confirma;
	int nr;

	printf("\nInforme o codigo do registro para excluir\n");
	scanf("%d", &nr);
	if ((nr <= tamanho(arq)) && (nr>0))
	{
		fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
		fread(&contato, sizeof(reg), 1, arq);
		if (contato.status == ' ')
		{
			printf("\nModelo da capa.............:%s", contato.model);
			printf("\nMaterial da capa...........:%s", contato.mat);
			printf("\nEspecífica ou Universal....:%s\n\n", contato.esp);
			printf("\nConfirma a exclusao: <s/n>\n");
			getchar();
			scanf("%c", &confirma);

			if (toupper(confirma) == 'S', 's')
			{
				printf("\nexcluindo...\n\n");
				fseek(arq, (nr - 1) * sizeof(reg), SEEK_SET);
				contato.status = '*';
				fwrite(&contato, sizeof(reg), 1, arq);
			}
		}
	else
		printf("Registro inexistente! \n");
	}
	else
	{
		printf("\nNumero de registro invalido!\n");
	}
	system("pause");
}

int tamanho(FILE *arq)
{
	fseek(arq, 0, SEEK_END);
	return ftell(arq) / sizeof(reg);
}
