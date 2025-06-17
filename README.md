**ImageToASCII**

---

**Description**
ImageToASCII is a lightweight C utility that converts 24-bit uncompressed bitmap (.bmp) images into ASCII art representations, outputting the result as plain text.

**Usage**

1. Place an `image.txt` file in the same directory as the executable.
2. Run the program with the following syntax:

   ```bash
   ImageToASCII.exe <path_to_bitmap>
   ```

**Common Errors and Troubleshooting**

* **Invalid or corrupted file format:**
  Ensure the image meets the following:

  * Header Size: 124 bytes
  * Compression: 0 (uncompressed)
  * Bit Depth: 24

* **Pixel count incorrect or file is not recognized:**
  Confirm the input is a valid 24-bit bitmap file. Use an image editor to re-export it if needed.

* **Failed to allocate pixels:**
  The image may be too large for available memory.

**Features**

* Cross-platform compatible (Windows types easily replaceable with standard C types).
* Single translation unit with clear and maintainable structure.
* Adjustable output resolution.
* Efficient nearest-neighbor scaling.

**Limitations**

* Only supports 24-bit uncompressed bitmap images.
* No support for JPEG, PNG, or other formats.
* Photo-realistic images can lose detail in ASCII rendering.

## Demonstration

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/UyrCpCYaGeU/0.jpg)](https://www.youtube.com/watch?v=UyrCpCYaGeU)
