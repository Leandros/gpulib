#version 330
#extension GL_ARB_gpu_shader5               : enable
#extension GL_ARB_gpu_shader_fp64           : enable
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_texture_cube_map_array    : enable
#extension GL_ARB_separate_shader_objects   : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_shading_language_packing  : enable
#extension GL_ARB_explicit_uniform_location : enable

vec3 IntToColor(int i)
{
  vec3 color;
  color.r = float(i >> 16 & 0xFF) / 255.0;
  color.g = float(i >> 8  & 0xFF) / 255.0;
  color.b = float(i       & 0xFF) / 255.0;
  return color;
}

layout(binding = 2) uniform sampler2DArray s_textures;
layout(binding = 3) uniform samplerCubeArray s_cubemaps;

layout(location = 0) uniform int id;
layout(location = 1) uniform vec3 cam_pos;
layout(location = 2) uniform int show_pass;

layout(location = 0) smooth in vec3 pos;
layout(location = 1) smooth in vec3 nor;
layout(location = 2) smooth in vec2 uv;

layout(location = 0) out vec4 fbo_color;

void main()
{
  vec4 diffuse = texture(s_textures, vec3(uv, id));
  vec4 reflect = texture(s_cubemaps, vec4(reflect((pos - cam_pos), nor), 0));
  vec4 colours = vec4(0);

  colours += id == 0 ? vec4(1.0, 0.0, 0.0, 1.0) : vec4(0);
  colours += id == 1 ? vec4(0.0, 1.0, 0.0, 1.0) : vec4(0);
  colours += id == 2 ? vec4(0.0, 0.0, 1.0, 1.0) : vec4(0);

  fbo_color = mix((diffuse * 0.4) + (colours * 0.6), reflect, dot(nor, normalize(pos - cam_pos)) * 0.5 + 0.5);

  if (show_pass == 1) fbo_color = diffuse;
  if (show_pass == 2) fbo_color = reflect;
  if (show_pass == 3) fbo_color = colours;
  if (show_pass == 4) fbo_color = vec4(IntToColor(id + 1), 1);
  if (show_pass == 5) fbo_color = vec4(uv, 0, 1);
  if (show_pass == 6) fbo_color = vec4(nor, 1);
  if (show_pass == 7) fbo_color = vec4(pos, 1);
  if (show_pass == 8) fbo_color = vec4(vec3(gl_FragCoord.z), 1);
}
