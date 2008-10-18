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

#include "SonettoKernel.h"
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include "TESTMapFileSerializer.h"

// Forward declarations
namespace Sonetto {
    class Module;
};

namespace Sonetto {
    //-----------------------------------------------------------------------------
    // Sonetto::Kernel implementation.
    //-----------------------------------------------------------------------------
    Kernel *Kernel::mSingleton = NULL;
    //-----------------------------------------------------------------------------
    Kernel::Kernel() : mAspectRatio(ASPECT_RATIO_4_3), mShutdown(false), mScreen(NULL),
    mRoot(NULL), mWindow(NULL), mViewport(NULL), mLogMan(NULL),
    mOverlayMan(NULL), mResourceMan(NULL),mResMan(NULL)
    #ifdef _DEBUG
    , mDebugOverlay(NULL)
    #endif
    , mFrameTime(0.0f)
    , mScreenWidth(640)
    , mScreenHeight(480)
    , mIsFullScreen(false)
    , mFadeOverlay(NULL)
    , mFadeStatus(FS_IDLE_IN)
    , mFadeSpeed(60.0f)
    , mFadeAlpha(0.0f)
    , mFullScreenSwitchLock(0)

    {
        #ifdef DEBUG_BUILD
        mFrameNumber = 0.0f;
        mGameSpeedUpSwitch = 0;
        mGameNormalSpeedSwitch = 0;
        mGameSpeedDownSwitch = 0;
        mGameSpeed = 1.0f;
        mGameSpeedScaleValue = 1.0f/2.0f;
        mAspectRatioMode = 0;
        mAspectRatioSwitch = 0;
        #endif

        NameValuePairList  wndParamList; // Needed for Ogre to use SDL rendering window
        SDL_SysWMinfo      wmInfo;       // Structure holding SDL window information

        // Initialises SDL video and joystick subsystems
        if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) == -1)
        {
            SONETTO_THROW("Could not initialise SDL");
        }

        // Create SDL rendering window
        // <todo> Load resolution from configuration file
        mScreen = SDL_SetVideoMode(640,480,0,0);
        if (!mScreen)
        {
            SONETTO_THROW("Could not create SDL window");
        }

        // RPGs don't use mouse, bwagahgah
        SDL_ShowCursor(SDL_DISABLE);
        SDL_WM_SetCaption("Sonetto Game Runtime","Sonetto Game Runtime");

        // <todo> Add some loading imagery here
        SDL_Flip(mScreen);

        // Get window info to attach Ogre at it
        SDL_VERSION(&wmInfo.version);
        SDL_GetWMInfo(&wmInfo);

        // Creates and configures Ogre root object, based on configuration
        // took from the configuration file
        #ifdef _DEBUG
        mRoot = new Ogre::Root("","","devlab.log");
        #else
        mRoot = new Ogre::Root("","",""); // Release version should not create logs.
       	#endif

        #ifdef _WINDOWS
            // Under Windows we just need to pass the window handle(HWND) as a string to
            // Ogre::Root::createRenderWindow() for Ogre to use such window for its drawings
            wndParamList["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)wmInfo.window);
        #else
            // Under *nixes, Ogre uses GLX for rendering. The syntax of
            // externalWindowHandle in this case is a bit different, but it
            // does essentially the same thing.
            std::string         wndHandleStr;
            std::string         dsp(&(DisplayString(wmInfo.info.x11.display)[1]));
            vector<Ogre::String> tokens = StringUtil::split(dsp,".");

            wndHandleStr = Ogre::StringConverter::toString((long)wmInfo.info.x11.display)+
                        ":"+tokens[1]+":"+
                        Ogre::StringConverter::toString((long)wmInfo.info.x11.window);

            wndParamList["externalWindowHandle"] = wndHandleStr;
        #endif

        // Create Input Manager
        mInputMan = new InputManager(4);

        // Create Audio Manager
        mAudioMan = new AudioManager();

        // Create Script Manager
        mScriptMan = new ScriptManager();

        loadConfig("config.ini",wndParamList);

        // Initialise Ogre
        mRoot->initialise(false);
        mWindow = mRoot->createRenderWindow("", mScreenWidth, mScreenHeight ,mIsFullScreen,&wndParamList);

        // Get Ogre singletons for easier use
        mLogMan      = LogManager::getSingletonPtr();
        mOverlayMan  = OverlayManager::getSingletonPtr();
        mResourceMan = ResourceGroupManager::getSingletonPtr();
        mMaterialMan = MaterialManager::getSingletonPtr();

        // Create the Database
        mDatabase = new Database();
        mDatabase->mKernel = this;
        mDatabase->load("testdatabase.dat");
        mDatabase->mSystemHeader.mCursorCancel = 1;
        mDatabase->mSystemHeader.mCursorMove = 2;
        mDatabase->mSystemHeader.mCursorOk = 3;
        mDatabase->mSoundList.push_back(SoundDef("cursor_cancel.ogg"));
        mDatabase->mSoundList.push_back(SoundDef("cursor_move.ogg"));
        mDatabase->mSoundList.push_back(SoundDef("cursor_ok.ogg"));
        mDatabase->mMusicList.push_back(Music("music_title.ogg",0));
        mDatabase->mMusicList.push_back(Music("music_map.ogg",0));
        mDatabase->initialise();

        // Register Sonetto Resources and Objects
        mStrManager = new STRManager();
        mFontManager = new FontManager();
        mWindowSkinManager = new WindowSkinManager();
        //mMapFileManager = new MapFileManager();

        // Create the object factory
        mPlaneFactory = new PlaneFactory();
        mTextElementFactory = new TextElementFactory();
        mWindowFactory = new WindowFactory();
        mTailedWindowFactory = new TailedWindowFactory();
        mSlimWindowFactory = new SlimWindowFactory();
        // Then register it.
        mOverlayMan->addOverlayElementFactory(mPlaneFactory);
        mOverlayMan->addOverlayElementFactory(mTextElementFactory);
        mOverlayMan->addOverlayElementFactory(mWindowFactory);
        mOverlayMan->addOverlayElementFactory(mTailedWindowFactory);
        mOverlayMan->addOverlayElementFactory(mSlimWindowFactory);

        mResourceMan->createResourceGroup("TEMP");

        ManualFontLoader * defaultFont = new ManualFontLoader();
        mResourceMan->declareResource("dfont.bin", "Font", "TEMP"/*, defaultFont*/);
        mResourceMan->addResourceLocation("font/","FileSystem","TEMP");
        mResourceMan->declareResource("dfont.dds", "Texture", "TEMP");

        ManualSTRLoader * defaultSTR = new ManualSTRLoader();
        mResourceMan->addResourceLocation("system/","FileSystem","TEMP");
        mResourceMan->declareResource("system.str", "STR", "TEMP"/*, defaultSTR*/);

        /*ManualMapFileLoader * defaultMap = new ManualMapFileLoader();
        mResourceMan->declareResource("system.map", "MapFile", "TEMP", defaultMap);*/

        ManualWindowSkinLoader * defaultWndSkin = new ManualWindowSkinLoader();

        mResourceMan->addResourceLocation("windowskin/","FileSystem","TEMP");
        mResourceMan->declareResource("windowskin_00_00.dds", "Texture", "TEMP");
        mResourceMan->declareResource("windowskin_00_01.dds", "Texture", "TEMP");
        mResourceMan->declareResource("windowskin_00_02.dds", "Texture", "TEMP");
        mResourceMan->declareResource("windowskin_00.bin", "WindowSkin", "TEMP"/*, defaultWndSkin*/);

        mResourceMan->initialiseResourceGroup("TEMP");

        /*MapFilePtr mapmap = mMapFileManager->load("system.map", "TEMP");
        MapFileSerializer exportmap;
        exportmap.exportMapFile(static_cast<MapFile *>(mapmap.getPointer()), "map/test_map/mapdata.dat");*/

        mDatabase->mSystemMessage = mStrManager->load("system.str", "TEMP");
        mDatabase->mGameFont = mFontManager->load("dfont.bin", "TEMP");
        WindowSkinPtr newWindowskin = mWindowSkinManager->load("windowskin_00.bin", "TEMP");
        mDatabase->mWindowSkinList.push_back(newWindowskin);

        initialiseFade();
        setupFade(1.0f/60.0f);
    }

    Kernel::~Kernel()
    {
        // Deinitialise Database
        if (mDatabase) {
            mDatabase->deinitialise();
            delete mDatabase;
            mDatabase = NULL;
        }

        // Deinitialise ScriptManager
        if (mScriptMan) {
            delete mScriptMan;
            mScriptMan = NULL;
        }

        // Deinitialise InputManager
        if (mInputMan) {
            delete mInputMan;
            mInputMan = NULL;
        }

        // Deinitialise AudioManager
        if (mAudioMan) {
            delete mAudioMan;
            mAudioMan = NULL;
        }

        // At last, delete the Ogre::Root
        if (mRoot) {
            delete mRoot;
            mRoot = NULL;
        }

        SDL_Quit();
    }

    void Kernel::initialise()
    {
        // Loads system sounds
        if (mDatabase->mSystemHeader.mCursorCancel != 0)
        {
            mAudioMan->loadSound(mDatabase->mSystemHeader.mCursorCancel);
        }

        if (mDatabase->mSystemHeader.mCursorMove != 0)
        {
            mAudioMan->loadSound(mDatabase->mSystemHeader.mCursorMove);
        }

        if (mDatabase->mSystemHeader.mCursorOk != 0)
        {
            mAudioMan->loadSound(mDatabase->mSystemHeader.mCursorOk);
        }
    }

    //-----------------------------------------------------------------------------
    bool Kernel::create()
    {
        if (isInitialised())
            SONETTO_THROW("Trying to create Kernel Singleton twice");

        mSingleton = new Kernel();
        if (mSingleton->mShutdown)
        {
            delete mSingleton;
            mSingleton = NULL;

            return false;
        }

        return true;
    }
    //-----------------------------------------------------------------------------
    bool Kernel::destroy()
    {
        if (!isInitialised())
            SONETTO_THROW("Trying to destroy unexistent Kernel Singleton");

        delete mSingleton;
        mSingleton = NULL;

        return true;
    }
    //-----------------------------------------------------------------------------
    int Kernel::run()
    {
        while (!mShutdown) {
            Ogre::ControllerValueRealPtr tmpFTV = Ogre::ControllerManager::getSingleton().getFrameTimeSource();
            mFrameTime = tmpFTV->getValue();
            SDL_Event evt;
            #ifdef DEBUG_BUILD

            if (SDL_GetKeyState(NULL)[SDLK_F8])
            {
                if(mGameSpeedUpSwitch == 0)
                {
                    mGameSpeed /= mGameSpeedScaleValue;
                }
                ++mGameSpeedUpSwitch;
            } else {
                mGameSpeedUpSwitch = 0;
            }

            if (SDL_GetKeyState(NULL)[SDLK_F6])
            {
                if(mGameSpeedDownSwitch == 0)
                {
                    mGameSpeed *= mGameSpeedScaleValue;
                }
                ++mGameSpeedDownSwitch;
            } else {
                mGameSpeedDownSwitch = 0;
            }

            if (SDL_GetKeyState(NULL)[SDLK_F7])
            {
                if(mGameNormalSpeedSwitch == 0)
                {
                    mGameSpeed = 1.0f;
                }
                ++mGameNormalSpeedSwitch;
            } else {
                mGameNormalSpeedSwitch = 0;
            }

            if (SDL_GetKeyState(NULL)[SDLK_F9])
            {
                if(mAspectRatioSwitch == 0)
                {
                    switch(mAspectRatioMode)
                    {
                        case 0:
                            mAspectRatio = ASPECT_RATIO_16_10;
                            mAspectRatioMode = 1;
                        break;
                        case 1:
                            mAspectRatio = ASPECT_RATIO_16_9;
                            mAspectRatioMode = 2;
                        break;
                        case 2:
                            mAspectRatio = ASPECT_RATIO_4_3;
                            mAspectRatioMode = 0;
                        break;
                    }
                }
                ++mAspectRatioSwitch;
            } else {
                mAspectRatioSwitch = 0;
            }

            mFrameTime *= mGameSpeed;
            //mFrameTime = 1.0f/85.0f; // Only to put the game to run on 85fps.
            #endif

            // Limit the minimum FPS to 15 (disable frameskip)
            if(mFrameTime > MINIMUM_FPS)
                mFrameTime = MINIMUM_FPS;

            // Small error check
            // A game cannot run without modules
            if (mModuleList.size() == 0)
                SONETTO_THROW("Module stack is empty");


            if (SDL_GetKeyState(NULL)[SDLK_LALT] && SDL_GetKeyState(NULL)[SDLK_F4])
            {
                shutdown();
            }

            if (SDL_GetKeyState(NULL)[SDLK_LALT] && SDL_GetKeyState(NULL)[SDLK_RETURN])
            {
                if(mWindow->isFullScreen() && (mFullScreenSwitchLock == 0))
                {
                    mWindow->setFullscreen(false, mScreenWidth, mScreenHeight);
                    mIsFullScreen = false;
                } else if(mFullScreenSwitchLock == 0) {
                    mWindow->setFullscreen(true, mScreenWidth, mScreenHeight);
                    mIsFullScreen = true;
                }
                ++mFullScreenSwitchLock;
            } else {
                mFullScreenSwitchLock = 0;
            }

#ifdef DEBUG_BUILD
            // Show Frame Status on Window Header (Debug Build Only)
            if(!mWindow->isFullScreen())
            {
                mFrameNumber += (1.0f/2.0f) * mFrameTime;
                if(mFrameNumber >= 1.0f)
                {
                setWindowCaption("Sonetto - Current FPS: "+Ogre::StringConverter::toString(mWindow->getLastFPS())+" | Frame Time: "+Ogre::StringConverter::toString(mFrameTime));
                mFrameNumber -= 1.0f;
                }
            }
#endif

                        // Pump events
            while (SDL_PollEvent(&evt))
            {
                if (evt.type == SDL_QUIT)
                {
                    // Shutdowns the game when asked to
                    shutdown();
                }
            }

            // Pump events
            while (SDL_PollEvent(&evt))
            {
                if (evt.type == SDL_QUIT)
                {
                    // Shutdowns the game when asked to
                    shutdown();
                }
            }

            // Stops game while window is deactivated (minimised)
            if (!(SDL_GetAppState() & SDL_APPACTIVE))
            {
                // Loop until the window gets activated again
                while (SDL_WaitEvent(&evt))
                {
                    if (SDL_GetAppState() & SDL_APPACTIVE)
                    {
                        // Window activated; break the loop and continue the game
                        break;
                    }
                }
            }

            // First update input
            mInputMan->update();

            // Update the FadeScreen
            updateFade();
            // Update stack's top Module
            mModuleList.top()->update(mFrameTime);

            // Update audio manager
            if (mAudioMan)
                mAudioMan->_update(mFrameTime);

            // Render !
            mRoot->renderOneFrame();

        }

        return 0;
    }
    //-----------------------------------------------------------------------------
    void Kernel::pushModule(Module *module,bool haltMode)
    {
        if(!module)
            SONETTO_THROW("The module pointer cannot be NULL");

        if (!mModuleList.empty()) {
            if (haltMode) {
                mModuleList.top()->halt(); // Put the current module to sleep
            } else {
                mModuleList.top()->exit(); // First, exit the current module.
                delete mModuleList.top();  // If it's going to change for real,
                                           // delete the current one.
                mModuleList.pop();         // Then remove it from the stack
            }
        }

        mModuleList.push(module);   // Insert the new module on the stack's top
        module->enter();            // Enter the new module
    }
    //-----------------------------------------------------------------------------
    bool Kernel::isWndFocused() const
    {
        return (SDL_GetAppState() & SDL_APPINPUTFOCUS);
    }
    //-----------------------------------------------------------------------------
    void Kernel::popModule() {
        if (mModuleList.size() > 0) {
            mModuleList.top()->exit();   // Cleanup
            delete mModuleList.top();    // Delete the module list
            mModuleList.pop();           // Then remove it from the stack

            mModuleList.top()->wakeup(); // Now wake up the previous module
        }
    }
    //-----------------------------------------------------------------------------
    bool Kernel::loadConfig(const Ogre::String& fname,Ogre::NameValuePairList &wndParamList)
    {
        Ogre::ConfigFile config;
        //config.loadDirect(fname, "\t:=", false);
        config.load(fname);

        // Video Configuration Section
        Ogre::String videoSectName = "video";
        mRoot->loadPlugin(config.getSetting("renderSystemPlugin",videoSectName));
        Ogre::RenderSystem* rs = mRoot->getRenderSystemByName(config.getSetting("renderSystem",videoSectName));

        Ogre::String aspectRatio = config.getSetting("aspectRatio",videoSectName);

        Ogre::String resolution = config.getSetting("screenResolution",videoSectName);

        size_t divpos = resolution.find_first_of('x');

        std::vector< String > res = Ogre::StringUtil::split(resolution, "x", 1);

        mScreenWidth = Ogre::StringConverter::parseUnsignedInt(res[0]);
        mScreenHeight = Ogre::StringConverter::parseUnsignedInt(res[1]);

        Ogre::String fullscreen = config.getSetting("fullScreen",videoSectName);

        if(fullscreen == "true")
        {
            mIsFullScreen = true;
        }
        if(fullscreen == "false")
        {
            mIsFullScreen = false;
        }

        wndParamList["vsync"] = config.getSetting("vsync",videoSectName);
        wndParamList["displayFrequency"] = config.getSetting("displayFrequency",videoSectName);
        wndParamList["colourDepth"] = config.getSetting("colourDepth",videoSectName);
        wndParamList["FSAA"] = config.getSetting("FSAA",videoSectName);

        aspectRatio = config.getSetting("aspectRatio",videoSectName);

        if(aspectRatio == "4_3")
        {
            mAspectRatio = ASPECT_RATIO_4_3;
            mAspectRatioMode = 0;
        }
        if(aspectRatio == "16_10")
        {
            mAspectRatio = ASPECT_RATIO_16_10;
            mAspectRatioMode = 1;
        }
        if(aspectRatio == "16_9")
        {
            mAspectRatio = ASPECT_RATIO_16_9;
            mAspectRatioMode = 2;
        }

        mScreen = SDL_SetVideoMode(mScreenWidth,mScreenHeight,Ogre::StringConverter::parseUnsignedInt(config.getSetting("colourDepth")),0);
        if (!rs)
        {
            // Unrecognised render system
            return false;
        }

        mRoot->setRenderSystem(rs);

        // Input Configuration Section
        Ogre::String inputSectName = "input";

        Ogre::String inputJoyAxisLeft = "JOY_AXIS_LEFT";
        Ogre::String inputJoyAxisRight = "JOY_AXIS_RIGHT";
        int numPlayers = Ogre::StringConverter::parseInt(config.getSetting("numPlayers",inputSectName));

        Ogre::String inputPlayerCnfName = "input player";
        for(int i = 0; i!= numPlayers; ++i)
        {
            PlayerInput *player = mInputMan->getPlayer(i);

            Ogre::String inputPCN_ID = inputPlayerCnfName + Ogre::StringConverter::toString(i+1);

            int useJoyID = Ogre::StringConverter::parseInt(config.getSetting("useJoystickID",inputPCN_ID));
            if(useJoyID != 0)
            {
                player->setJoystick(mInputMan->getJoystick(useJoyID));
            }
            if(Ogre::StringConverter::parseBool(config.getSetting("useAnalogAxisLeft",inputPCN_ID)))
            {
                bool invertOrder = Ogre::StringConverter::parseBool(config.getSetting("invertAnalogLeftAxisOrder",inputPCN_ID));
                bool invertX = Ogre::StringConverter::parseBool(config.getSetting("invertAnalogLeftAxisXPolarity",inputPCN_ID));
                bool invertY = Ogre::StringConverter::parseBool(config.getSetting("invertAnalogLeftAxisYPolarity",inputPCN_ID));
                Ogre::String analogLeftAxis      = config.getSetting("analogLeftAxis",inputPCN_ID);
                AXIS leftAxis;
                char inputInvert = 0;

                if(invertOrder)
                    inputInvert |= InputSource::INV_ORDER;

                if(invertX)
                    inputInvert |= InputSource::INV_X_POLARITY;

                if(invertY)
                    inputInvert |= InputSource::INV_Y_POLARITY;

                if(analogLeftAxis == inputJoyAxisLeft)
                {
                    leftAxis = AX_LEFT;
                }
                if(analogLeftAxis == inputJoyAxisRight)
                {
                    leftAxis = AX_RIGHT;
                }

                player->configAxis(Sonetto::AX_LEFT,Sonetto::InputSource(true,InputSource::IST_AXIS,leftAxis, inputInvert));

            } else {
                Ogre::String digitalLeftAxisUp      = config.getSetting("digitalLeftAxisUp",inputPCN_ID);
                Ogre::String digitalLeftAxisRight   = config.getSetting("digitalLeftAxisRight",inputPCN_ID);
                Ogre::String digitalLeftAxisDown    = config.getSetting("digitalLeftAxisDown",inputPCN_ID);
                Ogre::String digitalLeftAxisLeft    = config.getSetting("digitalLeftAxisLeft",inputPCN_ID);

                InputSource::TYPE digitalKeyType;
                size_t digitalKeyId;
                getKeyConfig(digitalLeftAxisUp,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_LEFT_UP, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
                getKeyConfig(digitalLeftAxisRight,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_LEFT_RIGHT, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
                getKeyConfig(digitalLeftAxisDown,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_LEFT_DOWN, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
                getKeyConfig(digitalLeftAxisLeft,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_LEFT_LEFT, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
            }

            if(Ogre::StringConverter::parseBool(config.getSetting("useAnalogAxisRight",inputPCN_ID)))
            {
                bool invertOrder = Ogre::StringConverter::parseBool(config.getSetting("invertAnalogRightAxisOrder",inputPCN_ID));
                bool invertX = Ogre::StringConverter::parseBool(config.getSetting("invertAnalogRightAxisXPolarity",inputPCN_ID));
                bool invertY = Ogre::StringConverter::parseBool(config.getSetting("invertAnalogRightAxisYPolarity",inputPCN_ID));
                Ogre::String analogRightAxis      = config.getSetting("analogRightAxis",inputPCN_ID);
                AXIS rightAxis;
                char inputInvert = 0;

                if(invertOrder)
                    inputInvert |= InputSource::INV_ORDER;

                if(invertX)
                    inputInvert |= InputSource::INV_X_POLARITY;

                if(invertY)
                    inputInvert |= InputSource::INV_Y_POLARITY;

                if(analogRightAxis == inputJoyAxisLeft)
                {
                    rightAxis = AX_LEFT;
                }
                if(analogRightAxis == inputJoyAxisRight)
                {
                    rightAxis = AX_RIGHT;
                }

                player->configAxis(Sonetto::AX_RIGHT,Sonetto::InputSource(true,InputSource::IST_AXIS, rightAxis, inputInvert));

            } else {
                Ogre::String digitalRightAxisUp      = config.getSetting("digitalRightAxisUp",inputPCN_ID);
                Ogre::String digitalRightAxisRight   = config.getSetting("digitalRightAxisRight",inputPCN_ID);
                Ogre::String digitalRightAxisDown    = config.getSetting("digitalRightAxisDown",inputPCN_ID);
                Ogre::String digitalRightAxisLeft    = config.getSetting("digitalRightAxisLeft",inputPCN_ID);

                InputSource::TYPE digitalKeyType;
                size_t digitalKeyId;
                getKeyConfig(digitalRightAxisUp,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_RIGHT_UP, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
                getKeyConfig(digitalRightAxisRight,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_RIGHT_RIGHT, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
                getKeyConfig(digitalRightAxisDown,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_RIGHT_DOWN, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
                getKeyConfig(digitalRightAxisLeft,digitalKeyType,digitalKeyId);
                player->configAxis(Sonetto::AXE_RIGHT_LEFT, Sonetto::InputSource(true, digitalKeyType, digitalKeyId));
            }
            InputSource::TYPE controlKeyType;
            size_t controlKeyId;

            getKeyConfig(config.getSetting("buttonTriangle",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_TRIANGLE,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonCircle",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_CIRCLE,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonCross",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_CROSS,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonSquare",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_SQUARE,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonL2",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_L2,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonR2",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_R2,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonL1",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_L1,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonR1",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_R1,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonStart",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_START,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonSelect",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_SELECT,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonL3",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_L3,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("buttonR3",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_R3,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("digitalPadUp",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_DPAD_UP,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("digitalPadRight",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_DPAD_RIGHT,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("digitalPadDown",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_DPAD_DOWN,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            getKeyConfig(config.getSetting("digitalPadLeft",inputPCN_ID),controlKeyType,controlKeyId);
            player->configBtn(Sonetto::BTN_DPAD_LEFT,Sonetto::InputSource(true,controlKeyType,controlKeyId));

            player->setEnabled(true);
        }

        // Audio Configuration
        Ogre::String audioSectName = "audio";
        Ogre::Real musicMasterVolume = Ogre::StringConverter::parseReal(config.getSetting("musicVolume",audioSectName));
        Ogre::Real soundMasterVolume = Ogre::StringConverter::parseReal(config.getSetting("soundVolume",audioSectName));
        mAudioMan->setMasterMusicVolume(musicMasterVolume);
        mAudioMan->setMasterSoundVolume(soundMasterVolume);

        return true;
    }
    void Kernel::getKeyConfig(const Ogre::String &key, InputSource::TYPE &type, size_t &keyid)
    {
        bool isKeyboard = Ogre::StringUtil::startsWith(key, INPUT_KEYBOARD_KEY, false);
        bool isJoystick = Ogre::StringUtil::startsWith(key, INPUT_JOYSTICK_KEY, false);

        if(!isKeyboard && !isJoystick)
        {
            type = InputSource::IST_KEY;
            keyid = 0;
            return;
        }

        Ogre::String keyTypeString;
        if(isKeyboard)
        {
            keyTypeString = INPUT_KEYBOARD_KEY;
            type = InputSource::IST_KEY;
        }

        if(isJoystick)
        {
            keyTypeString = INPUT_JOYSTICK_KEY;
            type = InputSource::IST_BUTTON;
        }

        Ogre::String keyvalue = key;

        keyvalue.erase(0, keyTypeString.size());

        keyid = Ogre::StringConverter::parseUnsignedInt(keyvalue);

    }
    //-----------------------------------------------------------------------------
    Viewport *Kernel::getViewport()
    {
        return mViewport;
    }
    //-----------------------------------------------------------------------------
    void Kernel::setViewport(Viewport *viewport)
    {
        mViewport = viewport;
    }
    //-----------------------------------------------------------------------------
    RenderWindow *Kernel::getRenderWindow()
    {
        return mWindow;
    }
    //-----------------------------------------------------------------------------
    Root *Kernel::getOgreRoot()
    {
        return mRoot;
    }
    void Kernel::setWindowCaption(const Ogre::String& capt)
    {

        SDL_WM_SetCaption(capt.c_str(),capt.c_str());
    }
    //-----------------------------------------------------------------------------
    InputManager *Kernel::getInputMan()
    {
        return mInputMan;
    }
    //-----------------------------------------------------------------------------
    /*AudioManager *Kernel::getAudioMan()
    {
        return mAudioMan;
    }*/
    //-----------------------------------------------------------------------------
    void Kernel::initialiseFade()
    {
        MaterialPtr mFadeMaterial = mMaterialMan->create("FADE_MATERIAL", "FADE_RESOURCES");
        mFadeMaterial->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
        mFadeOverlay = mOverlayMan->create("FADE_OVERLAY");
        mFadeOverlayPlane = static_cast<Ogre::OverlayContainer*>(mOverlayMan->createOverlayElement("Plane", "FADE_PLANE"));
        mFadeOverlayPlane->setPosition(0.0f,0.0f);
        mFadeOverlayPlane->setDimensions(1.0f,1.0f);
        mFadeOverlayPlane->setMaterialName("FADE_MATERIAL");
        mFadeOverlayPlane->setColour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));
        static_cast<Plane*>(mFadeOverlayPlane)->setScrMetricsMode(SMM_RELATIVE);
        mFadeOverlay->add2D(mFadeOverlayPlane);
        mFadeOverlay->setZOrder(600);
        mFadeOverlay->show();
    }
    //-----------------------------------------------------------------------------
    void Kernel::deinitialiseFade()
    {
        mFadeOverlay->clear();
    }
    FadeStatus Kernel::getFadeStatus()
    {
        return mFadeStatus;
    }
    void Kernel::setupFade(Ogre::Real fadespeed)
    {
        mFadeSpeed = fadespeed;
    }
    void Kernel::setFadeSpeed(Ogre::Real fadespeed)
    {
        mFadeSpeed = fadespeed;
    }
    void Kernel::startFade(bool fadeDir)
    {
        if( mFadeStatus == FS_IDLE_IN || mFadeStatus == FS_IDLE_OUT )
        {
            switch (fadeDir)
            {
                case false:
                    mFadeStatus = FS_FADE_ACTIVE_IN;
                    mFadeAlpha = 1.0f;
                break;
                case true:
                    mFadeStatus = FS_FADE_ACTIVE_OUT;
                    mFadeAlpha = 0.0f;
                    mFadeOverlayPlane->show();
                break;
            }
        }
    }
    void Kernel::updateFade()
    {
        switch (mFadeStatus)
        {
            case FS_FADE_ACTIVE_IN:
                mFadeAlpha -= mFadeSpeed * mFrameTime;
                if(mFadeAlpha <= 0.0f || mFadeAlpha > 1.0f)
                {
                    mFadeAlpha = 0.0f;
                    mFadeStatus = FS_IDLE_IN;
                    mFadeOverlayPlane->hide();
                }
                static_cast<Plane*>(mFadeOverlayPlane)->setAlpha(mFadeAlpha);
            break;
            case FS_FADE_ACTIVE_OUT:
                mFadeAlpha += mFadeSpeed * mFrameTime;
                if(mFadeAlpha >= 1.0f || mFadeAlpha < 0.0f)
                {
                    mFadeAlpha = 1.0f;
                    mFadeStatus = FS_IDLE_OUT;
                }
                static_cast<Plane*>(mFadeOverlayPlane)->setAlpha(mFadeAlpha);
            break;
            default:
                // Do Nothing
            break;
        }
    }
}; // namespace Sonetto
