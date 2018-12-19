#include <PR_2PosRegulator.hpp>


#define	REG_HYST_L					    1	    //[C]
#define	REG_HYST_H					    3	    //[C]
#define	REG_LOGIC					    false    // true mean direct, false means inverse
#define	REG_SWITCHPERIOD				000  	//[ms]




PR_2PosRegulator	regulator(REG_HYST_L, REG_HYST_H, REG_LOGIC, REG_SWITCHPERIOD);

#define	TEST_VALUE_MIN		-5.1		//[C]
#define	TEST_VALUE_MAX		+20.1		//[C]
#define	TEST_VALUE_STEP		0.2			//[C]

#define	TEST_TAGET			10.0			//[C]
#define	TEST_MODE			true			//true means "on", false means "off"

float	test_target = TEST_TAGET;
float	test_current = TEST_VALUE_MIN;


float	TEST_device_under_regulator(const float target, const float last_current) {
	
	float new_current;
	
	static bool direction = true;
	
	//going from TEST_VALUE_MIN up to TEST_VALUE_MAX then down
	if ( last_current >= TEST_VALUE_MAX ) direction = false;
	else if ( last_current <= TEST_VALUE_MIN ) direction = true;
	
	
	if ( direction ) new_current = last_current + TEST_VALUE_STEP; else new_current = last_current - TEST_VALUE_STEP;

	return new_current;
}



void setup() {
	
	Serial.begin(115200);
	Serial.flush();
	
	
	Serial.println();
	
	Serial.println("REG_HYST_L= " + String(REG_HYST_L) );
	Serial.println("REG_HYST_H= " + String(REG_HYST_H) );
	Serial.println("REG_LOGIC= " + String( REG_LOGIC ? "direct" : "inverse" ) );
	Serial.println("REG_SWITCHPERIOD= " + String(REG_SWITCHPERIOD) );
	
	Serial.println();
	
	regulator.setTarget(TEST_TAGET);
	Serial.println("Target was set to= " + String(TEST_TAGET) );
	Serial.println("Target result is= " + String(regulator.getTarget()) );
	
	Serial.println();
	
	Serial.println("Set mode to="+ String(TEST_MODE));
	regulator.setMode(TEST_MODE);
	Serial.println("Mode result is= " + String(regulator.getMode()) );
	Serial.println();
	
	
}

#define	TEST_TARGET		10		/[C]

void loop() {
	
	bool	regulator_out;
	test_current = TEST_device_under_regulator(test_target, test_current);
	
	regulator_out = regulator.loop(test_current);
	
	Serial.println("target= " + String(test_target) + "\t current= " + String(test_current) + "\t output= " + String(regulator_out));
	
	//Serial.println("Set mode to off");
	
	delay(50);
}



