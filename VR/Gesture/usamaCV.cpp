//usamaCV.cpp
//version: 0.1  
//13-11-2010
//Rough Version

#include <cv.h>
#include<cvcam.h>
#include<cvaux.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>


void colorDetect(IplImage* image)// this is general case color detection
{
	int nl= image->height; // number of lines
    int nc= image->width * image->nChannels; // total number of element per line
    int step= image->widthStep; // effective width
         

    // get the pointer to the image buffer
    unsigned char *data= reinterpret_cast<unsigned char *>(image->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0; j<nc; j+= image->nChannels) {

            // process each pixel ---------------------

			   if((data[j+2]>data[j] && data[j+1]>data[j])// condition for colour
				   &&(fabs((float)(data[j+2]-data[j+1]))<35)&&(fabs((float)(data[j+2]-data[j]))>50))
				{
					data[j+2]=255;
					data[j+1]=255;
					data[j]=225;
				}
			   
			   else
			   {
					data[j]=0;
					data[j+1]=0;
					data[j+2]=0;
			   }

            // end of pixel processing ----------------

		   } // end of line

		   data+= step;  // next line

      }
}



void colorDetectToSingleChannel(IplImage* src,IplImage* dst)
{
	if(src->height!=dst->height||src->width!=dst->width)
	{
		//cout<<"Source and dst images of different size";
		cvWaitKey(0);
		exit(0);
	}
	

	int nl= src->height; // number of lines
    int nc= src->width * src->nChannels; // total number of element per line
    int step_src= src->widthStep; // effective width
	int step_dst= dst->widthStep;         

    // get the pointer to the image buffer
    unsigned char *data_src= reinterpret_cast<unsigned char *>(src->imageData);
	unsigned char *data_dst= reinterpret_cast<unsigned char *>(dst->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0,k=0; j<nc; j+= src->nChannels,k+= dst->nChannels) {

            // process each pixel ---------------------
			if((data_src[j+2]>data_src[j] && data_src[j+1]>data_src[j])// condition for colour
				   &&(fabs((float)(data_src[j+2]-data_src[j+1]))<35)&&(fabs((float)(data_src[j+2]-data_src[j]))>50))
				{
					data_dst[k]=255;
				}
			   
			   else
			   data_dst[k]=0;
			   
			
			   
            // end of pixel processing ----------------

		   } // end of line

		   data_src+= step_src;  // next line
			data_dst+= step_dst;
      }
}


void placePixels(IplImage* src,IplImage* mask)
{
	if(src->height!=mask->height||src->width!=mask->width)
	{
		printf("Source and Mask images of different size");
		cvWaitKey(0);
		exit(0);
	}
	

	int nl= src->height; // number of lines
    int nc= src->width * src->nChannels; // total number of element per line
    int step_src= src->widthStep; // effective width
	int step_mask= mask->widthStep;         

    // get the pointer to the image buffer
    unsigned char *data_src= reinterpret_cast<unsigned char *>(src->imageData);
	unsigned char *data_mask= reinterpret_cast<unsigned char *>(mask->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0,k=0; j<nc; j+= src->nChannels,k+= mask->nChannels) {

            // process each pixel ---------------------

			   if(data_mask[k]==0)
			   {
					data_src[j+2]=0;
					data_src[j+1]=0;
					data_src[j]=0;
			   }
			   
            // end of pixel processing ----------------

		   } // end of line

		   data_src+= step_src;  // next line
			data_mask+= step_mask;
      }
}



void skinDetect(IplImage* image)// special cse of function colorDetect
{
	int nl= image->height; // number of lines
    int nc= image->width * image->nChannels; // total number of element per line
    int step= image->widthStep; // effective width
         

    // get the pointer to the image buffer
    unsigned char *data= reinterpret_cast<unsigned char *>(image->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0; j<nc; j+= image->nChannels) {

            // process each pixel ---------------------

			   if((data[j+2]>95 && data[j+1]>40&& data[j]>20)&&
				  (( (max(data[j+2], max(data[j+1],data[j])))-(min(data[j+2],min(data[j+1],data[j]))))>15)
				  &&(data[j+2]>data[j+1]&&data[j+2]>data[j])
				  &&(abs(data[j+2]-data[j+1])>15))
				{
					data[j+2]=0;
					data[j+1]=0;
					data[j]=255;
				
						
				}
			   
			   else
			   {
					data[j]=0;
					data[j+1]=0;
					data[j+2]=0;
			   }

            // end of pixel processing ----------------

		   } // end of line

		   data+= step;  // next line

      }
}



void skinDetectToSingleChannel(IplImage* src,IplImage* dst)
{
     double c1,c2,c3;     

	if(src->height!=dst->height||src->width!=dst->width)
	{
		//cout<<"Source and dst images of different size";
		cvWaitKey(0);
		exit(0);
	}
	

	int nl= src->height; // number of lines
    int nc= src->width * src->nChannels; // total number of element per line
    int step_src= src->widthStep; // effective width
	int step_dst= dst->widthStep;         

    // get the pointer to the image buffer
    unsigned char *data_src= reinterpret_cast<unsigned char *>(src->imageData);
	unsigned char *data_dst= reinterpret_cast<unsigned char *>(dst->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0,k=0; j<nc; j+= src->nChannels,k+= dst->nChannels) {

            // process each pixel ---------------------
			/*if((data_src[j+2]>95 && data_src[j+1]>40&& data_src[j]>20)&&
				  (( (max(data_src[j+2], max(data_src[j+1],data_src[j])))-(min(data_src[j+2],min(data_src[j+1],data_src[j]))))>15)
				  &&(data_src[j+2]>data_src[j+1]&&data_src[j+2]>data_src[j])
				  &&(abs(data_src[j+2]-data_src[j+1])>15))
				{
					data_dst[k]=255;
				}
			   
			   else
			   data_dst[k]=0;
			  */
              if (data_src[j+2] != 0 && data_src[j] != 0)
               {

                c1 = 1 - ((double)data_src[j+2] / (double)data_src[j]);
                c2 = (((double)data_src[j+2] + (double)data_src[j+1] + (double)data_src[j]) / (3 * (double)data_src[j+2])) + (((double)data_src[j+2] - (double)data_src[j+1]) / ((double)data_src[j+2] + (double)data_src[j+1] + (double)data_src[j]));
               }
               else
               {
                    c1 = 0;
                    c2 = 1;
               }
			   
			   if(c1 <= -0.0905 && c2 <= 0.9498)
               {
                    data_dst[k]=255;
               }
               else
			   {
					data_dst[k]=0;
			   }
			
			   
            // end of pixel processing ----------------

		   } // end of line

		   data_src+= step_src;  // next line
		   data_dst+= step_dst;
      }
}


void skinDetectToSingleChannelHSV(IplImage* src,IplImage* dst)
{
	cvCvtColor(src,src,CV_BGR2HSV);
     double c1,c2,c3;     

	if(src->height!=dst->height||src->width!=dst->width)
	{
		//cout<<"Source and dst images of different size";
		cvWaitKey(0);
		exit(0);
	}
	

	int nl= src->height; // number of lines
    int nc= src->width * src->nChannels; // total number of element per line
    int step_src= src->widthStep; // effective width
	int step_dst= dst->widthStep;         

    // get the pointer to the image buffer
    unsigned char *data_src= reinterpret_cast<unsigned char *>(src->imageData);
	unsigned char *data_dst= reinterpret_cast<unsigned char *>(dst->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0,k=0; j<nc; j+= src->nChannels,k+= dst->nChannels) {//

            // process each pixel ---------------------
			/*if((data_src[j+2]>95 && data_src[j+1]>40&& data_src[j]>20)&&
				  (( (max(data_src[j+2], max(data_src[j+1],data_src[j])))-(min(data_src[j+2],min(data_src[j+1],data_src[j]))))>15)
				  &&(data_src[j+2]>data_src[j+1]&&data_src[j+2]>data_src[j])
				  &&(abs(data_src[j+2]-data_src[j+1])>15))
				{
					data_dst[k]=255;
				}
			   
			   else
			   data_dst[k]=0;
			  */
			   if (data_src[j+2] >=40)
			   { if(data_src[j+1]>0.2*255&&data_src[j+1]<0.6*255)
				   {  if((data_src[j]>255*0/360&&data_src[j]<255*25/360)||(data_src[j]>255*355/360&&data_src[j]<360*255/360))
					 {
						data_dst[k]=255;
			   }}}
				else
				{
					data_dst[k]=0;
				}
			
			   
            // end of pixel processing ----------------

		   } // end of line

		   data_src+= step_src;  // next line
		   data_dst+= step_dst;
      }
}



void skinDetectToSingleChannel(IplImage* src,IplImage* dst,IplImage* mask)//single channel mask
{
     double c1,c2,c3;     

	if(src->height!=dst->height||src->width!=dst->width)
	{
		printf("Source and dst images of different size");
		cvWaitKey(0);
		exit(0);
	}
	

	int nl= src->height; // number of lines
    int nc= src->width * src->nChannels; // total number of element per line
    int step_src= src->widthStep; // effective width
	int step_dst= dst->widthStep;
    int step_mask= mask->widthStep;

    // get the pointer to the image buffer
    unsigned char *data_src= reinterpret_cast<unsigned char *>(src->imageData);
	unsigned char *data_dst= reinterpret_cast<unsigned char *>(dst->imageData);
    unsigned char *data_mask= reinterpret_cast<unsigned char *>(mask->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0,k=0,l=0; j<nc; j+= src->nChannels,k+= dst->nChannels,l+= mask->nChannels) {

            // process each pixel ---------------------
			/*if( (data_mask[k]!=0)&&   // condition for mask
                (data_src[j+2]>95 && data_src[j+1]>40&& data_src[j]>20)&&
				  (( (max(data_src[j+2], max(data_src[j+1],data_src[j])))-(min(data_src[j+2],min(data_src[j+1],data_src[j]))))>15)
				  &&(data_src[j+2]>data_src[j+1]&&data_src[j+2]>data_src[j])
				  &&(abs(data_src[j+2]-data_src[j+1])>15))
				{
					data_dst[k]=255;
				}
			   
			   else
			   data_dst[k]=0;
			  */
               if (data_src[j+2] != 0 && data_src[j] != 0 &&(data_mask[k]!=0))
               {

                c1 = 1 - ((double)data_src[j+2] / (double)data_src[j]);
                c2 = (((double)data_src[j+2] + (double)data_src[j+1] + (double)data_src[j]) / (3 * (double)data_src[j+2])) + (((double)data_src[j+2] - (double)data_src[j+1]) / ((double)data_src[j+2] + (double)data_src[j+1] + (double)data_src[j]));
               }
               else
               {
                    c1 = 0;
                    c2 = 1;
               }
			   
			   if(c1 <= -0.0905 && c2 <= 0.9498)
               {
                    data_dst[k]=255;
               }
               else
			   {
					data_dst[k]=0;
			   }
			
			   
            // end of pixel processing ----------------

		   } // end of line

		   data_src+= step_src;  // next line
			data_dst+= step_dst;
            data_mask+= step_mask; 
      }
}


