# generates expression classes from the loxc_expressions.txt file

description = '''
/**
 * In loxc, an expression is just data and as such has no associated methods.
 * Instead, loxc makes use of the visitor pattern to visit expressions. For
 * more information on each expression, and to add another one see
 * ../tools/loxc_expressions.txt
 */
'''

def make_init ():
    return "\n".join((
        '#ifndef expr_h',
        '#define expr_h\n',
        '// WARNING: THIS FILE IS AUTOGENERATED',
        '// Changes you make will not be kept.',
        description,
        '#include <memory> // std::shared_ptr',
        '#include <variant>',
        '#include <vector>',
        '#include "token.h"',
        '#include "val.h"',
    )) + "\n\n"

def make_expr (class_name, rest):
    out = ""
    rest = rest.split(":", 1)[1]
    arguments = rest.split(",")
    arguments = [a.strip().split() for a in arguments]
    # [type, name]

    out += "struct " + class_name
    out += "\n{\n"

    for arg in arguments:
        out += "\t{} {};\n".format(arg[0], arg[1])

    out += "\n\t{} (".format(class_name)
    out += ", ".join([ "{} {}_in".format(arg[0], arg[1]) for arg in arguments])
    out += ")\n\t\t: "
    out += ", ".join([ "{}(std::move({}_in))".format(arg[1], arg[1]) for arg in arguments])
    out += " {}\n"

    return out + "};\n\n"

def make_expr_using_declaration (names):
    out = "using Expr = std::variant<\n\tstd::monostate,\n\t"
    out += ",\n\t".join(["std::shared_ptr<struct {}>".format(name) for name in names])
    out += " >;\n\n"
    return out

def main ():
    source_file = open("tools/loxc_expressions.txt", "r")
    dest_file = open("src/expr.h", "w+")

    classes = []

    for line in source_file:
        line.strip()
        if not len(line) or line == "\n":
            continue
        if line[0] == "#":
            continue
        class_name, rest = line.split(' ', 1)
        classes.append((class_name, rest))
    
    source_file.close()

    names = [a[0] for a in classes]

    dest_file.write(make_init())
    dest_file.write(make_expr_using_declaration(names))
    dest_file.write('#include "stmt.h"\n\n')
    
    for class_name, rest in classes:
        dest_file.write(make_expr(class_name, rest))
        
    dest_file.write("#endif")
    dest_file.close()
    

if __name__ == "__main__":
    main()
