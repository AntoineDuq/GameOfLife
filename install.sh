#!/bin/bash

clear
echo ------------------------------
echo \* Game of Life - 1.5               
echo \* by Antoine Duquenoy
echo ------------------------------

isInstalled=`dpkg -s libncurses5 | grep Status | cut -d' ' -f3`

if [ $isInstalled = "ok" ]
then
	echo -e "\n"
	echo "NCURSES 5 is already installed"
	echo -e "\n"

else
	echo -e "\n"
	echo "NCURSES 5 is not installed"
	sudo apt-get install libncurses5-dev
	echo -e "\n"		
fi

gcc -o gol gol.c -lncurses
echo "-------- INSTRUCTIONS --------"
echo "* Use the arrow keys to move"
echo "* SPACE to place a cell"
echo "* BACKSPACE to remove a cell"
echo "* ENTER to validate"
echo "* ENTER to start"
echo "------------------------------"
echo -e "\n"
read -p "Start the game [y/n] : " yn
if [ $yn = "y" ] || [ $yn = "Y" ]
then
	read -p "Interval between two figures (microsecond - 1 000 000 µs = 1s) : " speed
	read -p "Number of iterations (0 : infinite) : " ite
	./gol $speed $ite
else
	echo "To play later ./gol <interval_µs> <iterations>"
fi

exit 0
