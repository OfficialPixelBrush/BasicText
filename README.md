# BasicText
 
BasicText is a very basic Text Editor. It's main purpose is to be
used as the basis for a Text Editor for some of my 8-Bit Computer Projects.

# Tutorial
There are two modes in BasicText (or bt for short):
- CMD Mode, which is used for offering Commands to the Editor
- EDIT Mode is for Editing the Contents of the current file

**CMD Mode** has a few functions, which are
- n: Used for creating a new file
- o: Used to open a file
- s: Used to save the current file
- g: Used for going to a specific line
- l: Used for listing a specific line
- x: Used to exit the application

It's important to note, lines cannot be inserted.
The entire file is one continous bytestream.

# Limitations
This application is severely limited, having a maximum line length of 128 Characters per Line,
and a maximum number of 256 Lines, making it unsuitable for more than editing short
text files.

# Building
Any C89 Compiler will do.

# License
Idk.