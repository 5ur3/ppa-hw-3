

from pysmt.shortcuts import Symbol, And, Or, LT, Not, LT, LE, Equals, NotEquals, Int, get_model, Bool
from pysmt.typing import INT

def is_read(expr):
    return expr[0:2] == "I_" and str(expr[2:]).isnumeric()

def make_symbol(expr):
    return Symbol(expr, INT)

def is_known_id(expr):
    if expr in ctx.keys():
        return True
    return False

def parse(expr):
    try:
        e = int(expr[0])
        return Int(e), expr[1:]
    except:
        pass 

    if is_read(expr[0]):
        return make_symbol(expr[0]), expr[1:]

    if is_known_id(expr[0]):
        return ctx[expr[0]], expr[1:]

    match expr[0]:
        case "PLUS":
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return val1 + val2, rem
        case "MUL":
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return val1 * val2, rem
        case "MINUS":
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return val1 - val2, rem
        case 'TRUE':
            return Bool(True), expr[1:]
        case 'FALSE':
            return Bool(False), expr[1:]
        case 'EQ':
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return Equals(val1, val2), rem
        case 'NE':
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return NotEquals(val1, val2), rem
        case 'LESS':
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return LT(val1, val2), rem
        case 'LEQ':
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return LE(val1, val2), rem
        case 'NOT':
            val1, rem = parse(expr[1:])
            return Not(val1), rem
        case 'AND':
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return And(val1, val2), rem
        case 'OR':
            val1, rem = parse(expr[1:])
            val2, rem = parse(rem)
            return Or(val1, val2), rem
        case _:
            print(expr[0])
            raise "што это" 

ctx = {}

def run_path(input: list): # throws exceptions
    left = input
    cond = Bool(True) 
    for line in input: 
        token = line.split()[0]
        match token:
            case "ASS":
                tokens = line.split()
                var = tokens[1]
                val, rem = parse(tokens[2:])
                ctx[var] = val
                if len(rem) != 0:
                    print("Reminder after assignment is not empty", rem)
                    raise "че?"
                continue
            case "COND":
                res = parse(line.split()[1:])
                cond = And(cond, res[0])
                if get_model(cond) is None:
                    return None
            case "ASSERT":
                res = parse(line.split()[1:])
                fail_cond = And(cond, Not(res[0]))
                cond = And(cond, res[0])
                model = get_model(fail_cond)
                if model is not None:
                    return fail_cond
    return None

def main():
    ok = True
    data = ""
    try:
        while True:
            data += (input() + '\n')
    except EOFError:
        pass

    if data[0] == '>' or data[:3] == 'ERR':
        print(data, end='')
        exit(1)

    execution_paths = data.split("***\n")
    for path in execution_paths[1:]:
        statements = path.split("\n")[:-1]
        cond = run_path(statements)
        if cond is not None:
            ok = False
            print("Assertion fails on:")
            print(get_model(cond))

    if ok:
        print('OK')

main()
