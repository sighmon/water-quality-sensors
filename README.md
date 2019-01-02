# Water quality sensors

Measuring water quality using a conductivity sensor and a The Things Uno.

<img src="atlas-scientific-conductivity-to-the-things-uno.jpg" width="100%">

## Hardware

* [Atlas Scientific Conductivity K 1.0 sensor kit](https://www.atlas-scientific.com/product_pages/kits/ec_k1_0_kit.html)
* [The Things Uno](https://www.thethingsnetwork.org/docs/devices/uno/)

## Wiring

Atlas sensor pin | Power isolator pin
--- | ---
VCC | 3.9V
GND | GND
TX | A (In)
RX | B (In)

Power isolator pin | Arduino The Things Uno (Leonardo) pin
--- | ---
VCC | 5V
GND | GND
A (Out) | 10 (Digital)
B (Out) | 11 (Digital)

## Output

Once running, you should see this debug output to your serial monitor, where:

* EC = conductivity
* TDS = total dissolved solids
* SAL = salinity
* GRAV = specific gravity (sea water only)

```bash
-- STATUS
EUI: <EUI>
Battery: 3253
AppEUI: <APP_EUI>
DevEUI: <DEV_EUI>
Data Rate: 3
RX Delay 1: 1000
RX Delay 2: 2000

-- SENSOR READING
EC:0.00
TDS:0
SAL:0.00
GRAV:1.000

-- BYTE ARRAY TO SEND
30 2E 30 30 2C 30 2C 30 2E 30 30 2C 31 2E 30 30 30 00 

Sending: mac tx uncnf 1 302E30302C302C302E30302C312E30303000
```

## Sample readings

Here are some sample readings from Port Willunga, South Australia.

```bash
// Fresh water
EC:601.2
TDS:325
SAL:0.29
GRAV:1.000

// Beach water
EC:51631
TDS:27881
SAL:33.94
GRAV:1.026
```
