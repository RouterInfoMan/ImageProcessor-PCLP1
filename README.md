
# ImageProcessor-PCLP1

A lightweight console-based image processor written in C for .bmp files.  
This assignment was given in the PCLP1 course for the academic year 2023-2024, CB series, UPB ACS CTI.


## Installation and compiling
Clone the repo with:
```
git clone https://github.com/RouterInfoMan/ImageProcessor-PCLP1.git
```

Use the following ```make``` command in the repo folder to compile the program:

```
  make interactive
```
    
Then just run the program like this:
```
./interactive
```
Then you will be able to input your commands, and edit your images!
## Documentation
| Command | Arguments     | Description                                   |
| ---     | ---           | ---                                           |
| e       | -             | Closes the program                            |
| l       | N M path      | Opens a N x M .bmp file located at ```path``` |
| s       | index path    | Saves the image at ```index``` to ```path```  |
| ah      | index         | Applies a horizantal flip to the image at ```index```|
| ar      | index         | Applies a 90&deg; anti-clockwise rotation to the image at ```index```|
| ac      | index x y w h | Applies a crop for the image at ```index``` with ```x y ``` coordinates for  
| | |  the top corner and ```w h``` width and height |
| ae      | index rows cols R G B | Applies an extend for the image at ```index``` extending  
| | |the image with ```rows``` pixels up and down and ```cols``` pixels left and right, pixels are of color R G B|
| ap      | index_dst index_src x y | Pastes image from ```index_src``` at ```index_dst``` at ```x y``` coordinates |
| cf     | size [list of values] | Creates a ```size x size``` filter using inputted values |
| af | index_img index_filter | Applies filter from ```index_filter``` to image at ```index_img``` |
| df | index_filter | Deletes filter at specified index |
| di | index_img | Deletes image at specified index |

Note: Deleting a filter/image, changes the next filters/images' index!


 
