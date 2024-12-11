import os
import sys
import csv

def generate_player_csv(directory):
    # Define the output CSV file
    output_file = 'player_list.csv'

    # Open the CSV file for writing
    with open(output_file, mode='w', newline='') as file:
        writer = csv.writer(file)

        # Write the header row
        writer.writerow(['First Name', 'Last Name'])

        # Loop over each file in the directory
        for filename in os.listdir(directory):
            if filename.endswith('.jpg'):  # Ensure processing only JPG files
                # Split the filename to extract the first and last names
                name_parts = filename.replace('.jpg', '').split('-')
                first_name = name_parts[0]
                last_name = name_parts[1] if len(name_parts) > 1 else ''

                # Write the names to the CSV file
                writer.writerow([first_name, last_name])

    print(f"Player names have been written to {output_file}")

if __name__ == "__main__":
    # Check if the directory name is provided
    if len(sys.argv) < 2:
        print("Usage: python script.py <directory_path>")
        sys.exit(1)

    # Directory path from the command line argument
    directory_path = sys.argv[1]

    # Generate the CSV
    generate_player_csv(directory_path)

