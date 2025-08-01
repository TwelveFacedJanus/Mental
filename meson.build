project('Mental', 'c', version : '0.1')

c_args = ['-std=c11']

srcs = [
  'main.c',
  'mathematica/mathematica.c',
  'engine/engine/engine.c',
  'engine/project/project.c',
  'engine/wm/wm.c',
]

inc_dirs = include_directories(
  'core',
  'mathematica',
  'engine/engine',
  'engine/project',
  'engine/wm'
)

# Попробуем найти GLFW через pkg-config
glfw_dep = dependency('glfw3', required : false)
if not glfw_dep.found()
  glfw_dep = declare_dependency(
    include_directories : include_directories('/opt/homebrew/include'),
    link_args : ['/opt/homebrew/lib/libglfw.dylib']
  )
endif

# Попробуем найти Vulkan через pkg-config
vulkan_dep = dependency('vulkan', required : false)
if not vulkan_dep.found()
  # Для macOS обычно используется MoltenVK через Homebrew
  vulkan_dep = declare_dependency(
    include_directories : include_directories('/opt/homebrew/include'),
    link_args : [
      '/opt/homebrew/lib/libvulkan.dylib',
      '-framework', 'MoltenVK'
    ]
  )
endif

executable(
  'Mental',
  srcs,
  include_directories : inc_dirs,
  dependencies : [glfw_dep, vulkan_dep],
  c_args : c_args,
  link_args : [
    '-framework', 'OpenGL',
    '-framework', 'Cocoa',
    '-framework', 'IOKit',
    '-framework', 'CoreVideo'
  ]
)