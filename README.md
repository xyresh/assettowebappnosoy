# assettowebapp
simple webapp that displays laptimes from an assetto corsa webserver
but without javascript

to install modify the makefile with your webserver directory, then run
`make install` with root privileges if your webserver directory has not got write permissions(allthough it is necessary for this to work properly)

modify the locations depending on your assetto corsa server directory

place the start.sh file in said assetto game server directory

this is designed to work with an apache webserver, however it can be modified appropriately to work with nginx or anything else
