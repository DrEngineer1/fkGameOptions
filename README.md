# fkGameOptions
Adds a new options tab within the Worms 2 frontend. Mainly a toggle for the use of W2SE's patch.\
Made with BCX BASIC to C/C++ Translator and MSVC.

## Building:
The C++ source file is generally the main file to compile with along with all the other source files and header files.\
The BCX BASIC Source file is behind on the latest changes that the C++ source file has and I recomend that you don't use it. Mainly because it doesn't do true switch cases and throws on a bunch of headers that the main source file doesn't need to compile.\
When building use the MAKEFILE with NMAKE to build it.

## Credits
Syroot/Pac-Man: Original Creator of WormKitTools\
Carlmundo: Creator* of the modifed WormKitTools called FrontendKitLib\
Jig and Ser: Edits of their Worms 2 start game button bitmaps from W2SE\
Team17: Worms 2 Assets that Jig and Ser edited along with decompiled code and the one asset that I took and edited to make it.\
<sub>Can't Believe i have to credit these bots but they did provide some really useful things.</sub>\
Github Copilot and whoever's code it regurgitated from: (initially broken) NMAKE Makefile, WriteString function, frontend function pointer to grab game.dat that will go unused likely.\
Google Gemini and whoever's code it regurgitated from: Get Resource bytes function.

<sub>*Assuming he did end up making this modifed version that is. Please correct me if I'm wrong.</sub>
