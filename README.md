# Paneliser
Utility to produce a single FPC file from multiple files representing different designs. Intended to work with FreePCB files

#Introduction 
This is a program to combine a number of fpc files representing printed circuit boards into a combined fpc file which can then be used
for manufacturing.

Combining boards in this way can significanly reduce costs with some board houses who price a standard sized board say 100X100mm or 100X80mm.

This program allows you to combine several different boards in a single panel.

This program was developed using GNU g++, and depends on MinGW being on the path when run under windows.

It is run from the command line.

#Command line arguments

The command line arguments are pairs of tags and values as follows:

|Tag|Meaning
|---|-------
|-b|Path of combined board specication file
|-o|Path of output .fpc file
|-c|Path of output CAM files to embed in .fpc file (default output folder with \CAM)
|-m|Name of a net to be merged across files.
|-w|Width of combined board in millimetres (default 100)
|-h|Height of combined board in millimetres (default 100)
|-nooutline|Specifies that individual board outlines are not included in the output

The -b and -o tags are mandatory, others are optional

##examples

path=C:\MinGW\bin;%path%  
c:\dev\paneliser\bin\release\paneliser.exe -b c:\DEV\SmallBoards\combinedboard_analog_2.txt -o c:\dev\SmallBoards\combined_analog3.fpc -c c:\dev\SmallBoards\CAM  



#Combined board specification file
The combined board specification file is a text file which defines the component boards that you wish to place on your combined board.
Each line of the file defines the placement, naming and path of a component file.
Fields in the specification file are separated by spaces.

The fields are as follows:
  
|Field Number|Contents  
|------------|--------  
|1|X coordinate in millimetres from the bottom left of the combined board of the origin of the current board.  
|2|Y coordinate in millimetres from the bottom left of the combined board of the origin of the current board.  
|3|Width of the current board in millimetres  
|4|Height of the current board in millimetres  
|5|Name of the board - this is used as a prefix for net names  
|6|Path of the fpc file defining the current board.  

## Example  
0 0 48.5 73 SPISWITCH1 C:\DEV\SmallBoards\AUDIO_BUS\SPI_SWITCH_8.fpc  
0 76 49 24 USBPSU C:\DEV\Smallboards\USBPSU\USBPSU_2.fpc  
50.5 0 13.3 49 RJ45_ADAPTER C:\DEV\Smallboards\RJ45Adapter\RJ45Adapter_1.fpc  
51.5 62 9.6 38 FET C:\DEV\Smallboards\FET\FET.fpc  
65 0 35 100 LIGHT_POWER C:\DEV\SmallBoards\LIGHT_CONTROLLER\LightController_only_1.fpc  
