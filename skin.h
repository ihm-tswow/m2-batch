/*
 * This file is part of m2-batch (https://github.com/ihm-tswow/m2-batch)
 * Copyright (c) 2022 ihm-tswow (https://github.com/ihm-tswow)
 * Based on wbs by skarn (https://gitlab.com/skarnproject/blender-wow-studio)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "m2.h"

#include <cstdint>
#include <fstream>

struct M2Submesh
{
    uint16_t skin_section_id;
    uint16_t level;
    uint16_t vertex_start;
    uint16_t vertex_count;

    uint16_t index_start;
    uint16_t index_count;

    uint16_t bone_count;
    uint16_t bone_combo_index;
    uint16_t bone_influences;

    uint16_t center_bone_index;
    Vec3D<float> center_position;
    Vec3D<float> sort_center_position;
    float sort_radius;
};

struct M2TextureUnit
{
    uint8_t flags;
    int8_t priority_plane;
    int16_t shader_id;
    uint16_t skin_section_index;
    uint16_t geoset_index;
    int16_t color_index;
    uint16_t material_index;
    uint16_t material_layer;
    uint16_t texture_count;
    uint16_t texture_combo_index;
    uint16_t texture_coord_combo_index;
    uint16_t texture_weight_combo_index;
    uint16_t texture_transform_combo_index;
};

struct M2SkinHeader
{
    uint32_t magic;
    M2Array<uint16_t> vertex_indices;
    M2Array<uint16_t> triangle_indices;
    M2Array<uint16_t> bone_indices;
    M2Array<M2Submesh> submeshes;
    M2Array<M2TextureUnit> texture_units;
    uint32_t bone_count_max;
    static M2SkinHeader read(std::ifstream& stream);
};
