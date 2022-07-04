// Hey, I'm a comment!  Huzzah!
{
  // Declarations with and without initializers
  integer a <- 17;
  real    b <- a * 12.345;
  integer c;
  string  d <- "<Front of string>" + " " + "<back of string>";
  real    e;
  string  f;

  write "What is c? ";
  read c;

  write "What are e and f? ";
  read e, f;

  a <- +1 + 2 + -3;
  b <- 4.5 * 6.7 ^ 8.9;
  c <- 0.12 / 34.5 + 6.78 - 9;
  d <- d - "o";
  e <- pi * a^2;
  f <- "\' \" \? \\ \a \b \f \n \r \t \v q\1w\22e\377rty" * 2;

  write "a = ", a, ", b = ", b, "\n";
  write "c = ", c, ", d = ", d, "\n";
  write "e = ", e, ", f = ", f, "\n";

  // If without an else
  if  a == c {
    write "If-without-else then block\n";
  };

  // If with an else
  if d <> f {
    write "If-with-else then block\n";
  } else {
    write "If-with-else else block\n";
  };

  while c == a {
    a <- c + 1;
    write "while body\n";
  }
}

// We can even have a comment at the end of the file
