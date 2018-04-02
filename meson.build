project('dl', 'c')

inc = include_directories('include')
cc = meson.get_compiler('c')

lib_allegro = dependency('allegro-5')
lib_allegro_primitives = dependency('allegro_primitives-5')
lib_m = cc.find_library('m', required : true)

executable('tween', 'src/tween.c',
	   include_directories : inc,
	   dependencies : [lib_m, lib_allegro, lib_allegro_primitives])

executable('suite',
	   'src/test_dl_collections.c', 'src/test_dl_math.c', 'src/test_dl_test.c', 'src/suite.c',
	   include_directories : inc,
	   dependencies : [lib_m])
