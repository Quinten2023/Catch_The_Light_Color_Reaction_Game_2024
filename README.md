# OSU-103-code
ENGR 103 repo

This code is designed for the Circuit playground express, the switch on the board decides if you are in the mode select or the actual game. While in mode select, an LED will oscillate between colors, that light will correspond with whichever game you are playing. LEDs 0-4 are difficulties 1-5 in survival, to win you need to get through 10 rounds and still have at least 1 remaining life. LEDs 5-9 are difficulties 1-5 in endless, currently they are basically just a practice mode but I am planning on developing the code to go until you lose your life count and then serial print or speak how many rounds you lasted. 

BUGS to be fixed
The code's speaker commands, only once the speaker starts talking, seem to make the board's power drop making LEDs dimmer, noises quieter and occasionally the buttons will stay on low.

Things that could be improved
The quick success animation (just meaning you survived a single round without dying) could be longer and for me isn't a problem in the easier difficulties but could be changed for the harder ones

Rules of Game
This game is essentially a reaction test. Whenever the light comes from the right, press the right button and when it comes from the left, press the button on the left. If you survive 10 rounds in survival, you will win or, in endless mode, your score will be printed.

HIDDEN FEATURE DO NOT READ IF YOU WANT TO BE SURPRISED:




Your choice to keep scrolling down






If you are in mode select, you can shake the device to increase initial amount of lives starting a round up to 10 and then back to 1
