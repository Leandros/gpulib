#version 330
#extension GL_ARB_gpu_shader5               : enable
#extension GL_ARB_gpu_shader_fp64           : enable
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_texture_cube_map_array    : enable
#extension GL_ARB_separate_shader_objects   : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_shading_language_packing  : enable
#extension GL_ARB_explicit_uniform_location : enable

layout(binding = 3) uniform samplerCubeArray s_cubemaps;

layout(location = 0) smooth in vec3 pos;

out vec4 color;

void main()
{
  color = texture(s_cubemaps, vec4(pos, 0));
}
