#ifndef SHADER_ENUM_H
#define SHADER_ENUM_H

enum EnumShaderVarTag
{
    cam_pos = 0,
    local_to_world, 
    world_to_view,
    view_to_proj,
    world_view_proj,
    tex_transform,
    world_inv_transpose,
    plain_material,
    texture,
    anisotropic_sampler
};

#endif