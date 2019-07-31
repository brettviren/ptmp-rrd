import generic

name = __name__

def options(opt):
    generic._options(opt, name)

def configure(cfg):
    generic._configure(cfg, name, incs=('rrd.h',), libs=('rrd',),
                       mandatory=True)
