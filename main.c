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
#include "assert.h"
#include "vec.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui-4.0/src/raygui.h"

#undef RAYGUI_IMPLEMENTATION // Avoid including raygui implementation again
#define GUI_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "gui_window_file_dialog.h"

#define debug true

const int screenWidth = 1080;
const int screenHeight = 720;

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

Vector3 Vector3Multiply(Vector3 v, float scalar)
{
    return (Vector3){ v.x*scalar, v.y*scalar, v.z*scalar };
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

void DrawModelBones(Model model, ModelAnimation* anims, unsigned animIndex, unsigned animCurrentFrame, Vector3 pos, Vector3 rot, Vector3 scl)
{
    Matrix rotationMatrix = MatrixRotateV(rot);

    for (unsigned i = 0; i < model.boneCount-1; i++)
    {
        // Get the current bone translation and apply scaling
        Vector3 translation = anims[animIndex].framePoses[animCurrentFrame][i].translation;
        Vector3 finalTranslation = Vector3Transform(Vector3Multiply(translation, scl), rotationMatrix);
        finalTranslation = Vector3Add(finalTranslation, pos); // Final transformed position

        DrawCubeV(finalTranslation, Vector3Multiply(scl, 0.1f), GREEN);

        int parentIndex = anims[animIndex].bones[i].parent;
        if (parentIndex >= 0)
        {
            // Get the parent's translation and apply transformations
            Vector3 parentTranslation = anims[animIndex].framePoses[animCurrentFrame][parentIndex].translation;
            Vector3 parentFinalTranslation = Vector3Transform(Vector3Multiply(parentTranslation, scl), rotationMatrix);
            parentFinalTranslation = Vector3Add(parentFinalTranslation, pos); // Final transformed position for parent

            // Draw a line between the bone and its parent
            DrawLine3D(finalTranslation, parentFinalTranslation, BLUE);
        }
    }
}

//----------------------------------------------------------------

//----------------------------------------------------------------

bool IsMousePressed()
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
}

bool GuiDropdownPro(Rectangle rec, char** v, unsigned* start, unsigned* end, bool* isDragging, unsigned* index)
{
    const int max = vector_size(v);

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

    // Draw dropdown items
    bool isSetIndex = false;
    for (unsigned i = 0, j = *start; i < max && j < *end; i++, j++)
    {
        Rectangle itemRec = { rec.x, rec.y + rec.height*i, rec.width, rec.height };

        if (GuiButton(itemRec, v[j]))
        {
            isSetIndex = true;
            *index = j;
        }
    }

    // Draw scrollbar if the number of items exceeds the visible range
    if (max > 5)
    {
        // Scrollbar background
        Rectangle scrollbarRec = { rec.x + rec.width + 1, rec.y, 10, rec.height*5 };
        DrawRectangleRec(scrollbarRec, LIGHTGRAY);

        // Calculate the height and position of the scroll thumb
        float thumbHeight = (5.0f/max)*scrollbarRec.height;
        float thumbY = scrollbarRec.y + (*start/(float)(max - 5))*(scrollbarRec.height - thumbHeight);
        
        Rectangle thumbRec = { scrollbarRec.x, thumbY, scrollbarRec.width, thumbHeight };
        DrawRectangleRec(thumbRec, DARKGRAY);

        // Handle mouse interaction with scrollbar thumb
        Vector2 mousePos = GetMousePosition();

        float thumbOffsetY = 0.0f;

        // Start dragging if mouse button is pressed within the thumb
        if (CheckCollisionPointRec(mousePos, thumbRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            *isDragging = true;
            thumbOffsetY = mousePos.y - thumbRec.y; // Calculate offset
        }

        bool result = true;

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

        if (IsMousePressed())
        {
            //return *isDragging && isSetIndex;
        }

        return result;
    }
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

int main()
{
    /* Window */
    
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "GLB Viewer");
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

    char* animNameOptions = " ";
    int animNameActiveOption = 0;   
    bool animNameDropdownEditMode = false;

    unsigned animDropdownStart = 0;
    unsigned animDropdownEnd = 5;
    bool animDropdownIsDragging = false;

    bool isPlayAnimation = true;
    float currentFrame = 0.0f;

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
        * Gizmo sphere x
        */

        Vector3 toSphereX = Vector3Subtract(gizmoX, ray.position);
        float projectionX = Vector3DotProduct(ray.direction, toSphereX);

        if (projectionX < 0)
        {
            projectionX = 0;
        }

        // Find the closest point on the ray to the sphere center
        Vector3 closestPointX = Vector3Add(ray.position, Vector3Scale(ray.direction, projectionX));
        float distanceX = Vector3Distance(closestPointX, gizmoX);

        // Check if the mouse is touching the sphere
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
        * Gizmo sphere y
        */

        Vector3 toSphereY = Vector3Subtract(gizmoY, ray.position);
        float projectionY = Vector3DotProduct(ray.direction, toSphereY);

        if (projectionY < 0)
        {
            projectionY = 0;
        }

        // Find the closest point on the ray to the sphere center
        Vector3 closestPointY = Vector3Add(ray.position, Vector3Scale(ray.direction, projectionY));
        float distanceY = Vector3Distance(closestPointY, gizmoY);

        // Check if the mouse is touching the sphere
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
        * Gizmo sphere z
        */

        Vector3 toSphereZ = Vector3Subtract(gizmoZ, ray.position);
        float projectionZ = Vector3DotProduct(ray.direction, toSphereZ);

        if (projectionZ < 0)
        {
            projectionZ = 0;
        }

        // Find the closest point on the ray to the sphere center
        Vector3 closestPointZ = Vector3Add(ray.position, Vector3Scale(ray.direction, projectionZ));
        float distanceZ = Vector3Distance(closestPointZ, gizmoZ);

        // Check if the mouse is touching the sphere
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

        if (isGizmoMod)
        {
            modelPos = (Vector3){ gizmoX.x - 4, gizmoX.y, gizmoX.z };
        }
        else
        {
            gizmoX = (Vector3){ modelPos.x + 4.0f, modelPos.y, modelPos.z };
            gizmoY = (Vector3){ modelPos.x, modelPos.y + 4.0f, modelPos.z };
            gizmoZ = (Vector3){ modelPos.x, modelPos.y, modelPos.z + 4.0f };
        }

        //----------------------------------------------------------------
                            /* Load model button */
        //----------------------------------------------------------------

        LoadRobot(&loadFromKey);

        if (fileDialogState.SelectFilePressed)
        {
            // Load model file (if supported extension)
            if (IsFileExtension(fileDialogState.fileNameText, ".glb"))
            {
                strcpy(fileNameToLoad, TextFormat("%s" PATH_SEPERATOR "%s", fileDialogState.dirPathText, fileDialogState.fileNameText));
                
                if (model != NULL)
                {
                    UnloadModel(*model);
                    free(model);
                    vector_free(animName);
                    currentFrame = 0.0f;
                }

                if (vector_size(animName) < 8)
                {
                    animName = (char**)vector_create();
                    model = (Model*)malloc(sizeof(Model));
                    *model = LoadModel(fileNameToLoad);
                    modelAnimation = LoadModelAnimations(fileNameToLoad, &animsCount);

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
                UnloadModel(*model);
                free(model);
                vector_free(animName);
                currentFrame = 0.0f;
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
        
        if (model != NULL)
        {
            if (isDrawWires)
            {
                DrawModelWiresPro(*model, modelPos, modelRot, modelScl);
                DrawModelBones(*model, modelAnimation, animIndex, animCurrentFrame, modelPos, modelRot, modelScl);
            }
            else
            {
                DrawModelPro(*model, modelPos, modelRot, modelScl);
            }
        }

        DrawLine3D((Vector3){ gizmoX.x - 4, gizmoX.y, gizmoX.z }, gizmoX, (gizmoXYZColors[0]) ? RED : MAROON);
        DrawLine3D((Vector3){ gizmoY.x, gizmoY.y - 4, gizmoY.z }, gizmoY, (gizmoXYZColors[1]) ? GREEN : DARKGREEN);
        DrawLine3D((Vector3){ gizmoZ.x, gizmoZ.y, gizmoZ.z - 4 }, gizmoZ, (gizmoXYZColors[2]) ? BLUE : DARKBLUE);

        DrawSphere(gizmoX, gizmoRad * 0.2f, (gizmoXYZColors[0]) ? RED : MAROON);
        DrawSphere(gizmoY, gizmoRad * 0.2f, (gizmoXYZColors[1]) ? GREEN : DARKGREEN);
        DrawSphere(gizmoZ, gizmoRad * 0.2f, (gizmoXYZColors[2]) ? BLUE : DARKBLUE);

        EndMode3D();

        DrawFPS(0, 0);

        /* Transform */

        const float uiTranformsLeft = screenWidth - 200;
        GuiGroupBox((Rectangle){ uiTranformsLeft, 20, 180, 300 }, "Transform");

        //----------------------------------------------------------------
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 50 + 20 * 0, 100, 15 }, 
            "PosX", 
            TextFormat("%3.2f", modelPos.x), 
            &modelPos.x, 
            -50.0f, 
            50.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 50 + 20 * 1, 100, 15 }, 
            "PosY", 
            TextFormat("%3.2f", modelPos.y), 
            &modelPos.y, 
            -50.0f, 
            50.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 50 + 20 * 2, 100, 15 }, 
            "PosZ", 
            TextFormat("%3.2f", modelPos.z), 
            &modelPos.z, 
            -50.0f, 
            50.0f
        );

        //----------------------------------------------------------------
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20 * 3, 100, 15 }, 
            "RotX", 
            TextFormat("%3.2f", modelRot.x), 
            &modelRot.x, 
            -360.0f, 
            360.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20 * 4, 100, 15 }, 
            "RotY", 
            TextFormat("%3.2f", modelRot.y), 
            &modelRot.y, 
            -360.0f, 
            360.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20 * 5, 100, 15 }, 
            "RotZ", 
            TextFormat("%3.2f", modelRot.z), 
            &modelRot.z, 
            -360.0f, 
            360.0f
        );

        //----------------------------------------------------------------
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20 * 6, 100, 15 }, 
            "SclX", 
            TextFormat("%3.2f", modelScl.x), 
            &modelScl.x, 
            0.01f, 
            maxScl
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20 * 7, 100, 15 }, 
            "SclY", 
            TextFormat("%3.2f", modelScl.y), 
            &modelScl.y, 
            0.01f, 
            maxScl
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20 * 8, 100, 15 }, 
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

        const float uiSettingsLeft = screenWidth - 200;
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
                        &animIndex
                    );

                    // If mouse is pressed and not in drag mode, set edit mode to false
                    if (!editMode)
                    {
                        animNameDropdownEditMode = false;
                    }
                }
                else
                {
                    // Draw the dropdown box
                    GuiDropdownBox(
                        (Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }, 
                        animNameOptions, 
                        &animNameActiveOption, 
                        animNameDropdownEditMode
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

        //----------------------------------------------------------------
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
                    "The file should be a .glb file.", 
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
        UnloadModelAnimations(modelAnimation, animsCount);
        UnloadModel(*model);
        free(model);
        free(animNameOptions);
        vector_free(animName);
    }

    CloseWindow();

    return 0;
}
