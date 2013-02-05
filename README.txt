Epoch
=====

This projet is a framework, which aim to analyse user's brain waves with the emotiv EPOC headset, while he is playing to a game. From the interface, you could run any executable file that you want. And you could look at the wave's frequency, the raw data, the analysed data and two specific types of wave : the alpha and the beta waves. All those data come from the raw data, that are analyse by the framework. It's easy to change algorithm of analyse, and put some signal filter to the raw data, before or after the analyse.

DEMONSTRATION : 
	https://www.dropbox.com/sh/pisc6kuynwvk41i/7BGhJ1biCq/presentation.mpg

INSTALL:
 Ubuntu 32bits (for EDK)
   Pong :
    • Dependencies :
	– SDL dev
	- SDL gfx
	– OpenGL dev
	– Freeglut dev
	• Compiling : 
                   - ~/Pong/V1.3 make -f makefile.linux
	Framework :
                • Dependencies :
                   – EDK
                • Copy file :
                   – move EDK libraries into the file Brain_Framework/lib/
	Interface :
                • Dependencies :
                   – QT4 dev
                • Compiling :
			– ~/Brain_Framework/Epoch_GUI qmake;
			– ~/Brain_Framework/Epoch_GUI make;

MORE DETAIL :
	https://github.com/Babouchot/Epoch


                o o"o"o"o"o"ooo o
           o M Mo"oM M Mo"o"o"o"o"ooo
         oo"o"o"o"o"o"oM M Mo"oM Mo M M
       oM M M"o"oM Mo"o"o"M M M M M M"o"oo
     oM M Mo"o"o"o"o"o""o"o"o"o"oM Mo"o"o"oo
    M M M M M"o"M "          " "o"o"o"M Mo"oo
   M M Mo"oM M "                 "o"oM M M M M
 o"M M"oo"o"o                       "o"oM M"o"o
 o"o"o"o""o"                        "o"o"oM M"o"
 M"o"M M""                            M M M M Mo
                                       "oM Mo"o"o
                                       M M M Mo"o
                                       o"o"o"o"o"
                                        M"o"M M"o
         o                             Mo"oM M M
M""M"""M M o                          "o"o"o"o"o"
o"o"oM"o"o"o  o                     "o"M M M"oM
 M"o"o"o"oM M                      M M M"o"o"o"
M M M Mo"o"o"o"o                o"o"o"M M Mo"o
 M"o"o"o"o"M M M"o"o o  o  o" oM M Mo"o"o"o"o
M M M"o"M M M"o"oM Mo"o"o"M M"o"o"oM M M"o"
 M"o"oM M M"o"o"o"o"o"M M M M Mo"o"o"oM M
"o"oM M M M M M"o"o"oM M"o"M M M M"o"o"
 M M M Mo   " M M"o"o"o"oM M Mo"oM "
  "M M Mo        " ""o"o"o"" " "
    ""o"oo


