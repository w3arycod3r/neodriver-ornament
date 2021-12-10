EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Neo Driver"
Date "2021-03-18"
Rev "v03"
Comp ""
Comment1 ""
Comment2 "creativecommons.org/licenses/by/4.0"
Comment3 "License: CC BY 4.0"
Comment4 "Author: Garrett Carter"
$EndDescr
$Comp
L Neo_Driver-rescue:VCC-power #PWR05
U 1 1 5EB7D3C7
P 4000 3200
F 0 "#PWR05" H 4000 3050 50  0001 C CNN
F 1 "VCC" H 4017 3373 50  0000 C CNN
F 2 "" H 4000 3200 50  0001 C CNN
F 3 "" H 4000 3200 50  0001 C CNN
	1    4000 3200
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:ATtiny85-20PU-MCU_Microchip_ATtiny U1
U 1 1 5EB73D8D
P 4000 3850
F 0 "U1" H 3600 4400 50  0000 R CNN
F 1 "ATtiny85" H 4500 3300 50  0001 R CNN
F 2 "Package_DIP:DIP-8_W7.62mm_Socket" H 4000 3850 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 4000 3850 50  0001 C CNN
	1    4000 3850
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:GND-power #PWR06
U 1 1 5EB7C5F4
P 4000 4500
F 0 "#PWR06" H 4000 4250 50  0001 C CNN
F 1 "GND" H 4005 4327 50  0000 C CNN
F 2 "" H 4000 4500 50  0001 C CNN
F 3 "" H 4000 4500 50  0001 C CNN
	1    4000 4500
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:Battery_Cell-Device BT1
U 1 1 5EB7F7B1
P 2500 3700
F 0 "BT1" H 2618 3796 50  0000 L CNN
F 1 "3.3V" H 2618 3705 50  0000 L CNN
F 2 "Neo_Driver:LiFePO4wered_USB_Slim" V 2500 3760 50  0001 C CNN
F 3 "~" V 2500 3760 50  0001 C CNN
	1    2500 3700
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:VCC-power #PWR01
U 1 1 5EB81E86
P 2600 2400
F 0 "#PWR01" H 2600 2250 50  0001 C CNN
F 1 "VCC" H 2617 2573 50  0000 C CNN
F 2 "" H 2600 2400 50  0001 C CNN
F 3 "" H 2600 2400 50  0001 C CNN
	1    2600 2400
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:GND-power #PWR02
U 1 1 5EB82B75
P 2500 3950
F 0 "#PWR02" H 2500 3700 50  0001 C CNN
F 1 "GND" H 2505 3777 50  0000 C CNN
F 2 "" H 2500 3950 50  0001 C CNN
F 3 "" H 2500 3950 50  0001 C CNN
	1    2500 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3350 2500 3500
Wire Wire Line
	2500 3800 2500 3950
$Comp
L Neo_Driver-rescue:GND-power #PWR016
U 1 1 5EB8CB53
P 7200 2350
F 0 "#PWR016" H 7200 2100 50  0001 C CNN
F 1 "GND" H 7205 2177 50  0000 C CNN
F 2 "" H 7200 2350 50  0001 C CNN
F 3 "" H 7200 2350 50  0001 C CNN
	1    7200 2350
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:VCC-power #PWR017
U 1 1 5EB8D4C4
P 8450 2150
F 0 "#PWR017" H 8450 2000 50  0001 C CNN
F 1 "VCC" H 8467 2323 50  0000 C CNN
F 2 "" H 8450 2150 50  0001 C CNN
F 3 "" H 8450 2150 50  0001 C CNN
	1    8450 2150
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:VCC-power #PWR014
U 1 1 5EB9B20E
P 7150 3600
F 0 "#PWR014" H 7150 3450 50  0001 C CNN
F 1 "VCC" H 7167 3773 50  0000 C CNN
F 2 "" H 7150 3600 50  0001 C CNN
F 3 "" H 7150 3600 50  0001 C CNN
	1    7150 3600
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:GND-power #PWR015
U 1 1 5EB9B968
P 7450 5450
F 0 "#PWR015" H 7450 5200 50  0001 C CNN
F 1 "GND" H 7455 5277 50  0000 C CNN
F 2 "" H 7450 5450 50  0001 C CNN
F 3 "" H 7450 5450 50  0001 C CNN
	1    7450 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 4400 7450 4750
Wire Wire Line
	7300 3750 7150 3750
Wire Wire Line
	7150 3750 7150 3600
Wire Wire Line
	7650 2850 7650 2750
$Comp
L Neo_Driver-rescue:AVR-ISP-6-Connector J2
U 1 1 5EB839E3
P 7750 2350
F 0 "J2" V 7283 2400 50  0000 C CNN
F 1 "ISP" V 7374 2400 50  0001 C CNN
F 2 "Connector_IDC:IDC-Header_2x03_P2.54mm_Horizontal" V 7500 2400 50  0001 C CNN
F 3 " ~" H 6475 1800 50  0001 C CNN
	1    7750 2350
	0    1    1    0   
$EndComp
$Comp
L Neo_Driver-rescue:VCC-power #PWR013
U 1 1 5EBA6EED
P 6850 2150
F 0 "#PWR013" H 6850 2000 50  0001 C CNN
F 1 "VCC" H 6867 2323 50  0000 C CNN
F 2 "" H 6850 2150 50  0001 C CNN
F 3 "" H 6850 2150 50  0001 C CNN
	1    6850 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 2150 6850 2300
Wire Wire Line
	6850 2600 6850 2850
$Comp
L Neo_Driver-rescue:R-Device R1
U 1 1 5EBA5447
P 6850 2450
F 0 "R1" H 6920 2496 50  0000 L CNN
F 1 "10K" H 6920 2405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6780 2450 50  0001 C CNN
F 3 "~" H 6850 2450 50  0001 C CNN
	1    6850 2450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7750 3150 7750 2750
Wire Wire Line
	7850 3150 7850 2750
Wire Wire Line
	7950 3150 7950 2750
$Comp
L Neo_Driver-rescue:GND-power #PWR010
U 1 1 5EBCEFD2
P 5950 3500
F 0 "#PWR010" H 5950 3250 50  0001 C CNN
F 1 "GND" H 5955 3327 50  0000 C CNN
F 2 "" H 5950 3500 50  0001 C CNN
F 3 "" H 5950 3500 50  0001 C CNN
	1    5950 3500
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:R_POT-Device RV1
U 1 1 5EBD7279
P 5250 5000
F 0 "RV1" V 5135 5000 50  0000 C CNN
F 1 "10K" V 5044 5000 50  0000 C CNN
F 2 "Potentiometer_THT:Potentiometer_Bourns_3386P_Vertical" H 5250 5000 50  0001 C CNN
F 3 "~" H 5250 5000 50  0001 C CNN
	1    5250 5000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 5000 5100 5000
$Comp
L Neo_Driver-rescue:VCC-power #PWR09
U 1 1 5EBDBB06
P 5600 4950
F 0 "#PWR09" H 5600 4800 50  0001 C CNN
F 1 "VCC" H 5617 5123 50  0000 C CNN
F 2 "" H 5600 4950 50  0001 C CNN
F 3 "" H 5600 4950 50  0001 C CNN
	1    5600 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 5000 5600 5000
Wire Wire Line
	5600 5000 5600 4950
Wire Wire Line
	4600 3950 5250 3950
Wire Wire Line
	5250 3950 5250 4200
$Comp
L Neo_Driver-rescue:R-Device R2
U 1 1 5EBDFE22
P 6700 3850
F 0 "R2" V 6493 3850 50  0000 C CNN
F 1 "330" V 6584 3850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6630 3850 50  0001 C CNN
F 3 "~" H 6700 3850 50  0001 C CNN
	1    6700 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	6850 3850 7300 3850
$Comp
L Neo_Driver-rescue:CP-Device C2
U 1 1 5EBEAD1A
P 8100 3850
F 0 "C2" H 8218 3896 50  0000 L CNN
F 1 "100uF" H 8218 3805 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D5.0mm_P2.00mm" H 8138 3700 50  0001 C CNN
F 3 "~" H 8100 3850 50  0001 C CNN
	1    8100 3850
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:VCC-power #PWR011
U 1 1 5EBECEBD
P 8100 3600
F 0 "#PWR011" H 8100 3450 50  0001 C CNN
F 1 "VCC" H 8117 3773 50  0000 C CNN
F 2 "" H 8100 3600 50  0001 C CNN
F 3 "" H 8100 3600 50  0001 C CNN
	1    8100 3600
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:GND-power #PWR012
U 1 1 5EBED4C6
P 8100 4100
F 0 "#PWR012" H 8100 3850 50  0001 C CNN
F 1 "GND" H 8105 3927 50  0000 C CNN
F 2 "" H 8100 4100 50  0001 C CNN
F 3 "" H 8100 4100 50  0001 C CNN
	1    8100 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 3600 8100 3700
Wire Wire Line
	8100 4000 8100 4100
Text Label 7950 3150 1    50   ~ 0
MISO
Text Label 7850 3150 1    50   ~ 0
MOSI
Text Label 7750 3150 1    50   ~ 0
SCK
Text Label 7650 3150 1    50   ~ 0
~RST
Wire Wire Line
	7650 2850 7650 3150
Connection ~ 7650 2850
Text Label 4850 4050 2    50   ~ 0
~RST
Wire Wire Line
	4600 4050 4850 4050
Text Label 4850 3650 2    50   ~ 0
MISO
Text Label 4850 3750 2    50   ~ 0
SCK
Text Label 4850 3550 2    50   ~ 0
MOSI
Wire Wire Line
	4600 3750 4850 3750
Wire Wire Line
	4600 3550 5250 3550
Wire Wire Line
	4600 3650 5350 3650
Wire Wire Line
	6850 2850 7650 2850
$Comp
L Neo_Driver-rescue:C-Device C1
U 1 1 5FD88BDC
P 3250 3800
F 0 "C1" H 3050 3800 50  0000 L CNN
F 1 "100nF" H 2900 3700 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D3.8mm_W2.6mm_P2.50mm" H 3288 3650 50  0001 C CNN
F 3 "~" H 3250 3800 50  0001 C CNN
	1    3250 3800
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:VCC-power #PWR03
U 1 1 5FD9606F
P 3250 3600
F 0 "#PWR03" H 3250 3450 50  0001 C CNN
F 1 "VCC" H 3267 3773 50  0000 C CNN
F 2 "" H 3250 3600 50  0001 C CNN
F 3 "" H 3250 3600 50  0001 C CNN
	1    3250 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3600 3250 3650
Wire Wire Line
	4000 3200 4000 3250
Wire Wire Line
	4000 4450 4000 4500
$Comp
L Neo_Driver-rescue:GND-power #PWR04
U 1 1 5FD995F5
P 3250 4000
F 0 "#PWR04" H 3250 3750 50  0001 C CNN
F 1 "GND" H 3255 3827 50  0000 C CNN
F 2 "" H 3250 4000 50  0001 C CNN
F 3 "" H 3250 4000 50  0001 C CNN
	1    3250 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3950 3250 4000
$Comp
L Neo_Driver-rescue:SW_SPDT-Switch SW1
U 1 1 5FD9C7D7
P 2500 3150
F 0 "SW1" V 2546 2962 50  0000 R CNN
F 1 "SPDT" V 2455 2962 50  0000 R CNN
F 2 "Neo_Driver:SW_Slide_SPDT_8.6x3.7mm_P2.50mm" H 2500 3150 50  0001 C CNN
F 3 "~" H 2500 3150 50  0001 C CNN
	1    2500 3150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 2400 2600 2550
Wire Wire Line
	4600 3850 6550 3850
$Comp
L Neo_Driver-rescue:Conn_01x03-Connector_Generic J1
U 1 1 5FDAB95B
P 7500 3850
F 0 "J1" H 7580 3892 50  0000 L CNN
F 1 "TO RING" H 7580 3801 50  0000 L CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 7500 3850 50  0001 C CNN
F 3 "~" H 7500 3850 50  0001 C CNN
	1    7500 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2250 7200 2250
Wire Wire Line
	7200 2250 7200 2350
Wire Wire Line
	8250 2250 8450 2250
Wire Wire Line
	8450 2250 8450 2150
$Comp
L Neo_Driver-rescue:Jumper_NC_Small-Device JP1
U 1 1 5FDB66C6
P 2600 2650
F 0 "JP1" V 2554 2725 50  0000 L CNN
F 1 "CURR MEAS" V 2645 2725 50  0001 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2600 2650 50  0001 C CNN
F 3 "~" H 2600 2650 50  0001 C CNN
	1    2600 2650
	0    1    1    0   
$EndComp
Wire Wire Line
	2600 2750 2600 2950
$Comp
L Neo_Driver-rescue:IRLZ44N-Transistor_FET Q1
U 1 1 600F61E2
P 7350 4950
F 0 "Q1" H 7554 4996 50  0000 L CNN
F 1 "IRLU8743PBF" H 7554 4905 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-251-3_Vertical" H 7600 4875 50  0001 L CIN
F 3 "" H 7350 4950 50  0001 L CNN
	1    7350 4950
	1    0    0    -1  
$EndComp
$Comp
L Neo_Driver-rescue:R-Device R3
U 1 1 600FEC82
P 6650 4950
F 0 "R3" V 6443 4950 50  0000 C CNN
F 1 "2K2" V 6534 4950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6580 4950 50  0001 C CNN
F 3 "~" H 6650 4950 50  0001 C CNN
	1    6650 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	7150 4950 7000 4950
Wire Wire Line
	7450 5150 7450 5400
Wire Wire Line
	7000 5350 7000 5400
Wire Wire Line
	7000 5400 7450 5400
Connection ~ 7450 5400
Wire Wire Line
	7450 5400 7450 5450
Wire Wire Line
	7000 4950 7000 5050
$Comp
L Neo_Driver-rescue:R-Device R4
U 1 1 600FF297
P 7000 5200
F 0 "R4" V 6793 5200 50  0000 C CNN
F 1 "10K" V 6884 5200 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 6930 5200 50  0001 C CNN
F 3 "~" H 7000 5200 50  0001 C CNN
	1    7000 5200
	-1   0    0    1   
$EndComp
Wire Wire Line
	6800 4950 7000 4950
Connection ~ 7000 4950
Text Label 6250 4950 0    50   ~ 0
SCK
Wire Wire Line
	6500 4950 6250 4950
Text Label 7450 4400 3    50   ~ 0
SW_GND
Text Label 6900 3950 0    50   ~ 0
SW_GND
Wire Wire Line
	6900 3950 7300 3950
Text Label 4700 5000 0    50   ~ 0
SW_GND
$Comp
L Neo_Driver-rescue:C-Device C3
U 1 1 6049F9DA
P 5600 4200
F 0 "C3" V 5348 4200 50  0000 C CNN
F 1 "100nF" V 5439 4200 50  0000 C CNN
F 2 "Capacitor_THT:C_Disc_D3.8mm_W2.6mm_P2.50mm" H 5638 4050 50  0001 C CNN
F 3 "~" H 5600 4200 50  0001 C CNN
	1    5600 4200
	0    1    1    0   
$EndComp
Wire Wire Line
	5450 4200 5250 4200
Wire Wire Line
	5250 4650 5250 4850
$Comp
L Neo_Driver-rescue:GND-power #PWR07
U 1 1 604A14A3
P 6000 4300
F 0 "#PWR07" H 6000 4050 50  0001 C CNN
F 1 "GND" H 6005 4127 50  0000 C CNN
F 2 "" H 6000 4300 50  0001 C CNN
F 3 "" H 6000 4300 50  0001 C CNN
	1    6000 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4200 6000 4200
Wire Wire Line
	6000 4200 6000 4300
Text Notes 3350 4350 2    50   ~ 0
Decoupling
Text Notes 8200 4450 2    50   ~ 0
Buffer
Text Notes 5800 4100 0    50   ~ 0
Low-pass filter\nif needed
$Comp
L Neo_Driver-rescue:R-Device R5
U 1 1 604AFA70
P 5250 4500
F 0 "R5" H 5100 4450 50  0000 C CNN
F 1 "330" H 5100 4550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5180 4500 50  0001 C CNN
F 3 "~" H 5250 4500 50  0001 C CNN
	1    5250 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	5250 4350 5250 4200
Connection ~ 5250 4200
Text Notes 2350 3700 2    50   ~ 0
LiFePO4wered/USB\nModule
Wire Wire Line
	5350 2950 5450 2950
Wire Wire Line
	5250 3550 5250 3400
Wire Wire Line
	5350 2950 5350 3650
$Comp
L Neo_Driver-rescue:SW_Push-Switch SW2
U 1 1 5EBB89E5
P 5650 3400
F 0 "SW2" H 5650 3685 50  0000 C CNN
F 1 "LEFT" H 5650 3594 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 5650 3600 50  0001 C CNN
F 3 "~" H 5650 3600 50  0001 C CNN
	1    5650 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 3400 5450 3400
Wire Wire Line
	5850 2950 5950 2950
Wire Wire Line
	5950 2950 5950 3400
Wire Wire Line
	5850 3400 5950 3400
Connection ~ 5950 3400
Wire Wire Line
	5950 3400 5950 3500
Text Notes 5400 5450 2    50   ~ 0
Brightness\nAdjust
Text Notes 2500 2700 2    50   ~ 0
Current\nMeasure
$Comp
L Neo_Driver-rescue:SW_Push-Switch SW3
U 1 1 5F2CC70B
P 5650 2950
F 0 "SW3" H 5650 3235 50  0000 C CNN
F 1 "RIGHT" H 5650 3144 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm_H5mm" H 5650 3150 50  0001 C CNN
F 3 "~" H 5650 3150 50  0001 C CNN
	1    5650 2950
	1    0    0    -1  
$EndComp
Text Notes 4050 4450 0    50   ~ 0
ATtiny85-20PU
Text Notes 7500 2000 0    50   ~ 0
AVR-ISP Header
$EndSCHEMATC
