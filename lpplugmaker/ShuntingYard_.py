#!/usr/bin/env python3
#
# Implementation of Dijkstra's Shunting-yard algorithm
#

import string

literal_chars = '_'+string.digits+string.ascii_letters
ValidLiteral = lambda x: x.strip() != '' and all(z in literal_chars for z in x.strip())

operators = ('++', '--', '<<', '>>', '<=', '>=', '==', '!=', '||', '&&', 
             '+=', '-=', '*=', '/=', '%=', '|=', '&=', '^=', '->', '.', 
             '!', '~', '+', '-', '*', '%', '/', '&', '|', '^', '?', ':', 
             '=', '(', ')', '[', ']', '<', '>', ',', '#')

unary_prefix = ('~', '!')

precedence = { 2: ('++', '--', '#', '.', '->'),
               3: ('u++', 'u--', 'u+', 'u-', '!', '~', ':=', 'u*', 'u&'),
               5: ('*', '/', '%'), 6: ('+', '-'), 7: ('<<', '>>'), 
               8: ('<', '<=', '>', '>='), 9: ('==', '!='), 10: ('&',),
               11: ('^',), 12: ('|',), 13: ('&&',), 14: ('||',), 15: ('?:'),
               16: ('=', '+=', '-=', '*=', '/=', '%=', '|=', '&=', '^='), 18: (',',) }

left_to_right = ('++', '--', '#', '.', '->', '*', '/', '%', '+', '-', '<<', '>>',
                           '<', '>', '<=', '>=', '==', '!=', '&', '^', '|', '&&', '||', ',' )

leftassoc = lambda x: x in left_to_right
rightassoc = lambda x: not leftassoc(x)

def first(gen):
    try: return (list(gen))[0]
    except IndexError: return None

precedenceof = lambda x: first(20-z for z in precedence if x in precedence[z])

def numeric(x):
    try:
       y = float(x)
       return True
    except:
       x = x.replace('E', 'e')
       if x.endswith('e') and numeric(x[:-1]): return True
       if x.endswith('e+') or x.endswith('e-') and numeric(x[:-2]): return True
       return False

def Tokenize(expr):
    i, tokens = 0, list()
    iswhite = lambda c: c in string.whitespace
    def advance(expr, i, f):
        i0 = i
        while i < len(expr) and f(expr, i0, i): i += 1
        return (expr[i0:i].strip(), i)
    while i < len(expr):
          matching_operator = lambda x: first(z for z in operators if x.startswith(z))
          token, op = '', matching_operator(expr[i:])
          if op is not None: token, i = op, i+len(op)
          elif expr[i] == '\"':
               token, i = advance(expr, i+1, lambda expr, i0, i: expr[i] != '\"')
               token, i = ('\"%s\"' % token), i+1
          elif iswhite(expr[i]): empty, i = advance(expr, i, lambda expr, i0, i: iswhite(expr[i]))
          elif expr[i] in string.digits: token, i = advance(expr, i, lambda expr, i0, i: numeric(expr[i0:i+1]))
          else: token, i = advance(expr, i, lambda expr, i0, i: matching_operator(expr[i:]) is None)
          if token != '': tokens.append(token)
    # Replace the closing square bracket with the closing parenthesis 
    allindices = lambda op, seq: [i for i, z in enumerate(seq) if z == op]
    tokens = [(')' if z == ']' else z) for z in tokens]
    # Find and replace unary versions of +, -, *, &
    for op in ('++', '--', '+', '-', '*', '&'):
        for i in allindices(op, tokens):
            if i == 0 or (tokens[i-1] != ')' and tokens[i-1] in operators): tokens[i] = 'u'+op
    modified = True
    while modified:
          modified = False
          # Replace ')(' by the cast binary operator
          for i in allindices(')', tokens):
              if i < len(tokens)-1 and tokens[i+1] == '(':
                 tokens.insert(i+1, ':=')
                 modified = True
                 break
          # Replace the square brackets with the "array" binary operator 
          for k in allindices('[', tokens):
              tokens[k] = '('
              tokens.insert(k, '#')
              modified = True
              break
    return tokens

def ToRPN(expr):
    tokens, functions = Tokenize(expr), list()
    for i, t in enumerate(tokens):
        if ValidLiteral(t) and i < len(tokens)-1 and tokens[i+1] == '(': functions.append(t)
    stack, output, real_operators, = list(), list(), list()
    for lst in precedence.values(): real_operators.extend(lst)
    kind_of_operator = lambda x: ('UNARY_PREFIX' if x.startswith('u') or x in unary_prefix else \
                                 ('UNARY_POSTFIX' if x in ('++', '--') else 'BINARY'))
    fix_u = lambda x: x.lstrip('u')
    while len(tokens) > 0:
          token = tokens.pop(0)
          if token in functions:
             output.append(('DELIM', '...'))
             stack.append(token)
          elif token.startswith('\"') or numeric(token) or all((z not in token) for z in operators): 
             output.append(('LITERAL', token))
          elif token == '(': stack.append(token)
          elif token == ')':
             while len(stack) > 0 and stack[-1] != '(': 
                   output.append((lambda op: (kind_of_operator(op), fix_u(op)))(op=stack.pop()))
             leftparen = stack.pop()
             if len(stack) > 0 and stack[-1] in functions: output.append(('FUNCTION', stack.pop()))
          elif token == ',':
             while stack[-1] != '(': output.append((lambda op: (kind_of_operator(op), fix_u(op)))(op=stack.pop()))
          elif token in real_operators:
             while len(stack) > 0 and stack[-1] in real_operators and \
                       ((leftassoc(token) and precedenceof(token) <= precedenceof(stack[-1])) or \
                         (rightassoc(token) and precedenceof(token) < precedenceof(stack[-1]))):
                   output.append((lambda op: (kind_of_operator(op), fix_u(op)))(op=stack.pop()))
             stack.append(token)
    while len(stack) > 0:
          if stack[-1] in ('(', ')'): raise SyntaxError('Mismatched parentheses')
          output.append((lambda op: (kind_of_operator(op), fix_u(op)))(op=stack.pop()))
    output = [z for z in output if z[1] != ':']
    output = [(('TERNARY', z[1]) if z[1] == '?' else z) for z in output]
    return output

