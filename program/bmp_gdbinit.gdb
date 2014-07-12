set mem inaccessible-by-default off
target extended-remote /dev/ttyACM0
mon swdp_scan
attach 1

define show_list
    printf "waypoint list, we have %d now\n", waypoint_cnt

    set $wpl = mission_wp_list
    set $i = 0
    while $i != waypoint_cnt

        printf "waypint %d|", $i
        printf "lat:%3.47f long:%3.7f alt:%3.2f \n", $wpl->data.x, $wpl->data.y, $wpl->data.z

        

        set $wpl = $wpl->next
        set $i = $i + 1
    end
end