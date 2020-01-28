// Copyright (C) 2019 Hannu Viitala
// 
// The source code in this file is released under the MIT license.
// Go to http://opensource.org/licenses/MIT for the full license details.
// 
// Convert to C++ by Filmote

// *** A TILEMAP DEMO FOR THE POKITTO ***

#include "Pokitto.h"
#include "Tilemap.hpp"
#include "Data.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

Tilemap tilemap;

int main(){

    PC::begin();
    PD::persistence = true;
    PD::invisiblecolor = 0;


    // Initialise the map ..
    
    tilemap.set(16, 16, Data::mapPixels1);
    tilemap.tiles[0x0b] = Data::green16;
    tilemap.tiles[0x05] = Data::tree16;
    tilemap.tiles[0x04] = Data::grass16;
    tilemap.tiles[0x08] = Data::water16;

    int16_t vx = 0;
    int16_t vy = 0;
    int16_t x = -120;
    int16_t y = -100;

    uint16_t mapW = 16 * 16;        // 16 tiles of 16 pixels
    uint16_t mapH = 16 * 16;        // 16 tiles of 16 pixels

    int16_t heroOnScreenX = PD::width / 2; 
    int16_t heroOnScreenY = PD::height / 2;

    
    while (PC::isRunning()) {
        
        if (!PC::update()) continue;

        PC::buttons.pollButtons();        

        int16_t oldX = x;
        int16_t oldY = y;

        if (PC::buttons.pressed(BTN_LEFT)  || PC::buttons.repeat(BTN_LEFT, 1))    { x = x + 1; }
        if (PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, 1))   { x = x - 1; }
        if (PC::buttons.pressed(BTN_UP)    || PC::buttons.repeat(BTN_UP, 1))      { y = y + 1; }
        if (PC::buttons.pressed(BTN_DOWN)  || PC::buttons.repeat(BTN_DOWN, 1))    { y = y - 1; }


        // Check the map tile under the girl.
        
        int16_t girlCenterInMapX = (heroOnScreenX + 6) - x;
        int16_t girlCerterInMapY = (heroOnScreenY + 7) - y;
        uint8_t tileId = tilemap.GetTileId(girlCenterInMapX, girlCerterInMapY, 16);

        // If the tile is not grass, do not move.
        
        if (tileId != 0xb) {
            x = oldX;
            y = oldY;
        }

        
        // Check for out of bounds.
        
         if (x > 0)             x = 0;
         if (x + mapW < 110)    x = 110 - mapW;
         if (y > 0)             y = 0;
         if (y + mapH < 88)     y = 88 - mapH;
        
        
        tilemap.draw(x, y);
        PD::drawBitmapData(heroOnScreenX, heroOnScreenY, 12, 15, Data::girl12x15Pixels);
        

    }
    
    return 0;
    
}

