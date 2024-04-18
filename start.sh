#!/bin/bash

# Run the AssettoServer binary
#./AssettoServer

# Path to the log directory
log_directory="/ac_server_directory/logs"

# Path to the destination directory
destination_directory="/var/www/html"




# Get the latest log file
latest_log=$(ls -t "$log_directory"/log-$(date +"%Y%m%d").txt | head -n1)

# Check if a log file was found
if [ -z "$latest_log" ]; then
    echo "No log files found."
    exit 1
fi

# Copy the latest log file to the destination directory
cp "$latest_log" "$destination_directory/latest_log.txt"

# Extract relevant lines from the latest log file and save them to the destination file
grep -oE 'Lap completed by .*, [0-9]+ cuts, laptime [0-9]+' "$latest_log" > "$destination_directory/latest_log.txt"

# Extract lap times from the latest log file and append them to another file
grep -oE 'Lap completed by .*, [0-9]+ cuts, laptime [0-9]+' "$latest_log" | cut -d' ' -f6 >> "$destination_directory/latest_lap_times.txt"


#run laptime updating program change the file names if you must
cd "$destination_directory" || exit 1
./update_laptimes latest_log.txt index.htnl

echo "Latest log file copied to $destination_directory/latest_log.txt"
echo "Lap times extracted and appended to $destination_directory/latest_lap_times.txt"

