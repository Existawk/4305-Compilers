// Here are some legal integer literals:

    0   1   2   3   01   012   0123

    0x0ddba11   0b0110   0o377

// Here are some legal character literals:

    'a'   '('   '$'   '0'   '9'   'b'   'f'   'n'   'r'   't'   'v'

// Legal character literals with escape sequences:

    '\''  '\"'  '\?'  '\\'  '\a'  '\b'  '\f'  '\n'  '\r'  '\t'  '\v'

// Weirdly, these are also legal:

    '"' '?'

// Here are some legal octal escape sequence character literals:

    '\7'  '\77'   '\377'  '\12'   '\70'   '\0'

// Legal hex escape sequence character literals:

    '\xF' '\xcc'  '\x01'  '\x0'   '\x12'  '\xfA'  '\x3b'

// These are not legal integer literals:

    -37   +113   16.75   fifteen   +1.34e+34
    0o88   0xG17   0b212

    '\q'
    '''
    '\'
    '\8'
    '\19'
    '\777'
    '\xfff'

// Were you surprised at how any of that scanned?

// Why or why not?