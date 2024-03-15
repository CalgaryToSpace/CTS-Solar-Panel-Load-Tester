# CTS-Solar-Panel-Load-Tester
A solar cell/panel testing device which places a variable load on a panel to characterize its IV/PV curve

## Description

An Arduino controls a MOSFET, acting as a resistor load, to allow varying current through the MOSFET from the solar panels. An INA219 current sensor measures the current flowing, and an ADC measures the total cell/panel voltage.

The Arduino logs the voltage/current values over UART to a computer.


## License

The software is licensed under [The Unlicense](https://choosealicense.com/licenses/unlicense/). The hardware is licensed under the [CERN Open Hardware Licence Version 2 - Permissive](https://choosealicense.com/licenses/cern-ohl-p-2.0/).

Please see the respective license documents in each sub-folder.
