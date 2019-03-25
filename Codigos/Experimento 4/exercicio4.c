#include <REG51F.H>

//#define VALOR_TH1 230 //When SMOD == 0 
#define VALOR_TH1 204 //When  SMOD == 1 (Works Best)

unsigned char byteFromSerial = 0x00;

void timer1_init() {
	TR1 = 0; // Disable Timer 1
	
	TMOD = (TMOD & 0x0F) | 0x20; // Setting Timer 1 in Mode 2
	TL1 = 0x00; // Reseting Timer 1
  TH1 = VALOR_TH1;  // Setting Refresher Register to achieve desired Baudrate
	
	TR1 = 1; // Enable Timer 1
}


void serial_init() {
	ES = 1; //Enable Serial Interruption

	timer1_init(); // Setting Timer 1 in Mode 2
	SCON = SCON | 0x50; // Setting Serial Interface to Mode 1 
	
	//PCON = PCON & 0x7F; // Setting SMOD(PCON 7th bit) to 0 for BaudRate control 
	PCON = PCON | 0x80; // Setting SMOD(PCON 7th bit) to 1 for BaudRate control 
}


void serial_int(void) interrupt 4 using 2 {
	if (TI) { //When message was Send
		TI = 0;
	}
	
	if (RI) { //When message was Received
		RI = 0;
		
		byteFromSerial = SBUF; // Receive
		
		byteFromSerial++;
		
		SBUF = byteFromSerial; // Transmit
	}
}


void main() {
	EA = 0; // Disable All Interruptions
	
	serial_init(); //Setting Serial Interface
	
	EA = 1; // Enable All Interruptions
	
	
	while(1) {
		//Do Nothing
		//Wait for Serial Interruptions
	}
}