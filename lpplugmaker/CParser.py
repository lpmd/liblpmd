#!/usr/bin/env python3
#
#
#

import sys, string
from lpplugmaker.ShuntingYard import literal_chars, ValidLiteral, ToRPN

block_keywords, separators = ('if', 'for', 'while', 'do'), '( *='
augmented = ('+=', '-=', '*=', '/=', '&=', '|=', '^=', '%=')
decl_flags = ('auto', 'const', 'extern', 'inline', 'register', 'static', 'restrict', 'volatile' )
alone_statements = ('return', 'break', 'continue')

Surrounded = lambda x, op, cl: x.startswith(op) and x.endswith(cl)

def ValidNumeral(x):
    try: return (float(x) != None)
    except: return False

DeclOrVar = lambda x: Expression(x) if ValidLiteral(x) else Statement(x)

def FreeOperator(op, line):
    pos, lst = 0, list()
    while True:
          p = line.find(op, pos)
          if p == -1: break
          if not Enclosed(p, line): lst.append((op, p))
          pos = p+len(op)
    return lst

def Enclosed(pos, x):
    ch = x[pos]
    for par in ('(', '['):
        while par in x:
              p1 = x.find(par)
              p2 = FindMatching(p1, x)
              x = x.replace(x[p1:p2+1], ' '*len(x[p1:p2+1]))
    return x[pos] != ch

class Node(dict):
    def __init__(self, nodetype, **kwargs):
        self.nodetype, self.type = nodetype, 'unknown'
        self.parms = dict(kwargs)
    def __repr__(self):
        return '%s(type=%s, %s)' % (self.nodetype, self.type, ', '.join('%s=%s' % (z, repr(self.parms[z])) for z in self.parms))

def Macro(value):
    obj = Node('Macro', value=value)
    if value.startswith('include'): return IncludeMacro(header=value[7:].strip())
    elif value.startswith('define'):
       ip = FindIf(value, lambda c: c not in literal_chars)
       s = FindIf(value[ip+1:], lambda c: c in separators)+ip+1
       return DefineMacro(key=value[6:s].strip(), value=value[s+1:].strip())

def NodeFactory(nodetype, subtype, params):
    def factory(**kwargs):
        obj = Node(nodetype)
        obj.type, obj.parms = subtype, dict((k,kwargs[k]) for k in params)
        return obj
    return factory

ExpressionFactory = lambda subtype, params: NodeFactory('Expression', subtype, params)
StatementFactory = lambda subtype, params: NodeFactory('Statement', subtype, params)

IncludeMacro = NodeFactory('Macro', 'include', ('header',))
DefineMacro = NodeFactory('Macro', 'define', ('key', 'value'))

Return = StatementFactory('return', ('value',))
TypeDef = StatementFactory('typedef', ('declaration',))
Declaration = StatementFactory('declaration', ('vartype', 'name', 'flags'))
FunctionCallStatement = StatementFactory('call', ('name', 'args'))
Assignment = StatementFactory('assignment', ('lvalue', 'rvalue'))
AugmentedAssignment = StatementFactory('augmented', ('operator', 'lvalue', 'rvalue'))
UnaryPrefixStatement = StatementFactory('unary_prefix', ('operator', 'a'))
UnaryPostfixStatement = StatementFactory('unary_postfix', ('operator', 'a'))
Continue = StatementFactory('continue', ())
Break = StatementFactory('break', ())

NullExpression = ExpressionFactory('null', ())
NumericLiteral = ExpressionFactory('numericliteral', ('value',))
StringLiteral = ExpressionFactory('stringliteral', ('value',))
Variable = ExpressionFactory('variable', ('name',))
Cast = ExpressionFactory('cast', ('vartype', 'value'))
FunctionCall = ExpressionFactory('call', ('name', 'args'))
UnaryPrefix = ExpressionFactory('unary_prefix', ('operator', 'a'))
UnaryPostfix = ExpressionFactory('unary_postfix', ('operator', 'a'))
Binary = ExpressionFactory('binary', ('operator', 'a', 'b'))
Ternary = ExpressionFactory('ternary', ('condition', 'iftrue', 'iffalse'))
ArrayAccess = ExpressionFactory('arrayaccess', ('array', 'index'))

def Expression(value):
    rpn, stack = ToRPN(value), list()
    if len(rpn) == 0: return NullExpression()
    while len(rpn) > 0:
          tt, token = rpn.pop(0)
          if token == '...': stack.append(token)
          elif tt == 'LITERAL':
             if ValidNumeral(token): stack.append(NumericLiteral(value=token))
             elif ValidLiteral(token): stack.append(Variable(name=token))
             elif token.startswith('\"'): stack.append(StringLiteral(value=token))
          elif tt == 'FUNCTION':
             args = list()
             while stack[-1] != '...': args.insert(0, stack.pop())
             assert (stack.pop() == '...')
             stack.append(FunctionCall(name=token, args=args))
          elif tt == 'UNARY_PREFIX':
             a = stack.pop()
             stack.append(UnaryPrefix(operator=token, a=a))
          elif tt == 'UNARY_POSTFIX':
             a = stack.pop()
             stack.append(UnaryPostfix(operator=token, a=a))
          elif tt == 'BINARY':
             b = stack.pop()
             a = stack.pop()
             if token == '#': stack.append(ArrayAccess(array=a, index=b))
             elif token == ':=': stack.append(Cast(vartype=a.parms['name'], value=b))
             else: stack.append(Binary(operator=token, a=a, b=b))
          elif tt == 'TERNARY':
             c = stack.pop()
             b = stack.pop()
             a = stack.pop()
             stack.append(Ternary(condition=a, iftrue=b, iffalse=c))
    return stack.pop()

def Statement(value):
    value = value.strip()
    assert (value != '')
    s = FindIf(value, lambda c: c in '(=')
    if value[s] == '(' and ValidLiteral(value[:s]): return FunctionCallStatement(**(Expression(value).parms))
    elif any(value.startswith(z) for z in ('++', '--')): return UnaryPrefixStatement(operator=value[0:2], a=Expression(value[2:]))
    elif any(value.endswith(z) for z in ('++', '--')): return UnaryPostfixStatement(operator=value[-2:], a=Expression(value[:-2]))
    elif value.startswith('else '): return MakeBlock('else', [OneLiner(value[4:].strip())])
    elif any((value.startswith(z) for z in alone_statements)):
       if value.startswith('return'): return Return(value=Expression(value[6:].strip()))
       elif value.startswith('continue'): return Continue()
       elif value.startswith('break'): return Break()
       elif value.startswith('typedef'): return TypeDef(declaration=ParseDeclaration(value[7:].strip()))
       else: raise Exception('What the fuck is this? \"%s\"' % value)
    elif ',' in value and not Enclosed(value.find(','), value):  # Multiple declaration
       subdecls, decltype = [z.strip() for z in value.split(',')], ''
       initvals = [(z[z.find('=')+1:].strip() if '=' in z else None) for z in subdecls]
       for z in range(len(subdecls)): subdecls[z] = (subdecls[z][:subdecls[z].find('=')] if '=' in subdecls[z] else subdecls[z])
       subdecls[0] = subdecls[0].strip()
       pos = len(subdecls[0])-1
       while subdecls[0][pos] in literal_chars+'[]': pos -= 1
       while subdecls[0][pos] in '* ': pos -= 1
       basetype = subdecls[0][:pos+1]
       putinit = lambda w: (' = '+w if w is not None else '')
       subdecls[0] = Statement(subdecls[0]+putinit(initvals[0]))
       for z in range(1,len(subdecls)): subdecls[z] = Statement(basetype+' '+subdecls[z]+putinit(initvals[z]))
       return MakeBlock('multipledecl', subdecls)
    elif any(z in value for z in augmented): # Augmented Assignment
       aug = [z for z in augmented if z in value][0]
       s = value.find(aug)
       return AugmentedAssignment(operator=aug, lvalue=Expression(value[:s].strip()), rvalue=Expression(value[s+len(aug):].strip()))
    elif '=' in value and len(FreeOperator('=', value)) > 0: # Simple assignment
       s = list(reversed(FreeOperator('=', value)))[0][1]
       lvalue, rvalue = value[:s].strip(), Expression(value[s+1:].strip())
       if '=' in lvalue and len(FreeOperator('=', value)) > 0: lvalue = Statement(lvalue)
       elif all((z in literal_chars+' *') for z in lvalue): lvalue = DeclOrVar(lvalue)
       elif all(z in lvalue for z in ('[', ']')) and not ValidLiteral(lvalue[:lvalue.find('[')]) and not any(z in lvalue[:lvalue.find('[')] for z in ('(', ')', '.')):
            s = FindIf(lvalue, lambda c: c in ' *', backwards=True)
            lvalue = Declaration(vartype=lvalue[:s-1], name=lvalue[s:], flags=list())
       else: lvalue = Expression(lvalue)
       return Assignment(lvalue=lvalue, rvalue=rvalue)
    elif any((z in value and not Enclosed(value.find(z),value)) for z in (' ', '*')): return ParseDeclaration(value)
    else: raise Exception('What the fuck is this? \"%s\"' % value)
 
def ParseDeclaration(value):
    name, vartype, flags = '', '', list()
    for z in decl_flags:
        if z in value:
           value = value.replace(z, '').strip()
           flags.append(z)
    s = FindIf(value, lambda c: c in separators, backwards=True)
    if s != -1: name, vartype = value[s:].strip(), value[:s].strip()
    else: name = value
    return Declaration(vartype=vartype, name=name, flags=flags)

def FindMatching(pos, line, backwards=False, absolute=True):
    closing = { '(': ')', '{': '}', '[': ']' }
    invclosing = { ')': '(', '}': '{', ']': '[' }
    t1, t2, balance = line[pos], (invclosing if backwards else closing)[line[pos]], 0
    for i, c in enumerate(reversed(line) if backwards else line):
        if ((i < pos) if not backwards else (i > pos)): continue
        if c == t1: balance += 1
        if c == t2: balance -= 1
        if balance == 0 and (((i < pos) if backwards else (i > pos)) if absolute else True):
           return ((len(line)-1-i if backwards else i)+(0 if absolute else pos))
    raise Exception('Unmatched parentheses, %s' % line)

def FindIf(line, cond, backwards=False):
    for i, c in enumerate(reversed(line) if backwards else line):
        if cond(c): return (len(line)-i if backwards else i)
    return -1

class Block(list):
    def __repr__(self):
        args = ', '.join('%s=%s' % (z, repr(self.parms[z])) for z in self.parms)
        s = 'Block(type=%s, %s)' % (self.type, args)
        return s

def MakeBlock(header, seq=None):
    obj = Block()
    obj.parms, obj.type = dict(), header.strip()
    if seq != None: obj.extend(seq)
    p1 = header.find('(')
    if p1 != -1:
       arg, obj.type = header[p1+1:FindMatching(p1, header)], header[:p1].strip()
       if obj.type not in block_keywords:
          pp = FindIf(obj.type, lambda c: c in separators, backwards=True)
          rettype, fname, flags = 'void', '', list()
          if pp == -1: rettype, fname = '', header[:p1].strip()
          else: rettype, fname = header[:pp].strip(), header[pp:p1].strip()
          for z in decl_flags:
              if z in rettype:
                rettype = rettype.replace(z, '').strip()
                flags.append(z)
          rt = DeclOrVar(rettype+' '+fname)
          rt.parms['flags'] = flags
          args = list([ DeclOrVar(z.strip()) for z in arg.split(',')] if arg.strip() != '' else list())
          name = rt.parms['name']
          return Function(rt, args, seq)
       elif obj.type == 'if' or obj.type == 'while': obj.parms['condition'] = Expression(arg)
       elif obj.type == 'for':
            a, b, c = arg.split(';')
            obj.parms['initializer'], obj.parms['stop'], obj.parms['step'] = Statement(a), Expression(b), Statement(c)
    return obj

def Function(rettype, args=list(), seq=None):
    obj = Block()
    if seq != None: obj.extend(seq)
    rettype.parms['function'] = 'True'
    obj.parms, obj.type = { 'args': args, 'declaration': rettype }, 'function'
    return obj

def OneLiner(line):
    for kw in block_keywords:
        if line[0:len(kw)] == kw and line[len(kw)] in separators:
           p1 = line.find('(')
           p2 = FindMatching(p1, line)
           arg = line[p1:p2+1] 
           head, body = (z.strip() for i, z in enumerate(line.partition(arg)) if i != 1)
           content = OneLiner(body)
           blk = MakeBlock(head+' '+arg, [ content ])
           blk.type = kw
           return blk
    return Statement(line)

class CParser:

    def Parse(self, code):
        changed, self.codetree, cnt, self.arraydict = True, list(), 0, dict()
        while changed:
              pos, changed = 0, False
              while True:
                    npos = code.find('{', pos)
                    if npos == -1: break
                    if code[:npos].strip().endswith('='): # Array initialization
                       m, changed = FindMatching(npos, code, absolute=True), True
                       self.arraydict['ARRAY_INIT%d' % cnt] = code[npos:m+1]
                       code = code.replace(code[npos:m+1], 'ARRAY_INIT%d' % cnt)
                       pos, cnt = m+1, cnt+1
                    else: pos = npos+1
        def RemoveBetween(op, cl, offset):
            nonlocal code
            while op in code:
                  q = code.find(op)
                  newp = code.find(cl, q+len(op))
                  code = code[:q]+code[newp+offset:]
        RemoveBetween('/*', '*/', 2)
        RemoveBetween('//', '\n', 0)
        cnt = 0
        while '\"' in code:
              q = code.find('\"')
              newp = code.find('\"', q+1)
              self.codetree.append(Macro('define QUOTED_STRING%d %s' % (cnt, code[q:newp+1])))
              code = code.replace(code[q:newp+1], 'QUOTED_STRING%d' % cnt)
              cnt += 1
        self.codetree.extend(self.ParseBlock(code))
        return self.codetree

    def ParseBlock(self, code):
        p, objects = 0, list()
        Between = lambda a, b, c: ((b != -1 and c != -1) and ((a > b) and (a < c)))
        IsForStatement = lambda x: (x.startswith('for') and x[3] in (' ', '(') and x[FindIf(x, lambda c: c in ');')] == ';')
        while p < len(code):
              if code[p] in string.whitespace: p += 1
              elif code[p] == '#':
                   newp = code.find('\n', p+1)
                   objects.append(Macro(code[p+1:newp]))
                   p = newp
              else:
                   def finder(s):
                       ikenai = False
                       for i in range(p, len(code)):
                           if code[i] == '(': ikenai = True
                           if code[i] == ')': ikenai = False
                           if not ikenai and code[i] == s: return i
                       return len(code)
                   sc, opbr = finder(';'), finder('{')
                   if sc < opbr: # Found one-liner
                      oneliner = OneLiner(code[p:sc].replace('\n', ' ').strip())
                      objects.append(oneliner)
                      p = sc+1
                   elif opbr < sc: # Found block
                      clbr, header = FindMatching(opbr, code), code[p:opbr].strip()
                      content = code[opbr+1:clbr].strip().replace('\n', ' ')
                      objects.append(MakeBlock(header, self.ParseBlock(content)))
                      p = clbr+1
        return objects

def Visit(tree, callable):
    def Search(x): # closure for recursive search
        callable(x)
        if isinstance(x, list) or isinstance(x, tuple):
           for z in x: Search(z)
        if hasattr(x, 'parms'):
           for key in x.parms: Search(x.parms[key])
    Search(tree)

