#!/usr/bin/env python3
#
#
#

from lpplugmaker.CParser import Block

class CFormatter:

    def Format(self, tree):
        code, subst = '', dict()
        for x in tree:
            if hasattr(x, 'nodetype'): 
               if x.nodetype == 'Macro' and x.type == 'define':
                  subst[x.parms['key']] = x.parms['value']
               else: code += self.FormatNode(x)
            elif isinstance(x, Block): code += self.FormatBlock(x)
        for key in subst: code = code.replace(key, subst[key])
        return code

    def FormatNode(self, node):
        if node.nodetype == 'Statement':
           if node.type == 'assignment':
              return '%s = %s;\n' % (self.FormatNode(node.parms['lvalue']).strip().strip(';'), self.FormatNode(node.parms['rvalue']))
           elif node.type == 'augmented':
              return '%s %s %s;\n' % (self.FormatNode(node.parms['lvalue']), node.parms['operator'], self.FormatNode(node.parms['rvalue']))
           elif node.type == 'declaration':
              return '%s %s %s;\n' % (' '.join(node.parms['flags']), node.parms['vartype'], node.parms['name'])
           elif node.type == 'return': return 'return (%s);\n' % (self.FormatNode(node.parms['value']))
           elif node.type in ('break', 'continue'): return '%s;\n' % node.type
           elif node.type == 'call': return '%s(%s);\n' % (node.parms['name'], ', '.join(self.FormatNode(z) for z in node.parms['args']))
           elif node.type == 'unary_prefix': return '%s(%s);' % (node.parms['operator'], self.FormatNode(node.parms['a']))
           elif node.type == 'unary_postfix': return '(%s)%s;' % (self.FormatNode(node.parms['a']), node.parms['operator'])
           elif node.type == 'typedef': return 'typedef %s;\n' % (self.FormatNode(node.parms['declaration']).strip().strip(';'))
           else: return '<FUCK Statement: %s>' % str(node)
        elif node.nodetype == 'Expression':
           if node.type == 'numericliteral': return '%s' % node.parms['value']
           elif node.type == 'stringliteral': return '%s' % node.parms['value']
           elif node.type == 'variable': return '%s' % node.parms['name']
           elif node.type == 'call': return '%s(%s)' % (node.parms['name'], ', '.join(self.FormatNode(z) for z in node.parms['args']))
           elif node.type == 'cast': return '(%s)(%s)' % (node.parms['vartype'], self.FormatNode(node.parms['value']))
           elif node.type == 'unary_prefix': return '%s(%s)' % (node.parms['operator'], self.FormatNode(node.parms['a']))
           elif node.type == 'unary_postfix': return '(%s)%s' % (self.FormatNode(node.parms['a']), node.parms['operator'])
           elif node.type == 'binary': return '(%s %s %s)' % (self.FormatNode(node.parms['a']), node.parms['operator'], self.FormatNode(node.parms['b']))
           elif node.type == 'ternary':
              return '( (%s) ? (%s) : (%s) )' % tuple(self.FormatNode(node.parms[z]) for z in ('condition', 'iftrue', 'iffalse'))
           elif node.type == 'arrayaccess': return '%s[%s]' % (self.FormatNode(node.parms['array']), self.FormatNode(node.parms['index']))
           elif node.type == 'null': return ''
           else: return '<FUCK Expression: %s>' % str(node)
        elif node.nodetype == 'Macro':
           if node.type == 'include': return '#include %s\n' % (node.parms['header'])
           elif node.type == 'define': return '#define %s %s\n' % (node.parms['key'], node.parms['value'])
           else: return '<FUCK Macro: %s>' % str(node)
        else: return '<FUCK Unknown: %s>' % str(node)

    def FormatBlock(self, block):
        if block.type == 'function':
           code = '%s(%s)\n{\n' % (self.FormatNode(block.parms['declaration']).strip().strip(';'), ', '.join(self.FormatNode(z).strip().strip(';') for z in block.parms['args']))
           code += (self.Format(block)+'}\n')
           return code
        elif block.type == 'if':
           code = 'if (%s)\n{\n' % self.FormatNode(block.parms['condition'])
           code += (self.Format(block)+'}\n')
           return code
        elif block.type == 'for':
           code = 'for (%s;%s;%s)\n{\n' % tuple(self.FormatNode(block.parms[z]).strip().strip(';') for z in ('initializer', 'stop', 'step'))
           code += (self.Format(block)+'}\n')
           return code
        elif block.type == 'while':
           code = 'while (%s)\n{\n' % self.FormatNode(block.parms['condition'])
           code += (self.Format(block)+'}\n')
           return code
        elif block.type == 'multipledecl': return self.Format(block)
        elif block.type == 'else': return 'else\n{\n'+self.Format(block)+'}\n'
        elif block.type == 'EMPTY': return '{\n'+self.Format(block)+'}\n'
        return ('<FUCK Block: %s>\n{\n' % block.type)+self.Format(block)+'}\n'

