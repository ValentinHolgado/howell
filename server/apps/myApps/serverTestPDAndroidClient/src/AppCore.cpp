#include "AppCore.h"

void AppCore::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {

    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    server.setup(44500);
    ofSetWindowTitle("SERVIDOR");
    mutex = CreateMutexA( NULL, FALSE, NULL );
    font.loadFont("NewMediaFett.ttf",32);
    listFont.loadFont("NewMediaFett.ttf",12);

    // double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;

    if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) {
		OF_EXIT_APP(1);
	}

    pd.addReceiver(*this);
	pd.start();


	/*for(int i = 0; i < 10; i++)
    {
        Patch patch = pd.openPatch("pd/osc.pd");
        instances.push_back(patch);
        pd.sendFloat(instances[i].dollarZeroStr()+"-fromOF",ofRandom(100,300));
    }*/

    //setGUI();

	for(int i = 0; i < 5; i++)
    {
        //gui->addLabel(label);
    }
}
//--------------------------------------------------------------
void AppCore::update() {
    //Se comprueba que el servidor este funcionando antes de comenzar con las demas operaciones
    if(server.isConnected())
    {
        char buffer[BUFFER_SIZE];   //Buffer para almacenar los bytes de los mensajes entrantes
        for (int i = 0; i < server.getLastID();i++) //Se hace una pasada por todos los clientes
        {
            if(server.isClientConnected(i)) {   //Se comprueba que el cliente este efectivamente conectado
                if (server.receiveRawBytes(i,buffer,BUFFER_SIZE) > 0) //Si se recibio algo
                {
                    cout << "Mensaje recibido" << endl;

                    osc::ReceivedPacket p(buffer, BUFFER_SIZE);
                    osc::ReceivedMessage m(p);
                    ProcessMessage(m);
                    ofxOscMessage mosc;

                    if(hasWaitingMessages())
                    {
                        getNextMessage(&mosc);
                        if (mosc.getAddress() == "/touchscreen")
                        {
                            for(int i = 0; i < clientList.size();i++)
                            {
                                if (clientList[i].id == i)
                                {
                                    pd.startMessage();
                                    pd.addSymbol("x");
                                    pd.addFloat(mosc.getArgAsFloat(0));
                                    pd.finishList(clientList[i].patch.dollarZeroStr()+"-fromOF");

                                    pd.startMessage();
                                    pd.addSymbol("y");
                                    pd.addFloat(mosc.getArgAsFloat(1));
                                    pd.finishList(clientList[i].patch.dollarZeroStr()+"-fromOF");
                                }
                            }
                        }
                        else if (mosc.getAddress() == "/name")
                        {
                            client tempC;
                            tempC.sIp = server.getClientIP(i);
                            tempC.sName = mosc.getArgAsString(0);
                            tempC.id = i;
                            tempC.patch = pd.openPatch("pd/osc.pd");
                            clientList.push_back(tempC);

                            cout << "IP: " << server.getClientIP(i) << " ";
                            cout << "Name:" << mosc.getArgAsString(0) << " ";
                            cout << "ClientID: " << i << endl;

                            ofxOscMessage m;
                            m.setAddress("/toast");
                            m.addStringArg("Conectado al servidor.");
                            if(sendTCPMessage(m, clientList[i].id, &server))
                            {
                                cout << "Confirmacion enviada" << endl;
                            }
                        }
                        else if (mosc.getAddress() == "/accelerometer")
                        {
                            cout << "Accel.x: " << mosc.getArgAsFloat(0);
                            cout << " Accel.y: " << mosc.getArgAsFloat(1) << endl;
                        }
                        else
                        {
                            cout << "Direccion OSC no valida" << endl;
                        }
                    }
                }
            }
        }

        if(!clientList.empty())
        {
            for (unsigned int i = 0; i < clientList.size(); i++)
            {
                if(!server.isClientConnected(clientList[i].id))
                {
                    pd.closePatch(clientList[i].patch);
                    cout << "Cliente nr " << clientList[i].id << " de nombre " << clientList[i].sName << " se ha desconectado." << endl;
                    clientList.erase(clientList.begin()+i);
                }

            }
        }
    }
}
//--------------------------------------------------------------
void AppCore::draw() {
    ofBackground(254,229,173);
    ofSetColor(244,93,76);
    font.drawString("Servidor",20,font.getLineHeight());
    if (clientList.size() > 0)
    {
        //cout << clientList.size() << endl;
        for (unsigned int i = 0; i < clientList.size(); i++)
        {
            ofPushStyle();
            ofSetColor(244,93,76);
            string tempS = clientList[i].sIp + " " + clientList[i].sName + " " + ofToString(clientList[i].id);
            listFont.drawString(tempS,
                                20,
                                font.getLineHeight()+(listFont.getLineHeight()*(i+1)));

            //Botonera por cada cliente
            clientList[i].rcTouchSw.x = 20+listFont.stringWidth(tempS)+4;
            clientList[i].rcTouchSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcButtonSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+8;
            clientList[i].rcButtonSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcFaderSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+clientList[i].rcButtonSw.width+12;
            clientList[i].rcFaderSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcKeyboardSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+clientList[i].rcButtonSw.width+clientList[i].rcFaderSw.width+16;
            clientList[i].rcKeyboardSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcAccelSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+clientList[i].rcButtonSw.width+clientList[i].rcFaderSw.width+clientList[i].rcKeyboardSw.width+20;
            clientList[i].rcAccelSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            ofSetColor(250,202,102);
            ofRect(clientList[i].rcTouchSw);
            ofRect(clientList[i].rcButtonSw);
            ofRect(clientList[i].rcFaderSw);
            ofRect(clientList[i].rcKeyboardSw);
            ofRect(clientList[i].rcAccelSw);

            ofSetColor(247,165,65);
            listFont.drawString("TOUCH",clientList[i].rcTouchSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("BTN",clientList[i].rcButtonSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("FADER",clientList[i].rcFaderSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("KEY",clientList[i].rcKeyboardSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("ACCEL",clientList[i].rcAccelSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            ofPopStyle();
        }
    }
}
//--------------------------------------------------------------
void AppCore::exit() {
    //delete gui;
    server.close();
}
//--------------------------------------------------------------
void AppCore::keyPressed(int key) {
}
//--------------------------------------------------------------
void AppCore::mousePressed(int x, int y, int button){
    if (clientList.size() > 0)
    {
        //cout << clientList.size() << endl;
        for (unsigned int i = 0; i < clientList.size(); i++)
        {
            if(clientList[i].rcTouchSw.inside(x,y))
            {
                ofxOscMessage m;
                m.setAddress("/touchscreen");
                if(sendTCPMessage(m, clientList[i].id, &server))
                {
                    cout << "TOUCH al cliente " << clientList[i].sName << endl;
                }
            }
            if(clientList[i].rcButtonSw.inside(x,y))
            {
                ofxOscMessage m;
                m.setAddress("/button");
                if(sendTCPMessage(m, clientList[i].id, &server))
                {
                    cout << "BUTTON al cliente " << clientList[i].sName << endl;
                }
            }
            if(clientList[i].rcFaderSw.inside(x,y))
            {
                ofxOscMessage m;
                m.setAddress("/fader");
                if(sendTCPMessage(m, clientList[i].id, &server))
                {
                    cout << "FADER al cliente " << clientList[i].sName << endl;
                }
            }
            if(clientList[i].rcKeyboardSw.inside(x,y))
            {
                ofxOscMessage m;
                m.setAddress("/keyboard");
                if(sendTCPMessage(m, clientList[i].id, &server))
                {
                    cout << "KEYBOARD al cliente " << clientList[i].sName << endl;
                }
            }
            if(clientList[i].rcAccelSw.inside(x,y))
            {
                ofxOscMessage m;
                m.setAddress("/accelerometer");
                if(sendTCPMessage(m, clientList[i].id, &server))
                {
                    cout << "ACCELEROMETER al cliente " << clientList[i].sName << endl;
                }
            }
        }
    }
}
//--------------------------------------------------------------
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}
//--------------------------------------------------------------
void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}
//--------------------------------------------------------------
void AppCore::print(const std::string& message) {
    cout << message << endl;
}
//--------------------------------------------------------------
void AppCore::setGUI() {
    gui = new ofxUISuperCanvas("Control Panel");
    gui->addSpacer();
    gui->addLabel("Clients:");
    gui->removeWidgets();
}
//--------------------------------------------------------------
void AppCore::ProcessMessage( const osc::ReceivedMessage &m)
{	// convert the message to an ofxOscMessage
	ofxOscMessage* ofMessage = new ofxOscMessage();

	// set the address
	ofMessage->setAddress( m.AddressPattern() );


	// transfer the arguments
	for ( osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
		  arg != m.ArgumentsEnd();
		  ++arg )
	{
		if ( arg->IsInt32() )
			ofMessage->addIntArg( arg->AsInt32Unchecked() );
		else if ( arg->IsInt64() )
			ofMessage->addInt64Arg( arg->AsInt64Unchecked() );
		else if ( arg->IsFloat() )
			ofMessage->addFloatArg( arg->AsFloatUnchecked() );
		else if ( arg->IsString() )
			ofMessage->addStringArg( arg->AsStringUnchecked() );
		else
		{
			cout << "message argument is not int, float, or string" << endl;
		}
	}

	// now add to the queue

	// at this point we are running inside the thread created by startThread,
	// so anyone who calls hasWaitingMessages() or getNextMessage() is coming
	// from a different thread

	// so we have to practise shared memory management

	// grab a lock on the queue
	grabMutex();

	// add incoming message on to the queue
	messages.push_back( ofMessage );

	// release the lock
	releaseMutex();
}
//--------------------------------------------------------------
void AppCore::grabMutex()
{
#ifdef TARGET_WIN32
	WaitForSingleObject( mutex, INFINITE );
#else
	pthread_mutex_lock( &mutex );
#endif
}
//--------------------------------------------------------------
void AppCore::releaseMutex()
{
#ifdef TARGET_WIN32
	ReleaseMutex( mutex );
#else
	pthread_mutex_unlock( &mutex );
#endif
}
//--------------------------------------------------------------
bool AppCore::hasWaitingMessages()
{
	// grab a lock on the queue
	grabMutex();

	// check the length of the queue
	int queue_length = (int)messages.size();

	// release the lock
	releaseMutex();

	// return whether we have any messages
	return queue_length > 0;
}
//--------------------------------------------------------------
bool AppCore::getNextMessage( ofxOscMessage* message )
{
	// grab a lock on the queue
	grabMutex();

	// check if there are any to be got
	if ( messages.size() == 0 )
	{
		// no: release the mutex
		releaseMutex();
		return false;
	}

	// copy the message from the queue to message
	ofxOscMessage* src_message = messages.front();
	message->copy( *src_message );

	// now delete the src message
	delete src_message;
	// and remove it from the queue
	messages.pop_front();

	// release the lock on the queue
	releaseMutex();

	// return success
	return true;
}
//--------------------------------------------------------------
bool AppCore::sendTCPMessage( ofxOscMessage& message, int client, ofxTCPServer * server )
{
	static const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	// serialise the message
	//p << osc::BeginBundleImmediate;
	appendMessage( message, p );
	//p << osc::EndBundle;

    //server->sendRawBytes( client, p.Data(), p.Size() );
	return server->sendRawBytes( client, p.Data(), p.Size() );
}
//--------------------------------------------------------------
void AppCore::appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p )
{
    p << osc::BeginMessage( message.getAddress().c_str() );
	for ( int i=0; i< message.getNumArgs(); ++i )
	{
		if ( message.getArgType(i) == OFXOSC_TYPE_INT32 )
			p << message.getArgAsInt32( i );
		else if ( message.getArgType(i) == OFXOSC_TYPE_INT64 )
			p << (osc::int64)message.getArgAsInt64( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			p << message.getArgAsFloat( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING )
			p << message.getArgAsString( i ).c_str();
		else
		{
			ofLogError() << "bad argument type" + ofToString(message.getArgType( i ));
			assert( false );
		}
	}
	p << osc::EndMessage;
}

