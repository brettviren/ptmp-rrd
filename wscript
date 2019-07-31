#!/usr/bin/env waf

import sys
from waflib.Utils import to_list

sys.path.append('tools')


pkg_deps = ['librrd', 'libzmq','libczmq','protobuf','ptmp']

def options(opt):
    opt.load('compiler_c compiler_cxx')
    opt.load('utests')
    for pkg in pkg_deps:
        opt.load(pkg)
    opt.add_option('--cxxflags', default='-O2 -ggdb3')

    
def configure(cfg):
    cfg.load('compiler_c compiler_cxx')
    cfg.load('utests')

    for pkg in pkg_deps:
        cfg.load(pkg)

    cfg.env.CXXFLAGS += to_list(cfg.options.cxxflags)


def build(bld):
    bld.load('utests')

    uses = [p.upper() for p in pkg_deps]
    rpath = [bld.env["PREFIX"] + '/lib']
    for u in uses:
        p = bld.env["LIBPATH_%s"%u]
        if p: rpath += p

    src = bld.path.ant_glob("src/*.cc")

    bld.shlib(features='c cxx',
              includes='inc include .',
              rpath = rpath,
              source = src,
              target='ptmp-rrd',
              use=uses)


    bld.utesting('ptmp-rrd', uses)
