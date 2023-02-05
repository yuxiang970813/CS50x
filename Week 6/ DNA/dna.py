import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read database file into a variable
    CSV = sys.argv[1]

    # Declare variable
    names = []
    STR = []

    # Read file
    with open(CSV, "r") as file:
        reader = csv.reader(file)

        # Store the name of STR
        STR_name = next(reader)
        STR_name.remove("name")

        # Iterate csv file
        for row in reader:
            # Store name in list
            name = row[0]
            names.append(name)

            # Store STR in list
            str = row[1:]
            STR.append(str)

    # Making databases
    databases = {}
    i = 0
    while i < len(names):
        # Store name: list of STR as key value pair
        databases[names[i]] = STR[i]
        i += 1

    # Read DNA sequence file into a variable
    TXT = open(sys.argv[2])
    sequence = TXT.read()

    # Find longest match of each STR in DNA sequence
    j = 0
    DNA_reult = []
    while j < len(STR_name):
        # Store result in a list to used for compare
        DNA_reult.append(longest_match(sequence, STR_name[j]))
        j += 1

    # Check database for matching profiles
    k = 0
    while k < len(names):
        # Return result if find match
        if (databases[names[k]]) == DNA_reult:
            print(names[k])
            break
        k += 1
    # Return No match if no found
    if k == len(names):
        print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return str(longest_run)


main()
