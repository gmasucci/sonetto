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

#ifndef SONETTO_OPCODE_H
#define SONETTO_OPCODE_H

#include <vector>
#include <map>

namespace Sonetto
{
    struct SONETTO_API OpcodeArgument
    {
        OpcodeArgument(size_t aSize,void *aArg)
                : size(aSize), arg(aArg) {}

        size_t size;
        void *arg;
    };

    typedef std::vector<OpcodeArgument> ArgumentVector;

    class SONETTO_API Opcode
    {
    public:
        Opcode(OpcodeHandler *aHandler)
                : handler(aHandler) {}

        virtual ~Opcode() {}

        virtual Opcode *create() const = 0;

        OpcodeHandler *handler;
        ArgumentVector arguments;
    };

    typedef std::map<size_t,const Opcode *> OpcodeTable;
} // namespace

#endif
