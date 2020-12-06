# openexpraw
a wrapper for Libraw to get unprocessed data from raw file

# Intro
1. Clone or download LibRaw 0.19/0.20 and extract to a folder, such as named Libraw
2. put expraw folder in Libraw folder, it looks like:
  Libraw
    |--bin
    |--buildfiles
    |--doc
    |--*expraw*
    |   |--expraw.cpp
    |    |--expraw.h
    |    |--expraw.pro
    |    |--expraw_global.h
    |    |--expraw_test.cpp
    |    |--expraw_test.pro
    |
    |--GoPro
    |--internal
    |--lib
    |--libraw
    |--balabala...
    |--balabala...
  
  3. First, compile Libraw using default setting, and output file in *buildfiles* folder
  4. use QtCreator open the expraw.pro, and the expraw_test.pro is a test project for expraw
  5. It can support VS2015 and Qt 5.9.6 and above
  
  # License
  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                    
  Version 2, December 2004 
  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net> 
  Everyone is permitted to copy and distribute verbatim or modified copies of this license document, and changing it is allowed as long as the name is changed.            
  DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE   
  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION  
  0. You just DO WHAT THE FUCK YOU WANT TO.
  
  ref. http://www.wtfpl.net/
  

