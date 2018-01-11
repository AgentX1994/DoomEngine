#pragma once
#include "main.hpp"
#include <map>

/* 
 * Implements a caching system for font glyphs using std::map.
 * 
 * Use get method to get a glyph. If the glyph is in the cache, the glyph is
 * immediately returned, along with the GL texture id. Otherwise, the glyph is
 * loaded using FT2, and is sent to the GPU, and then it is returned.
 */
typedef struct {
    GLuint tex_id;
    FT_GlyphSlot glyph;
} cache_entry;

class glyphcache {
    private:
        FT_Face face;
        std::map<const char, cache_entry> cache;
        
        void alloc_texture(cache_entry &entry);
        
    public:
        // Need the font face to load glyphs on misses
        glyphcache(FT_Face f) { face = f; }
        cache_entry get(const char c);
};
