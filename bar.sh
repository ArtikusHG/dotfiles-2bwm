#!/bin/sh

# Function that gets active windows

workspace() {
	echo "$(xprop -root _NET_CURRENT_DESKTOP | sed -e 's/_NET_CURRENT_DESKTOP(CARDINAL) = //')"
}

# Function that gets the date
dateFunc() {
	date '+%B %d, %Y'
}

# Function that gets the time
clock() {
        date +%H:%M:%S
}

# Function that gets the battery percentage and charging status
battery() {
	if [[ $(sudo cat /sys/class/power_supply/BAT0/status) = "Charging" ]]; then
		echo "%{B#999E61} $(sudo cat /sys/class/power_supply/BAT0/capacity)%"
	else
		echo "%{B#305E85} $(sudo cat /sys/class/power_supply/BAT0/capacity)%"
	fi
}

# Function that gets the volume (and checks if the sound is disabled)
sound() {
        volume=$(amixer get Master | sed -n 's/^.*\[\([0-9]\+\)%.*$/\1/p'| uniq)
	if [[ $(amixer get Master | tail -n 1 | awk '{print $6}' | grep "off") ]]; then
        	echo -e "off"
	else
		echo -e "$volume%"
	fi


}

# Function that gets used RAM (in percentage)
ram() {
	echo "RAM: $(free | grep Mem | awk '{ print $3/$2 * 100.0 }' | cut -f1 -d ".")%"
}

# Function that gets used CPU (in percentage)
cpu() {
	echo "CPU: $(grep 'cpu ' /proc/stat | awk '{usage=($2+$4)*100/($2+$4+$5)} END {print usage }' | cut -f1 -d ".")%"
}

# Function that gets screen brightness
backlight() {
	echo -e "$(xbacklight | awk 'END { print int(($0 / 5) + 0.5) * 5 }')%"
}

# Infinite loop that checks if something has changed 50 times in a second and echoes the bar input
while 'true'; do
	echo -e "%{B#384156} $(workspace) %{B#E0B053} $(ram) $(cpu) %{B#305E85} $(dateFunc) %{B#568FC0} $(clock) %{r}%{B#568FC0} $(backlight) $(battery) %{B#E0B053} $(sound) %{B#272D3A}"
	sleep 0.1
done
