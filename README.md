# openexpraw
a wrapper for Libraw to get unprocessed data from raw file
before using openexpraw, you should compile libraw and opencv core library

# Intro
1. Clone or download LibRaw 0.19/0.20 and extract to a folder, such as named Libraw
2. put expraw folder in Libraw folder, it looks like:  
  **Libraw**  
    |--bin  
    |--buildfiles  
    |--doc  
    |--**expraw**  
    |     |--expraw.cpp  
    |     |--expraw.h  
    |     |--expraw.pro  
    |     |--expraw_global.h  
    |     |--expraw_test.cpp  
    |     |--expraw_test.pro  
    |  
    |--GoPro  
    |--internal  
    |--lib  
    |--libraw  
    |--balabala...  
    |--balabala...  
      
  
  3. First, compile Libraw using default setting, and output file in *buildfiles* folder
  4. use QtCreator open the expraw.pro, and the expraw_test.pro is a test project for expraw, ***modify LIBS and INCLUDES setting in expraw.pro*** for your environment.
  5. It can support VS2015 and Qt 5.9.6 and above
  
  # License
  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                    
  http://www.wtfpl.net/
  

