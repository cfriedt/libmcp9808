#include <iostream>

#include <stdlib.h>
#include <unistd.h>

#include <libgpio/Gpio.h>
#include <libi2c/I2c.h>
#include <libat24/At24.h>
#include <libmcp9808/Mcp9808.h>

using namespace ::std;
using namespace ::com::github::cfriedt;

#define EXAMPLE1_I2C_BUS 0
#define EXAMPLE1_GPIO    496
#define TLOWER_F (-5.0f)
#define TUPPER_F (+25.0f)
#define TCRIT_F (+100.0f)

int main( int argc, char *argv[] ) {

	int r;

	At24 at;
	Gpio gpio;
	Mcp9808 mcp;

	mcp9808_config_t config;
	mcp9808_temperature_t tambient;
	mcp9808_temperature_t tcrit;
	mcp9808_temperature_t tupper;
	mcp9808_temperature_t tlower;

	at = At24( EXAMPLE1_I2C_BUS, I2C_ADDR_AT24_MIN );
	gpio = Gpio( EXAMPLE1_GPIO, GPIO_EDGE_FALLING );
	mcp = Mcp9808( EXAMPLE1_I2C_BUS, I2C_ADDR_MCP9808_MIN );

	cout << "enabling interrupts in mcp9808" << endl;
	config = mcp.config();
	config.value.vector.alert_enabled = true;
	mcp.config( config );

	mcp.tlower( Mcp9808::degreesCelciusToTemperature( TLOWER_F ) );
	mcp.tupper( Mcp9808::degreesCelciusToTemperature( TUPPER_F ) );
	mcp.tcrit( Mcp9808::degreesCelciusToTemperature( TCRIT_F ) );

	for( ;; ) {
		cout << "waiting on interrupt from gpio" << EXAMPLE1_GPIO << endl;
		gpio.wait();
		cout << "received interrupt from gpio" << EXAMPLE1_GPIO << endl;
		if ( GPIO_VALUE_LOW != gpio.value() ) {
			cout << "received interrupt from gpio" << EXAMPLE1_GPIO << " but line is high - spurious?" << endl;
			continue;
		}
		for( ; GPIO_VALUE_LOW == gpio.value(); ) {

			cout << "gpio" << EXAMPLE1_GPIO << " line is low" << endl;

			config = mcp.config();
			tambient = mcp.tambient();
			tcrit = mcp.tcrit();
			tupper = mcp.tupper();
			tlower = mcp.tlower();

			if ( tambient.value.vector.ta_ge_tcrit ) {
				cout << "tambient ( " << Mcp9808::temperatureToDegreesCelcius( tambient ) << " ) >= tcrit ( " << Mcp9808::temperatureToDegreesCelcius( tcrit ) << " )" << endl;
			}

			if (
				true
				&&
					MCP9808_ALERT_OUTPUT_SELECT_TUPPER_TLOWER_TCRIT
					==
					config.value.vector.alert_select
			) {
				if ( tambient.value.vector.ta_gt_tupper ) {
					cout << "tambient ( " << Mcp9808::temperatureToDegreesCelcius( tambient ) << " ) > tupper ( " << Mcp9808::temperatureToDegreesCelcius( tupper ) << " )" << endl;
				}

				if ( tambient.value.vector.ta_lt_tlower ) {
					cout << "tambient ( " << Mcp9808::temperatureToDegreesCelcius( tambient ) << " ) > tupper ( " << Mcp9808::temperatureToDegreesCelcius( tlower ) << " )" << endl;
				}
			}

			cout << "writing 'interrupt clear' to mcp" << endl;
			config.value.vector.interrupt_clear = true;
			mcp.config( config );

			for( ; GPIO_VALUE_LOW == gpio.value(); ) {
				cout << "waiting for 'interrupt clear' to change value of gpio" <<  EXAMPLE1_GPIO << endl;
				usleep( 10000 );
			}
		}
	}

	r = EXIT_SUCCESS;

	return r;
}
