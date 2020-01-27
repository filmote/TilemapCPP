// # Copyright (C) 2019 Hannu Viitala
// #
// # The source code in this file is released under the MIT license.
// # Go to http://opensource.org/licenses/MIT for the full license details.
// #
// # Converted to C++ by Filmote

// # *** A TILEMAP DEMO FOR THE POKITTO ***

#include "Pokitto.h"
#include "Tilemap.hpp"
#include "Data.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;

const uint16_t mapW = 16 * 16;        // 16 tiles of 16 pixels
const uint16_t mapH = 16 * 16;        // 16 tiles of 16 pixels

Tilemap tilemap;

enum TileType {
	Water = 8,
	Grass = 4,
	Tree = 5,
	Green = 11,
}; 

int main(){

    PC::begin();
    PD::persistence = true;
    PD::invisiblecolor = 0;


    // Initialise the map ..
    
    tilemap.set(16, 16, Data::mapPixels1);
    tilemap.tiles[TileType::Green] = Data::green16;
    tilemap.tiles[TileType::Tree] = Data::tree16;
    tilemap.tiles[TileType::Grass] = Data::grass16;
    tilemap.tiles[TileType::Water] = Data::water16;

    
    #ifdef PROJ_MODE2
    int16_t x = -120;
    int16_t y = -100;
    #endif

    #ifdef PROJ_MODE15
    int16_t x = -20;
    int16_t y = -50;
    #endif

    int16_t xOffset = 0;
    int16_t yOffset = 0;

    int16_t heroOnScreenX = PD::width / 2; 
    int16_t heroOnScreenY = PD::height / 2;

    
    while (PC::isRunning()) {
        
        if (!PC::update()) continue;

        PC::buttons.pollButtons();        


        // capture the old coordinates in case we need to move back ..
        
        int16_t oldX = x;
        int16_t oldY = y;
        int16_t oldXOffset = xOffset;
        int16_t oldYOffset = yOffset;


        if (PC::buttons.pressed(BTN_LEFT) || PC::buttons.repeat(BTN_LEFT, 1))    { 


            // If we are already on the right hand side of the screen, come back to the centre ..
            
            if (xOffset < 0) {                                  
                xOffset++;
            }
            
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (x < 0) {
                x++;
            }
            
            
            // If we cannot scroll the screen, move the player to the left ..
            
            else if (x <= (PD::width / 2)) {     
                xOffset++;
            }


        }
        
        if (PC::buttons.pressed(BTN_RIGHT) || PC::buttons.repeat(BTN_RIGHT, 1))   { 


            // If we are already on the left hand side of the screen, come back to the centre ..
            
            if (xOffset > 0) {
                xOffset--;
            }
                 
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (x > (PD::width - mapW) && x <= 0) {
                x--;
            }
            
            
            // If we cannot scroll the screen, move the player to the right ..
            
            else if (x >= (PD::width - mapW)) {
                xOffset--;
            }

            
        }
        
        
        if (PC::buttons.pressed(BTN_UP) || PC::buttons.repeat(BTN_UP, 1))      { 

            
            // If we are already in the bottom section of the screen, come back to the centre ..
            
            if (yOffset < 0) {                                  
                yOffset++;
            }
            
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (y < 0) {
                y++;
            }
            
            
            // If we cannot scroll the screen, move the player up ..
            
            else if (y <= (PD::height / 2)) {     
                yOffset++;
            }
            
            
        }
        
        if (PC::buttons.pressed(BTN_DOWN) || PC::buttons.repeat(BTN_DOWN, 1))    { 


            // If we are already in the top section of the screen, come back to the centre ..
            
            if (yOffset > 0) {
                yOffset--;
            }
                 
            
            // Otherwise scroll the screen itself if we can ..
            
            else if (y > (PD::height - mapH) && y <= 0) {
                y--;
            }
            
            
            // If we cannot scroll the screen, move the player down ..
            
            else if (y >= (PD::height - mapH)) {
                yOffset--;
            }
           
        }



        // Check the map tile under the player.
        
        int16_t girlCenterInMapX = (heroOnScreenX + 6) - x - xOffset;
        int16_t girlCerterInMapY = (heroOnScreenY + 7) - y - yOffset;
        uint8_t tileId = tilemap.GetTileId(girlCenterInMapX, girlCerterInMapY, 16);


        // If the tile is not green, do not move.
        
        if (tileId != TileType::Green) {
            
            x = oldX;
            y = oldY;
            xOffset = oldXOffset;
            yOffset = oldYOffset;
            
        }

        
        tilemap.draw(x, y);
        PD::drawBitmapData(heroOnScreenX - xOffset, heroOnScreenY - yOffset, 12, 15, Data::girl12x15Pixels);

    }
    
    return 0;
    
}

