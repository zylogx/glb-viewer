/*******************************************************************************************
* 
*   Copyright (c) 2024 Wildan R Wijanarko
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#include "raylib.h"

typedef struct 
{
    Color idleColor;
    Color hoverColor;
    Color draggedColor;
    Color currentThumbColor;
    Color backgroundColor;
    bool isDragging;
} ScrollbarColor;

#define CBLUE CLITERAL(Color){ 11, 174, 219, 255 }
#define DBLUE CLITERAL(Color){ 20, 146, 181, 255 }