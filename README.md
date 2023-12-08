# HD OSD Font Tool
A tool for compiling fonts for HD system OSDs. Currently, only the Avatar (Walksnail/Caddx) and HDZero HD systems are supported.

## Introduction
This work was initially done as part of preparing the [INAV](https://github.com/iNavFlight) font for the Avatar HD system. In INAV, the characters each have their own image file. This makes editing and customising the fonts much simpler. My plan was to take the same approach with the Avatar font; and have a tool to compile the image used in their firmware. Inspiration coming from the [max67456tool](https://github.com/fiam/max7456tool) by fiam.

## The Tool
The HD OSD Font Tool will take the individual character images and convert them in to the image used by OSDs. It will also convert alternate fonts found in the same main directory that the program is run from or pointed to. An optional name can also be given for the output files. This is all done without the need for configuration files. However, there are a couple of caveats for using the tool.

### The Avatar font image file
The Avatar firmware uses a PNG file for it's fonts. It does use alpha transparency, so you can create some really cool symbols. The current Avatar firmware uses a font file which is one character wide by 256 characters x the number of pages high. At this point in time, INAV uses 2 pages (so 512 characters high). Whereas, BetaFlight and ArduPilot use 1 page (256 characters high). BetaFlight now also use 4 pages, from version 4.4.

### The image files
Image files must be stored as PNG files with transparency. The Avatar and WTF OS systems use transparent pixels, rather than having a mid grey background which is removed. This means that alpha transparency and full colour can be used in your fonts. HDZero uses a grey background for transparency. This will be added by the tool for HDZero. There are three sizes of fonts:
- **12 x 18 pixels** This is the small font, the same size as analogue
- **24 x 36 pixels** This is a mid size font for Avatar, and the font size for HDZero
- **36 x 54 pixels** This is the large font for Avatar, and the font size for WTF OS

The fonts should be places in directories named `12x18`, `24x36`, and `36x54` respectively.

### Image file names
The image file name should refer to the images position in the font only. For example `001.png`. These images will be individually imported by the tool in order.

You can also combine images to for single characters. There are two methods for this.

#### Specifiying the images indexes for the characters within the file
The first method is mostly used for an image which has a double character. This uses a **-** separator to show the indexes of the different characters within the image. For example it is a mid sized character with an image size of 48 x 36 pixels; like the satellites symbol in INAV. The file would be named `008-008.png`, indicating that the image should be divided in to two, for character indexes 8 and 9. The numbers do not have to be sequential, but the lowest number should be first. For example, 010-057.png would separate the image into characters 10 and 57. You can also specify multiple characters, such as `200-201-202-205.png`. This would result in four separate character images in indexes 200, 201, 202, and 205.

#### Sequentially grouped characters.
The second method of having multiple characters within a single image is to use the **_** separator. These characters have to be sequential. But, you can also specify a range. This makes for smaller, easier to read filenames. Examples in INAV include the logo and crosshair files. This could be used for a double character file, like the satellite too. It would work in the same way. This time our filename would be `008_009.png`. It would still create the two images, for 8 and 9. However, we can use this filename `406_408.png`. This would result in three images for indexes 406, 407, and 408.

### Directory structure
The tool uses the directory structre to find and create fonts. Here is an example directory structure.
![image](https://user-images.githubusercontent.com/17590174/189497586-e7ef02ce-e54d-4270-83c3-11d0325ae0ac.png)

The main directory is the one highlighted in blue called `fonts`. I will call that the **font root**. You don't have to name it fonts, but the structure after that needs some rigidity. This is just the starting point, and where the compiled font files are stored. You can see the three INAV font files in the image.

The first subdirectory you see is named `default`. This level of directory I will call **fontset** directories. The `default `directory must exist. For INAV, the default font directory can be found in the [INAV Configurator repository](https://github.com/iNavFlight/inav-configurator/tree/master/resources/osd/digital). Maybe BetaFlight and ArduPilot will do the same at some point? If these appear, let me know and I will link here for convenience. This contains the default font for the firmware. Inside the `default` directory are the three font size directories, which must be named `12x18`, `24x36`, and `36x54`. The default font specifies if the fontset in question uses 256 or 512 characters. It is also used to fill in any missing characters.

Underneath the `default` directory, you can see it's sibling **fontset** called `supercoolfont`. This is where I will put my customised character images. Notice that I only have a `36x54` sub directory. I took the lasy option, and decided to only change the largest font. The tool will see that the characters are missing from the smaller sizes, and will find the best alternatives for them.

### Handling missing characters
If a character is missing, the tool will try to find the next best alternative to use. Firstly it will look in the same **fontset** and check the other sizes. It does this using the most optimum to least, with regards of the amount of resizing needed. If the character is found, it will use that and resize it to the correct size.

If the character can't be found in the current **fontset**, one of two things will happen. If the font is the default font, there is no alternative but to fill the space with a blank character. This is pretty useful though, as you only need to draw characters which actually have an icon in them. The blank spaces look after themselves. However, if this is not the default font. The default font will be checked to see if it has a character it can use. The blank character is used when no other options can be found.

Currently, multiple character images are not found when searching for missing font images. This is something I hope to remedy in a future release.

### Using the tool
The tool is a simple command line executable. You can put the tool in to the **font root** directory and just run it. Or, you can specify a path where the **font root** is located. There are only three argument options:
- **?** Help
- **-system** _AVATAR_ or _HDZERO_. If no option is provided, AVATAR is generated
- **-p path** Specify the location of the font root.
- **-n prefix_name** Which is added to the beginning of the compiled font filename.
- **-xcols** Generate a multi-column font file. Note, this is not currently used by any HD system.
- **-v** Use verbose mode. This will output more detailed messages during the program execution.

If the tool is in the **font root**, you can just run
```
hdosd-font-tool
```
If you are in a different directory, or have added the tool to the OS path. You can use the first argument to specify the **font root**
```
avatar-font-tool -p "C:\my stuff\fonts"
```
Note that directories with spaces should be wrapped in quotess. The above options will result in the generation of six Avatar font files. Three for default and three for supercoolfont. These will be named:
- default_12.png
- default_24.png
- default_36.png
- supercoolfont_12.png
- supercoolfont_24.png
- supercoolfont_36.png

If you want, you can add a prefix to the filenames. To do this, you add a second argument to the command line. For example, adding the prefix **INAV**. These two examples show how to add the prefix. The first example for when the tool is in the **font root**. The second example when the tool is in the path.
```
hdosd-font-tool .\ INAV

hdosd-font-tool -p "C:\my stuff\fonts" -n INAV
```
The above options will result in the generation of six Avatar font files:
- INAV_default_12.png
- INAV_default_24.png
- INAV_default_36.png
- INAV_supercoolfont_12.png
- INAV_supercoolfont_24.png
- INAV_supercoolfont_36.png

## Resources

### Walksnail OSD
**Walksnail FC OSD Font update** can be found in the [Walksnail download centre](https://walksnail.com/pages/download-center). Their instructions for updating are:
1. Modify the font_update.ini
2. Copy font_update.ini and font png files to the sd card
3. Insert the SD card in to goggles
4. Open the goggles **display** menu, click **Font Update** to update the font
5. select **Custom Font** and exit the menu
