#include "roach.h"

int main() {
    SDL_Surface* floor = NULL;
    SDL_Surface* screen = init("img/marioWorld.bmp", &floor);
    SDL_Surface *sprites = loadSprites("img/roach.bmp");
    SDL_Surface *tapis = LoadImage ("img/bathmat.bmp", X_INIT_TAPIS, Y_INIT_TAPIS);

   
    Roach* roaches = CreateRoaches(screen, sprites, MAX_ROACHES);
	int deltax;
    int deltay;
    int btndown;
    int stop = 1;
    SDL_Event event;
    
    while(stop) {
 while ( SDL_PollEvent (&event)) { // tant qu'il y a un évènement
    switch ( event.type ){
        case SDL_QUIT:
            stop = 0;
	       break;
	   case SDL_MOUSEBUTTONDOWN:
	       if (event.button.button == SDL_BUTTON_LEFT
			&& PointInRect( event.button.x, event.button.y,
					tapis->clip_rect.x,tapis->clip_rect.y,
					tapis->w,tapis->h)){
			/* Clic sur le tapis */
			btndown = 1;
			// Ecart entre clic et coin sup gauche tapis
			deltax = event.button.x - tapis->clip_rect.x;
			deltay = event.button.y - tapis->clip_rect.y;
            }
		  break;
        case SDL_MOUSEMOTION :
		  if (btndown) { // Drag and drop
	    		// "Nettoyage" du tapis (2 options )
			SDL_BlitSurface(floor,NULL, screen,NULL);//option 1
			// Mise à jour position du tapis
			tapis->clip_rect.x = event.button.x - deltax;
			tapis->clip_rect.y = event.button.y - deltay;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            btndown = 0;
        break;
    }
 }
        SDL_BlitSurface(floor, NULL, screen, NULL);
        int nVis = MarkHiddenRoaches(roaches, MAX_ROACHES, tapis);
        if (nVis > 0){
        MoveRoaches(roaches, MAX_ROACHES,screen);
        DrawRoaches(roaches, MAX_ROACHES, screen);
        }
        DrawImage (tapis, screen);
        SDL_Flip(screen);
        SDL_Delay(SPEED);
        
    }


    //CleanRoaches(roaches, MAX_ROACHES, screen, floor);

    SDL_Quit();
    return EXIT_SUCCESS;
}
