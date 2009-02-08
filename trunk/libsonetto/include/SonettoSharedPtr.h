/*-----------------------------------------------------------------------------
Copyright (c) 2009, Sonetto Project Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1.  Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
3.  Neither the name of the Sonetto Project nor the names of its contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------*/

#ifndef SONETTO_SHAREDPTR_H
#define SONETTO_SHAREDPTR_H

#include <OgreSharedPtr.h>

namespace Sonetto
{
    template<class PtrType>
    class SharedPtr : public Ogre::SharedPtr<PtrType>
    {
    public:
        SharedPtr() : Ogre::SharedPtr<PtrType>() {}

        explicit SharedPtr(PtrType *rep) : Ogre::SharedPtr<PtrType>(rep) {}

        SharedPtr(const SharedPtr<PtrType> &r) : Ogre::SharedPtr<PtrType>(r) {}

        template<class RhsPtrType>
        SharedPtr(const Ogre::SharedPtr<RhsPtrType> &r) :
                Ogre::SharedPtr<PtrType>()
        {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)

            RhsPtrType *rep = r.getPointer();
            if (!rep)
            {
                Ogre::SharedPtr<PtrType>::setNull();
                return;
            }

            // <todo> FIXME: Why on earth do I need to explicitly show the
            // Ogre::SharedPtr<PtrType> scope here? Shouldn't the compiler
            // dig for it and find it implicitly?
            Ogre::SharedPtr<PtrType>::pRep = dynamic_cast<PtrType *>(rep);
            assert(Ogre::SharedPtr<PtrType>::pRep);
            Ogre::SharedPtr<PtrType>::pUseCount = r.useCountPointer();

            if (Ogre::SharedPtr<PtrType>::pUseCount)
            {
                ++(*Ogre::SharedPtr<PtrType>::pUseCount);
            }
        }

        template<class RhsPtrType>
        SharedPtr &operator=(const Ogre::SharedPtr<RhsPtrType> &r)
        {
            PtrType *rep = dynamic_cast<PtrType *>(r.getPointer());
            assert(rep);

            if (Ogre::SharedPtr<PtrType>::pRep == rep)
                return *this;

            Ogre::SharedPtr<PtrType>::release();

            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            Ogre::SharedPtr<PtrType>::pRep = rep;
            Ogre::SharedPtr<PtrType>::pUseCount = r.useCountPointer();

            if (Ogre::SharedPtr<PtrType>::pUseCount)
            {
                ++(*Ogre::SharedPtr<PtrType>::pUseCount);
            }

            return *this;
        }
    };
} // namespace

#endif
