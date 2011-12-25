#!/usr/bin/env python

import sys, os
from os.path import abspath

def set_options(conf):
    conf.tool_options("compiler_cxx")

def configure(conf):
    conf.check_tool("compiler_cxx")
    conf.check_tool('node_addon')

    # hasSigarHeader = conf.check(header_name="sigar.h", errmsg="Missing include files for sigar", mandatory=False)
    # hasSigarLib = conf.check_cxx(lib="sigar", uselib_store='SIGAR', errmsg="Missing sigar", mandatory=False)
    

    conf.env.append_value("LIBPATH_SIGAR", "/usr/local/lib")
    conf.env.append_value("CPPPATH_SIGAR", "/usr/local/include/")
    conf.env.append_value("LIB_SIGAR", "sigar")

    # if not hasSigarHeader and not hasSigarLib:
    #     srcpath = abspath("deps/sigar")
    #     temppath = abspath("build/sigar")
    #     buildpath = abspath("build/Release")
    #     cmd = "rm -rf \"%s\" && cp -rp \"%s\" \"%s\" && cd \"%s\" && cmake . && make && make install"
    #     if os.system(cmd % (temppath, srcpath, temppath, temppath)) != 0:
    #         conf.fatal("Configuring sigar failed.")
    #     conf.check(header_name="sigar.h", errmsg="Missing include files for sigar", mandatory=True)
    #     conf.check_cxx(lib="sigar", uselib_store='SIGAR', errmsg="Missing sigar", mandatory=True)

    # if not hasSigarHeader and not hasSigarLib:
    #     srcpath = abspath("deps/sigar")
    #     temppath = abspath("build/sigar")
    #     buildpath = abspath("build/Release")
    #     cmd = "cp -rp %s %s && cd \"%s\" && cmake -DCMAKE_INSTALL_PREFIX=%s && make && make install"
    #     if os.system(cmd % (srcpath, temppath, temppath, buildpath)) != 0:
    #         conf.fatal("Configuring sigar failed.")
    #     conf.env.append_value("LIBPATH_SIGAR", abspath("build/Release/lib/"))
    #     conf.env.append_value("CPPPATH_SIGAR", abspath("build/Release/include/"))
    #     conf.env.append_value("STATICLIB_SIGAR",["sigar"])

def build(conf):
    t = conf.new_task_gen('cxx', 'shlib', 'node_addon')
    t.source = [
        'src/node_sigar.cc',
        # conf.glob('deps/sigar/include/*.h'),
        # conf.glob('deps/sigar/src/**/*.[ch]'),
        # conf.glob('deps/sigar/src/**/*.in')
    ]
    t.target = 'sigar'
    t.uselib = 'SIGAR'
    t.linkflags = ['-lsigar']
    t.cxxflags = ["-g", "-fPIC", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE", "-Wall"]
    #t.lib = ['sigar']

    # Dir.glob("bindings/ruby/**/*") +
    # Dir.glob("include/*.h") +
    # Dir.glob("src/**/*.[ch]") +
    # Dir.glob("src/**/*.in")
    # t.includes = ["lib/", "/usr/local/include/libgtop-2.0/", "/usr/local/include/glib-2.0/",]
    #t.use = ['SIGAR']
    #t.include = ['lib/']
