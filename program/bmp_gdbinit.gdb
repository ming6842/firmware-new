set mem inaccessible-by-default off
target extended-remote /dev/ttyACM0
mon swdp_scan
attach 1

define show_list
    printf "waypoint list, we have %d now\n", waypoint_info.waypoint_count

    set $wpl = waypoint_info.waypoint_list
    set $i = 0
    while $i != waypoint_info.waypoint_count

        printf "waypint %d address %x|", $i, $wpl
        printf "lat:%3.47f long:%3.7f alt:%3.2f \n", $wpl->data.x, $wpl->data.y, $wpl->data.z
     
        set $wpl = $wpl->next
        set $i = $i + 1
    end
end

define show_navi_wp_list

	set $wp_number = 0
	set $i = 0

	while $i != 250

	        if navigation_info.wp_info[$i].data_available == 1
	        	set $wp_number = $wp_number + 1
	        else 
	        	
	        end
	        set $i = $i + 1
	end

	printf "navigation has %d waypoints| mavlink waypoint has %d waypoints \n", $wp_number, waypoint_info.waypoint_count
end

define show_pid_parameter
	printf "[Please set the breakpoint to the function \"PID_init\"]\n"
	printf "[Roll] Kp:%f Ki:%f Kd:%f\n", PID_roll->kp, PID_roll->ki, PID_roll->kd
	printf "[Pitch] Kp:%f Ki:%f Kd:%f\n", PID_pitch->kp, PID_pitch->ki, PID_pitch->kd
	printf "[Yaw] Kp:%f Ki:%f Kd:%f\n", PID_yaw_rate->kp, PID_yaw_rate->ki, PID_yaw_rate->kd
	printf "[Heading] Kp:%f Ki:%f Kd:%f\n", PID_heading->kp, PID_heading->ki, PID_heading->kd
	printf "[Zd] Kp:%f Ki:%f Kd:%f\n", PID_Zd->kp, PID_Zd->ki, PID_Zd->kd
	printf "[Z] Kp:%f Ki:%f Kd:%f\n", PID_Z->kp, PID_Z->ki, PID_Z->kd
	printf "[Navigation] Kp:%f Ki:%f Kd:%f\n", PID_nav->kp, PID_nav->ki, PID_nav->kd
end
