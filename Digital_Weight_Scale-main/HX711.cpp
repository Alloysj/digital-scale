#include "hx711.h"

long HX711_Buffer = 0;
long Weight_Tare = 0, Weight_Actual = 0;

//****************************************************
// Initialize HX711
//****************************************************
void Init_Hx711()
{
	pinMode(HX711_SCK, OUTPUT);	
	pinMode(HX711_DT, INPUT);
}

//****************************************************
// Get tare weight
//****************************************************
void Get_Tare()
{
	HX711_Buffer = HX711_Read();
	Weight_Tare = HX711_Buffer / 100;		
} 

//****************************************************
// Weighing function
//****************************************************
unsigned int Get_Weight()
{
	HX711_Buffer = HX711_Read();
	HX711_Buffer = HX711_Buffer / 100;

	Weight_Actual = HX711_Buffer;
	Weight_Actual = Weight_Actual - Weight_Tare; // Get actual item's AD sampled value.
	
	Weight_Actual = (unsigned int)((float)Weight_Actual / 4.11); 	
		// Calculate the actual weight of the item
		// Since different sensors have different characteristic curves, each sensor needs to calibrate this divisor (4.30).
		// If the measured weight is too high, increase this value.
		// If the measured weight is too low, decrease this value.
		// This value is generally around 7.16, depending on the sensor.
		// +0.05 is added to round to the nearest hundredth

	return Weight_Actual;
}

//****************************************************
// Read HX711
//****************************************************
unsigned long HX711_Read(void)	// Gain of 128
{
	unsigned long count; 
	unsigned char i;
	bool Flag = 0;

	digitalWrite(HX711_DT, HIGH);
	delayMicroseconds(1);

	digitalWrite(HX711_SCK, LOW);
	delayMicroseconds(1);

  	count = 0; 
  	while(digitalRead(HX711_DT)); 
  	for(i = 0; i < 24; i++)
	{ 
	  	digitalWrite(HX711_SCK, HIGH); 
		delayMicroseconds(1);
	  	count = count << 1; 
		digitalWrite(HX711_SCK, LOW); 
		delayMicroseconds(1);
	  	if(digitalRead(HX711_DT))
			count++; 
	} 
 	digitalWrite(HX711_SCK, HIGH); 
	count ^= 0x800000;
	delayMicroseconds(1);
	digitalWrite(HX711_SCK, LOW); 
	delayMicroseconds(1);
	
	return(count);
}
