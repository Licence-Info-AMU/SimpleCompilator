main() {
  ecrire( 5 - 3 + 2 );   # doit afficher 4 et non pas 0
  ecrire( 3 * 2 + 1 );   # doit afficher 7 et non pas 9
  ecrire( 1 + 2 * 3 );   # doit afficher 7 et non pas 9
  ecrire( 0 = 1 & 2 = 2 ); # doit afficher zéro
  ecrire( ! ( 0 = 1 ) | ! ( 2 = 2 ) ); # doit afficher différent de zéro (-1)
}
