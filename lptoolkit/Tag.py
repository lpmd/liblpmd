#
#
#

from lptoolkit.LPMD import globaltags

def Tag(name):
    if name not in globaltags: 
       if len(globaltags) == 64: raise IndexError('Attempted to register more than 64 tags')
       globaltags[name] = str(len(globaltags))
    return globaltags[name]

