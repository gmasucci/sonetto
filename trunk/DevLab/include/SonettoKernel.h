/*-----------------------------------------------------------------------------
This source file is part of Sonetto RPG Engine.

Copyright (C) 2007,2008 Arthur Carvalho de Souza Lima, Guilherme Pr� Vieira


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

#ifndef SONETTO_KERNEL_H
#define SONETTO_KERNEL_H

#include <stack>
#include <Ogre.h>
#include "SonettoException.h"
#include "SonettoModule.h"
#include "SonettoAudioManager.h"
#include "SonettoInputManager.h"
#include "SonettoWindowSkinManager.h"
#include "SonettoFontManager.h"
#include "SonettoTextElement.h"
#include "SonettoTextFactory.h"
#include "SonettoWindowFactory.h"

namespace Sonetto {
    class TempWindowSkinLoader : public Ogre::ManualResourceLoader {
    public:
        TempWindowSkinLoader() {}
        virtual ~TempWindowSkinLoader() {}

        void loadResource(Ogre::Resource * resource) {
            WindowSkin *ws = static_cast<WindowSkin *>(resource);

            ws->mWindowMaskTextureName = "windowskin_mask3.dds";
            ws->mWindowBgTextureName = "windowskin_bg.dds";
            ws->mBorderTextureName = "windowskin_border3.dds";

            ws->mWinTexCoord.slim_left.left = (4/256.0f);
            ws->mWinTexCoord.slim_left.right = (20/256.0f);
            ws->mWinTexCoord.slim_left.top = (160/256.0f);
            ws->mWinTexCoord.slim_left.bottom = (188/256.0f);

            ws->mWinTexCoord.slim_center.left = (20/256.0f);
            ws->mWinTexCoord.slim_center.right = (61/256.0f);
            ws->mWinTexCoord.slim_center.top = (160/256.0f);
            ws->mWinTexCoord.slim_center.bottom = (188/256.0f);

            ws->mWinTexCoord.slim_right.left = (60/256.0f);
            ws->mWinTexCoord.slim_right.right = (75/256.0f);
            ws->mWinTexCoord.slim_right.top = (160/256.0f);
            ws->mWinTexCoord.slim_right.bottom = (188/256.0f);


            ws->mWinTexCoord.top_left.left = (4/256.0f);
            ws->mWinTexCoord.top_left.right = (28/256.0f);
            ws->mWinTexCoord.top_left.top = (196/256.0f);
            ws->mWinTexCoord.top_left.bottom = (220/256.0f);

            ws->mWinTexCoord.top.left = (100/256.0f);
            ws->mWinTexCoord.top.right = (124/256.0f);
            ws->mWinTexCoord.top.top = (196/256.0f);
            ws->mWinTexCoord.top.bottom = (220/256.0f);

            ws->mWinTexCoord.top_right.left = (36/256.0f);
            ws->mWinTexCoord.top_right.right = (60/256.0f);
            ws->mWinTexCoord.top_right.top = (196/256.0f);
            ws->mWinTexCoord.top_right.bottom = (220/256.0f);

            ws->mWinTexCoord.left.left = (68/256.0f);
            ws->mWinTexCoord.left.right = (92/256.0f);
            ws->mWinTexCoord.left.top = (196/256.0f);
            ws->mWinTexCoord.left.bottom = (220/256.0f);

            ws->mWinTexCoord.center.left = (196/256.0f);
            ws->mWinTexCoord.center.right = (220/256.0f);
            ws->mWinTexCoord.center.top = (228/256.0f);
            ws->mWinTexCoord.center.bottom = (252/256.0f);

            ws->mWinTexCoord.right.left = (68/256.0f);
            ws->mWinTexCoord.right.right = (92/256.0f);
            ws->mWinTexCoord.right.top = (228/256.0f);
            ws->mWinTexCoord.right.bottom = (252/256.0f);

            ws->mWinTexCoord.bottom_left.left = (4/256.0f);
            ws->mWinTexCoord.bottom_left.right = (28/256.0f);
            ws->mWinTexCoord.bottom_left.top = (228/256.0f);
            ws->mWinTexCoord.bottom_left.bottom = (252/256.0f);

            ws->mWinTexCoord.bottom.left = (100/256.0f);
            ws->mWinTexCoord.bottom.right = (124/256.0f);
            ws->mWinTexCoord.bottom.top = (228/256.0f);
            ws->mWinTexCoord.bottom.bottom = (252/256.0f);

            ws->mWinTexCoord.bottom_right.left = (36/256.0f);
            ws->mWinTexCoord.bottom_right.right = (60/256.0f);
            ws->mWinTexCoord.bottom_right.top = (228/256.0f);
            ws->mWinTexCoord.bottom_right.bottom = (252/256.0f);

            ws->mWinTexCoord.tail_up.left = (132/256.0f);
            ws->mWinTexCoord.tail_up.right = (156/256.0f);
            ws->mWinTexCoord.tail_up.top = (204/256.0f);
            ws->mWinTexCoord.tail_up.bottom = (252/256.0f);

            ws->mWinTexCoord.tail_down.left = (164/256.0f);
            ws->mWinTexCoord.tail_down.right = (187/256.0f);
            ws->mWinTexCoord.tail_down.top = (204/256.0f);
            ws->mWinTexCoord.tail_down.bottom = (252/256.0f);

            ws->mBorderDimensions = Ogre::Vector2(24/480.0f,24/480.0f);

            ws->mTileMode = true;
            ws->mTiling = Ogre::Vector2(7.5f,7.5f);

            ws->mMenuColor          = Ogre::ColourValue(28/255.0f,104/255.0f,120/255.0f,216/255.0f);
            ws->mMenuBorderColor    = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            ws->mMainColor          = Ogre::ColourValue(0/255.0f,72/255.0f,144/255.0f,232/255.0f);
            ws->mMainBorderColor    = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            ws->mEnemyColor         = Ogre::ColourValue(136/255.0f,40/255.0f,40/255.0f,232/255.0f);
            ws->mEnemyBorderColor   = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
            ws->mSystemColor        = Ogre::ColourValue(24/255.0f,88/255.0f,80/255.0f,232/255.0f);
            ws->mSystemBorderColor  = Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);

            ws->mMenuBgTopLeft      = Ogre::ColourValue(24/255.0f,48/255.0f,48/255.0f,216/255.0f);
            ws->mMenuBgTopRight     = Ogre::ColourValue(24/255.0f,48/255.0f,48/255.0f,216/255.0f);
            ws->mMenuBgBottomLeft   = Ogre::ColourValue(32/255.0f,80/255.0f,88/255.0f,216/255.0f);
            ws->mMenuBgBottomRight  = Ogre::ColourValue(32/255.0f,80/255.0f,88/255.0f,216/255.0f);

            ws->mCursorLineColor    = Ogre::ColourValue(112/255.0f,144/255.0f,24/255.0f,128/255.0f);

            ws->deleteMaterial();
            ws->createMaterial();
        }
    };

    class Kernel {
    public:
        inline static Kernel *getSingleton()  { return mSingleton; }
        inline static bool    isInitialised() { return mSingleton; }

        static bool           initialise();
        static bool           deinitialise();

        int  run();
        void pushModule(Module *module,bool haltMode = false);
        void popModule();
        void shutdown() { mShutDown = true; }

        inline Ogre::Root         *getOgreRoot()     { return mRoot;     }
        inline Ogre::RenderWindow *getRenderWindow() { return mWindow;   }
        inline Ogre::Viewport     *getViewport()     { return mViewport; }
        inline Ogre::LogManager   *getLogMan()       { return mLogMan;   }
        inline InputManager       *getInputMan()     { return mInputMan; }
        inline AudioManager       *getAudioMan()     { return mAudioMan; }

        inline void setViewport(Ogre::Viewport *viewport) { mViewport = viewport; }

    private:
        friend class InputManager;
        friend class AudioManager;

        Kernel();
        ~Kernel() {}

        bool _initialise();
        bool _deinitialise();

        static Kernel *mSingleton;

        bool mShutDown;

        std::stack<Module *>          mModuleList;

        Ogre::Root                   *mRoot;
        Ogre::RenderWindow           *mWindow;
        Ogre::Viewport               *mViewport;
        Ogre::LogManager             *mLogMan;
        Ogre::OverlayManager         *mOverlayMan;
        Ogre::ResourceGroupManager   *mResourceMan;
        Ogre::ResourceManager        *mResMan;

        AudioManager        *mAudioMan;
        InputManager        *mInputMan;
        FontManager         *mFontMan;
        TextElementFactory  *mTextElementFactory;
        WindowFactory       *mWindowFactory;
        SlimWindowFactory   *mSWindowFactory;
        TailedWindowFactory *mTWindowFactory;
        WindowSkinManager   *mWSkinMan;

        // Temporary stuff
        FontPtr                  mMainFont;
        WindowSkinPtr            mWSkinPtr;

        Ogre::Overlay           *mDebugOverlay;
        Ogre::OverlayContainer  *mDebugOverlayContainer;
        TextElement             *mDebugText;
        TextElement             *mDebugErrorText;
        TempWindowSkinLoader    *mTmpWinSkinLoader;
    };
}; // namespace

#endif
