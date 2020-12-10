## openexpraw
openexpraw is a personal utility class, it is just a c++ wrapper class to get unprocessed data from raw file via open with Libraw.  
before using openexpraw, you should compile libraw (https://github.com/LibRaw/LibRaw/).  

and, thanks Libraw https://github.com/LibRaw/LibRaw/ , it is a great job!    
  
  
## Intro
* Clone or download LibRaw 0.19/0.20 and extract to a folder, such as named Libraw
* Put expraw folder in Libraw folder, it looks like:  
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
      
  
* First, compile Libraw using default setting, and output file in *buildfiles* folder
* Use QtCreator open the expraw.pro, and the expraw_test.pro is an example project for expraw, you ***MUST modify LIBS and INCLUDES setting in expraw.pro*** for your environment before using.
* It can support VS2015 and Qt 5.9.6 and above
  
## License
  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                    
  http://www.wtfpl.net/
  
  
