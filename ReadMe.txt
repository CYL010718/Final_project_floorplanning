How to use:

Compile: Type "make" in command line

Execute: ./fp  [α value] [input.block name] [input.net name] [output file name]
*** The program will also generate a file name "treeStruct" for drawing

Draw: python3 ./svg.py [input.block name] [output file name] treeStruct

Prerequisities: 
1. Install drawsvg (type "pip3 install drawsvg" in command line) and cairo()
2. Install cairo (type "brew install cairo"(for mac) or "sudo apt-get install libcairo2"(for others) in command line)
3. Install numpy