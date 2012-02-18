#!/usr/bin/env python

import sys, os
from os.path import abspath, exists
from shutil import copy, rmtree

def all(all):
    os.system('node-waf clean')
    os.system('node-waf configure')
    os.system('node-waf')

def clean(cln):
  if exists('build'): rmtree('build')

def set_options(conf):
    conf.tool_options("compiler_cxx")

def configure(conf):
    conf.check_tool("compiler_cxx")
    conf.check_tool('node_addon')

    srcpath = abspath("deps/sigar")
    buildpath = abspath("build/sigar")
    destpath = abspath("build/Release")
    cmd = "cp -rp %s %s && cd \"%s\" && ./autogen.sh && ./configure --prefix=%s --with-pic && make && make install"
    if os.system(cmd % (srcpath, buildpath, buildpath, destpath)) != 0:
        conf.fatal("Configuring sigar failed.")
    conf.env.append_value("LIBPATH_SIGAR", abspath("build/Release/lib/"))
    conf.env.append_value("CPPPATH_SIGAR", abspath("build/Release/include/"))
    conf.env.append_value("STATICLIB_SIGAR", ["sigar"])

def build(conf):
    t = conf.new_task_gen('cxx', 'shlib', 'node_addon')
    t.source = [
        'src/node_sigar.cc'
    ]
    #t.cxxflags = ['-g', '-D_FILE_OFFSET_BITS=64', '-D_LARGEFILE_SOURCE', '-Wall'] 
    t.target = 'sigar'
    t.uselib = 'SIGAR'
