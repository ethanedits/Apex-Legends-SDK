import Entity

#Apex Legends Health-based Glow ESP

def main():
    #pLocal = Entity.LocalEntity() #We dont need it for this script, but this is how you would get the localPlayer

    for i in range(0, 100):
        pEntity = Entity.BaseEntity(i)
        if pEntity.isPlayer() and pEntity.iHealth() > 0: #Valid Player
            #pEntity.GlowStyle(101,101,46,90)

            health = pEntity.iHealth() + pEntity.iShield()
            bleedoutstate = pEntity.isKnocked()

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
            if bleedoutstate != 0: #purple
                pEntity.Glow(136, 0, 255)
