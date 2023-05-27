___
# Smart soil moisture controlling system using ESP8266
___

Here we are building an IoT-based Irrigation System using ESP8266 NodeMCU Module, Soil moisture sensor, and DHT11 Sensor. The System will consist of a water pump which will be used to sprinkle water on the land depending upon the land's environmental conditions such as Moisture, Temperature, and Humidity. The system will maintain the soil moisture based on the temperature of the environment.

## Hardware design
![Hardware design](./docs/images/Harware design.png)

## Soil moisture sensor

To monitor the moisture content of the soil, the soil moisture sensor is frequently used in smart agriculture or other garden automation projects. It has four pins, of which two, labeled Vcc and Gnd, are used to supply voltage. The final two pins are the output pins, which are digital (D0) and analog (A0). The output of the digital pin (D0) will become low if the soil moisture content rises above the threshold level (the output of the digital pin is either logic 0 or 1). By adjusting the onboard potentiometer, the sensor module's threshold value can be changed. The approximate moisture content of the soil can be determined using the analog output pin.
When purchased, a single unit of soil moisture sensor module will be shipped with the sensor probes and the measurement module. The measurement module will be wired to the sensor probe, which will be dipped in the soil. Using the LM393 OP-Amp comparator, the measurement module will compare the measured value with the threshold value (which may be set using a 10k pot), and then output on the digital pin. The measuring module and sensor probe are both seen below.

### Specifications of soil moisture sensor
Operating voltage (Vcc): 3.3V to 5V
Analog Output voltage: 0 to 4.2V@ Vcc = 5V
Digital Output Voltage: 0V or 5V @ Vcc=5v
Current consumption: 32mA

Because of the presence of the onboard Lm393 comparator circuit, it can also be used directly in the project without connecting a microcontroller unit (MCU) to it. When the moisture content of the soil is higher than the threshold value, the digital output of the moisture sensor is logic 0 (0V). The output from the digital pin can be used to drive an LED or a buzzer circuit.
Also, the Digital and Analog Output pins can be connected with a microcontroller like Arduino to built measure and monitor the soil moisture sensor and build interesting projects.

## DHT11–Temperature and Humidity Sensor

The DHT11 is a commonly used Temperature and humidity sensor. The sensor comes with a dedicated NTC to measure temperature and an 8-bit microcontroller to output the values of temperature and humidity as serial data. The sensor is also factory calibrated and hence easy to interface with other microcontrollers.

### Specifications of DHT11

Operating Voltage: 3.5V to 5.5V
Operating current: 0.3mA (measuring) 60uA (standby)
Output: Serial data
Temperature Range: 0°C to 50°C
Humidity Range: 20% to 90%
Resolution: Temperature and Humidity both are 16-bit
Accuracy: ±1°C and ±1%
	
### Water pump - 3V To 12V Mini DC Submersible Pump

This DC-operated mini submersible water pump is ideal for small vending machines and other applications, in which a small amount of water has to be pumped. The operating voltage for this DC pump is between 3 to 12V and can be easily controlled with development boards like Arduino, Raspberry Pi, ESP, and other microcontrollers, so also frequently used in DIY electronics projects and hobby projects. The pumping height for this mini DC water pump is between 40 to 110 cm.


For programming the ESP8266 NodeMCU module, only the DHT11 sensor library is used as an external library. The moisture sensor gives an analog output that can be read through the ESP8266 NodeMCU analog pin A0. Since the NodeMCU cannot give an output voltage greater than 3.3V from its GPIO so we are using a relay module to drive the 5V motor pump. Also, the Moisture sensor and DHT11 sensor are powered by an external 5V power supply
