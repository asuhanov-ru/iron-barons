#! /usr/bin/env python
# encoding: utf-8
# a1batross, mittorn, 2018

from waflib import Build, Context, Logs
from waflib.Tools import waf_unit_test, c_tests
import sys
import os

VERSION = '0.99'
APPNAME = 'iron-barons'
top = '.'
default_prefix = '/' # Waf uses it to set default prefix

c_tests.LARGE_FRAGMENT='''#include <unistd.h>
int check[sizeof(off_t) >= 8 ? 1 : -1]; int main(void) { return 0; }'''

class Subproject:
    def __init__(self, name, fnFilter = None):
        self.name = name
        self.fnFilter = fnFilter

    def is_exists(self, ctx):
        return ctx.path.find_node(self.name + '/wscript')

    def is_enabled(self, ctx):
        if not self.is_exists(ctx):
            return False

        if self.fnFilter:
            return self.fnFilter(ctx)

        return True

class RefDll:
    def __init__(self, name, default, key = None):
        self.name = name
        self.default = default
        self.dest = key if key else name.upper()

    def register_option(self, opt):
        kw = dict()
        if self.default:
            act = 'disable'
            kw['action'] = 'store_false'
        else:
            act = 'enable'
            kw['action'] = 'store_true'

        key = '--%s-%s' % (act, self.name)

        kw['dest'] = self.dest
        kw['default'] = self.default
        kw['help'] = '%s %s renderer [default: %%default]' % (act, self.name)

        opt.add_option(key, **kw)

    def register_env(self, env, opts, force):
        env[self.dest] = force or opts.__dict__[self.dest]

    def register_define(self, conf):
        conf.define_cond('REF_%s_ENABLED' % self.dest, conf.env[self.dest])

SUBDIRS = [
    # always configured and built
	Subproject('public'),
    Subproject('engine'),
    Subproject('game', False),
    Subproject('editor', False),
]

REFDLLS = [
    RefDll('soft', True),
    RefDll('gl', True),
]

Context.Context.line_just = 55 # should fit for everything on 80x26

def options(opt):
    opt.load('reconfigure compiler_optimizations xshlib xcompile compiler_cxx compiler_c sdl2 clang_compilation_database strip_on_install waf_unit_test msdev msvs msvc subproject cmake')

    grp = opt.add_option_group('Common options')

    grp.add_option('-d', '--dedicated', action = 'store_true', dest = 'DEDICATED', default = False,
        help = 'build Xash Dedicated Server [default: %default]')

    grp.add_option('--gamedir', action = 'store', dest = 'GAMEDIR', default = 'valve',
        help = 'engine default game directory [default: %default]')

    grp.add_option('-8', '--64bits', action = 'store_true', dest = 'ALLOW64', default = False,
        help = 'allow targetting 64-bit engine(Linux/Windows/OSX x86 only) [default: %default]')

    grp.add_option('-P', '--enable-packaging', action = 'store_true', dest = 'PACKAGING', default = False,
        help = 'respect prefix option, useful for packaging for various operating systems [default: %default]')

    grp.add_option('--enable-bundled-deps', action = 'store_true', dest = 'BUILD_BUNDLED_DEPS', default = False,
        help = 'prefer to build bundled dependencies (like opus) instead of relying on system provided')

    grp.add_option('--enable-bsp2', action = 'store_true', dest = 'SUPPORT_BSP2_FORMAT', default = False,
        help = 'build engine and renderers with BSP2 map support(recommended for Quake, breaks compatibility!) [default: %default]')

    grp.add_option('--low-memory-mode', action = 'store', dest = 'LOW_MEMORY', default = 0, type = 'int',
        help = 'enable low memory mode (only for devices have <128 ram)')

    grp.add_option('--disable-werror', action = 'store_true', dest = 'DISABLE_WERROR', default = False,
        help = 'disable compilation abort on warning')

    grp.add_option('--enable-tests', action = 'store_true', dest = 'TESTS', default = False,
        help = 'enable building standalone tests (does not enable engine tests!) [default: %default]')

    # a1ba: special option for me
    grp.add_option('--debug-all-servers', action='store_true', dest='ALL_SERVERS', default=False, help='')

    grp = opt.add_option_group('Renderers options')

    grp.add_option('--enable-all-renderers', action='store_true', dest='ALL_RENDERERS', default=False,
        help = 'enable all renderers supported by Xash3D FWGS [default: %default]')

    for dll in REFDLLS:
        dll.register_option(grp)
        
    for i in SUBDIRS:
        if not i.is_exists(opt):
            continue

        opt.add_subproject(i.name)

def configure(conf):
	conf.load('fwgslib reconfigure compiler_optimizations')
	conf.env.MSVC_TARGETS = ['x86' if not conf.options.ALLOW64 else 'x64']

	# Load compilers early
	conf.load('xshlib xcompile compiler_c compiler_cxx cmake gccdeps')

	# HACKHACK: override msvc DEST_CPU value by something that we understand
	if conf.env.DEST_CPU == 'amd64':
		conf.env.DEST_CPU = 'x86_64'

	if conf.env.COMPILER_CC == 'msvc':
		conf.load('msvc_pdb')

	conf.load('msvs msdev subproject clang_compilation_database strip_on_install waf_unit_test enforce_pic cmake')

	# Force XP compatibility, all build targets should add subsystem=bld.env.MSVC_SUBSYSTEM
	if conf.env.MSVC_TARGETS[0] == 'x86':
		conf.env.MSVC_SUBSYSTEM = 'WINDOWS,5.01'
		conf.env.CONSOLE_SUBSYSTEM = 'CONSOLE,5.01'
	else:
		conf.env.MSVC_SUBSYSTEM = 'WINDOWS'
		conf.env.CONSOLE_SUBSYSTEM = 'CONSOLE'

	enforce_pic = True # modern defaults

	conf.check_pic(enforce_pic)

	# We restrict 64-bit builds ONLY for Win/Linux/OSX running on Intel architecture
	# Because compatibility with original GoldSrc
	if conf.env.DEST_OS in ['win32', 'linux', 'darwin'] and conf.env.DEST_CPU == 'x86_64':
		conf.env.BIT32_MANDATORY = not conf.options.ALLOW64
		if conf.env.BIT32_MANDATORY:
			Logs.info('WARNING: will build engine for 32-bit target')
	else:
		conf.env.BIT32_MANDATORY = False

	conf.load('force_32bit')

	cflags, linkflags = conf.get_optimization_flags()
	cxxflags = list(cflags) # optimization flags are common between C and C++ but we need a copy

	conf.check_cc(cflags=cflags, linkflags=linkflags, msg='Checking for required C flags')
	conf.check_cxx(cxxflags=cxxflags, linkflags=linkflags, msg='Checking for required C++ flags')

	conf.env.append_unique('CFLAGS', cflags)
	conf.env.append_unique('CXXFLAGS', cxxflags)
	conf.env.append_unique('LINKFLAGS', linkflags)

	if conf.env.COMPILER_CC != 'msvc':
		opt_flags = [
			# '-Wall', '-Wextra', '-Wpedantic',
			'-fdiagnostics-color=always',

			# stable diagnostics, forced to error, sorted
			'-Werror=alloc-size',
			'-Werror=bool-compare',
			'-Werror=bool-operation',
			'-Werror=cast-align=strict',
			'-Werror=duplicated-cond',
			# '-Werror=format=2',
			'-Werror=implicit-fallthrough=2',
			'-Werror=logical-op',
			'-Werror=nonnull',
			'-Werror=packed',
			'-Werror=packed-not-aligned',
			'-Werror=parentheses',
			'-Werror=return-type',
			'-Werror=sequence-point',
			'-Werror=sizeof-pointer-memaccess',
			'-Werror=sizeof-array-div',
			'-Werror=sizeof-pointer-div',
			'-Werror=strict-aliasing',
			'-Werror=string-compare',
			'-Werror=tautological-compare',
			'-Werror=use-after-free=3',
			'-Werror=unsequenced', # clang's version of -Werror=sequence-point
			'-Werror=vla',
			'-Werror=write-strings',

			# unstable diagnostics, may cause false positives
			'-Winit-self',
			'-Wmisleading-indentation',
			'-Wstringop-overflow',
			'-Wunintialized',
			'-Walloc-zero',

			# disabled, flood
			# '-Wdouble-promotion',

			'-Wunused-function',
			'-Wunused-variable',
			'-Wunused-but-set-variable',
		]

		opt_cflags = [
			'-Werror=declaration-after-statement',
			'-Werror=enum-conversion',
			'-Werror=implicit-int',
			'-Werror=implicit-function-declaration',
			'-Werror=incompatible-pointer-types',
			'-Werror=int-conversion',
			'-Werror=jump-misses-init',
			'-Werror=old-style-declaration',
			'-Werror=old-style-definition',
			'-Werror=strict-prototypes',
			'-fnonconst-initializers', # owcc
			'-Wmissing-prototypes', # not an error yet
		]

		opt_cxxflags = [] # TODO:

		if conf.options.DISABLE_WERROR:
			opt_flags = []
			opt_cflags = ['-Werror=implicit-function-declaration']
			opt_cxxflags = []

		conf.env.CFLAGS_werror = conf.filter_cflags(opt_flags + opt_cflags, cflags)
		conf.env.CXXFLAGS_werror = conf.filter_cxxflags(opt_flags + opt_cxxflags, cxxflags)

	conf.env.TESTS         = conf.options.TESTS
	conf.env.DEDICATED     = conf.options.DEDICATED
	conf.env.SUPPORT_BSP2_FORMAT = conf.options.SUPPORT_BSP2_FORMAT

	conf.env.DEFAULT_RPATH = '$ORIGIN'

	setattr(conf, 'refdlls', REFDLLS)

	for refdll in REFDLLS:
		refdll.register_env(conf.env, conf.options, conf.options.ALL_RENDERERS)

	conf.env.GAMEDIR = conf.options.GAMEDIR
	conf.define('IB_GAMEDIR', conf.options.GAMEDIR)
	conf.define_cond('IB_ALL_SERVERS', conf.options.ALL_SERVERS)

	if conf.env.DEST_OS == 'win32':
		# Common Win32 libraries
		# Don't check them more than once, to save time
		# Usually, they are always available
		# but we need them in uselib
		a = [ 'user32', 'shell32', 'gdi32', 'advapi32', 'dbghelp', 'psapi', 'ws2_32' ]
		if conf.env.COMPILER_CC == 'msvc':
			for i in a:
				conf.start_msg('Checking for MSVC library')
				conf.check_lib_msvc(i)
				conf.end_msg(i)
		else:
			for i in a:
				conf.check_cc(lib = i)
	else:
		conf.check_cc(lib='dl', mandatory = False)
		conf.check_cc(lib='m')

	conf.define('IB_LOW_MEMORY', conf.options.LOW_MEMORY)

	for i in SUBDIRS:
		if not i.is_enabled(conf):
			continue

		conf.add_subproject(i.name)

def build(bld):
	# guard rails to not let install to root
	if bld.is_install and not bld.options.PACKAGING and not bld.options.destdir:
		bld.fatal('Set the install destination directory using --destdir option')

	# don't clean QtCreator files and reconfigure saved options
	bld.clean_files = bld.bldnode.ant_glob('**',
		excl='*.user configuration.py .lock* *conf_check_*/** config.log %s/*' % Build.CACHE_DIR,
		quiet=True, generator=True)

	bld.load('xshlib')

	for i in SUBDIRS:
		if not i.is_enabled(bld):
			continue

		bld.add_subproject(i.name)

	if bld.env.TESTS:
		bld.add_post_fun(waf_unit_test.summary)
		bld.add_post_fun(waf_unit_test.set_exit_code)
