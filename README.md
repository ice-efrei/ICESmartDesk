## Écran de contrôle

### Design

![Untitled](https://www.notion.so/image/https%3A%2F%2Fprod-files-secure.s3.us-west-2.amazonaws.com%2F9d871417-2fcd-46e1-b211-64eebd6f288f%2F89781444-d92a-4d79-87c2-ed388b8c4a0c%2FUntitled.png?table=block&id=4afeda0c-53f0-4784-8494-4ebc76c092d1&spaceId=9d871417-2fcd-46e1-b211-64eebd6f288f&width=2000&userId=90264d4b-b513-41ed-b099-40d3e25b3110&cache=v2)

Il est intéressant de noter que les couleurs ne fonctionnement pas de la même manière avec les écran TFT, voici un tableau de corespondance que vous pourrez retrouver dans le fichier `includes/constants.h`

|  | HEX RGB | RGB565 |
| --- | --- | --- |
| ice dark blue | 143649 | 0x11A9 |
| ice light blue | C9DDE9 | 0xCEFD |
| temp blue | 00C2FF | 0x061F |
| temp yellow | FFD600 | 0xFEA0 |
| temp red | FF0000 | 0xF800 |

[Rinky-Dink Electronics](http://www.rinkydinkelectronics.com/calc_rgb565.php)

[The library could not display some color correctly · Bodmer TFT_eSPI · Discussion #2322](https://github.com/Bodmer/TFT_eSPI/discussions/2322)

On peut aussi noter que notre écran fait 240 pixel par 240 pixel. À partir de ça, on peut simplement réutiliser les valeurs de figma pour simplement connaitre les coordonnées où placer les textes et autres éléments. e.g. texte du haut en x:87.25 y:10 ou la barre sous le texte en y:41 de H:5 W:130 et R:3.

### Configurer https://github.com/Bodmer/TFT_eSPI

Nous utilisons la librairie https://github.com/Bodmer/TFT_eSPI. C’est une librairie qui propose de nombreuses possibilités et options. Afin de l’utiliser correctement, il faut toutefois la configurer après installation.

Pour commencer, nous devons ajouter la librairie au projet en modifiant `platformio.ini` et lançant un build du projet.

```
[env:nodemcu-32s]
platform = espressif32
board = nodemcu-32s
framework = arduino
board_build.filesystem = littlefs

lib_deps =
    SPIFFS
    FS
    SPI
    TFT_eSPI
```

Une fois fait, nous devrions avoir accès à un dossier `.pio/libdeps/nodemcu-32s/TFT_eSPI`.

<aside>
👉 Note: il arrive que le dossier de soit pas synchronisé, dans ce cas n’hésitez pas à recharger le dossier `.pio` à partir du disque.

![Untitled](https://www.notion.so/image/https%3A%2F%2Fprod-files-secure.s3.us-west-2.amazonaws.com%2F9d871417-2fcd-46e1-b211-64eebd6f288f%2F84392357-53f4-4279-b47a-29e49e51a39a%2FUntitled.png?table=block&id=1f395dbb-0973-47ba-99cf-0c23ddc1d6f6&spaceId=9d871417-2fcd-46e1-b211-64eebd6f288f&width=2000&userId=90264d4b-b513-41ed-b099-40d3e25b3110&cache=v2)

</aside>

À partir de là, nous pouvons sélectionner notre propre fichier de configuration contenant les version d’écran et branchements.

In `.pio\\libdeps\\nodemcu-32s\\TFT_eSPI\\User_Setup_Select.h` replace line 27 to include the custom made setup file.

```cpp
#include <User_Setup.h>

```

Turns into

```cpp
#include "../../../../include/ICE_Setup_GC9A01_NodeMCU_32S.h"
```

This allows the library to use our configuration of PINs and screens. For custom use please take a look at [TFT_eSPI github](https://github.com/Bodmer/TFT_eSPI).

Cette configuration est faite à partir des branchements que vous retrouverez du projet [DeskClock](https://www.notion.so/DeskClock-043bb707f59040c6bb3c084b1daeaa4a?pvs=21).

[Interfacing ESP8266 NodeMCU with ST7789 TFT Display](https://simple-circuit.com/esp8266-nodemcu-st7789-tft-ips-display/)

### Charger des font

Pour ce projet nous allons utiliser la police Poppins, incroyable pour absolument tous les projets. De nouveau, nous utilisons la librairie https://github.com/Bodmer/TFT_eSPI qui supporte les polices personnalisées. Toutefois quelques manipulations sont requises.

Pour commencer, nous devons configurer https://github.com/littlefs-project/littlefs dans notre projet plaformio. Pour cela nous spécifions le build system de l’esp dans la `platformio.ini` .

```
[env:nodemcu-32s]
platform = espressif32
board = nodemcu-32s
framework = arduino
board_build.filesystem = littlefs

lib_deps =
    SPIFFS
    FS
    SPI
    TFT_eSPI
```

Une fois cela fait et le projet build à nouveau, nous pouvons commencer à traiter nos fonts. Pour commencer nous pouvons télécharger Poppins Regular et Bold sur google font.

[PoppinsBold.ttf](https://prod-files-secure.s3.us-west-2.amazonaws.com/9d871417-2fcd-46e1-b211-64eebd6f288f/d65944fb-5b9d-458d-a38b-4a31640a7130/PoppinsBold.ttf)

[PoppinsRegular.ttf](https://prod-files-secure.s3.us-west-2.amazonaws.com/9d871417-2fcd-46e1-b211-64eebd6f288f/182fde29-3b01-4f9a-b5ed-1abb9cdff62a/PoppinsRegular.ttf)

Pour les utiliser, https://github.com/Bodmer/TFT_eSPI nécessite de convertir ces font en un format plus brut. Pour cela nous utilisons le script mis à disposition par la librairie dispo en `.pio/libdeps/nodemcu-32s/TFT_eSPI/Tools/Create_Smooth_Font/Create_font/Create_font.pde`.

C’est un simple script [processing](https://processing.org/), pour le faire fonctionner il vous faudra simplement placer les font dans le dossier data et modifier le script pour générer les fonts. Tous les détails sont dans le script en lui même mais pour résumer changer le nom du fichier font et de la taille souhaité à partir de la ligne 130.

```java
String fontName = "Final-Frontier";  // Manually crop the filename length later after creation if needed
                                     // Note: SPIFFS does NOT accept underscore in a filename!
String fontType = ".ttf";
//String fontType = ".otf";

// Define the font size in points for the TFT_eSPI font file
int  fontSize = 28;

// Font size to use in the Processing sketch display window that pops up (can be different to above)
int displayFontSize = 28;
```

Répétez l’opération autant de fois que nécessaire plus placer les fichier obtenue dans un dossier `data` à la racine du projet plateformio.

[PoppinsBold20.vlw](https://prod-files-secure.s3.us-west-2.amazonaws.com/9d871417-2fcd-46e1-b211-64eebd6f288f/10d6ab52-167f-46ca-8769-ea300d7c34ed/PoppinsBold20.vlw)

[PoppinsBold50.vlw](https://prod-files-secure.s3.us-west-2.amazonaws.com/9d871417-2fcd-46e1-b211-64eebd6f288f/1e893c88-00aa-4195-9ac4-c3209e9c9934/PoppinsBold50.vlw)

[PoppinsRegular20.vlw](https://prod-files-secure.s3.us-west-2.amazonaws.com/9d871417-2fcd-46e1-b211-64eebd6f288f/a327a204-b3ec-4a79-b030-539b73ca7876/PoppinsRegular20.vlw)

On peut maintenant build le filesystem et l’uploader avant de continuer vers l’affichage concret sur l’écran.

![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/9d871417-2fcd-46e1-b211-64eebd6f288f/aa56ec3d-0671-48ca-b522-9f7433ec3754/Untitled.png)

[ESP8266 VS Code PlatformIO: Upload Files to Filesystem LittleFS | Random Nerd Tutorials](https://randomnerdtutorials.com/esp8266-nodemcu-vs-code-platformio-littlefs/)

# Find more in the Notion page [ICESmartDesk](https://www.notion.so/ice-efrei/ICE-Smart-Desk-2e7f50113be043d7afbce4b6cf5c90bd?pvs=4)