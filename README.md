# Pong

This game requires two UCFK4 (University of Canterbury Fun Kit v4) boards, which contain an [ATmega32u2 microcontroller](http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/Atmel_ATmega32u2), a reset push button, a general-purpose pushbutton, a five-way navigation switch, a green LED that indicates if power is on, a user switchable blue LED, a seven by five dot-matrix display, an infrared LED, a 36 kHz infrared receiver, and a USB connector. The USB connector provides 5 V to run the microcontroller and to allow programs to be up-loaded.

_UCFK4 data is per the [University of Canterbury UCFK4 wiki page](http://ecewiki.elec.canterbury.ac.nz/mediawiki/index.php/UCFK4)_.

## Getting the source

If the [ence260-ucfk4](https://eng-git.canterbury.ac.nz/rmc84/ence260-ucfk4) repository is not present:

```shell
git clone https://eng-git.canterbury.ac.nz/rmc84/ence260-ucfk4.git
```

Clone the repo into `/ence260-ucfk4/assignment/`

```shell
cd /ence260-ucfk4/assignment/
git clone https://eng-git.canterbury.ac.nz/ence260-2018/group436.git
```

## Overview of the game

The ball rebounds off the puck/paddle in the following ways:

![Image of the various possibilities for rebounding off the puck](media/rebound.png)

## Diagrams

### Display

The display is a 7 row × 5 column dot-matrix display.

The display, in terms of the board is thought of as the following:

![Image of the board, which illustrates the orientation of the display's array](media/board.png)

The puck/paddle has its `bottom` towards the top of the display (i.e. it has a _lower_ value than the `top`).

### Compass/`Direction`

Compass directions, with the same orientiation as the board above:

![Image of the compass directions with the same orientation as above](media/compass.png)
