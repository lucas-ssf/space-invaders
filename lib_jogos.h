#include "config.h"

void coloca_objetos(Objeto_t**p);

void destroi_objeto(Objeto_t*o, int n);

void desenha_tela(Jogador_t j, Objeto_t**p);

void movimento(Jogador_t*j, char c);

void movimento_projetil(Objeto_t*p);

void movimento_inimigo(Objeto_t*p);

void desenha_borda();

void gerar_objeto(Objeto_t*o, long aleatorio);

void tocou_objeto(Jogador_t*j, Objeto_t*o);

int objeto_tocou_objeto(Objeto_t o1, Objeto_t o2, int*retorno1, int*retorno2);

void tela_game_over(Jogador_t j, long tempo);
