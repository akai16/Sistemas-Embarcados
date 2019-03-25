#include <REG51F.H>
#define FrClk 12000000
#define FreqTimer0_emHz 100
#define CORRECAO 6
#define VALOR_TH0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) >>8) 
#define VALOR_TL0 ((65536 - (FrClk / (12 * FreqTimer0_emHz)) + CORRECAO) & 0xFF)
//Calcule o valor de CORRECAO!!!

sbit p2bit0 = P2^0;
sbit p2bit1 = P2^1;

unsigned char contadorTimerL = 0;
unsigned char contadorTimerH = 0;
char deveContarL = 0;
char deveContarH = 0;

void timer0_inicializa() {
  TR0 = 0;              // Desliga Timer0
  TMOD = (TMOD & 0xF0) | 0x01;  // Timer 0 programado como timer de 16 bits
  TH0 = VALOR_TH0;  // Programa contagem do Timer0
  TL0 = VALOR_TL0;
  ET0 = 1;      // Habilita interrupcao do timer 0
  TR0 = 1;      // Habilita contagem do timer 0
}

void timer0_int (void) interrupt 1 using 2 {
  TR0 = 0;              // Desliga Timer0
  
  TL0 += VALOR_TL0; // Programa contagem do Timer0
  TH0 += VALOR_TH0 + (unsigned char) CY;
  TR0 = 1;      // Habilita contagem do timer 0
  
	if (deveContarL) {
		contadorTimerL++;
	}
	if (deveContarH) {
		contadorTimerH++;
	}
} 


void funcL(){
	static enum estados_enum {inicial, estadotransicao, estadoEspera} estados;

	unsigned long countMax = 20000;
	
	switch(estados){
		case inicial:
			if(!p2bit0){
				estados = estadotransicao;
			}
		break;
		case estadotransicao:
			if(p2bit0){
				P1 = (P1 & 0x0F) | (P0 & 0xF0);
				estados = estadoEspera;
			}
			
		break;	
		case estadoEspera:
			deveContarL = 1;
			if (contadorTimerL >= 100) {
				P1 = P1 & 0x0F;
				estados = inicial;
				deveContarL = 0;
				contadorTimerL = 0;
			}		
		break;
	}
}
	

void funcH(){
	static enum estados_enum {inicial, estadotransicao, estadoEspera} estados;

	unsigned long countMax = 20000;
	
	switch(estados){
		case inicial:
			if(!p2bit1){
				estados = estadotransicao;
			}
		break;
		case estadotransicao:
			if(p2bit1){
				P1 = (P1 & 0xF0) | (P0 & 0x0F);
				estados = estadoEspera;
			}
			
		break;	
		case estadoEspera:
			deveContarH = 1;
			if (contadorTimerH >= 100) {
				P1 = P1 & 0xF0;
				estados = inicial;
				deveContarH = 0;
				contadorTimerL = 0;
			}		
		break;
	}
}


void main() {
	EA=0;
	timer0_inicializa(); 
	EA=1; //Habilita o tratamento de interrup��es

	while(1){
		
		funcL();
		funcH();
		
	}
	
}