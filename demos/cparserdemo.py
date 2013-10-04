#!/usr/bin/env python3

import sys
from CParser import CParser, Block
from CFormatter import CFormatter

#
#
#
if len(sys.argv) < 2: sys.exit('Usage: cparserdemo.py <C source file>')
cp = CParser()
tree = cp.Parse(open(sys.argv[1], 'r').read())
#def visitor(node):
#    if isinstance(node, Block) and node.type == 'for': print (node, end='\n\n')
#cp.Visit(tree, visitor)
code = CFormatter().Format(tree)
print (code)

