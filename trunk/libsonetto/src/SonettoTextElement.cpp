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
#include "SonettoTextElement.h"
#include "SonettoKernel.h"
namespace Sonetto
{
    //--------------------------------------------------------------------------
    // Sonetto::TextElement implementation.
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    Ogre::String TextElement::msTypeName = "Text";
    //--------------------------------------------------------------------------
    TextElement::TextElement(const Ogre::String& name)
            :   OverlayElement(name),
                mInitialised(false),
                mAlpha(1.0f),
                mpIconSetRenderable(NULL),
                mpIconSet(NULL),
                mTextState(TS_NOT_INITIALISED),
                mCharAllocSize(0),
                mIconAllocSize(0),
                mCharStringSize(0),
                mIconStringSize(0),
                mStrCursorPosition(0),
                mTimeSinceLastFrame(0.0f),
                mDifference(0.0f),
                mViewportAspectCoef(0.0f),
                mScrMetricsMode(SMM_RELATIVE_ASPECT_ADJUSTED),
                mTextSize(Ogre::Vector2(0.0f,0.0f)),
                mLineSize(1.0f),
                mCurTextColor(Ogre::ColourValue::White),
                mIsAnimated(false),
                mUseFadeIn(false),
                mUseFadeOut(false),
                mSkipWhiteSpaces(false),
                mAnimSpeed(0.0f),
                mDefAnimSpeed(0.0f),
                mFadeSpeed(0.0f),
                mFadeOutSpeed(0.0f),
                mDefaultColor(0)
    {
        initialise();
    }
    //--------------------------------------------------------------------------
    TextElement::~TextElement()
    {
        // Delete Render Operations.
        delete mRenderOp.vertexData;
        delete mIconSetRenderOp.vertexData;
        // Delete the Icon Set Renderable.
        delete mpIconSetRenderable;
    }
    //--------------------------------------------------------------------------
    void TextElement::initialise(void)
    {
        // Check if the object has been initialised already.
        if(!mInitialised)
        {
            // Set up the render op
            // Combine positions and texture coords since they tend to change together
            // since character sizes are different
            mRenderOp.vertexData = new Ogre::VertexData();
            Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            size_t offset = 0;
            // Positions
            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
            // Texcoords
            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            // Colours - store in the same buffer
            decl->addElement(1, 0, Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
            mRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            mRenderOp.useIndexes = false;
            mRenderOp.vertexData->vertexStart = 0;
            //---------------------------
            // Set up the icon render op
            //---------------------------
            mpIconSetRenderable = new TextIconSetRenderable(this);
            // Combine positions and texture coords since they tend to change together
            // since character sizes are different
            mIconSetRenderOp.vertexData = new Ogre::VertexData();
            decl = mIconSetRenderOp.vertexData->vertexDeclaration;
            offset = 0;
            // Positions
            decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
            // Texcoords
            decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
            offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
            mIconSetRenderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
            mIconSetRenderOp.useIndexes = false;
            mIconSetRenderOp.vertexData->vertexStart = 0;
            // Vertex buffer will be created in checkMemoryAllocation
            checkMemoryAllocation( DEFAULT_INITIAL_CHARS , DEFAULT_INITIAL_CHARS );
            allocateFadeList(DEFAULT_INITIAL_CHARS * 2,0.0f);
            mInitialised = true;
            mTextState = TS_INITIALISED;
            mCharStringSize = DEFAULT_INITIAL_CHARS;
            mIconStringSize = DEFAULT_INITIAL_CHARS;
            mTotalStringSize = DEFAULT_INITIAL_CHARS;
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::setCaption(const Ogre::String & text)
    {
        // Copy the string to the local string.
        mText = text;
        // Get the number of printable characters.
        mCharStringSize = getNumStringChars(mText,0);
        // then get the number of icons.
        mIconStringSize = getNumStringIcons(mText,0);
        // set the total string size
        // to the sum of total printable chars plus the icons.
        mTotalStringSize = mCharStringSize + mIconStringSize;
        // allocate memory if necessary,
        // for both printable characters and icons
        checkMemoryAllocation(mCharStringSize, mIconStringSize);
        // allocate the fade list
        // the same fade list is used for both
        // icons and printable characters.
        allocateFadeList(mTotalStringSize, 0.0f);
    }
    //--------------------------------------------------------------------------
    void TextElement::setMessage(const Ogre::String & text)
    {
        setCaption(text);
    }
    //--------------------------------------------------------------------------
    void TextElement::setMessage(const Ogre::String * text)
    {
        setCaption(*text);
    }
    //--------------------------------------------------------------------------
    void TextElement::setFont(FontPtr font)
    {
        // Set the font to the font pointer.
        mFontPtr = font;
        // Assign the Font Material to the object material.
        mpMaterial = mFontPtr->font_material;
        // As the text is an Overlay, force the material
        // to have depth check disabled
        // and disable lighting.
        mpMaterial->setDepthCheckEnabled(false);
        mpMaterial->setLightingEnabled(false);
    }
    //--------------------------------------------------------------------------
    FontPtr TextElement::getFont()
    {
        return mFontPtr;
    }
    //--------------------------------------------------------------------------
    void TextElement::setAnimSettings(  bool isAnim, bool useFade, bool useFOut, bool skipWS,
                                        Ogre::Real anmSpd, Ogre::Real fdSpd,
                                        Ogre::Real fdOutSpd)
    {
        // Configure the Text Element
        mIsAnimated = isAnim;
        mUseFadeIn = useFade;
        mUseFadeOut = useFOut;
        mSkipWhiteSpaces = skipWS;
        mDefAnimSpeed = anmSpd;
        mAnimSpeed = anmSpd;
        mFadeSpeed = fdSpd;
        mFadeOutSpeed = fdOutSpd;
    }
    //--------------------------------------------------------------------------
    void TextElement::setAnimated(bool isAnim)
    {
        mIsAnimated = isAnim;
    }
    //--------------------------------------------------------------------------
    bool TextElement::getAnimated()
    {
        return mIsAnimated;
    }
    //--------------------------------------------------------------------------
    void TextElement::setUseFade(bool useFade)
    {
        mUseFadeIn = useFade;
    }
    //--------------------------------------------------------------------------
    bool TextElement::getUseFade()
    {
        return mUseFadeIn;
    }
    //--------------------------------------------------------------------------
    void TextElement::setUseFadeOut(bool useFOut)
    {
        mUseFadeOut = useFOut;
    }
    //--------------------------------------------------------------------------
    bool TextElement::getUseFadeOut()
    {
        return mUseFadeOut;
    }
    //--------------------------------------------------------------------------
    void TextElement::setFadeSpeed(Ogre::Real fdSpd)
    {
        mFadeSpeed = fdSpd;
    }
    //--------------------------------------------------------------------------
    Ogre::Real TextElement::getFadeSpeed()
    {
        return mFadeSpeed;
    }
    //--------------------------------------------------------------------------
    void TextElement::setFadeOutSpeed(Ogre::Real fdOutSpd)
    {
        mFadeOutSpeed = fdOutSpd;
    }
    //--------------------------------------------------------------------------
    Ogre::Real TextElement::getFadeOutSpeed()
    {
        return mFadeOutSpeed;
    }
    //--------------------------------------------------------------------------
    void TextElement::setTextSize(Ogre::Vector2 size)
    {
        mTextSize = size;
    }
    //--------------------------------------------------------------------------
    Ogre::Vector2 TextElement::getTextSize()
    {
        return mTextSize;
    }
    //--------------------------------------------------------------------------
    void TextElement::setLineDistance(Ogre::Real dist)
    {
        mLineSize = dist;
    }
    //--------------------------------------------------------------------------
    Ogre::Real TextElement::getLineDistance()
    {
        return mLineSize;
    }
    //--------------------------------------------------------------------------
    void TextElement::forceAnimEnd()
    {
        if(mIsAnimated)
        {
            mStrCursorPosition = mTotalStringSize;
            mTextState = TS_ANIMATION_FINISHED;
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::forceAnimReset()
    {
        if(mIsAnimated)
        {
            mStrCursorPosition = 0;
            mDifference = 0.0f;
            allocateFadeList(mTotalStringSize,0.0f);
            mTextState = TS_INITIALISED;
        }
    }
    //--------------------------------------------------------------------------
    const Ogre::String& TextElement::getTypeName(void) const
    {
        return msTypeName;
    }
    //--------------------------------------------------------------------------
    void TextElement::getRenderOperation(Ogre::RenderOperation& op)
    {
        op = mRenderOp;
    }
    //--------------------------------------------------------------------------
    void TextElement::setMaterialName(const Ogre::String& matName)
    {
        OverlayElement::setMaterialName(matName);
    }
    //--------------------------------------------------------------------------
    void TextElement::setIconSet(IconSet * icn)
    {
        mpIconSet = icn;
    }
    //--------------------------------------------------------------------------
    IconSet * TextElement::getIconSet()
    {
        return mpIconSet;
    }
    //--------------------------------------------------------------------------
    void TextElement::setAlpha(Ogre::Real alpha)
    {
        mAlpha = alpha;
    }
    //--------------------------------------------------------------------------
    Ogre::Real TextElement::getAlpha()
    {
        return mAlpha;
    }
    //--------------------------------------------------------------------------
    void TextElement::setColor(size_t colorID)
    {
        mDefaultColor = colorID;
    }
    //--------------------------------------------------------------------------
    size_t TextElement::getColor()
    {
        return mDefaultColor;
    }
    //--------------------------------------------------------------------------
    Ogre::Vector2 TextElement::getTextDimensions()
    {
        // Calculate the largest line, then calculate the text height.
        Ogre::Vector2 strdimensions(getLargestLineLenght(mText.c_str()),getStringHeight(mText.c_str()));
        return strdimensions;
    }
    //--------------------------------------------------------------------------
    void TextElement::setScrMetricsMode(ScreenMetricsMode smm)
    {
        mScrMetricsMode = smm;
    }
    //--------------------------------------------------------------------------
    ScreenMetricsMode TextElement::getScrMetricsMode()
    {
        return mScrMetricsMode;
    }
    //--------------------------------------------------------------------------
    Ogre::Real TextElement::getLineLenght(const char * line)
    {
        Ogre::Real ret = 0;
        const char * cur = line;
        while( (*cur != 0) && (*cur != NEW_LINE_CHARACTER ) )
        {
            switch(*cur)
            {
                case TEXT_COMMAND:
                    ++cur;
                    switch(*cur)
                    {
                        case TEXT_COLOR_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_SPEED_COMMAND:
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_ITEM_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            switch(mScrMetricsMode)
                            {
                                case SMM_RELATIVE:
                                    ret += mTextSize.x;
                                break;
                                case SMM_RELATIVE_ASPECT_ADJUSTED:
                                    ret += mViewportAspectCoef * mTextSize.x;
                                break;
                            }
                        break;
                    }
                break;
                default:
                {
                    Glyphstruct  glyphData = mFontPtr->glyphs[static_cast<int>(*cur)];
                    switch(mScrMetricsMode)
                    {
                        case SMM_RELATIVE:
                            ret += glyphData.x_offset * mTextSize.x;
                        break;
                        case SMM_RELATIVE_ASPECT_ADJUSTED:
                            ret += (glyphData.x_offset / mViewportAspectCoef) * mTextSize.x;
                        break;
                    }
                    ++cur;
                }
                break;
            }
        }
        ret += mViewportAspectCoef * mTextSize.x;
        return ret;
    }
    //--------------------------------------------------------------------------
    const char * TextElement::getLargestLine(const char * str)
    {
        size_t l_line_size = 0;
        size_t c_line_size = 0;
        const char * cur = str;
        const char * l_line_ptr = cur;
        const char * c_line_ptr = cur;
        while( (*cur != 0) )
        {
            switch(*cur)
            {
                case NEW_LINE_CHARACTER:
                {
                    if(c_line_size > l_line_size)
                    {
                        l_line_size = c_line_size;
                        l_line_ptr = c_line_ptr;
                        ++cur;
                        c_line_size = 0;
                        c_line_ptr = cur;
                    }
                    else
                    {
                        ++cur;
                        c_line_size = 0;
                        c_line_ptr = cur;
                    }
                }
                break;
                case 0:
                {
                    if(c_line_size > l_line_size)
                    {
                        ++cur;
                        l_line_ptr = cur;
                        l_line_size = c_line_size;
                        c_line_size = 0;

                    }
                    else
                    {
                        ++cur;
                    }
                }
                break;
                case TEXT_COMMAND:
                    ++cur;
                    switch(*cur)
                    {
                        case TEXT_COLOR_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_SPEED_COMMAND:
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_ITEM_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                        break;
                    }
                break;
                default:
                {
                    ++c_line_size;
                    ++cur;
                }
                break;
            }
        }
        return l_line_ptr;
    }
    //--------------------------------------------------------------------------
    Ogre::Real TextElement::getLargestLineLenght(const char * str)
    {
        Ogre::Real ret = 0;
        const char * cur = getLargestLine(str);
        while( (*cur != 0) && (*cur != NEW_LINE_CHARACTER ) )
        {
            switch(*cur)
            {
                case TEXT_COMMAND:
                    ++cur;
                    switch(*cur)
                    {
                        case TEXT_COLOR_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_SPEED_COMMAND:
                            ++cur;
                            ++cur;
                        break;
                        case TEXT_ITEM_COMMAND:
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            ++cur;
                            switch(mScrMetricsMode)
                            {
                                case SMM_RELATIVE:
                                    ret += mTextSize.x;
                                break;
                                case SMM_RELATIVE_ASPECT_ADJUSTED:
                                    ret += mViewportAspectCoef * mTextSize.x;
                                break;
                            }
                        break;
                    }
                break;
                default:
                {
                    Glyphstruct  glyphData = mFontPtr->glyphs[static_cast<int>(*cur)];
                    switch(mScrMetricsMode)
                    {
                        case SMM_RELATIVE:
                            ret += glyphData.x_offset * mTextSize.x;
                        break;
                        case SMM_RELATIVE_ASPECT_ADJUSTED:
                            ret += (glyphData.x_offset / mViewportAspectCoef) * mTextSize.x;
                        break;
                    }
                    ++cur;
                }
                break;
            }
        }
        ret += mTextSize.x/mViewportAspectCoef;
        return ret;
    }
    //--------------------------------------------------------------------------
    Ogre::Real TextElement::getStringHeight(const char * str)
    {
        Ogre::Real ret = 0;
        const char * cur = str;
        bool finished = false;
        while(!finished)
        {
            switch(*cur)
            {
                default:
                    ++cur;
                break;
                case NEW_LINE_CHARACTER:
                    ret += mTextSize.y * mLineSize;
                    ++cur;
                break;
                case 0:
                    finished = true;
                break;
            }
        }
        ret += mTextSize.y;
        return ret;
    }
    //--------------------------------------------------------------------------
    void TextElement::startAnimation()
    {
        if(mIsAnimated)
        {
            forceAnimReset();
            mTextState = TS_ANIMATION_ACTIVE;
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::stopAnimation()
    {
        if(mIsAnimated)
        {
            forceAnimReset();
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::pauseAnimation()
    {
        if(mIsAnimated && mTextState == TS_ANIMATION_ACTIVE)
        {
            mTextState = TS_ANIMATION_PAUSED;
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::resumeAnimation()
    {
        if(mIsAnimated && mTextState == TS_ANIMATION_PAUSED)
        {
            mTextState = TS_ANIMATION_ACTIVE;
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::showText()
    {
        if(!mIsAnimated && mTextState == TS_INITIALISED)
        {
            mTextState = TS_STATIC;
            forceAnimEnd();
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::hideText()
    {
        if(!mIsAnimated && mTextState == TS_STATIC)
        {
            mTextState = TS_INITIALISED;
            forceAnimReset();
        }
    }
    //--------------------------------------------------------------------------
    TextState TextElement::getTextState()
    {
        return mTextState;
    }
    //--------------------------------------------------------------------------
    void TextElement::_update(void)
    {
        mTimeSinceLastFrame = Kernel::get()->mFrameTime;
        mViewportAspectCoef = Kernel::get()->mAspectRatio;
        OverlayElement::_update();
        if(mIsAnimated && (mTextState == TS_ANIMATION_ACTIVE))
        {
            if(mAnimSpeed)
            {
                mDifference += mAnimSpeed * mTimeSinceLastFrame;
                mStrCursorPosition = (size_t) mDifference;
            } else {
                mStrCursorPosition = mTotalStringSize;
            }
            if(mStrCursorPosition == mTotalStringSize)
                mTextState = TS_ANIMATION_FINISHED;
        }
        //------------------------------
        // Update Geometry and Textures
        //------------------------------

        // Pointer to the vertex buffer.
        float *pVert = 0;
        float *pIconVert = 0;
        // Current text position.
        size_t t_curPos = 0;
        size_t t_charPos = 0;
        size_t t_iconPos = 0;
        // Check the text state.
        switch(mTextState)
        {
            case TS_NOT_INITIALISED:
            case TS_INITIALISED:
            break;

            case TS_ANIMATION_PAUSED:
            break;
            // Case its not Active or is not static, just skip this part.
            // If the animation is active, get the total size skiping commands.
            case TS_ANIMATION_ACTIVE:
                if(mStrCursorPosition)
                {
                    t_curPos = getStrSize(mText, mStrCursorPosition);
                    t_charPos = getNumStringChars(mText, mStrCursorPosition);
                    t_iconPos = getNumStringIcons(mText, mStrCursorPosition);
                }
            break;
            // If static, set the cursor position to the total string size, so the entire text show at once.
            case TS_ANIMATION_FINISHED:
            case TS_STATIC:
                t_charPos = mCharStringSize;
                t_iconPos = mIconStringSize;
                t_curPos = mTotalStringSize;
            break;
            default:
            break;
        }

        // Get the lowest depth value for overlays.
        Ogre::Real zValue = Ogre::Root::getSingleton().getRenderSystem()->getMaximumDepthInputValue();

        // Allocate Memory for the text and icons.
        checkMemoryAllocation(t_charPos,t_iconPos);

        // Set the vertex count to the current number of chracters.
        mRenderOp.vertexData->vertexCount = t_charPos * 6;
        // Do the same to the icons.
        mIconSetRenderOp.vertexData->vertexCount = t_charPos * 6;

        // Get the vertex buffer for texture and position
        HardwareVertexBufferSharedPtr p_vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
        pVert = static_cast<float*>(
                    p_vbuf->lock(HardwareBuffer::HBL_DISCARD) );

        // Get the vertex buffer for Icon texture and position
        HardwareVertexBufferSharedPtr p_iconvbuf =
            mIconSetRenderOp.vertexData->vertexBufferBinding->getBuffer(0);
        pIconVert = static_cast<float*>(
                    p_iconvbuf->lock(HardwareBuffer::HBL_DISCARD) );

        // Get the vertex buffer for colors
        HardwareVertexBufferSharedPtr c_vbuf =
            mRenderOp.vertexData->vertexBufferBinding->getBuffer(1);
        RGBA* pDest = static_cast<RGBA*>(
                          c_vbuf->lock(HardwareBuffer::HBL_DISCARD) );
        RGBA color_cursor;
        Real fade_alpha;

        // Set the current text color ID to the default,
        // this must be done to reset the last
        // update possible color changes.
        mCurTextColor = mFontPtr->mColorList[mDefaultColor];

        // Initialise the ratio variable to default 1.0f.
        Real view_ratio = 1.0f;

        // Set the geometry ratio to the viewport aspect coefficient
        // if the screen metrics mode is set to
        // relative aspect adjusted ratio.
        if(mScrMetricsMode == SMM_RELATIVE_ASPECT_ADJUSTED)
            view_ratio = mViewportAspectCoef;

        Ogre::Real o_txtPosX = ((_getDerivedLeft() * 2.0) - 1.0f)/view_ratio;
        Ogre::Real txtPosX = o_txtPosX;
        Ogre::Real o_txtPosY = -( (_getDerivedTop() * 2.0 ) - 1.0f );
        Ogre::Real txtPosY = o_txtPosY;

        // Setup the base plane values to ease calculations.
        Ogre::Real square[8];
        // Top Left
        square[0] = 0.0f;
        square[1] = 0.0f;
        // Top Right
        square[2] = ((2.0f /  view_ratio) * mTextSize.x);
        square[3] = 0.0f;
        // Bottom Left
        square[4] = 0.0f;
        square[5] = (2.0f * mTextSize.y);
        // Bottom Right
        square[6] = ((2.0f /  view_ratio) * mTextSize.x);
        square[7] = (2.0f * mTextSize.y);

        // Start drawing the text.
        // Loop drawing each letter for this frame
        std::string::iterator itr;
        itr = mText.begin();
        // Start the Loop.
        for ( size_t cur = 0; cur < t_curPos;)
        {
            switch(*itr)
            {
                case 0x00:
                    ++itr;
                break;
                case NEW_LINE_CHARACTER:
                    txtPosY -= 2.0 * (mTextSize.y * mLineSize);
                    txtPosX = o_txtPosX;
                    ++itr;
                break;
                case TEXT_COMMAND:
                    parseTextCommand(itr,cur,txtPosX,txtPosY,zValue,square,&pIconVert, false);
                break;
                case SPACE_CHARACTER:
                {
                    mRenderOp.vertexData->vertexCount -= 6;
                    Glyphstruct  glyphData = mFontPtr->glyphs[SPACE_CHARACTER];
                    txtPosX += ((glyphData.x_offset * 2.0f)/view_ratio) * mTextSize.x;
                    if(!mSkipWhiteSpaces)
                        ++cur;

                    ++itr;
                }
                break;
                default:
                    // Check if fade in is in use.
                    if(mUseFadeIn)
                    {
                        fade_alpha = mFadeList[cur];
                    } else {
                        fade_alpha = mAlpha;
                    }
                    // Start by updating the geometry and textures.
                    // Get the Glyph data structure.
                    Glyphstruct  glyphData = mFontPtr->glyphs[static_cast<unsigned char>(*itr)];
                    // each vert is (x, y, z, u, v, r, g, b, a)
                    //-----------------------------------------
                    // First tri
                    // 0  2
                    // 4  6
                    // Upper left
                    *pVert++ = (txtPosX + square[0]);
                    *pVert++ = (txtPosY - square[1]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.texcoord0.x;
                    *pVert++ = glyphData.texcoord0.y;
                    // Bottom left
                    *pVert++ = (txtPosX + square[4]);
                    *pVert++ = (txtPosY - square[5]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.texcoord2.x;
                    *pVert++ = glyphData.texcoord2.y;
                    // Top right
                    *pVert++ = (txtPosX + square[2]);
                    *pVert++ = (txtPosY - square[3]);
                    *pVert++ = -1.0;
                    *pVert++ = glyphData.texcoord1.x;
                    *pVert++ = glyphData.texcoord1.y;
                    //-----------------------------------------
                    //-----------------------------------------
                    // Second tri
                    // Top right (again)
                    *pVert++ = (txtPosX + square[2]);
                    *pVert++ = (txtPosY - square[3]);
                    *pVert++ = zValue;
                    *pVert++ = glyphData.texcoord1.x;
                    *pVert++ = glyphData.texcoord1.y;
                    // Bottom left (again)
                    *pVert++ = (txtPosX + square[4]);
                    *pVert++ = (txtPosY - square[5]);
                    *pVert++ = zValue;
                    *pVert++ = glyphData.texcoord2.x;
                    *pVert++ = glyphData.texcoord2.y;
                    // Bottom right
                    *pVert++ = (txtPosX + square[6]);
                    *pVert++ = (txtPosY - square[7]);
                    *pVert++ = zValue;
                    *pVert++ = glyphData.texcoord3.x;
                    *pVert++ = glyphData.texcoord3.y;
                    //-----------------------------------------
                    // Move the text cursor forward.
                    txtPosX += ((glyphData.x_offset * 2.0f)/view_ratio) * mTextSize.x;
                    // Set the current fade alpha to the cursor, and apply the base alpha into it.
                    mCurTextColor.a = fade_alpha * mAlpha;
                    Root::getSingleton().convertColourValue(mCurTextColor, &color_cursor);
                    // Now update the colors
                    // First tri (top, bottom, top)
                    *pDest++ = color_cursor;
                    *pDest++ = color_cursor;
                    *pDest++ = color_cursor;
                    // Second tri (top, bottom, bottom)
                    *pDest++ = color_cursor;
                    *pDest++ = color_cursor;
                    *pDest++ = color_cursor;

                    if(mUseFadeIn)
                    {
                        fade_alpha += mFadeSpeed * mTimeSinceLastFrame;
                        if (fade_alpha > 1.0f)
                        {
                            fade_alpha = 1.0f;
                        }
                        mFadeList[cur] = fade_alpha;
                    }
                    ++cur;
                    ++itr;
                break;
            }
        }
        // Release the vertex buffers.
        c_vbuf->unlock();
        p_vbuf->unlock();
        // Release the icon vertex buffer.
        p_iconvbuf->unlock();


    }
    //--------------------------------------------------------------------------
    void TextElement::allocateFadeList(size_t size, Ogre::Real value)
    {
        mFadeList.clear();
        //mFadeList.erase(mFadeList.begin(),mFadeList.end());
        for (size_t i = 0; i <= size; ++i)
        {
            mFadeList.push_back(value);
        }
    }
    //--------------------------------------------------------------------------
    void TextElement::animate()
    {
    }
    //--------------------------------------------------------------------------
    void TextElement::checkMemoryAllocation(size_t num_chars,size_t num_icons)
    {
        if ( mCharAllocSize < num_chars)
        {
            // Create and bind new buffers
            // Note that old buffers will be deleted automatically through reference counting
            // 6 verts per char since we're doing tri lists without indexes
            // Allocate space for positions & texture coords
            Ogre::VertexDeclaration* decl = mRenderOp.vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = mRenderOp.vertexData->vertexBufferBinding;
            mRenderOp.vertexData->vertexCount = num_chars * 6;
            // Create dynamic since text tends to change alot
            // positions & texcoords
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                HardwareBufferManager::getSingleton().
                createVertexBuffer(
                    decl->getVertexSize(0),
                    mRenderOp.vertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(0, vbuf);
            // colours
            vbuf = Ogre::HardwareBufferManager::getSingleton().
                   createVertexBuffer(
                       decl->getVertexSize(1),
                       mRenderOp.vertexData->vertexCount,
                       Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(1, vbuf);

            mCharAllocSize = num_chars;

        }
        if ( mIconAllocSize < num_icons)
        {
            // Create and bind new buffers
            // Note that old buffers will be deleted automatically through reference counting
            // 6 verts per char since we're doing tri lists without indexes
            // Allocate space for positions & texture coords
            Ogre::VertexDeclaration* decl = mIconSetRenderOp.vertexData->vertexDeclaration;
            Ogre::VertexBufferBinding* bind = mIconSetRenderOp.vertexData->vertexBufferBinding;
            mIconSetRenderOp.vertexData->vertexCount = num_icons * 6;
            // Create dynamic since text tends to change alot
            // positions & texcoords
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                HardwareBufferManager::getSingleton().
                createVertexBuffer(
                    decl->getVertexSize(0),
                    mIconSetRenderOp.vertexData->vertexCount,
                    Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
            bind->setBinding(0, vbuf);

            mIconAllocSize = num_icons;
        }
    }
    //--------------------------------------------------------------------------
    size_t TextElement::getStrSize(Ogre::String &a_str, size_t max_len)
    {
        return getNumStringChars(a_str, max_len) + getNumStringIcons(a_str, max_len);
    }
    //--------------------------------------------------------------------------
    size_t TextElement::getNumStringChars(Ogre::String &a_str, size_t max_len)
    {
        std::string::iterator itr, iend;
        itr =  a_str.begin();
        iend =  a_str.end();
        size_t temp = 0;
        for (; itr < iend;)
        {
            if (max_len != 0 && temp == max_len) // If max_len is not equal to zero and temp is higher or equal to max len...
            {
                break; // If max_lend is not equal zero, it means that it reached the limit imposed by the user
            }
            while (*itr == 0x00 || *itr == NEW_LINE_CHARACTER || *itr == TEXT_COMMAND)
            {
                if (*itr == NEW_LINE_CHARACTER)
                {
                    ++itr; // It's a New Line character, just skip it
                }
                if (*itr == 0x00)
                {
                    ++itr; // It's a Null character, just skip it
                }
                if (*itr == 0x20 && mSkipWhiteSpaces == true)
                {
                    ++itr; // It's a Null character, if Skip White Spaces is enabled, skip it then.
                }
                while (*itr == TEXT_COMMAND)
                {
                    ++itr;
                    if (*itr == TEXT_COLOR_COMMAND)
                    {
                        ++itr; // go to the next character
                        ++itr; // Thousands
                        ++itr; // Hundreds
                        ++itr; // Decimals?
                        ++itr; // Units?
                        ++itr;
                        ++itr;
                    }
                    else if (*itr == TEXT_SPEED_COMMAND)
                    {
                        ++itr; // go to the next character
                        ++itr; // Command type
                    }
                    else if (*itr == TEXT_ITEM_COMMAND)
                    {
                        ++itr; // go to the next character
                        ++itr; // Thousands
                        ++itr; // Hundreds
                        ++itr; // Decimals?
                        ++itr; // Units?
                        ++itr;
                        ++itr;
                    }
                }
            }
            ++temp;
            ++itr;
        }
        return temp;
    }
    //--------------------------------------------------------------------------
    size_t TextElement::getNumStringIcons(Ogre::String &a_str, size_t max_len)
    {
        std::string::iterator itr, iend;
        itr =  a_str.begin();
        iend =  a_str.end();
        size_t temp = 0;
        for (; itr != iend;)
        {
            if (max_len != 0 && temp >= max_len) // If max_len is not equal to zero and temp is higher or equal to max len...
            {
                break; // If max_lend is not equal zero, it means that it reached the limit imposed by the user
            }
            while (*itr == 0x00 || *itr == NEW_LINE_CHARACTER || *itr == TEXT_COMMAND)
            {
                if (*itr == NEW_LINE_CHARACTER)
                {
                    ++itr; // It's a New Line character, just skip it
                }
                if (*itr == 0x00)
                {
                    ++itr; // It's a Null character, just skip it
                }
                if (*itr == 0x20 && mSkipWhiteSpaces == true)
                {
                    ++itr; // It's a Null character, if Skip White Spaces is enabled, skip it then.
                }
                while (*itr == TEXT_COMMAND)
                {
                    ++itr;
                    if (*itr == TEXT_COLOR_COMMAND)
                    {
                        ++itr; // go to the next character
                        ++itr; // Thousands
                        ++itr; // Hundreds
                        ++itr; // Decimals?
                        ++itr; // Units?
                        ++itr;
                    }
                    else if (*itr == TEXT_SPEED_COMMAND)
                    {
                        ++itr; // go to the next character
                        ++itr; // Command type
                    }
                    else if (*itr == TEXT_ITEM_COMMAND)
                    {
                        ++itr; // go to the next character
                        ++itr; // Thousands
                        ++itr; // Hundreds
                        ++itr; // Decimals?
                        ++itr; // Units?
                        ++itr;
                        ++temp;
                    }
                }
            }
            ++itr;
        }
        return temp;
    }
    //--------------------------------------------------------------------------
    size_t TextElement::parseTextCommand(   std::string::iterator &itr,
                                            size_t &offset,
                                            Ogre::Real &txtPosX,
                                            Ogre::Real txtPosY,
                                            Ogre::Real zvalue,
                                            Ogre::Real *square,
                                            float ** pVert,
                                            bool countonly)
    {
        if(*itr == TEXT_COMMAND)
        {
            size_t offset2 = 0;
            ++itr;
            ++offset2;
            switch(*itr)
            {
                case TEXT_COMMAND:
                    // Only return, the next is a printable character
                    return offset2;
                break;
                case TEXT_COLOR_COMMAND:
                    ++itr;
                    ++offset2;
                    switch(*itr)
                    {
                        case TEXT_COLOR_COMMAND_HEX_FLAG:
                        {
                            ++itr;
                            ++offset2;
                            Ogre::ColourValue curcolor = getColorFromString(itr, offset2);
                            if(!countonly)
                                mCurTextColor = curcolor;
                        }
                        break;
                        case TEXT_COMMAND_OPEN:
                        {
                            ++itr;
                            ++offset2;
                            size_t id = getValueTillClose(itr, offset2,4);
                            if(!countonly)
                                mCurTextColor = mFontPtr->mColorList[id];
                        }
                        break;
                        default:
                        break;
                    }

                break;
                case TEXT_SPEED_COMMAND:
                    ++itr;
                    switch(*itr)
                    {
                        case TEXT_SPEED_COMMAND_UP:
                            mAnimSpeed += 5.0f;
                            ++itr;
                        break;
                        case TEXT_SPEED_COMMAND_DOWN:
                            mAnimSpeed -= 5.5f;
                            ++itr;
                        break;
                        case TEXT_SPEED_COMMAND_DEFAULT:
                            mAnimSpeed = mDefAnimSpeed;
                            ++itr;
                        break;
                    }
                case TEXT_ITEM_COMMAND:
                    case TEXT_COMMAND_OPEN:
                        size_t id = getValueTillClose(itr, offset2);
                        if(!countonly)
                        {
                            IconSetFrame icon = mpIconSet->iconSetData[0].iconFrames[0];
                            // each vert is (x, y, z, u, v, r, g, b, a)
                            //-----------------------------------------
                            // First tri
                            // 0  2
                            // 4  6
                            // Upper left
                            **pVert++ = (txtPosX + square[0])*icon.x;
                            **pVert++ = (txtPosY - square[1])*icon.y;
                            **pVert++ = zvalue;
                            **pVert++ = icon.u1;
                            **pVert++ = icon.v1;
                            // Bottom left
                            **pVert++ = (txtPosX + square[4])*icon.x;
                            **pVert++ = (txtPosY - square[5])*icon.y;
                            **pVert++ = zvalue;
                            **pVert++ = icon.u2;
                            **pVert++ = icon.v1;
                            // Top right
                            **pVert++ = (txtPosX + square[2])*icon.x;
                            **pVert++ = (txtPosY - square[3])*icon.y;
                            **pVert++ = zvalue;
                            **pVert++ = icon.u1;
                            **pVert++ = icon.v2;
                            //-----------------------------------------
                            //-----------------------------------------
                            // Second tri
                            // Top right (again)
                            **pVert++ = (txtPosX + square[2])*icon.x;
                            **pVert++ = (txtPosY - square[3])*icon.y;
                            **pVert++ = zvalue;
                            **pVert++ = icon.u1;
                            **pVert++ = icon.v2;
                            // Bottom left (again)
                            **pVert++ = (txtPosX + square[4])*icon.x;
                            **pVert++ = (txtPosY - square[5])*icon.y;
                            **pVert++ = zvalue;
                            **pVert++ = icon.u2;
                            **pVert++ = icon.v1;
                            // Bottom right
                            **pVert++ = (txtPosX + square[6])*icon.x;
                            **pVert++ = (txtPosY - square[7])*icon.y;
                            **pVert++ = zvalue;
                            **pVert++ = icon.u2;
                            **pVert++ = icon.v2;

                            txtPosX = (txtPosX + square[2])*icon.x;
                        }
                    break;
                    default:
                    break;
                break;
            }
        }
        return 0;
    }
    //--------------------------------------------------------------------------
    size_t TextElement::getValueTillClose(std::string::iterator &itr, size_t & offset, size_t max)
    {

        size_t id = 0;
        offset += max+1;
        for (size_t pos1 = max; pos1 > 0; --pos1)
        {
            size_t c = *itr;

            size_t pos = pos1 - 1;
            if (c == '0')
                id += 0 * (size_t) pow( 10, pos );
            if (c == '1')
                id += 1 * (size_t) pow( 10, pos );
            if (c == '2')
                id += 2 * (size_t) pow( 10, pos );
            if (c == '3')
                id += 3 * (size_t) pow( 10, pos );
            if (c == '4')
                id += 4 * (size_t) pow( 10, pos );
            if (c == '5')
                id += 5 * (size_t) pow( 10, pos );
            if (c == '6')
                id += 6 * (size_t) pow( 10, pos );
            if (c == '7')
                id += 7 * (size_t) pow( 10, pos );
            if (c == '8')
                id += 8 * (size_t) pow( 10, pos );
            if (c == '9')
                id += 9 * (size_t) pow( 10, pos );
            ++itr;
        }
        ++itr;
        return id;
    }
    //--------------------------------------------------------------------------
    Ogre::ColourValue TextElement::getColorFromString(std::string::iterator &itr, size_t & offset)
    {
        itr += 6;
        offset += 6;
        return Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f);
    }
    //--------------------------------------------------------------------------
} // namespace
