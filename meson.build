project('dl', 'c')

inc = include_directories('include')
cc = meson.get_compiler('c')

lib_allegro = dependency('allegro-5')
lib_allegro_primitives = dependency('allegro_primitives-5')
lib_m = cc.find_library('m', required : true)

executable('tween', 'src/tween.c',
	   include_directories : inc,
#	   c_args: ['-DDL_USE_TEST=0', '-DDL_USE_LOGGING=0', '-std=c89', '-pedantic'],
#	   c_args: ['-s', '-Os', '-fomit-frame-pointer', '-ffunction-sections', '-fdata-sections', '-Wl,--gc-sections', '-fno-unwind-tables', '-fno-asynchronous-unwind-tables', '-fno-math-errno', '-fmerge-all-constants', '-fno-unroll-loops', '-fsingle-precision-constant', '-Wl,-z,norelro'],
	   dependencies : [lib_m, lib_allegro, lib_allegro_primitives])

executable('suite',
	   'src/test_dl_collections.c', 'src/test_dl_math.c', 'src/test_dl_test.c', 'src/suite.c',
	   include_directories : inc,
#	   c_args: ['-DDL_USE_TEST=0', '-DDL_USE_LOGGING=0', '-std=c89', '-pedantic'],
#	   c_args: ['-s', '-Os', '-fomit-frame-pointer', '-ffunction-sections', '-fdata-sections', '-Wl,--gc-sections', '-fno-unwind-tables', '-fno-asynchronous-unwind-tables', '-fno-math-errno', '-fmerge-all-constants', '-fno-unroll-loops', '-fsingle-precision-constant', '-Wl,-z,norelro'],
	   dependencies : [lib_m])
