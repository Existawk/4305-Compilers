{
     [ declaration | statement ]*    -- separated by ';'
}

statement
    <whilestmt>
    <ifstmt>
    <forstmt>
    <readstmt>
    <writestmt>
    <breakstmt>
    <continuestmt>


whileStmt
    : tok_WHILE expr block    { $$ = makeWhileStmt( $2, $3 ); }
    ;

ifStmt
    : tok_IF expr block elseOpt   { $$ = makeIfStmt( $2, $3, $4 ); }
    ;

elseOpt
    : epsilon           { $$ = NULL; }
    | tok_ELSE block    { $$ = $2; }
    ;

forStmt
    : tok_FOR id tok_ASSIGN expr direction expr stepOpt block
      { $$ = makeForStmt( $2, $4, $5, $6, $7, $8 ); }
    ;

direction
    : tok_TO      { $$ = +1; }
    | tok_DOWNTO  { $$ = -1; }
    ;

stepOpt
    : epsilon     { $$ = NULL; }
    | tok_BY expr { $$ = $2; }
    ;

breakStmt
    : tok_BREAK   { $$ = makeBreakStmt(); }
    ;

continueStmt
    : tok_CONTINUE    { $$ = makeContinue(); }
    ;

readStmt
    : tok_READ '(' idList ')'   { $$ = makeReadStmt( $3 ); }
    ;

writeStmt
    : tok_WRITE '(' exprList ')'    { $$ = makeWriteStmt( $3 ); }
    ;

