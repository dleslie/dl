project('dl', 'c')

inc = include_directories('include')
cc = meson.get_compiler('c')

lib_allegro = dependency('allegro-5')
lib_allegro_primitives = dependency('allegro_primitives-5')
lib_m = cc.find_library('m', required : true)

c_stds = ['c11']#['c89', 'c90', 'c99', 'c11']

gcc_configs = [
	['no-malloc', ['-DDL_USE_MALLOC=0']],
	['pedantic', ['-pedantic', '-Wall', '-Werror']],
	['small', ['-Os']],
	['fast', ['-O3']]
]

build_config = gcc_configs

executables = [
	['tween', ['src/tween.c'], [lib_m, lib_allegro, lib_allegro_primitives]],
	['suite', ['src/test_dl_collections.c', 'src/test_dl_math.c', 'src/test_dl_test.c', 'src/suite.c'], [lib_m]],
]

foreach exe : executables
	exe_name = exe[0]
	exe_files = exe[1]
	exe_deps = exe[2]
	foreach std : c_stds
		foreach conf : build_config
			conf_name = conf[0]
			conf_flags = conf[1]
			executable(exe_name + '-' + std + '-' + conf_name, exe_files,
				include_directories: inc,
				c_args: conf_flags + ['-std=' + std],
				dependencies: exe_deps)
		endforeach
	endforeach
endforeach

executable('container_speed_linked_list',
	'src/container_speed.c',
	c_args: ['-DDL_TEST_LINKED_LIST=1 -O3'],
	include_directories : inc,
	dependencies : [lib_m])

executable('container_speed_vector',
	'src/container_speed.c',
	c_args: ['-DDL_TEST_VECTOR=1 -O3'],
	include_directories : inc,
	dependencies : [lib_m])
