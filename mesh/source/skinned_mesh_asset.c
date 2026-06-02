/**
 * @file skinned_mesh_asset.c
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-22
 *
 * @copyright Copyright (c) 2026
 *
 */
#include <assert.h>
#include <string.h>
#include <mesh/skinned_mesh_asset.h>
#include <library/allocator/allocator.h>
#include <library/asset/asset_ref.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
skinned_mesh_asset_def(void *ptr)
{
  assert(ptr);

  {
    skinned_mesh_asset_t *skinned_mesh = (skinned_mesh_asset_t *)ptr;
    memset(skinned_mesh, 0, sizeof(skinned_mesh_asset_t));
  }
}

uint32_t
skinned_mesh_asset_is_def(const void *ptr)
{
  assert(ptr);

  {
    const skinned_mesh_asset_t *skinned_mesh =
      (const skinned_mesh_asset_t *)ptr;
    skinned_mesh_asset_t def;
    skinned_mesh_asset_def(&def);
    return !memcmp(skinned_mesh, &def, sizeof(skinned_mesh_asset_t));
  }
}

static
void
skinned_mesh_bone_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const skinned_mesh_bone_t *bone = (const skinned_mesh_bone_t *)src;
    cstring_serialize(&bone->name, stream);
    binary_stream_write(stream, &bone->offset_matrix, sizeof(matrix4f));
  }
}

void
skinned_mesh_asset_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const skinned_mesh_asset_t *skinned_mesh =
      (const skinned_mesh_asset_t *)src;
    mesh_asset_serialize(&skinned_mesh->mesh, stream);
    cvector_serialize_func(
      &skinned_mesh->bones, stream, skinned_mesh_bone_serialize);
    cvector_serialize(&skinned_mesh->vertex_to_bones, stream);
  }
}

static
void
skinned_mesh_bone_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    size_t s_m4 = sizeof(matrix4f);
    skinned_mesh_bone_t *bone = (skinned_mesh_bone_t *)dst;
    cstring_def(&bone->name);
    cstring_deserialize(&bone->name, allocator, stream);
    binary_stream_read(stream, (uint8_t *)&bone->offset_matrix, s_m4, s_m4);
  }
}

void
skinned_mesh_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    skinned_mesh_asset_t *skinned_mesh = (skinned_mesh_asset_t *)dst;
    mesh_asset_deserialize(&skinned_mesh->mesh, allocator, stream);
    cvector_deserialize_func(
      &skinned_mesh->bones, allocator, stream, skinned_mesh_bone_deserialize);
    cvector_deserialize(&skinned_mesh->vertex_to_bones, allocator, stream);
  }
}

size_t
skinned_mesh_asset_type_size(void)
{
  return sizeof(skinned_mesh_asset_t);
}

uint32_t
skinned_mesh_asset_owns_alloc(void)
{
  return 0;
}

const allocator_t *
skinned_mesh_asset_get_alloc(const void *ptr)
{
  return NULL;
}

static
void
skinned_mesh_bone_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && allocator);

  {
    skinned_mesh_bone_t *bone = (skinned_mesh_bone_t *)ptr;
    cstring_cleanup2(&bone->name);
    memset(bone, 0, sizeof(skinned_mesh_bone_t));
  }
}

void
skinned_mesh_asset_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !skinned_mesh_asset_is_def(ptr));
  assert(allocator);

  {
    skinned_mesh_asset_t *skinned_mesh = (skinned_mesh_asset_t *)ptr;
    mesh_asset_cleanup(&skinned_mesh->mesh, allocator);
    cvector_cleanup_func(&skinned_mesh->bones, NULL, skinned_mesh_bone_cleanup);
    cvector_cleanup2(&skinned_mesh->vertex_to_bones);
    memset(skinned_mesh, 0, sizeof(skinned_mesh_asset_t));
  }
}

const char *
skinned_mesh_asset_get_dir(void)
{
  static const char *directory = "skinned_meshes";
  return directory;
}

static
void
skinned_mesh_asset_loader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(skinned_mesh_asset_t));

  {
    skinned_mesh_asset_t **ptr = (skinned_mesh_asset_t **)ptr_addr;
    skinned_mesh_asset_t *asset_ptr = *ptr;
    binary_stream_t *stream = binary_stream_from_file(
      asset_ref->path.str, allocator);
    *ptr = allocator->mem_alloc(sizeof(skinned_mesh_asset_t));
    skinned_mesh_asset_def(asset_ptr);
    skinned_mesh_asset_deserialize(asset_ptr, allocator, stream);
    binary_stream_cleanup(stream);
    allocator->mem_free(stream);
  }
}

static
void
skinned_mesh_asset_deloader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(skinned_mesh_asset_t));

  {
    skinned_mesh_asset_t **ptr = (skinned_mesh_asset_t **)ptr_addr;
    skinned_mesh_asset_t *asset_ptr = *ptr;
    skinned_mesh_asset_cleanup(asset_ptr, allocator);
    allocator->mem_free(asset_ptr);
    *ptr = NULL;
  }
}

loader_t
skinned_mesh_asset_get_loader(void)
{
  return skinned_mesh_asset_loader;
}

deloader_t
skinned_mesh_asset_get_deloader(void)
{
  return skinned_mesh_asset_deloader;
}

uint32_t
skinned_mesh_asset_type_asset_count(const void *src)
{
  assert(src);

  {
    const skinned_mesh_asset_t *skinned_mesh = src;
    return mesh_asset_type_asset_count(&skinned_mesh->mesh);
  }

  return 0;
}

void
skinned_mesh_asset_type_get_assets(const void *src, const asset_ref_t *refs[])
{
  assert(src);

  {
    const skinned_mesh_asset_t *skinned_mesh = src;
    mesh_asset_type_get_assets(&skinned_mesh->mesh, refs);
  }
}

uint32_t
skinned_mesh_asset_is_asset_type(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_skinned_mesh_asset_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = skinned_mesh_asset_def;
  vtable.fn_is_def = skinned_mesh_asset_is_def;
  vtable.fn_serialize = skinned_mesh_asset_serialize;
  vtable.fn_deserialize = skinned_mesh_asset_deserialize;
  vtable.fn_type_size = skinned_mesh_asset_type_size;
  vtable.fn_owns_alloc = skinned_mesh_asset_owns_alloc;
  vtable.fn_get_alloc = skinned_mesh_asset_get_alloc;
  vtable.fn_cleanup = skinned_mesh_asset_cleanup;
  vtable.fn_get_dir = skinned_mesh_asset_get_dir;
  vtable.fn_get_loader = skinned_mesh_asset_get_loader;
  vtable.fn_get_deloader = skinned_mesh_asset_get_deloader;
  vtable.fn_type_asset_count = skinned_mesh_asset_type_asset_count;
  vtable.fn_type_get_assets = skinned_mesh_asset_type_get_assets;
  vtable.fn_is_asset_type = skinned_mesh_asset_is_asset_type;
  register_type(get_type_id(skinned_mesh_asset_t), &vtable);
}