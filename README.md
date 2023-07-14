# keynrf
nrf52840 keyboard perf

## Board
nRF52840-Dongle:
    
    Switches:
		SW1 Button	- GPIO 1.06
		SW2 Reset	- GPIO 0.18
	
    Leds:
        LD1 Green   - GPIO 0.06
		LD2 Red		- GPIO 0.08
		LD2 Green	- GPIO 1.09
		LD2 Blue	- GPIO 0.12

Perfboard:

	Button matrix:
		Row A 		- GPIO 0.02
		Row B 		- GPIO 1.15
		Row C 		- GPIO 1.13
		Row D 		- GPIO 1.10

		Col 1 		- GPIO 0.24
		Col 2 		- GPIO 1.00
		Col 3 		- GPIO 0.09
		Col 4 		- GPIO 0.10

	Rotary encoder:
		A    		- GPIO 0.13
		B 	    	- GPIO 0.15

	Display I2C:
		SCK 		- GPIO 0.31
		SDA 		- GPIO 0.29

	UART (midi):
		TX 	    	- GPIO 0.17
		RX 	    	- GPIO 0.20

