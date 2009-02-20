#include "Camera.h"
#include "Detector.h"
#include <cstdio>

const char *WINDOW_NAME = "Display Window";

void DrawFace(IplImage *img, Features &f) {
  static CvScalar colors[] = 
  {
      {{0,0,255}},
      {{0,128,255}},
      {{0,255,255}},
      {{0,255,0}},
      {{255,128,0}},
      {{255,255,0}},
      {{255,0,0}},
      {{255,0,255}}
  };
  
  CvPoint center;
  int radius;
  center.x = cvRound(f.face_position.x + 0.5*f.face_size);
  center.y = cvRound(f.face_position.y + 0.5*f.face_size);
  radius = cvRound(f.face_size*0.5);
  cvCircle( img, center, radius, colors[0], 3, 8, 0 ); // draw face
  
  // draw lips
  cvCircle(img, cvPoint(f.lip_positions[0].x,f.lip_positions[0].y), 1, colors[1], 3, 8, 0);
  cvCircle(img, cvPoint(f.lip_positions[1].x,f.lip_positions[1].y), 1, colors[1], 3, 8, 0);
	// draw nostrils
	cvCircle(img, cvPoint(f.nostril_positions[0].x,f.nostril_positions[0].y), 1, colors[0], 3, 8, 0);
	cvCircle(img, cvPoint(f.nostril_positions[1].x,f.nostril_positions[1].y), 1, colors[0], 3, 8, 0);
	
	// draw nose bridge
	cvCircle(img, cvPoint(f.nose_bridge.x,f.nose_bridge.y), 1, colors[2], 3, 8, 0);

	// draw pupils
	cvCircle(img, cvPoint(f.pupils[0].x,f.pupils[0].y), 10, colors[3], 3, 8, 0);
	cvCircle(img, cvPoint(f.pupils[1].x,f.pupils[1].y), 10, colors[3], 3, 8, 0);

}

int main(int argc, char **argv) {
  const char *filename = NULL;
  if(argc > 1)
    filename = argv[1];
  
  cvNamedWindow (WINDOW_NAME, CV_WINDOW_AUTOSIZE);
  
  const IplImage *current_frame;
  IplImage *gray = cvCreateImage(cvSize(640,480), 8, 1);
  IplImage *small_img = cvCreateImage(cvSize(640,480), 8, 3);
  Camera cam(filename);
  Detector detector;
  
  while((current_frame = cam.GetFrame())) {
    cvResize(current_frame, small_img, CV_INTER_LINEAR);
    cvCvtColor(small_img, gray, CV_BGR2GRAY);
    cvEqualizeHist(gray,gray);
    Features f = detector.ColdStart(gray);
    
    if(f.face_size) {
      DrawFace(small_img, f);
    }
//    printf("(%d,%d) %d\n",f.face_position.x,f.face_position.y,f.face_size);
    cvShowImage(WINDOW_NAME, small_img);
    cvWaitKey(10);
  }
  
  cvReleaseImage( &gray );
  cvReleaseImage( &small_img );
  return 0;
}