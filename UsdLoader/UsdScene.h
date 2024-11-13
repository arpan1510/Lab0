#pragma once

#include "tinyusdz.hh"
#include "tydra/render-data.hh"
#include <GL/glew.h>

struct UsdScene
{

   tinyusdz::Stage mStage;
   tinyusdz::tydra::RenderScene mRenderScene;

   const std::string mAssetDir = "assets/";

   GLuint mVao = -1;
   GLuint mVerticesVbo = -1;
   GLuint mNormalsVbo = -1;
   GLuint mIndexBuffer = -1;
   int mNumIndices = 0;

   void Init(const std::string& filename);
   void Draw();
   void DrawGui();
};
