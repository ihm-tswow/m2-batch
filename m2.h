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

#include "file.h"

#include <cstdint>
#include <fstream>
#include <vector>

template <typename T>
struct Vec2D
{
    T x;
    T y;
};

template <typename T>
struct Vec3D : public Vec2D<T>
{
    T z;
};

struct M2Quat
{
    uint16_t w;
    uint16_t x;
    uint16_t y;
    uint16_t z;
};

enum M2SequenceFlags
{
    blended_animation_auto = 0x1,
    load_low_priority_sequence = 0x10,
    looped_animation = 0x20,
    is_alias = 0x40,
    blended_animation = 0x80,
    sequence_stored_in_model = 0x100,
    some_legion_flag = 0x800
};

struct M2String
{
    uint32_t char_count;
    uint32_t ofs_chars;
    template <typename T>
    std::string read(File<T>& file)
    {
        file.stream.seekg(ofs_chars, std::ios::beg);
        std::string str;
        std::copy_n(std::istreambuf_iterator<char>(file.stream.rdbuf()),
            char_count, std::back_inserter(str));
        return str;
    }

};

struct M2Range
{
    uint32_t min;
    uint32_t max;
};

struct M2Box
{
    Vec3D<float> min;
    Vec3D<float> max;
};

struct M2Bounds
{
    M2Box box;
    float radius;
};

template <typename T>
struct M2Array
{
    uint32_t count;
    uint32_t offset;

    template <typename G>
    std::vector<T> read(File<G> & file)
    {
        std::vector<T> vec(count);
        file.stream.seekg(offset, std::ios::beg);
        for (size_t i = 0; i < count; ++i)
        {
            file.stream.read(reinterpret_cast<char*>(&vec[i]),sizeof(T));
        }
        return vec;
    }
};

template <typename T>
struct M2SplineKey
{
    T value;
    T in_tan;
    T out_tan;
};

template <typename T>
struct FBlock
{
    M2Array<M2Array<uint32_t>> timestamps;
    M2Array<M2Array<T>> values;
};

struct M2TrackTime
{
    uint16_t interpolation_type;
    int16_t global_sequence;
    M2Array<uint16_t> timestamps;
};

template <typename T>
struct M2Track
{
    uint16_t interpolation_type;
    int16_t global_sequence;
    FBlock<T> block;
};

struct M2Sequence
{
    uint16_t id;
    uint16_t variation_index;
    uint32_t duration;
    float movespeed;
    uint32_t flags;
    int16_t frequency;
    uint16_t padding;
    M2Range replay;
    uint32_t blend_time;
    M2Bounds bounds;
    int16_t variation_next;
    uint16_t alias_next;
};

struct M2Bone
{
    int32_t key_bone_id;
    uint32_t flags;
    int16_t parent_bone;
    uint16_t submesh_id;
    uint32_t bone_name_crc;
    M2Track<Vec3D<float>> translation;
    M2Track<M2Quat> rotation;
    M2Track<Vec3D<float>> scale;
    Vec3D<float> Pivot;
};

struct M2Vertex
{
    Vec3D<float> pos;
    uint8_t bone_weights[4];
    uint8_t bone_indices[4];
    Vec3D<float> normal;
    Vec2D<float> tex_coords[2];
};

struct M2Color
{
    M2Track<Vec3D<float>> color;
    M2Track<uint16_t> alpha;
};

struct M2Texture
{
    uint32_t type;
    uint32_t flags;
    M2String filename;
};

struct M2TextureTransform
{
    M2Track<Vec3D<float>> translation;
    M2Track<M2Quat> rotation;
    M2Track<Vec3D<float>> scaling;
};

struct M2Material
{
    uint16_t flags;
    uint16_t blending_mode;
};

struct M2Attachment
{
    uint32_t id;
    uint16_t bone;
    uint16_t unknown;
    Vec3D<float> position;
    M2Track<bool> animate_attached;
};

struct M2Event
{
    char identifier[4];
    uint32_t data;
    uint32_t bone;
    Vec3D<float> position;
    M2TrackTime enabled;
};

struct M2Light
{
    uint16_t type;
    int16_t bone;
    Vec3D<float> position;
    M2Track<Vec3D<float>> ambient_color;
    M2Track<float> ambient_intensity;
    M2Track<Vec3D<float>> diffuse_color;
    M2Track<float> diffuse_intensity;
    M2Track<float> attenuation_start;
    M2Track<float> attenuation_end;
    M2Track<uint8_t> visibility;
};

struct M2Camera
{
    int32_t type;
    float fov;
    float far_clip;
    float near_clip;
    M2Track<M2SplineKey<Vec3D<float>>> positions;
    Vec3D<float> position_base;
    M2Track<M2SplineKey<Vec3D<float>>> target_positions;
    Vec3D<float> target_position_base;
    M2Track<M2SplineKey<float>> roll;
};

struct M2Ribbon
{
    int32_t ribbon_id;
    uint32_t bone_index;
    Vec3D<float> position;
    M2Array<uint16_t> texture_indices;
    M2Array<uint16_t> material_indices;
    M2Track<Vec3D<float>> color_track;
    M2Track<int16_t> alpha_track;
    M2Track<float> height_above_track;
    M2Track<float> height_below_track;
    float edges_per_second;
    float edge_lifetime;
    float gravity;
    uint16_t texture_rows;
    uint16_t texture_cols;
    M2Track<uint16_t> tex_slot_track;
    M2Track<uint8_t> visibility_track;
    int16_t priority_plane;
    uint16_t padding;
};

struct M2Particle
{
    uint32_t particle_id;
    uint32_t flags;
    Vec3D<float> position;
    uint16_t bone;
    uint16_t texture;
    M2String geometry_model_filename;
    M2String recursion_model_filename;
    uint8_t blending_type;
    uint8_t emitter_type;
    uint16_t particle_color_index;
    uint8_t particle_type;
    uint8_t head_or_tail;
    uint16_t texture_tile_rotation;
    uint16_t texture_dimensions_rows;
    uint16_t texture_dimension_columns;
    M2Track<float> emission_speed;
    M2Track<float> speed_variation;
    M2Track<float> vertical_range;
    M2Track<float> horizontal_range;
    M2Track<float> gravity;
    M2Track<float> lifespan;
    float life_span_vary;
    M2Track<float> emission_rate;
    float emission_rate_vary;
    M2Track<float> emission_area_length;
    M2Track<float> emission_area_width;
    M2Track<float> z_source;
    FBlock<Vec3D<float>> color_track;
    FBlock<uint16_t> alpha_track;
    FBlock<Vec2D<float>> scale_track;
    Vec2D<float> scale_vary;
    FBlock<uint16_t> head_cell_track;
    FBlock<uint16_t> tail_cell_track;
    float tail_length;
    float twinkle_speed;
    float twinkle_percent;
    M2Range twinkle_scale;
    float burst_multiplier;
    float drag;
    float basespin;
    float base_spin_vary;
    float spin;
    float spin_vary;
    M2Box tumble;
    Vec3D<float> wind_vector;
    float wind_time;
    struct Follow
    {
        float speed;
        float scale;
    } follow[2];
    M2Array<Vec3D<float>> spline_points;
    M2Track<uint8_t> enabled_in;
};

struct M2Header
{
    uint32_t magic;
    uint32_t version;
    M2String name;
    uint32_t global_flags;
    M2Array<uint32_t> global_sequences;
    M2Array<M2Sequence> sequences;
    M2Array<uint32_t> sequence_lookup;
    M2Array<M2Bone> bones;
    M2Array<uint32_t> key_bone_lookups;
    M2Array<M2Vertex> vertices;
    uint32_t unknown;
    M2Array<M2Color> colors;
    M2Array<M2Texture> textures;
    M2Array<M2Track<uint16_t>> texture_weights;
    M2Array<M2TextureTransform> texture_transforms;
    M2Array<int16_t> replacable_texture_lookup;
    M2Array<M2Material> materails;
    M2Array<uint16_t> bone_lookup_table;
    M2Array<uint16_t> texture_lookup_table;
    M2Array<uint16_t> tex_unit_lookup_table;
    M2Array<uint16_t> transparency_lookup_table;
    M2Array<int16_t> texture_transforms_lookup_table;
    M2Bounds bounds;
    M2Bounds collision;
    M2Array<uint16_t> collision_triangles;
    M2Array<Vec3D<float>> collision_vertices;
    M2Array<Vec3D<float>> collision_normals;
    M2Array<M2Attachment> attachments;
    M2Array<uint16_t> attachment_lookup_table;
    M2Array<M2Event> events;
    M2Array<M2Light> lights;
    M2Array<M2Camera> cameras;
    M2Array<uint16_t> camera_lookup_table;
    M2Array<M2Ribbon> ribbons;
    M2Array<M2Particle> particles;
    M2Array<uint16_t> texture_combiner_combos;
    static M2Header read(std::ifstream& stream);
};