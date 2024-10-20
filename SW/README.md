# Arduino IDE Software

Installing the Pre Built UF2
--

I have prebuilt the UF2 file in the build folder, connect the XAIO board to your computer, and double press reset on the board.

A Drive should appear usually named Arduino, copy the UF2 file to this drive, the drive will then disconnect, and sofware should be uploaded.

(Same method as using a Pi Pico)


Building the software
--

1. Open the arduino sketch PregnantLaserMouse
2. Make sure you have the Seeduino XIAO board selected
	* If not guide is here from XIAO (https://wiki.seeedstudio.com/Seeeduino-XIAO/)
3. You should be able to compile and upload to the XIAO Board, if you have connected via USB and selected the comm port


Building the UF2
--

1. Click "Sketch" in the menu, and select "Export Compiled Binary"
2. Open the PregnantLaserMouse folder and there should be a build folder with a bin file. This file needs converting to uf2, see below.


Converting bin to uf2 for SAMD21
--

1. Download the Microsoft uf2 tool from the official site to your local drive somewhere and keep a note:

``
$ git clone https://github.com/microsoft/uf2.git
``

2. Navigate to the build directory in command line:

``
$ cd {SOMEWHERE}/PregnantLaserMouse/SW/PregnantLaserMouse/build/Seeeduino.samd.seeed_XIAO_m0
``

3. Run the uf2conv.py python script in this location as follow:

``
$ python {SOMEWHERE}/uf2/utils/uf2conv.py -f 0x68ed2b88 -b 0x2000 ./PregnantLaserMouse.ino.bin -o ./PregnantLaserMouse.ino.uf2
``

_Note: -f 0x68ed2b88 is for SAMD21 and -b 0x2000 is used for base address._

4. Plug in the XIA via USB-c into your computer, it should appear as a flash drive.

5. Simply drag the flash.uf2 into the XIAO flash drive.

6. Done
