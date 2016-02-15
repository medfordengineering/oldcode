#include <ms_ps2x.h>

volatile unsigned long t0_overflow_count = 0;
volatile unsigned long t0_ms = 0;
static unsigned char t0_fraction = 0;

static unsigned char enter_config[]={0x01,0x43,0x00,0x01,0x00};
static unsigned char set_mode[]={0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};
static unsigned char set_bytes_large[]={0x01,0x4F,0x00,0xFF,0xFF,0x03,0x00,0x00,0x00};
static unsigned char exit_config[]={0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};
static unsigned char enable_rumble[]={0x01,0x4D,0x00,0x00,0x01};
static unsigned char type_read[]={0x01,0x45,0x00,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A};

unsigned int last_buttons;
unsigned int buttons;
unsigned char read_delay;
unsigned char controller_type;
unsigned char en_Rumble;
unsigned char en_Pressures;
unsigned long last_read;
unsigned char PS2data[21];

ISR (TIMER2_OVF_vect)
{
	unsigned long m = t0_ms;
	unsigned char f = t0_fraction;

	m += MS_INC;
	f += FRACTION_INC;
	if (f >= FRACTION_MAX) {
		f -= FRACTION_MAX;
		m += 1;
	}

	t0_fraction = f;
	t0_ms = m;
	t0_overflow_count++;
}

unsigned long ms_count()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	cli();
	m = t0_ms;
	SREG = oldSREG;

	return m;
}

void ps2x_init(void)
{
	TCCR2A = (0<<WGM00);
	TCCR2B = (4<<CS00);
	TIMSK2 = (1<<TOIE2);
	sei();
}

long ms_map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned char NewButtonStateN() {
   return ((last_buttons ^ buttons) > 0);
}

unsigned char NewButtonState(unsigned int button) {
  return (((last_buttons ^ buttons) & button) > 0);
}

unsigned char ButtonPressed(unsigned int button) {
  return(NewButtonState(button) & Button(button));
}

unsigned char ButtonReleased(unsigned int button) {
  return((NewButtonState(button)) & ((~last_buttons & button) > 0));
}
  
unsigned char Button(uint16_t button) {
   return ((~buttons & button) > 0);
}

unsigned int ButtonDataByte() {
   return (~buttons);
}

unsigned char Analog(unsigned char button) {
  return PS2data[button];
}

unsigned char gp_shiftdata( unsigned char byte) 
{
	unsigned char old_sreg = SREG;
	unsigned char tmp = 0;
	unsigned char i;
	
	cli();
	for (i = 0; i < 8; i++) {
		if (tst_bit(byte, i)) 	//STC
		set_bit(PORTB, MSCMD);
	  else
		clr_bit(PORTB, MSCMD);
	   
	  clr_bit(PORTB, MSCLK);
	  SREG = old_sreg;  
	  
	  delay_us(CTRL_CLK);
	  cli();	
	  
	  if (tst_bit(PINB, MSDAT)) //STC
		set_bit(tmp, i);
	 
	   set_bit(PORTB, MSCLK);
   }
   set_bit(PORTB, MSCMD);	//STC
   SREG = old_sreg;   
   
   delay_us(CTRL_BYTE_DELAY);
   return tmp;
}
	
void read_gp(void)
{
	unsigned char motor1 = FALSE;
	unsigned char motor2 = 0x00;
	double temp;
	int i;
	unsigned char old_sreg = SREG;
	char dword[9] = {0x01,0x42,0,motor1,motor2,0,0,0,0};
	unsigned char dword2[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	
	//double temp = millis() - last_read;
	temp = ms_count() - last_read;
	
	if (temp > 1500) // waited too long
		reconfig_gamepad();
		
	if (temp < read_delay) //waited too short
		delay_ms(read_delay - temp);
		
	last_buttons = buttons;
	
	 if(motor2 != 0x00)
		motor2 = ms_map(motor2,0,255,0x40,0xFF); //noting below 40 will make it spin
		
	  cli();
		set_bit(PORTB, MSCMD);	//STC
		set_bit(PORTB, MSCLK);
		clr_bit(PORTB, MSATT);
	  SREG = old_sreg; 
	  
	delay_us(CTRL_BYTE_DELAY);

	for ( i = 0; i<9; i++) {
		PS2data[i] = gp_shiftdata(dword[i]);
	}
	if(PS2data[1] == 0x79) {  //if controller is in full data return mode, get the rest of data
       for ( i = 0; i<12; i++) {
			PS2data[i+9] = gp_shiftdata(dword2[i]);
       }
  }
  
  cli();
  set_bit(PORTB, MSATT);	//STC
  SREG = old_sreg;  // Interrupts may be enabled again    
	
   buttons = *(unsigned int*)(PS2data+3);   //store as one value for multiple functions
   //last_read = millis();
   last_read = ms_count();
}

unsigned char config_gp()
{
	unsigned char pressures = FALSE;
	unsigned char rumble = FALSE;
	unsigned char old_sreg = SREG;
	int y, i;
	unsigned char temp[sizeof(type_read)];
	
	set_bit(DDRB, MSCLK);			//STC
	set_bit(DDRB, MSATT);			//STC
	set_bit(DDRB, MSCMD);			//STC
	clr_bit(DDRB, MSDAT);			//STC
  
	set_bit(PORTB, MSDAT);	//STC
    
   cli();                          
 
	set_bit(PORTB, MSCMD);	//STC
	set_bit(PORTB, MSCLK);	//STC
	
   SREG = old_sreg;  
   
   read_gp();
   read_gp();
   
    if(PS2data[1] != 0x41 && PS2data[1] != 0x73 && PS2data[1] != 0x79)
	//see if mode came back. If still anything but 41, 73 or 79, then it's not talking
		return 1; //return error code 1
		
	read_delay = 1;
  
	for( y = 0; y <= 10; y++) {
		sendCMD(enter_config, sizeof(enter_config)); //start config run
   
		delay_us(CTRL_BYTE_DELAY);

		cli();                          // *** KJE *** disable for now
	
		set_bit(PORTB, MSCMD);
		set_bit(PORTB, MSCLK);
		clr_bit(PORTB, MSATT);
		SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
	
		delay_us(CTRL_BYTE_DELAY);

		for ( i = 0; i<9; i++) 
			temp[i] = gp_shiftdata(type_read[i]);
    

		cli();                          // *** KJE *** disable for now
	
		set_bit(PORTB, MSATT);
		SREG = old_sreg;  // *** *** KJE *** *** Interrupts may be enabled again 
	
		controller_type = temp[3];
   
		sendCMD(set_mode, sizeof(set_mode));
		if(rumble){ sendCMD(enable_rumble, sizeof(enable_rumble)); en_Rumble = TRUE; }
		if(pressures){ sendCMD(set_bytes_large, sizeof(set_bytes_large)); en_Pressures = TRUE; }
		sendCMD(exit_config, sizeof(exit_config));
   
		read_gp();
   
		if(pressures){
			if(PS2data[1] == 0x79)
				break;
			if(PS2data[1] == 0x73)
				return 3;
		}
   
		if(PS2data[1] == 0x73)
			break;
      
		if(y == 10){
		/*
		#ifdef PS2X_DEBUG
		Serial.println("Controller not accepting commands");
		Serial.print("mode stil set at");
		Serial.println(PS2data[1], HEX);
		#endif
		*/
      return 2; //exit function with error
	  }
    
    read_delay += 1; //add 1ms to read_delay
  }
   
 return 0; //no error if here

} 

void sendCMD(unsigned char string[], unsigned char len)
{
	unsigned char old_sreg = SREG;
	int y;
	
	cli();
	clr_bit(PORTB, MSATT);
	SREG = old_sreg;
	for (y = 0; y < len; y++)
		gp_shiftdata(string[y]);
		
	cli();
	set_bit(PORTB, MSATT);
	SREG = old_sreg;
	delay_ms(read_delay);
}

unsigned char readType() {

	if(controller_type == 0x03)
		return 1;
	else if(controller_type == 0x01)
		return 2;
	
	return 0;
	
}

void reconfig_gamepad(){
  
   sendCMD(enter_config, sizeof(enter_config));
   sendCMD(set_mode, sizeof(set_mode));
   if (en_Rumble)
      sendCMD(enable_rumble, sizeof(enable_rumble));
   if (en_Pressures)
      sendCMD(set_bytes_large, sizeof(set_bytes_large));
   sendCMD(exit_config, sizeof(exit_config));
   
}
