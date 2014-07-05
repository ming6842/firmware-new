set mem inaccessible-by-default off
target extended-remote /dev/ttyACM2
mon swdp_scan
attach 1
r
load 
kill
quit
