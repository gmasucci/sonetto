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
using namespace std;
using namespace Ogre;

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
    Kernel::Kernel() : mShutdown(false), mScreen(NULL),
    mRoot(NULL), mWindow(NULL), mViewport(NULL), mLogMan(NULL),
    mOverlayMan(NULL), mResourceMan(NULL),mResMan(NULL)
    #ifdef _DEBUG
    , mDebugOverlay(NULL)
    #endif
    , mFrameTime(0.0f)
    , mFadeOverlay(NULL)
    , mFadeStatus(FS_IDLE_IN)
    , mFadeSpeed(60.0f)
    , mFadeAlpha(0.0f)

    {
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

        #ifdef _WINDOWS
            // Under Windows we just need to pass the window handle(HWND) as a string to
            // Ogre::Root::createRenderWindow() for Ogre to use such window for its drawings
            wndParamList["externalWindowHandle"] = StringConverter::toString((unsigned long)wmInfo.window);
        #else
            // Under *nixes, Ogre uses GLX for rendering. The syntax of
            // externalWindowHandle in this case is a bit different, but it
            // does essentially the same thing.
            string         wndHandleStr;
            string         dsp(&(DisplayString(wmInfo.info.x11.display)[1]));
            vector<String> tokens = StringUtil::split(dsp,".");

            wndHandleStr = StringConverter::toString((long)wmInfo.info.x11.display)+
                        ":"+tokens[1]+":"+
                        StringConverter::toString((long)wmInfo.info.x11.window);

            wndParamList["externalWindowHandle"] = wndHandleStr;
        #endif

        // Creates and configures Ogre root object, based on configuration
        // took from the configuration file
        mRoot = new Root("plugins.dlc","devlab.dlc","devlab.log");
        if (mRoot->showConfigDialog()) {
            // If returned true, the user clicked OK, so initialise
            mRoot->initialise(false);
            mWindow = mRoot->createRenderWindow("",640,480,false,&wndParamList);
        } else {
            delete mRoot;
            mRoot     = NULL;
            mShutdown = true;

            return;
        }

        // Get Ogre singletons for easier use
        mLogMan      = LogManager::getSingletonPtr();
        mOverlayMan  = OverlayManager::getSingletonPtr();
        mResourceMan = ResourceGroupManager::getSingletonPtr();
        mMaterialMan = MaterialManager::getSingletonPtr();

        // Create Audio Manager
//        mAudioMan = new AudioManager();
//        mAudioMan->initialise();

        // Create Input Manager
        mInputMan = new InputManager(4);

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
        // Create the Database
        mDatabase = new Database();
        mDatabase->mKernel = this;
        mDatabase->load("testdatabase.dat");
        mDatabase->initialise();

        mResourceMan->createResourceGroup("TEMP");

        ManualFontLoader * defaultFont = new ManualFontLoader();
        mResourceMan->declareResource("dfont.bin", "Font", "TEMP", defaultFont);
        mResourceMan->addResourceLocation("font/","FileSystem","TEMP");
        mResourceMan->declareResource("dfont_texture_512.dds", "Texture", "TEMP");

        ManualSTRLoader * defaultSTR = new ManualSTRLoader();
        mResourceMan->declareResource("system.str", "STR", "TEMP", defaultSTR);

        /*ManualMapFileLoader * defaultMap = new ManualMapFileLoader();
        mResourceMan->declareResource("system.map", "MapFile", "TEMP", defaultMap);*/

        ManualWindowSkinLoader * defaultWndSkin = new ManualWindowSkinLoader();

        mResourceMan->addResourceLocation("windowskin/","FileSystem","TEMP");
        mResourceMan->declareResource("windowskin_00_00.dds", "Texture", "TEMP");
        mResourceMan->declareResource("windowskin_00_01.dds", "Texture", "TEMP");
        mResourceMan->declareResource("windowskin_00_02.dds", "Texture", "TEMP");
        mResourceMan->declareResource("windowskin_00.bin", "WindowSkin", "TEMP", defaultWndSkin);

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
        // Deinitialise InputManager
        if (mInputMan) {
            delete mInputMan;
            mInputMan = NULL;
        }

        // Deinitialise AudioManager
/*        if (mAudioMan) {
            mAudioMan->deinitialise();

            delete mAudioMan;
            mAudioMan = NULL;
        }*/

        // At last, delete the Ogre::Root
        if (mRoot) {
            delete mRoot;
            mRoot = NULL;
        }

        SDL_Quit();
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
                if(mWindow->isFullScreen())
                {
                    mWindow->setFullscreen(false, 640, 480);
                } else {
                    mWindow->setFullscreen(true, 1024, 768);
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
/*            if (mAudioMan)
                mAudioMan->update();
*/
            // Render !
            mRoot->renderOneFrame();
        }

        return 0;
    }
    //-----------------------------------------------------------------------------
    void Kernel::pushModule(Module *module,bool haltMode)
    {
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
    std::map<std::string,std::string> Kernel::loadConfig(const char *fname)
    {
        std::ifstream           file;
        std::map<std::string,std::string> entries;

        // Tries to open specified file
        file.open(fname,std::ios::binary);
        if (!file.is_open())
            return std::map<std::string,std::string>();

        // Reads the file line by line
        while (!file.eof())
        {
            size_t separator;
            std::string line,entry,value;

            // Reads a line
            getline(file,line);

            // Removes eventual carriedge returns inserted by Windows text editors
            while (line.find('\r') != std::string::npos)
                line.replace(line.find('\r'),1,"");

            // Skips empty lines
            if (line.length() == 0)
                continue;

            // Skips first tabs and spaces
            line = line.substr(line.find_first_not_of(" \t"));

            // Skips comment lines
            if (line[0] == '#')
                continue;

            // Each entry line should follow the syntax: Entry=Value
            separator = line.find("=");
            if (count(line.begin(),line.end(),'=') != 1)
                return std::map<std::string,std::string>();

            entry = line.substr(0,separator);
            value = line.substr(separator+1);

            // Aborts if the entry already exists
            if (entries.find(entry) != entries.end())
                return std::map<std::string,std::string>();

            // Adds to entry map
            entries[entry] = value;
        }

        file.close();
        return entries;
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
