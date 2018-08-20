project('dl', 'c')

inc = include_directories('include')
cc = meson.get_compiler('c')

lib_allegro = dependency('allegro-5')
lib_allegro_primitives = dependency('allegro_primitives-5')
lib_m = cc.find_library('m', required : true)

gcc_configs = [
	['c99-no-malloc', ['-std=c99', '-DDL_USE_MALLOC=0']],
	['c89-pedantic', ['-std=c89', '-pedantic']],
	['small', ['-s', '-Os', '-fomit-frame-pointer', '-ffunction-sections', '-fdata-sections', '-Wl,--gc-sections', '-fno-unwind-tables', '-fno-asynchronous-unwind-tables', '-fno-math-errno', '-fmerge-all-constants', '-fno-unroll-loops', '-fsingle-precision-constant', '-Wl,-z,norelro', '-Wl,--gc-sections', '-Wl,--strip-all']]
]

build_config = gcc_configs

executables = [
	['tween', ['src/tween.c'], [lib_m, lib_allegro, lib_allegro_primitives]],
	['suite', ['src/test_dl_collections.c', 'src/test_dl_math.c', 'src/test_dl_test.c', 'src/suite.c'], [lib_m]]
]

foreach exe : executables
	foreach conf : build_config
		name = conf[0]
		flags = conf[1]
		executable(exe[0] + '-' + name, exe[1],
			include_directories: inc,
			c_args: flags,
			dependencies: exe[2])
	endforeach
endforeach
