How to install dependency packs:
--

  This is a folder where you can extract Sonetto dependency library packs.

  Any project will look for their libraries in this folder, so you just have
to download the packs from http://code.google.com/p/sonetto/downloads/list
and extract them here.

  Each pack have a lib/ and an include/ folder, so you only need
to extract both inside your dependencies/ folder and the files will fit perfectly.
The only exception is Ogre3D, for which we preferred to use the environmental variable
OGRE_HOME for lib searching.