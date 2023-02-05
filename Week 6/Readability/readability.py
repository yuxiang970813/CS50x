# Import library
from cs50 import get_string

# Prompt user for input
input_text = get_string("Text: ")

# Count letters
letter_counter = 0
for letter in input_text:
    # Update counter if find alphabet
    if letter.lower() >= "a" and letter.lower() <= "z":
        letter_counter += 1

# Count words
word_counter = 0
strlen = len(input_text)
i = 0
while (i < strlen):
    # Update counter if find space(ignore multiple spaces)
    if input_text[i] == " " and input_text[i + 1] != " ":
        word_counter += 1
    i += 1
# Check containing at least one word
if strlen > 0:
    word_counter += 1
    # Ignore start with space
    if input_text[0] == " ":
        word_counter -= 1
    # Ignore end with a space
    if input_text[strlen - 1] == " ":
        word_counter -= 1

# Count sentences
sentence_counter = 0
for sentence in input_text:
    if sentence == "." or sentence == "!" or sentence == "?":
        sentence_counter += 1

# Calculate Coleman-Liau Index
L = 100 * letter_counter / word_counter
S = 100 * sentence_counter / word_counter
index = round(0.0588 * L - 0.296 * S - 15.8)

# Print result
if index >= 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print(f"Grade {index}")
