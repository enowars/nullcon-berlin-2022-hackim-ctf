#!/usr/bin/env python3
import os
import sys


def valid_hex(char: int):
    return (
        (ord("0") <= char <= ord("9"))
        or (ord("A") <= char <= ord("F"))
        or (ord("a") <= char <= ord("f"))
    )


def compile_template(in_file, out_file):
    vars = []

    def handle_line(output: "IO", line: str):

        # An \x escape cannot be folloewd with a valid hex number directly.
        was_escaped = False
        var_splits = line.split(b"{{")
        for i, var_split in enumerate(var_splits):
            if i == 0:
                remainder = var_split
            else:
                variable, remainder = var_split.split(b"}}")
                try:
                    name, printtype = variable.split(b"%")
                except ValueError as ex:
                    raise Exception(r"Variables need a type like {{varname%s}}", ex)
                vars.append(name)
                output.write(b"%")
                output.write(printtype)

                was_escaped = False

            for char in remainder:
                # print("char", chr(char), "was_escaped", was_escaped)
                # something strange with questionmarks...
                # if (char == ord("?")):
                if (char in [ord(x) for x in "%()/,\\'\""]) or (
                    was_escaped and valid_hex(char)
                ):
                    output.write(b"\\")
                    output.write(hex(char)[1:].encode())

                    if char == ord("%"):
                        # we need %% so that printf ignores this %
                        output.write(b"\\")
                        output.write(hex(char)[1:].encode())

                    was_escaped = True
                else:
                    output.write(b"%c" % char)

                    was_escaped = False
        # C preprocessor eats up the newlines for some reason.
        output.write(b"\\n")

    with open(in_file, "rb") as input:
        with open(out_file, "wb") as output:
            for line in input:
                if line.startswith(b"TEMPLATE("):
                    output.write(line)
                    continue
                if line.startswith(b")") and line.strip() == (b")"):
                    # Last element, closing the template.
                    output.write(line)
                    continue
                handle_line(output, line)
            for var in vars:
                output.write(b", (")
                output.write(var)
                output.write(b")")


if __name__ == "__main__":
    # for file in os.listdir("."):
    # if file.endswith(".templ.html"):
    if len(sys.argv) < 2:
        print(
            "Templates need to start with `TEMPLATE(` and end with `)`, each on a new line"
        )
        print(
            r"Templates may use parameters like {{this%s}} - with types as normal printf types."
        )
        print("Usage: to_templ.py <in_file> [<out_file>]")
        exit(1)

    in_file = sys.argv[1]
    if len(sys.argv) < 3:
        if not in_file.endswith(".html"):
            print("Usage: to_templ.py <in_file>templ.html [<out_file>.templ]")
            exit(2)
        out_file = in_file[:-5]  # cut off .html
    else:
        out_file = sys.argv[2]

    compile_template(in_file, out_file)
