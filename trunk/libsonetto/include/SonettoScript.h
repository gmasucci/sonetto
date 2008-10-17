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

#ifndef SONETTO_SCRIPT_H
#define SONETTO_SCRIPT_H

// Typedefs and forward declarations
namespace Sonetto {
    class Script;
}

#include <vector>
#include "SonettoMain.h"
#include "SonettoScriptFile.h"

namespace Sonetto {
    class SONETTO_EXPORT Script
    {
    public:
        Script(ScriptFilePtr file)
                : mScriptFile(file), mOffset(0) {}

        ~Script();

        ScriptFilePtr _getScriptFile() { return mScriptFile; }

        void _setOffset(size_t offset) { mOffset = offset; }

        size_t _getOffset() const { return mOffset; }

    private:
        /** ScriptFile pointer

            Holds opcodes to be used by this script.
        */
        ScriptFilePtr mScriptFile;

        size_t mOffset;
    };
} // namespace Sonetto

#endif
