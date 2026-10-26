# Image Steganography Using LSB

## Description
A C-based Image Steganography project that hides secret text data inside a BMP image using the Least Significant Bit (LSB) technique. The hidden data can later be extracted from the encoded image without significantly changing its visual appearance.

## Features
Encode secret text files into BMP images.
Decode hidden text data from steganographic images.
Validate command-line arguments and input files.
Check image capacity before encoding.
Preserve the BMP header and copy remaining image data.
Verify encoded data using a magic string.

## Technologies Used
Programming Language: C

## Concepts
File Handling, Bitwise Operations, Pointers, Structures, Command-Line Arguments

## Compiler: GCC

## Project Structure
encode.c, encode.h – Encoding functionality.
decode.c, decode.h – Decoding functionality.
test_encode.c – Main function and command-line argument handling.
types.h – User-defined data types and status codes.
common.h – Common definitions.
beautiful.bmp – Sample source image.
secret.txt – Sample secret text file.

## Concepts Learned
Hiding information using LSB manipulation.
Reading and writing binary image files.
Extracting data using bitwise operators.
Modular programming using header and source files.
Handling command-line arguments and file operations.

## Author
Soundharya Lahari
