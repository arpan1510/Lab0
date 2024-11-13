#include "UsdScene.h"

#include "io-util.hh"
#include "pprinter.hh"

#include "tydra/scene-access.hh"

// Visit all Prims in the Stage.
bool scene_print(const tinyusdz::Path& abs_path, const tinyusdz::Prim& prim, const int32_t level, void* userdata, std::string* err)
{
   std::cout << tinyusdz::pprint::Indent(level) << "[" << level << "] (" << prim.data().type_name() << ") " << prim.local_path().prim_part() << " : AbsPath " << tinyusdz::to_string(abs_path) << "\n";
   return true;
};

void UsdScene::Init(const std::string& filename)
{
   std::string warn;
   std::string err;
   std::string scene_path = mAssetDir + filename;

   bool ret = tinyusdz::LoadUSDFromFile(scene_path, &mStage, &warn, &err);
   if (!warn.empty())
   {
      std::cout << "WARN : " << warn << "\n";
   }
   if (!err.empty())
   {
      std::cout << "ERR : " << err << "\n";
   }

   if (ret == false)
   {
      return;
   }

   //Traverse scene and print node information
   void* userdata = nullptr;
   tinyusdz::tydra::VisitPrims(mStage, scene_print, userdata);


   //Convert to RenderScene and create buffers 
   tinyusdz::tydra::RenderSceneConverter converter;

   tinyusdz::tydra::RenderSceneConverterEnv converter_env(mStage);
   std::string basedir = tinyusdz::io::GetBaseDir(scene_path);
   converter_env.set_search_paths({ basedir });

   ret = converter.ConvertToRenderScene(converter_env, &mRenderScene);
   if (converter.GetWarning().size() > 0)
   {
      std::cout << "ConvertToRenderScene WARN: " << converter.GetWarning() << "\n";
   }

   if (ret = false)
   {
      std::cout << "Failed to convert USD Stage " << converter.GetError() << "\n";
      return;
   }

   //example load mesh[0]
   if (mRenderScene.meshes.size() > 0)
   {
      const int pos_loc = 0;
      const int tex_coord_loc = 1;
      const int normal_loc = 2;

      tinyusdz::tydra::RenderMesh& mesh = mRenderScene.meshes[0];
      glGenVertexArrays(1, &mVao);
      glBindVertexArray(mVao);

      //Buffer indices
      mNumIndices = mesh.triangulatedFaceVertexIndices.size();
      glGenBuffers(1, &mIndexBuffer);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mNumIndices, mesh.triangulatedFaceVertexIndices.data(), GL_STATIC_DRAW);

      //Buffer vertices
      glGenBuffers(1, &mVerticesVbo);
      glBindBuffer(GL_ARRAY_BUFFER, mVerticesVbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.points.size(), mesh.points.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(pos_loc);
      glVertexAttribPointer(pos_loc, 3, GL_FLOAT, 0, 0, 0);

      if (mesh.normals.data.size() > 0)
      {
         glGenBuffers(1, &mNormalsVbo);
         glBindBuffer(GL_ARRAY_BUFFER, mNormalsVbo);
         glBufferData(GL_ARRAY_BUFFER, mesh.normals.data.size(), mesh.normals.data.data(), GL_STATIC_DRAW);
         glEnableVertexAttribArray(normal_loc);
         glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);
      }

      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }
}

void UsdScene::Draw()
{
   if(mNumIndices > 0 && mVao >= 0)
   {
      glBindVertexArray(mVao);
      glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);
   }
}

#include "imgui.h"

struct traverse_info
{
   int id = 0;
   int last_level = 0;
};

bool scene_tree_gui(const tinyusdz::Path& abs_path, const tinyusdz::Prim& prim, const int32_t level, void* userdata, std::string* err)
{
   traverse_info* pInfo = (traverse_info*)userdata;

   if(level > 0)
   {
      ImGui::TreePop();
   }

   pInfo->id++;
   if(ImGui::TreeNode((void*)pInfo->id, "[%d] %s %s", level, prim.data().type_name().c_str(), prim.local_path().prim_part().c_str()))
   {
      pInfo->last_level = level;
      return true;
   } 

   return false;
};

void UsdTreeNodeGuiRec(const tinyusdz::Path& root_abs_path, const tinyusdz::Prim& root, int32_t level);

void UsdTreeNodeGui(const tinyusdz::Stage& stage) 
{
   // if `primChildren` is available, use it
   if (stage.metas().primChildren.size() == stage.root_prims().size()) 
   {
      std::map<std::string, const tinyusdz::Prim*> primNameTable;
      for (size_t i = 0; i < stage.root_prims().size(); i++) 
      {
         primNameTable.emplace(stage.root_prims()[i].element_name(), &stage.root_prims()[i]);
      }

      for (size_t i = 0; i < stage.metas().primChildren.size(); i++) 
      {
         tinyusdz::value::token nameTok = stage.metas().primChildren[i];
         const auto it = primNameTable.find(nameTok.str());
         if (it != primNameTable.end()) 
         {
            const tinyusdz::Path root_abs_path("/" + nameTok.str(), "");
            UsdTreeNodeGuiRec(root_abs_path, *it->second, 0);
         }
      }
   }
   else 
   {
      for (const auto& root : stage.root_prims()) 
      {
         const tinyusdz::Path root_abs_path("/" + root.element_name(), /* prop part */ "");
         UsdTreeNodeGuiRec(root_abs_path, root, /* root level */ 0);
      }
   }
}

void UsdTreeNodeGuiRec(const tinyusdz::Path& root_abs_path, const tinyusdz::Prim& root, int32_t level) 
{

   std::string label = "(" + root.data().type_name() + ") " + tinyusdz::to_string(root_abs_path);
   if(ImGui::TreeNode(label.c_str()))
   {
      // if `primChildren` is available, use it
      if (root.metas().primChildren.size() == root.children().size()) 
      {
         std::map<std::string, const tinyusdz::Prim*> primNameTable;
         for (size_t i = 0; i < root.children().size(); i++) 
         {
            primNameTable.emplace(root.children()[i].element_name(), &root.children()[i]);
         }

         for (size_t i = 0; i < root.metas().primChildren.size(); i++) 
         {
            tinyusdz::value::token nameTok = root.metas().primChildren[i];
            const auto it = primNameTable.find(nameTok.str());
            if (it != primNameTable.end()) 
            {
               const tinyusdz::Path child_abs_path = root_abs_path.AppendPrim(nameTok.str());
               UsdTreeNodeGuiRec(child_abs_path, *it->second, level + 1);
            }
         }
      }
      else 
      {
         for (const auto& child : root.children()) 
         {
            const tinyusdz::Path child_abs_path = root_abs_path.AppendPrim(child.element_name());
            UsdTreeNodeGuiRec(child_abs_path, child, level + 1);
         }
      }

      ImGui::TreePop();
   }
}

void UsdScene::DrawGui()
{
   ImGui::Begin("Usd");
      ImGui::Text("# of meshes: %d", mRenderScene.meshes.size());
      ImGui::Text("# of textures: %d", mRenderScene.textures.size());

      UsdTreeNodeGui(mStage);

   ImGui::End();
}