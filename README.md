## Wiicraft - Nintendo Wii Client for Minecraft Version 1.2.5
![Wiicraft Icon](/icon.png)![Wiicraft Icon](/icon.png)![Wiicraft Icon](/icon.png)![Wiicraft Icon](/icon.png)![Wiicraft Icon](/icon.png)![Wiicraft Icon](/icon.png)   

![Wiicraft Icon1](/ingame_wiicraft1.PNG)

![Wiicraft Icon2](/ingame_wiicraft2.PNG)

## Demo
[![Demo](https://img.youtube.com/vi/lv3MmPLz3wI/default.jpg)](https://youtu.be/lv3MmPLz3wI)

## Compile from Source
1. Make sure you have installed [devkitpro](https://devkitpro.org/) and [GRRLIB](https://github.com/GRRLIB/GRRLIB)
2. Open cmd and redirect to your local Wiicraft folder and run
```
make 
```
## Run Wiicraft
1. On your SD Card create a new folder ```Wiicraft``` under ```apps```  and copy the ```Game.ini``` into the folder and create a new empty subfolder ```world``` (this is used to cache the chunk data from the server)
2. Open the ```Game.ini``` with a Texteditior and make sure that the values of "Host" and "Port" are your Minecraft Server IP and Port
3. Replace the ```server.properties``` of your Minecraft Server 1.2.5 with the one from the Wiicraft Release and start the server
4. Configure ```wiiload``` for your Wii IP and run it by ```wiiload Wiicraft.dol```. You can also try to copy ```Wiicraft.dol``` onto your SD Card and launch it but for me it doesn't work. 
5. Make sure you put the SD Card back into your Wii

## Controls
- ```Wiimote:``` Rotate Player
- ```Nunchuk Stick:``` Move Player
- ```Button A:``` Add Block
- ```Button B:``` Remove Block
- ```Button Z (Nunchuk):``` Drop Block
- ```Button Left:``` Inventory Left Scroll
- ```Button Right:``` Inventory Right Scroll 
- ```Button Up:``` Toggle Debug Infos
