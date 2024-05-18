# CTS-Solar-Panel-Load-Tester
A solar cell/panel testing device which places a variable load on a panel to characterize its IV/PV curve

This project is heavily inspired by similar open-source designs - https://circuitdigest.com/microcontroller-projects/adustable-electronic-dc-load-using-arduino

## Description

An Arduino controls a MOSFET, acting as a resistor load, to allow varying current through the MOSFET from the solar panels. Two measures are used to monitor current - an INA219 current sensor, and a shunt resistor with an op-amp network. An ADC measures the total cell/panel voltage.

The Arduino logs the voltage/current values over UART to a computer.

## Notes

* R_shunt = Full Scale Voltage / Max Expected Load Current =  40mV / 0.5A = 80mOhms, so 0.1 ohms is reasonable

## License

The software is licensed under [The Unlicense](https://choosealicense.com/licenses/unlicense/). The hardware is licensed under the [CERN Open Hardware Licence Version 2 - Permissive](https://choosealicense.com/licenses/cern-ohl-p-2.0/).

Please see the respective license documents in each sub-folder.

## Similar Projects

* https://jasper.sikken.nl/electronicload/index.html
    * Uses an [IRLZ44Z MOSFET](https://www.infineon.com/dgdl/irlz44zpbf.pdf?fileId=5546d462533600a4015356722836272a)
    * Archived on [the Wayback Machine](https://web.archive.org/web/20240229184606/https://jasper.sikken.nl/electronicload/index.html)
    
