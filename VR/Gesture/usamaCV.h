//usamaCV.h
//version: 0.1  
//14-11-2010
//Rough Version


void colorDetect(IplImage* image);// this is general case color detection
void colorDetectToSingleChannel(IplImage* src,IplImage* dst);
void placePixels(IplImage* src,IplImage* mask);
void skinDetect(IplImage* image);// special case of function colorDetect
void skinDetectToSingleChannel(IplImage* src,IplImage* dst);
void skinDetectToSingleChannelHSV(IplImage* src,IplImage* dst);
void skinDetectToSingleChannel(IplImage* src,IplImage* dst,IplImage* mask);//preferebly single channel mask
