#include <REG51F.H>

#define VALOR_TH1 204 //When  SMOD == 1
#define BUFFER_SIZE 16

typedef char bool;
#define true 1
#define false 0

unsigned char byteFromSerial = 0x00;


typedef struct circularBuffer {
	
	unsigned char buffer[BUFFER_SIZE];
	unsigned char* head;
	unsigned char* tail;
	int length;
	bool full;
	
} CircularBuffer;

CircularBuffer rxBuffer;
CircularBuffer txBuffer;

void rxBuffer_init() {
	
	rxBuffer.head = rxBuffer.buffer;
	rxBuffer.tail = rxBuffer.head;
	rxBuffer.length = BUFFER_SIZE;
	rxBuffer.full = false;
	
}


void txBuffer_init() {
	
	txBuffer.head = txBuffer.buffer;
	txBuffer.tail = txBuffer.head;
	txBuffer.length = BUFFER_SIZE;
	txBuffer.full = false;
	
}

void movePtrInCircularBuffer(CircularBuffer* buffer, unsigned char** ptr) {

	unsigned char* endOfBuffer = ((*buffer).buffer) + ( ( (*buffer).length - 1) * sizeof(unsigned char));
	
	*ptr = *ptr + sizeof(unsigned char); // Move to next position	

	if (*ptr > endOfBuffer ) { // If ptr is out of array boundaries
		int position = (*ptr - endOfBuffer) % ((*buffer).length); // How many positions out of the limit
		*ptr = (*buffer).buffer + position; // Ptr goes back to right tposition
	}
	
}

char TxBufferVazio() {
	return ((txBuffer.full == false) & (txBuffer.head == txBuffer.tail)) ?  true : false;
}


char RxBufferVazio() {
	return ((rxBuffer.full == false) & (rxBuffer.head == rxBuffer.tail)) ?  true : false;
}


void sendChar(char c) {
	
	if (txBuffer.full) {
		movePtrInCircularBuffer(&txBuffer, &txBuffer.head);
	}	
		
	*(txBuffer.tail) = (unsigned char) c;
	movePtrInCircularBuffer(&txBuffer, &txBuffer.tail); //Move the pointer of the buffer in a Circular manner.
	
	if (txBuffer.head == txBuffer.tail) { //Adjust Full status and move Head along
		txBuffer.full = true;
	}
		
}


void sendString(char* s) {
	
	int i;
	
	for (i=0; s[i]!='\0'; i++) {
		sendChar(s[i]);		
	}
	
}


unsigned char receiveChar() {
		
	if (RxBufferVazio()) {
		return 0;
	}
	else {
		unsigned char receivedChar = *(rxBuffer.head);
		movePtrInCircularBuffer(&rxBuffer, &rxBuffer.head);
	
		if (rxBuffer.head == rxBuffer.tail) { //Check for buffer emptiness
			rxBuffer.full = false;
		}
		
		return receivedChar;
	}
}


void receiveString(char* s) {
		
		int i = 0;
	
		while(!RxBufferVazio()) {
			*(s + (i * sizeof(char))) = (char) receiveChar(); // Receive from buffer
			i++;
		}
		
		*(s + (i * sizeof(char))) = '\0'; //End String
	
}


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
	PCON = PCON | 0x80; // Setting SMOD(PCON 7th bit) to 1 for BaudRate control 
	
}


void serial_interrupt(void) interrupt 4 using 2 {
	
	if (TI) { // Message was Send
		TI = 0;
	}
	
	if (RI) { // Message was Received
		RI = 0;
	}
	
}


void main() {
	
	EA = 0; // Disable All Interruptions
	
	rxBuffer_init(); //Initialize Receive Buffer
	txBuffer_init(); //Initialize Transmission Buffer
		
	serial_init(); //Setting Serial Interface
	
	EA = 1; // Enable All Interruptions

	while(1) {
		 char oi = receiveChar();
		 
	}
}