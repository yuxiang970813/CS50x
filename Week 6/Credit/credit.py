# Import library
from cs50 import get_int

# Prompt user for number
number = get_int("Number: ")
number = str(number)

# Initialize counter
i = len(number) - 1
sum = 0

# Calculate checksum
while (i >= 0):
    # Calculate second-to-last digit
    if (i - 1 >= 0):
        num = int(number[i - 1]) * 2
        if (num >= 10):
            # Split digit
            num1 = num // 10
            num2 = num % 10
            # Sum those products’ digits
            sum += num1
            sum += num2
        else:
            sum += num
    # Calculate other than second-to-last digit
    sum += int(number[i])
    # Update counter
    i -= 2
checksum = sum % 10

# Compute result
if (checksum == 0):
    # AMEX
    if len(number) == 15 and (number[:2] == "34" or number[:2] == "37"):
        print("AMEX")
    # MASTERCARD
    elif len(number) == 16 and (int(number[:2]) >= 51 and int(number[:2]) <= 55):
        print("MASTERCARD")
    # VISA
    elif (len(number) == 13 or len(number) == 16) and number[0] == "4":
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")
