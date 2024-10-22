#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib_jogos.h"

#define gotoxy(x,y) printf("%c[%d;%df",0x1B,y,x)
#define limpa_tela() printf("\e[1;1H\e[2J")
int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;
 
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
  ch = getchar();
 
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }
 
  return 0;
}

int main(){
	Jogador_t j = {.pos = (Posicao){INICIO_X,ALTURA-1}, .vida=5, .pontos=0};
	time_t t_inicio, t_atual, t_total;
	Objeto_t projetil = {.qtd=0, .identificador='|'}, inimigo={.qtd=0,.identificador='H',.qtd_max=10, .raridade=5};
	Objeto_t*p_obj[QTD_OBJETOS] = {&projetil, &inimigo};
	system ("/bin/stty raw");
	char c;
	int retorno1, retorno2, mover = 1, trava = 1;
	long aleatorio;
	
	time(&t_inicio);
	srand(time(NULL));
	while(c != 'q' && j.vida>0){
		time(&t_atual);
		c=0;
		if(kbhit()) c = getc(stdin);
		limpa_tela();
		movimento_projetil(&projetil);
		if((t_atual%t_inicio)%2 == 0) mover = 1;
		else{
			mover = 0;
			trava = 1;
		}
		if(mover && trava){
			trava = 0;
			movimento_inimigo(&inimigo);
		}
		movimento(&j,c);
		aleatorio = rand();
		gerar_objeto(&inimigo, aleatorio);
		if(objeto_tocou_objeto(projetil, inimigo,&retorno1,&retorno2)) {
			gotoxy(inimigo.pos[retorno2].x,inimigo.pos[retorno2].y);
			printf(" BOOOM\n");
			destroi_objeto(&projetil, retorno1);
			destroi_objeto(&inimigo, retorno2);
            j.pontos++;
		}
		desenha_tela(j,p_obj);	
		gotoxy(1, ALTURA+1);
		printf("%ld\n", (t_total = t_atual%t_inicio));
		        for(int i=0; i < inimigo.qtd; i++){
            if(inimigo.pos[i].y==ALTURA-1){
                j.vida--;
			    destroi_objeto(&inimigo, i);
            }
        }
        usleep(.05*1000000);
		if(c == ' '){
			projetil.qtd++;
			projetil.pos[projetil.qtd-1] = j.pos;
		};

	}
	system ("/bin/stty cooked");
    tela_game_over(j,t_total);
	return 0;
}
