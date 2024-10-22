#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib_jogos.h"

#define gotoxy(x,y) printf("%c[%d;%df",0x1B,y,x)

void barra_info(Jogador_t j){
	gotoxy(1,ALTURA+2);
	printf("[%s]",j.nome);
	gotoxy(1,ALTURA+3);
	printf("|Vida:%d",j.vida);
	gotoxy(1,ALTURA+4);
	printf("|Pontos:%d",j.pontos);
}

void coloca_objetos(Objeto_t**p){
	for(int j = 0; j < QTD_OBJETOS; j++){
		if(p[j]->qtd){
			for(int i = 0; i < p[j]->qtd; i++){
				gotoxy(p[j]->pos[i].x,p[j]->pos[i].y);
				printf("%c\n", p[j]->identificador);
			}
		}
	}
}

void destroi_objeto(Objeto_t*o, int n){
	for(int i = n; i < (o->qtd); i++) o->pos[i] = o->pos[i+1];
		o->qtd--;
}

void desenha_borda(){
	for(int i = 2; i < ALTURA; i++){ //vertical
		gotoxy(1,i);
		printf("||");
		gotoxy(LARGURA,i);
		printf("||");
	}
	for(int i = 1; i < LARGURA+2; i++){ //horizontal
		gotoxy(i,1);
		printf("=");	
		gotoxy(i,ALTURA);
		printf("=");
	} 
}


void desenha_tela(Jogador_t j, Objeto_t**p){
	desenha_borda();
    barra_info(j);
	coloca_objetos(p);
	gotoxy(j.pos.x,j.pos.y);
	printf("x\n");
}

void movimento(Jogador_t*j, char c){
	//if(c == 'k' && j->pos.y >INICIO_Y) j->pos.y--;
	//if(c == 'j' && j->pos.y <(ALTURA-1)) j->pos.y++;
	if(c == 'h' && j->pos.x >INICIO_X) j->pos.x--;
	if(c == 'l' && j->pos.x <(LARGURA-1)) j->pos.x++;
}

void movimento_projetil(Objeto_t*p){
	for(int i = 0; i < (p->qtd); i++){
		if(p->pos[i].y > INICIO_Y) p->pos[i].y--;
		else destroi_objeto(p, i);
	}
}

void movimento_inimigo(Objeto_t*p){
	for(int i = 0; i < (p->qtd); i++){
		if(p->pos[i].y < ALTURA-1){
			p->pos[i].y++;
		} else destroi_objeto(p, i);
	}
}

void gerar_objeto(Objeto_t*o, long aleatorio){
	if(aleatorio%o->raridade == 1 && o->qtd < o->qtd_max){
		o->pos[o->qtd] = (Posicao){aleatorio%(LARGURA-3)+3,INICIO_Y};
		o->qtd++;
	}
}

int objeto_tocou_objeto(Objeto_t o1, Objeto_t o2, int*retorno1, int*retorno2){
	for(int i = 0; i < (o1.qtd); i++){
		for(int j = 0; j < (o2.qtd); j++){
			if(o1.pos[i].x == o2.pos[j].x && o1.pos[i].y == o2.pos[j].y){
                *retorno1=i;
                *retorno2=j;   
                return 1;
            }
		}
	}
	return 0;
}

void tela_game_over(Jogador_t j, long tempo){
    printf("\e[1;1H\e[2J");
    printf("=======+GAME+OVER+=======\n");
    printf("%s\n",j.nome);
    printf("Pontos: %d\n", j.pontos);
    printf("Tempo total: %ld\n", tempo);
    printf("Pontuação total: %ld\n", tempo*j.pontos);
}
