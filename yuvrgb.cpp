#include "DBG_MACROS.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef unsigned char uchar;

// ---------------------------------------
// adjustLimit
// adjust the rgb to 8 bit limits
// ---------------------------------------
double adjustLimit(double val)
{
   if (val < 0)
   {
      return 0;
   }
   else if (val > 255)
   {
      return 255;
   }
   else
   {
      return val;
   }   
}

// ---------------------------------------
// return red pixel
// ---------------------------------------
double getR(double y, double cr)
{
//   double r = y + (1.4065 * (cr - 128));
//   double r = y + 1.139834579 * cr;
//   double r = 1.164 * (y - 16) + 1.596 * (cr - 128); // from fourcc.org/fccyvrgb.php
   cr -= 128;
   double r = y + 1.403 * cr;
   return adjustLimit(r);
}


// ---------------------------------------
// return green pixel
// ---------------------------------------
double getG(double y, double cb, double cr)
{
//   double g = y - (0.3455 * (cb - 128)) - (0.7169 * (cr - 128));
//   double g = y - 0.3946460533 * cb - 0.58060 * cr;
//   double g = 1.164 * (y - 16) - 0.813 * (cr - 128) - 0.391 * (cb -128); // from fourcc.org/fccyvrgb.php
   cr -= 128;
   cb -= 128;
   double g = y - 0.344 * cb - 0.714 * cr;
   return adjustLimit(g);
}

// ---------------------------------------
// return blue pixel
// ---------------------------------------
double getB(double y, double cb)
{
//   double b = y + (1.7790 * (cb - 128));
//   double b = y + 2.032111938 * cb;
//   double b = 1.164 * (y - 16) + 2.018 * (cb - 128); // from fourcc.org/fccyvrgb.php
   cb -= 128;
   double b = y + 1.772 * cb;
   return adjustLimit(b);
}

// ---------------------------------------
// main
// ---------------------------------------
int main(int argc, char *argv[] )
{
   // read in data from file
   
   FILE *pInput = fopen("./images/imgYUV.raw", "rb");

   
   if (pInput == NULL)
   {
      DBG_WRN("input file is null");
   }
   else
   {
      DBG_MSG("input file is NOT null");
      // read file into a buffer
      
      fseek(pInput, 0, SEEK_END);
      long sz = ftell(pInput);
      fseek(pInput, 0, SEEK_SET);
      
      DBG_MSG("file size = %lu bytes", sz);
      
      uchar *pBuf = new uchar[sz];
      
      if (pBuf == NULL)
      {
         DBG_WRN("pBuf = NULL");
      }
      else
      {
         DBG_MSG("pBuf is NOT NULL");
         
         // read the file into the buffer
         fread(pBuf, 1, sz, pInput);
         
         uchar *pBufY =  new uchar[sz/2];
         uchar *pBufCb = new uchar[sz/4];
         uchar *pBufCr = new uchar[sz/4];
         
         // extract the planar yCbCr components from the image
         
         // memcopy the Y 
         memcpy(pBufY, pBuf, sz/2);
         
         // memcopy the Cb
         pBuf += sz/2;
         memcpy(pBufCb, pBuf, sz/4);
         
         // memcpy the Cr
         pBuf += sz/4;
         memcpy(pBufCr, pBuf, sz/4);
         
         DBG_MSG("done memcpys");
                  
         
         // open destination files
         FILE *pY     = fopen("./images/imgY.raw", "wb");
         FILE *pCb    = fopen("./images/imgCb.raw", "wb");
         FILE *pCr    = fopen("./images/imgCr.raw", "wb");

         DBG_WRN("write buffers to files");
                  
         // write buffers to the files
         fwrite(pBufY, 1, sz/2, pY);
         fwrite(pBufCb, 1, sz/4, pCb);
         fwrite(pBufCr, 1, sz/4, pCr);
                           
         
         // create rgb files & buffers
         int rgbSz = 640*480;
         uchar *pBufR   = new uchar[rgbSz];
         uchar *pBufG   = new uchar[rgbSz];
         uchar *pBufB   = new uchar[rgbSz];
         uchar *pBufRGB = new uchar[rgbSz*3];
         
         double r = 0;
         double g = 0;
         double b = 0;
         double y = 0;
         double cb = 0;
         double cr = 0;
         int cbcrIdx = 0;
         
         FILE *pR    = fopen("./images/imgR.raw", "wb");
         FILE *pG    = fopen("./images/imgG.raw", "wb");
         FILE *pB    = fopen("./images/imgB.raw", "wb");
         FILE *pRGB  = fopen("./images/imgRGB.raw", "wb");
         
         for (int i = 0 ; i < rgbSz; i++)
         {
            // ---- first macro pixel ----
            y  =  pBufY[i];
            cb = pBufCb[cbcrIdx];
            cr = pBufCb[cbcrIdx];
            
            // calculate rgb
            r = getR(y, cr);
            g = getG(y, cb, cr);
            b = getB(y, cb);
            
            // set the array values
            pBufR[i] = static_cast<uchar>(r);
            pBufG[i] = static_cast<uchar>(g);
            pBufB[i] = static_cast<uchar>(b);
            
            // ---- second macro pixel ----
            ++i;
            
            y  = pBufY[i];
            cb = pBufCb[cbcrIdx];
            cr = pBufCb[cbcrIdx];
            
            // calculate rgb
            r = getR(y, cr);
            g = getG(y, cb, cr);
            b = getB(y, cb);
            
            // set the array values
            pBufR[i]   = static_cast<uchar>(r);
            pBufG[i]   = static_cast<uchar>(g);
            pBufB[i]   = static_cast<uchar>(b);
            
            ++cbcrIdx;
         } // ~ for (i<rgbSz)
         
         // create RGB data
         for (int i = 0, pixIdx = 0 ; i < rgbSz * 3; i+=3, pixIdx++)
         {
            pBufRGB[i]   = pBufR[pixIdx];
            pBufRGB[i+1] = pBufG[pixIdx];
            pBufRGB[i+2] = pBufB[pixIdx];
         } // ~ for (i < rgbSz*3)
         
         // write rgb buffers to the files
         fwrite(pBufR,   1, rgbSz, pR);
         fwrite(pBufG,   1, rgbSz, pG);
         fwrite(pBufB,   1, rgbSz, pB);
         fwrite(pBufRGB, 1, rgbSz*3, pRGB);
         
        
         DBG_WRN("close the files");
         // close the files
         fclose(pInput);
         fclose(pY);
         fclose(pCb);
         fclose(pCr);
         fclose(pR);
         fclose(pG);
         fclose(pB);  
         fclose(pRGB);
     
         DBG_WRN("cleanup the memory");
     
         delete[] pBufY;
         delete[] pBufCb;
         delete[] pBufCr;
         delete[] pBufR;
         delete[] pBufG;
         delete[] pBufB;
         delete[] pBufRGB;

         if (pBuf != NULL)
         {
            // reset the pointer to delete it properly
            pBuf -= sz/4;
            pBuf -= sz/2;
            delete[] pBuf;
         }  
      }
      
      
      

   }
   
    
   return 0;
}

