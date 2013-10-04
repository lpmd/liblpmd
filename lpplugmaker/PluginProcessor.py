#!/usr/bin/env python3
#
#
#

import sys, binascii, zlib, pickle
import os.path
from copy import deepcopy
from lpplugmaker.CParser import *
from lpplugmaker.CFormatter import *

oneliner_keywords = ('plugin', 'version', 'author', 'parameter', 'global', 'require', 'define')
block_kw = ('example', 'test', 'beforetest', 'aftertest', 'macro', 'slot', 'function')
plugin_includes = ('<stdio.h>', '<stdlib.h>', '<string.h>', '<assert.h>', '<math.h>', '<unistd.h>', 
                   '<time.h>', '<lpmd/dapi.h>', '<lpmd/mtwist.h>', '<lpmd/params.h>')
realvtype = { 'real': 'double', 'integer': 'long', 'integer buffer': 'long *', 'AtomPair buffer': 'AtomPair *', 
              'tag': 'int', 'boolean': 'int', 'Vector': 't_innervector', 'Vector buffer': 'double *', 
              'string': 'const char *', 'file': 'FILE *' }

def Serialize(d):
    return binascii.b2a_base64(zlib.compress(binascii.b2a_base64(pickle.dumps(d)))).decode('ascii')

MAXVECTORS = 1024

def unique_list(seq, key):
    tmp = list()
    for item in seq:
        if key(item) not in (key(z) for z in tmp): tmp.append(item)
    return tmp

def ExpandIncludes(incode, path):
    path = os.path.abspath(path)
    while '@include ' in incode:
          s1 = incode.find('@include ')
          s2 = incode.find('\n', s1+9)
          relpath = os.path.join(os.path.dirname(path), incode[s1+9:s2].strip())+'.plugcode'
          incode = incode.replace(incode[s1:s2], open(relpath, 'r').read())
    return incode

def CompileMetadata(incode, path):
    Ignorable = lambda line: (line.strip() == '' or line.lstrip().startswith('//'))
    incode = ExpandIncludes(incode, path)
    while '\\\n' in incode: incode = incode.replace('\\\n', '')
    pos, stack, metadata = 0, [['GLOBAL', list()]], { 'plugcode': incode }
    cp = CParser()
    while '\n' in incode[pos:]:
        newpos = incode.find('\n', pos)
        line = incode[pos:newpos].strip()
        pos = newpos+1
        if Ignorable(line): continue
        elif line.startswith('@'):
           keyword, rest = line.split()[0][1:], ' '.join(line.split()[1:])
           if keyword in block_kw:
              if keyword in ('slot', 'function'):
                 q1 = incode.find('{', pos)
                 q2 = FindMatching(q1, incode)
                 slotcode, r2 = incode[q1+1:q2], -1
                 while '@macro ' in slotcode:
                       r1 = slotcode.find('@macro ', r2+1)
                       r2 = slotcode.find('{', r1+6)
                       r3 = FindMatching(r2, slotcode)
                       stack[-1][1].append([slotcode[r1+1:r2].strip(), slotcode[r2+1:r3]])
                       slotcode = slotcode[:r1]+'\n'+slotcode[r3+1:]
                 stack[-1][1].append([line[1:], slotcode])
                 pos = q2+1
              elif keyword == 'macro':
                 q1 = incode.find('{', pos)
                 q2 = FindMatching(q1, incode)
                 stack[-1][1].append([line[1:], incode[q1+1:q2]])
                 pos = q2+1
              else: stack.append([line[1:], list()])
           elif keyword == 'end':
              z = stack.pop()
              stack[-1][1].append(z)
           elif keyword in oneliner_keywords: stack[-1][1].append([line[1:], None])
           else: raise SyntaxError('Unknown keyword, \'%s\'' % keyword)
        elif len(stack) > 0: stack[-1][1].append(line)
    assert stack[0][0] == 'GLOBAL'
    def AddAnother(obj, key):
        if key not in metadata: metadata[key] = list()
        metadata[key].append(obj)
    def ParseVariable(kw, x):
        x = x.replace('(', ' ( ').replace(')', ' ) ')
        options, opd = ('as', 'default', 'unit'), dict()
        while any(' %s ' % word in x for word in options):
              for word in options:
                  if (' %s ' % word) not in x: continue
                  lspl = x.split()
                  i = [ i for i, c in enumerate(lspl) if c.strip() == word ][0]
                  opd[word], x = lspl[i+1], ' '.join(lspl[:i]+lspl[i+2:])
        cl = (x.find(':') if ':' in x else len(x))
        decl, doc = x[len(kw):cl].strip(), x[cl+1:].strip()
        if '(' not in decl: raise SyntaxError('Missing parameter type, \"%s\"', decl)
        vtype = (lambda z: decl[z+1:FindMatching(z,decl)].strip())(z=decl.find('('))
        names = (lambda z: [z.strip()] if ',' not in z else [w.strip() for w in z.split(',')])(z=decl[:decl.find('(')].strip())
        return (names, vtype, opd, doc)
    def ParseDec(kw, x, code):
        decl = x[len(kw):]
        name = decl[:decl.find('(')].strip()
        dd = ParseDeclaration(name)
        name, vartype = dd.parms['name'], dd.parms['vartype']
        flags = [w.strip() for w in (lambda z: decl[z+1:FindMatching(z,decl)].strip())(z=decl.find('(')).split(',')]
        return (name, vartype, flags, cp.Parse(code))
    def ParseTest(x): return x
    def ParseDefine(x):
        decl = x[len('define'):]
        name, args, fullname, code = '', None, decl[:decl.find('=')].strip(), decl[decl.find('=')+1:].strip()
        if '(' in fullname:
           name = fullname[:fullname.find('(')].strip()
           args = [w.strip() for w in (lambda z: fullname[z+1:FindMatching(z,fullname)].strip())(z=fullname.find('(')).split(',')]
        else: name = fullname
        return (name, args, Expression(code))
    for item in stack[0][1]:
        zwords = item[0].split()
        z = zwords[0]
        if z == 'plugin':
           cl = item[0].find(':')
           metadata['plugin'] = (item[0][6:cl].strip(), item[0][(cl+1 if cl != -1 else len(item[0])-1):].strip())
        elif z == 'version': metadata['version'] = zwords[1]
        elif z == 'author': metadata['author'] = ' '.join(zwords[1:])
        elif z == 'example': metadata['example'] = '\n'.join(item[1])
        elif z == 'define': AddAnother(ParseDefine(item[0]), z)
        elif z in ('parameter', 'global'): AddAnother(ParseVariable(z, item[0]), z)
        elif z == 'require': AddAnother((zwords[1], zwords[2]), z)
        elif z == 'test': AddAnother(ParseTest(item), z)
        elif z in ('slot', 'macro', 'function'): AddAnother(ParseDec(z, item[0], item[1]), z)
    return metadata

def ExpandMacro(macro, args):
    decl_args, body = macro[1], deepcopy(macro[2])
    if len(decl_args) != len(args): raise SyntaxError('Not enough arguments for expanding macro \'%s\'' % macro[0])
    def replace_args(node):
        for i, arg in enumerate(decl_args):
            if hasattr(node, 'parms'):
               for key in node.parms:
                   subnode = node.parms[key]
                   if hasattr(subnode, 'type') and subnode.type == 'variable' and subnode.parms['name'] == arg:
                      node.parms[key] = args[i]
    Visit(body, replace_args) 
    return MakeBlock('EMPTY', body)

def ExpandDefine(d, args):
    decl_args, body = d[1], deepcopy(d[2])
    cf = CFormatter()
    if len(decl_args) != len(args): raise SyntaxError('Not enough arguments for expanding define \'%s\'' % d[0])
    to_replace = list()
    def replace_args(node):
        for i, arg in enumerate(decl_args):
            if hasattr(node, 'parms'):
               for key in node.parms:
                   subnode = node.parms[key]
                   if hasattr(subnode, 'type') and subnode.type == 'variable' and subnode.parms['name'] == arg:
                      to_replace.append((node, key, args[i]))
    Visit(body, replace_args)
    for (node, key, val) in to_replace: node.parms[key] = val
    return body

def ExpandAllocate(args):
    if len(args) == 2:
       vtype, length = args[0].parms['name'], CFormatter().FormatNode(args[1])
       return Cast(vartype='%s *' % vtype, value=FunctionCall(name='malloc', args=[Expression('(%s)*(sizeof(%s))' % (length, vtype))]))
    elif len(args) == 3:
       vtype, length = args[0].parms['name'], CFormatter().FormatNode(args[1])
       return Cast(vartype='%s *' % vtype, value=FunctionCall(name='realloc', args=[args[2], Expression('(%s)*(sizeof(%s))' % (length, vtype))]))
    else: raise SyntaxError('Allocate needs two or three arguments')

def InternalMacro(name):
    if name == 'size':
       return Statement('const long size = ASet_LocalSize(aset)')
    elif name == 'extsize':
       return Statement('const long extsize = ASet_ExtendedLocalSize(aset)')
    elif name == 'totalsize':
       return Statement('const long totalsize = ASet_TotalSize(aset)')
    elif name == 'nodes':
       return Statement('const int nodes = ASet_NumberOfRanks(aset)')
    elif name == 'masternode':
       return Statement('const int masternode = (ASet_Rank(aset) == 0)')
    elif name == 'neighborlist': 
       return Statement('ASet_GetNeighborList(aset, &(neighborlist.pair), &(neighborlist.size))')
    raise SyntaxError('Unknown internal macro, \'%s\'' % name)

def KindOfVector(expr, dtypes):
    if not hasattr(expr, 'type'): return None
    if expr.type in ('variable', 'call') and expr.parms['name'] in dtypes and dtypes[expr.parms['name']] == 'Vector':
       return 'Vector'
    if expr.type == 'call' and expr.parms['name'] == 'Vector': return 'VectorConstructor'
    if expr.type == 'arrayaccess' and hasattr(expr.parms['array'], 'type') and expr.parms['array'].type == 'variable' and expr.parms['array'].parms['name'] in dtypes and dtypes[expr.parms['array'].parms['name']] == 'Vector *':
       return 'Vector *'
    if expr.type == 'arrayaccess' and hasattr(expr.parms['array'], 'type') and expr.parms['array'].type == 'variable' and expr.parms['array'].parms['name'] in dtypes and dtypes[expr.parms['array'].parms['name']] == 'Matrix':
       return 'MatrixColumn'
    return None

def ExpandVectorLoop(vloop, dtypes):
    body, vector_expr = deepcopy([z for z in vloop]), list()
    # Modify statements inside the vector loop
    def find_vectors(node):
        if hasattr(node, 'parms'):
           for key in node.parms:
               kv = KindOfVector(node.parms[key], dtypes)
               if kv is not None: vector_expr.append((node, key, kv))
    Visit(body, find_vectors)
    vector_expr = unique_list((z for z in vector_expr), key=lambda z: id(z[0].parms[z[1]]))
    for node, key, kv in vector_expr:
        oldnk = deepcopy(node.parms[key])
        node.parms[key] = Node('Expression')
        if kv == 'Vector':
           node.parms[key] = ArrayAccess(array=oldnk, index=Expression('qq'))
        elif kv == 'Vector *':
           node.parms[key].type, node.parms[key].parms['array'] = 'arrayaccess', oldnk.parms['array']
           node.parms[key].parms['index'] = Binary(operator='+', a=Binary(operator='*', a=Expression('3'), b=oldnk.parms['index']), b=Expression('qq'))
        elif kv == 'VectorConstructor':
           node.parms[key] = FunctionCall(name='VectorComponent', args=oldnk.parms['args']+[Expression('qq')])
        elif kv == 'MatrixColumn':
           node.parms[key] = ArrayAccess(array=oldnk, index=Expression('qq'))
    # Returns the modified block 
    return MakeBlock('for (int qq=0;qq<3;++qq)', body)

def ExpandVectorArgument(arg, dtypes):
    if KindOfVector(arg, dtypes) == 'Vector': return arg
    if KindOfVector(arg, dtypes) == 'Vector *': return UnaryPrefix(operator='&', a=ArrayAccess(array=arg.parms['array'], index=Binary(operator='*', a=NumericLiteral(value='3'), b=arg.parms['index'])))
    return arg

def ExpandVectorReturnFunction(nreg, name, args):
    new_args = [ Expression('v_array'), Expression(str(nreg)) ] + args
    return FunctionCall(name=name, args=new_args)

def ParseValue(x):
    if Surrounded(x, '<', '>'): return 'Vector(%s)' % (x[1:-1].strip())
    else: return x

def RenderPlugin(metadata):
    tree, cf = list(), CFormatter()
    s = Serialize(metadata)
    tree.append(DefineMacro(key='META', value='\"%s\"' % s.replace('\n', '\\n')))
    # Plugin_Meta
    tree.append(Function(rettype=Declaration(vartype='char *', flags = ['const'], name='Plugin_Meta'), args=list(), seq=[Return(value=Expression('META'))]))
    #
    for header in plugin_includes: tree.append(IncludeMacro(header=header))
    pd = [p for p in metadata['parameter']] if 'parameter' in metadata else list()
    inter_name, parameter_decls = dict(), list()
    for x in pd:
        inter_name[x[0][0]] = x[2]['as'] if 'as' in x[2] else x[0][0]
        parameter_decls.extend([(x[1], z, ParseValue(x[2]['default']) if 'default' in x[2] else None) for z in x[0]])
    actual_param_decls = deepcopy(parameter_decls)
    gd = [p for p in metadata['global']] if 'global' in metadata else list()
    for x in gd: parameter_decls.extend([(x[1], z, ParseValue(x[2]['default']) if 'default' in x[2] else None) for z in x[0]])
    internalname = lambda x: inter_name[x] if x in inter_name else x
    for vtype, name, defval in parameter_decls: 
        tree.append(Declaration(vartype=realvtype[vtype], name=internalname(name), flags=['static']))
    # 
    tree.append(Statement('static double v_array[%d]' % (MAXVECTORS*3)))
    #
    plugname = metadata['plugin'][0]
    # Plugin_SetKeyword
    setkw_code, init_code = list(), [ Statement('v_array[0] = 0.0e0') ]
    nametransf = lambda x: x.replace(' ', '').title()
    for param in actual_param_decls:
        parse_kw = Statement('Parse%sKeyword(value, &(%s))' % (nametransf(param[0]), internalname(param[1])))
        if param[2] is not None: init_code.append(Statement('Parse%sKeyword(\"%s\", &(%s))' % (nametransf(param[0]), str(param[2]), internalname(param[1]))))
        setkw_code.append(MakeBlock('if (!strcmp(name, \"%s\"))' % param[1], [parse_kw]))
    setkw_code.append(Return(value=Expression('0')))
    tree.append(Function(rettype=Declaration(vartype='void', name='Plugin_Init', flags=list()), args=list(), seq=init_code))
    tree.append(Function(rettype=Declaration(vartype='int', name='Plugin_SetKeyword', flags=()), args=[ Declaration(vartype='char *', flags=['const'], name='name'), Declaration(vartype='char *', name='value', flags = ['const'])], seq=setkw_code))
    #
    macro_names = [macro[0] for macro in metadata['macro']] if 'macro' in metadata else list()
    define_names = [define[0] for define in metadata['define']] if 'define' in metadata else list()
    slot_names = [slot[0] for slot in metadata['slot']]
    if 'function' in metadata:
       for slot in metadata['function']:
           args = [ ParseDeclaration(z) for z in slot[2] ]
           tree.append(Function(rettype=Declaration(vartype=slot[1], name=slot[0], flags=list()), args=args, seq=slot[3]))
    if 'slot' in metadata:
       for ns, slot in enumerate(metadata['slot']):
           varlist = list()
           def find_variables(node):
               if hasattr(node, 'type') and node.type == 'variable': varlist.append(node.parms['name'])
           Visit(slot[3], find_variables)
           if 'extsize' in varlist: slot[3].insert(0, InternalMacro('extsize'))
           if 'size' in varlist: slot[3].insert(0, InternalMacro('size'))
           if 'totalsize' in varlist: slot[3].insert(0, InternalMacro('totalsize'))
           if 'nodes' in varlist: slot[3].insert(0, InternalMacro('nodes'))
           if 'masternode' in varlist: slot[3].insert(0, InternalMacro('masternode'))
           if 'neighborlist' in varlist:
              slot[3].insert(0, InternalMacro('neighborlist'))
              slot[3].insert(0, Statement('NeighborList neighborlist'))
           args = list([Declaration(vartype=v, name=n, flags=(['const'] if 'reader' in slot[2] else list())) for (v, n) in (('AtomSet *', 'aset'), ('Cell *', 'cell'))])
           tree.append(Function(rettype=Declaration(vartype='void', name=slot[0], flags=list()), args=args, seq=slot[3]))
    def find_macros(node):
        if hasattr(node, 'parms'):
           for key in node.parms:
               subnode = node.parms[key]
               if hasattr(subnode, 'nodetype') and subnode.nodetype == 'Expression':
                  if subnode.type == 'call' and subnode.parms['name'] in define_names:
                     d = [z for z in metadata['define'] if z[0] == subnode.parms['name']][0]
                     assert isinstance(d[1], list)
                     node.parms[key] = ExpandDefine(d, subnode.parms['args'])
                  if subnode.type == 'variable' and subnode.parms['name'] in define_names:
                     d = [z for z in metadata['define'] if z[0] == subnode.parms['name']][0]
                     assert d[1] is None
                     node.parms[key] = d[2]
        if isinstance(node, list):
           for i, subnode in enumerate(node):
               if hasattr(subnode, 'nodetype') and subnode.nodetype == 'Statement' and subnode.type == 'call' and subnode.parms['name'] in macro_names:
                  m = [z for z in metadata['macro'] if z[0] == subnode.parms['name']][0]
                  m = tuple(z for i, z in enumerate(m) if i != 1) 
                  expm = ExpandMacro(m, subnode.parms['args'])
                  assert isinstance(expm, Block) and expm.type == 'EMPTY'
                  node[i] = expm
               if hasattr(subnode, 'nodetype') and subnode.nodetype == 'Expression':
                  if subnode.type == 'call' and subnode.parms['name'] in define_names:
                     d = [z for z in metadata['define'] if z[0] == subnode.parms['name']][0]
                     assert isinstance(d[1], list)
                     node[i] = ExpandDefine(d, subnode.parms['args'])
                  if subnode.type == 'variable' and subnode.parms['name'] in define_names:
                     d = [z for z in metadata['define'] if z[0] == subnode.parms['name']][0]
                     assert d[1] is None
                     node[i] = d[2]
    Visit(tree, find_macros)
    orig_dtypes = dict()
    if 'function' in metadata:
       for function in metadata['function']: orig_dtypes[function[0]] = function[1]
    for subtree in tree:
        decls = deepcopy(parameter_decls)
        def collect_decl(node):
            if hasattr(node, 'type') and node.type == 'declaration': 
               decls.append((node.parms['vartype'], node.parms['name']))
        Visit(subtree, collect_decl)
        decl_dict = dict((z[1], z[0]) for z in decls)
        vloops, dtypes = list(), deepcopy(orig_dtypes)
        # Expand VectorLoops
        def find_vectorloops(node):
            if isinstance(node, list):
               for i, subnode in enumerate(node):
                   if isinstance(subnode, Block) and subnode.type == 'VectorLoop': vloops.append((node, i))
        Visit(subtree, find_vectorloops)
        decls = [(z[0], z[1]) for z in decls]
        for v, n in decls:
            if 'Vector' in v or 'Matrix' in v: dtypes[n] = v
        for (node, i) in vloops: node[i] = ExpandVectorLoop(node[i], dtypes)
        # Expand Allocate and others
        nreg = 0
        def find_internalmacros(node):
            nonlocal nreg
            functions_with_vector_args = ('Distance', 'SquareModule', 'Dot', 'Cross', 'VectorFormat', 'Fractional', 'Cartesian', 'CenterOfMass')
            if hasattr(node, 'parms'):
               for key in node.parms:
                   subnode = node.parms[key]
                   if hasattr(subnode, 'type') and subnode.type == 'call':
                      if subnode.parms['name'] == 'Allocate': node.parms[key] = ExpandAllocate(subnode.parms['args'])
                      if subnode.parms['name'] in ('Vector', 'Cross'):
                         if nreg == MAXVECTORS: raise Exception('Too many Vector() calls exceed the limit per plugin')
                         node.parms[key] = ExpandVectorReturnFunction(nreg, subnode.parms['name'], subnode.parms['args'])
                         nreg += 1
                      if subnode.parms['name'] in functions_with_vector_args:
                         node.parms[key].parms['args'] = [ExpandVectorArgument(z, dtypes) for z in node.parms[key].parms['args']]
                      if subnode.parms['name'] == 'Matrix': node.parms[key].parms['name'] = 'MatrixFromHeap'
                      if subnode.parms['name'] in slot_names:
                         node.parms[key].parms['args'] = [Expression(z) for z in ('aset', 'cell')]+node.parms[key].parms['args']
            if isinstance(node, list):
               for i, subnode in enumerate(node):
                   if hasattr(subnode, 'type') and subnode.type == 'call':
                      if subnode.parms['name'] in functions_with_vector_args:
                         node[i].parms['args'] = [ExpandVectorArgument(z, dtypes) for z in node[i].parms['args']]
                      if subnode.parms['name'] in ('Vector', 'Cross'):
                         if nreg == MAXVECTORS: raise Exception('Too many Vector() calls exceed the limit per plugin')
                         node[i] = ExpandVectorReturnFunction(nreg, subnode.parms['name'], subnode.parms['args'])
                         nreg += 1
                      if subnode.parms['name'] == 'Matrix': node[i].parms['name'] = 'MatrixFromHeap'
                      if subnode.parms['name'] in slot_names:
                         node[i].parms['args'] = [Expression(z) for z in ('aset', 'cell')]+node[i].parms['args']
        Visit(subtree, find_internalmacros)
        # Expand return statements
        if not ('declaration' in subtree.parms and subtree.parms['declaration'].parms['vartype'] == 'void'): continue
        ret_stat = list()
        def find_returns(node):
            if isinstance(node, list):
               for i, x in enumerate(node):
                   if hasattr(x, 'type') and x.type == 'return': ret_stat.append((node, i))
        Visit(subtree, find_returns)
        for node, i in ret_stat:
            val, tpof = node[i].parms['value'], None
            if val.type == 'call' and val.parms['name'] in ('AtomSelection', 'NeighborList'):
               tpof = val.parms['name']
            elif val.type == 'cast': tpof = val.parms['vartype']
            elif val.type == 'variable': tpof = decl_dict[val.parms['name']] if val.parms['name'] in decl_dict else None
            args = [ Expression('aset') ]
            if tpof in ('float', 'double', 'Vector', 'Matrix'):
               args.append(Expression('RET_VECTOR' if tpof == 'Vector' else ('RET_MATRIX' if tpof == 'Matrix' else 'RET_DOUBLE')))
               args.append(Expression('&(%s)' % CFormatter().FormatNode(val)))
               args.append(Expression('3' if tpof == 'Vector' else '1'))
            elif tpof in ('AtomSelection', 'NeighborList'):
               args.append(Expression('RET_ASELECT' if tpof == 'AtomSelection' else 'RET_NLIST'))
               args.append(val.parms['args'][0])
               args.append(val.parms['args'][1])
            else: raise SyntaxError('Unknown/Invalid return type for slot: \'%s\'' % tpof)
            node[i] = Node('Statement')
            node[i].type, node[i].parms = 'call', {'name': 'ASet_ReturnValue', 'args': args}
    # Replace Vector declarations
    vectordecls, matrices = list(), list()
    def collect_vectordecls(node):
        if hasattr(node, 'parms') and 'vartype' in node.parms:
           if node.parms['vartype'] == 'Vector *': vectordecls.append(('Vector *', node))
           elif node.parms['vartype'] == 'Vector' and 'function' in node.parms: vectordecls.append(('Vector *', node))
           elif node.parms['vartype'] == 'Vector': vectordecls.append(('Vector', node))
           elif node.parms['vartype'] == 'Matrix': matrices.append(node.parms['name'])
    Visit(tree, collect_vectordecls)
    for vtype, node in vectordecls:
        if vtype == 'Vector *': node.parms['vartype'] = 'double *'
        elif vtype == 'Vector': node.parms['vartype'] = 't_innervector'
    # Replace Matrix expressions
    def replace_matrixexpr(node):
        if hasattr(node, 'parms'):
           for key in node.parms:
               subnode = node.parms[key]
               if hasattr(subnode, 'type') and subnode.type == 'arrayaccess' and 'name' in subnode.parms['array'].parms and subnode.parms['array'].parms['name'] in matrices:
                  node.parms[key].parms['array'] = Binary(operator='.', a=subnode.parms['array'], b=Expression('values'))
        if isinstance(node, list):
           for i, subnode in enumerate(node):
               if hasattr(subnode, 'type') and subnode.type == 'arrayaccess' and 'name' in subnode.parms['array'].parms and subnode.parms['array'].parms['name'] in matrices:
                  node[i].parms['array'] = Binary(operator='.', a=subnode.parms['array'], b=Expression('values'))
    Visit(tree, replace_matrixexpr)
    return cf.Format(tree)

