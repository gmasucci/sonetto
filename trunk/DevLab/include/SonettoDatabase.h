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

namespace Sonetto {
    class Database {
    public:
        Database();
        ~Database();

        /** @brief Initialise the Game's Database
         *
         *  This function will load, create and manage the game's database.
         */
        bool initialise();
        
        /** @brief Item access by ID
         *
         *  @remarks Access to an unexistent item ID will cause unexpected behaviour.
         *  @param itemID Item index
         *  @return Desired item data
         */
        const HeroData *getHero(size_t heroID) { return mHero[heroID]; }
        const ItemData *getItem(size_t itemID) { return mItems[itemID]; }
        const SkillData *getSkill(size_t skillID) { return mSkill[skillID]; }
        const EnemyData *getEnemy(size_t enemyID) { return mEnemy[enemyID]; }
        const BtlBgData *getBtlBgData(size_t btlBgID) { return mBtlBgData[btlBgID]; }
        
        /// @brief Gets the number of items in the database
        size_t getNumHeroes() { return mHero.size(); }
        size_t getNumItems() { return mItem.size(); }
        size_t getNumSkills() { return mSkill.size(); }
        size_t getEnemy() { return mEnemy.size(); }
        size_t getBtlBg() { return mBtlBg.size(); }

    private:
        // Hero Data
        std::vector<HeroData *> mHero;
        // Item Data
        std::vector<ItemData *> mItem;
        // Skill Data
        std::vector<SkillData *> mSkill;
        // Enemy Data
        std::vector<EnemyData *> mEnemy;
        // Enemy Group Data
        std::vector<EnemuGroupData *> mEnemyGroupData;
        // Battle Background
        std::vector<BtlBgData *> mBtlBgData;
        // TODO: Everything

    };
} // namespace

#endif // SONETTO_DATABASE_H
