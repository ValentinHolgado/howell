#ifndef CLIENTE_H
#define CLIENTE_H

#include "ofMain.h"

class cliente
{
    public:
        cliente();
        virtual ~cliente();

        string sIp;
        string sName;
        int sId;
        string sTrackName;
        vector<string> assignedTracks;

        bool hasAssignedTrack();
        bool bTouch = false;
        bool bAudio = false;
        bool bButton = false;
        bool bFader = false;
        bool bAccel = false;
        bool bKeyboard = false;

        ofRectangle rcTouchSw {20,20,50,16};
        ofRectangle rcButtonSw {20,20,50,16};
        ofRectangle rcFaderSw {20,20,50,16};
        ofRectangle rcKeyboardSw {20,20,50,16};
        ofRectangle rcAccelSw {20,20,50,16};


    protected:
    private:
};

#endif // CLIENTE_H
