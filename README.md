# ImageToAscii

## Description
A simple C program that converts a bitmap image to a text file with ASCII characters.

## Usage
In the same directory as the executable, create a file called `image.txt`. Then, open a command line and run the program like so:
```
ImageToASCII.exe (path to bitmap)
```

## Potential Mistakes
If you get an error message that says:
```
Error: Invalid or corrupted file format.
Size: (number)
Compression: (number)
Bit Depth: (number)
```
Check the size, compression, and bit depth. Size should be 124, Compression should be 0, and Bit Depth should be 24. If any of these numbers are wrong, your file is formatted wrong. Open it with a photo editing software that supports bitmap and make sure to save it as a 24-bit non-compressed bitmap.

If your error message says:
```
Error: Invalid or corrupted file format.
// or
Error: Pixel count incorrect. File is likely corrupted.
```
Your file is either not a bitmap file at all, or it is corrupted.

Finally, if your error message says:
```
Error: Failed to allocate (number) pixels.
```
Your image is probably way too big.

## Strengths
* The code is cross platform; I did use windows types as a stylistic choice, but it works the same when they are replaced with standard types (think BYTE -> uint8_t)
* It is a single translation unit and very readable.
* The output can be scaled to your liking. (Nearest neighbor (?))
* 60% of the time, it works every time.
* It looks pretty cool.

## Limitations
* Realistic images/photos tend to be pretty muddy, since you can really only take the value of the colors with ASCII art. This effect can be minimized if you increase the contrast of the pictures.
* No support for other file formats; only bitmaps.
* No support for compressed bitmaps.
* Bitmaps have to be 24 bit.
