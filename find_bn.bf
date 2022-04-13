++++++++++      case 0 = retour chariot
> ,             case 1 = nb recherché

< ...........................................................................

>> +            déplacement à la case 2

[
    
    [-]         réinitialise la case 2
    
    >
    [-]         réinitialise la case 3
    
    <<
    [ > + > +
      << - ]    copie de 1 vers 2 et 3

    >>
    [ << +    
      >> - ]    copie de 3 vers 1
    
    > ,         case 4 = nb demandé

    [ << - 
      >> -  
    ]           on décrémente la case 2 tant que la case 4 n'est pas à 0
    <<          retour à la case 2
]


O 79    K 75    exclamation 33

>>> ++++++++++  case 5 = compteur de 10

[
    > +++++++   case 6 = O et K
    > +++       case 7 = exclamation
    << -
]

> +++++++++.    affiche O
----.           affiche K
> +++ .         affiche exclamation
<<<<<<< .       affiche \n