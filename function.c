#include "roach.h"

SDL_Surface* init(char* imgFond_filename, SDL_Surface**pFond) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    // Load background image
    SDL_Surface* image = SDL_LoadBMP(imgFond_filename);
    if (image == NULL) {
        fprintf(stderr, "ERROR: Can't load image %s: %s\n", imgFond_filename, SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    // Create window and surface
    SDL_Surface* screen = SDL_SetVideoMode(image->w, image->h, 32, SDL_SWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        fprintf(stderr, "ERROR: Can't create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    // Blit background image
    SDL_BlitSurface(image, NULL, screen, NULL);
    
    // Set background image
    *pFond = image;
    return screen;
}

SDL_Surface* loadSprites(char * sprites_filename) {
    SDL_Surface* image = SDL_LoadBMP(sprites_filename);
    if (image ==  NULL) {
       fprintf(stderr, "ERROR: Can't load image %s: %s\n", sprites_filename, SDL_GetError());
       exit(EXIT_FAILURE);
    }

    SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 255, 255));
    return image;
}

void DrawRoach (Roach roach, SDL_Surface *ecran){
    SDL_Rect rect_src; // Rectangle source
    SDL_Rect rect_dest; // Rectangle destination

    rect_src.x = ( roach.dir % NB_SPRITES_P_LINE ) * ROACH_WIDTH;
    rect_src.y = ( roach.dir / NB_SPRITES_P_LINE ) * ROACH_HEIGHT;
    rect_src.w = ROACH_WIDTH;
    rect_src.h = ROACH_HEIGHT;

    rect_dest.x = roach.x;
    rect_dest.y = roach.y;

    SDL_BlitSurface(roach.sprites, &rect_src, ecran, &rect_dest);
}

int RandInt(int maxVal) {
    return rand() % maxVal;
}

Roach CreateRoach(SDL_Surface *ecran, SDL_Surface *sprites) {
    Roach roach;
    roach.sprites = sprites;
    roach.dir = RandInt(ROACH_ORIENTATIONS);
    roach.x = RandInt(ecran->w - ROACH_WIDTH);
    roach.y = RandInt(ecran->h - ROACH_HEIGHT);
    roach.hidden = 0;
    roach.steps = RandInt(MAX_STEPS);
    roach.angle = roach.dir * ROACH_ANGLE / 180.0 * M_PI;
    roach.turnLeft = RandInt(2);
    return roach;
}

int RoachInRect(int x, int y, int rectx, int recty, int rectwidth, int rectheight){
    if (x < rectx) return 0;
    if ((x + ROACH_WIDTH) > (rectx + rectwidth)) return 0;
    if (y < recty) return 0;
    if ((y + ROACH_HEIGHT) > (recty + rectheight)) return 0;
    return 1;
}

void TurnRoach(Roach *roach) {
    if (roach->turnLeft) {
	   roach->dir += RandInt(3) + 1;
	   if (roach->dir >= ROACH_ORIENTATIONS)
	       roach->dir -= ROACH_ORIENTATIONS;
    } else {
	   roach->dir -= RandInt(3) + 1;
	   if (roach->dir < 0)
	       roach->dir += ROACH_ORIENTATIONS;
    }
    roach->angle = roach->dir * ROACH_ANGLE / 180.0 * M_PI;
}

Roach *CreateRoaches(SDL_Surface *ecran, SDL_Surface *sprites, int nbRoach) {
    Roach* roaches = (Roach*)malloc(nbRoach * sizeof(Roach));
    for(int i = 0; i < nbRoach ; i++){
        roaches[i] = CreateRoach(ecran, sprites);
    }
    return roaches;
}

void DrawRoaches(Roach *roaches, int nbRoach, SDL_Surface *ecran) {
    for(int i = 0; i < nbRoach; i++){
        DrawRoach(roaches[i], ecran);
    }
}

void CleanRoaches( Roach *roaches,int nbRoach, SDL_Surface *ecran, SDL_Surface *floor) {
    free(roaches);
}

int RoachIntersectRect(int x, int y, int rectx, int recty, int rectwidth, int rectheight) {
    if (x >= (rectx + rectwidth)) return 0;
    if ((x + ROACH_WIDTH) <= rectx) return 0;
    if (y >= (recty + rectheight)) return 0;
    if ((y + ROACH_HEIGHT) <= recty) return 0;
    return 1;
}

void MoveRoach(Roach *roaches, int nbRoaches, int index, float roachSpeed, SDL_Surface *ecran){
    Roach *roach = &roaches[index];
    int newX, newY;
    // si le cafard est hidden => return
    newX = roach->x + (int)(roachSpeed * cos(roach->angle));
    newY = roach->y - (int)(roachSpeed * sin(roach->angle));
    
    if (RoachInRect(newX, newY, 0, 0, ecran->w, ecran->h)) {
       for (int i = 0; i < index; i++ ){
	    if (RoachIntersectRect(newX, newY, roaches[i].x, roaches[i].y, ROACH_WIDTH, ROACH_HEIGHT)){	
	        TurnRoach(roach);
	        break;
	    }
	  }
        roach->x = newX;
        roach->y = newY;

        if (roach->steps-- <= 0) {
	    	TurnRoach(roach);
	    	roach->steps = RandInt(MAX_STEPS);
        }
    } else {
        TurnRoach(roach);
    }
}


void MoveRoaches(Roach *roaches, int nbRoach, SDL_Surface *ecran) {
    for(int i = 0; i < nbRoach; i++){
        if(!roaches[i].hidden) MoveRoach(roaches,MAX_ROACHES,i,SPEED, ecran);
    }
}


SDL_Surface *LoadImage (char * img_filename, int x, int y) {
    SDL_Surface* image = SDL_LoadBMP(img_filename);
    if (image ==  NULL) {
       fprintf(stderr, "ERROR: Can't load image %s: %s\n", img_filename, SDL_GetError());
       exit(EXIT_FAILURE);
    }

    // Set the initial position
    SDL_Rect clip_rect = { x, y, image->w, image->h };
    SDL_SetClipRect(image, &clip_rect);
    return image;
}

void DrawImage(SDL_Surface* img, SDL_Surface* ecran) {
    // Blit the image onto the screen
    SDL_BlitSurface(img, NULL, ecran, &(img->clip_rect));
}

/*   Marque les cafards cachés  */
int MarkHiddenRoaches(Roach *roaches,int nbRoaches,SDL_Surface *rect) {
    int i;
    int nVisible = 0;

    for ( i = 0; i < nbRoaches; i++) {
	 if (RoachInRect( roaches[i].x, roaches[i].y, rect->clip_rect.x, rect->clip_rect.y, rect->w, rect->h)) {
	    roaches[i].hidden = 1;
	 } else {
            roaches[i].hidden = 0;
            nVisible++;
        }
    }
    return nVisible;
}


/* Teste si le point est dans le rectangle spécifié   */
int PointInRect(int x, int y, int rectx, int recty,int rectwidth, int rectheight) {
    if (x < rectx) return 0;
    if (x > (rectx + rectwidth)) return 0;
    if (y < recty) return 0;
    if (y > (recty + rectheight)) return 0;
    
    return 1;
}
