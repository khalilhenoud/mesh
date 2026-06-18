/**
 * @file mesh_asset_utils.h
 * @author khalilhenoud@gmail.com
 * @brief
 * @version 0.1
 * @date 2026-06-17
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef MESH_ASSET_UTILS_H
#define MESH_ASSET_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <mesh/internal/module.h>


typedef struct mesh_asset_t mesh_asset_t;
typedef struct allocator_t allocator_t;

MESH_API
mesh_asset_t *
create_unit_cube(const allocator_t *allocator);

MESH_API
mesh_asset_t *
create_unit_sphere(const int32_t factor, const allocator_t *allocator);

MESH_API
mesh_asset_t *
create_unit_capsule(
  const int32_t factor,
  const float half_height_to_radius_ratio,
  const allocator_t *allocator);

#ifdef __cplusplus
}
#endif

#endif