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

#ifndef SONETTO_DATABASE_H
#define SONETTO_DATABASE_H

#include "SonettoMain.h"
#include "SonettoWindowSkinManager.h"
#include "SonettoSTRManager.h"
#include "SonettoFontManager.h"
#include "SonettoSaveMap.h"
#include "SonettoMusicStream.h"
#include "SonettoSoundSource.h"
#include "SonettoTextElement.h"

namespace Sonetto {

    class Kernel;

    struct MapIndexData
    {
        std::string mType;
        std::string mFile;
    };

	struct SystemHeader
	{
		// Internal Identification Strings.
		std::string mGameTitle;
		std::string mGameVersion;
		std::string mGameDatabaseVersion;
		std::string mGameAuthor;

		// In-Game Menu/System Sounds.
		size_t mCursorOk;
		size_t mCursorCancel;
		size_t mCursorMove;
		size_t mUseItem;
		size_t mSystemError;

		// Game/System Messages.
		size_t mMsgOk;
		size_t mMsgCancel;
		size_t mMsgYes;
		size_t mMsgNo;
		size_t mMsgPressStart;
		size_t mMsgNewGame;
		size_t mMsgLoadGame;
		size_t mMsgExitGame;
		size_t mMsgOptions;
		size_t mMsgLoadMsg;
		size_t mMsgLoadScrSlot;
		size_t mMsgLoadMsgOpt;
		size_t mMsgLoading;
		size_t mMsgLoadComplete;
		size_t mMsgSaveMsg;
		size_t mMsgSaveScrSlot;
		size_t mMsgSaveMsgOpt;
		size_t mMsgSaving;
		size_t mMsgSaveComplete;
		size_t mMsgOptionsTitle;
		size_t mMsgOptsWinSkinID; // Some games allow multiple windowskins.

		// Default WindowSkin;
		size_t mDefaultWindowSkin;
		// Default Font
		size_t mDefaultFont;
	};
    class SONETTO_EXPORT Database
    {
    public:
        Database();
        ~Database();

        /** @brief Initialise Game's Database
         *
         *  This function will load, create and manage the game's database.
         */
        bool initialise();
        /** Deinitialise database. */
        bool deinitialise();
        /** Load a database file from disk. */
        bool load(std::string filename);
        /** Save the current database to a file. */
        bool save(std::string filename);

    //private:
        Kernel * mKernel;
    	// System configuration.
    	SaveMap * mSaveMap;
    	SystemHeader mSystemHeader;
    	std::vector<WindowSkinPtr> mWindowSkinList;
    	FontPtr mGameFont;
    	STRDataPtr mSystemMessage;
    	IconSet mTextIconSet;
    	// Temporary "Map To Load" memory area.
    	// This area is set up by future scripts or commands
    	// used to change from one map to another.
    	// The function will always read from here to set its "destiny".
    	size_t mDestMapID;
    	// Temporary Map Start Position.
    	float mPlayerPosX;
    	float mPlayerPosY;
    	float mPlayerPosZ;
    	// Temporary Map Start Rotation. (only Y axis is used for now)
    	float mPlayerRotX; // Unused
    	float mPlayerRotY;
    	float mPlayerRotZ; // Unused
    	float mPlayerRotW; // Unused

    	// Temporary map list.
    	std::map<size_t, MapIndexData> mMapList;

    	/** Vector of Music structures

            This is a vector of Music structures. The indexes of the Musics
            inside this vector represent them in AudioManager::playBGM() and
            AudioManager::playME().
        @see
            Music
        */
        MusicVector mMusicList;

    	/** Vector of SoundDef structures

            This is a vector of SoundDef structures. The indexes of the SoundDefs
            inside this vector represent them in methods like AudioManager::loadSound(),
            AudioManager::createSound(), AudioManager::playSound(), etc.
        @see
            SoundDef
        */
        SoundDefVector mSoundList;
    };
} // namespace

#endif // SONETTO_DATABASE_H
