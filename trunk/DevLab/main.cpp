/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme Prá Vieira


Sonetto RPG Engine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Sonetto RPG Engine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation,
Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA or go to
http://www.gnu.org/copyleft/lesser.txt
-----------------------------------------------------------------------------*/
#include <Ogre.h>
#include <OgreConfigFile.h>
//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB
#include <OIS/OIS.h>
//#include "CustomPanelElementFactory.h"

#include "SonettoWindowFactory.h"
#include "SonettoFontSerializer.h"
#include "SonettoFontManager.h"
#include "SonettoTextFactory.h"
#include "SonettoSTRSerializer.h"
#include "SonettoSTRManager.h"

using namespace Ogre;

class ManualSTRLoader : public Ogre::ManualResourceLoader
{
public:

   ManualSTRLoader() {}
   virtual ~ManualSTRLoader() {}

   void loadResource(Ogre::Resource *resource)
   {
       Sonetto::STRData *strdata = static_cast<Sonetto::STRData *>(resource);

       Ogre::String tmp1 = "A STR File contain a set of\tstrings to be shown at the screen.";
       Ogre::String tmp2 = "A STR File can contain a ilimited\tnumber of strings (or messages)";
       Ogre::String tmp3 = "Each map must contain a STR\tfile for storing local messages.";
       Ogre::String tmp4 = "The main application of STR\tFiles is to ease localization of games.";

       strdata->insertMessage(tmp1);
       strdata->insertMessage(tmp2);
       strdata->insertMessage(tmp3);
       strdata->insertMessage(tmp4);
   }
};

class ManualFontLoader : public Ogre::ManualResourceLoader
{
public:

   ManualFontLoader() {}
   virtual ~ManualFontLoader() {}

   void loadResource(Ogre::Resource *resource)
   {
        Sonetto::Font *fontfile = static_cast<Sonetto::Font *>(resource);

        fontfile->font_blend_mode = Sonetto::FBM_TRANSPARENT_ALPHA;
        fontfile->font_alpha_reject_settings = Sonetto::SCF_ALWAYS_PASS;
        fontfile->alpha_rejection = 250;
        fontfile->font_filtering_mode = Sonetto::FT_LINEAR;

        fontfile->setUpFont(16, 16, 0.359375f, "BasicFont0.dds");

        fontfile->setSpaceWidth(0x20, 0.3f);
        fontfile->setSpaceWidth(0x21, 0.203125f);
        fontfile->setSpaceWidth(0x22, 0.28125f);
        fontfile->setSpaceWidth(0x24, 0.40625f);
        fontfile->setSpaceWidth(0x25, 0.40625f);
        fontfile->setSpaceWidth(0x26, 0.484375f);
        fontfile->setSpaceWidth(0x27, 0.1875f);
        fontfile->setSpaceWidth(0x28, 0.234375f);
        fontfile->setSpaceWidth(0x29, 0.234375f);
        fontfile->setSpaceWidth(0x2A, 0.359375f);
        fontfile->setSpaceWidth(0x2C, 0.25f);
        fontfile->setSpaceWidth(0x2D, 0.265625f);
        fontfile->setSpaceWidth(0x2E, 0.21875f);
        fontfile->setSpaceWidth(0x2F, 0.4375f);
        fontfile->setSpaceWidth(0x30, 0.40625f);
        fontfile->setSpaceWidth(0x31, 0.25f);
        fontfile->setSpaceWidth(0x32, 0.453125f);
        fontfile->setSpaceWidth(0x33, 0.46875f);
        fontfile->setSpaceWidth(0x34, 0.5f);
        fontfile->setSpaceWidth(0x35, 0.453125f);
        fontfile->setSpaceWidth(0x36, 0.453125f);
        fontfile->setSpaceWidth(0x37, 0.421875f);
        fontfile->setSpaceWidth(0x38, 0.453125f);
        fontfile->setSpaceWidth(0x39, 0.46875f);
        fontfile->setSpaceWidth(0x3A, 0.234375f);
        fontfile->setSpaceWidth(0x3B, 0.234375f);
        fontfile->setSpaceWidth(0x3C, 0.390625f);
        fontfile->setSpaceWidth(0x3D, 0.3125f);
        fontfile->setSpaceWidth(0x3E, 0.390625f);
        fontfile->setSpaceWidth(0x3F, 0.40625f);
        fontfile->setSpaceWidth(0x41, 0.421875f);
        fontfile->setSpaceWidth(0x42, 0.40625f);
        fontfile->setSpaceWidth(0x43, 0.375f);
        fontfile->setSpaceWidth(0x44, 0.390625f);
        fontfile->setSpaceWidth(0x45, 0.375f);
        fontfile->setSpaceWidth(0x46, 0.375f);
        fontfile->setSpaceWidth(0x47, 0.375f);
        fontfile->setSpaceWidth(0x48, 0.453125f);
        fontfile->setSpaceWidth(0x49, 0.203125f);
        fontfile->setSpaceWidth(0x4A, 0.3125f);
        fontfile->setSpaceWidth(0x4B, 0.421875f);
        fontfile->setSpaceWidth(0x4C, 0.375f);
        fontfile->setSpaceWidth(0x4D, 0.515625f);
        fontfile->setSpaceWidth(0x4E, 0.421875f);
        fontfile->setSpaceWidth(0x4F, 0.40625f);
        fontfile->setSpaceWidth(0x50, 0.40625f);
        fontfile->setSpaceWidth(0x51, 0.40625f);
        fontfile->setSpaceWidth(0x52, 0.40625f);
        fontfile->setSpaceWidth(0x53, 0.421875f);
        fontfile->setSpaceWidth(0x54, 0.4375f);
        fontfile->setSpaceWidth(0x55, 0.4375f);
        fontfile->setSpaceWidth(0x56, 0.4375f);
        fontfile->setSpaceWidth(0x57, 0.609375f);
        fontfile->setSpaceWidth(0x58, 0.421875f);
        fontfile->setSpaceWidth(0x59, 0.421875f);
        fontfile->setSpaceWidth(0x5A, 0.453125f);
        fontfile->setSpaceWidth(0x5B, 0.328125f);
        fontfile->setSpaceWidth(0x5C, 0.4375f);
        fontfile->setSpaceWidth(0x5D, 0.3125f);
        fontfile->setSpaceWidth(0x5F, 0.453125f);
        fontfile->setSpaceWidth(0x60, 0.25f);
        // Lower case alphabet
        fontfile->setSpaceWidth(0x61, 0.390625f);
        fontfile->setSpaceWidth(0x62, 0.390625f);
        fontfile->setSpaceWidth(0x63, 0.390625f);
        fontfile->setSpaceWidth(0x64, 0.421875f);
        fontfile->setSpaceWidth(0x65, 0.421875f);
        fontfile->setSpaceWidth(0x66, 0.359375f);
        fontfile->setSpaceWidth(0x67, 0.40625f);
        fontfile->setSpaceWidth(0x68, 0.390625f);
        fontfile->setSpaceWidth(0x69, 0.265625f);
        fontfile->setSpaceWidth(0x6A, 0.3125f);
        fontfile->setSpaceWidth(0x6B, 0.4375f);
        fontfile->setSpaceWidth(0x6C, 0.25f);
        fontfile->setSpaceWidth(0x6D, 0.53125f);
        fontfile->setSpaceWidth(0x6E, 0.390625f);
        fontfile->setSpaceWidth(0x6F, 0.40625f);

        fontfile->setSpaceWidth(0x70, 0.40625f);
        fontfile->setSpaceWidth(0x71, 0.40625f);
        fontfile->setSpaceWidth(0x72, 0.390625f);
        fontfile->setSpaceWidth(0x73, 0.40625f);
        fontfile->setSpaceWidth(0x74, 0.359375f);
        fontfile->setSpaceWidth(0x75, 0.421875f);
        fontfile->setSpaceWidth(0x76, 0.40625f);
        fontfile->setSpaceWidth(0x77, 0.53125f);
        fontfile->setSpaceWidth(0x78, 0.390625f);
        fontfile->setSpaceWidth(0x79, 0.40625f);
        fontfile->setSpaceWidth(0x7A, 0.40625f);

        // Latin Characters (Upper Case)
        fontfile->setSpaceWidth(0xC0, 0.46875f);
        fontfile->setSpaceWidth(0xC1, 0.46875f);
        fontfile->setSpaceWidth(0xC2, 0.46875f);
        fontfile->setSpaceWidth(0xC3, 0.46875f);
        fontfile->setSpaceWidth(0xC4, 0.46875f);
        fontfile->setSpaceWidth(0xC5, 0.46875f);
        fontfile->setSpaceWidth(0xC6, 0.640625f);
        fontfile->setSpaceWidth(0xC7, 0.4375f);
        fontfile->setSpaceWidth(0xC8, 0.4375f);
        fontfile->setSpaceWidth(0xC9, 0.4375f);
        fontfile->setSpaceWidth(0xCA, 0.4375f);
        fontfile->setSpaceWidth(0xCB, 0.4375f);
        fontfile->setSpaceWidth(0xCC, 0.28125f);
        fontfile->setSpaceWidth(0xCD, 0.28125f);
        fontfile->setSpaceWidth(0xCE, 0.28125f);
        fontfile->setSpaceWidth(0xCF, 0.28125f);

        fontfile->setSpaceWidth(0xD1, 0.5f);
        fontfile->setSpaceWidth(0xD2, 0.46875f);
        fontfile->setSpaceWidth(0xD3, 0.46875f);
        fontfile->setSpaceWidth(0xD4, 0.46875f);
        fontfile->setSpaceWidth(0xD5, 0.46875f);
        fontfile->setSpaceWidth(0xD6, 0.46875f);

        fontfile->setSpaceWidth(0xD8, 0.46875f);
        fontfile->setSpaceWidth(0xD9, 0.5f);
        fontfile->setSpaceWidth(0xDA, 0.5f);
        fontfile->setSpaceWidth(0xDB, 0.5f);
        fontfile->setSpaceWidth(0xDC, 0.5f);
        fontfile->setSpaceWidth(0xDD, 0.609375f);
        fontfile->setSpaceWidth(0xDE, 0.609375f);
        fontfile->setSpaceWidth(0xDF, 0.609375f);

        // Latin Characters (Lower case)
        fontfile->setSpaceWidth(0xE0, 0.390625f);
        fontfile->setSpaceWidth(0xE1, 0.390625f);
        fontfile->setSpaceWidth(0xE2, 0.390625f);
        fontfile->setSpaceWidth(0xE3, 0.390625f);
        fontfile->setSpaceWidth(0xE4, 0.390625f);
        fontfile->setSpaceWidth(0xE5, 0.390625f);
        fontfile->setSpaceWidth(0xE6, 0.578125f);
        fontfile->setSpaceWidth(0xE7, 0.390625f);
        fontfile->setSpaceWidth(0xE8, 0.421875f);
        fontfile->setSpaceWidth(0xE9, 0.421875f);
        fontfile->setSpaceWidth(0xEA, 0.421875f);
        fontfile->setSpaceWidth(0xEB, 0.421875f);
        fontfile->setSpaceWidth(0xEC, 0.265625f);
        fontfile->setSpaceWidth(0xED, 0.265625f);
        fontfile->setSpaceWidth(0xEE, 0.265625f);
        fontfile->setSpaceWidth(0xEF, 0.265625f);
        fontfile->setSpaceWidth(0xF0, 0.515625f);
        fontfile->setSpaceWidth(0xF1, 0.390625f);
        fontfile->setSpaceWidth(0xF2, 0.40625f);
        fontfile->setSpaceWidth(0xF3, 0.40625f);
        fontfile->setSpaceWidth(0xF4, 0.40625f);
        fontfile->setSpaceWidth(0xF5, 0.40625f);
        fontfile->setSpaceWidth(0xF6, 0.40625f);

        fontfile->setSpaceWidth(0xF8, 0.421875f);
        fontfile->setSpaceWidth(0xF9, 0.421875f);
        fontfile->setSpaceWidth(0xFA, 0.421875f);
        fontfile->setSpaceWidth(0xFB, 0.421875f);
        fontfile->setSpaceWidth(0xFC, 0.421875f);


        fontfile->setSpaceWidth(0xFF, 0.40625f);

        /*ColourValue color1 = ColourValue(210.0f/255.0f,200.0f/255.0f,160.0f/255.0f,1.0f);
        ColourValue color2 = ColourValue(1.0f,0.9f,0.5f,1.0f);
        ColourValue color3 = ColourValue(170 / 255.0f, 1.0f, 244 / 255.0f,1.0f);*/
        ColourValue color1 = ColourValue(1.0f,1.0f,1.0f,1.0f);
        ColourValue color2 = ColourValue(1.0f,0.9f,0.5f,1.0f);
        ColourValue color3 = ColourValue(170 / 255.0f, 1.0f, 244 / 255.0f,1.0f);
        fontfile->mColorList.push_back(color1);
        fontfile->mColorList.push_back(color2);
        fontfile->mColorList.push_back(color3);

   }
};

class DevLabFrameListener: public FrameListener, public WindowEventListener
{
protected:
    virtual void updateStats(const Ogre::FrameEvent &evt)
    {
        static String currFps = "QPS Atual: ";
        static String avgFps = "Media de QPS: ";
        static String bestFps = "Melhor QPS: ";
        static String worstFps = "Pior QPS: ";
        static String tris = "Time Since Last Frame: ";
        static String batches = "St. de Batches: ";
        // update stats when necessary
        try
        {
            OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
            OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
            OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
            OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
            const RenderTarget::FrameStats& stats = mWindow->getStatistics();
            guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
            guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
            guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
                                +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
            guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
                                 +" "+StringConverter::toString(stats.worstFrameTime)+" ms");
            OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
            guiTris->setCaption(tris + StringConverter::toString(evt.timeSinceLastFrame));
            OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
            guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));
            OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
            guiDbg->setCaption(mDebugText);
        }
        catch (...) { /* ignore */ }
    }
public:
    // Constructor takes a RenderWindow because it uses that to determine input context
    DevLabFrameListener(RenderWindow* win, Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false,
                        bool bufferedJoy = false ) :
            mCamera(cam), mTranslateVector(Vector3::ZERO), mWindow(win), mStatsOn(true), mNumScreenShots(0),
            mMoveScale(0.0f), mRotScale(0.0f), mTimeUntilNextToggle(0), mFiltering(TFO_BILINEAR),
            mAniso(1), mSceneDetailIndex(0), mMoveSpeed(100), mRotateSpeed(36), mDebugOverlay(0),
            mInputManager(0), mMouse(0), mKeyboard(0), mJoy(0)
    {
        using namespace OIS;
        mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
        LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
        ParamList pl;
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;
        win->getCustomAttribute("WINDOW", &windowHnd);
        windowHndStr << windowHnd;
        pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
        mInputManager = InputManager::createInputSystem( pl );
        //Create all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
        mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, bufferedKeys ));
        mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, bufferedMouse ));
        try
        {
            mJoy = static_cast<JoyStick*>(mInputManager->createInputObject( OISJoyStick, bufferedJoy ));
        }
        catch (...)
        {
            mJoy = 0;
        }
        //Set initial mouse clipping size
        windowResized(mWindow);
        showDebugOverlay(true);
        //Register as a Window listener
        WindowEventUtilities::addWindowEventListener(mWindow, this);
    }
    //Adjust mouse clipping area
    virtual void windowResized(RenderWindow* rw)
    {
        unsigned int width, height, depth;
        int left, top;
        rw->getMetrics(width, height, depth, left, top);
        const OIS::MouseState &ms = mMouse->getMouseState();
        ms.width = width;
        ms.height = height;
    }
    //Unattach OIS before window shutdown (very important under Linux)
    virtual void windowClosed(RenderWindow* rw)
    {
        //Only close for window that created OIS (the main window in these demos)
        if ( rw == mWindow )
        {
            if ( mInputManager )
            {
                mInputManager->destroyInputObject( mMouse );
                mInputManager->destroyInputObject( mKeyboard );
                mInputManager->destroyInputObject( mJoy );
                OIS::InputManager::destroyInputSystem(mInputManager);
                mInputManager = 0;
            }
        }
    }
    virtual ~DevLabFrameListener()
    {
        //Remove ourself as a Window listener
        WindowEventUtilities::removeWindowEventListener(mWindow, this);
        windowClosed(mWindow);
    }
    virtual bool processUnbufferedKeyInput(const FrameEvent& evt)
    {
        using namespace OIS;
        if (mKeyboard->isKeyDown(KC_A))
            mTranslateVector.x = -mMoveScale;	// Move camera left
        if (mKeyboard->isKeyDown(KC_D))
            mTranslateVector.x = mMoveScale;	// Move camera RIGHT
        if (mKeyboard->isKeyDown(KC_UP) || mKeyboard->isKeyDown(KC_W) )
            mTranslateVector.z = -mMoveScale;	// Move camera forward
        if (mKeyboard->isKeyDown(KC_DOWN) || mKeyboard->isKeyDown(KC_S) )
            mTranslateVector.z = mMoveScale;	// Move camera backward
        if (mKeyboard->isKeyDown(KC_PGUP))
            mTranslateVector.y = mMoveScale;	// Move camera up
        if (mKeyboard->isKeyDown(KC_PGDOWN))
            mTranslateVector.y = -mMoveScale;	// Move camera down
        if (mKeyboard->isKeyDown(KC_RIGHT))
            mCamera->yaw(-mRotScale);
        if (mKeyboard->isKeyDown(KC_LEFT))
            mCamera->yaw(mRotScale);
        if ( mKeyboard->isKeyDown(KC_ESCAPE) || mKeyboard->isKeyDown(KC_Q) )
            return false;
        if ( mKeyboard->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0 )
        {
            mStatsOn = !mStatsOn;
            showDebugOverlay(mStatsOn);
            mTimeUntilNextToggle = 1;
        }
        if ( mKeyboard->isKeyDown(KC_T) && mTimeUntilNextToggle <= 0 )
        {
            switch (mFiltering)
            {
            case TFO_BILINEAR:
                mFiltering = TFO_TRILINEAR;
                mAniso = 1;
                break;
            case TFO_TRILINEAR:
                mFiltering = TFO_ANISOTROPIC;
                mAniso = 8;
                break;
            case TFO_ANISOTROPIC:
                mFiltering = TFO_BILINEAR;
                mAniso = 1;
                break;
            default:
                break;
            }
            MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
            MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);
            showDebugOverlay(mStatsOn);
            mTimeUntilNextToggle = 1;
        }
        if (mKeyboard->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
        {
            std::ostringstream ss;
            ss << "screenshot_" << ++mNumScreenShots << ".png";
            mWindow->writeContentsToFile(ss.str());
            mTimeUntilNextToggle = 0.5;
            mDebugText = "Saved: " + ss.str();
        }
        if (mKeyboard->isKeyDown(KC_R) && mTimeUntilNextToggle <=0)
        {
            mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
            switch (mSceneDetailIndex)
            {
            case 0 :
                mCamera->setPolygonMode(PM_SOLID);
                break;
            case 1 :
                mCamera->setPolygonMode(PM_WIREFRAME);
                break;
            case 2 :
                mCamera->setPolygonMode(PM_POINTS);
                break;
            }
            mTimeUntilNextToggle = 0.5;
        }
        static bool displayCameraDetails = false;
        if (mKeyboard->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
        {
            displayCameraDetails = !displayCameraDetails;
            mTimeUntilNextToggle = 0.5;
            if (!displayCameraDetails)
                mDebugText = "";
        }
        // Print camera details
        if (displayCameraDetails)
            mDebugText = "P: " + StringConverter::toString(mCamera->getDerivedPosition()) +
                         " " + "O: " + StringConverter::toString(mCamera->getDerivedOrientation());
        // Return true to continue rendering
        return true;
    }
    virtual bool processUnbufferedMouseInput(const FrameEvent& evt)
    {
        using namespace OIS;
        // Rotation factors, may not be used if the second mouse button is pressed
        // 2nd mouse button - slide, otherwise rotate
        const MouseState &ms = mMouse->getMouseState();
        if ( ms.buttonDown( MB_Right ) )
        {
            mTranslateVector.x += ms.X.rel * 0.13;
            mTranslateVector.y -= ms.Y.rel * 0.13;
        }
        else
        {
            mRotX = Degree(-ms.X.rel * 0.13);
            mRotY = Degree(-ms.Y.rel * 0.13);
        }
        return true;
    }
    virtual void moveCamera()
    {
        // Make all the changes to the camera
        // Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW
        //(e.g. airplane)
        mCamera->yaw(mRotX);
        mCamera->pitch(mRotY);
        mCamera->moveRelative(mTranslateVector);
    }
    virtual void showDebugOverlay(bool show)
    {
        if (mDebugOverlay)
        {
            if (show)
                mDebugOverlay->show();
            else
                mDebugOverlay->hide();
        }
    }
    // Override frameRenderingQueued event to process that (don't care about frameEnded)
    bool frameRenderingQueued(const FrameEvent& evt)
    {
        using namespace OIS;
        if (mWindow->isClosed())	return false;
        //Need to capture/update each device
        mKeyboard->capture();
        mMouse->capture();
        if ( mJoy ) mJoy->capture();
        bool buffJ = (mJoy) ? mJoy->buffered() : true;
        //Check if one of the devices is not buffered
        if ( !mMouse->buffered() || !mKeyboard->buffered() || !buffJ )
        {
            // one of the input modes is immediate, so setup what is needed for immediate movement
            if (mTimeUntilNextToggle >= 0)
                mTimeUntilNextToggle -= evt.timeSinceLastFrame;
            // If this is the first frame, pick a speed
            if (evt.timeSinceLastFrame == 0)
            {
                mMoveScale = 1;
                mRotScale = 0.1;
            }
            // Otherwise scale movement units by time passed since last frame
            else
            {
                // Move about 100 units per second,
                mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
                // Take about 10 seconds for full rotation
                mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
            }
            mRotX = 0;
            mRotY = 0;
            mTranslateVector = Ogre::Vector3::ZERO;
        }
        //Check to see which device is not buffered, and handle it
        if ( !mKeyboard->buffered() )
            if ( processUnbufferedKeyInput(evt) == false )
                return false;
        if ( !mMouse->buffered() )
            if ( processUnbufferedMouseInput(evt) == false )
                return false;
        if ( !mMouse->buffered() || !mKeyboard->buffered() || !buffJ )
            moveCamera();
        return true;
    }
    bool frameEnded(const FrameEvent& evt)
    {
        updateStats(evt);
        return true;
    }
protected:
    Camera* mCamera;
    Vector3 mTranslateVector;
    RenderWindow* mWindow;
    bool mStatsOn;
    std::string mDebugText;
    unsigned int mNumScreenShots;
    float mMoveScale;
    Degree mRotScale;
    // just to stop toggles flipping too fast
    Real mTimeUntilNextToggle ;
    Radian mRotX, mRotY;
    TextureFilterOptions mFiltering;
    int mAniso;
    int mSceneDetailIndex ;
    Real mMoveSpeed;
    Degree mRotateSpeed;
    Overlay* mDebugOverlay;
    //OIS Input devices
    OIS::InputManager* mInputManager;
    OIS::Mouse*    mMouse;
    OIS::Keyboard* mKeyboard;
    OIS::JoyStick* mJoy;
};
class DevLab
{
public:
    DevLab()
    {
        mFrameListener = 0;
        mRoot = 0;
        mResourcePath = "";
    }
    ~DevLab()
    {
        if (mFrameListener)
            delete mFrameListener;
        if (mRoot)
            delete mRoot;
    }
    void go(void)
    {
        if (!setup())
            return;
        mRoot->startRendering();
        // clean up
        destroyScene();
    }
protected:
    Root *mRoot;
    Camera* mCamera;
    SceneManager* mSceneMgr;
    DevLabFrameListener* mFrameListener;
    RenderWindow* mWindow;
    Sonetto::String mResourcePath;
    Sonetto::WindowFactory * mWindowFactory;
    Sonetto::TailedWindowFactory * mTailedWindowFactory;
    Sonetto::SlimWindowFactory * mSlimWindowFactory;
    Sonetto::FontManager *mFontManager;
    Sonetto::STRManager *mSTRManager;
    OverlayElementFactory * mTextElementFactory;
    bool setup(void)
    {
        String pluginsPath;
        // only use plugins.cfg if not static
#ifndef OGRE_STATIC_LIB
        pluginsPath = mResourcePath + "plugins.dlc";
#endif
        mRoot = new Root(pluginsPath,
                         mResourcePath + "devlab.dlc", mResourcePath + "DevLab.log");
        setupResources();
        bool carryOn = configure();
        if (!carryOn) return false;
        chooseSceneManager();
        createCamera();
        createViewports();
        // Set default mipmap level (NB some APIs ignore this)
        TextureManager::getSingleton().setDefaultNumMipmaps(5);

        // Set up the custom objects factory and managers.
        mFontManager = new Sonetto::FontManager();

        mWindowFactory = new Sonetto::WindowFactory();
        OverlayManager::getSingleton().addOverlayElementFactory(mWindowFactory);

        mTailedWindowFactory = new Sonetto::TailedWindowFactory();
        OverlayManager::getSingleton().addOverlayElementFactory(mTailedWindowFactory);

        mSlimWindowFactory = new Sonetto::SlimWindowFactory();
        OverlayManager::getSingleton().addOverlayElementFactory(mSlimWindowFactory);

        mTextElementFactory = new Sonetto::TextElementFactory();
        Ogre::OverlayManager::getSingleton().addOverlayElementFactory(mTextElementFactory);

        mSTRManager = new Sonetto::STRManager();

        ManualFontLoader *mtfl = new ManualFontLoader;
        Ogre::ResourceGroupManager::getSingleton ().declareResource("BASE.fnt", "Font", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mtfl);

        ManualSTRLoader *mstrl = new ManualSTRLoader;
        Ogre::ResourceGroupManager::getSingleton ().declareResource("BASE.STR", "STR", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, mstrl);

        // Create any resource listeners (for loading screens)
        createResourceListener();
        // Load resources
        loadResources();

        // Create the scene
        createScene();
        createFrameListener();
        return true;
    }
    bool configure(void)
    {
        // Show the configuration dialog and initialise the system
        // You can skip this and use root.restoreConfig() to load configuration
        // settings if you were sure there are valid ones saved in ogre.cfg
        if (mRoot->showConfigDialog())
        {
            // If returned true, user clicked OK so initialise
            // Here we choose to let the system create a default rendering window by passing 'true'
            mWindow = mRoot->initialise(true, "Sonetto Development Laboratory");
            return true;
        }
        else
        {
            return false;
        }
    }
    void chooseSceneManager(void)
    {
        // Create the SceneManager, in this case a generic one
        mSceneMgr = mRoot->createSceneManager(ST_GENERIC, "ExampleSMInstance");
    }
    void createCamera(void)
    {
        // Create the camera
        mCamera = mSceneMgr->createCamera("PlayerCam");
        // Position it at 500 in Z direction
        mCamera->setPosition(Vector3(10,20,10));
        // Look back along -Z
        mCamera->lookAt(Vector3(0,0,-300));
        mCamera->setNearClipDistance(5);
        mCamera->setFOVy(Ogre::Radian(Ogre::Degree(45)));
    }
    void createFrameListener(void)
    {
        mFrameListener= new DevLabFrameListener(mWindow, mCamera);
        mFrameListener->showDebugOverlay(true);
        mRoot->addFrameListener(mFrameListener);
    }
    void createScene(void)
    {
        //Sonetto::FontPtr fontfile = mFontManager->load("BasicFont1.fnt",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        Sonetto::FontPtr fontfile = mFontManager->load("BASE.fnt",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        //Sonetto::FontSerializer serializer;
        //serializer.exportFont(static_cast<Sonetto::Font *>(fontfile.getPointer()), "data/BaseFont.fnt");

        Sonetto::STRDataPtr strfile = mSTRManager->load("BASE.STR",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        Sonetto::STRSerializer serializer2;
        //serializer2.exportSTR(static_cast<Sonetto::STRData *>(strfile.getPointer()), "data/TestMessage.str");

        strfile = mSTRManager->load("TestMessage.str",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        fontfile = mFontManager->load("BaseFont.fnt",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);


        Sonetto::WindowTexCoord * tmpTexCoords = new Sonetto::WindowTexCoord();

        tmpTexCoords->slim_left.left = (4/256.0f);
        tmpTexCoords->slim_left.right = (20/256.0f);
        tmpTexCoords->slim_left.top = (160/256.0f);
        tmpTexCoords->slim_left.bottom = (188/256.0f);

        tmpTexCoords->slim_center.left = (20/256.0f);
        tmpTexCoords->slim_center.right = (61/256.0f);
        tmpTexCoords->slim_center.top = (160/256.0f);
        tmpTexCoords->slim_center.bottom = (188/256.0f);

        tmpTexCoords->slim_right.left = (60/256.0f);
        tmpTexCoords->slim_right.right = (75/256.0f);
        tmpTexCoords->slim_right.top = (160/256.0f);
        tmpTexCoords->slim_right.bottom = (188/256.0f);


        tmpTexCoords->top_left.left = (4/256.0f);
        tmpTexCoords->top_left.right = (28/256.0f);
        tmpTexCoords->top_left.top = (196/256.0f);
        tmpTexCoords->top_left.bottom = (220/256.0f);

        tmpTexCoords->top.left = (100/256.0f);
        tmpTexCoords->top.right = (124/256.0f);
        tmpTexCoords->top.top = (196/256.0f);
        tmpTexCoords->top.bottom = (220/256.0f);

        tmpTexCoords->top_right.left = (36/256.0f);
        tmpTexCoords->top_right.right = (60/256.0f);
        tmpTexCoords->top_right.top = (196/256.0f);
        tmpTexCoords->top_right.bottom = (220/256.0f);

        tmpTexCoords->left.left = (68/256.0f);
        tmpTexCoords->left.right = (92/256.0f);
        tmpTexCoords->left.top = (196/256.0f);
        tmpTexCoords->left.bottom = (220/256.0f);

        tmpTexCoords->center.left = (196/256.0f);
        tmpTexCoords->center.right = (220/256.0f);
        tmpTexCoords->center.top = (228/256.0f);
        tmpTexCoords->center.bottom = (252/256.0f);

        tmpTexCoords->right.left = (68/256.0f);
        tmpTexCoords->right.right = (92/256.0f);
        tmpTexCoords->right.top = (228/256.0f);
        tmpTexCoords->right.bottom = (252/256.0f);

        tmpTexCoords->bottom_left.left = (4/256.0f);
        tmpTexCoords->bottom_left.right = (28/256.0f);
        tmpTexCoords->bottom_left.top = (228/256.0f);
        tmpTexCoords->bottom_left.bottom = (252/256.0f);

        tmpTexCoords->bottom.left = (100/256.0f);
        tmpTexCoords->bottom.right = (124/256.0f);
        tmpTexCoords->bottom.top = (228/256.0f);
        tmpTexCoords->bottom.bottom = (252/256.0f);

        tmpTexCoords->bottom_right.left = (36/256.0f);
        tmpTexCoords->bottom_right.right = (60/256.0f);
        tmpTexCoords->bottom_right.top = (228/256.0f);
        tmpTexCoords->bottom_right.bottom = (252/256.0f);

        tmpTexCoords->tail_up.left = (132/256.0f);
        tmpTexCoords->tail_up.right = (156/256.0f);
        tmpTexCoords->tail_up.top = (204/256.0f);
        tmpTexCoords->tail_up.bottom = (252/256.0f);

        tmpTexCoords->tail_down.left = (164/256.0f);
        tmpTexCoords->tail_down.right = (187/256.0f);
        tmpTexCoords->tail_down.top = (204/256.0f);
        tmpTexCoords->tail_down.bottom = (252/256.0f);

        Ogre::Entity * mBG = mSceneMgr->createEntity("BTLBG","BTLbg012.mesh");

        mSceneMgr->getRootSceneNode()->attachObject(mBG);

        Ogre::Overlay * m_pOverlay = Ogre::OverlayManager::getSingleton().create("shapeName");
        Ogre::OverlayContainer * m_pContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
                                                    "TailedWindow", "container1"));
        m_pContainer->setMaterialName("windowskin_bg");
        static_cast<Sonetto::TailedWindow*>(m_pContainer)->setBorderMaterialName("windowskin_border");
        //m_pContainer->setDimensions(0.33958333333333333333333333333333f,0.8375f);
        //m_pContainer->setPosition(0.82708333333333333333333333333333f, 0.0f);
        m_pContainer->setDimensions(0.5f,0.5f);
        m_pContainer->setPosition(0.5f,0.5f);
        static_cast<Sonetto::TailedWindow*>(m_pContainer)->setTexCoords(tmpTexCoords);
        //static_cast<Sonetto::SonettoWindow*>(m_pContainer)->setBackgroundColor(56/255.0f,56/255.0f,80/255.0f,232/255.0f);
        static_cast<Sonetto::TailedWindow*>(m_pContainer)->setBackgroundColor(0/255.0f,72/255.0f,144/255.0f,232/255.0f);
        static_cast<Sonetto::TailedWindow*>(m_pContainer)->setBorderColor(1.0f,1.0f,1.0f,1.0f);
        static_cast<Sonetto::TailedWindow*>(m_pContainer)->setTailDirection(true);
        static_cast<Sonetto::TailedWindow*>(m_pContainer)->setTailPosition(0.5f);
        //static_cast<Sonetto::SonettoWindow*>(m_pContainer)->setScrMetricsMode(Sonetto::SMM_RELATIVE);

        m_pOverlay->add2D(m_pContainer);
        m_pContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
                           "Window", "container2"));
        m_pContainer->setMaterialName("windowskin_bg");
        static_cast<Sonetto::Window*>(m_pContainer)->setBorderMaterialName("windowskin_border");
        /*m_pContainer->setDimensions(0.33958333333333333333333333333333f,0.1625f);
        m_pContainer->setPosition(0.82708333333333333333333333333333f, 0.8375f);*/
        m_pContainer->setDimensions(1.0f,0.1f);
        m_pContainer->setPosition(0.0f, 0.2f);
        static_cast<Sonetto::Window*>(m_pContainer)->setTexCoords(tmpTexCoords);
        static_cast<Sonetto::Window*>(m_pContainer)->setBackgroundColor(48/255.0f,104/255.0f,120/255.0f,216/255.0f);
        m_pOverlay->add2D(m_pContainer);
        m_pOverlay->show();

        m_pContainer = static_cast<Ogre::OverlayContainer*>(Ogre::OverlayManager::getSingleton().createOverlayElement(
                           "SlimWindow", "container3"));
        m_pContainer->setMaterialName("windowskin_bg");
        static_cast<Sonetto::SlimWindow*>(m_pContainer)->setBorderMaterialName("windowskin_border");
        m_pContainer->setDimensions(1.0f,0.0f);
        m_pContainer->setPosition(0.0f, 0.0f);
        static_cast<Sonetto::SlimWindow*>(m_pContainer)->setTexCoords(tmpTexCoords);
        static_cast<Sonetto::SlimWindow*>(m_pContainer)->setBackgroundColor(136/255.0f,40/255.0f,40/255.0f,232/255.0f);
        m_pOverlay->add2D(m_pContainer);
        m_pOverlay->show();

        Ogre::OverlayElement * m_pText = Ogre::OverlayManager::getSingleton().createOverlayElement("Text", "shapeNameText");
        m_pText->setDimensions(1.0, 1.0);
        m_pText->setPosition(0.0f,0.0f);
        static_cast<Sonetto::TextElement*>(m_pText)->setFont(fontfile);
        m_pContainer->addChild(m_pText);
        static_cast<Sonetto::TextElement*>(m_pText)->setCaption(strfile->getMessage(0));
        static_cast<Sonetto::TextElement*>(m_pText)->forceAnimReset();
        static_cast<Sonetto::TextElement*>(m_pText)->setTextSize(0.1f);

        m_pText = Ogre::OverlayManager::getSingleton().createOverlayElement("Text", "shapeNameText2");
        m_pText->setDimensions(1.0, 1.0);
        m_pText->setPosition(0.0f,0.3f);
        static_cast<Sonetto::TextElement*>(m_pText)->setFont(fontfile);
        m_pContainer->addChild(m_pText);
        static_cast<Sonetto::TextElement*>(m_pText)->setCaption(strfile->getMessage(1));
        static_cast<Sonetto::TextElement*>(m_pText)->forceAnimReset();
        static_cast<Sonetto::TextElement*>(m_pText)->setTextSize(0.1f);

        m_pText = Ogre::OverlayManager::getSingleton().createOverlayElement("Text", "shapeNameText3");
        m_pText->setDimensions(1.0, 1.0);
        m_pText->setPosition(0.0f,0.5f);
        static_cast<Sonetto::TextElement*>(m_pText)->setFont(fontfile);
        m_pContainer->addChild(m_pText);
        static_cast<Sonetto::TextElement*>(m_pText)->setCaption(strfile->getMessage(2));
        static_cast<Sonetto::TextElement*>(m_pText)->forceAnimReset();
        static_cast<Sonetto::TextElement*>(m_pText)->setTextSize(0.1f);

        //static_cast<Sonetto::TextElement*>(m_pText)->setCaption("The solution we are slowly moving\ntowards is a voluntary collective license for music,\nwhich consumers could choose to pay, or not.\nIt needs to work all over the world.\nNational boundaries don’t apply to\nthis kind of information anymore.\nTo pretend they do is\nas nostalgic a notion as DRM.");

    }
    void destroyScene(void)
    {}
    void createViewports(void)
    {
        // Create one viewport, entire window
        Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(ColourValue((16/255.0f),(102/255.0f),(208/255.0f)));
        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(
            Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    }
    void setupResources(void)
    {
        // Load resource paths from config file
        ConfigFile cf;
        cf.load(mResourcePath + "data.dlc");
        // Go through all sections & settings in the file
        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap *settings = seci.getNext();
            ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
            }
        }
    }
    void createResourceListener(void)
    {}
    void loadResources(void)
    {
        // Initialise, parse scripts etc
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }
};
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
    DevLab devLab;
    try
    {
        devLab.go();
    }
    catch ( Exception& e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }
    return 0;
}
