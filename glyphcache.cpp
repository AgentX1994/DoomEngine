#include "glyphcache.hpp"

void glyphcache::alloc_texture(cache_entry &entry)
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &entry.tex_id);
    glBindTexture(GL_TEXTURE_2D, entry.tex_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            entry.glyph->bitmap.width,
            entry.glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            entry.glyph->bitmap.buffer
    );
}

cache_entry glyphcache::get(const char c)
{
    if(cache.find(c) != cache.end())
    {
        return cache.at(c);
    } else
    {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "Cannot load character '" << c << "'!" << std::endl;
            return {0, nullptr};
        }
        cache_entry entry;
        entry.tex_id = 0;
        entry.advancex = face->glyph->advance.x;
        entry.advancey = face->glyph->advance.y;
        FT_Glyph g;
        if(FT_Get_Glyph(face->glyph, &g))
        {
            std::cerr << "Cannot get glyph of character '" << c << "'!" << std::endl;
            return entry;
        }
        entry.glyph = (FT_BitmapGlyph) g;
        alloc_texture(entry);
        cache[c] = entry;
        return entry;
    }
}

glyphcache::~glyphcache()
{
    for (auto pair : cache)
    {
        FT_Glyph g = (FT_Glyph)(pair.second.glyph);
        FT_Done_Glyph(g);
    }
}
