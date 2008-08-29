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

#ifndef SONETTO_Kernel_H
#define SONETTO_Kernel_H

#include "SonettoMain.h"

#include <fstream>
#include <stack>
#include <Ogre.h>
#include <OgreVector2.h>

//#include "SonettoAudioManager.h"
#include "SonettoDatabase.h"
#include "SonettoInputManager.h"
#include "SonettoModule.h"
#include "SonettoPlaneFactory.h"
#include "SonettoRARC.h"
#include "SonettoSTRManager.h"
#include "SonettoFontSerializer.h"
#include "SonettoFontManager.h"
#include "SonettoTextFactory.h"
#include "SonettoWindowSkinSerializer.h"
#include "SonettoWindowSkinManager.h"
#include "SonettoWindowFactory.h"
#include "SonettoGauge.h"
#include "SonettoCounter.h"

#include <SDL/SDL_video.h>

namespace Sonetto {
    class Module; // Forward declaration
}

namespace Sonetto
{

    enum FadeStatus
    {
        FS_IDLE_IN,
        FS_IDLE_OUT,
        FS_FADE_ACTIVE_IN,
        FS_FADE_ACTIVE_OUT
    };

    class SONETTO_EXPORT Kernel
    {
    public:
        friend class InputManager;
        friend class AudioManager;

        /// @brief Return the singleton pointer to this class.
        inline static Kernel *get() { return mSingleton; }

        /// @brief Check if the Kernel is initialised.
        inline static bool isInitialised() { return mSingleton; }

        /** @brief Create a Sonetto singleton and initialises Ogre, SDL, and other Sonetto dependencies
        @return
            Whether it was successful or not.
        */
        static bool create();

        /// @brief Deinitialise everything initialised before by initialise().
        static bool destroy();

        /// @brief Start the game's Main Loop
        int run();

        /** @brief Add a module in the module stack
        @remarks
            The module in the top of the stack is
            Allways the active module, basically,
            if you put a new module in the stack,
            It'll become the active module.
        @param
            pModule The Module pointer to add in the stack
        @param
            haldMode If it's true, the old module will be kept in memory
            and the new module will become the active one. If it's false, then
            the old module will be deleted and the new module will become the active module.
        */
        void pushModule(Module *pModule,bool haltMode = false);

        /// @brief Remove a module from the top of the stack.
        void popModule();

        /// @brief Shutdown / Close the game.
        void shutdown() { mShutdown = true; }

        /// @brief Checks whether window is focused or not
        bool isWndFocused() const;

        Ogre::Viewport     *getViewport();
        void                setViewport(Ogre::Viewport * viewport);
        Ogre::RenderWindow *getRenderWindow();
        Ogre::Root         *getOgreRoot();

        void setWindowCaption(const Ogre::String& capt);

        InputManager *getInputMan();
//        AudioManager *getAudioMan();

        /// @brief Load and parse the configuration file.
        std::map<std::string,std::string> loadConfig(const char *fname);
    private:
        /// @brief Constructor.
        Kernel();

        /// @brief Destructor.
        ~Kernel();
    public:
        /// @brief Flag indicating the program will be closed.
        bool mShutdown;

        /// @brief Static pointer to this class.
        static Kernel *mSingleton;

        /// @brief Module List stack (holds the active modules).
        std::stack<Module *> mModuleList;

        // SDL pointers.
        SDL_Surface *mScreen;

        // Ogre pointers.
        Ogre::Root                   *mRoot; // Ogre Root pointer.
        Ogre::RenderWindow           *mWindow; // Ogre Render Window.
        Ogre::Viewport               *mViewport; // Over Viewport.
        Ogre::LogManager             *mLogMan; // Ogre Log Manager.
        Ogre::OverlayManager         *mOverlayMan; // Ogre Overlay Manager.
        Ogre::ResourceGroupManager   *mResourceMan; // Ogre Resource Group Manager.
        Ogre::ResourceManager        *mResMan; // Ogre Resource Manager.
        Ogre::MaterialManager        *mMaterialMan; // Ogre Material Manager.

        // Debug Stuff.
        #ifdef _DEBUG
        Ogre::Overlay                *mDebugOverlay; // Sonetto Debug Overlay.
        #endif

        // Sonetto Pointers.
//        AudioManager                 *mAudioMan; // Sonetto Audio Manager.
        InputManager                 *mInputMan; // Sonetto Input Manager.

        // Sonetto Resources and Objects.
        STRManager                  *mStrManager;
        FontManager                 *mFontManager;
        WindowSkinManager           *mWindowSkinManager;

        PlaneFactory                *mPlaneFactory;
        TextElementFactory          *mTextElementFactory;
        WindowFactory               *mWindowFactory;
        TailedWindowFactory         *mTailedWindowFactory;
        SlimWindowFactory           *mSlimWindowFactory;

        // Sonetto Database
        Database                    *mDatabase;

        Ogre::Real                   mFrameTime;

        // Sonetto Screen Fade effect data
        Ogre::Overlay               *mFadeOverlay;
        Ogre::OverlayContainer      *mFadeOverlayPlane;

        Ogre::MaterialPtr            mFadeMaterial;

        FadeStatus mFadeStatus;
        Ogre::Real mFadeSpeed;
        Ogre::Real mFadeAlpha;

        void initialiseFade();
        void deinitialiseFade();
        FadeStatus getFadeStatus();
        void setupFade(Ogre::Real fadespeed);
        void setFadeSpeed (Ogre::Real fadespeed);
        void startFade(bool fadeDir);
        void updateFade();

    };

    class ManualFontLoader : public Ogre::ManualResourceLoader
    {
    public:
        ManualFontLoader() {}
        virtual ~ManualFontLoader() {}

        void loadResource(Ogre::Resource *resource)
        {
            Font *fontfile = static_cast<Font *>(resource);
            fontfile->font_blend_mode = Sonetto::FBM_TRANSPARENT_ALPHA;
            fontfile->font_alpha_reject_settings = Sonetto::SCF_ALWAYS_PASS;
            fontfile->alpha_rejection = 250;
            fontfile->font_filtering_mode = Sonetto::FT_LINEAR;

            fontfile->setUpFont(16,16,0.5f,"dfont_texture_512.dds");

            fontfile->setSpaceWidth(0x21, 12.0f / 32.0f);
            fontfile->setSpaceWidth(0x22, 14.0f / 32.0f);
            fontfile->setSpaceWidth(0x23, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x24, 24.0f / 32.0f);
            fontfile->setSpaceWidth(0x25, 27.0f / 32.0f);
            fontfile->setSpaceWidth(0x26, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x27, 11.0f / 32.0f);
            fontfile->setSpaceWidth(0x28, 14.0f / 32.0f);
            fontfile->setSpaceWidth(0x29, 14.0f / 32.0f);
            fontfile->setSpaceWidth(0x2A, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x2B, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x2C, 10.0f / 32.0f);
            fontfile->setSpaceWidth(0x2D, 13.0f / 32.0f);
            fontfile->setSpaceWidth(0x2E, 11.0f / 32.0f);
            fontfile->setSpaceWidth(0x2F, 19.0f / 32.0f);

            fontfile->setSpaceWidth(0x30, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x31, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x32, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x33, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x34, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x35, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x36, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x37, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x38, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x39, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x3A, 11.0f / 32.0f);
            fontfile->setSpaceWidth(0x3B, 11.0f / 32.0f);
            fontfile->setSpaceWidth(0x3C, 18.0f / 32.0f);
            fontfile->setSpaceWidth(0x3D, 18.0f / 32.0f);
            fontfile->setSpaceWidth(0x3E, 18.0f / 32.0f);
            fontfile->setSpaceWidth(0x3F, 20.0f / 32.0f);

            fontfile->setSpaceWidth(0x40, 29.0f / 32.0f);
            fontfile->setSpaceWidth(0x41, 27.0f / 32.0f);
            fontfile->setSpaceWidth(0x42, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x43, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x44, 26.0f / 32.0f);
            fontfile->setSpaceWidth(0x45, 26.0f / 32.0f);
            fontfile->setSpaceWidth(0x46, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x47, 28.0f / 32.0f);
            fontfile->setSpaceWidth(0x48, 26.0f / 32.0f);
            fontfile->setSpaceWidth(0x49, 12.0f / 32.0f);
            fontfile->setSpaceWidth(0x4A, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x4B, 27.0f / 32.0f);
            fontfile->setSpaceWidth(0x4C, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x4D, 30.0f / 32.0f);
            fontfile->setSpaceWidth(0x4E, 30.0f / 32.0f);
            fontfile->setSpaceWidth(0x4F, 27.0f / 32.0f);

            fontfile->setSpaceWidth(0x50, 24.0f / 32.0f);
            fontfile->setSpaceWidth(0x51, 28.0f / 32.0f);
            fontfile->setSpaceWidth(0x52, 28.0f / 32.0f);
            fontfile->setSpaceWidth(0x53, 24.0f / 32.0f);
            fontfile->setSpaceWidth(0x54, 26.0f / 32.0f);
            fontfile->setSpaceWidth(0x55, 28.0f / 32.0f);
            fontfile->setSpaceWidth(0x56, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x57, 27.0f / 32.0f);
            fontfile->setSpaceWidth(0x58, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x59, 26.0f / 32.0f);
            fontfile->setSpaceWidth(0x5A, 26.0f / 32.0f);
            fontfile->setSpaceWidth(0x5B, 15.0f / 32.0f);
            fontfile->setSpaceWidth(0x5C, 27.0f / 32.0f);
            fontfile->setSpaceWidth(0x5D, 14.0f / 32.0f);
            fontfile->setSpaceWidth(0x5E, 16.0f / 32.0f);
            fontfile->setSpaceWidth(0x5F, 19.0f / 32.0f);

            fontfile->setSpaceWidth(0x60, 14.0f / 32.0f);
            fontfile->setSpaceWidth(0x61, 24.0f / 32.0f);
            fontfile->setSpaceWidth(0x62, 23.0f / 32.0f);
            fontfile->setSpaceWidth(0x63, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x64, 24.0f / 32.0f);
            fontfile->setSpaceWidth(0x65, 22.0f / 32.0f);
            fontfile->setSpaceWidth(0x66, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x67, 20.0f / 32.0f);
            fontfile->setSpaceWidth(0x68, 23.0f / 32.0f);
            fontfile->setSpaceWidth(0x69, 13.0f / 32.0f);
            fontfile->setSpaceWidth(0x6A, 16.0f / 32.0f);
            fontfile->setSpaceWidth(0x6B, 22.0f / 32.0f);
            fontfile->setSpaceWidth(0x6C, 12.0f / 32.0f);
            fontfile->setSpaceWidth(0x6D, 29.0f / 32.0f);
            fontfile->setSpaceWidth(0x6E, 23.0f / 32.0f);
            fontfile->setSpaceWidth(0x6F, 22.0f / 32.0f);

            fontfile->setSpaceWidth(0x70, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x71, 20.0f / 32.0f);
            fontfile->setSpaceWidth(0x72, 19.0f / 32.0f);
            fontfile->setSpaceWidth(0x73, 19.0f / 32.0f);
            fontfile->setSpaceWidth(0x74, 22.0f / 32.0f);
            fontfile->setSpaceWidth(0x75, 25.0f / 32.0f);
            fontfile->setSpaceWidth(0x76, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x77, 23.0f / 32.0f);
            fontfile->setSpaceWidth(0x78, 21.0f / 32.0f);
            fontfile->setSpaceWidth(0x79, 22.0f / 32.0f);
            fontfile->setSpaceWidth(0x7A, 22.0f / 32.0f);
            fontfile->setSpaceWidth(0x7B, 12.0f / 32.0f);
            fontfile->setSpaceWidth(0x7C, 10.0f / 32.0f);
            fontfile->setSpaceWidth(0x7D, 12.0f / 32.0f);
            fontfile->setSpaceWidth(0x7E, 19.0f / 32.0f);
            /*fontfile->setSpaceWidth(0x7F, 0.0f / 32.0f);

            fontfile->setSpaceWidth(0x60, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x61, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x62, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x63, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x64, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x65, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x66, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x67, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x68, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x69, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x6A, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x6B, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x6C, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x6D, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x6E, 0.0f / 32.0f);
            fontfile->setSpaceWidth(0x6F, 0.0f / 32.0f);*/

            ColourValue color1 = ColourValue(1.0f,1.0f,1.0f,1.0f);
            ColourValue color2 = ColourValue(1.0f, 170.0f/255.0f, 0.0f, 1.0f);
            ColourValue color3 = ColourValue(1.0f,0.9f,0.5f,1.0f);
            ColourValue color4 = ColourValue(170 / 255.0f, 1.0f, 244 / 255.0f,1.0f);

            fontfile->mColorList.push_back(color1);
            fontfile->mColorList.push_back(color2);
            fontfile->mColorList.push_back(color3);
            fontfile->mColorList.push_back(color4);
        }
    };

    class ManualSTRLoader : public Ogre::ManualResourceLoader
    {
    public:
        ManualSTRLoader() {}
        virtual ~ManualSTRLoader() {}

        void loadResource(Ogre::Resource *resource)
        {
            STRData *strdata = static_cast<STRData *>(resource);
            Ogre::String msg0 = "Sonetto Module Test.";
            Ogre::String msg1 = "Press [O] to select the desired Module.\nWhile insde the module,\npress [Esc] on the keyboard\nto return to the main menu.";
            Ogre::String msg2 = "Map Module";
            Ogre::String msg3 = "Battle Module";
            Ogre::String msg4 = "Menu Module";
            Ogre::String msg5 = "Field Module";
            Ogre::String msg6 = "Cancel";
            strdata->insertMessage(msg0);
            strdata->insertMessage(msg1);
            strdata->insertMessage(msg2);
            strdata->insertMessage(msg3);
            strdata->insertMessage(msg4);
            strdata->insertMessage(msg5);
            strdata->insertMessage(msg6);
        }
    };

    class ManualWindowSkinLoader : public Ogre::ManualResourceLoader
    {
    public:
        ManualWindowSkinLoader() {}
        virtual ~ManualWindowSkinLoader() {}

        void loadResource(Ogre::Resource *resource)
        {
            WindowSkin *windowskin = static_cast<WindowSkin *>(resource);
            windowskin->mWindowBgTextureName = "windowskin_00_02.dds";
            windowskin->mWindowMaskTextureName = "windowskin_00_01.dds";
            windowskin->mBorderTextureName = "windowskin_00_00.dds";
            windowskin->mCursorTextureName = "windowskin_00_00.dds";
            windowskin->mCursorLineTextureName = "windowskin_00_00.dds";

            float tempS = 1.0f/8.0f;

            windowskin->mWinTexCoord.top_left.left         = 0.0f * tempS;
            windowskin->mWinTexCoord.top_left.right        = 1.0f * tempS;
            windowskin->mWinTexCoord.top_left.top          = 5.0f * tempS;
            windowskin->mWinTexCoord.top_left.bottom       = 6.0f * tempS;

            windowskin->mWinTexCoord.top.left              = 1.0f * tempS;
            windowskin->mWinTexCoord.top.right             = 2.0f * tempS;
            windowskin->mWinTexCoord.top.top               = 5.0f * tempS;
            windowskin->mWinTexCoord.top.bottom            = 6.0f * tempS;

            windowskin->mWinTexCoord.top_right.left        = 2.0f * tempS;
            windowskin->mWinTexCoord.top_right.right       = 3.0f * tempS;
            windowskin->mWinTexCoord.top_right.top         = 5.0f * tempS;
            windowskin->mWinTexCoord.top_right.bottom      = 6.0f * tempS;

            windowskin->mWinTexCoord.left.left             = 0.0f * tempS;
            windowskin->mWinTexCoord.left.right            = 1.0f * tempS;
            windowskin->mWinTexCoord.left.top              = 6.0f * tempS;
            windowskin->mWinTexCoord.left.bottom           = 7.0f * tempS;

            windowskin->mWinTexCoord.center.left           = 1.0f * tempS;
            windowskin->mWinTexCoord.center.right          = 2.0f * tempS;
            windowskin->mWinTexCoord.center.top            = 6.0f * tempS;
            windowskin->mWinTexCoord.center.bottom         = 7.0f * tempS;

            windowskin->mWinTexCoord.right.left            = 2.0f * tempS;
            windowskin->mWinTexCoord.right.right           = 3.0f * tempS;
            windowskin->mWinTexCoord.right.top             = 6.0f * tempS;
            windowskin->mWinTexCoord.right.bottom          = 7.0f * tempS;

            windowskin->mWinTexCoord.bottom_left.left      = 0.0f * tempS;
            windowskin->mWinTexCoord.bottom_left.right     = 1.0f * tempS;
            windowskin->mWinTexCoord.bottom_left.top       = 7.0f * tempS;
            windowskin->mWinTexCoord.bottom_left.bottom    = 8.0f * tempS;

            windowskin->mWinTexCoord.bottom.left           = 1.0f * tempS;
            windowskin->mWinTexCoord.bottom.right          = 2.0f * tempS;
            windowskin->mWinTexCoord.bottom.top            = 7.0f * tempS;
            windowskin->mWinTexCoord.bottom.bottom         = 8.0f * tempS;

            windowskin->mWinTexCoord.bottom_right.left     = 2.0f * tempS;
            windowskin->mWinTexCoord.bottom_right.right    = 3.0f * tempS;
            windowskin->mWinTexCoord.bottom_right.top      = 7.0f * tempS;
            windowskin->mWinTexCoord.bottom_right.bottom   = 8.0f * tempS;

            windowskin->mWinTexCoord.cursor.left           = 3.0f * tempS;
            windowskin->mWinTexCoord.cursor.right          = 4.0f * tempS;
            windowskin->mWinTexCoord.cursor.top            = 7.0f * tempS;
            windowskin->mWinTexCoord.cursor.bottom         = 8.0f * tempS;

            windowskin->mWinTexCoord.cursor_line.left      = 4.0f * tempS;
            windowskin->mWinTexCoord.cursor_line.right     = 8.0f * tempS;
            windowskin->mWinTexCoord.cursor_line.top       = 7.0f * tempS;
            windowskin->mWinTexCoord.cursor_line.bottom    = 8.0f * tempS;

            windowskin->mBorderDimensions = Ogre::Vector2(32/480.0f,32/480.0f);
            windowskin->mTileMode = false;
            windowskin->mTiling = Ogre::Vector2(1.0,1.0);

            windowskin->mMenuColor = Ogre::ColourValue(120/255.0f,75/255.0f,60/255.0f,0.9f);
            windowskin->mMenuBorderColor = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            windowskin->mMainColor = Ogre::ColourValue(120/255.0f,75/255.0f,60/255.0f,0.9f);
            windowskin->mMainBorderColor = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            windowskin->mEnemyColor = Ogre::ColourValue(120/255.0f,75/255.0f,60/255.0f,0.9f);
            windowskin->mEnemyBorderColor = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            windowskin->mSystemColor = Ogre::ColourValue(120/255.0f,75/255.0f,60/255.0f,0.9f);
            windowskin->mSystemBorderColor = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);

            windowskin->mMenuBgTopLeft = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            windowskin->mMenuBgTopRight = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            windowskin->mMenuBgBottomLeft = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            windowskin->mMenuBgBottomRight = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);

            windowskin->mCursorColor = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);

            windowskin->mCursorLineColor = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);

            windowskin->deleteMaterial();
            windowskin->createMaterial();
        }
    };
}; // namespace Sonetto

#endif // __SONETTO_Kernel__
