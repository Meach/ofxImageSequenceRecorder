/*

Based on code by Memo from this thread:
http://forum.openframeworks.cc/index.php?topic=1687.0

*/

#include "ofMain.h"

#ifdef OFX_TURBO_JPEG
#include "ofxTurboJpeg.h"
#endif

typedef struct { 
    string fileName;    
    ofPixels image;
} QueuedImage;

class ofxImageSequenceRecorder : public ofThread {    
public:    
  
    int counter;  
    queue<QueuedImage> q;
    string prefix;
    string format;
    int numberWidth;

#ifdef OFX_TURBO_JPEG
	ofxTurboJpeg turbo;	
	int quality;
#else
	ofImageQualityType quality;
#endif

    ofxImageSequenceRecorder(){  
        counter=0;  
        numberWidth=4;

#ifdef OFX_TURBO_JPEG
		quality = 100; 
#else
		quality = OF_IMAGE_QUALITY_BEST;
#endif
    }  
    
    void setPrefix(string pre){
        prefix = pre;
    }
    
    void setFormat(string fmt){
        format = fmt;
    }
       
    void setCounter(int count){
        counter = count;
    }
       
    void setNumberWidth(int nbwidth){
        numberWidth = nbwidth;
    }


#ifdef OFX_TURBO_JPEG
	void setQuality(int qlt) {
		quality = qlt;
	}
#else
	void setQuality(ofImageQualityType qlt) {
		quality = qlt;
	}
#endif
       
    void threadedFunction() {    
        while(isThreadRunning()) {
            if(!q.empty()){
                QueuedImage i = q.front();
#ifdef OFX_TURBO_JPEG
				turbo.save(i.image, i.fileName, quality);
#else
				ofSaveImage(i.image, i.fileName, quality);
#endif
                q.pop();
            }
        }
        

        
    }   
    
    void addFrame(ofImage &img){
        addFrame(img.getPixelsRef());
    }
    
    void addFrame(ofVideoGrabber &cam){
        addFrame(cam.getPixelsRef());
    }
    
    void addFrame(ofVideoPlayer &player){
        addFrame(player.getPixelsRef());
    }
        
    void addFrame(ofPixels imageToSave) {  

        
        
        //char fileName[100]; 
        //snprintf(fileName,  "%s%.4i.%s" , prefix.c_str(), counter, format.c_str());     
        string fileName = prefix + ofToString(counter, numberWidth, '0') + "." + format;
        counter++;   
        
        QueuedImage qImage;
        
        qImage.fileName = fileName;    
        qImage.image = imageToSave; 
        
        q.push(qImage);
        
    }    
};  
