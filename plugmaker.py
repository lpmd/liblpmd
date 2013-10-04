#!/usr/bin/env python3

import os, sys, string, os.path

fakereadlink = lambda x: os.readlink(x) if os.path.islink(x) else x
exename = os.path.abspath(os.path.join(os.path.dirname(sys.argv[0]), fakereadlink(sys.argv[0])))
mydir = os.path.join(os.path.dirname(os.path.dirname(exename)), 'lib')
sys.path = [mydir]+sys.path

from lpplugmaker import *
from lpplugmaker.PluginProcessor import CompileMetadata, RenderPlugin

#
#
#
if len(sys.argv) < 3: sys.exit('Usage: plugmaker <plugcode file> <output file>')

md = CompileMetadata(open(sys.argv[1], 'r').read(), sys.argv[1])
stream = sys.stdout if sys.argv[2] == '-' else open(sys.argv[2], 'w')
stream.write(RenderPlugin(md))

