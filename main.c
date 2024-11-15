/*******************************************************************************************
* 
*          _____ _   _______ ______  __      _______ ________          ________ _____   
*         / ____| | |__   __|  ____| \ \    / /_   _|  ____\ \        / /  ____|  __ \  
*        | |  __| |    | |  | |__     \ \  / /  | | | |__   \ \  /\  / /| |__  | |__) | 
*        | | |_ | |    | |  |  __|     \ \/ /   | | |  __|   \ \/  \/ / |  __| |  _  /  
*        | |__| | |____| |  | |         \  /   _| |_| |____   \  /\  /  | |____| | \ \  
*         \_____|______|_|  |_|          \/   |_____|______|   \/  \/   |______|_|  \_\ 
*
********************************************************************************************
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

#include "main.h"

//----------------------------------------------------------------
Matrix MatrixMultiply(Matrix a, Matrix b)
{
    Matrix result = { 0 };

    result.m0 = a.m0*b.m0 + a.m4*b.m1 + a.m8*b.m2 + a.m12*b.m3;
    result.m1 = a.m1*b.m0 + a.m5*b.m1 + a.m9*b.m2 + a.m13*b.m3;
    result.m2 = a.m2*b.m0 + a.m6*b.m1 + a.m10*b.m2 + a.m14*b.m3;
    result.m3 = a.m3*b.m0 + a.m7*b.m1 + a.m11*b.m2 + a.m15*b.m3;

    result.m4 = a.m0*b.m4 + a.m4*b.m5 + a.m8*b.m6 + a.m12*b.m7;
    result.m5 = a.m1*b.m4 + a.m5*b.m5 + a.m9*b.m6 + a.m13*b.m7;
    result.m6 = a.m2*b.m4 + a.m6*b.m5 + a.m10*b.m6 + a.m14*b.m7;
    result.m7 = a.m3*b.m4 + a.m7*b.m5 + a.m11*b.m6 + a.m15*b.m7;

    result.m8 = a.m0*b.m8 + a.m4*b.m9 + a.m8*b.m10 + a.m12*b.m11;
    result.m9 = a.m1*b.m8 + a.m5*b.m9 + a.m9*b.m10 + a.m13*b.m11;
    result.m10 = a.m2*b.m8 + a.m6*b.m9 + a.m10*b.m10 + a.m14*b.m11;
    result.m11 = a.m3*b.m8 + a.m7*b.m9 + a.m11*b.m10 + a.m15*b.m11;

    result.m12 = a.m0*b.m12 + a.m4*b.m13 + a.m8*b.m14 + a.m12*b.m15;
    result.m13 = a.m1*b.m12 + a.m5*b.m13 + a.m9*b.m14 + a.m13*b.m15;
    result.m14 = a.m2*b.m12 + a.m6*b.m13 + a.m10*b.m14 + a.m14*b.m15;
    result.m15 = a.m3*b.m12 + a.m7*b.m13 + a.m11*b.m14 + a.m15*b.m15;

    return result;
}

Matrix MatrixTranslateV(Vector3 v)
{
    return (Matrix){ 1.0f, 0.0f, 0.0f, v.x,
                      0.0f, 1.0f, 0.0f, v.y,
                      0.0f, 0.0f, 1.0f, v.z,
                      0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix MatrixScaleV(Vector3 v)
{
    return (Matrix){ v.x, 0.0f, 0.0f, 0.0f,
                      0.0f, v.y, 0.0f, 0.0f,
                      0.0f, 0.0f, v.z, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };
}

Matrix MatrixRotateXYZ(Vector3 angle)
{
    Matrix result = { 0 };

    float cosz = cosf(-angle.z);
    float sinz = sinf(-angle.z);
    float cosy = cosf(-angle.y);
    float siny = sinf(-angle.y);
    float cosx = cosf(-angle.x);
    float sinx = sinf(-angle.x);

    result.m0 = cosz*cosy;
    result.m1 = (cosz*siny*sinx) - (sinz*cosx);
    result.m2 = (cosz*siny*cosx) + (sinz*sinx);
    result.m3 = 0.0f;

    result.m4 = sinz*cosy;
    result.m5 = (sinz*siny*sinx) + (cosz*cosx);
    result.m6 = (sinz*siny*cosx) - (cosz*sinx);
    result.m7 = 0.0f;

    result.m8 = -siny;
    result.m9 = cosy*sinx;
    result.m10 = cosy*cosx;
    result.m11 = 0.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = 0.0f;
    result.m15 = 1.0f;

    return result;
}

Matrix QuaternionToMatrix(Quaternion q) 
{
    Matrix result = { 0 };

    // Normalize the quaternion
    float norm = sqrt(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    if (norm > 0.0f) 
    {
        q.x /= norm;
        q.y /= norm;
        q.z /= norm;
        q.w /= norm;
    }

    // Pre-calculate repeated values
    float xx = q.x*q.x;
    float yy = q.y*q.y;
    float zz = q.z*q.z;
    float xy = q.x*q.y;
    float xz = q.x*q.z;
    float yz = q.y*q.z;
    float wx = q.w*q.x;
    float wy = q.w*q.y;
    float wz = q.w*q.z;

    // Set matrix elements
    result.m0 = 1.0f - 2.0f*(yy + zz);
    result.m1 = 2.0f*(xy - wz);
    result.m2 = 2.0f*(xz + wy);
    result.m3 = 0.0f;

    result.m4 = 2.0f*(xy + wz);
    result.m5 = 1.0f - 2.0f*(xx + zz);
    result.m6 = 2.0f*(yz - wx);
    result.m7 = 0.0f;

    result.m8  = 2.0f*(xz - wy);
    result.m9  = 2.0f*(yz + wx);
    result.m10 = 1.0f - 2.0f*(xx + yy);
    result.m11 = 0.0f;

    result.m12 = 0.0f;
    result.m13 = 0.0f;
    result.m14 = 0.0f;
    result.m15 = 1.0f;

    return result;
}

Matrix MatrixRotateV(Vector3 v)
{
    v.x *= DEG2RAD;
    v.y *= DEG2RAD;
    v.z *= DEG2RAD;

    return MatrixRotateXYZ(v);
}

//----------------------------------------------------------------

//----------------------------------------------------------------

Quaternion QuaternionMultiply(Quaternion a, Quaternion b) 
{
    Quaternion result;

    result.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    result.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    result.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
    result.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;

    return result;
}

Quaternion QuaternionInvert(Quaternion q) 
{
    Quaternion result;
    
    // Calculate the magnitude squared of the quaternion
    float normSquared = q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w;

    if (normSquared > 0.0f) 
    {
        // Inverse of a quaternion q = (conjugate(q)) / (norm(q)^2)
        float invNorm = 1.0f / normSquared;
        result.x = -q.x*invNorm;
        result.y = -q.y*invNorm;
        result.z = -q.z*invNorm;
        result.w = q.w*invNorm;
    } 
    else 
    {
        // Return identity quaternion if the input quaternion is zero (non-invertible)
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        result.w = 1.0f;
    }

    return result;
}

Quaternion QuaternionFromEuler(Vector3 angle)
{
    // Calculate half angles
    float halfPitch = angle.x*0.5f;
    float halfYaw   = angle.y*0.5f;
    float halfRoll  = angle.z*0.5f;

    // Calculate sin and cos for each half angle
    float sinPitch = sinf(halfPitch);
    float cosPitch = cosf(halfPitch);
    float sinYaw   = sinf(halfYaw);
    float cosYaw   = cosf(halfYaw);
    float sinRoll  = sinf(halfRoll);
    float cosRoll  = cosf(halfRoll);

    return (Quaternion) 
    {
        cosYaw*sinPitch*cosRoll + sinYaw*cosPitch*sinRoll,  
        sinYaw*cosPitch*cosRoll - cosYaw*sinPitch*sinRoll, 
        cosYaw*cosPitch*sinRoll - sinYaw*sinPitch*cosRoll, 
        cosYaw*cosPitch*cosRoll + sinYaw*sinPitch*sinRoll   
    };
}

//----------------------------------------------------------------

//----------------------------------------------------------------

Vector3 Vector3Zero() 
{ 
    return (Vector3){ 0.0f, 0.0f, 0.0f }; 
}

Vector3 Vector3One() 
{ 
    return (Vector3){ 1.0f, 1.0f, 1.0f }; 
}

Vector3 Vector3Add(Vector3 a, Vector3 b)
{
    return (Vector3){ a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3 Vector3Subtract(Vector3 a, Vector3 b)
{
    return (Vector3){ a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3 Vector3Multiply(Vector3 a, Vector3 b)
{
    return (Vector3){ a.x*b.x, a.y*b.y, a.z*b.z };
}

Vector3 Vector3Scale(Vector3 v, float scalar)
{
    return (Vector3){ v.x*scalar, v.y*scalar, v.z*scalar };
}

float Vector3DotProduct(Vector3 a, Vector3 b)
{
    return (float){ a.x*b.x + a.y*b.y + a.z*b.z };
}

float Vector3Distance(Vector3 a, Vector3 b)
{
    float result = 0.0f;

    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float dz = b.z - a.z;
    result = sqrtf(dx*dx + dy*dy + dz*dz);

    return result;
}

Vector3 Vector3RotateByQuaternion(Vector3 v, Quaternion q)
{
    // Quaternion-vector multiplication: v' = q * v * q⁻¹
    Quaternion qConjugate = (Quaternion){ -q.x, -q.y, -q.z, q.w };

    // Convert vector to quaternion form (v.x, v.y, v.z, 0)
    Quaternion vQuat = (Quaternion){ v.x, v.y, v.z, 0.0f };

    // Calculate q * v
    Quaternion qv = (Quaternion)
    {
        q.w*vQuat.x + q.y*vQuat.z - q.z*vQuat.y,
        q.w*vQuat.y + q.z*vQuat.x - q.x*vQuat.z,
        q.w*vQuat.z + q.x*vQuat.y - q.y*vQuat.x,
        -q.x*vQuat.x - q.y*vQuat.y - q.z*vQuat.z
    };

    // Calculate (q * v) * q⁻¹
    Quaternion resultQuat = (Quaternion)
    {
        qv.w*qConjugate.x + qv.x*qConjugate.w + qv.y*qConjugate.z - qv.z*qConjugate.y,
        qv.w*qConjugate.y - qv.x*qConjugate.z + qv.y*qConjugate.w + qv.z*qConjugate.x,
        qv.w*qConjugate.z + qv.x*qConjugate.y - qv.y*qConjugate.x + qv.z*qConjugate.w,
        qv.w*qConjugate.w - qv.x*qConjugate.x - qv.y*qConjugate.y - qv.z*qConjugate.z
    };

    return (Vector3){ resultQuat.x, resultQuat.y, resultQuat.z };
}

Vector3 Vector3Transform(Vector3 v, Matrix mat)
{
    return (Vector3)
    {
        v.x*mat.m0 + v.y*mat.m4 + v.z*mat.m8 + mat.m12,
        v.x*mat.m1 + v.y*mat.m5 + v.z*mat.m9 + mat.m13,
        v.x*mat.m2 + v.y*mat.m6 + v.z*mat.m10 + mat.m14
    };
}

//----------------------------------------------------------------

Camera CreateCamera()
{
    Camera camera;
    camera.position    = (Vector3){ 20.0f, 20.0f, 20.0f };  
    camera.target      = (Vector3){ 3.0f, 3.0f, 3.0f };     
    camera.up          = (Vector3){ 0.0f, 1.0f, 0.0f };     
    camera.fovy        = 45.0f;                             
    camera.projection  = CAMERA_PERSPECTIVE;

    return camera;   
}

//----------------------------------------------------------------

void DrawModelPro(Model model, Vector3 pos, Vector3 rot, Vector3 scl)
{
    model.transform = MatrixMultiply(MatrixTranslateV(pos), MatrixMultiply(MatrixRotateV(rot), MatrixScaleV(scl)));
    DrawModel(model, Vector3Zero(), 1.0f, WHITE);
}

void DrawModelWiresPro(Model model, Vector3 pos, Vector3 rot, Vector3 scl)
{
    model.transform = MatrixMultiply(MatrixTranslateV(pos), MatrixMultiply(MatrixRotateV(rot), MatrixScaleV(scl)));
    DrawModelWires(model, Vector3Zero(), 1.0f, WHITE);
}

void DrawTransform(Vector3 pos, Vector3 rot, Vector3 scl)
{
    Matrix rotMatrix = MatrixRotateV(rot);

    float fScl = (scl.x + scl.y + scl.z)/3*0.1f;
  
    DrawLine3D(
        pos,
        Vector3Add(pos, (Vector3){ fScl*rotMatrix.m0, fScl*rotMatrix.m1, fScl*rotMatrix.m2 }),
        RED
    );
        
    DrawLine3D(
        pos,
        Vector3Add(pos, (Vector3){ fScl*rotMatrix.m4, fScl*rotMatrix.m5, fScl*rotMatrix.m6 }),
        GREEN
    );
        
    DrawLine3D(
        pos,
        Vector3Add(pos, (Vector3){ fScl*rotMatrix.m8, fScl*rotMatrix.m9, fScl*rotMatrix.m10 }),
        BLUE
    );
}

void DrawModelBones(Model model, ModelAnimation* anims, unsigned animIndex, unsigned animCurrentFrame, Vector3 pos, Vector3 rot, Vector3 scl, bool isDrawCircles, bool isDrawCubes, bool isDrawAnimTransform, BoneColor colors)
{
    Matrix rotMatrix = MatrixRotateV(rot);

    for (unsigned i = 0; i < model.boneCount-1; i++)
    {
        // Get the current bone translation and apply scaling
        Vector3 translation = anims[animIndex].framePoses[animCurrentFrame][i].translation;
        Vector3 finalTranslation = Vector3Transform(Vector3Multiply(translation, scl), rotMatrix);
        finalTranslation = Vector3Add(finalTranslation, pos); // Final transformed position

        if (isDrawCubes)
        {
            DrawCubeV(finalTranslation, Vector3Scale(scl, 0.1f), colors.cubeColor);
        }

        if (isDrawCircles)
        {
            float radius = (scl.x + scl.y + scl.z)/3.0f*0.1f;
            DrawCircle3D(finalTranslation, radius, (Vector3){ 90.0f, 0.0f, 0.0f }, 130.0f, colors.circleColor);
        }

        if (isDrawAnimTransform)
        {
            DrawTransform(finalTranslation, rot, scl);
        }

        int parentIndex = anims[animIndex].bones[i].parent;
        if (parentIndex >= 0)
        {
            // Get the parent's translation and apply transformations
            Vector3 parentTranslation = anims[animIndex].framePoses[animCurrentFrame][parentIndex].translation;
            Vector3 parentFinalTranslation = Vector3Transform(Vector3Multiply(parentTranslation, scl), rotMatrix);
            parentFinalTranslation = Vector3Add(parentFinalTranslation, pos); // Final transformed position for parent

            // Draw a line between the bone and its parent
            DrawLine3D(finalTranslation, parentFinalTranslation, colors.baseLineColor);
        }
    }
}

void DrawGizmo(Vector3* modelPos, Vector3* posX, Vector3* posY, Vector3* posZ, float size, bool colors[3], bool isGizmoMode)
{
    if (isGizmoMode)
    {
        *modelPos = (Vector3){ posX->x - size, posY->y - size, posZ->z - size };
    }
    else
    {
        *posX = (Vector3){ modelPos->x + size, modelPos->y, modelPos->z };
        *posY = (Vector3){ modelPos->x, modelPos->y + size, modelPos->z };
        *posZ = (Vector3){ modelPos->x, modelPos->y, modelPos->z + size };
    }    

    DrawLine3D((Vector3){ posX->x - size, posX->y, posX->z }, *posX, (colors[0]) ? RED : MAROON);
    DrawLine3D((Vector3){ posY->x, posY->y - size, posY->z }, *posY, (colors[1]) ? GREEN : DARKGREEN);
    DrawLine3D((Vector3){ posZ->x, posZ->y, posZ->z - size }, *posZ, (colors[2]) ? BLUE : DARKBLUE);

    Vector3 gizmoCubeSize = (Vector3){ size*0.05f, size*0.05f, size*0.05f };

    DrawCubeV(*posX, gizmoCubeSize, (colors[0]) ? RED : MAROON);
    DrawCubeV(*posY, gizmoCubeSize, (colors[1]) ? GREEN : DARKGREEN);
    DrawCubeV(*posZ, gizmoCubeSize, (colors[2]) ? BLUE : DARKBLUE);
}

//----------------------------------------------------------------

//----------------------------------------------------------------

bool IsMousePressed()
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
}

bool GuiDropdownPro(Rectangle rec, char** v, unsigned* start, unsigned* end, bool* isDragging, unsigned* index, ScrollbarColor* color)
{
    const unsigned max = vector_size(v);

    // Adjust start and end based on scroll input (mouse wheel)
    if (GetMouseWheelMove() < 0.0f && *end < max)
    {
        *start += 1;
        *end += 1;
    }
    if (GetMouseWheelMove() > 0.0f && *start > 0)
    {
        *start -= 1;
        *end -= 1;
    }

    Rectangle editModeRec = { rec.x, rec.y + rec.height, rec.width + 11, rec.height*5 };

    // Draw scrollbar if the number of items exceeds the visible range
    if (max > 5)
    {
        // Scrollbar background
        Rectangle scrollbarRec = { rec.x + rec.width + 1, rec.y + rec.height, 10, rec.height*5 };
        DrawRectangleRec(scrollbarRec, color->backgroundColor);

        // Calculate the height and position of the scroll thumb
        float thumbHeight = (5.0f/max)*scrollbarRec.height;
        float thumbY = scrollbarRec.y + (*start/(float)(max - 5))*(scrollbarRec.height - thumbHeight);

        Vector2 mousePos = GetMousePosition();

        Rectangle thumbRec = { scrollbarRec.x, thumbY, scrollbarRec.width, thumbHeight };
        bool isHovering = CheckCollisionPointRec(mousePos, thumbRec);

        if (isHovering && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) 
        {
            color->isDragging = true;
        }
        else if (color->isDragging && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) 
        {
            color->isDragging = false;
        }

        if (isHovering && color->isDragging) 
        {
            color->currentThumbColor = color->draggedColor;
        }
        else if (isHovering) 
        {
            color->currentThumbColor = color->hoverColor;
        }
        else if (!isHovering && !color->isDragging)
        {
            color->currentThumbColor = color->idleColor;
        }
        
        DrawRectangleRec(thumbRec, color->currentThumbColor);

        float thumbOffsetY = 0.0f;

        // Start dragging if mouse button is pressed within the thumb
        if (CheckCollisionPointRec(mousePos, thumbRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            *isDragging = true;
            thumbOffsetY = mousePos.y - thumbRec.y; // Calculate offset
        }

        // Update thumb position while dragging
        if (*isDragging)
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                *isDragging = false;
            }
            
            float newThumbY = mousePos.y - thumbOffsetY;

            // Clamp the thumb's position within the scrollbar
            newThumbY = fmax(scrollbarRec.y, fmin(newThumbY, scrollbarRec.y + scrollbarRec.height - thumbHeight));

            // Update the scroll start based on the new thumb position
            float scrollRatio = (newThumbY - scrollbarRec.y)/(scrollbarRec.height - thumbHeight);
            *start = round(scrollRatio*(max - 5));
            *end = *start + 5;
        }

        // Draw dropdown items
        for (unsigned i = 0, j = *start; i < max && j < *end; i++, j++)
        {
            Rectangle itemRec = { rec.x, rec.y + rec.height*i + rec.height, rec.width, rec.height };

            if (GuiButton(itemRec, v[j]))
            {
                *index = j;

                return false;
            }

            if (IsMousePressed() && !CheckCollisionPointRec(mousePos, editModeRec))
            {
                return false;
            }
        }

        // Debug rec
        //DrawRectangleRec(editModeRec, RED);
    }

    return true;
}

//----------------------------------------------------------------

//----------------------------------------------------------------

void LoadRobot(bool* flag)
{
#if debug
    if (IsKeyPressed(KEY_K)) *flag = true;
#endif
}

//----------------------------------------------------------------

BoneColor InitBoneColor(Color circleColor, Color cubeColor, Color baseLineColor)
{
    BoneColor colors = { 0 }; 
    colors.circleColor   = circleColor;
    colors.cubeColor     = cubeColor;
    colors.baseLineColor = baseLineColor;

    return colors;
}

//----------------------------------------------------------------

int main()
{
    /* Window */
    
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "GLTF Viewer");
    SetTargetFPS(60);
    SetExitKey(0);

    /* Model */

    Model* model = NULL;
    ModelAnimation* modelAnimation = NULL;

    int animsCount = 0;
    unsigned animCurrentFrame = 0;
    unsigned animIndex = 0;

    Vector3 modelPos = Vector3Zero();
    Vector3 modelRot = Vector3Zero();
    Vector3 modelScl = Vector3One();

    /* Camera */
    
    Camera camera = CreateCamera();

    /* GUI */

    // Custom file dialog
    GuiWindowFileDialogState fileDialogState = InitGuiWindowFileDialog(GetWorkingDirectory());

    char fileNameToLoad[512] = { 0 };

    bool warningMessage = false;

    //----------------------------------------------------------------
    float maxScl = 2.0f;

    const char* maxSclOptions = "1.0f;2.0f;3.0f"; 
    int maxSclActiveOption = 1;   
    bool maxSclDropdownEditMode = false;

    //----------------------------------------------------------------
    int targetFPS = 60;

    const char* targetFPSOptions = "30;60;120"; 
    int targetFPSActiveOption = 1;   
    bool targetFPSDropdownEditMode = false;

    //----------------------------------------------------------------
    char** animName = (char**)vector_create();
    char** animNameSlice = (char**)vector_create();

    /*.....................................*/
    char* animNameOptions = " ";
    int animNameActiveOption = 0;   
    bool animNameDropdownEditMode = false;

    /*.....................................*/
    unsigned animDropdownStart = 0;
    unsigned animDropdownEnd = 5;
    bool animDropdownIsDragging = false;

    bool isPlayAnimation = true;
    float currentFrame = 0.0f;

    /*.....................................*/
    ScrollbarColor animScrollbarColor = { 0 };
    BoneColor animBoneColor = InitBoneColor(LIME, GREEN, BLUE);
    
    animScrollbarColor.idleColor = DARKGRAY;
    animScrollbarColor.hoverColor = CBLUE;
    animScrollbarColor.draggedColor = DBLUE;
    animScrollbarColor.backgroundColor = LIGHTGRAY;
    
    /*.....................................*/
    bool isUpdateBoneCircleColor = false;
    bool isUpdateBoneCubeColor = false;
    bool isUpdateBoneBaseLineColor = false;

    /*.....................................*/
    bool isAnimDrawCubes = true;
    bool isAnimDrawCircles = true;
    bool isAnimDrawMainWires = true;
    bool isDrawAnimTransform = true;

    /*.....................................*/
    bool isAnimColorUpdate = false;

    //----------------------------------------------------------------
    float lastX = GetMouseX();
    float lastY = GetMouseY();

    Vector3 gizmoX = { 0.0f + 4.0f, 0.0f, 0.0f };
    Vector3 gizmoY = { 0.0f, 0.0f + 4.0f, 0.0f };
    Vector3 gizmoZ = { 0.0f, 0.0f, 0.0f + 4.0f };

    float gizmoRad = 0.5f;

    bool isGizmoMod = false;

    bool gizmoXYZColors[3] = { false, false, false };

    bool isDrawWires = false;

    bool loadFromKey = false; 

    while (!WindowShouldClose())
    {
        /* Update functions */

        //----------------------------------------------------------------
                            /* Camera */
        //----------------------------------------------------------------

        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            UpdateCamera(&camera, CAMERA_FREE);
        }

        //----------------------------------------------------------------
                            /* Gizmo */
        //----------------------------------------------------------------

        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            for (unsigned i = 0; i < 3; i++)
            {
                gizmoXYZColors[i] = false;
            }
        }

        isGizmoMod = false;

        float currentX = GetMouseX();
        float currentY = GetMouseY();

        // Get the current mouse position
        Vector2 mousePosition = GetMousePosition();
        Ray ray = GetMouseRay(mousePosition, camera);

        /*
        * Gizmo point x
        */

        Vector3 toPointX = Vector3Subtract(gizmoX, ray.position);
        float projectionX = Vector3DotProduct(ray.direction, toPointX);

        if (projectionX < 0)
        {
            projectionX = 0;
        }

        // Find the closest point on the ray to the point center
        Vector3 closestPointX = Vector3Add(ray.position, Vector3Scale(ray.direction, projectionX));
        float distanceX = Vector3Distance(closestPointX, gizmoX);

        // Check if the mouse is touching the point
        if (distanceX <= gizmoRad && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            isGizmoMod = true;

            gizmoXYZColors[0] = true;

            if (modelPos.z < camera.position.z)
            {
                // Move the gizmo based on mouse movement
                if (currentX < lastX) 
                {
                    gizmoX.x -= 0.1f;
                    gizmoY.x -= 0.1f;
                    gizmoZ.x -= 0.1f;
                } 
                else if (currentX > lastX) 
                {
                    gizmoX.x += 0.1f;
                    gizmoY.x += 0.1f;
                    gizmoZ.x += 0.1f;
                }
            }
            else
            {
                // Move the gizmo based on mouse movement
                if (currentX < lastX) 
                {
                    gizmoX.x += 0.1f;
                    gizmoY.x += 0.1f;
                    gizmoZ.x += 0.1f;
                } 
                else if (currentX > lastX) 
                {
                    gizmoX.x -= 0.1f;
                    gizmoY.x -= 0.1f;
                    gizmoZ.x -= 0.1f;
                }
            }
        }

        /*
        * Gizmo point y
        */

        Vector3 toPointY = Vector3Subtract(gizmoY, ray.position);
        float projectionY = Vector3DotProduct(ray.direction, toPointY);

        if (projectionY < 0)
        {
            projectionY = 0;
        }

        // Find the closest point on the ray to the point center
        Vector3 closestPointY = Vector3Add(ray.position, Vector3Scale(ray.direction, projectionY));
        float distanceY = Vector3Distance(closestPointY, gizmoY);

        // Check if the mouse is touching the point
        if (distanceY <= gizmoRad && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            isGizmoMod = true;

            gizmoXYZColors[1] = true;
        
            // Move the gizmo based on mouse movement
            if (currentY < lastY) 
            {
                gizmoX.y += 0.1f;
                gizmoY.y += 0.1f;
                gizmoZ.y += 0.1f;
            } 
            else if (currentY > lastY) 
            {
                gizmoX.y -= 0.1f;
                gizmoY.y -= 0.1f;
                gizmoZ.y -= 0.1f;
            }
        }

        /*
        * Gizmo point z
        */

        Vector3 toPointZ = Vector3Subtract(gizmoZ, ray.position);
        float projectionZ = Vector3DotProduct(ray.direction, toPointZ);

        if (projectionZ < 0)
        {
            projectionZ = 0;
        }

        // Find the closest point on the ray to the point center
        Vector3 closestPointZ = Vector3Add(ray.position, Vector3Scale(ray.direction, projectionZ));
        float distanceZ = Vector3Distance(closestPointZ, gizmoZ);

        // Check if the mouse is touching the point
        if (distanceZ <= gizmoRad && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            isGizmoMod = true;

            gizmoXYZColors[2] = true;

            if (modelPos.x < camera.position.x)
            {
                // Move the gizmo based on mouse movement
                if (currentX < lastX) 
                {
                    gizmoX.z += 0.1f;
                    gizmoY.z += 0.1f;
                    gizmoZ.z += 0.1f;
                } 
                else if (currentX > lastX) 
                {
                    gizmoX.z -= 0.1f;
                    gizmoY.z -= 0.1f;
                    gizmoZ.z -= 0.1f;
                }
                else if (currentY < lastY) 
                {
                    gizmoX.z -= 0.1f;
                    gizmoY.z -= 0.1f;
                    gizmoZ.z -= 0.1f;
                } 
                else if (currentY > lastY) 
                {
                    gizmoX.z += 0.1f;
                    gizmoY.z += 0.1f;
                    gizmoZ.z += 0.1f;
                }
            }
            else
            {
                // Move the gizmo based on mouse movement
                if (currentX < lastX) 
                {
                    gizmoX.z -= 0.1f;
                    gizmoY.z -= 0.1f;
                    gizmoZ.z -= 0.1f;
                } 
                else if (currentX > lastX) 
                {
                    gizmoX.z += 0.1f;
                    gizmoY.z += 0.1f;
                    gizmoZ.z += 0.1f;
                }
                else if (currentY < lastY) 
                {
                    gizmoX.z += 0.1f;
                    gizmoY.z += 0.1f;
                    gizmoZ.z += 0.1f;
                } 
                else if (currentY > lastY) 
                {
                    gizmoX.z -= 0.1f;
                    gizmoY.z -= 0.1f;
                    gizmoZ.z -= 0.1f;
                }
            }
        }

        lastX = currentX; // Update lastX to current position
        lastY = currentY; // Update lastY to current position

        //----------------------------------------------------------------
                            /* Load model button */
        //----------------------------------------------------------------

        LoadRobot(&loadFromKey);

        if (fileDialogState.SelectFilePressed)
        {
            // Load model file (if supported extension)
            if (IsFileExtension(fileDialogState.fileNameText, ".glb") || IsFileExtension(fileDialogState.fileNameText, ".gltf"))
            {
                strcpy(fileNameToLoad, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState.dirPathText, fileDialogState.fileNameText));
                
                if (model != NULL)
                {
                    if (animsCount > 0)
                    {
                        UnloadModelAnimations(modelAnimation, animsCount);
                        free(animNameOptions);
                        vector_free(animName);
                    }
                    
                    UnloadModel(*model);
                    free(model);

                    currentFrame = 0.0f;
                    animNameOptions = " ";
                    animNameActiveOption = 0; 
                    animsCount = 0;
                }

                animName = (char**)vector_create();
                model = (Model*)malloc(sizeof(Model));
                *model = LoadModel(fileNameToLoad);
                modelAnimation = LoadModelAnimations(fileNameToLoad, &animsCount);

                if (animsCount > 0)
                {
                    if (vector_size(animName) < 8)
                    {
                        for (unsigned i = 0; i < animsCount; i++)
                        {
                            char* name = strdup(modelAnimation[i].name); // Duplicate the string
                            vector_add(&animName, name); // Add the duplicated name to the vector
                            
                            TraceLog(LOG_INFO, "Animation %d: %s", i, name);
                        }

                        // Calculate the size of the new string
                        size_t totalLength = 0;
                        for (unsigned i = 0; i < animsCount; i++)
                        {
                            totalLength += strlen(animName[i]);
                        }
                        totalLength += animsCount - 1; // For the semicolons
                        totalLength += 1; // For the null terminator

                        // Allocate memory for the new concatenated string
                        animNameOptions = (char*)malloc(totalLength * sizeof(char));
                        
                        assert(animNameOptions != NULL);

                        strcpy(animNameOptions, animName[0]); // Copy the first name

                        // Concatenate the rest of the names
                        for (unsigned i = 1; i < animsCount; i++)
                        {
                            strcat(animNameOptions, ";");
                            strcat(animNameOptions, animName[i]);
                        }
                    }
                    else
                    {
                        for (unsigned i = 0; i < animsCount; i++)
                        {
                            char* name = strdup(modelAnimation[i].name); // Duplicate the string
                            vector_add(&animNameSlice, name); // Add the duplicated name to the vector
                            
                            TraceLog(LOG_INFO, "Animation %d: %s", i, name);
                        }

                        // Calculate the size of the new string
                        size_t totalLength = 0;
                        for (unsigned i = 0; i < animsCount; i++)
                        {
                            totalLength += strlen(animNameSlice[i]);
                        }
                        totalLength += animsCount - 1; // For the semicolons
                        totalLength += 1; // For the null terminator

                        // Allocate memory for the new concatenated string
                        animNameOptions = (char*)malloc(totalLength * sizeof(char));
                        
                        assert(animNameOptions != NULL);

                        strcpy(animNameOptions, animNameSlice[0]); // Copy the first name

                        // Concatenate the rest of the names
                        for (unsigned i = 1; i < animsCount; i++)
                        {
                            strcat(animNameOptions, ";");
                            strcat(animNameOptions, animNameSlice[i]);
                        }
                    }
                }
            }
            else
            {
                warningMessage = true;
            }

            fileDialogState.SelectFilePressed = false;
        }

        if (loadFromKey)
        {
            if (model != NULL)
            {
                if (animsCount > 0)
                {
                    UnloadModelAnimations(modelAnimation, animsCount);
                    free(animNameOptions);
                    vector_free(animName);
                }
                
                UnloadModel(*model);
                free(model);

                currentFrame = 0.0f;
                animNameOptions = " ";
                animNameActiveOption = 0; 
                animsCount = 0;
            }

            animName = (char**)vector_create();
            model = (Model*)malloc(sizeof(Model));
            *model = LoadModel("./robot.glb");
            modelAnimation = LoadModelAnimations("./robot.glb", &animsCount);

            if (vector_size(animName) < 8)
            {
                for (unsigned i = 0; i < animsCount; i++)
                {
                    char* name = strdup(modelAnimation[i].name); // Duplicate the string
                    vector_add(&animName, name); // Add the duplicated name to the vector
                    
                    TraceLog(LOG_INFO, "Animation %d: %s", i, name);
                }

                // Calculate the size of the new string
                size_t totalLength = 0;
                for (unsigned i = 0; i < animsCount; i++)
                {
                    totalLength += strlen(animName[i]);
                }
                totalLength += animsCount - 1; // For the semicolons
                totalLength += 1; // For the null terminator

                // Allocate memory for the new concatenated string
                animNameOptions = (char*)malloc(totalLength * sizeof(char));
                
                assert(animNameOptions != NULL);

                strcpy(animNameOptions, animName[0]); // Copy the first name

                // Concatenate the rest of the names
                for (unsigned i = 1; i < animsCount; i++)
                {
                    strcat(animNameOptions, ";");
                    strcat(animNameOptions, animName[i]);
                }
            }
            else
            {
                for (unsigned i = 0; i < animsCount; i++)
                {
                    char* name = strdup(modelAnimation[i].name); // Duplicate the string
                    vector_add(&animNameSlice, name); // Add the duplicated name to the vector
                    
                    TraceLog(LOG_INFO, "Animation %d: %s", i, name);
                }

                // Calculate the size of the new string
                size_t totalLength = 0;
                for (unsigned i = 0; i < animsCount; i++)
                {
                    totalLength += strlen(animNameSlice[i]);
                }
                totalLength += animsCount - 1; // For the semicolons
                totalLength += 1; // For the null terminator

                // Allocate memory for the new concatenated string
                animNameOptions = (char*)malloc(totalLength * sizeof(char));
                
                assert(animNameOptions != NULL);

                strcpy(animNameOptions, animNameSlice[0]); // Copy the first name

                // Concatenate the rest of the names
                for (unsigned i = 1; i < animsCount; i++)
                {
                    strcat(animNameOptions, ";");
                    strcat(animNameOptions, animNameSlice[i]);
                }
            }

            loadFromKey = false;
        }

        //----------------------------------------------------------------
                            /* Model */
        //----------------------------------------------------------------

        // 0 = 1.0f, 1 = 2.0f, 2 = 3.0f
        maxScl = (float)maxSclActiveOption + 1.0f;

        /* Draw functions */

        BeginDrawing();

        BeginMode3D(camera);
        ClearBackground(BLACK);
        DrawGrid(40, 1.0f);

        float gizmoSize = gizmoRad*10.0f - 1.0f;
        DrawGizmo(&modelPos, &gizmoX, &gizmoY, &gizmoZ, gizmoSize, gizmoXYZColors, isGizmoMod);
        
        if (model != NULL)
        {
            if (isDrawWires)
            {
                if (isAnimDrawMainWires)
                {
                    DrawModelWiresPro(*model, modelPos, modelRot, modelScl);
                }

                if (animsCount > 0)
                {
                    DrawModelBones(
                        *model, 
                        modelAnimation, 
                        animIndex, 
                        animCurrentFrame, 
                        modelPos, 
                        modelRot, 
                        modelScl,
                        isAnimDrawCircles,
                        isAnimDrawCubes, 
                        isDrawAnimTransform,
                        animBoneColor
                    );
                }
            }
            else
            {
                DrawModelPro(*model, modelPos, modelRot, modelScl);
            }
        }

        EndMode3D();

        DrawFPS(0, 0);

        /* Transform */

        const int uiTranformsLeft = screenWidth - 200;
        GuiGroupBox((Rectangle){ uiTranformsLeft, 20, 180, 300 }, "Transform");

        //----------------------------------------------------------------
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 50 + 20*0, 100, 15 }, 
            "PosX", 
            TextFormat("%3.2f", modelPos.x), 
            &modelPos.x, 
            -50.0f, 
            50.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 50 + 20*1, 100, 15 }, 
            "PosY", 
            TextFormat("%3.2f", modelPos.y), 
            &modelPos.y, 
            -50.0f, 
            50.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 50 + 20*2, 100, 15 }, 
            "PosZ", 
            TextFormat("%3.2f", modelPos.z), 
            &modelPos.z, 
            -50.0f, 
            50.0f
        );

        //----------------------------------------------------------------
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20*3, 100, 15 }, 
            "RotX", 
            TextFormat("%3.2f", modelRot.x), 
            &modelRot.x, 
            -360.0f, 
            360.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20*4, 100, 15 }, 
            "RotY", 
            TextFormat("%3.2f", modelRot.y), 
            &modelRot.y, 
            -360.0f, 
            360.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20*5, 100, 15 }, 
            "RotZ", 
            TextFormat("%3.2f", modelRot.z), 
            &modelRot.z, 
            -360.0f, 
            360.0f
        );

        //----------------------------------------------------------------
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20*6, 100, 15 }, 
            "SclX", 
            TextFormat("%3.2f", modelScl.x), 
            &modelScl.x, 
            0.01f, 
            maxScl
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20*7, 100, 15 }, 
            "SclY", 
            TextFormat("%3.2f", modelScl.y), 
            &modelScl.y, 
            0.01f, 
            maxScl
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20*8, 100, 15 }, 
            "SclZ", 
            TextFormat("%3.2f", modelScl.z), 
            &modelScl.z, 
            0.01f, 
            maxScl
        );

        if (GuiButton((Rectangle){ uiTranformsLeft + 5, 280, 70, 25 }, "Reset Scale"))
        {
            modelScl = (Vector3){ 1.0f, 1.0f, 1.0f };
        }

        if (GuiButton((Rectangle){ uiTranformsLeft + 80, 280, 95, 25 }, "Reset Transform"))
        {
            modelPos = Vector3Zero();
            modelRot = Vector3Zero();
            modelScl = Vector3One();
        }

        /* Settings */

        const int uiSettingsLeft = screenWidth - 200;
        GuiGroupBox((Rectangle){ uiSettingsLeft, 350, 180, 220 }, "Settings");

        GuiDrawText("Max Scale:", (Rectangle){ uiSettingsLeft + 10, 360, 100, 20 }, 0, GRAY);

        // Draw the dropdown box
        GuiDropdownBox((Rectangle){ uiSettingsLeft + 40, 380, 100, 20 }, maxSclOptions, &maxSclActiveOption, maxSclDropdownEditMode);

        // Check if dropdown box is in edit mode (expanded)
        if (maxSclDropdownEditMode)
        {
            maxSclDropdownEditMode = !IsMousePressed();

            // Check for clicks on each option when expanded
            for (unsigned i = 0; i < 3; i++)
            {
                // Calculate the rectangle for each dropdown option
                Rectangle optionRect = { uiSettingsLeft + 40, 380 + 20*(i + 1), 100, 20 };

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                    CheckCollisionPointRec(GetMousePosition(), optionRect))
                {
                    // Set active option based on which one was clicked
                    maxSclActiveOption = i;
                    maxSclDropdownEditMode = false;  // Close the dropdown
                    break;
                }
            }
        }
        else
        {
            // Toggle dropdown edit mode when clicking the top rectangle (collapsed dropdown)
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                CheckCollisionPointRec(GetMousePosition(), (Rectangle){ uiSettingsLeft + 40, 380, 100, 20 }))
            {
                maxSclDropdownEditMode = !maxSclDropdownEditMode;  // Open dropdown
            }
        }

        if (!maxSclDropdownEditMode)
        {
            GuiDrawText("Target FPS:", (Rectangle){ uiSettingsLeft + 10, 360 + 50, 100, 20 }, 0, GRAY);

            // Draw the dropdown box
            GuiDropdownBox((Rectangle){ uiSettingsLeft + 40, 430, 100, 20 }, targetFPSOptions, &targetFPSActiveOption, targetFPSDropdownEditMode);

            // Check if dropdown box is in edit mode (expanded)
            if (targetFPSDropdownEditMode)
            {
                targetFPSDropdownEditMode = !IsMousePressed();

                // Check for clicks on each option when expanded
                for (unsigned i = 0; i < 3; i++)
                {
                    // Calculate the rectangle for each dropdown option
                    Rectangle optionRect = { uiSettingsLeft + 40, 430 + 20*(i + 1), 100, 20 };

                    // 0 = 30, 1 = 60, 2 = 120
                    targetFPS = (i == 0) ? 30 : (i == 1) ? 60 : 120;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                        CheckCollisionPointRec(GetMousePosition(), optionRect))
                    {
                        // Set active option based on which one was clicked
                        targetFPSActiveOption = i;
                        targetFPSDropdownEditMode = false;  // Close the dropdown
                        SetTargetFPS(targetFPS);
                        break;
                    }
                }
            }
            else
            {
                // Toggle dropdown edit mode when clicking the top rectangle (collapsed dropdown)
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                    CheckCollisionPointRec(GetMousePosition(), (Rectangle){ uiSettingsLeft + 40, 430, 100, 20 }))
                {
                    targetFPSDropdownEditMode = !targetFPSDropdownEditMode;  // Open dropdown
                }
            }
        }

        /* Animation */
        
        if (!maxSclDropdownEditMode && !targetFPSDropdownEditMode)
        {
            GuiDrawText("Current Animation:", (Rectangle){ uiSettingsLeft + 10, 410 + 50, 100, 20 }, 0, GRAY);

            if (vector_size(animName) < 8)
            {
                // Draw the dropdown box
                GuiDropdownBox(
                    (Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }, 
                    animNameOptions, 
                    &animNameActiveOption, 
                    animNameDropdownEditMode
                );
                
                // Check if dropdown box is in edit mode (expanded)
                if (animNameDropdownEditMode)
                {
                    animNameDropdownEditMode = !IsMousePressed();

                    // Check for clicks on each option when expanded
                    for (unsigned i = 0; i < animsCount; i++)
                    {
                        // Calculate the rectangle for each dropdown option
                        Rectangle optionRect = { uiSettingsLeft + 40, 480 + 20*(i + 1), 100, 20 };

                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                            CheckCollisionPointRec(GetMousePosition(), optionRect))
                        {
                            // Set active option based on which one was clicked
                            animNameActiveOption = i;
                            animNameDropdownEditMode = false;  // Close the dropdown
                            animIndex = i;
                            break;
                        }
                    }
                }
                else
                {
                    // Toggle dropdown edit mode when clicking the top rectangle (collapsed dropdown)
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                        CheckCollisionPointRec(GetMousePosition(), (Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }))
                    {
                        animNameDropdownEditMode = !animNameDropdownEditMode;  // Open dropdown
                    }
                }
            }
            else
            {
                // Check if dropdown box is in edit mode (expanded)
                if (animNameDropdownEditMode)
                {
                    bool editMode = GuiDropdownPro(
                        (Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }, 
                        animName, 
                        &animDropdownStart, 
                        &animDropdownEnd, 
                        &animDropdownIsDragging,
                        &animIndex,
                        &animScrollbarColor 
                    );

                    // If mouse is pressed and not in drag mode, set edit mode to false
                    if (!editMode)
                    {
                        animNameOptions = animName[animIndex];
                        animNameDropdownEditMode = false;
                    }

                    // Draw the static dropdown box
                    GuiDropdownBox(
                        (Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }, 
                        animNameOptions, 
                        &animNameActiveOption, 
                        false
                    );
                }
                else
                {
                    // Draw the static dropdown box
                    GuiDropdownBox(
                        (Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }, 
                        animNameOptions, 
                        &animNameActiveOption, 
                        false
                    );

                    // Toggle dropdown edit mode when clicking the top rectangle (collapsed dropdown)
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
                        CheckCollisionPointRec(GetMousePosition(), (Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }))
                    {
                        animNameDropdownEditMode = !animNameDropdownEditMode;  // Open dropdown
                    }
                }
            }
        }

        //----------------------------------------------------------------
        if (!maxSclDropdownEditMode && !targetFPSDropdownEditMode && !animNameDropdownEditMode)
        {
            GuiCheckBox(
                (Rectangle){ uiSettingsLeft + 10, 520, 15, 15 }, 
                "Draw Wires",
                &isDrawWires
            );
        }

        /* Bone view settings */

        //----------------------------------------------------------------
        if (isDrawWires)
        {
            const int bonesViewSettingsLeft = 20;
            GuiGroupBox((Rectangle){ bonesViewSettingsLeft, 100, 120, 125 }, "Bone View Settings");

            GuiCheckBox(
                (Rectangle){ bonesViewSettingsLeft + 4, 110, 15, 15 }, 
                "Draw Cubes",
                &isAnimDrawCubes
            );

            GuiCheckBox(
                (Rectangle){ bonesViewSettingsLeft + 4, 130, 15, 15 }, 
                "Draw Circles",
                &isAnimDrawCircles
            );

            GuiCheckBox(
                (Rectangle){ bonesViewSettingsLeft + 4, 150, 15, 15 }, 
                "Draw Main Wires",
                &isAnimDrawMainWires
            );

            GuiCheckBox(
                (Rectangle){ bonesViewSettingsLeft + 4, 170, 15, 15 }, 
                "Draw Transform",
                &isDrawAnimTransform
            );

            if (GuiButton((Rectangle){ bonesViewSettingsLeft + 4, 200, 80, 20 }, "Change Color"))
            {
                isAnimColorUpdate = !isAnimColorUpdate;
            }

            if (isAnimColorUpdate)
            {
                const int bonesColorUpdateLeft = 150;
                GuiGroupBox((Rectangle){ bonesColorUpdateLeft, 100, 120, 125 }, "Bone Colors");

                GuiDrawText("Circle Color", (Rectangle){ bonesColorUpdateLeft + 5, 105, 120, 20 }, 0, GRAY);
                DrawRectangleRec((Rectangle){ bonesColorUpdateLeft + 5, 125, 65, 16 }, animBoneColor.circleColor);

                GuiDrawText("Cube Color", (Rectangle){ bonesColorUpdateLeft + 5, 145, 120, 20 }, 0, GRAY);
                DrawRectangleRec((Rectangle){ bonesColorUpdateLeft + 5, 165, 65, 16 }, animBoneColor.cubeColor);
                
                GuiDrawText("Base Bone Color", (Rectangle){ bonesColorUpdateLeft + 5, 185, 120, 20 }, 0, GRAY);
                DrawRectangleRec((Rectangle){ bonesColorUpdateLeft + 5, 205, 65, 16 }, animBoneColor.baseLineColor);

                if (GuiButton((Rectangle){ bonesColorUpdateLeft + 77, 125, 40, 17 }, "Update") 
                    && !isUpdateBoneCircleColor && !isUpdateBoneCubeColor && !isUpdateBoneBaseLineColor)
                {
                    isUpdateBoneCircleColor = !isUpdateBoneCircleColor;
                }
                if (GuiButton((Rectangle){ bonesColorUpdateLeft + 77, 165, 40, 17 }, "Update")
                    && !isUpdateBoneCircleColor && !isUpdateBoneCubeColor && !isUpdateBoneBaseLineColor)
                {
                    isUpdateBoneCubeColor = !isUpdateBoneCubeColor;
                }
                if (GuiButton((Rectangle){ bonesColorUpdateLeft + 77, 205, 40, 17 }, "Update")
                    && !isUpdateBoneCircleColor && !isUpdateBoneCubeColor && !isUpdateBoneBaseLineColor)
                {
                    isUpdateBoneBaseLineColor = !isUpdateBoneBaseLineColor;
                }
            }

            if (isUpdateBoneCircleColor && !isUpdateBoneCubeColor && !isUpdateBoneBaseLineColor)
            {
                GuiColorPicker((Rectangle){ 280, 100, 140, 140 }, " ", &animBoneColor.circleColor);

                if (GuiButton((Rectangle){ 380, 245, 40, 25 }, "OK"))
                {
                    isUpdateBoneCircleColor = !isUpdateBoneCircleColor;
                }
            }

            if (!isUpdateBoneCircleColor && isUpdateBoneCubeColor && !isUpdateBoneBaseLineColor)
            {
                GuiColorPicker((Rectangle){ 280, 100, 140, 140 }, " ", &animBoneColor.cubeColor);

                if (GuiButton((Rectangle){ 380, 245, 40, 25 }, "OK"))
                {
                    isUpdateBoneCubeColor = !isUpdateBoneCubeColor;
                }
            }

            if (!isUpdateBoneCircleColor && !isUpdateBoneCubeColor && isUpdateBoneBaseLineColor)
            {
                GuiColorPicker((Rectangle){ 280, 100, 140, 140 }, " ", &animBoneColor.baseLineColor);

                if (GuiButton((Rectangle){ 380, 245, 40, 25 }, "OK"))
                {
                    isUpdateBoneBaseLineColor = !isUpdateBoneBaseLineColor;
                }
            }
        }

        //----------------------------------------------------------------
        if (animsCount > 0)
        {
            if (model != NULL)
            {
                const ModelAnimation anim = modelAnimation[animIndex];

                if (isPlayAnimation)
                {
                    if (anim.frameCount > 0)
                    {
                        animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount;
                        UpdateModelAnimation(*model, anim, animCurrentFrame);
                    }
                    else
                    {
                        animCurrentFrame = 0;
                    }

                    currentFrame = animCurrentFrame;
                }
                else
                {
                    animCurrentFrame = (unsigned)currentFrame;
                    UpdateModelAnimation(*model, anim, animCurrentFrame);
                }
                
                //----------------------------------------------------------------
                if (!maxSclDropdownEditMode && !targetFPSDropdownEditMode && !animNameDropdownEditMode)
                {
                    GuiSliderBar(
                        (Rectangle){ 50, screenHeight - 80, 900, 35 }, 
                        "Frame:", 
                        TextFormat("%3.2f", currentFrame), 
                        &currentFrame, 
                        0, 
                        anim.frameCount
                    );

                    if (GuiButton((Rectangle){ screenWidth - 90, screenHeight - 80, 75, 30 }, 
                        (isPlayAnimation) ? GuiIconText(ICON_PLAYER_PAUSE, "PAUSE") : GuiIconText(ICON_PLAYER_PLAY, "PLAY")))
                    {
                        isPlayAnimation = !isPlayAnimation;
                    }
                }
            }
        }

        //----------------------------------------------------------------
        if (fileDialogState.windowActive)
        {
            GuiLock();
        }

        if (GuiButton((Rectangle){ 20, 20, 140, 30 }, GuiIconText(ICON_FILE_OPEN, "Load Model")))
        {
            fileDialogState.windowActive = true;
        }

        GuiUnlock();

        GuiWindowFileDialog(&fileDialogState);

        //----------------------------------------------------------------
        if (warningMessage)
        {
            int result = GuiMessageBox(
                (Rectangle){ screenWidth/2 - 100, screenHeight/2 - 100, 250, 100 },
                    "#191#Message Box", 
                    "The file should be a .glb or .gltf file.", 
                    "OK"
            );
            
            if (result >= 0)
            {
                warningMessage = false;
            }
        }

        EndDrawing();
    }

    //----------------------------------------------------------------
    if (model != NULL)
    {
        if (animsCount > 0)
        {
            UnloadModelAnimations(modelAnimation, animsCount);
            free(animNameOptions);
            vector_free(animName);
        }

        UnloadModel(*model);
        free(model);
    }

    CloseWindow();

    return 0;
}
