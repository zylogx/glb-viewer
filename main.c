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

const int screenWidth = 1080;
const int screenHeight = 720;

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
    Matrix result = { 1.0f, 0.0f, 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f };

    float cosz = cosf(-angle.z);
    float sinz = sinf(-angle.z);
    float cosy = cosf(-angle.y);
    float siny = sinf(-angle.y);
    float cosx = cosf(-angle.x);
    float sinx = sinf(-angle.x);

    result.m0 = cosz*cosy;
    result.m1 = (cosz*siny*sinx) - (sinz*cosx);
    result.m2 = (cosz*siny*cosx) + (sinz*sinx);

    result.m4 = sinz*cosy;
    result.m5 = (sinz*siny*sinx) + (cosz*cosx);
    result.m6 = (sinz*siny*cosx) - (cosz*sinx);

    result.m8 = -siny;
    result.m9 = cosy*sinx;
    result.m10= cosy*cosx;

    return result;
}

Vector3 Vector3Zero() 
{ 
    return (Vector3){ 0.0f, 0.0f, 0.0f }; 
}

Vector3 Vector3One() 
{ 
    return (Vector3){ 1.0f, 1.0f, 1.0f }; 
}

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

void DrawModelPro(Model model, Vector3 pos, Vector3 rot, Vector3 scl)
{
    model.transform = MatrixMultiply(MatrixTranslateV(pos), MatrixMultiply(MatrixRotateXYZ(rot), MatrixScaleV(scl)));
    DrawModel(model, Vector3Zero(), 1.0f, WHITE);
}

bool IsMousePressed()
{
    return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON);
}

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

    char* animNameOptions = " ";
    int animNameActiveOption = 0;   
    bool animNameDropdownEditMode = false;

    bool isPlayAnimation = true;
    float currentFrame = 0.0f;

    while (!WindowShouldClose())
    {
        /* Update functions */
        
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            UpdateCamera(&camera, CAMERA_FREE);
        }

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
                warningMessage = true;
            }

            fileDialogState.SelectFilePressed = false;
        }

        // 0 = 1.0f, 1 = 2.0f, 2 = 3.0f
        maxScl = (maxSclActiveOption == 0) ? 1.0f : (maxSclActiveOption == 1) ? 2.0f : 3.0f;

        /* Draw functions */

        BeginDrawing();

        BeginMode3D(camera);
        ClearBackground(BLACK);
        DrawGrid(40, 1.0f);
        
        if (model != NULL)
        {
            DrawModelPro(*model, modelPos, modelRot, modelScl);
        }

        DrawLine3D(modelPos, (Vector3){ modelPos.x + 4, modelPos.y, modelPos.z }, RED);
        DrawLine3D(modelPos, (Vector3){ modelPos.x, modelPos.y + 4, modelPos.z }, DARKGREEN);
        DrawLine3D(modelPos, (Vector3){ modelPos.x, modelPos.y, modelPos.z + 4 }, DARKBLUE);

        DrawSphere((Vector3){ modelPos.x + 4, modelPos.y, modelPos.z }, 0.1f, RED);
        DrawSphere((Vector3){ modelPos.x, modelPos.y + 4, modelPos.z }, 0.1f, DARKGREEN);
        DrawSphere((Vector3){ modelPos.x, modelPos.y, modelPos.z + 4 }, 0.1f, DARKBLUE);

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
            -50.0f, 
            50.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20 * 4, 100, 15 }, 
            "RotY", 
            TextFormat("%3.2f", modelRot.y), 
            &modelRot.y, 
            -50.0f, 
            50.0f
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 70 + 20 * 5, 100, 15 }, 
            "RotZ", 
            TextFormat("%3.2f", modelRot.z), 
            &modelRot.z, 
            -50.0f, 
            50.0f
        );

        //----------------------------------------------------------------
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20 * 6, 100, 15 }, 
            "SclZ", 
            TextFormat("%3.2f", modelScl.x), 
            &modelScl.x, 
            0.01f, 
            maxScl
        );
        GuiSliderBar(
            (Rectangle){ uiTranformsLeft + 40, 90 + 20 * 7, 100, 15 }, 
            "SclZ", 
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
        GuiGroupBox((Rectangle){ uiSettingsLeft, 350, 180, 180 }, "Settings");

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
                Rectangle optionRect = { uiSettingsLeft + 40, 380 + 20 * (i + 1), 100, 20 };

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
                    Rectangle optionRect = { uiSettingsLeft + 40, 430 + 20 * (i + 1), 100, 20 };

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
            
            // Draw the dropdown box
            GuiDropdownBox((Rectangle){ uiSettingsLeft + 40, 480, 100, 20 }, animNameOptions, &animNameActiveOption, animNameDropdownEditMode);

            // Check if dropdown box is in edit mode (expanded)
            if (animNameDropdownEditMode)
            {
                animNameDropdownEditMode = !IsMousePressed();

                // Check for clicks on each option when expanded
                for (unsigned i = 0; i < animsCount; i++)
                {
                    // Calculate the rectangle for each dropdown option
                    Rectangle optionRect = { uiSettingsLeft + 40, 480 + 20 * (i + 1), 100, 20 };

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

        if (model != NULL)
        {
            const ModelAnimation anim = modelAnimation[animIndex];
            
            if (isPlayAnimation)
            {
                animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount;
                UpdateModelAnimation(*model, anim, animCurrentFrame);

                currentFrame = animCurrentFrame;
            }
            else
            {
                animCurrentFrame = (unsigned)currentFrame;
                UpdateModelAnimation(*model, anim, animCurrentFrame);
            }

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
