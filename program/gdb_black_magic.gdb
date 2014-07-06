set mem inaccessible-by-default off
target extended-remote /dev/ttyACM0
mon swdp_scan
mon connect_srst enable
attach 1
r
load 
kill
quit
