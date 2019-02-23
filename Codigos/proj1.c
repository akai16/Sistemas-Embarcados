#include <REG51F.H>

sbit p2bit0 = P2^0;
sbit p2bit1 = P2^1;


void main() {

	while(1){
		if (p2bit0 && p2bit1) {
			P1 = P0;
		}
		else if (p2bit0 == 0 && p2bit1== 0) {
			P1 = 0;
		}
		else if (p2bit0) {
			P1 = P0 & 15;	
		}
		else {
			P1 = P0 & 240;
		}
		
	}
	

}