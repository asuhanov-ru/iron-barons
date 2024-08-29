#! /usr/bin/env python
# encoding: utf-8
# a1batross, mittorn, 2018

from waflib import Build, Context, Logs
from waflib.Tools import waf_unit_test, c_tests
import sys
import os

VERSION = '0.01'
APPNAME = 'iron-barons'
top = '.'
default_prefix = '/' # Waf uses it to set default prefix

Context.Context.line_just = 55 # should fit for everything on 80x26

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


SUBDIRS = [
	# always configured and built

	# Game dlls
	Subproject('game/cl_dll'),
	Subproject('game/ib'),
	# Subproject('game/common'),
]


def options(opt):
	opt.load('reconfigure compiler_optimizations compiler_cxx compiler_c sdl2 clang_compilation_database strip_on_install waf_unit_test msdev msvs subproject cmake')

	grp = opt.add_option_group('Common options')

	grp.add_option('--gamedir', action = 'store', dest = 'GAMEDIR', default = 'valve',
		help = 'engine default game directory [default: %(default)s]')
	
	grp.add_option('-8', '--64bits', action = 'store_true', dest = 'ALLOW64', default = False,
		help = 'allow targetting 64-bit engine(Linux/Windows/OSX x86 only) [default: %(default)s]')

	grp.add_option('--disable-werror', action = 'store_true', dest = 'DISABLE_WERROR', default = False,
		help = 'disable compilation abort on warning')


	for i in SUBDIRS:
		if not i.is_exists(opt):
			continue

		opt.add_subproject(i.name)

def configure(conf):
	conf.load('fwgslib reconfigure compiler_optimizations')
	conf.env.MSVC_TARGETS = ['x86' if not conf.options.ALLOW64 else 'x64']

	# Load compilers early
	conf.load('compiler_c compiler_cxx cmake gccdeps')

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

		opt_cxxflags = ['/EHsc'] # TODO:

		if conf.options.DISABLE_WERROR:
			opt_flags = []
			opt_cflags = ['-Werror=implicit-function-declaration']
			opt_cxxflags = []

		conf.env.CFLAGS_werror = conf.filter_cflags(opt_flags + opt_cflags, cflags)
		conf.env.CXXFLAGS_werror = conf.filter_cxxflags(opt_flags + opt_cxxflags, cxxflags)
	conf.env.DEFAULT_RPATH = '$ORIGIN'

	conf.env.GAMEDIR = conf.options.GAMEDIR
	conf.define('IB_GAMEDIR', conf.options.GAMEDIR)

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


	# set _FILE_OFFSET_BITS=64 for filesystems with 64-bit inodes
	# must be set globally as it changes ABI
	if conf.env.DEST_OS not in ['psvita']:
		conf.check_large_file(compiler = 'c', execute = False)

	# indicate if we are packaging for Linux/BSD
	conf.env.SHAREDIR = conf.env.LIBDIR = conf.env.BINDIR = conf.env.PREFIX

	for i in SUBDIRS:
		if not i.is_enabled(conf):
			continue

		conf.add_subproject(i.name)

def build(bld):
	# guard rails to not let install to root
	if bld.is_install and not bld.options.destdir:
		bld.fatal('Set the install destination directory using --destdir option')

	# don't clean QtCreator files and reconfigure saved options
	bld.clean_files = bld.bldnode.ant_glob('**',
		excl='*.user configuration.py .lock* *conf_check_*/** config.log %s/*' % Build.CACHE_DIR,
		quiet=True, generator=True)

	for i in SUBDIRS:
		if not i.is_enabled(bld):
			continue

		bld.add_subproject(i.name)

	if bld.env.TESTS:
		bld.add_post_fun(waf_unit_test.summary)
		bld.add_post_fun(waf_unit_test.set_exit_code)
