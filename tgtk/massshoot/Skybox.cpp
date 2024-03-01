#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include "defines.h"
#include <iostream>
#include "Skybox.h"

Model skybox;
bool skyboxGenerated = false;

LOADING_FUNC MassShoot::Skybox::LoadSkybox(std::string filePath) {
    if(skyboxGenerated) UnloadTexture(skybox.materials->maps->texture);
    // vestigial
    bool useHDR = false;

    // workarounds from converting c to c++
    int zeroarray[1] = { 0 };
    int onearray[1] = { 1 };
    int matMapCube[1] = { MATERIAL_MAP_CUBEMAP };

    /*
    
        Quick timeout here: Why doesn't C++ allow me to write (int[1]){value} like
                            C does? Sure it's lame and stupid but C++ is C, plus
                            plus. To be fair C++ has so many other stupid features
                            I'm convinced they removed this feature to make the
                            compiler less obese on slower hard drives.

    */

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    skybox = LoadModelFromMesh(cube);
    skybox.materials[0].shader = LoadShader("massshoot/shaders/skybox.vs", "massshoot/shaders/skybox.fs");

    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), matMapCube, SHADER_UNIFORM_INT);

    // Load cubemap shader and setup required shader locations
    Shader shdrCubemap = LoadShader("massshoot/shaders/cubemap.vs", "massshoot/shaders/cubemap.fs");

    SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), zeroarray, SHADER_UNIFORM_INT);

    // Half Vestigial TBH Kinda I Don't Fucking Know Anymore™
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), zeroarray, SHADER_UNIFORM_INT);
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), zeroarray, SHADER_UNIFORM_INT);

    // Load the fucking texture you illiterate criss.
    Image img = LoadImage(filePath.c_str());
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA
    UnloadImage(img);
    skyboxGenerated = true;
}

RENDERING_FUNC MassShoot::Skybox::DrawSkybox() {
    // We are inside the cube, we need to disable backface culling!
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(skybox, { 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}