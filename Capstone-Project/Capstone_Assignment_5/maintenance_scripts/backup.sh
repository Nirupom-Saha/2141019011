#!/bin/bash

# Define source and destination directories
SOURCE_DIR="$HOME/Documents"  # Default to Documents folder in home directory
DEST_DIR="$HOME/backup"       # Backup will be stored in ~/backup

# Check if source directory exists
if [ ! -d "$SOURCE_DIR" ]; then
    echo "Source directory $SOURCE_DIR does not exist."
    echo "Creating the directory $SOURCE_DIR."
    mkdir -p "$SOURCE_DIR"  # Create the directory if it doesn't exist
fi

# Create backup directory if it doesn't exist
mkdir -p "$DEST_DIR"

# Create a timestamped backup file
BACKUP_FILE="$DEST_DIR/backup_$(date +%Y%m%d_%H%M%S).tar.gz"

# Perform the backup using tar
tar -czf "$BACKUP_FILE" "$SOURCE_DIR"

# Check if tar command succeeded
if [ $? -eq 0 ]; then
    echo "Backup completed successfully at: $BACKUP_FILE"
else
    echo "Backup failed. Please check the error above."
fi

