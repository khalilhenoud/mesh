/**
 * @file mesh_asset.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-04-22
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef MESH_ASSET_H
#define MESH_ASSET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mesh/internal/module.h>
#include <library/asset/types.h>
#include <library/containers/cvector.h>


////////////////////////////////////////////////////////////////////////////////
//| mesh_asset_t, '*' = mesh_asset
//|=============================================================================
//| OPERATION                   | SUPPORTED
//|=============================================================================
//|    *_def                    | YES
//|    *_is_def                 | YES
//|    *_replicate              |
//|    *_fullswap               |
//|    *_serialize              | YES
//|    *_deserialize            | YES
//|    *_hash                   |
//|    *_is_equal               |
//|    *_type_size              | YES
//|    *_type_alignment         |
//|    *_type_id_count          |
//|    *_type_ids               |
//|    *_owns_alloc             | YES
//|    *_get_alloc              | YES
//|    *_cleanup                | YES
//|    *_get_dir                | YES
//|    *_get_loader             | YES
//|    *_get_deloader           | YES
//|    *_type_asset_count       | YES
//|    *_type_get_assets        | YES
//|    *_is_asset_type          | YES
////////////////////////////////////////////////////////////////////////////////

typedef struct allocator_t allocator_t;
typedef struct binary_stream_t binary_stream_t;

typedef
struct mesh_asset_t {
  cvector_t vertices;           // float
  cvector_t normals;            // float
  cvector_t uvs;                // float
  cvector_t indices;            // uint32_t
  cvector_t materials;          // asset_ref_t (material asset or bulk variant)
} mesh_asset_t;

MESH_API
void
mesh_asset_def(void *ptr);

MESH_API
uint32_t
mesh_asset_is_def(const void *ptr);

MESH_API
void
mesh_asset_serialize(
  const void *src,
  binary_stream_t *stream);

MESH_API
void
mesh_asset_deserialize(
  void *dst,
  const allocator_t *allocator,
  binary_stream_t* stream);

MESH_API
size_t
mesh_asset_type_size(void);

MESH_API
uint32_t
mesh_asset_owns_alloc(void);

MESH_API
const allocator_t *
mesh_asset_get_alloc(const void *ptr);

MESH_API
void
mesh_asset_cleanup(
  void *ptr,
  const allocator_t *allocator);

MESH_API
const char *
mesh_asset_get_dir(void);

MESH_API
loader_t
mesh_asset_get_loader(void);

MESH_API
deloader_t
mesh_asset_get_deloader(void);

MESH_API
uint32_t
mesh_asset_type_asset_count(const void *src);

MESH_API
void
mesh_asset_type_get_assets(const void *src, const asset_ref_t *refs[]);

MESH_API
uint32_t
mesh_asset_is_asset_type(void);

#ifdef __cplusplus
}
#endif

#endif