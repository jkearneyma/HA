HA
==

This is an experiment to see how far I can get using a D-Bus component model to enable generic scripting of home automation tasks.

The components I have in mind for the initial spike are:
- timer
- Z-wave (using open-zwave)
- J-Works USB (selected models)
- GPIO (Rpi and similar)

My hardware target is:
- an Odroid C1 (http://hardkernel.com/)
- a Aeon Z-stick (http://aeotec.com/z-wave-usb-stick)
- various J-Works USB interfaces (http://www.j-works.com/index.php)

Though almost any Linux system with D-Bus available should work.

There are some external libraries used:
  open-zwave
  wiringPi (port to ODROID-C1)

These are added to the source tree using a tool called 'ext'.  ext is a Ruby program, so install ruby and then use the command 'gem install ext' to add it to the system.  

build instructions:

ext update
( cd open-zwave ; make ; sudo make install )
( cd wiringPi ; ./build ; sudo make install )
qmake all.pro
make

