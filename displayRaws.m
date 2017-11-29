# octave script (linux version of matlab)
# clear previous variables and screen
clear;
clc;

###################################################
# variables
###################################################
wd = 640;
ht = 480;
cbcrht = ht/2;
plotRows = 3;
plotCols = 3;

###################################################
# read in the image data 
###################################################
yuv = fread( fopen('./images/imgYUV.raw', 'r' ), wd*ht*2,     'uint8=>uint8' ); 

y  = fread( fopen('./images/imgY.raw', 'r' ), wd*ht,     'uint8=>uint8' );
cr = fread( fopen('./images/imgCb.raw', 'r'), wd*cbcrht, 'uint8=>uint8' );
cb = fread( fopen('./images/imgCr.raw', 'r'), wd*cbcrht, 'uint8=>uint8' );

r = fread( fopen('./images/imgR.raw', 'r'), wd*ht, 'uint8=>uint8' );
g = fread( fopen('./images/imgG.raw', 'r'), wd*ht, 'uint8=>uint8' );
b = fread( fopen('./images/imgB.raw', 'r'), wd*ht, 'uint8=>uint8' );

###################################################
# make the inputs into matrices
###################################################
yuv = reshape(yuv, [wd ht 2] );
y   = reshape(y,   [wd ht] );
cb  = reshape(cb,  [wd cbcrht] );
cr  = reshape(cr,  [wd cbcrht] );
r   = reshape(r,   [wd ht] );
g   = reshape(g,   [wd ht] );
b   = reshape(b,   [wd ht] );

###################################################
# flip the dimensions because its mirrored (transpose )
###################################################
yuv = flipdim(yuv, 2);

y  = flipdim(y,  2);
cb = flipdim(cb, 2);
cr = flipdim(cr, 2);

r = flipdim(r,  2);
g = flipdim(g, 2);
b = flipdim(b, 2);

###################################################
# rotate the matrix to align it correctly
###################################################
yuv = rot90(yuv);

y  = rot90(y);
cb = rot90(cb);
cr = rot90(cr);

r  = rot90(r);
g  = rot90(g);
b  = rot90(b);

###################################################
# create RGB 3d matrix
###################################################
RGB = cat(3, b, r, g); # something wrong with color spaces...

###################################################
# plot the images
###################################################

##### row 1 - Y input, UV input, RGB output
subplot(plotRows, plotCols,1);
imshow(yuv(:,:,1));
title 'Y input';

subplot(plotRows, plotCols,2);
imshow(yuv(:,:,2));
title 'UV input';

subplot(plotRows, plotCols,3)
imshow(RGB);
title 'RGB output';

##### row 2 - Y, U, V separated
subplot(plotRows, plotCols,4);
imshow(y);
title 'Y';

subplot(plotRows, plotCols,5);
imshow(cb);
title 'Cb';

subplot(plotRows, plotCols,6);
imshow(cr);
title 'Cr';

##### row 3 - R, G, B converted
R = cat(3, r, zeros(ht, wd), zeros(ht, wd) );
subplot(plotRows, plotCols,7);
imshow(R);
title 'R';

G = cat(3, zeros(ht, wd), g, zeros(ht, wd) );
subplot(plotRows, plotCols,8);
imshow(G);
title 'G';

B = cat(3, zeros(ht, wd), zeros(ht, wd), b );
subplot(plotRows, plotCols,9);
imshow(B);
title 'B';






