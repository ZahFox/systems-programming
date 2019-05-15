#!/usr/bin/python3

from typing import Dict, List
import os.path
import sys


def checkForSourceFile():
    arg_count = len(sys.argv)
    if arg_count < 2:
        print('Please provide an input file path')
        sys.exit()

    file_path = sys.argv[1]
    file_exists = os.path.isfile(file_path)

    if file_exists is False:
        print('No file named', file_path, 'could be found.')
        sys.exit()
    return file_path


def parseSourceFile(file_path: str) -> List[str]:
    return list(filter(None, (line.rstrip().lstrip().upper() for line in open(file_path, 'r'))))


def initSymbolTable():
    return dict([('SP', 0), ('LCL', 1), ('ARG', 2), ('THIS', 3), ('THAT', 4),
                 ('R0', 0), ('R1', 1), ('R2', 2), ('R3',
                                                   3), ('R4', 4), ('R5', 5), ('R6', 6),
                 ('R7', 7), ('R8', 8), ('R9', 9), ('R10',
                                                   10), ('R11', 11), ('R12', 12),
                 ('R13', 13), ('R14', 14), ('R15', 15), ('SCREEN', 16384), ('KBD', 24576)])


def firstPass(source_lines: List[str], symbol_table: Dict[str, int]):
    line_counter = 0
    for line in source_lines:
        if line[0] == '(':
            r_paren = line.find(')', 1)
            symbol = line[1:r_paren]
            symbol_table[symbol] = line_counter
        else:
            line_counter += 1


def parseAInstruction(line: str, symbol_table: Dict[str, int], binary_lines: List[str], next_symbol: int):
    line_value = line[1:]
    if line_value[0].isdigit():
        address = format(int(line_value), 'b').zfill(16)
        binary_lines.append(address)
        return False
    else:
        symbol_address = symbol_table.get(line_value)
        if symbol_address:
            address = format(int(symbol_address), 'b').zfill(16)
            binary_lines.append(address)
            return False
        else:
            symbol_table[line_value] = next_symbol
            address = format(int(next_symbol), 'b').zfill(16)
            binary_lines.append(address)
            return True


def parseCInstruction(line: str, symbol_table: Dict[str, int], binary_lines: List[str]):
    binary_lines.append(line)


def secondPass(source_lines: List[str], symbol_table: Dict[str, int]) -> List[str]:
    next_symbol = 16
    binary_lines = []
    for line in source_lines:
        if line[0] == '@':
            new_symbol = parseAInstruction(
                line, symbol_table, binary_lines, next_symbol)
            if new_symbol:
                next_symbol += 1
        elif line[0] != '(':
            parseCInstruction(line, symbol_table, binary_lines)
    return binary_lines


def main():
    file_path = checkForSourceFile()
    source_lines = parseSourceFile(file_path)
    source_line_count = len(source_lines)

    if source_line_count is 0:
        print(file_path, 'contains no source code.')
        sys.exit()

    symbol_table = initSymbolTable()
    firstPass(source_lines, symbol_table)
    binary_lines = secondPass(source_lines, symbol_table)

    for line in binary_lines:
        print(line)


main()
