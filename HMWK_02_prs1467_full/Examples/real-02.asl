// Here are some real literals:

    1.2   12345.67890
     .0        .54321
    0.    09876.

    12.345e21     9.876e+0   6758.4932e-34
        .0e0       .123e+13       .456e-0009
      000.e0000    321.e+32     98712.e-17

       123e54        72e+00008       9e-23

// These are not legal real literals:

    2..3   .2.3   3.2.1   -.23   +23.
    1.23e+-12   1e--4   .34e++9   6.5e-+22   .e+
    e12   1e   0e+   12.34e-

// Were you surprised at how any of that scanned?

// Why or why not?