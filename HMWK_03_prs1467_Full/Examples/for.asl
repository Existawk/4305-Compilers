// The for statement
{
  for i <- 1 to 10 {
    write "i is now ", i, "\n";
  };

  for j <- 1 + 2 * 3 downto 17 / 4 - ( 1 + 1 + 1 ) {
    write "j is now ", j, "\n";
  };

  for k <- ( 19 + 21 ) / ( 8 * 5 ) to 10 by 22 / 11 {
    write "k is now ", k, "\n";
  };

  for l <- ( 100 - 63 ) / 3 - ( 1 + 1 ) downto 1 by 4 * 17 / 33 {
    write "l is now ", l, "\n";
  };
}
// OK, these are a _little_ more interesting than break and continue.