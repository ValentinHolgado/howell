#pragma once

#include "ofMain.h"

#include "ofxPd.h"

// a namespace for the Pd types
using namespace pd;

class AppCore : public PdReceiver {

	public:

		// main
		void setup(const int numOutChannels, const int numInChannels,
				   const int sampleRate, const int ticksPerBuffer);
		void update();
		void draw();
        void exit();

		// input callbacks
		void keyPressed(int key);

		// audio callbacks
		void audioReceived(float * input, int bufferSize, int nChannels);
		void audioRequested(float * output, int bufferSize, int nChannels);

		// pd message receiver callbacks
		void print(const std::string& message);

		ofxPd pd;	//< pd instance
		Patch patch;

        bool bTouch = false;
		float x = 0;
		float y = 0;

};
