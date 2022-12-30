Proccessing of 8-bit rgb .ppm images
-

**Build**: 
```
gcc src/main.c src/Obraz.c src/FiltryPGM.c src/FiltryPPM.c src/Opcje.c -o Obrazy2 -lm
```

**run**: `./Obrazy2 {options}` 

**example**: `./Obrazy2 -i k.ppm -k -d`

Ten program mozna uruchomic tylko w srodowisku sytemu Linux lub Unix.
