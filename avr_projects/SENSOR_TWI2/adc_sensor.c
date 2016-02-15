#include <analog.h>

signed int convert_acceleration( signed int sensorValue)
{
	/* Conversion for accelleration */
	return (sensorValue - 512) * 10;
}

signed int convert_temperature( signed int sensorValue)
{
	/* CHECK ACCURACY 3 ~ 3.22 */
	return ((3 * sensorValue) - 600) / 10;
}

void record_analog(void)
{
	signed int values;
	
	/* Record sound */
	values = adc_read(0);
	gpsParsed[30] = (char) values;
	gpsParsed[31] = (char) (values >> 8);

	/* Record X axis */
	values = convert_acceleration(adc_read(1));
	gpsParsed[32] = (char) values;
	gpsParsed[33] = (char) (values >> 8);

	/* Record Y axis */
	values = convert_acceleration(adc_read(2));
	gpsParsed[34] = (char) values;
	gpsParsed[35] = (char) (values >> 8);

	/* Record Z axis */
	values = convert_acceleration(adc_read(3));
	gpsParsed[36] = (char) values;
	gpsParsed[37] = (char) (values >> 8);

	/* Record temperature */
	values = convert_temperature(adc_read(6));
 	gpsParsed[38] = (char) values;
	gpsParsed[39] = (char) (values >> 8);

	/* Record light */
	values = adc_read(7);
	gpsParsed[40] = (char) values;
	gpsParsed[41] = (char) (values >> 8);
}

