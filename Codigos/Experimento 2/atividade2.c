#include <REG51F.H>

sbit p2bit0 = P2^0;
sbit p2bit1 = P2^1;


void funcL(){
	static enum estados_enum {inicial, estadotransicao, estadoEspera} estados;
	
	
	static unsigned long count = 0;
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
			count++;
			if(count > countMax){
				count = 0;
				P1 = P1 & 0x0F;
				estados = inicial;
			}
		break;
	}
}
	

void funcH(){
	static enum estados_enum {inicial, estadotransicao, estadoEspera} estados;
	
	
	static unsigned long count = 0;
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
			count++;
			if(count > countMax){
				count = 0;
				P1 = P1 & 0xF0;
				estados = inicial;
			}
		break;
	}
}


void main() {

	while(1){
		
		funcL();
		funcH();
		
	}
	
}