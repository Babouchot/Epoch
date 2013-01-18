
// Data will be saved to "EmoStateLogger.csv" file

//#include <QtCore/QCoreApplication>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include<stdlib.h>

#include "edk.h"
#include "edkErrorCode.h"
#include "EmoStateDLL.h"

void logEmoState(std::ostream& os, unsigned int userID, EmoStateHandle eState, bool withHeader = false);
int kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);

    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
}

int main(int argc, char** argv) {

    EmoEngineEventHandle eEvent			= EE_EmoEngineEventCreate();
    EmoStateHandle eState				= EE_EmoStateCreate();
    unsigned int userID					= 0;
    const unsigned short composerPort	= 1726;
    int option = 0;
    int state  = 0;
    std::string input;

    std::cout << "===================================================================" << std::endl;
    std::cout << "Example to show how to log the EmoState from EmoEngine/EmoComposer." << std::endl;
    std::cout << "===================================================================" << std::endl;
    std::cout << "Press '1' to start and connect to the EmoEngine                    " << std::endl;
    std::cout << "Press '2' to connect to the EmoComposer                            " << std::endl;
    std::cout << ">> ";

    std::getline(std::cin, input, '\n');
    option = atoi(input.c_str());

    bool connected = false;
    switch(option)
    {
       case 1:
       {
        if(EE_EngineConnect() == EDK_OK)
        {
            connected = true;
            std::cout << "Start receiving EmoState ! Press any key to stop logging...\n" << std::endl;
        }
        else
        {
            connected = false;
            std::cout <<"EmoEngine start up failed" << std::endl;
        }
        break;
       }
       case 2:
       {
        if(EE_EngineRemoteConnect("127.0.0.1",1726)==EDK_OK)
        {
            connected = true;
            std::cout << "Start receiving EmoState! Press any key to stop logging...\n" << std::endl;
        }
        else
        {
            connected = false;
            std::cout <<"EmoComposer start up failed" << std::endl;
        }
        break;
       }
       default:
       {
         std::cout <<"Invalid option" << std::endl;
         break;
       }

    }

    std::ofstream ofs("Logger.csv",std::ios::trunc);
    bool writeHeader = true;

    while (connected && !kbhit())
    {
        state = EE_EngineGetNextEvent(eEvent);
       // std::cout << "state : " << state << std::endl;
        // New event needs to be handled
        if (state == EDK_OK)
        {

             EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
             EE_EmoEngineEventGetUserId(eEvent, &userID);

             // Log the EmoState if it has been updated
             if (eventType == EE_EmoStateUpdated)
             {                 
                  EE_EmoEngineEventGetEmoState(eEvent, eState);
                  const float timestamp = ES_GetTimeFromStart(eState);

                  std::cout<<"Press any to stop logging... " << timestamp <<" New EmoState from user " << userID << std::endl;

                  logEmoState(ofs, userID, eState, writeHeader);
                  writeHeader = false;
              }
         }
         else if (state != EDK_NO_EVENT)
         {
             std::cout << "Internal error in Emotiv Engine!" << std::endl;
             break;
         }
    }

    ofs.close();

    EE_EngineDisconnect();
    EE_EmoStateFree(eState);
    EE_EmoEngineEventFree(eEvent);
    return 0;
}


void logEmoState(std::ostream& os, unsigned int userID, EmoStateHandle eState, bool withHeader) {

    // Create the top header
    if (withHeader) {
        os << "Time,";
        os << "UserID,";
        os << "Wireless Signal Status,";
        os << "Blink,";
        os << "Wink Left,";
        os << "Wink Right,";
        os << "Look Left,";
        os << "Look Right,";
        os << "Eyebrow,";
        os << "Furrow,";
        os << "Smile,";
        os << "Clench,";
        os << "Smirk Left,";
        os << "Smirk Right,";
        os << "Laugh,";
        os << "Short Term Excitement,";
        os << "Long Term Excitement,";
        os << "Engagement/Boredom,";
        os << "Cognitiv Action,";
        os << "Cognitiv Power,";
        os << std::endl;
    }

    // Log the time stamp and user ID
    os << ES_GetTimeFromStart(eState) << ",";
    os << userID << ",";
    os << static_cast<int>(ES_GetWirelessSignalStatus(eState)) << ",";

    // Expressiv Suite results
    os << ES_ExpressivIsBlink(eState) << ",";
    os << ES_ExpressivIsLeftWink(eState) << ",";
    os << ES_ExpressivIsRightWink(eState) << ",";

    os << ES_ExpressivIsLookingLeft(eState) << ",";
    os << ES_ExpressivIsLookingRight(eState) << ",";

    std::map<EE_ExpressivAlgo_t, float> expressivStates;

    EE_ExpressivAlgo_t upperFaceAction = ES_ExpressivGetUpperFaceAction(eState);
    float			   upperFacePower  = ES_ExpressivGetUpperFaceActionPower(eState);

    EE_ExpressivAlgo_t lowerFaceAction = ES_ExpressivGetLowerFaceAction(eState);
    float			   lowerFacePower  = ES_ExpressivGetLowerFaceActionPower(eState);

    expressivStates[ upperFaceAction ] = upperFacePower;
    expressivStates[ lowerFaceAction ] = lowerFacePower;

    os << expressivStates[ EXP_EYEBROW     ] << ","; // eyebrow
    os << expressivStates[ EXP_FURROW      ] << ","; // furrow
    os << expressivStates[ EXP_SMILE       ] << ","; // smile
    os << expressivStates[ EXP_CLENCH      ] << ","; // clench
    os << expressivStates[ EXP_SMIRK_LEFT  ] << ","; // smirk left
    os << expressivStates[ EXP_SMIRK_RIGHT ] << ","; // smirk right
    os << expressivStates[ EXP_LAUGH       ] << ","; // laugh

    // Affectiv Suite results
    os << ES_AffectivGetExcitementShortTermScore(eState) << ",";
    os << ES_AffectivGetExcitementLongTermScore(eState) << ",";

    os << ES_AffectivGetEngagementBoredomScore(eState) << ",";

    // Cognitiv Suite results
    os << static_cast<int>(ES_CognitivGetCurrentAction(eState)) << ",";
    os << ES_CognitivGetCurrentActionPower(eState);

    os << std::endl;
}


