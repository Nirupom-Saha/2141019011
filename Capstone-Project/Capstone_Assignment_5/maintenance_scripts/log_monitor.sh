#!/bin/bash

# Define the log file to scan (dpkg.log is used here)
LOG_FILE="/var/log/dpkg.log"  # Changed to an existing log file
# Define the list of keywords to search for
KEYWORDS=("error" "fail" "critical")

# Notify the user that logs are being scanned
echo "Scanning logs for issues..."
# Loop through the keywords and search for them in the log file
for keyword in "${KEYWORDS[@]}"; do
    grep -i "$keyword" "$LOG_FILE" > /tmp/log_alerts.txt  # Search each keyword in the log file
done

# Check if any alerts were found in the log file
if [[ -s /tmp/log_alerts.txt ]]; then
    echo " Alert: Issues found in logs!"  # Notify user of issues
    cat /tmp/log_alerts.txt  # Display the issues found
else
    echo " No critical issues found in logs."  # Inform the user if no issues were found
fi

