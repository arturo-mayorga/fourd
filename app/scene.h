#pragma once

#include <vector>
#include "../common/chunkloader.h"
#include "../common/component.h"
#include "../common/fourmath.h"

namespace fd {

class Camera;
class Entity;
class MeshBuffer;
class Mesh;
class Physics;
class QuaxolChunk;
class Shader;
class Texture;


class Scene {
protected:
  typedef std::vector<Entity*> TEntityList;
  TEntityList m_dynamicEntities;
  TEntityList m_toBeDeleted;

  typedef std::vector<Vec4f> ColorList;
  ColorList m_colorArray;

  //Mesh m_groundPlane;

  ComponentBus m_componentBus;

public:
  // leaving some vars public to avoid getter/setter as an experiment
  // requires knowing about write update functions
  Physics* m_pPhysics; //owned

  TVecQuaxol m_quaxols;
  Shader* m_pQuaxolShader; //not owned
  Mesh* m_pQuaxolMesh; //not owned

  typedef std::vector<Texture*> TTextureList;
  TTextureList m_texList;
  MeshBuffer* m_pQuaxolBuffer; // owned

  QuaxolChunk* m_pQuaxolChunk;

public:
  Scene();
  ~Scene();

  void AddLoadedChunk(const ChunkLoader* pChunk);

  void SetQuaxolAt(const QuaxolSpec& pos, bool present);

  void AddTexture(Texture* pTex);

  // Let the scene do the allocation to allow for mem opt
  // Some kind of entity def that at least includes shader, texture and mesh
  // types would be better for this as we could pre-sort by shader/texture/mesh
  Entity* AddEntity();
  void RemoveEntity(Entity* pEntity);

  void OnDeleteEntity(Entity* pEntity);

  void Step(float fDelta);

  // ugh this is all wrong, not going to be shader sorted, etc
  // but let's just do the stupid thing first
  void RenderEntitiesStupidly(Camera* pCamera);

  // Hacky garbage, should be on the mesh/quaxol
  void BuildColorArray();

protected:
  // horrible way to index textures
  // going to need a shader context or something soon
  void SetTexture(int index, GLint hTex);

};

} // namespace fd