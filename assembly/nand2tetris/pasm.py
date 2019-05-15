#!/usr/bin/python3

from typing import Dict, List, Tuple
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


def writeBinaryFile(file_path: str, binary_lines: List[str]):
    dest_path = (file_path.rsplit('.', 1)[0]) + '.hack'
    with open(dest_path, 'w+') as file:
        file.write('\n'.join(binary_lines) + '\n')


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
    """ Convert an A instruction into a 16 bit binary string
    If a new symbol is encountered, return True so that the
    parent function can increment next_symbol
    """
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


def getCInstructionTokens(line: str) -> Tuple[str, str, str]:
    comp = dest = jump = nextSplit = None
    split_one = line.split('=')

    if len(split_one) is 2:
        dest = split_one[0].strip()
        nextSplit = split_one[1]
    else:
        nextSplit = split_one[0]

    split_two = nextSplit.split(';')
    if len(split_two) is 2:
        jump = split_two[1].strip()

    comp = split_two[0].strip()
    return (comp, dest, jump)


def getCompCode(token: str) -> str:
    comp_type = '0'
    for char in token:
        if char is 'M':
            comp_type = '1'
    comp_char = 'A' if comp_type is '0' else 'M'
    return comp_type + {
        '1': '111111',
        '-1': '111010',
        'D': '001100',
        comp_char: '110000',
        '!D': '001101',
        '!'+comp_char: '110001',
        '-D': '001111',
        '-'+comp_char: '110011',
        'D+1': '011111',
        comp_char+'+1': '110111',
        'D-1': '001110',
        comp_char+'-1': '110010',
        'D+'+comp_char: '000010',
        'D-'+comp_char: '010011',
        comp_char+'-D': '000111',
        'D&'+comp_char: '000000',
        'D|'+comp_char: '010101',
    }.get(token, '101010')


def getDestCode(token: str) -> str:
    return {
        'M': '001',
        'D': '010',
        'MD': '011',
        'A': '100',
        'AM': '101',
        'AD': '110',
        'AMD': '111'
    }.get(token, '000')


def getJumpCode(token: str) -> str:
    return {
        'JGT': '001',
        'JEQ': '010',
        'JGE': '011',
        'JLT': '100',
        'JNE': '101',
        'JLE': '110',
        'JMP': '111',
    }.get(token, '000')


def parseCInstruction(line: str, symbol_table: Dict[str, int], binary_lines: List[str]):
    (comp, dest, jump) = getCInstructionTokens(line)
    binary_lines.append('111' + getCompCode(comp) +
                        getDestCode(dest) + getJumpCode(jump))


def secondPass(source_lines: List[str], symbol_table: Dict[str, int]) -> List[str]:
    """ Translate each line that represents an A or C instruction """
    next_symbol = 16
    binary_lines = []
    for line in source_lines:
        if line[0] == '@':
            new_symbol = parseAInstruction(
                line, symbol_table, binary_lines, next_symbol)
            if new_symbol:
                next_symbol += 1
        elif line[0] != '(' and line[0] != '/':
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
    writeBinaryFile(file_path, binary_lines)


main()
