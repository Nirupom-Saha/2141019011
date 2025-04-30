#!/bin/bash

# Start an infinite loop to display the menu repeatedly
while true; do
    echo ""
    # Display the system maintenance menu
    echo "===== System Maintenance Menu ====="
    echo "1. Backup Files"
    echo "2. System Update and Cleanup"
    echo "3. Monitor System Logs"
    echo "4. Exit"
    echo "==================================="
    # Prompt the user to select an option
    read -p "Enter your choice: " choice

    # Perform actions based on the user's choice
    case $choice in
        1) bash backup.sh ;;  # Run the backup script
        2) bash update_cleanup.sh ;;  # Run the update and cleanup script
        3) bash log_monitor.sh ;;  # Run the log monitoring script
        4) echo "Exiting Maintenance Suite. Goodbye!" ; exit ;;  # Exit the program
        *) echo "Invalid option, please try again." ;;  # Handle invalid input
    esac
done

