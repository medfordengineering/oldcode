/* Needs work */
void button_init( void )
{
	/* Enable pin change interrupt on 16 to 23 */
	set_bit(PCICR, PCIE2);

	/* Unmask interrupt on 21, 22 and 23 */
	PCMSK2 = 0xE0;

	/* Enable pull-ups on PORTD 5, 6 and 7 */	
	PORTD = 0xE0;
}
