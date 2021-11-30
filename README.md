# Apex-Legends-SDK
*Open Source Cheat* for **Apex Legends**, designed for ease of use. Made to understand reversing of Apex Legends and respawn's modified source engine as well as their Easy Anti Cheat Implementation.

This Project consists of two parts, *Driver* and *Usermode*.
Since this project utilizes a Kernel Driver you will most likely want to change the ***hooked function*** as well as the *shellcode* that goes along with it. You can change both of these inside ***Memory.h*** in the Driver

Current function: `NtOpenCompositionSurfaceSectionInfo`

Current shellcode: `0x8B, 0x04, 0x24, 0x89, 0x41, 0x44, 0xC7, 0x41, 0x30, 0x0F, 0x00, 0x10`

To build this project, please download the ```Windows Driver Kit (WDK)```, and build in **Release | x64**. Build and place *KDMapper* (https://github.com/TheCruZ/kdmapper) in the directory of your build, as the project will attempt to map the driver for you.

Additionally, to correctly replace *EAC's splash screen*, you will need to change paths in `Utils.h`


![Image1](https://media.discordapp.net/attachments/733974815735808041/913891455209181205/unknown.png)

![Image2](https://media.discordapp.net/attachments/733974815735808041/913897236210008074/unknown.png)

Inside Apex Legends SDK you will find the SDK, consisting of the ***Entity system*** and ***Engine***. I have also included a *DirectX9 Overlay* which is used for rendering the menu as well as ESP.

This project also has support for *Python scripting*, to ***easily test and create features***.
Below is an example of a basic *Health-Glow* which is in the *x64/Release/Userscripts folder*, to see the implementation of python scripting, see `Scripts.h`


```python
import Entity

#Apex Legends Health-based Glow ESP

def main():
    #pLocal = Entity.LocalEntity() #We dont need it for this script, but this is how you would get the localPlayer

    for i in range(0, 100):
        pEntity = Entity.BaseEntity(i)
        if pEntity.isPlayer() and pEntity.iHealth() > 0: #Valid Player
            #pEntity.GlowStyle(101,101,46,90) #Bright Glow Style

            health = pEntity.iHealth() + pEntity.iShield()
            if 180 <= health <= 300: #WHITE
                pEntity.Glow(255, 255, 255)
            if 160 <= health <= 179: #GREEN
                pEntity.Glow(0, 255, 0)
            if 140 <= health <= 159: #YELLOW
                pEntity.Glow(255, 255, 0)
            if 110 <= health <= 139: #RED
                pEntity.Glow(255, 0 , 0)
            if 70 <= health <= 109: #PINK
                pEntity.Glow(255, 0, 255)
            if 40 <= health <= 69: #BLUE
                pEntity.Glow(0, 0, 255)
            if health < 40:        #CYAN
                pEntity.Glow(0, 255, 255)
```
