//
//  Font.cpp
//  GameTest
//
//  Created by Theo Fanet on 07/05/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include "Font.h"

namespace GRender {
    
    inline int next_p2 ( int a )
    {
        int rval=1;
        while(rval<a) rval<<=1;
        return rval;
    }
    
    void make_dlist ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base ) {
        
        if(FT_Load_Glyph( face, FT_Get_Char_Index( face, ch ), FT_LOAD_DEFAULT ))
            throw std::runtime_error("FT_Load_Glyph failed");
        
        FT_Glyph glyph;
        if(FT_Get_Glyph( face->glyph, &glyph ))
            throw std::runtime_error("FT_Get_Glyph failed");
        
        FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
        
        FT_Bitmap& bitmap=bitmap_glyph->bitmap;
        
        int width = next_p2( bitmap.width );
        int height = next_p2( bitmap.rows );
        
        GLubyte* expanded_data = new GLubyte[ 2 * width * height];
        
        for(int j=0; j <height;j++) {
            for(int i=0; i < width; i++){
                expanded_data[2*(i+j*width)]= expanded_data[2*(i+j*width)+1] =
                (i>=bitmap.width || j>=bitmap.rows) ?
                0 : bitmap.buffer[i + bitmap.width*j];
            }
        }
        
        glBindTexture( GL_TEXTURE_2D, tex_base[ch]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                     0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data );
        
        delete [] expanded_data;
        
        glNewList(list_base+ch,GL_COMPILE);
        
        glBindTexture(GL_TEXTURE_2D,tex_base[ch]);
        
        glTranslatef(bitmap_glyph->left, 0, 0);
        
        glPushMatrix();
        if(bitmap.rows - bitmap_glyph->top != 0)
            glTranslatef(0, (float)bitmap_glyph->top - (float)bitmap.rows, 0);
        
        float x = (float)bitmap.width / (float)width;
        float y = (float)bitmap.rows / (float)height;
        
        glBegin(GL_QUADS);
        glTexCoord2d(0,0); glVertex2f(0, bitmap.rows);
        glTexCoord2d(0,y); glVertex2f(0, 0);
        glTexCoord2d(x,y); glVertex2f(bitmap.width, 0);
        glTexCoord2d(x,0); glVertex2f(bitmap.width, bitmap.rows);
        glEnd();
        glPopMatrix();
        glTranslatef(face->glyph->advance.x >> 6, 0, 0);
        
        glEndList();
    }
    
    void Font::setBlink(int steps){
        if(steps > 0){
            _blink = true;
            _blinkStep = steps;
        }
        else
            _blink = false;
    }
    
    Font::Font(string filename, unsigned int h) {
        
        _textures = new GLuint[128];
        _h        = h;
        
        _color.set();
        _br = 0.0;
        _bg = 0.0;
        _bb = 0.0;
        _blink = false;
        _blinkStep = 2;
        _blinkCurrentStep = 0;
        _drawBlinked = false;
        
        filename = "../" + filename;
        
        FT_Library library;
        if (FT_Init_FreeType( &library ))
            throw std::runtime_error("FT_Init_FreeType failed");
        
        FT_Face face;
        
        if (FT_New_Face( library, filename.c_str(), 0, &face ))
            throw std::runtime_error("FT_New_Face failed (there is probably a problem with your font file)");
        
        FT_Set_Char_Size( face, h << 6, h << 6, 96, 96);
        
        _list_base = glGenLists(128);
        glGenTextures( 128, _textures );
        
        for(unsigned char i=0;i<128;i++)
            make_dlist(face,i, _list_base, _textures);
        
        FT_Done_Face(face);
        
        FT_Done_FreeType(library);
    }
    
    Font::~Font() {
        glDeleteLists(_list_base, 128);
        glDeleteTextures(128, _textures);
        delete[] _textures;
    }
    
    inline void pushScreenCoordinateMatrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        GLint	viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(viewport[0],viewport[2],viewport[1],viewport[3]);
        glPopAttrib();
    }
    
    inline void pop_projection_matrix() {
        glPushAttrib(GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glPopAttrib();
    }
    
    void Font::print(float x, float y, const char *fmt, ...)  {
        
        pushScreenCoordinateMatrix();
        
        GLuint font = _list_base;
        float h     = _h / .63f;						//We make the height about 1.5* that of
        
        char		text[256];								// Holds Our String
        va_list		ap;										// Pointer To List Of Arguments
        
        if (fmt == NULL)									// If There's No Text
            *text=0;											// Do Nothing
        
        else {
            va_start(ap, fmt);									// Parses The String For Variables
            vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
            va_end(ap);											// Results Are Stored In Text
        }
        
        const char *start_line=text;
        vector<string> lines;
        
        const char * c = text;
        
        for(;*c;c++) {
            if(*c=='\n') {
                string line;
                for(const char *n=start_line;n<c;n++) line.append(1,*n);
                lines.push_back(line);
                start_line=c+1;
            }
        }
        if(start_line) {
            string line;
            for(const char *n=start_line;n<c;n++) line.append(1,*n);
            lines.push_back(line);
        }
        
        glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glListBase(font);
        
        float modelview_matrix[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);
        
        for(unsigned int i=0;i<lines.size();i++) {
            glPushMatrix();
            glLoadIdentity();
            glTranslatef(x,y-h*i,0);
            glMultMatrixf(modelview_matrix);
            
            if(_blink && _blinkCurrentStep == _blinkStep){
                _drawBlinked = !_drawBlinked;
                _blinkCurrentStep = 0;
            }
            
            if(_drawBlinked)
                glColor3f(_br, _bg, _bb);
            else
                glColor3f(_color.r, _color.g, _color.b);
            
            ++_blinkCurrentStep;
            
            glCallLists((int)lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());
            
            glPopMatrix();
        }
        
        
        glPopAttrib();
        
        pop_projection_matrix();
    }
    
}