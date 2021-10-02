# OCRSudoku

Comment installer SDL2:

Sur linux (Debian ou unbuntu):

sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-ttf-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-mixer-dev

Pour verifier l'installation:

gcc sdl2minimal.c -lSDL2 -lSDL2main -o sdl2minimal
(si la commande ne renvoie pas d'erreur c'est que sdl est correctement installer)

---------------------------------------

Le programme affiche une image pendant 5 secondes
Il suffit d'utiliser la commande:
./main [chemin de l'image]

Ex: ./main test.jpg

