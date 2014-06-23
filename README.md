imclip
======

Simple program to store images to and retrieve images from the clipboard using the command-line.

Syntax
------

    imclip [-o outfile [-t type]| -i infile]

Flags
-----

 - `-o` or `--out`: The name of the file to which `imclip` must write image data from the clipboard.
 - `-t` or `--type`: The image type of the file (e.g. `png`, `jpeg`, etc.). This is an optional flag that can be supplied alongside `-o` or `--out`.
 - `-i` or `--in`: The name of the file from which `imclip` must read image data and store it into the clipboard.

Examples
--------

    imclip -o awesome.jpg

Writes the image that is currently in the clipboard to the file named `awesome.jpg`. The type of the image is discerned from the extension. In this case, data is written into a JPEG file.

    imclip --out=awesome.png --type=png

Writes the image that is currently in the clipboard to the file named `awesome.png`. The type of the image is explicitly specified with the `--type` parameter. In this case, data is written into a PNG file.

    imclip -i puppies.jpg

Copies the image inside `puppies.jpg` into the clipboard. 

    imclip --in=robots.png

Copies the image inside `robots.png` into the clipboard.
