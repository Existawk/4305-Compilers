This is a file of things that match the ID regular expression.

Well, except for the punctuation symbols, which will show up as
illegal characters.

Remember, an ID can start with a _ so _Fred is a legal ID.

Also, _ is a legal ID by itself, so _ is OK as is ___.

Digits are not allowed at the front of an ID so 0123 is not a
legal ID.  (It's an integer literal, of course.)

Even if a token has some letters or underscores after the
digit(s), it's still not a legal ID.  So 0_Fred doesn't work as an
ID.

Were you surprised at how 0_Fred scanned?

Why or why not?
