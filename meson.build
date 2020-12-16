project('dl', 'c')

inc = include_directories('include')
cc = meson.get_compiler('c')
c_stds = ['c89', 'c90', 'c99', 'c11']
standard_flags = []

lib_m = cc.find_library('m', required : true)

gcc_configs = [
#  ['no-malloc', ['-DDL_USE_MALLOC=0']],
  ['test-checked', ['-g', '-O0', '-DDL_USE_SAFETY_CHECKS=1']],
  ['test-unchecked', ['-g', '-O0', '-DDL_USE_SAFETY_CHECKS=0']],
#  ['pedantic', ['-pedantic', '-Wall', '-Werror', '-Wduplicated-cond', '-Wduplicated-branches', '-Wlogical-op', '-Wrestrict', '-Wnull-dereference', '-Wjump-misses-init', '-Wformat=2', '-Wshadow', '-DDL_USE_EXTENSIONS=0']],
#  ['small', ['-Os']],
#  ['fast', ['-O3']],
#  ['prof', ['-pg']]
]

build_config = gcc_configs

executables = [
	['suite',
   ['src/test_dl_math.c', 'src/test_dl_test.c', 'src/test_dl_vector.c', 'src/test_dl_linked_list.c', 'src/test_dl_collections.c', 'src/test_dl_algorithms.c', 'src/test_containers.c', 'src/test_dl_macro.c', 'src/suite.c'],
   [lib_m],
   c_stds]
]

if target_machine.system() == 'windows'
  lib_allegro = cc.find_library('allegro', dirs : [meson.source_root() + '/deps/windows/allegro/lib'])
  lib_allegro_primitives = cc.find_library('allegro_primitives', dirs : [meson.source_root() + '/deps/windows/allegro/lib'])
  inc = include_directories('deps/windows/allegro/include', 'include')
else
  lib_allegro = dependency('allegro-5', required : false)
  lib_allegro_primitives = dependency('allegro_primitives-5', required : false)
endif

if lib_allegro.found() and lib_allegro_primitives.found()
	executables += [['tween', ['src/tween.c'], [lib_m, lib_allegro, lib_allegro_primitives], [ 'c99', 'c11']]]
	standard_flags += ['-DENABLE_TWEEN_TEST=1']
endif

foreach exe : executables
	exe_name = exe[0]
	exe_files = exe[1]
	exe_deps = exe[2]
  exe_stds = exe[3]
	foreach std : exe_stds
		foreach conf : build_config
			conf_name = conf[0]
			conf_flags = conf[1]
			executable(exe_name + '-' + std + '-' + conf_name, exe_files,
				include_directories: inc,
				c_args: conf_flags + ['-std=' + std] + standard_flags,
				link_args: conf_flags,
				dependencies: exe_deps)
		endforeach
	endforeach
endforeach

executable('container_speed_linked_list',
	'src/container_speed.c',
	c_args: ['-DDL_TEST_LINKED_LIST=1', '-O3', '-pg'],
	link_args: ['-O3', '-pg'],
	include_directories : inc,
	dependencies : [lib_m])

executable('container_speed_vector',
	'src/container_speed.c',
	c_args: ['-DDL_TEST_VECTOR=1', '-O3', '-pg'],
	link_args: ['-O3', '-pg'],
	include_directories : inc,
	dependencies : [lib_m])
