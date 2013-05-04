#include <cv.h>
#include<cvcam.h>
#include<cvaux.h>
#include <highgui.h>


#include <stdlib.h>
#include <stdio.h>
#include "usamaCV.h"
#include <windows.h>
//#include <strmif.h>
//#include <dshow.h>
#include "display.h"

#include <GL/glut.h>



#define ScreenX 1280
#define ScreenY 800


IplImage* dst = 0;
IplImage* cap = 0;
IplImage* bg = 0;
IplImage* newskin;
IplImage* testing;
IplImage* skingray;
IplImage* dstGray;
IplImage* skin;
IplImage* graph;
IplImage* newgray;



//the address of variable which receives trackbar position update 
int dilate = 1;
int dilate2=3;
int erode = 1;
int thresh= 10;
int level = 3;
int precision=30;
int precision2=7;
int histkey;
int scale=1;
float Z=0;
int factor=1;

//Capture
CvCapture* captureL;
//Storage and contours
CvMemStorage* stor;
CvMemStorage* stor2;
CvMemStorage* stor3;
CvMemStorage* stor4;
CvMemStorage* stor5;
CvMemStorage* stor6;
CvMemStorage* stor7;
CvSeq* cont;
CvSeq* cont1;
CvSeq* cont2;
CvSeq* cont3;
CvSeq* cont4;
CvSeq* cont5;
CvSeq* cont6;
CvSeq* cont7;
//CvPoint *WholePointArray;
//CvPoint *PointArray;
CvPoint* pto;
CvPoint* pto1;
float* dist;
CvPoint* del;
CvPoint* tempo;

CvPoint maxYpt, cent;

void drawKeyboad(IplImage* src,int x,int y,int width, int height,CvPoint pt);
bool isInside(CvRect rec,CvPoint pt);
void skinDetection(IplImage* image);
float calcAngle(CvPoint x,CvPoint y,CvPoint z);

DWORD WINAPI Communicate(LPVOID Parameter);

bool debug=true;//true to show lines,  countours ...
bool skin_motion=true;//true for skin, false for motion
bool oldLogic=false;
bool newLogic=false;
bool useconvexhull=true;
bool zSet=false;
int BGchoice=1;//1 for absolute background i.e choose background. 0 for bg = previous frame


CvPoint fingers[5];

int argcG; char **argvG;

IplImage* snap1,*snap2,*snap3,*snap4,*test,*object;
int*** HLS;//[3][2][3];
//0-Orange
CvMat* mmat = cvCreateMat(3,3,CV_32FC1);

int initColours()
{
	HLS=(int***)calloc(3,sizeof(int**));
	for(int x=0;x<3;x++)
	{
		HLS[x]=(int**)calloc(2,sizeof(int*));
		for(int y=0;y<2;y++)
		HLS[x][y]=(int*)calloc(3,sizeof(int));
	}
	
	//0-GREEN
	HLS[0][0][0]=47;		HLS[0][1][0]=94;
	HLS[0][0][1]=28;	HLS[0][1][1]=58;
	HLS[0][0][2]=78;	HLS[0][1][2]=192;

	//1-
	HLS[1][0][0]=83;	HLS[1][1][0]=120;
	HLS[1][0][1]=41;	HLS[1][1][1]=136;
	HLS[1][0][2]=60;	HLS[1][1][2]=179;

	//2-
	HLS[2][0][0]=113;	HLS[2][1][0]=161;
	HLS[2][0][1]=64;	HLS[2][1][1]=172;
	HLS[2][0][2]=70;	HLS[2][1][2]=157;

	return 0;
}


CvPoint pointFinder(IplImage* src)
{
	CvMemStorage* stor;
	CvSeq* cont;
	CvSeq* contMax;
	CvBox2D32f* box;
	CvPoint* PointArray;
    CvPoint2D32f* PointArray2D32f;
    
	CvPoint center;

	stor = cvCreateMemStorage(0);
	cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
	
	cvFindContours( src, stor, &cont, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0,0));

	float maxArea=0;
	CvPoint maxCenter=cvPoint(0,0);
	
	bool pointFound=false;
	
	for(int j=0;cont;cont = cont->h_next,j++)
    {
		float area;
		area=  cvContourArea(cont,CV_WHOLE_SEQ );
		area=fabs(area);
		
		if(maxArea<area)
		{
			maxArea=area;
			pointFound=true;
			contMax=cont;
		}
		
	}

	if(pointFound)
	{
		int count = contMax->total;
		
		if(debug)
		cvDrawContours(skin,contMax,CV_RGB(0,255,0),CV_RGB(255,0,0),0,2,8,cvPoint(0,0));
		
		box = (CvBox2D32f*)malloc(sizeof(CvBox2D32f));
		PointArray = (CvPoint*)malloc( count*sizeof(CvPoint) );
        PointArray2D32f= (CvPoint2D32f*)malloc( count*sizeof(CvPoint2D32f) );
        
		cvCvtSeqToArray(contMax, PointArray, CV_WHOLE_SEQ);
        
        // Convert CvPoint set to CvBox2D32f set.
        for(int i=0; i<count; i++)
        {
            PointArray2D32f[i].x = (float)PointArray[i].x;
            PointArray2D32f[i].y = (float)PointArray[i].y;
        }
        
        // Fits ellipse to current contour.
        cvFitEllipse(PointArray2D32f, count, box);
        
		center.x = cvRound(box->center.x);
        center.y = cvRound(box->center.y);
        
        free(PointArray);
        free(PointArray2D32f);
        free(box);
	}
	else
		return cvPoint(-1,-1);

	cvReleaseMemStorage(&stor);
	return center;
}


void colorDetectToSingleChannelHLS(IplImage* src,IplImage* dst, int** HLS)
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
			 if(
				 data_src[j]>HLS[0][0]	&&	data_src[j]<HLS[1][0]
				 
				 &&
				 
				 data_src[j+1]>HLS[0][1]	&&	data_src[j+1]<HLS[1][1]
				 
				 &&
				 
				 data_src[j+2]>HLS[0][2]	&&	data_src[j+2]<HLS[1][2]
				 
				 )
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
int main( int argc, char** argv )
{
	initColours();
    argcG=argc;
    argvG=argv;
   HANDLE hThread;	//Handle to thread
    DWORD ThreadId;	//used to store the thread id


    int maxcount=0;
    char c=0;
    captureL = cvCaptureFromCAM(0);//cvCaptureFromFile( "test.avi" );
    
    if(!cvGrabFrame(captureL)){              // capture a frame 
		printf("Could not grab a frame from camera\n");
        cvWaitKey(0);
		exit(0);
    } 



	cvNamedWindow("Tools",0);
	
	cvCreateTrackbar("Min Hue","Tools",&HLS[0][0][0],255,0);
	cvCreateTrackbar("Max Hue","Tools",&HLS[0][1][0],255,0);

	cvCreateTrackbar("Min Saturation","Tools",&HLS[0][0][1],255,0);
	cvCreateTrackbar("Max Saturation","Tools",&HLS[0][1][1],255,0);

	cvCreateTrackbar("Min Luminisence","Tools",&HLS[0][0][2],255,0);
	cvCreateTrackbar("Max Luminisence","Tools",&HLS[0][1][2],255,0);
    
    cvNamedWindow("Capture", 0); 
    cvResizeWindow("Capture",300,300);
    cvMoveWindow("Capture", 0, 300);
    //cvCreateTrackbar("Dilate","Capture",&dilate2,10,0);
    cvCreateTrackbar("Dilate","Capture",&dilate,10,0);
	cvCreateTrackbar("Erode","Capture",&erode,10,0);
    
    
    cvNamedWindow("Skin", 0); 
    cvResizeWindow("Skin",800,600);
    cvMoveWindow("Skin", 0, 0);
	//cvCreateTrackbar("Level 'val-3' ","Skin",&level,6,0);
	cvCreateTrackbar("Precision","Skin",&precision,200,0);
    cvCreateTrackbar("Precision Skin Erode","Skin",&precision2,200,0);
    cvCreateTrackbar("Erode","Skin",&scale,10,0); 
    cvCreateTrackbar("Factor","Skin",&factor,10,0); 
	//cvCreateTrackbar("Z","Skin",&factor,10,0);

    cvNamedWindow("Testing", 1); 
    cvMoveWindow("Testing", 630, 0);
    cvResizeWindow("Testing",300,300);
    
	cap = cvQueryFrame(captureL);
    bg=cvCreateImage( cvGetSize(cap), 8, 3 );
     //bg=cvCloneImage(cap);
     //src=cvCloneImage(cap);

    //hThread = CreateThread(NULL,0,Communicate,0,0,&ThreadId);


	object=  cvLoadImage("axis.bmp");
    while(1)
    {
        
        dst=cvCreateImage( cvGetSize(cap), 8, 3 );
        skingray=cvCreateImage( cvGetSize(cap), IPL_DEPTH_32F, 1 );
		newgray=cvCreateImage( cvGetSize(cap), 8, 1 );
        skin=cvCloneImage(cap);
        graph=cvCloneImage(cap);//cvCreateImage( cvSize(320,200), 8, 3 );
        testing=cvCreateImage( cvGetSize(cap), 8, 1 );
        
       
		stor = cvCreateMemStorage(0);
	stor2 = cvCreateMemStorage(0);
	stor3 = cvCreateMemStorage(0);
    stor4 = cvCreateMemStorage(0);
    stor5 = cvCreateMemStorage(0);
    stor6 = cvCreateMemStorage(0);
	stor7 = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
	cont2 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor2);
	cont3 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor3);
    cont4 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor4);
    cont5 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor5);
    cont6 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor6);
    cont7 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor7);

	
    	
        cap = cvQueryFrame(captureL);
		
        if(skin_motion)//true for skin
		{
             skinDetectToSingleChannel(cap,newgray);
			//skinDetectToSingleChannelHSV(graph,newgray);
               
               cvErode(newgray,testing,0,scale);
               
               cvFindContours( testing, stor4, &cont4, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
               //cont7 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor7);
               for(;cont4;cont4 = cont4->h_next)
		        {
			        cont6= cvApproxPoly(cont4,sizeof(CvContour),stor6,CV_POLY_APPROX_DP,precision2,0 );
                    
 
                    //cont2=cont;
                    if(cont6->total>maxcount)
			        {
				        cont5=cont6;
				        maxcount=cont6->total;
			        }
		        }
               //if ( cont6->total >= ) 
               
			   if(debug)
              //  cvDrawContours(skin,cont5,CV_RGB(0,0,255),CV_RGB(255,0,0),level-3,3,8,cvPoint(0,0));
                
			   pto1= (CvPoint*)calloc( maxcount,sizeof(CvPoint) );
                del= (CvPoint*)calloc( maxcount,sizeof(CvPoint) );
                dist= (float*)calloc( maxcount,sizeof(float) );
                tempo =(CvPoint*)calloc( maxcount,sizeof(CvPoint) );
            
                cvCvtSeqToArray(cont5, pto1, CV_WHOLE_SEQ);
                

                float sumx=0,sumy=0;
            
            
            
                for(int i=0;i<cont5->total;i++)
			{
				sumx+=pto1[i].x;
				sumy+=pto1[i].y;
			}
			CvPoint cent=cvPoint(cvRound(sumx/maxcount),cvRound(sumy/maxcount));
			if(debug)
            cvCircle(skin,cent,5,CV_RGB(255,0,0),3,8,0);// drawing centroid
			
			
                free(pto1);
               //cvShowImage("Testing",testing);
               
        }  
        else// false for motion
        {
        
              cvAbsDiff( bg,cap , dst );
              cvCvtColor( dst, newgray, CV_BGR2GRAY );
              cvThreshold(newgray, newgray, thresh, 255, CV_THRESH_BINARY);
        }
        
        if(c=='s'||c=='S')
			{
                skin_motion=!skin_motion;       
                
            } 

        if(c=='l'||c=='L')
			{
                oldLogic=!oldLogic;
                
            } 

        if(c=='d'||c=='D')
			{
                debug=!debug;       
                //cvSaveImage("bg.bmp",bg);
            }  
         
        if(BGchoice==1)//1 for absolute background i.e choose background. 0 for bg = previous frame
        {
         if(c=='b'||c=='B')
			{
                cvReleaseImage( &bg );
                bg=cvCreateImage( cvGetSize(cap), 8, 3 );
                bg=cvCloneImage(cap);
                       
                //cvSaveImage("bg.bmp",bg);
            }  
         
         
        }
        else
         {
         cvReleaseImage( &bg );
         bg=cvCreateImage( cvGetSize(cap), 8, 3 );
         bg=cvCloneImage(cap);
         }
       
       
        cvErode(newgray, newgray, 0, erode);
		cvDilate(newgray, newgray, 0, dilate);
		
        cvShowImage("Capture",newgray);
        
        newskin=cvCloneImage(newgray);

        cvFindContours( newgray, stor, &cont, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
                
        
        maxcount=0;
		for(;cont;cont = cont->h_next)
		{
			cont2= cvApproxPoly(cont,sizeof(CvContour),stor2,CV_POLY_APPROX_DP,precision,0 );
            //cont2=cont;
            if(cont2->total>maxcount)
			{
				cont3=cont2;
				maxcount=cont2->total;
			}
		}
		//printf("\tMaxcount: %d\n",maxcount);
		
        
		if(maxcount>0)
        {
            
            cvSet( graph,CV_RGB(255,255,255) );
            float maxdist=0,xf,yf;CvPoint maxpt=cvPoint(0,0);
            
            if(debug)
			    cvDrawContours(skin,cont3,CV_RGB(0,255,0),CV_RGB(255,0,0),level-3,7,8,cvPoint(0,0));
			
            pto= (CvPoint*)calloc( maxcount,sizeof(CvPoint) );
            del= (CvPoint*)calloc( maxcount,sizeof(CvPoint) );
            dist= (float*)calloc( maxcount,sizeof(float) );
            tempo =(CvPoint*)calloc( maxcount,sizeof(CvPoint) );
            
			cvCvtSeqToArray(cont3, pto, CV_WHOLE_SEQ);
			
            
            float sumx=0,sumy=0;
            
            
            
            for(int i=0;i<maxcount;i++)
			{
				sumx+=pto[i].x;
				sumy+=pto[i].y;
			}
			CvPoint cent=cvPoint(cvRound(sumx/maxcount),cvRound(sumy/maxcount));
			if(debug)
            cvCircle(skin,cent,5,CV_RGB(0,255,255),3,8,0);// drawing centroid
			
            if(useconvexhull)
            {
                //cvErode(skin, skin, 0, erode);
                char key;
                int i, hullcount;
                CvPoint pt0;
                
                int* hull = (int*)malloc( maxcount * sizeof(hull[0]));
                CvMat pointMat = cvMat( 1, maxcount, CV_32SC2, pto );
                CvMat hullMat = cvMat( 1, maxcount, CV_32SC1, hull );
    
                
                cvConvexHull2( &pointMat, &hullMat, CV_CLOCKWISE, 0 );
                hullcount = hullMat.cols;

                
                for( i = 0; i < maxcount; i++ )
                {
		        pt0 = pto[i];
				if(debug)
		        cvCircle( skin, pt0, 2, CV_RGB( 255, 0, 255 ), CV_FILLED, CV_AA, 0 );
                
                }

                
				CvPoint* arr= (CvPoint*)calloc( hullcount,sizeof(CvPoint) );
				pt0 = pto[hull[hullcount-1]];
		 
				

				CvPoint fingers[5];
				for( i = 0; i < hullcount; i++ )
                {
					arr[i]=pto[hull[i]];
				}
				

				CvPoint tmp;
				              
				
				for( i = 0; i < hullcount; i++ )
                {
					CvPoint pt=pto[hull[i]];
					if(debug)
					cvLine( skin, pt0, pt, CV_RGB( 0, 0, 0 ), 1, CV_AA, 0 );
					pt0 = pt;

				for( int j = i+1; j < hullcount; j++ )
					{
						if(arr[i].y<arr[j].y)
						{
							tmp=arr[i];
							arr[i]=arr[j];
							arr[j]=tmp;
						}
					}
                }

				if(hullcount>=5)
                {
                    for( i = 0; i < 5; i++ )
                {
                     fingers[i]=arr[i];
					cvCircle( skin,arr[i], 8, CV_RGB( 255, 255,0 ), CV_FILLED, CV_AA, 0 );
				}

                for( i = 0; i < 5; i++ )
                {
					
				for( int j = i+1; j < 5; j++ )
                {
                    if(fingers[i].x<fingers[j].x)
						{
							tmp=fingers[i];
							fingers[i]=fingers[j];
							fingers[j]=tmp;
						}
                }}
                
                cvCircle( skin,fingers[0], 5, CV_RGB( 255, 0,127 ), CV_FILLED, CV_AA, 0 );
                cvCircle( skin,fingers[2], 5, CV_RGB( 255, 0,127 ), CV_FILLED, CV_AA, 0 );
                
                int m=0;
                m=(fingers[0].x+fingers[2].x)/2;
                int n=0;
                n=(fingers[0].y+fingers[2].y)/2;
                int z1,z2;
                float z3,z4,z;
                z1=fingers[2].x-fingers[0].x;
                z2=fingers[2].y-fingers[0].y;
                z3=pow((float)z1,2);
                z4=pow((float)z2,2);
                z=sqrt(z3+z4);

               

                Z=cvRound(z)/4;
                
                cvCircle(skin,cvPoint(cvRound(m),cvRound(n)),Z,CV_RGB(127,255,100),1,8,0);
                printf("X: %.2f\nY: %.2f\nZ: %.2f\n\n\n",m,n,Z);


				snap2=  cvCreateImage(	cvGetSize(skin),IPL_DEPTH_8U, 1);
				test=  cvCreateImage(	cvGetSize(skin),IPL_DEPTH_8U, 3);

				CvPoint p4;
				float m1,m2,c1,c2;

				cvCvtColor(skin,skin,CV_BGR2HLS);
				colorDetectToSingleChannelHLS(skin,snap2,HLS[0]);
				cvCvtColor(skin,skin,CV_HLS2BGR);

				cvErode(snap2,snap2, 0, 1);
				cvDilate(snap2,snap2, 0, 1);		

				CvPoint center=pointFinder(snap2);
				CvPoint thumb=fingers[0];
				CvPoint finger=fingers[2];

				bool allFound=false;
                if(center.x!=-1)
				allFound=true;
                
				if(allFound)
				{
				cvLine(skin,center,thumb,CV_RGB(0,0,255),5);
				cvLine(skin,center,finger,CV_RGB(255,0,0),5);

				m1=((float)(center.y-thumb.y))/((float)(center.x-thumb.x));
				m2=((float)(center.y-finger.y))/((float)(center.x-finger.x));

				c2=thumb.y-m2*thumb.x;
				c1=finger.y-m1*finger.x;

				p4.x=(c2-c1)/(m1-m2);
				p4.y=m1*p4.x+c1;

			
				cvCircle(skin,p4,5,CV_RGB(255,255,0),5);

				cvLine(skin,p4,thumb,CV_RGB(0,0,255),5);
				cvLine(skin,p4,finger,CV_RGB(255,0,0),5);
	
				CvPoint2D32f dstP[4] = {cvPoint2D32f(p4.x,p4.y),cvPoint2D32f(finger.x,finger.y),cvPoint2D32f(thumb.x,thumb.y),cvPoint2D32f(center.x,center.y)};
				CvPoint2D32f srcP[4] = {cvPoint2D32f(0,0),cvPoint2D32f(object->width,0),cvPoint2D32f(0,object->height),cvPoint2D32f(object->width,object->height)};
				
				mmat = cvGetPerspectiveTransform(srcP, dstP, mmat);
				cvWarpPerspective(object,test,mmat);

				cvAdd(test,skin,skin);
				
				}
                
				xm=(m-320)/10;
                ym=(n-240)/10;
                zm=-(50*factor-Z)-30;
                
                
                //OnIdle();
                
                
                }
                //cvShowImage( "Skin", skin );
				cvShowImage( "Testing", skin );
                //cont7 = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
                
                free( hull );

                }
           

            
              printf("\n\t going to release...");
             
              

              printf("\n\t free(dist)");
              free(dist);
              printf("\n\t free(tempo)");
              free(tempo);
              printf("\n\t free(pto)");
              free(pto);
              printf("\n\t free(del)");
              free(del);
              
              

            printf("\n released ");
        }
		
      
        
        //cvShowImage("Skin", skin );
        
              cvShowImage("Graph", graph );
              cvReleaseImage( &graph );

        c=cvWaitKey(1);  
        
       
        
		cvReleaseImage( &dst );
		cvReleaseImage( &snap2 );
		cvReleaseImage( &test );
        
        cvReleaseImage( &skingray );
        cvReleaseImage( &skin );
        cvReleaseImage( &newskin);
        cvReleaseImage(&newgray);
        
        cvReleaseImage(&testing);
        cvReleaseMemStorage( &stor7 );
        cvReleaseMemStorage( &stor6 );
        cvReleaseMemStorage( &stor5 );
        cvReleaseMemStorage( &stor4 );
        cvReleaseMemStorage( &stor3 );
        cvReleaseMemStorage( &stor2 );
        cvReleaseMemStorage( &stor );
		
        //cvReleaseImage( &dstGray );
    }
    

    
	
    return 0;
}


/*int DrawGLScene(GLvoid)						            // Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();					                // Reset The View
	glTranslatef(-1.5f,0.0f,-6.0f);				        // Move Into The Screen And Left
    glRotatef(rtri,0.0f,1.0f,0.0f);	
    glBegin(GL_TRIANGLES);					            // Start Drawing A Triangle
	glColor3f(1.0f,0.0f,0.0f);			                // Set Top Point Of Triangle To Red
	glVertex3f( 0.0f, 1.0f, 0.0f);			            // First Point Of The Triangle
	glColor3f(0.0f,1.0f,0.0f);			                // Set Left Point Of Triangle To Green
	glVertex3f(-1.0f,-1.0f, 0.0f);			            // Second Point Of The Triangle
	glColor3f(0.0f,0.0f,1.0f);			                // Set Right Point Of Triangle To Blue
	glVertex3f( 1.0f,-1.0f, 0.0f);			            // Third Point Of The Triangle
	glEnd();			
    glLoadIdentity();					                // Reset The Current Modelview Matrix
	glTranslatef(1.5f,0.0f,-6.0f);				        // Move Right 1.5 Units And Into The Screen 6.0
	glRotatef(rquad,1.0f,0.0f,0.0f);
    glColor3f(0.5f,0.5f,1.0f);				            // Set The Color To A Nice Blue Shade
	glBegin(GL_QUADS);					                // Start Drawing A Quad
	glVertex3f(-1.0f, 1.0f, 0.0f);			        // Top Left Of The Quad
	glVertex3f( 1.0f, 1.0f, 0.0f);			        // Top Right Of The Quad
	glVertex3f( 1.0f,-1.0f, 0.0f);			        // Bottom Right Of The Quad
	glVertex3f(-1.0f,-1.0f, 0.0f);			        // Bottom Left Of The Quad
	glEnd();
    rtri+=0.2f;						                // Increase The Rotation Variable For The Triangle ( NEW )
	rquad-=0.15f;						            // Decrease The Rotation Variable For The Quad     ( NEW )
	return TRUE;
}*/
float calcAngle(CvPoint x,CvPoint y,CvPoint z)
{
      
                    
    float angle,M1,M2;   
    
    M1=(  ( (float)(z.y-y.y)  )  /  (  (float)(z.x-y.x)  )  );

    M2=(  ( (float)(x.y-y.y)  )  /  (  (float)(x.x-y.x)  )  );

    //printf("\nM1: %.1f,M2: %.1f",M1,M2);
    
    
    angle=(atan((M1-M2)/(1+M1*M2))*180)/3.14;
    
    return angle;
}


void skinDetection(IplImage* image)// special cse of function colorDetect
{
	int nl= image->height; // number of lines
    int nc= image->width * image->nChannels; // total number of element per line
    int step= image->widthStep; // effective width
    double c1,c2,c3;     

    // get the pointer to the image buffer
    unsigned char *data= reinterpret_cast<unsigned char *>(image->imageData);

	for (int i=1; i<nl; i++) {
           for (int j=0; j<nc; j+= image->nChannels) {

           

               if (data[j+2] != 0 && data[j] != 0)
               {

                c1 = 1 - ((double)data[j+2] / (double)data[j]);
                c2 = (((double)data[j+2] + (double)data[j+1] + (double)data[j]) / (3 * (double)data[j+2])) + (((double)data[j+2] - (double)data[j+1]) / ((double)data[j+2] + (double)data[j+1] + (double)data[j]));
               }
               else
               {
                    c1 = 0;
                    c2 = 1;
               }
			   
			   if(c1 <= -0.0905 && c2 <= 0.9498)
               {
                    data[j+2]=255;
					data[j+1]=0;
					data[j]=0;
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
bool isInside(CvRect rec,CvPoint pt)
{
    if(pt.x>rec.x&&pt.x<(rec.x+rec.width)&&pt.y<rec.y&&pt.y>(rec.y-rec.height))
        return true;
    else return false;
}



DWORD WINAPI Communicate(LPVOID Parameter)
{
    GLmain(argcG,argvG);
    return 0;
}