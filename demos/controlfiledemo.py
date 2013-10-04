#!/usr/bin/env python3
#
#

import LPMD

c = LPMD.ControlFile('test.control')
for name in c:
    for item in c[name]:
        if type(item) == LPMD.Statement:
           print ('STATEMENT ', name, item.options, item.positional)
           if item.extended != None: print ('-> EXTENDED ', item.extended.options, item.extended.positional)
        else:
           print ('BLOCK ', item.name, item.alias, item.options)

