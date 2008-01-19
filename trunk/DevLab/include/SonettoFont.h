#ifndef SONETTOFONT_H_INCLUDED
#define SONETTOFONT_H_INCLUDED

#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreTechnique.h>
#include "SonettoMain.h"

namespace Sonetto
{
    using namespace Ogre;
    /** @brief This is a class that stores the font data.
    *
    * This class is used to set up a Font Texture for use with Sonetto::SonettoTextElement.
    */
    class Font : public Ogre::Resource
    {
    public:
        /// Number of glyphs per line and number of lines.
        GlyphTable glyph_tbl;
        /// Default Vertical Space for each new line.
        float default_y_space;
        /// Number of Glyphs.
        Ogre::uint32 num_glyphs;
        /// Offset value for Glyph Mapping.
        Ogre::Vector2 glyph_coord_offset;
        /// Name of the current texture used on this font. (deprecated?)
        Ogre::String font_texture_name;

        Ogre::uint16 font_blend_mode;

        Ogre::uint16 font_alpha_reject_settings;

        Ogre::uint16 alpha_rejection;

        Ogre::uint16 font_filtering_mode;

        /// Glyph Structure List.
        std::vector<GlyphStruct> glyphs;
        /// Glyph Structure List Iterator.
        std::vector<GlyphStruct>::iterator glyph_itr;
        /// Color List.
        std::vector<ColourValue> mColorList;
        /// Color List Iterator.
        std::vector<ColourValue>::iterator mColorList_itr;

        /// Pointer to the material for this font.
        MaterialPtr font_material;
        /// Pointer to the font material's texture.
        TexturePtr font_texture;

        /// If the font material exist or not.
        bool materialExist;


    protected:

        // must implement these from the Ogre::Resource interface
        void loadImpl();
        void unloadImpl();
        size_t calculateSize() const;

    public:
        /// Constructor.
        Font(Ogre::ResourceManager *creator, const Ogre::String &name,
                    Ogre::ResourceHandle handle, const Ogre::String &group, bool isManual = false,
                    Ogre::ManualResourceLoader *loader = 0);
        /// Destructor.
        virtual ~Font();

        // Manual Custom Font file creation methods
        /** @brief Set up the font
        *
        * This function initialise the font.
        * @param x is the number of horizontal glyphs in the texture.
        * @param y is the number of vertical glyphs in the texture.
        * @param default_space is the default vertical space between the letters.
        * @param name is the texture name for this font.
        */
        void setUpFont(unsigned long x, unsigned long y, float default_space, const Ogre::String &name);
        /// Calculate the Glyph coords for a new font texture.
        void calculateGlyphCoords();
        /** @brief Set the horizontal space for a specifig glyph.
        *
        * @param glyph is the Glyph ID.
        * @param spcWidth is the horizontal space for this glyph.
        */
        void setSpaceWidth(size_t glyph, float spcWidth)
        {
            glyphs[glyph].x_offset = spcWidth;
        }

        /** @brief Create the font material.
        *
        * @return wheter the material was sucessfully created.
        */
        bool createMaterial();
        /** @brief Destroy/Delete font material from memory.
        *
        * @return wheter the material was sucessfully deleted.
        */
        bool deleteMaterial();     // Destroy/Delete font material from memory

        /// Set the font texture name
        void setTextureName(const Ogre::String &name_a);

        /// Get the font texture name.
        const Ogre::String & getTextureName();

    };

    class FontPtr : public Ogre::SharedPtr<Font>
    {
    public:
        FontPtr() : Ogre::SharedPtr<Font>() {}
        explicit FontPtr(Font *rep) : Ogre::SharedPtr<Font>(rep) {}
        FontPtr(const FontPtr &r) : Ogre::SharedPtr<Font>(r) {}
        FontPtr(const Ogre::ResourcePtr &r) : Ogre::SharedPtr<Font>()
        {
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            pRep = static_cast<Font*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
        }

        /// Operator used to convert a ResourcePtr to a FontPtr
        FontPtr& operator=(const Ogre::ResourcePtr& r)
        {
            if (pRep == static_cast<Font*>(r.getPointer()))
                return *this;
            release();
            // lock & copy other mutex pointer
            OGRE_LOCK_MUTEX(*r.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(r.OGRE_AUTO_MUTEX_NAME)
            pRep = static_cast<Font*>(r.getPointer());
            pUseCount = r.useCountPointer();
            if (pUseCount)
            {
                ++(*pUseCount);
            }
            return *this;
        }
    };

} // namespace
#endif // SONETTOFONT_H_INCLUDED
