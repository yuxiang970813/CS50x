# Import library
from cs50 import get_int

# Prompt user for height
while True:
    height = get_int("Height: ")
    if height > 0 and height <= 8:
        break

# Print result
for i in range(height):
    print(" " * (height - i - 1), end="")
    print("#" * (i + 1), end="")
    print("  ", end="")
    print("#" * (i + 1))
