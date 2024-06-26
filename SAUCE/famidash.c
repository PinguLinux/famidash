// THE GAME LOOP ==============================================
//
// yup, this is the entire game loop. it's that simple.
// I wanted to make the code super easy to read, so I split it
// into dozens of files, each containing segments of code.
//
// ============================================================



// THE INCLUDE FILE ===========================================
//
// The entirety of the project is on the shoulders of this
// singular line. 
//
//                    DO. NOT. REMOVE. IT.
//
// If you have issues with the repository and the cause is you
// removing this line, the entire team will be informed to not
// communicate with you further.
#include "include.h"
// ============================================================



// VOID MAIN() ================================================
//
// This isn't an int main() because i don't use the terminal to
// debug lmao
void main(){
    ppu_off();

    ppu_mask(0x00 | (1 << 1) | (1 << 2));

	mmc3_set_8kb_chr(MENUBANK);

    pal_bg(paletteDefault);
    pal_spr(paletteDefaultSP);
    // use the second set of tiles for sprites
	// both bg and sprites are set to 0 by default
	bank_spr(1);

    set_vram_buffer(); // do at least once

    // ppu_on_all();
    // pal_fade_to(4,0);

	// These are done at init time
    // gameState = 0x01;
    // level = 0x00;
	// auto_fs_updates = 0;

	// Get it? it spells DASH
	if (SRAM_VALIDATE[0] != 0x0D
	 || SRAM_VALIDATE[1] != 0x0A
	 || SRAM_VALIDATE[2] != 0x05
	 || SRAM_VALIDATE[3] != 0x20) {
		// set the validation header and then reset coin counts
		setdefaultoptions();

	}
    while (1){
        ppu_wait_nmi();

		switch (gameState){
			case 0x01: {
				mmc3_set_prg_bank_1(GET_BANK(state_menu));
				if (!kandowatchesyousleep) state_menu();
				else {
					pal_fade_to_withmusic(4,0);
					ppu_off();
					pal_bg(splashMenu);
					mmc3_set_8kb_chr(MENUBANK);

					set_scroll_x(0);
					set_scroll_y(0);

					kandowatchesyousleep = 1;

					//	mmc3_set_prg_bank_1(GET_BANK(state_menu));

					switch (kandotemp){
						case 0x00:	music_play(song_menu_theme); break;
						case 0x01:	break;
					}

					settingvalue = 0;

					has_practice_point = 0;

					#include "defines/mainmenu_charmap.h"
					// Enable SRAM write
					POKE(0xA001, 0x80);

					oam_clear();	

					levelselection();
				}
					break;
			}
			case 0x02: {
  				player_gravity[0] = 0x00;
			//	if (twoplayer)
				player_gravity[1] = 0x00;
			//	else player_gravity[1] = 0x01;						
				state_game(); break;
			}
			case 0x03: {
				mmc3_set_prg_bank_1(GET_BANK(state_lvldone));
				state_lvldone();
				break;
			}
			case 0x04: {
				mmc3_set_prg_bank_1(GET_BANK(bgmtest));
				bgmtest();
				break;
			}
			//case 0x04: state_demo(); break;
		}
    }
}
// ============================================================


// WHY I SPLIT THE GAME LOOP ==================================
//
// I like making my code super readable by outsiders, and I
// felt like this was the best way to do it.
// ============================================================

void setdefaultoptions() {
		SRAM_VALIDATE[0] = 0x0d;
		SRAM_VALIDATE[1] = 0x0a;
		SRAM_VALIDATE[2] = 0x05;
		SRAM_VALIDATE[3] = 0x20;
		for (tmp2 = 0; tmp2 <= LEVEL_COUNT; tmp2++) {
			coin1_obtained[tmp2] = 0;
			coin2_obtained[tmp2] = 0;
			coin3_obtained[tmp2] = 0;
			LEVELCOMPLETE[tmp2] = 0;
		}
		memfill(SRAM_VALIDATE+0x0E, 0, 0x1F-(0x0E - 1));
		twoplayer = 0;
		//musicoff = 0;
		//sfxoff = 0;
		//jumpsound = 0;
		//oneptwoplayer = 0;
		//platformer = 0;
		options = 0; 


		invisible = 0;
		color1 = 0x2A;
		color2 = 0X2C;		
		color3 = 0x0F;
		discomode = 0;
		icon = 0;
		trails = 0;
		retro_mode = 0;
		palette_cycle_mode = 0;
		gameboy_mode = 0;
		decorations = 1;
		for (tmp2 = 0; tmp2 < 20; tmp2++) {
			achievements[tmp2] = 0;
		}
		return;
}