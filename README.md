GpuLib
------

<img width="800px" src="https://i.imgur.com/dQEm83w.gif" />
<img width="800px" src="https://i.imgur.com/oDLY5rY.png" />

GpuLib is a Public Domain header-only C library that uses 64 modern DSA AZDO OpenGL functions to draw geometry, post-process textures and compute arrays on GPU.

The contract:

 * SDL2, desktop OpenGL 3.3 with extensions, Linux and Windows only. Doesn't support macOS, WebGL or GLES.
 * GPU memory is immutable for resize. Once allocated you can't resize it, but you can still change its content.
 * No multithreaded or asynchronous CPU<->GPU interactions. No barriers or sync points except for glFinish calls.
 * Not all modern OpenGL extensions are used, only those which are supported on low-end hardware and Mesa 12.0+.

Dependencies for Ubuntu 16.10+ (libc++ is needed only for Dear ImGui):

```bash
sudo apt install git clang libsdl2-dev libc++-dev
```

List of all the structs, enums, functions and macros you need to use:

```c
struct gpu_cmd_t {};
struct gpu_op_t {};
enum gpu_draw_t {};
enum gpu_shader_t {};
enum gpu_global_t {};
enum gpu_tex_mem_format_t {};
enum gpu_tex_format_t {};
enum gpu_smp_filter_t {};
enum gpu_smp_wrapping_t {};
enum gpu_pixel_format_t {};
enum gpu_pixel_t {};
static inline uint32_t gpu_window() {}
static inline void * gpu_malloc() {}
static inline uint32_t gpu_cast() {}
static inline uint32_t gpu_malloc_msi() {}
#define gpu_malloc_img()
#define gpu_malloc_cbm()
#define gpu_cast_img()
#define gpu_cast_cbm()
#define gpu_get()
#define gpu_set()
#define gpu_mip()
static inline void gpu_bmp_img() {}
static inline void gpu_bmp_cbm() {}
static inline uint32_t gpu_smp() {}
#define gpu_vert()
#define gpu_frag()
#define gpu_vert_file()
#define gpu_frag_file()
#define gpu_vert_xfb()
#define gpu_frag_xfb()
#define gpu_vert_xfb_file()
#define gpu_frag_xfb_file()
#define gpu_f64()
#define gpu_f32()
#define gpu_i32()
#define gpu_u32()
#define gpu_v_2()
#define gpu_v_3()
#define gpu_v_4()
#define gpu_vert_head
#define gpu_frag_head
static inline uint32_t gpu_ppo() {}
static inline uint32_t gpu_fbo() {}
static inline uint32_t gpu_xfb() {}
#define gpu_bind_fbo()
#define gpu_bind_xfb()
static inline void gpu_draw() {}
static inline void gpu_draw_xfb() {}
static inline void gpu_blit() {}
static inline void gpu_blit_to_screen() {}
#define gpu_clear()
#define gpu_swap()
```

Naming convention:

 * `mem`: Memory
 * `tex`: Texture
 * `img`: Image
 * `msi`: Multisample Image
 * `cbm`: Cubemap
 * `smp`: Sampler
 * `pro`: Program Object
 * `ppo`: Pipeline Program Object
 * `fbo`: Framebuffer Object
 * `xfb`: Transform Feedback Object

Special thanks to Nicolas [@nlguillemot](https://github.com/nlguillemot) and Andreas [@ands](https://github.com/ands) for answering my OpenGL questions and Micha [@vurtun](https://github.com/vurtun) for suggestions on how to improve the library!

