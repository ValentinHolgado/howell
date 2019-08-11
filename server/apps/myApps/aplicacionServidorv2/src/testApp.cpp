#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    oscSender.setup(HOST, PORT);
    oscReceiver.setup(12346);
    timeline.setup();
    //timeline.reset();
   // timeline.setupPost();
    transportBGColor = ofColor(44,133,141);

    //Se cargan las fuentes para el texto de la interfaz grafica
    logoFont.loadFont("Neris-SemiBold.otf",32);
    transportFont.loadFont("UniversLTStd-Light.otf",18);

    /*---------------TIMELINE-----------------*/
    /*
    timeline.setup();
    mainOffset = ofVec2f(10,60); //Variable mainOffset para almacenar el vector de corrimiento de la ventana timeline

    timeline.setOffset(mainOffset); //Corre el timeline en el eje x y en el eje y

    ofBackground(timeline.getColors().guiBackgroundColor);
    transportBGColor = ofColor(44,133,141);

    //Se cargan las fuentes para el texto de la interfaz grafica
    logoFont.loadFont("Neris-SemiBold.otf",32);
    transportFont.loadFont("UniversLTStd-Light.otf",18);

    timeline.setSpacebarTogglePlay(false);   //La barra espaciadora funciona como play

    ofAddListener(timeline.events().bloqued, this, &testApp::receivedBloque);
    ofAddListener(timeline.events().removeBloqueTrackEvent, this, &testApp::removeBloqueTrack);
    ofAddListener(timeline.events().pageChanged, this, &testApp::timelinePageChanged);
    ofAddListener(timeline.events().playbackEnded, this, &testApp::nextPage);
    //cout << (timeline.getCurrentPage()->getTracks()[0])->getName() << endl; //ANDA

    cargarProyecto(timeline.getWorkingFolder());
    cout << ofToDataPath(timeline.getWorkingFolder(),true) << endl;

    //Se utiliza una lista aparte de la cantidad de tracks para no saturar de llamadas a la función timeline.getCurrentPage()->getTracks()

    if(timeline.getPages().size() > 0) {
      for(int i = 0; i < timeline.getPages().size(); i++) {
            vector<pista> _PistasListTemp;
                if(timeline.getPages().at(i)->getTracks().size() > 0) {
                   for (int j =0; j<timeline.getPages().at(i)->getTracks().size();j++) {
                        pista _pista;
                        _pista.trackName = timeline.getPages().at(i)->getTracks()[j]->getName();
                        _pista.clientsAssigned = 0;
                        //pistasList.push_back(_pista);
                        _PistasListTemp.push_back(_pista);
                   }
                   pageTracksList.push_back(_PistasListTemp);
            }
        }
    }
    */

    /*------FIN TIMELINE ------*/

    //const char * thePath = R"(E:\Facultad\Tesis\of_v0.8.0\apps\myApps\aplicacionServidorv2\bin\data)";
    setupTimeline("");

    //--- TEXT INPUT DURACIONES ---

    timeFont.loadFont("NexaRegular.otf",18);

    timelineHDuration.setup();
    timelineHDuration.setFont(timeFont);
    timelineMDuration.setup();
    timelineMDuration.setFont(timeFont);
    timelineSDuration.setup();
    timelineSDuration.setFont(timeFont);
    vector<string> tlTimecodeInit = ofSplitString(timeline.getCurrentPage()->getPageTimecode(),":");
    ofRect(9,ofGetHeight()-43,142,35);

    timelineHDuration.text  = tlTimecodeInit.at(0);
    timelineHDuration.bounds.x = 16;
    timelineHDuration.bounds.y = ofGetHeight()-70;
    timelineHDuration.bounds.height = 35;
    timelineHDuration.bounds.width = 40;
    HDurDisplay = timelineHDuration.bounds;

    timelineMDuration.text  = tlTimecodeInit.at(1);
    timelineMDuration.bounds.x = timelineHDuration.bounds.x + timelineHDuration.bounds.width + 5;
    timelineMDuration.bounds.y = timelineHDuration.bounds.y;
    timelineMDuration.bounds.height = timelineHDuration.bounds.height;
    timelineMDuration.bounds.width = timelineHDuration.bounds.width;
    MDurDisplay = timelineMDuration.bounds;

    timelineSDuration.text  = tlTimecodeInit.at(2);
    timelineSDuration.bounds.x = timelineMDuration.bounds.x + timelineMDuration.bounds.width + 5;
    timelineSDuration.bounds.y = timelineHDuration.bounds.y;
    timelineSDuration.bounds.height = timelineHDuration.bounds.height;
    timelineSDuration.bounds.width = timelineHDuration.bounds.width;
    SDurDisplay = timelineSDuration.bounds;

    ofAddListener(timelineHDuration.textChanged, this, &testApp::setCurPageHValue);
    ofAddListener(timelineMDuration.textChanged, this, &testApp::setCurPageMValue);
    ofAddListener(timelineSDuration.textChanged, this, &testApp::setCurPageSValue);

    //--- FIN TEXT INPUT DURACIONES ---

    playIdle.loadImage("iconos/playIdle.png");
    playHover.loadImage("iconos/playHover.png");
    playActive.loadImage("iconos/playActive.png");

    stopIdle.loadImage("iconos/stopIdle.png");
    stopHover.loadImage("iconos/stopHover.png");
    stopActive.loadImage("iconos/stopActive.png");

    pauseIdle.loadImage("iconos/pauseIdle.png");
    pauseHover.loadImage("iconos/pauseHover.png");
    pauseActive.loadImage("iconos/pauseActive.png");

    loopIdle.loadImage("iconos/loopIdle.png");
    loopHover.loadImage("iconos/loopHover.png");
    loopActive.loadImage("iconos/loopActive.png");

    newPageHover.loadImage("iconos/newPageActive.png");
    newPageIdle.loadImage("iconos/newPage.png");

    howellLogo.loadImage("iconos/Howell.png");

    /*
    if (timeline.getCurrentPage()->getTracks().size() > 0)
    {
       for (int i = 0; i < timeline.getCurrentPage()->getTracks().size(); i++) {
            pista _pista;
            _pista.trackName = timeline.getCurrentPage()->getTracks()[i]->getName();
            _pista.clientsAssigned = 0;
            pistasList.push_back(_pista);
        }
        cout << timeline.getCurrentPage()->getTracks().size() << endl;
    }
    */

    /*---------------SERVER--------------------*/

    server.setup(44500);
    ofSetWindowTitle("Howell");
    mutex = CreateMutexA( NULL, FALSE, NULL );
    font.loadFont("Neris-SemiBold.otf",32);
    listFont.loadFont("UniversLTStd-Light.otf",12);
    headerFont.loadFont("NexaRegular.otf",15);
    bodyFont.loadFont("NexaLight.otf",14);

    nuevoProyButton.height = 50;
    nuevoProyButton.width = 100;
    nuevoProyButton.y = 0;
    cargarButton.height = 50;
    cargarButton.width = 115;
    nuevoProyButton.y = 0;
}
//--------------------------------------------------------------
void testApp::update(){
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

                    while(hasWaitingMessages())
                    {
                        getNextMessage(&mosc);

                        if (mosc.getAddress() == "/name")
                        {
                            bool idAlreadyConnected = false;
                            for(int j = 0; j < clientList.size(); j++) {
                                if(clientList[j].sId == i) {
                                    idAlreadyConnected = true;
                                }
                                else{
                                    idAlreadyConnected = false;
                                }
                            }
                            if(!idAlreadyConnected)
                            {
                                bool nameTaken = false;
                                for(int k=0; k <clientList.size(); k++) {
                                    if (mosc.getArgAsString(0) == clientList[k].sName) {
                                        nameTaken = true;
                                        break;
                                    }
                                }

                                cliente tempC;
                                tempC.sIp = server.getClientIP(i);
                                if(nameTaken) {
                                    tempC.sName = mosc.getArgAsString(0)+ofToString(i);
                                }
                                else {
                                    tempC.sName = mosc.getArgAsString(0);
                                }
                                tempC.sId = i;

                                if (pageTracksList.size() > 0) {
                                    for (int k = 0; k < pageTracksList.size(); k++) {
                                        if (pageTracksList.at(k).size() > 0) {
                                            vector<int> cantClientes;
                                            for (int l = 0; l < pageTracksList.at(k).size(); l++) {
                                                cantClientes.push_back(pageTracksList.at(k).at(l).clientsAssigned);
                                            }
                                            for (int l = 0; l < pageTracksList.at(k).size(); l++) {

                                                if (pageTracksList.at(k).at(l).clientsAssigned == *min_element(cantClientes.begin(),cantClientes.end()))
                                                {
                                                    //tempC.sTrackName = pageTracksList.at(k).at(l).trackName;
                                                    tempC.assignedTracks.push_back(pageTracksList.at(k).at(l).trackName);
                                                    pageTracksList.at(k).at(l).clientsAssigned += 1;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }

                                //Chequea cual de las pistas tiene menos usuarios conectados y le asigna uno
                                /*
                                if (pistasList.size() > 0)
                                {
                                    vector<int> cantClientes;
                                    for (int k = 0; k < pistasList.size(); k++) {
                                        cantClientes.push_back(pistasList[k].clientsAssigned);
                                    }
                                    for (int k = 0; k < pistasList.size(); k++) {

                                        if (pistasList[k].clientsAssigned == *min_element(cantClientes.begin(),cantClientes.end()))
                                        {
                                        tempC.sTrackName = pistasList[k].trackName;
                                        pistasList[k].clientsAssigned += 1;
                                        break;
                                        }
                                    }
                                }

                                */

                                //Agrega el nombre del cliente al header de la pista asignada
                                ofxTLBloques * bloquePointer;
                                if(tempC.assignedTracks.size() > 0 ) {
                                    for (int k = 0; k < tempC.assignedTracks.size(); k++) {
                                        //bloquePointer = dynamic_cast<ofxTLBloques*>(timeline.getTrack(tempC.sTrackName));
                                        bloquePointer = dynamic_cast<ofxTLBloques*>(timeline.getTrack(tempC.assignedTracks.at(k)));
                                        bloquePointer->addToClientHeader(tempC.sName);
                                    }
                                }


                                clientList.push_back(tempC); //Agrega un cliente a la lista

                                cout << "IP: " << server.getClientIP(i) << " ";
                                cout << "Name:" << mosc.getArgAsString(0) << " ";
                                cout << "ClientID: " << i << endl;

                                ofxOscMessage m;
                                m.setAddress("/toast");
                                m.addStringArg("Conectado al servidor.");
                                if(sendTCPMessage(m, clientList[i].sId, &server))
                                {
                                    cout << "Confirmacion enviada" << endl;
                                     ofxOscMessage n;
                                        n.setAddress("/allsensorsoff");
                                        n.addIntArg(1);
                                        if(sendTCPMessage(n, clientList[i].sId, &server)){
                                            cout << "allsensorsoff" << endl;
                                        }
                                }
                            }
                        }
                        else if (mosc.getAddress() == "/touchscreen")
                        {
                            cout << "Touchscreen X: " << mosc.getArgAsFloat(0) << " Y: " << mosc.getArgAsFloat(1) << endl;
                            for(int j = 0; j < clientList.size();j++)
                            {
                                if (clientList[j].sId == i)
                                {
                                    //Envía al send que corresponde según la pista
                                    /*
                                    for(int k=0;k<pistasList.size();k++)
                                    {
                                        if(clientList[j].assignedTracks.size() > 0) {
                                                                                    cout << "Llegue hasta 2da base" << endl;

                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pistasList[k].trackName) {
                                                    cout << "Llegue hasta 3ra base" << endl;
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/touchscreen");
                                                    oscM.addFloatArg(mosc.getArgAsFloat(0));
                                                    oscM.addFloatArg(mosc.getArgAsFloat(1));
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }
                                        /*
                                        if(clientList[j].sTrackName == pistasList[k].trackName) {
                                            ofxOscMessage oscM;
                                            oscM.setAddress("/track/"+ofToString(k)+"/touchscreen");
                                            oscM.addFloatArg(mosc.getArgAsFloat(0));
                                            oscM.addFloatArg(mosc.getArgAsFloat(1));
                                            oscSender.sendMessage(oscM);
                                        }
                                    }
                                    */
                                    for (int k = 0; k < pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).size(); k++) {
                                         if(clientList[j].assignedTracks.size() > 0) {

                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).at(k).trackName) {
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/touchscreen");
                                                    oscM.addFloatArg(mosc.getArgAsFloat(0));
                                                    oscM.addFloatArg(mosc.getArgAsFloat(1));
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        else if (mosc.getAddress() == "/accelerometer")
                        {
                            cout << "Accel.x: " << mosc.getArgAsFloat(0);
                            cout << " Accel.y: " << mosc.getArgAsFloat(1) << endl;
                            for(int j = 0; j < clientList.size();j++)
                            {
                                if (clientList[j].sId == i)
                                {
                                    for (int k = 0; k < pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).size(); k++) {
                                         if(clientList[j].assignedTracks.size() > 0) {

                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).at(k).trackName) {
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/accelerometer");
                                                    oscM.addFloatArg(mosc.getArgAsFloat(0));
                                                    oscM.addFloatArg(mosc.getArgAsFloat(1));
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }
                                    }

                                    /*
                                    //Envía al send que corresponde según la pista
                                    for(int k=0;k<pistasList.size();k++)
                                    {
                                        if(clientList[j].assignedTracks.size() > 0) {
                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pistasList[k].trackName) {
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/accelerometer");
                                                    oscM.addFloatArg(mosc.getArgAsFloat(0));
                                                    oscM.addFloatArg(mosc.getArgAsFloat(1));
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }

                                    }
                                    */
                                }
                            }
                        }
                        else if (mosc.getAddress() == "/keyboard")
                        {
                            cout << "keyboard " << mosc.getArgAsString(0) << endl;;
                        }
                        else if (mosc.getAddress() == "/buttons")
                        {
                            cout << " buttons: " << mosc.getArgAsInt32(0) << endl;
                            for(int j = 0; j < clientList.size();j++)
                            {
                                if (clientList[j].sId == i)
                                {
                                    for (int k = 0; k < pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).size(); k++) {
                                         if(clientList[j].assignedTracks.size() > 0) {

                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).at(k).trackName) {
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/buttons");
                                                    oscM.addIntArg(mosc.getArgAsFloat(0));
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }
                                    }
                                    /*
                                    //Envía al send que corresponde según la pista
                                    for(int k=0;k<pistasList.size();k++)
                                    {
                                        if(clientList[j].assignedTracks.size() > 0) {
                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pistasList[k].trackName) {
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/buttons");
                                                    oscM.addIntArg(mosc.getArgAsInt32(0));
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }
                                        /*
                                        if(clientList[j].sTrackName == pistasList[k].trackName) {

                                        }
                                        */
                                    //}

                                }
                            }
                        }
                        else if (mosc.getAddress() == "/faders")
                        {
                            cout << "faders " << mosc.getArgAsInt32(0) << "  " <<  mosc.getArgAsFloat(1)<< endl;
                            for(int j = 0; j < clientList.size();j++)
                            {
                                if (clientList[j].sId == i)
                                {
                                    for (int k = 0; k < pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).size(); k++) {
                                         if(clientList[j].assignedTracks.size() > 0) {

                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pageTracksList.at(ofToInt(timeline.getCurrentPage()->getName())-1).at(k).trackName) {
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/faders/"+ofToString(mosc.getArgAsFloat(0)));
                                                    //oscM.addIntArg(mosc.getArgAsFloat(0));
                                                    float faderValue = ofMap(mosc.getArgAsFloat(1), 0, 1, 1, 0);
                                                    oscM.addFloatArg(faderValue);
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }
                                    }

                                    //Envía al send que corresponde según la pista

                                    /*for(int k=0;k<pistasList.size();k++)
                                    {
                                        if(clientList[j].assignedTracks.size() > 0) {
                                            for(int l = 0; l < clientList[j].assignedTracks.size(); l++) {
                                               if(clientList[j].assignedTracks.at(l) == pistasList[k].trackName) {
                                                    ofxOscMessage oscM;
                                                    oscM.setAddress("/track/"+ofToString(k)+"/fader");
                                                    oscM.addIntArg(mosc.getArgAsInt32(0));
                                                    oscM.addIntArg(mosc.getArgAsInt32(1));
                                                    oscSender.sendMessage(oscM);
                                                }
                                            }
                                        }

                                    }
                                    */
                                }
                            }
                        }
                        else if (mosc.getAddress() == "/page")
                        {
                            timeline.stop();
                            allSensorsOff(); //Enviar a todos los clientes que dejen de enviar datos de sensores
                            timeline.setCurrentPage(mosc.getArgAsInt32(0));
                            timeline.play();
                            cout << "Cambiando a pagina " << mosc.getArgAsInt32(0) << endl;
                        }
                        else if (mosc.getAddress() == "/loop")
                        {
                            if (mosc.getArgAsInt32(0) == 0)
                            {
                                timeline.setLoopType(OF_LOOP_NONE);
                            }
                            else
                            {
                                timeline.setLoopType(OF_LOOP_NORMAL);
                            }
                        }
                        else if (mosc.getAddress() == "/testing")
                        {
                            cout << "framecount " << mosc.getArgAsInt32(0) << endl;
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
                if(!server.isClientConnected(clientList[i].sId))
                {
                    //pd.closePatch(clientList[i].patch);
                    cout << "Cliente nr " << clientList[i].sId << " de nombre " << clientList[i].sName << " se ha desconectado." << endl;

                    for (int k = 0; k < clientList[i].assignedTracks.size(); k++) {
                        ofxTLBloques * bloquePointer;
                        bloquePointer = dynamic_cast<ofxTLBloques*>(timeline.getTrack(clientList[i].assignedTracks.at(k)));
                        bloquePointer->removeFromClientHeader(clientList[i].sName);

                        if (pistasList.size() > 0) {
                            for (int j=0; j < pistasList.size(); j++) {
                                if ( pistasList[j].trackName == clientList[i].assignedTracks.at(k) ) {
                                    pistasList[j].clientsAssigned -= 1;
                                    break;
                                }
                            }
                        }
                    }
                    clientList.erase(clientList.begin()+i);
                    /*
                    ofxTLBloques * bloquePointer;
                    bloquePointer = dynamic_cast<ofxTLBloques*>(timeline.getTrack(clientList[i].sTrackName));
                    bloquePointer->removeFromClientHeader(clientList[i].sName);
                    for (int j=0; j < pistasList.size(); j++) {
                        if ( pistasList[j].trackName == clientList[i].sTrackName ) {
                            pistasList[j].clientsAssigned -= 1;
                            break;
                        }
                    }
                    clientList.erase(clientList.begin()+i);
                    */
                }
            }
        }

        if (ofGetFrameNum() % 10 == 0) {
            ofxOscMessage infoMess;
            infoMess.setAddress("/isplaying");
            if(timeline.getIsPlaying()){
                infoMess.addIntArg(1);
            } else {
                infoMess.addIntArg(0);
            }
            oscSender.sendMessage(infoMess);
        }


        while (oscReceiver.hasWaitingMessages()){
            ofxOscMessage recMess;
            oscReceiver.getNextMessage(&recMess);

            if (recMess.getAddress() == "/page") {

                if ( timeline.hasPage(ofToString(recMess.getArgAsInt32(0)))) {
                    timeline.stop();
                    allSensorsOff(); //Enviar a todos los clientes que dejen de enviar datos de sensores
                    timeline.setCurrentPage(recMess.getArgAsInt32(0));
                    timeline.setCurrentTimeMillis(0);
                    timeline.play();
                    cout << "Cambiando a pagina " << recMess.getArgAsInt32(0) << endl;
                }
            }

            if (recMess.getAddress() == "/stop") {
                if (recMess.getArgAsInt32(0) == 1) {
                    timeline.stop();
                    timeline.setPercentComplete(0);
                    allSensorsOff();
                }
            }

            if (recMess.getAddress() == "/play") {
                if (recMess.getArgAsInt32(0) == 1) {
                    timeline.play();
                }
            }

            if (recMess.getAddress() == "/loop") {
                if (recMess.getArgAsInt32(0) == 1) {
                    timeline.setLoopType(OF_LOOP_NORMAL);
                }
                else if (recMess.getArgAsInt32(0) == 0) {
                    timeline.setLoopType(OF_LOOP_NONE);
                }
            }
        }
    }
    else {
        timeline.stop();
        timeline.setPercentComplete(0);
    }
}
//--------------------------------------------------------------
void testApp::draw(){

    /*--- INICIO LOGO Y MANEJO DE ARCHIVOS ------------------------*/
    //Dibujar barra de herramientas y logo superior
    ofSetColor(timeline.getColors().keyColor);
    ofRect(0,0,ofGetWidth(),50);

    ofSetColor(255);
    howellLogo.draw(0,0);
    //ofRectangle logo = headerFont.getStringBoundingBox("SERVIDOR",10,30);
    //ofRect(logo);
    //ofSetColor(255);
    //headerFont.drawString("Logo",10,30);
    //logo.x += 30; //Separacion entre logo y transporte

    //Barra de transporte, cambia el color si el mouse esta sobre el texto
    ofSetColor(0,0);
    if(hoverNuevoProyButton) ofSetColor(timeline.getColors().bColor);
    nuevoProyButton.x = ofGetWidth()-nuevoProyButton.width;
    ofRect(nuevoProyButton);
    ofSetColor(255);
    headerFont.drawString("NUEVO",nuevoProyButton.x+14,31);

    ofSetColor(0,0);
    if(hoverCargarButton) ofSetColor(timeline.getColors().bColor);
    cargarButton.x = ofGetWidth()-nuevoProyButton.width-cargarButton.width;
    ofRect(cargarButton);
    ofSetColor(255);
    headerFont.drawString("CARGAR",cargarButton.x+14,31);

    /*
    if(hoverPlayButton || timeline.getIsPlaying()){
        transportTextColor = ofColor(0,255,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    playButton = dibujarBotonTransporte("play",logo,playButton,transportTextColor);

    if(hoverPauseButton){
        transportTextColor = ofColor(220,220,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    pauseButton = dibujarBotonTransporte("pause",playButton,pauseButton,transportTextColor);

    if(hoverStopButton || !timeline.getIsPlaying()){
        transportTextColor = ofColor(220,0,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    stopButton = dibujarBotonTransporte("stop",pauseButton,stopButton,transportTextColor);

    if(hoverLoopButton || timeline.getLoopType()== OF_LOOP_NORMAL){
        transportTextColor = ofColor(0,220,150);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    loopButton = dibujarBotonTransporte("loop",stopButton,loopButton,transportTextColor);

    if(!server.isConnected()){
        transportTextColor = ofColor(255,0,0);
    }
    else {
        transportTextColor = ofColor(0,255,0);
    }
    serverSwitch = dibujarBotonTransporte("status",loopButton,serverSwitch,transportTextColor);

    transportTextColor = ofColor(timeline.getColors().textColor);

    addTrackRect = dibujarBotonTransporte("+",serverSwitch,addTrackRect,transportTextColor);

    cargarButton = dibujarBotonTransporte("cargar",addTrackRect,cargarButton,transportTextColor);

    guardarButton = dibujarBotonTransporte("guardar",cargarButton,guardarButton,transportTextColor);

    nuevoProyButton = dibujarBotonTransporte("nuevo",guardarButton,nuevoProyButton,transportTextColor);

    nuevaPaginaButton = dibujarBotonTransporte("nuevapag",nuevoProyButton,nuevaPaginaButton,transportTextColor);

    */

    /*--- FIN LOGO Y MANEJO de ARCHIVO ---*/

    /*--LÍNEA DE TIEMPO--*/
    timeline.draw();
    /*--FIN LÍNEA DE TIEMPO--*/

    ofPushStyle();

    ofSetColor(timeline.getColors().lightBackground);
    ofRect(0,ofGetHeight()-50,ofGetWidth(),50);

    ofSetColor(timeline.getColors().keyColor);
    ofRect(9,ofGetHeight()-43,142,35);

    ofRect(ofGetWidth()-50,ofGetHeight()-50,50,50);

    ofSetColor(255);
    timelineHDuration.draw();
    timelineMDuration.draw();
    timelineSDuration.draw();
    timeFont.drawString(":",55,ofGetHeight()-20);
    timeFont.drawString(":",98,ofGetHeight()-20);

    //ofSetColor(200,0,0,120);
    //ofRect(HDurDisplay);
    //ofRect(MDurDisplay);
    //ofRect(SDurDisplay);

    ofSetColor(timeline.getColors().lightKeyColor);
    bodyFont.drawString("Servidor",ofGetWidth()-134,ofGetHeight()-19);

    ofSetLineWidth(1);
    ofLine(ofGetWidth()-145,ofGetHeight()-38, ofGetWidth()-145, ofGetHeight()-12);

    if(hoverServerSwitchButton) {
        ofSetColor(timeline.getColors().lightKeyColor);
    } else if ( server.isConnected() ) {
        ofSetColor(0,200,0);
    } else {
        ofSetColor(timeline.getColors().bColor);
    }

    ofRect(ofGetWidth()-29, ofGetHeight()-42,8,22);
    ofRect(ofGetWidth()-29, ofGetHeight()-16,8,8);
    ofPopStyle();

    if(hoverPlayButton && !timeline.getIsPlaying()) {
        playHover.draw(ofGetWidth()/2-88,ofGetHeight()-47);
    } else if (timeline.getIsPlaying()){
        playActive.draw(ofGetWidth()/2-88,ofGetHeight()-47);
    } else {
        playIdle.draw(ofGetWidth()/2-88,ofGetHeight()-47);
    }

    if(hoverPauseButton) {
        pauseHover.draw(ofGetWidth()/2-44,ofGetHeight()-47);
    } else if (!timeline.getIsPlaying() && !timeline.getCurrentTimeMillis() == 0){
        pauseActive.draw(ofGetWidth()/2-44,ofGetHeight()-47);
    } else {
        pauseIdle.draw(ofGetWidth()/2-44,ofGetHeight()-47);
    }

    if(hoverStopButton && timeline.getIsPlaying() ) {
        stopHover.draw(ofGetWidth()/2,ofGetHeight()-47);
    } else if (!timeline.getIsPlaying() && timeline.getCurrentTimeMillis() == 0){
        stopActive.draw(ofGetWidth()/2,ofGetHeight()-47);
    } else {
        stopIdle.draw(ofGetWidth()/2,ofGetHeight()-47);
    }

    if(hoverLoopButton && timeline.getLoopType() != OF_LOOP_NORMAL ) {
        loopHover.draw(ofGetWidth()/2+44,ofGetHeight()-47);
    } else if (timeline.getLoopType()== OF_LOOP_NORMAL){
        loopActive.draw(ofGetWidth()/2+44,ofGetHeight()-47);
    } else {
        loopIdle.draw(ofGetWidth()/2+44,ofGetHeight()-47);
    }

    if(hoverNuevaPagButton) {
        newPageHover.draw(ofGetWidth()-25,60);
    } else {
        newPageIdle.draw(ofGetWidth()-25,60);
    }

    ofSetColor(timeline.getColors().lightBackground);
    ofRect(addTrackRect);
    if(hoverAddTrackButton) {
        ofSetColor(timeline.getColors().bColor);
    } else {
        ofSetColor(timeline.getColors().keyColor);
    }
    ofRect(addTrackRect.x+21, addTrackRect.y+8,8,34);
    ofRect(addTrackRect.x+8, addTrackRect.y+21,34,8);


    /*--INICIO MONITOR DE CLIENTES--*/
    /*
    if (clientList.size() > 0)
    {
        //cout << clientList.size() << endl;
        for (unsigned int i = 0; i < clientList.size(); i++)
        {
            ofPushStyle();
            ofSetColor(244,93,76);
            string tempS = clientList[i].sIp + " " + clientList[i].sName + " " + ofToString(clientList[i].sId);
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
    */
    /*--FIN MONITOR DE CLIENTES--*/
}
//-------------------------------------------------------------
void testApp::exit(){
    server.close();
    timeline.reset();
    ofEventArgs args;
    timeline.exit(args);
    releaseMutex();
}
//--------------------------------------------------------------
void testApp::cargarProyecto(string projectFolder){
    ofDirectory dir(projectFolder);
    dir.allowExt("xml");
    dir.listDir();
    timeline.setWorkingFolder(projectFolder);
    timeline.reset();
    timeline.setupPost();
    timeline.getPages().clear();
    timeline.enable();
    for(int i = 0; i < dir.numFiles(); i++){
        if (ofIsStringInString(dir.getPath(i), "_trackPositions.xml") && ofIsStringInString(dir.getPath(i), "timeline0_"))
        {
            string pageName = ofSplitString(dir.getPath(i),"_trackPositions.xml").at(0);
            pageName = ofSplitString(pageName,"timeline0_").at(1);
            ofStringReplace(pageName,"_"," ");

            //ofStringReplace(xmlPageName," ", "_");
            //string positionFileName = ofToDataPath(timeline->getWorkingFolder() + timeline->getName() + "_" + xmlPageName + "_trackPositions.xml");

            //if(pageName == "1"){
            //    timeline.setCurrentPage(0);
            //}
            //else {
                timeline.addPage(pageName,true);
            //}
            timeline.getCurrentPage()->clear();

            //cout << timeline.getCurrentPage()->getName();

            ofxXmlSettings trackPositions;
            if(trackPositions.loadFile(dir.getPath(i))){
                //cout << "antes" << timeline.getCurrentPage()->getTracks().size() << endl;
                //cout << dir.getPath(i) << endl;
                trackPositions.pushTag("positions");
                int numtracks = trackPositions.getNumTags("element");
                for(int i = 0; i < numtracks; i++){
                    string name = trackPositions.getAttribute("element", "name", "", i);
                    timeline.addBloques(name);
                }
                trackPositions.popTag();
                //cout << "despues" << timeline.getCurrentPage()->getTracks().size() << endl;
            }
        }
    }
}
//--------------------------------------------------------------
void testApp::receivedBloque(ofxTLBloqueEventArgs& bloque){
    for (int cantTracks = 0; cantTracks < timeline.getCurrentPage()->getTracks().size(); cantTracks++) {
        if(timeline.getCurrentPage()->getTracks()[cantTracks]->getName() == bloque.track->getName()) {
            if(clientList.size() > 0)
                {
                   for(int i = 0; i < clientList.size(); i++) {
                        for (int aTracks = 0; aTracks < clientList[i].assignedTracks.size(); aTracks++) {
                            if(clientList[i].assignedTracks.at(aTracks) == bloque.track->getName()){
                                ofxOscMessage m;
                                ofxOscMessage n;
                                ofxOscMessage infx;
                                ofxOscMessage infy;
                                ofxOscMessage inf1;
                                ofxOscMessage inf2;
                                ofxOscMessage inf3;
                                ofxOscMessage inf4;

                                n.setAddress("/info");
                                infx.setAddress("/infox");
                                infy.setAddress("/infoy");
                                inf1.setAddress("/info1");
                                inf2.setAddress("/info2");
                                inf3.setAddress("/info3");
                                inf4.setAddress("/info4");


                                n.addStringArg(bloque.bloqueName);

                                if(bloque.sensorArg == OFXTLBLOQUE_TOUCHSCREEN) {
                                    m.setAddress("/touchscreen");
                                }
                                else if (bloque.sensorArg == OFXTLBLOQUE_ACCELEROMETER) {
                                    m.setAddress("/accelerometer");
                                }
                                else if (bloque.sensorArg == OFXTLBLOQUE_KEYBOARDINPUT) {
                                    m.setAddress("/keyboard");
                                }
                                else if (bloque.sensorArg == OFXTLBLOQUE_BUTTONS) {
                                    m.setAddress("/buttons");
                                }
                                else if (bloque.sensorArg == OFXTLBLOQUE_FADERS) {
                                    m.setAddress("/faders");
                                }



                                if(bloque.on){
                                    m.addIntArg(1);
                                }
                                else {
                                    m.addIntArg(0);
                                }

                                if (bloque.sensorArg == OFXTLBLOQUE_BUTTONS) {
                                    m.addIntArg(bloque.BFadersNumArg);
                                    m.addStringArg(bloque.infoButFadersArg[0]);
                                    m.addStringArg(bloque.infoButFadersArg[1]);
                                    m.addStringArg(bloque.infoButFadersArg[2]);
                                    m.addStringArg(bloque.infoButFadersArg[3]);
                                }
                                else if(bloque.sensorArg == OFXTLBLOQUE_TOUCHSCREEN) {
                                    m.addStringArg(bloque.infoXArg);
                                    m.addStringArg(bloque.infoYArg);

                                }
                                else if (bloque.sensorArg == OFXTLBLOQUE_ACCELEROMETER) {
                                    m.addStringArg(bloque.infoXArg);
                                    m.addStringArg(bloque.infoYArg);
                                }
                                else if (bloque.sensorArg == OFXTLBLOQUE_FADERS) {
                                    m.addIntArg(bloque.BFadersNumArg);
                                    m.addStringArg(bloque.infoButFadersArg[0]);
                                    m.addStringArg(bloque.infoButFadersArg[1]);
                                    m.addStringArg(bloque.infoButFadersArg[2]);
                                    m.addStringArg(bloque.infoButFadersArg[3]);
                                }
                                m.addStringArg(bloque.bloqueName);
                                /*
                                sendTCPMessage(inf1, clientList[i].sId, &server);
                                sendTCPMessage(inf2, clientList[i].sId, &server);
                                sendTCPMessage(inf3, clientList[i].sId, &server);
                                sendTCPMessage(inf4, clientList[i].sId, &server);
                                */
                                if(sendTCPMessage(m, clientList[i].sId, &server))
                                {
                                    //if(sendTCPMessage(n, clientList[i].sId, &server)){
                                        cout << m.getAddress() << " " << m.getArgAsInt32(0) << " al cliente " << clientList[i].sName << endl;
                                        /*if(sendTCPMessage(infy, clientList[i].sId, &server)) {
                                            if(sendTCPMessage(infx, clientList[i].sId, &server)) {
                                                if(sendTCPMessage(inf1, clientList[i].sId, &server)) {
                                                    if(sendTCPMessage(inf2, clientList[i].sId, &server)) {
                                                        if(sendTCPMessage(inf3, clientList[i].sId, &server)) {
                                                            if(sendTCPMessage(inf4, clientList[i].sId, &server)) {
                                                                cout << "Todos los mensajes enviados." << endl;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }*/
                                    //}
                                }
                            }
                        }
                    }
                }
        } else {
        //Se puede notificar si se recibe un track de otra pagina
        }
    }

}
//--------------------------------------------------------------
void testApp::removeBloqueTrack(ofxTLBloqueTrackRemoveEventArgs& args) {
    if(!server.isConnected()){
        timeline.removeTrack(args.track);
        for (int i=0; i<pistasList.size(); i++) {
            if(pistasList[i].trackName == args.track->getName()) {
                pistasList.erase(pistasList.begin()+i);
                break;
            }
        }
    }
}
//--------------------------------------------------------------
ofRectangle testApp::dibujarBotonTransporte(string name, ofRectangle anterior, ofRectangle actual, ofColor colorTexto) {
    ofSetColor(transportBGColor);
    actual = transportFont.getStringBoundingBox(name,anterior.getX() + anterior.getWidth()+20, 20+transportFont.stringHeight(name)/2);
    ofRect(actual);

    ofSetColor(colorTexto);
    transportFont.drawString(name,actual.getX(), 20 + transportFont.stringHeight(name)/2);

    return actual;
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' ' && !timeline.isModal())
    {
       if (timeline.getIsPlaying()) {
            timeline.stop();
            allSensorsOff();
        }
        else {
            timeline.play();
        }
    }
}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //Comprobar que el mouse este sobre los iconos de la barra de transporte

    hoverPlayButton = playButton.inside(x,y);
    hoverPauseButton = pauseButton.inside(x,y);
    hoverStopButton = stopButton.inside(x,y);
    hoverLoopButton = loopButton.inside(x,y);
    hoverAddTrackButton = addTrackRect.inside(x,y);
    hoverServerSwitchButton = serverSwitch.inside(x,y);
    hoverCargarButton = cargarButton.inside(x,y);
    hoverGuardarButton = guardarButton.inside(x,y);
    hoverNuevoProyButton = nuevoProyButton.inside(x,y);
    hoverNuevaPagButton = nuevaPaginaButton.inside(x,y);
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(hoverPlayButton && server.isConnected()) {
        timeline.play();
    }
    if(hoverPauseButton) {
        timeline.stop();

        allSensorsOff();
    }
    if(hoverStopButton) {
        timeline.stop();
        timeline.setPercentComplete(0);

        allSensorsOff();
    }
    if(hoverLoopButton) {
            if(timeline.getLoopType() == OF_LOOP_NONE) {
                timeline.setLoopType(OF_LOOP_NORMAL);
            }
            else {
                timeline.setLoopType(OF_LOOP_NONE);
            }
    }
    if(hoverServerSwitchButton) {
        if(server.isConnected()) {
            server.close();
        }
        else {
            server.setup(44500);
        }
    }
    if(hoverAddTrackButton && !server.isConnected()) {
        timeline.addBloques(""+ofToString(pistasList.size()+1));
        pista _pista;
        _pista.trackName = timeline.getCurrentPage()->getTracks().at(timeline.getCurrentPage()->getTracks().size()-1)->getName();
        _pista.clientsAssigned = 0;
        pistasList.push_back(_pista);
    }

    if(hoverCargarButton && !server.isConnected()) {
        ofFileDialogResult thepath;
        thepath = ofSystemLoadDialog("Elegir carpeta de proyecto",true);
        setupTimeline(thepath.filePath);
    }

    if(hoverGuardarButton) {

    }

    if(hoverNuevaPagButton && !server.isConnected()){
        timeline.addPage(ofToString(timeline.getPages().size()+1));
    }
}
//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}
//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    timelineHDuration.bounds.y = h-45;
    timelineMDuration.bounds.y = timelineHDuration.bounds.y;
    timelineSDuration.bounds.y = timelineHDuration.bounds.y;

    HDurDisplay = timelineHDuration.bounds;
    MDurDisplay = timelineMDuration.bounds;
    SDurDisplay = timelineSDuration.bounds;

    playButton.setPosition(ofGetWidth()/2-88,ofGetHeight()-47);
    playButton.setWidth(44);
    playButton.setHeight(44);

    pauseButton.setPosition(ofGetWidth()/2-44,ofGetHeight()-47);
    pauseButton.setWidth(44);
    pauseButton.setHeight(44);

    stopButton.setPosition(ofGetWidth()/2,ofGetHeight()-47);
    stopButton.setWidth(44);
    stopButton.setHeight(44);

    loopButton.setPosition(ofGetWidth()/2+44,ofGetHeight()-47);
    loopButton.setWidth(44);
    loopButton.setHeight(44);

    serverSwitch.setPosition(ofGetWidth()-50,ofGetHeight()-50);
    serverSwitch.setWidth(50);
    serverSwitch.setHeight(50);

    addTrackRect.setPosition(ofGetWidth()/2-25,ofGetHeight()-110);
    addTrackRect.setWidth(50);
    addTrackRect.setHeight(50);

    nuevaPaginaButton.setPosition(ofGetWidth()-25,60);
    nuevaPaginaButton.setWidth(20);
    nuevaPaginaButton.setHeight(20);
}
//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}
//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
//--------------------------------------------------------------
void testApp::setCurPageHValue(string& value) {
    string tlTimecode;
    tlTimecode = value + ":" + timelineMDuration.text + ":" + timelineSDuration.text + ":0";
    timeline.getCurrentPage()->setPageTimecode(tlTimecode);
    timeline.setCurrentTimeSeconds(0);
    timeline.setDurationInTimecode(tlTimecode);
    timeline.getCurrentPage()->update();
}
//--------------------------------------------------------------
void testApp::setCurPageMValue(string& value) {
    string tlTimecode;
    tlTimecode = timelineHDuration.text + ":" + value + ":" + timelineSDuration.text + ":0";
    timeline.getCurrentPage()->setPageTimecode(tlTimecode);
    timeline.setCurrentTimeSeconds(0);
    timeline.setDurationInTimecode(tlTimecode);
    timeline.getCurrentPage()->update();
}
//--------------------------------------------------------------
void testApp::setCurPageSValue(string& value) {
    string tlTimecode;
    tlTimecode = timelineHDuration.text + ":" + timelineMDuration.text + ":" + value + ":0";
    timeline.getCurrentPage()->setPageTimecode(tlTimecode);
    timeline.setCurrentTimeSeconds(0);
    timeline.setDurationInTimecode(tlTimecode);
    timeline.getCurrentPage()->update();
}
//--------------------------------------------------------------
void testApp::timelinePageChanged(ofxTLPageEventArgs& argsx){
    string pTimecode = timeline.getCurrentPage()->getPageTimecode();
    vector<string> hmsml = ofSplitString(pTimecode,":");
    timelineHDuration.text = hmsml.at(0);
    timelineMDuration.text = hmsml.at(1);
    timelineSDuration.text = hmsml.at(2);

    ofxOscMessage oscM;
    oscM.setAddress("/page");
    oscM.addIntArg(ofToInt(timeline.getCurrentPage()->getName()));
    oscSender.sendMessage(oscM);
}
//--------------------------------------------------------------
void testApp::nextPage(ofxTLPlaybackEventArgs& args){
    if (!timeline.getIsPlaying() && timeline.getLoopType() == OF_LOOP_NONE) {
            if(args.currentPercent == 1) {
               string pagNueva;
                pagNueva =  ofToString(ofToInt(timeline.getCurrentPage()->getName())+1);
                if ( timeline.hasPage(pagNueva) ) {
                    timeline.stop();
                    allSensorsOff(); //Enviar a todos los clientes que dejen de enviar datos de sensores
                    timeline.setCurrentPage(pagNueva);
                    timeline.setCurrentTimeMillis(0);
                    timeline.play();


                    cout << "Cambiando a pagina " << pagNueva << endl;
                }
            }
    }
}
//--------------------------------------------------------------
void testApp::ProcessMessage( const osc::ReceivedMessage &m)
{
	// convert the message to an ofxOscMessage
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
void testApp::grabMutex()
{
#ifdef TARGET_WIN32
	WaitForSingleObject( mutex, INFINITE );
#else
	pthread_mutex_lock( &mutex );
#endif
}
//--------------------------------------------------------------
void testApp::releaseMutex()
{
#ifdef TARGET_WIN32
	ReleaseMutex( mutex );
#else
	pthread_mutex_unlock( &mutex );
#endif
}
//--------------------------------------------------------------
bool testApp::hasWaitingMessages()
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
bool testApp::getNextMessage( ofxOscMessage* message )
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
bool testApp::sendTCPMessage( ofxOscMessage& message, int client, ofxTCPServer * server )
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
void testApp::appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p )
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
//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}
//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}
//--------------------------------------------------------------
void testApp::print(const std::string& message) {
    cout << message << endl;
}
//--------------------------------------------------------------
void testApp::enlazarCliente(string _cName, int _cId, string _cIP) {
    cliente tempC;
    tempC.sName = _cName;
    tempC.sId = _cId;
    tempC.sIp = _cIP;
    clientList.push_back(tempC);
}
//--------------------------------------------------------------
void testApp::desenlazarCliente(int _cId) {
    if(!clientList.empty())
        {
            for (unsigned int i = 0; i < clientList.size(); i++)
            {
                if(clientList[i].sId == _cId)
                {
                    cout << "Cliente nr " << clientList[i].sId << " de nombre " << clientList[i].sName << " se ha desenlazado de la pista." << endl;
                    clientList.erase(clientList.begin()+i);
                }

            }
        }
}
//--------------------------------------------------------------
bool testApp::tieneCliente() {
    if (clientList.size() > 0) {
        return true;
    }
    else {
        return false;
    }
}
//--------------------------------------------------------------
void testApp::allSensorsOff() {
    ofxOscMessage m;
    m.setAddress("/allsensorsoff");
    m.addIntArg(1);
    for(int i=0; i<clientList.size();i++)
    {
        sendTCPMessage(m, clientList[i].sId, &server);
    }
}
//--------------------------------------------------------------
void testApp::setupTimeline(string folderPath) {
    pistasList.clear();
    clientList.clear();
    pageTracksList.clear();
    mainOffset = ofVec2f(10,60); //Variable mainOffset para almacenar el vector de corrimiento de la ventana timeline

    timeline.setOffset(mainOffset); //Corre el timeline en el eje x y en el eje y

    ofBackground(timeline.getColors().guiBackgroundColor);

    //cout << timeline.getWorkingFolder() << endl;
    if (folderPath == "") {
        cargarProyecto(timeline.getWorkingFolder());
    } else {
        //timeline.clear();
        //timeline.reset();
        //timeline.setupPost();
        //timeline.setupPost();
        //timeline.clear();
        //timeline.getPages().clear();
        //timeline.removePage("1");
        cargarProyecto(folderPath);
    }

    timeline.setSpacebarTogglePlay(false);   //La barra espaciadora funciona como play

    ofAddListener(timeline.events().bloqued, this, &testApp::receivedBloque);
    ofAddListener(timeline.events().removeBloqueTrackEvent, this, &testApp::removeBloqueTrack);
    ofAddListener(timeline.events().pageChanged, this, &testApp::timelinePageChanged);
    ofAddListener(timeline.events().playbackEnded, this, &testApp::nextPage);

    //cout << folderPath << endl;
    //cout << ofToDataPath(timeline.getWorkingFolder(),true) << endl;

    //Se utiliza una lista aparte de la cantidad de tracks para no saturar de llamadas a la función timeline.getCurrentPage()->getTracks()

    if(timeline.getPages().size() > 0) {
        for(int i = 0; i < timeline.getPages().size(); i++) {
            vector<pista> _PistasListTemp;
                if(timeline.getPages().at(i)->getTracks().size() > 0) {
                   for (int j =0; j<timeline.getPages().at(i)->getTracks().size();j++) {
                        pista _pista;
                        _pista.trackName = timeline.getPages().at(i)->getTracks()[j]->getName();
                        _pista.clientsAssigned = 0;
                        //pistasList.push_back(_pista);
                        _PistasListTemp.push_back(_pista);
                   }
                   pageTracksList.push_back(_PistasListTemp);
                }
        }
    }
}
