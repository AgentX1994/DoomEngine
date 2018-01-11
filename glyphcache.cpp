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
        cache_entry entry = {0, face->glyph};
        alloc_texture(entry);
        cache[c] = entry;
        return entry;
    }
}
