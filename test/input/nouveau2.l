main() 
entier $temp, entier $g, entier $l, entier $v;
{
  $temp = lire();                       # temperature de l'eau
  $g = $temp < 0 | $temp = 0 ? 1 : 0;   # glace ? 
  $l = $temp < 100 & 0 < $temp ? 1 : 0; # liquide ?
  $v = !$g & !$l ? 1 : 0;               # vapeur ?
  ecrire($g);
  ecrire($l);  
  ecrire($v);  
}
