#
#
#

import sys, os, binascii, pickle, zlib
from os import path

from lpmdmodule import SlotHandler, Slot

RemoveQuotes = lambda x: x.replace('\"', '')

default_test = ['test \"Default test\"', [ ['beforetest', []], ['aftertest', []], 
                 ['test', ['assert False']]] ]

def Deserialize(d):
    return pickle.loads(binascii.a2b_base64(zlib.decompress(binascii.a2b_base64(bytes(d, 'ascii')))))

class HiddenSlot:

    def __init__(self, realslot, options):
        self.slot, self.options = realslot, options

    def __call__(self, simulation, options=None, modify=False, useboundaries=False, ismask=False, inverse=False):
        p_options = (self.options if options == None else options)
        self.slot.ismask = ismask
        self.slot.inverse = inverse
        return self.slot(simulation, p_options, modify, useboundaries)

class Plugin:

    def __init__(self, path):
        self.slothandler, self.options, self.metadata = SlotHandler(path), dict(), dict()
        try:
           self.metadata = Deserialize(self.slothandler.GetMetadata())
        except: pass
        
    def __getattr__(self, name):
        if name == 'metadata': return self.metadata
        elif name == 'ShowDocumentation': return self.ShowDocumentation
        elif name == 'PerformTests': return self.PerformTests
        slot = self.slothandler.GetSlot(name)
        return HiddenSlot(Slot(slot, self.slothandler.GetSlot('Plugin_SetKeyword')), self.options)

    def Parameters(self): return [' '.join(z[0]) for z in self.metadata['parameter']]

    def ShowDocumentation(self):
        print ('%s plugin, version %s' % (self.metadata['plugin'][0], self.metadata['version']))
        print ('Written by %s' % RemoveQuotes(self.metadata['author']))
        print ('\n%s\n' % RemoveQuotes(self.metadata['plugin'][1]))
        if 'parameter' in self.metadata:
           print ('Parameters recognized:\n')
           for param in self.metadata['parameter']:
               pr_opts = ''
               if 'default' in param[2]: pr_opts += (', default: %s' % param[2]['default'])
               if 'unit' in param[2]: pr_opts += (', unit: %s' % param[2]['unit'])
               print ('%s (type: %s%s): %s' % (' '.join(param[0]), param[1], pr_opts, RemoveQuotes(param[3])))
        else: print ('This plugins does not recognize any parameters\n')
        if 'example' in self.metadata: print ('\nExample of use:\n\n%s' % self.metadata['example'])
        else: print ('\nNo example available\n\n')

    def PerformTests(self):
        test_battery = self.metadata['test'] if 'test' in self.metadata else [ default_test ]
        failed = 0
        for test in test_battery:
            sys.stderr.write('%s: ' % RemoveQuotes(' '.join(test[0].split()[1:])))
            d, result = dict(), False
            for x in test[1]: d[x[0]] = '\n'.join(x[1])
            exec (d['beforetest'])
            try:
               exec (d['test'])
               result = True
            except: result = False
            exec (d['aftertest'])
            sys.stderr.write(('PASSED' if result else 'FAILED')+'\n')
            if not result: failed += 1
        print ('%d out of %d tests failed.' % (failed, len(test_battery)))

class PluginManager:

    def __init__(self, pluginpath):
        pluginpath = pluginpath
        self.pluginpath = pluginpath
        self.plugins = dict()

    def FindPlugin(self, name):
        for d in self.pluginpath:
            if path.exists(path.join(d, name+'.so')): return path.join(d, name+'.so')
        raise RuntimeError('Could not find plugin named %s' % name)

    def __getitem__(self, key):
        if key in self.plugins: return self.plugins[key]
        path = self.FindPlugin(key)
        self.plugins[key] = Plugin(path)
        return self.plugins[key]

    def Load(self, name, options=dict(), alias=None):
        plugin = None
        if alias is None:
           plugin, plugin.options = self[name], options
        else: 
           self.plugins[alias] = Plugin(self.FindPlugin(name))
           plugin, plugin.options = self.plugins[alias], options
        return plugin

