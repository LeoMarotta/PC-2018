/*
 * loja.c
 * 
 * Copyright 2019 senak <senak@LGM>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct estoque{
	int cod;
	int quant;
	int valaq;
	int lucro;
	int totalv;
	struct estoque *next;
};
void inserir(int cod, int quant, int valaq, int lucro, struct estoque **l){
	struct estoque* aux;
	
	if(*l == NULL){
		*l = (struct estoque*) malloc(sizeof(struct estoque));
		(*l)->cod = cod;
		(*l)->quant = quant;
		(*l)->valaq = valaq;
		(*l)->lucro = lucro;
		(*l)->totalv = 0;
		(*l)->next = NULL;
	}
	else{
		aux = (struct estoque*) malloc(sizeof(struct estoque));
		aux->cod = cod;
		aux->quant = quant;
		aux->valaq = valaq;
		aux->lucro = lucro;
		aux->totalv = 0;
		aux->next = *l;
		*l = aux;
	}
}
void ordenar(struct estoque **l){
	struct estoque *aux1, *aux2;
	int cod, quant, valaq, lucro, totalv;

	for(aux1 = (*l); aux1->next != NULL; aux1 = aux1->next){
		aux2 = aux1->next;
		while(aux2 != NULL){
			if(aux1->cod > aux2->cod){
				cod = aux2->cod;
				quant = aux2->quant;
				valaq = aux2->valaq;
				lucro = aux2->lucro;
				totalv = aux2->totalv;
				aux2->cod = aux1->cod;  
				aux2->quant = aux1->quant;  
				aux2->valaq = aux1->valaq;  
				aux2->lucro = aux1->lucro;  
				aux2->totalv = aux1->totalv;  
				aux1->cod = cod;
				aux1->quant = quant;
				aux1->valaq = valaq;
				aux1->lucro = lucro;
				aux1->totalv = totalv;
			}
			aux2 = aux2->next;
		}
	}
}
void imprimirl(struct estoque *l){
	char pc = '%';
	printf("\n");
	for(;l != NULL; l = l->next) 
	   printf("Cod:%d Quant.:%d Val.Aq.:%d Lucro:%d%c \n", l->cod, l->quant, l->valaq, l->lucro, pc);
	printf("\n");
}
void regvenda(int cod, int venda, struct estoque *l){
	struct estoque *r = l;
	for(; l != NULL; l = l->next){
		if(cod == l->cod){
			if(l->quant == 0){
				printf("Não tem mais esse item no estoque");
				return;
			}else if((l->quant - venda) < 0){
				printf("Quantidade de estoque negativa, operação cancelada!\n");
				return;
			}
			l->quant = (l->quant) - venda;
			l->totalv += venda;
			return;
		}
	}
	printf("Nao achou o código: %d!\n",cod);
}
int atualizar(int cod, int quant, int valaq, int lucro, struct estoque *l){
	for(; l != NULL; l = l->next){
		if(l->cod == cod){
			l->quant = quant;
			l->valaq = valaq;
			l->lucro = lucro;
			return 1;
		}
	}
	return 0;
}
void salvar(struct estoque *l){
	FILE *arq2;
	char old[9] = "loja.txt";
	char new[9] = "loja.bak";
	rename(old, new);
	if((arq2 = fopen("loja.txt", "w")) == NULL){	
		printf("Erro ao carregar arquivo.\n");
		exit(1);
	}
	for(; l != NULL; l = l->next){
		if(l->quant != 0) {
			fprintf(arq2, "%d,%d,%d,%d\n", l->cod, l->quant, l->valaq, l->lucro);
		}
	}
	fclose(arq2);
}
void gerarrel(struct estoque *l){
	float arrec;
	FILE *arq3;
	if((arq3 = fopen("relatório.txt", "w")) == NULL){	
		printf("Erro ao carregar arquivo.\n");
		exit(1);
	}
	for(; l != NULL; l = l->next){
		arrec = (l->valaq / 100 * l->lucro) * l->totalv;
		fprintf(arq3, "====================\n");
		fprintf(arq3, "Código: %d\nEstoque:%d\nVendas: %d\nLucro total: %.2f\n", l->cod, l->totalv, l->valaq, arrec);
		fprintf(arq3, "====================\n");
	}
	fclose(arq3);
}
int main() {
	FILE *arq1;
	struct estoque *e = NULL;
	char flag, buffer, *p, linha[50], comando[] = "clear";
	int cod, quant, valaq, lucro, totalv, c, a, v[4];

	system(comando);
	printf("++++++++++PROGRAMA INICIADO++++++++++\n");	
	if((arq1 = fopen("loja.txt", "r")) == NULL){
		printf("Erro ao carregar arquivo\n");
		exit(1);
	}
	while(fscanf(arq1, "%s", linha) != EOF){
		c = 0;
		p = strtok(linha, ",");
		while(p != NULL){
			v[c] = atoi(p);
			c++;
			p = strtok(NULL, ",");
		}
		cod = v[0];
		quant = v[1];
		valaq = v[2];
		lucro = v[3];
		inserir(cod,quant,valaq,lucro,&e);
		ordenar(&e);
	}
	fclose(arq1);
	while(flag != 'x'){
		imprimirl(e);
		printf("Digite a letra da opção desejada: \n");
		printf("[[v]] para venda de um produto\n");
		printf("[[a]] adicionar itens ao estoque\n");
		printf("[[s]] salvar a lista\n");
		printf("[[r]] gerar relatório de vendas\n");
		printf("[[x]] salvar a lista e sair do programa\n");
		scanf("%c", &flag);
		scanf("%c", &buffer);
		switch(flag){
		case 'v':
			printf("++++++++++REPORTAR VENDA++++++++++\n");
			printf("Digite o código do item: \n");
			scanf("%d", &cod);
			scanf("%c", &buffer);
			printf("Digite a quantidade que foi vendida: \n");
			scanf("%d", &quant);
			scanf("%c", &buffer);
			regvenda(cod, quant, e);
			break;
		case 'a':
			printf("++++++++++ADICIONAR ITENS++++++++++\n");
			printf("Digite o código do item: \n");
			scanf("%d", &cod);
			scanf("%c", &buffer);
			printf("Digite a quantidade disponível: \n");
			scanf("%d", &quant);
			scanf("%c", &buffer);
			printf("Digite o valor de aquisição: \n");
			scanf("%d", &valaq);
			scanf("%c", &buffer);
			printf("Digite a margem de lucro: \n");
			scanf("%d", &lucro);
			scanf("%c", &buffer);
			a = atualizar(cod, quant, valaq, lucro, e);
			if(a == 0) inserir(cod, quant, valaq, lucro, &e);
			ordenar(&e);
			break;
		case 's':
			printf("Salvando...\n");
			ordenar(&e);
			imprimirl(e);
			salvar(e);	
			break;
		case 'r':
			printf("Gerando relatório...\n");
			ordenar(&e);
			gerarrel(e);	
			break;
		case 'x':		
			printf("Salvando...\n");
			ordenar(&e);
			salvar(e);
			printf("Gerando relatório...\n");
			gerarrel(e);
			break;
		default:
			printf("Opção inválida, digite novamente!\n");
			break;
		}
	}
	printf("++++++++++PROGRAMA ENCERRADO++++++++++\n");	
	return 0;
}
