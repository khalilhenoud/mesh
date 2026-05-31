/**
 * @file mesh_asset.c
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
#include <mesh/mesh_asset.h>
#include <library/allocator/allocator.h>
#include <library/asset/asset_ref.h>
#include <library/core/core.h>
#include <library/type_registry/type_registry.h>


void
mesh_asset_def(void *ptr)
{
  assert(ptr);

  {
    mesh_asset_t *mesh = (mesh_asset_t *)ptr;
    memset(mesh, 0, sizeof(mesh_asset_t));
  }
}

uint32_t
mesh_asset_is_def(const void *ptr)
{
  assert(ptr);

  {
    const mesh_asset_t *mesh = (const mesh_asset_t *)ptr;
    mesh_asset_t def;
    mesh_asset_def(&def);
    return !memcmp(mesh, &def, sizeof(mesh_asset_t));
  }
}

void
mesh_asset_serialize(
  const void *src,
  binary_stream_t *stream)
{
  assert(src && stream);

  {
    const mesh_asset_t *mesh = (const mesh_asset_t *)src;
    cvector_serialize(&mesh->vertices, stream);
    cvector_serialize(&mesh->normals, stream);
    cvector_serialize(&mesh->uvs, stream);
    cvector_serialize(&mesh->indices, stream);
    cvector_serialize(&mesh->materials, stream);
  }
}

void
mesh_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t *stream)
{
  assert(dst && allocator && stream);

  {
    mesh_asset_t *mesh = (mesh_asset_t *)dst;
    cvector_def(&mesh->vertices);
    cvector_deserialize(&mesh->vertices, allocator, stream);
    cvector_def(&mesh->normals);
    cvector_deserialize(&mesh->normals, allocator, stream);
    cvector_def(&mesh->uvs);
    cvector_deserialize(&mesh->uvs, allocator, stream);
    cvector_def(&mesh->indices);
    cvector_deserialize(&mesh->indices, allocator, stream);
    cvector_def(&mesh->materials);
    cvector_deserialize(&mesh->materials, allocator, stream);
  }
}

size_t
mesh_asset_type_size(void)
{
  return sizeof(mesh_asset_t);
}

uint32_t
mesh_asset_owns_alloc(void)
{
  return 0;
}

const allocator_t *
mesh_asset_get_alloc(const void *ptr)
{
  return NULL;
}

void
mesh_asset_cleanup(
  void *ptr,
  const allocator_t *allocator)
{
  assert(ptr && !mesh_asset_is_def(ptr));
  assert(allocator);

  {
    mesh_asset_t *mesh = (mesh_asset_t *)ptr;
    cvector_cleanup2(&mesh->vertices);
    cvector_cleanup2(&mesh->normals);
    cvector_cleanup2(&mesh->uvs);
    cvector_cleanup2(&mesh->indices);
    cvector_cleanup2(&mesh->materials);
  }
}

const char *
mesh_asset_get_dir(void)
{
  static const char *directory = "meshes";
  return directory;
}

static
void
mesh_asset_loader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(mesh_asset_t));

  {
    mesh_asset_t **ptr = (mesh_asset_t **)ptr_addr;
    mesh_asset_t *asset_ptr = *ptr;
    binary_stream_t *stream = binary_stream_from_file(
      asset_ref->path.str, allocator);
    *ptr = allocator->mem_alloc(sizeof(mesh_asset_t));
    mesh_asset_def(asset_ptr);
    mesh_asset_deserialize(asset_ptr, allocator, stream);
    binary_stream_cleanup(stream);
    allocator->mem_free(stream);
  }
}

static
void
mesh_asset_deloader(
  void **ptr_addr,
  const asset_ref_t *asset_ref,
  const allocator_t *allocator)
{
  assert(ptr_addr && asset_ref && allocator);
  asset_ref_sanity_check(asset_ref, get_type_id(mesh_asset_t));

  {
    mesh_asset_t **ptr = (mesh_asset_t **)ptr_addr;
    mesh_asset_t *asset_ptr = *ptr;
    mesh_asset_cleanup(asset_ptr, allocator);
    allocator->mem_free(asset_ptr);
    *ptr = NULL;
  }
}

loader_t
mesh_asset_get_loader(void)
{
  return mesh_asset_loader;
}

deloader_t
mesh_asset_get_deloader(void)
{
  return mesh_asset_deloader;
}

uint32_t
mesh_asset_type_asset_count(const void *src)
{
  assert(src);

  {
    const mesh_asset_t *mesh = src;
    return (uint32_t)mesh->materials.size;
  }

  return 0;
}

void
mesh_asset_type_get_assets(const void *src, const asset_ref_t *refs[])
{
  assert(src);

  {
    const mesh_asset_t *mesh = src;
    for (uint32_t i = 0; i < mesh->materials.size; ++i) {
      const asset_ref_t *material_ref = cvector_as_c(
        &mesh->materials, i, asset_ref_t);
      refs[i] = material_ref;
    }
  }
}

uint32_t
mesh_asset_is_asset_type(void)
{
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
INITIALIZER(register_mesh_asset_t)
{
  vtable_t vtable;
  memset(&vtable, 0, sizeof(vtable_t));
  vtable.fn_def = mesh_asset_def;
  vtable.fn_is_def = mesh_asset_is_def;
  vtable.fn_serialize = mesh_asset_serialize;
  vtable.fn_deserialize = mesh_asset_deserialize;
  vtable.fn_type_size = mesh_asset_type_size;
  vtable.fn_owns_alloc = mesh_asset_owns_alloc;
  vtable.fn_get_alloc = mesh_asset_get_alloc;
  vtable.fn_cleanup = mesh_asset_cleanup;
  vtable.fn_get_dir = mesh_asset_get_dir;
  vtable.fn_get_loader = mesh_asset_get_loader;
  vtable.fn_get_deloader = mesh_asset_get_deloader;
  vtable.fn_type_asset_count = mesh_asset_type_asset_count;
  vtable.fn_type_get_assets = mesh_asset_type_get_assets;
  vtable.fn_is_asset_type = mesh_asset_is_asset_type;
  register_type(get_type_id(mesh_asset_t), &vtable);
}