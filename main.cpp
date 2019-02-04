#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include <iostream>
#include <vector>
#include <math.h>
#include <stdlib.h> 
#include <time.h>   

#define __PI 3.14159265358979323846
#define __E	 2.71828

#define FPS					15.0

#define LENGTH_NONE			0
#define LENGTH_SHORT		25
#define LENGTH_MEDIUM		50
#define LENGTH_LONG			100

#define HILL_NONE			0
#define HILL_LOW			20
#define HILL_MEDIUM			40
#define HILL_HIGH			60

#define CURVE_NONE			0
#define CURVE_EASY			2
#define CURVE_MEDIUM		4
#define CURVE_HARD			6
      

void drawFilledTrapezium(SDL_Renderer* renderer, int poly[4][2], const SDL_Color color) {
	int 	dy, dx, x1, x2;
	float 	slopeLeft, slopeRight, cteLeft, cteRight;
	// --
	dx = poly[3][0] - poly[0][0];
	dy = poly[3][1] - poly[0][1];
	// std::cout << "dy: " << dy << " dx: " << dx << std::endl;
	if (dy == 0) slopeLeft = 1.0f; else
		if (dx == 0)  slopeLeft = 0.0f; else
			slopeLeft = (float)(dy)/(float)(dx);
	cteLeft = poly[0][1] - slopeLeft * poly[0][0];
	dx = poly[2][0] - poly[1][0];
	dy = poly[2][1] - poly[1][1];
	// std::cout << "dy: " << dy << " dx: " << dx << std::endl;
	if (dy == 0) slopeRight = 1.0; else
		if (dx == 0)  slopeRight = 0.0; else
			slopeRight = (float)(dy)/(float)(dx);
	cteRight = poly[1][1] - slopeRight * poly[1][0];
	// std::cout << "dy/dx: " << slopeLeft << " dy/dx: " << slopeRight << std::endl;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);		
	for (int i = poly[0][1]; i < poly[3][1]; i++) {
		if (slopeLeft == 0.0)  x1 = poly[0][0]; else x1 = (i - cteLeft)/slopeLeft;
		if (slopeRight == 0.0) x2 = poly[1][0]; else x2 = (i - cteRight)/slopeRight;
		SDL_RenderDrawLine(renderer, x1, i, x2, i);
		//std::cout << "y: " << i << "x1: " << x1 << " x2: " << x2 << std::endl;
	}
}

bool collision(float x1, float w1, float x2, float w2, float percent) {
    float 	half = percent / 2.0,
    		min1 = x1 - (w1 * half),
    		max1 = x1 + (w1 * half),
    		min2 = x2 - (w2 * half),
    		max2 = x2 + (w2 * half);
    return !((max1 < min2) || (min1 > max2));
  }

// -------------------------------------------------------------------------------------------------

#include <sstream>
#define SSTR( x ) static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << x ) ).str()

class spriteFont {
	public:
		spriteFont(SDL_Renderer* renderer, const char * filename, int sideX, int sideY);
		~spriteFont();
		print(SDL_Renderer* renderer, int x, int y, int w, int h, std::string text);
	private:
		SDL_Texture * texture = NULL;
		int sideX, sideY;
};

spriteFont::spriteFont(SDL_Renderer* renderer, const char * filename, int sideX, int sideY) {
    SDL_Surface * spriteSheet = IMG_Load(filename);
	this->texture = SDL_CreateTextureFromSurface(renderer, spriteSheet); 
	SDL_FreeSurface(spriteSheet);
	this->sideX = sideX;
	this->sideY = sideY;
}

spriteFont::~spriteFont() {
	if (this->texture != NULL) {
		SDL_DestroyTexture(this->texture);
		this->texture = NULL;
	}
}
  
spriteFont::print(SDL_Renderer* renderer, int x, int y, int w, int h, std::string text) {
	SDL_Rect sourceRect, dstRect;
	char c;
	for (int i = 0; i < text.length(); ++i) {
		dstRect.x = x + i * w;
		dstRect.y = y;
		dstRect.w = w;
		dstRect.h = h;

		c =	text.at(i);

		if ((c >= '0') && (c <= '9'))
			sourceRect.x = (c - 48) * this->sideX;
		else
			if ((c >= 'A') && (c <= 'Z'))
				sourceRect.x = (this->sideX * 11) + (c - 65) * (this->sideX);
			else
				sourceRect.x = this->sideX * 40;
		
		sourceRect.y = 0;
		sourceRect.w = this->sideX;
		sourceRect.h = this->sideY;
		
		SDL_RenderCopy(renderer, this->texture, &sourceRect, &dstRect);		
	}		
}

// -------------------------------------------------------------------------------------------------

const SDL_Rect PALM_TREE_SPRITE        	 	 = { .x =    5, .y =    5, .w =  215, .h =  540 };
const SDL_Rect BILLBOARD08_SPRITE      		 = { .x =  230, .y =    5, .w =  385, .h =  265 };
const SDL_Rect TREE1_SPRITE            		 = { .x =  625, .y =    5, .w =  360, .h =  360 };
const SDL_Rect DEAD_TREE1_SPRITE      		 = { .x =    5, .y =  555, .w =  135, .h =  332 };
const SDL_Rect BILLBOARD09_SPRITE      		 = { .x =  150, .y =  555, .w =  328, .h =  282 };
const SDL_Rect BOULDER3_SPRITE 	       		 = { .x =  230, .y =  280, .w =  320, .h =  220 };
const SDL_Rect COLUMN_SPRITE           		 = { .x =  995, .y =    5, .w =  200, .h =  315 };
const SDL_Rect BILLBOARD01_SPRITE      		 = { .x =  625, .y =  375, .w =  300, .h =  170 };
const SDL_Rect BILLBOARD06_SPRITE      		 = { .x =  488, .y =  555, .w =  298, .h =  190 };
const SDL_Rect BILLBOARD05_SPRITE      		 = { .x =    5, .y =  897, .w =  298, .h =  190 };
const SDL_Rect BILLBOARD07_SPRITE      		 = { .x =  313, .y =  897, .w =  298, .h =  190 };
const SDL_Rect BOULDER2_SPRITE         		 = { .x =  621, .y =  897, .w =  298, .h =  140 };
const SDL_Rect TREE2_SPRITE            		 = { .x = 1205, .y =    5, .w =  282, .h =  295 };
const SDL_Rect BILLBOARD04_SPRITE      		 = { .x = 1205, .y =  310, .w =  268, .h =  170 };
const SDL_Rect DEAD_TREE2_SPRITE       		 = { .x = 1205, .y =  490, .w =  150, .h =  260 };
const SDL_Rect BOULDER1_SPRITE         		 = { .x = 1205, .y =  760, .w =  168, .h =  248 };
const SDL_Rect BUSH1_SPRITE                  = { .x =    5, .y = 1097, .w =  240, .h =  155 };
const SDL_Rect CACTUS_SPRITE                 = { .x =  929, .y =  897, .w =  235, .h =  118 };
const SDL_Rect BUSH2_SPRITE                  = { .x =  255, .y = 1097, .w =  232, .h =  152 };
const SDL_Rect BILLBOARD03_SPRITE            = { .x =    5, .y = 1262, .w =  230, .h =  220 };
const SDL_Rect BILLBOARD02_SPRITE            = { .x =  245, .y = 1262, .w =  215, .h =  220 };
const SDL_Rect STUMP_SPRITE                  = { .x =  995, .y =  330, .w =  195, .h =  140 };
const SDL_Rect SEMI_SPRITE                   = { .x = 1365, .y =  490, .w =  122, .h =  144 };
const SDL_Rect TRUCK_SPRITE                  = { .x = 1365, .y =  644, .w =  100, .h =   78 };
const SDL_Rect CAR03_SPRITE                  = { .x = 1383, .y =  760, .w =   88, .h =   55 };
const SDL_Rect CAR02_SPRITE                  = { .x = 1383, .y =  825, .w =   80, .h =   59 };
const SDL_Rect CAR04_SPRITE                  = { .x = 1383, .y =  894, .w =   80, .h =   57 };
const SDL_Rect CAR01_SPRITE                  = { .x = 1205, .y = 1018, .w =   80, .h =   56 };
const SDL_Rect PLAYER_UPHILL_LEFT_SPRITE     = { .x = 1383, .y =  961, .w =   80, .h =   45 };
const SDL_Rect PLAYER_UPHILL_STRAIGHT_SPRITE = { .x = 1295, .y = 1018, .w =   80, .h =   45 };
const SDL_Rect PLAYER_UPHILL_RIGHT_SPRITE    = { .x = 1385, .y = 1018, .w =   80, .h =   45 };
const SDL_Rect PLAYER_LEFT_SPRITE      		 = { .x =  995, .y =  480, .w =   80, .h =   41 };
const SDL_Rect PLAYER_STRAIGHT_SPRITE  		 = { .x = 1085, .y =  480, .w =   80, .h =   41 };
const SDL_Rect PLAYER_RIGHT_SPRITE     		 = { .x =  995, .y =  531, .w =   80, .h =   41 };
const SDL_Rect BACKGROUND_HILLS				 = { .x =    5, .y =    5, .w = 1280, .h =  480 };
const SDL_Rect BACKGROUND_SKY				 = { .x =    5, .y =  495, .w = 1280, .h =  480 };
const SDL_Rect BACKGROUND_TREES				 = { .x =    5, .y =  985, .w = 1280, .h =  480 };

class Sprite {
	public:
		SDL_Rect 	spriteRect;
		float		x_offset;
};

class Car {
	public:
		SDL_Rect 	spriteRect;
		float		x_offset;
		int			z_offset; // Change segment because is moving....
		int 		speed;
		bool		moved;	
};

float scaleSprites = 0.3 * (1.0 / (float)PLAYER_STRAIGHT_SPRITE.w);

// -------------------------------------------------------------------------------------------------

const SDL_Color WHITE_COLOR 		= {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};
const SDL_Color SKY_COLOR 			= {.r = 0x72, .g = 0xD7, .b = 0xEE, .a = 0xFF};
const SDL_Color TREE_COLOR 			= {.r = 0x00, .g = 0x51, .b = 0x08, .a = 0xFF};
const SDL_Color FOG_COLOR 			= {.r = 0x00, .g = 0x51, .b = 0x08, .a = 0xFF};
const SDL_Color LIGHT_ROAD_COLOR 	= {.r = 0x6B, .g = 0x6B, .b = 0x6B, .a = 0xFF};
const SDL_Color DARK_ROAD_COLOR 	= {.r = 0x69, .g = 0x69, .b = 0x69, .a = 0xFF};
const SDL_Color LIGHT_GRASS_COLOR 	= {.r = 0x10, .g = 0xAA, .b = 0x10, .a = 0xFF};
const SDL_Color DARK_GRASS_COLOR 	= {.r = 0x00, .g = 0x9A, .b = 0x00, .a = 0xFF};
const SDL_Color LIGHT_RUMBLE_COLOR 	= {.r = 0xDD, .g = 0xDD, .b = 0xDD, .a = 0xFF};
const SDL_Color DARK_RUMBLE_COLOR 	= {.r = 0xBB, .g = 0x00, .b = 0x00, .a = 0xFF};
const SDL_Color LIGHT_LANE_COLOR 	= {.r = 0xCC, .g = 0xCC, .b = 0xCC, .a = 0xFF};
const SDL_Color DARK_LANE_COLOR 	= {.r = 0xCC, .g = 0xCC, .b = 0xCC, .a = 0x00};	  
	  
int 	SCREEN_WIDTH    = 1024,		
		SCREEN_HEIGHT   = 768,
		totalCars      	= 200,                     // total number of cars on the road
		fogDensity     	= 5,                       // exponential fog density
		fieldOfView    	= 100,                     // angle (degrees) for field of view
		segmentLength	= 200,
		rumbleLength 	= 3,
		roadWidth       = 2000,                    // actually half the roads width, easier math if the road spans from -roadWidth to +roadWidth
		cameraHeight    = 1000,                    // z height of camera
		numLanes		= 3,					   // Number of lanes on road
		accel           = 2400,					   // acceleration rate - tuned until it 'felt' right
		decel           = -2400,   				   // 'natural' deceleration rate when neither accelerating, nor braking
		breaking        = -12000,                  // deceleration rate when braking
		maxSpeed		= 12000,				   // top speed (ensure we can't move more than 1 segment in a single frame to make collision detection easier)
		offRoadLimit   	=  maxSpeed / 4,		   // limit when off road deceleration no longer applies (e.g. you can always go at least this speed even when off road) 		
		offRoadDecel    = -maxSpeed / 2,	   	   // off road deceleration is somewhere in between		
		trackLength,
		drawDistance 	= 300;                     // number of segments to draw

float 	cameraDepth		= 1.0 / tan(((float)fieldOfView / 2.0) * __PI/180.0),
		resolution 		= SCREEN_HEIGHT / 480.0,
		playerX			= 0, 
	  	playerZ			= (cameraHeight * cameraDepth),
		centrifugal    	= 0.3,	
		dt 				= 1.0 / FPS,			    // Period of time between frames = (1 / frames per second)
    	skySpeed       	= 0.001,                    // background sky layer scroll speed when going around curve (or up hill)
    	hillSpeed      	= 0.002,                    // background hill layer scroll speed when going around curve (or up hill)
    	treeSpeed      	= 0.003;                    // background tree layer scroll speed when going around curve (or up hill)		
    	
SDL_Rect playerSprite;
    	
class Segment {
	public:
		int index;
		int curve;
		SDL_Color colorRoad, colorGrass, colorRumble, colorLane;
		float p1worldY , p1worldX  , p1worldZ ;
		float p1cameraY, p1cameraX , p1cameraZ;
		float p1screenY, p1screenX , p1screenW;
		float p2worldY , p2worldX  , p2worldZ ;
		float p2cameraY, p2cameraX , p2cameraZ;
		float p2screenY, p2screenX , p2screenW;
		bool  looped;
		float fog;
		int   clip;
		std::vector<Sprite> sprites;
		std::vector<Car> cars;		
};
	
std::vector<Segment> segments;	

Segment findSegment(int value) {
	return segments[(value/segmentLength) % segments.size()]; 
}
	
void addSegment(int curve, float y) {
	Segment segment;
	// --
	segment.p1worldX  = 0.0;
	segment.p1cameraY = 0.0;
	segment.p1cameraX = 0.0;
	segment.p1cameraZ = 0.0;
	segment.p1screenY = 0.0;
	segment.p1screenX = 0.0;
	segment.p1screenW = 0.0;
	segment.p2worldX  = 0.0;
	segment.p2cameraY = 0.0;
	segment.p2cameraX = 0.0;
	segment.p2cameraZ = 0.0;
	segment.p2screenY = 0.0;
	segment.p2screenX = 0.0;
	segment.p2screenW = 0.0;	
	// --
	segment.index = segments.size();
	segment.curve = curve;
	if (segments.size() == 0)
		segment.p1worldY = 0;
	else
		segment.p1worldY = ((Segment)segments.back()).p2worldY;
	segment.p1worldZ = segment.index * segmentLength;
	segment.p2worldY = y;
	segment.p2worldZ = (segment.index+1) * segmentLength;
	if ( (segment.index == 2) || (segment.index == 3) ) {
		segment.colorRoad	= WHITE_COLOR;
		segment.colorGrass	= WHITE_COLOR;
		segment.colorRumble	= WHITE_COLOR;
		segment.colorLane	= WHITE_COLOR;
	} else 
		if ((segment.index / rumbleLength)%2 == true) {
			segment.colorRoad	= LIGHT_ROAD_COLOR;
			segment.colorGrass	= LIGHT_GRASS_COLOR;
			segment.colorRumble	= LIGHT_RUMBLE_COLOR;
			segment.colorLane	= LIGHT_LANE_COLOR;
		} else {
			segment.colorRoad	= DARK_ROAD_COLOR;
			segment.colorGrass	= DARK_GRASS_COLOR;
			segment.colorRumble = DARK_RUMBLE_COLOR;
			segment.colorLane	= DARK_LANE_COLOR;
		}
	segments.push_back(segment); 
}	
	
void addRoad(int numSegmentsEnter, int numSegmentsHold, int numSegmentsLeave, int curve, int y) {
	float startY;
	if (segments.size() == 0)
		startY = 0;
	else 	
		startY = ((Segment)segments.back()).p2worldY;
	float endY     = startY + (float)(y) * (float)(segmentLength);
	// --
	for (int i = 0; i < numSegmentsEnter; i++)
		addSegment (curve * (i / numSegmentsEnter) * (i / numSegmentsEnter), 
					startY + (endY - startY) * ((-cos(((float)i / (float)(numSegmentsEnter+numSegmentsHold+numSegmentsLeave))*__PI) / 2.0) + 0.5));
	for (int i = 0; i < numSegmentsHold; i++)
		addSegment (curve,
					startY + (endY - startY) * ((-cos(((float)(numSegmentsEnter + i) / (float)(numSegmentsEnter+numSegmentsHold+numSegmentsLeave))*__PI) / 2.0) + 0.5));
	for (int i = 0; i < numSegmentsLeave; i++)
		addSegment (curve + (-curve) * ((-cos(((float)i / (float)numSegmentsLeave)*__PI) / 2.0) + 0.5),
					startY + (endY - startY) * ((-cos(((float)(numSegmentsEnter + numSegmentsHold + i) / (float)(numSegmentsEnter+numSegmentsHold+numSegmentsLeave))*__PI) / 2.0) + 0.5));
}

void addStraight(int length) {
    addRoad(length, length, length, 0, 0);
}

void addHill(int length, int height) {
    addRoad(length, length, length, 0, height);
}

void addCurve(int length, int curve, int height) {
    addRoad(length, length, length, curve, height);
}
        
void addLowRollingHills(int length, int height) {
	addRoad(length, length, length,  0,          height/2);
    addRoad(length, length, length,  0,         -height);
    addRoad(length, length, length,  CURVE_EASY, height);
    addRoad(length, length, length,  0,          0);
    addRoad(length, length, length, -CURVE_EASY, height/2);
    addRoad(length, length, length,  0,          0);
}

void addSCurves(void) {
	addRoad(LENGTH_MEDIUM, LENGTH_MEDIUM, LENGTH_MEDIUM,  -CURVE_EASY,    HILL_NONE);
	addRoad(LENGTH_MEDIUM, LENGTH_MEDIUM, LENGTH_MEDIUM,   CURVE_MEDIUM,  HILL_MEDIUM);
	addRoad(LENGTH_MEDIUM, LENGTH_MEDIUM, LENGTH_MEDIUM,   CURVE_EASY,   -HILL_LOW);
	addRoad(LENGTH_MEDIUM, LENGTH_MEDIUM, LENGTH_MEDIUM,  -CURVE_EASY,    HILL_MEDIUM);
	addRoad(LENGTH_MEDIUM, LENGTH_MEDIUM, LENGTH_MEDIUM,  -CURVE_MEDIUM, -HILL_MEDIUM);
}
   
void addBumps(void) {
	addRoad(10, 10, 10, 0,  5);
    addRoad(10, 10, 10, 0, -2);
    addRoad(10, 10, 10, 0, -5);
    addRoad(10, 10, 10, 0,  8);
    addRoad(10, 10, 10, 0,  5);
    addRoad(10, 10, 10, 0, -7);
    addRoad(10, 10, 10, 0,  5);
    addRoad(10, 10, 10, 0, -2);
}

void addDownhillToEnd(int length) {
    addRoad(length, length, length, -CURVE_EASY, -(((Segment)segments.back()).p2worldY)/segmentLength);
}
    
// --------------------------------------------------------------------------------------

void resetRoad(void) {
	addStraight(LENGTH_SHORT);
	addLowRollingHills(LENGTH_SHORT, HILL_LOW);
	addSCurves();
	addCurve(LENGTH_MEDIUM, CURVE_MEDIUM, HILL_LOW);
	addBumps();
	addLowRollingHills(LENGTH_SHORT, HILL_LOW);
	addCurve(LENGTH_LONG * 2, CURVE_MEDIUM, HILL_MEDIUM);
	addStraight(LENGTH_MEDIUM);
	addHill(LENGTH_MEDIUM, HILL_HIGH);
	addSCurves();
	addCurve(LENGTH_LONG, -CURVE_MEDIUM, HILL_NONE);
	addHill(LENGTH_LONG, HILL_HIGH);
	addCurve(LENGTH_LONG, CURVE_MEDIUM, -HILL_LOW);
	addBumps();
	addHill(LENGTH_LONG, -HILL_MEDIUM);
	addStraight(LENGTH_MEDIUM);
	addSCurves();
	addDownhillToEnd(200);
	
	trackLength = segments.size() * segmentLength;	
}

#define randomize()			((double) rand() / (double)(RAND_MAX))
#define random(min, max)	({ __typeof__ (min) _min = (min); __typeof__ (max) _max = (max); (rand()%(_max - _min + 1)) + _min; })

SDL_Texture * loadSpriteSheet(SDL_Renderer* renderer, const char * filename) {
    SDL_Surface * spriteSheet = IMG_Load(filename);
   	SDL_Texture * texture 	  = SDL_CreateTextureFromSurface(renderer, spriteSheet);    
   	SDL_FreeSurface(spriteSheet);
   	return texture;
}

void addSprite(int numSegment, SDL_Rect spriteRect, float x_offset) {
	Sprite sprite;
	sprite.spriteRect	= spriteRect;
	sprite.x_offset   	= x_offset;
	if ((numSegment >= 0) && (numSegment < segments.size()))
		segments[numSegment].sprites.push_back(sprite);
}

void resetSprites(void) {
	addSprite(20,  					BILLBOARD07_SPRITE, -1);
	addSprite(40,  					BILLBOARD06_SPRITE, -1);
	addSprite(60, 					BILLBOARD08_SPRITE, -1);
	addSprite(80,  					BILLBOARD09_SPRITE, -1);
	addSprite(100, 					BILLBOARD01_SPRITE, -1);
	addSprite(120, 					BILLBOARD02_SPRITE, -1);
	addSprite(140, 					BILLBOARD03_SPRITE, -1);
	addSprite(160, 					BILLBOARD04_SPRITE, -1);
	addSprite(180, 					BILLBOARD05_SPRITE, -1);
	addSprite(240,                  BILLBOARD07_SPRITE, -1.2);
	addSprite(240,                  BILLBOARD06_SPRITE,  1.2);
	addSprite(segments.size() - 25, BILLBOARD07_SPRITE, -1.2);
	addSprite(segments.size() - 25, BILLBOARD06_SPRITE,  1.2);

	for (int numSegment = 10; numSegment < 200 ; numSegment += 4 + (float)numSegment/100.0) {
		addSprite(numSegment, PALM_TREE_SPRITE, 0.5 + randomize()*0.5);
	    addSprite(numSegment, PALM_TREE_SPRITE,   1 + randomize()*2);
	}
	
	for (int numSegment = 250; numSegment < 1000; numSegment += 5) {
	    addSprite(numSegment							, COLUMN_SPRITE, 1.1);
	    addSprite(numSegment + random(0,5), TREE1_SPRITE, -1 - (randomize() * 2));
	    addSprite(numSegment + random(0,5), TREE2_SPRITE, -1 - (randomize() * 2));
	}
	
	SDL_Rect plants[12] 	= { TREE1_SPRITE, TREE2_SPRITE, DEAD_TREE1_SPRITE, DEAD_TREE2_SPRITE, PALM_TREE_SPRITE, BUSH1_SPRITE, BUSH2_SPRITE, CACTUS_SPRITE, STUMP_SPRITE, BOULDER1_SPRITE, BOULDER2_SPRITE, BOULDER3_SPRITE}; 
	SDL_Rect billboards[9]  = { BILLBOARD01_SPRITE, BILLBOARD02_SPRITE, BILLBOARD03_SPRITE, BILLBOARD04_SPRITE, BILLBOARD05_SPRITE, BILLBOARD06_SPRITE, BILLBOARD07_SPRITE, BILLBOARD08_SPRITE, BILLBOARD09_SPRITE};
	
	for (int numSegment = 200; numSegment < segments.size(); numSegment += 3)
	    addSprite(numSegment, plants[random(0, 11)], random(1,-1) * (2 + randomize() * 5));

	int side;
	for (int numSegment = 1000; numSegment < (segments.size()-50); numSegment += 100) {
	    side = random(1, -1); // Left or Right
		addSprite(numSegment + random(0, 50), billboards[random(0,8)], side);
	    for (int i = 0; i < 20; i++) 
	      	addSprite(numSegment + random(0, 50), plants[random(0, 11)], side * (1.5 + randomize()));
	}
}

void resetCars(void) {
	SDL_Rect cars [6] = {CAR01_SPRITE, CAR02_SPRITE, CAR03_SPRITE, CAR04_SPRITE, SEMI_SPRITE, TRUCK_SPRITE};
	//Segment segment;
	Car car;
	int carType;
	for (int i = 0; i < totalCars; i++) {	
		carType 		= random(0, 5);
		car.x_offset	= randomize() * ((float) (random(0,18) - 9) / 10.0);
		car.z_offset	= randomize() * segments.size() * segmentLength;
		car.speed	 	= maxSpeed / 4.0 + (randomize() * maxSpeed / (carType == 4 ? 4.0 : 2.0));	
		car.spriteRect	= cars[carType];
		car.moved 		= false; 
		segments[(car.z_offset/segmentLength) % segments.size()].cars.push_back(car);
/*		segment = findSegment(car.z_offset);
		segment.cars.push_back(car);
		segments[segment.index]	= segment;*/
	}
}

// --------------------------------------------------------------------------------------

#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

void renderSprite(SDL_Renderer* renderer, int width, int height, float resolution, int roadWidth, SDL_Texture * spriteSheet, SDL_Rect spriteRect, float spriteScale, float X, float Y, float offsetX, float offsetY, int clipY, bool flip) {
	SDL_Rect dstrect;
	dstrect.w	= (spriteRect.w * spriteScale * width / 2.0) * (scaleSprites * roadWidth);
	dstrect.h	= (spriteRect.h * spriteScale * width / 2.0) * (scaleSprites * roadWidth);
	dstrect.x	= X + dstrect.w * offsetX;
	dstrect.y 	= Y + dstrect.h * offsetY;	 
    float clipH = (clipY != 0) ? max(0, dstrect.y + dstrect.h - clipY) : 0;
	// --
	if (clipH < dstrect.h) {
		spriteRect.h -= (spriteRect.h * clipH / dstrect.h);
		dstrect.h -= clipH;
		if (flip == true)
			SDL_RenderCopyEx(renderer, spriteSheet, &spriteRect, &dstrect, NULL, NULL, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopy(renderer, spriteSheet, &spriteRect, &dstrect);
	}
}

void renderPlayer(SDL_Renderer* renderer, int width, int height, float resolution, int roadWidth, SDL_Texture * spriteSheet, float speedPercent, float spriteScale, float X, float Y, float steer, float updown, bool offroad) {
	SDL_Rect spriteRect;	
	float bounce = (1.5 * randomize() * speedPercent * resolution) * ( (random(0, 20) - 10) / 10.0);
	
	if (offroad) bounce *= 5;
	
	if (steer < 0)
     	spriteRect = (updown > 0) ? PLAYER_UPHILL_LEFT_SPRITE : PLAYER_LEFT_SPRITE;
    else if (steer > 0)
      	spriteRect = (updown > 0) ? PLAYER_UPHILL_RIGHT_SPRITE : PLAYER_RIGHT_SPRITE;
    else
      	spriteRect = (updown > 0) ? PLAYER_UPHILL_STRAIGHT_SPRITE : PLAYER_STRAIGHT_SPRITE;

	playerSprite = spriteRect;
    renderSprite(	renderer, 
					width, 
					height, 
					resolution, 
					roadWidth, 
					spriteSheet, 
					spriteRect,
					spriteScale,
					X, 
					Y + bounce, 
					-0.5, 
					-1.0,
					0,
					false
				);	
}
 
void renderBackground(SDL_Renderer* renderer, SDL_Texture * backgrounds, int width, int height, SDL_Rect spriteRect, float rotation, float y_offset)  {
    SDL_Rect srcrect;
    srcrect.x = spriteRect.x + (spriteRect.w * rotation);
    srcrect.y = spriteRect.y;
    srcrect.w = spriteRect.w + spriteRect.x - srcrect.x;//min(spriteRect.w / 2, spriteRect.w - (spriteRect.w * rotation));
    srcrect.h = spriteRect.h;
    
    SDL_Rect dstrect;
	dstrect.x = 0;
    dstrect.y = y_offset;
    dstrect.w = srcrect.w * ((float)width/(float)spriteRect.w) + 1;//width * (srcrect.w / (spriteRect.w / 2));
    dstrect.h = height;

	SDL_RenderCopy(renderer, backgrounds, &srcrect, &dstrect);

 	srcrect.x = spriteRect.x;
   	srcrect.w = spriteRect.w;//(spriteRect.w / 2) - srcrect.w;
   	dstrect.x = dstrect.w - 1;
   	dstrect.w = spriteRect.w * ((float)width/(float)spriteRect.w) + 1;//width - (width * (srcrect.w / (spriteRect.w / 2)));

	SDL_RenderCopy(renderer, backgrounds, &srcrect, &dstrect);
}

void renderSegment(SDL_Renderer* renderer, int width, int numLanes, Segment segment) {
	float 	x1 = segment.p1screenX,
			y1 = segment.p1screenY,
			w1 = segment.p1screenW,
			x2 = segment.p2screenX,
			y2 = segment.p2screenY,
			w2 = segment.p2screenW;	
	
	float 	r1 = w1 / max(6 , 2 * numLanes),
			r2 = w2 / max(6 , 2 * numLanes),
			l1 = w1 / max(32, 8 * numLanes),
			l2 = w2 / max(32, 8 * numLanes);

	int points[4][2];
	
	points[0][0] = 0;  			points[0][1] = y2;
	points[1][0] = width-1; 	points[1][1] = y2;
	points[2][0] = width-1;		points[2][1] = y1;
	points[3][0] = 0;  			points[3][1] = y1;
	
	drawFilledTrapezium(renderer, points, segment.colorGrass);
		
	points[0][0] = x2-w2-r2;	points[0][1] = y2;
	points[1][0] = x2-w2;		points[1][1] = y2;
	points[2][0] = x1-w1;	 	points[2][1] = y1;
	points[3][0] = x1-w1-r1;	points[3][1] = y1;
	
	drawFilledTrapezium(renderer, points, segment.colorRumble);
	
	points[0][0] = x2+w2+r2;	points[0][1] = y2;
	points[1][0] = x2+w2;		points[1][1] = y2;
	points[2][0] = x1+w1;	 	points[2][1] = y1;
	points[3][0] = x1+w1+r1;	points[3][1] = y1;
	
	drawFilledTrapezium(renderer, points, segment.colorRumble);
	
	points[0][0] = x2-w2;		points[0][1] = y2;
	points[1][0] = x2+w2;		points[1][1] = y2;
	points[2][0] = x1+w1;	 	points[2][1] = y1;
	points[3][0] = x1-w1;		points[3][1] = y1;
	
	drawFilledTrapezium(renderer, points, segment.colorRoad);
	
	float 	lane_w1 = w1 * 2 / numLanes,
			lane_w2 = w2 * 2 / numLanes,
			lane_x1 = x1 - w1 + lane_w1,
			lane_x2 = x2 - w2 + lane_w2;

	if (segment.colorLane.a == 0xFF) {
		SDL_SetRenderDrawColor(renderer, segment.colorLane.r, segment.colorLane.g, segment.colorLane.b, segment.colorLane.a);
	
		for (int lane = 1; lane < numLanes; lane_x1 += lane_w1, lane_x2 += lane_w2, lane++) {
			points[0][0] = (lane_x2 - l2 / 2);	points[0][1] = y2;
			points[1][0] = (lane_x2 + l2 / 2);	points[1][1] = y2;
			points[2][0] = (lane_x1 + l1 / 2);	points[2][1] = y1;
			points[3][0] = (lane_x1 - l1 / 2);	points[3][1] = y1;

			drawFilledTrapezium(renderer, points, segment.colorLane);
		}
	}

	//SDL_Rect rect = {.x = 0, .y = y1, .w = SCREEN_WIDTH, .h = y2 - y1};
	SDL_Rect rect = {.x = 0, .y = (int)y1, .w = SCREEN_WIDTH, .h = (int)(y2 - y1)};
	
	if (segment.fog < 1) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // To allow alpha blending
		SDL_SetRenderDrawColor(renderer, FOG_COLOR.r, FOG_COLOR.g, FOG_COLOR.b, 255 - segment.fog*255);
		SDL_RenderFillRect(renderer, & rect);
	}
}

void render(SDL_Renderer* renderer, int position, SDL_Texture * spriteSheet, float speed, bool touchLeft, bool touchRight, SDL_Texture * backgrounds, float skyOffset, float hillOffset, float treeOffset) {
	Segment baseSegment   = findSegment(position),
			playerSegment = findSegment(position + playerZ),
			segment;
	float 	basePercent   = (float)(position%segmentLength)/(float)segmentLength,
			playerPercent = (float)( (int)  (position+playerZ)%segmentLength)/(float)segmentLength;
	float 	playerY       = playerSegment.p1worldY + (playerSegment.p2worldY - playerSegment.p1worldY) * playerPercent;
	int		maxy          = SCREEN_HEIGHT;
	int 	x  			  = 0;
	float 	dx 			  = - (baseSegment.curve * basePercent);
	int 	leftRight	  = 0;
	
	renderBackground(renderer, backgrounds, SCREEN_WIDTH, SCREEN_HEIGHT, BACKGROUND_SKY,   skyOffset,  resolution * skySpeed  * playerY);
    renderBackground(renderer, backgrounds, SCREEN_WIDTH, SCREEN_HEIGHT, BACKGROUND_HILLS, hillOffset, resolution * hillSpeed * playerY);
    renderBackground(renderer, backgrounds, SCREEN_WIDTH, SCREEN_HEIGHT, BACKGROUND_TREES, treeOffset, resolution * treeSpeed * playerY);
	
	// Render road
	for (int i = 0; i < drawDistance; i++) {
		segment        = segments[(baseSegment.index + i) % segments.size()];
		segment.looped = ((segment.index < baseSegment.index)? true : false);
		//segment.fog    = 1.0/pow(__E, (float)(i)/(float)(drawDistance) * (float)(i)/(float)(drawDistance) * fogDensity);
		segment.fog    = 1.0/pow((float)__E, (float)(i)/(float)(drawDistance) * (float)(i)/(float)(drawDistance) * (float)fogDensity);
		segment.clip   = maxy;
	
		// Project
    	segment.p1cameraX = segment.p1worldX - ((playerX * roadWidth) - x);
		segment.p1cameraY = segment.p1worldY - (playerY + cameraHeight); 
		segment.p1cameraZ = segment.p1worldZ - (position - (segment.looped ? trackLength : 0));
		segment.p1screenX = round((SCREEN_WIDTH/2)  + ((cameraDepth/segment.p1cameraZ) * segment.p1cameraX  * SCREEN_WIDTH/2));
		segment.p1screenY = round((SCREEN_HEIGHT/2) - ((cameraDepth/segment.p1cameraZ) * segment.p1cameraY  * SCREEN_HEIGHT/2));
		segment.p1screenW = round((((cameraDepth/segment.p1cameraZ))* roadWidth * SCREEN_WIDTH/2));		

		segment.p2cameraX = segment.p2worldX - ((playerX * roadWidth) - x - dx);
		segment.p2cameraY = segment.p2worldY - (playerY + cameraHeight);
		segment.p2cameraZ = segment.p2worldZ - (position - (segment.looped ? trackLength : 0));
		segment.p2screenX = round((SCREEN_WIDTH/2)  + ((cameraDepth/segment.p2cameraZ) * segment.p2cameraX  * SCREEN_WIDTH/2));
		segment.p2screenY = round((SCREEN_HEIGHT/2) - ((cameraDepth/segment.p2cameraZ) * segment.p2cameraY  * SCREEN_HEIGHT/2));
		segment.p2screenW = round((((cameraDepth/segment.p2cameraZ))* roadWidth * SCREEN_WIDTH/2));		

		segments[(baseSegment.index + i) % segments.size()]	= segment;
	
		x  = x + dx;
		dx = dx + segment.curve;

		if ((segment.p1cameraZ <= cameraDepth)       || // behind us
			(segment.p2screenY >= segment.p1screenY) || // back face cull
			(segment.p2screenY >= maxy))                // clip by (already rendered) hill
			continue;
		
		renderSegment(renderer, SCREEN_WIDTH, numLanes, segment);
		
		maxy = segment.p1screenY;
	}

	// Render Sprites and Cars
	for (int i = (drawDistance-1); i > 0; i--) {
        segment        = segments[(baseSegment.index + i) % segments.size()];
	    // Render Cars
		for (int j = 0; j < segment.cars.size(); j++) 
			renderSprite(	renderer, 
							SCREEN_WIDTH, 
							SCREEN_HEIGHT, 
							resolution, 
							roadWidth, 
							spriteSheet, 
							segment.cars[j].spriteRect, 
							(float)cameraDepth/(float)segment.p1cameraZ + ( (float)cameraDepth/(float)segment.p2cameraZ - (float)cameraDepth/(float)segment.p1cameraZ ) * ((float) (segment.cars[j].z_offset%segmentLength)/(float)segmentLength),
							(segment.p1screenX + (segment.p2screenX - segment.p1screenX) * ((float) (segment.cars[j].z_offset%segmentLength)/(float)segmentLength)) + (((float)cameraDepth/(float)segment.p1cameraZ + ( (float)cameraDepth/(float)segment.p2cameraZ - (float)cameraDepth/(float)segment.p1cameraZ ) * ((float) (segment.cars[j].z_offset%segmentLength)/(float)segmentLength)) * segment.cars[j].x_offset * roadWidth * SCREEN_WIDTH/2),
							segment.p1screenY + ((segment.p2screenY - segment.p1screenY)) * ((float) (segment.cars[j].z_offset%segmentLength)/(float)segmentLength),
							-0.5, 
							-1, 
							segment.clip,
							(segment.cars[j].x_offset < playerX ? false : true)
						);
        // Render Sprites
    	for (int j = 0; j < segment.sprites.size(); j++)
  			renderSprite(	renderer, 
			  				SCREEN_WIDTH, 
							SCREEN_HEIGHT, 
							resolution, 
							roadWidth, 
							spriteSheet, 
							segment.sprites[j].spriteRect, 
							(float)cameraDepth/(float)segment.p1cameraZ, 
							segment.p1screenX + (((float)cameraDepth/(float)segment.p1cameraZ) * (segment.sprites[j].x_offset) * ((float)roadWidth) * ((float)SCREEN_WIDTH / 2.0)), 
							segment.p1screenY, 
							(segment.sprites[j].x_offset < 0 ? -1 : 0), 
							-1, 
							segment.clip,
							false
						);
        // Render PlayerCar
        if (touchLeft  == true) leftRight -= 1;
        if (touchRight == true) leftRight += 1;
        if (segment.index == playerSegment.index)
          	renderPlayer(	renderer, 
			  				SCREEN_WIDTH, 
							SCREEN_HEIGHT, 
							resolution, 
							roadWidth, 
							spriteSheet, 
							(float)speed / (float)maxSpeed,
							(float)cameraDepth / (float)playerZ,
                        	SCREEN_WIDTH / 2.0,
							SCREEN_HEIGHT,
							//(SCREEN_HEIGHT / 2.0) - (((float)cameraDepth/(float)playerZ) * (playerSegment.p1cameraY + (playerSegment.p2cameraY - playerSegment.p1cameraY) * playerPercent) * SCREEN_HEIGHT / 2.0),
	                       	speed * leftRight,
							playerSegment.p2worldY - playerSegment.p1worldY,
							((playerX < -1) || (playerX > 1))
						);
   	}
}

// --------------------------------------------------------------------------------------

float updateCarXOffset(int position, int speed, Car car, Segment carSegment) {
	Segment segment, playerSegment = findSegment(position + playerZ);
	float spriteScale;
	Car otherCar;
	float dir;
	
	if ((carSegment.index - playerSegment.index) > drawDistance) {
		return 0.0;
	} else {
		for (int i = 1; i < 40; i++) {
			segment = segments[(carSegment.index + i) % segments.size()];
			spriteScale = (float)cameraDepth/(float)segment.p1cameraZ;
			if ((segment.index == playerSegment.index) && (car.speed > speed) && (collision(playerX, playerSprite.w * spriteScale, car.x_offset, car.spriteRect.w * spriteScale, 1.2))) {
				if (playerX > 0.5)  dir = -1; else 
					if (playerX < -0.5) dir = 1;  else
            			dir = (car.x_offset > playerX) ? 1 : -1;
          		return (float)(dir * (1.0 / i) * ((float)(car.speed - speed) / (float)maxSpeed));
       		}
		    for (int j = 0; j < segment.cars.size(); j++) {
          		otherCar  = segment.cars[j];
          		if ((car.speed > otherCar.speed) && collision(car.x_offset, car.spriteRect.w * spriteScale, otherCar.x_offset, otherCar.spriteRect.w * scaleSprites, 1.2)) {
					if (otherCar.x_offset > 0.5)  dir = -1; else 
						if (otherCar.x_offset < -0.5) dir = 1;  else
              				dir = (car.x_offset > otherCar.x_offset) ? 1 : -1;
            		return (float)(dir * (1.0 / i) * ((float)(car.speed-otherCar.speed)/(float)maxSpeed));
          		}
        	}	
		}	
	    if (car.x_offset < -0.9) return 0.1; else 
			if (car.x_offset > 0.9) return -0.1; else
				return 0.0;
	}
}

void updateCars(int position, int speed) {
	//Segment segment;
	Car car;
	for (int i = 0; i < segments.size(); i++) 
		for (int j = 0; j < segments[i].cars.size(); j++)
			segments[i].cars[j].moved = false;
	for (std::vector<Segment>::iterator itSegment = segments.begin(); itSegment < segments.end(); itSegment++) {
		for (std::vector<Car>::iterator itCar = itSegment->cars.begin(); itCar < itSegment->cars.end(); itCar++) {
			if (itCar->moved == false) {
				car 			= *itCar;
				car.moved 		= true;
				car.x_offset	= car.x_offset + updateCarXOffset(position, speed, car, *itSegment);  		
				car.z_offset    = car.z_offset + dt * car.speed;
				while (car.z_offset >= trackLength) car.z_offset -= trackLength;
				while (car.z_offset < 0) 			car.z_offset += trackLength;
				itSegment->cars.erase(itCar);
				segments[(car.z_offset/segmentLength) % segments.size()].cars.push_back(car);
/*			 	segment = findSegment(car.z_offset);
				segment.cars.push_back(car);
				segments[segment.index]	= segment;*/				
			}
		}
	}
}

void updateBackgrounds(int startPosition, int position, float * skyOffset, float * hillOffset, float * treeOffset) {
	Segment playerSegment = findSegment(position + playerZ);
	
	*skyOffset = *skyOffset + skySpeed * playerSegment.curve * (float)(position-startPosition)/(float)segmentLength;
	while (*skyOffset >= 1) *skyOffset -= 1;
    while (*skyOffset  < 0) *skyOffset += 1;

	*hillOffset = *hillOffset + hillSpeed  * playerSegment.curve * (float)(position-startPosition)/(float)segmentLength;
	while (*hillOffset >= 1) *hillOffset -= 1;
    while (*hillOffset  < 0) *hillOffset += 1;

    *treeOffset = *treeOffset + treeSpeed  * playerSegment.curve * (float)(position-startPosition)/(float)segmentLength;
	while (*treeOffset >= 1) *treeOffset -= 1;
    while (*treeOffset  < 0) *treeOffset += 1;
}

int main(int argc, char** argv) {
	SDL_Event event;
	SDL_DisplayMode displayMode;
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    	std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    	return 1;
  	}

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
    	std::cout << "IMG_Init Error: " << IMG_GetError() << std::endl;
    	return 1;
  	}
  	  	
	SDL_Window *win = SDL_CreateWindow("Prueba", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  	if (win == NULL) {
    	std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    	return 1;
  	}
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  	if (ren == NULL) {
    	std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    	return 1;
  	}

#ifndef _WIN32
	if (SDL_GetCurrentDisplayMode(0, &displayMode) == 0) {
        SCREEN_WIDTH  = displayMode.w;
        SCREEN_HEIGHT = displayMode.h;
    } else {
    	std::cout << "SDL_GetCurrentDisplayMode Error" << std::endl;
    	return -1;
	}
#endif

	srand (time(NULL));
  	
	int speed 		= 0;
	int x, y;
    int startPosition, 
		position 	= 0;
	bool touchUp 	= false, 
		 touchLeft	= false, 
		 touchRight	= false,
		 touchDown	= false;
 	float skyOffset = 0,
	 	  hillOffset= 0, 
		  treeOffset= 0;
	unsigned int lastTime = 0, currentTime;		  

//////////////////////////////////////////////////////////////////////////////////
	spriteFont sFont(ren, "./images/font/speedFont.png", 9, 14);
//////////////////////////////////////////////////////////////////////////////////
		  
	Segment playerSegment;
		  
	resetRoad();
    resetSprites();
	resetCars();
	
	SDL_Texture * spriteSheet = loadSpriteSheet(ren, "sprites.png");
  	if (spriteSheet == NULL) {
    	std::cout << "SpriteSheet not loaded" << std::endl;
    	return 1;
  	}

	SDL_Texture * backgrounds = loadSpriteSheet(ren, "background.png");
  	if (backgrounds == NULL) {
    	std::cout << "backgrounds not loaded" << std::endl;
    	return 1;
  	}
		
    while(1) {
    	    	
		startPosition = position;
			
		position = position + dt * speed;
		while (position >= trackLength) position -= trackLength;
		while (position < 0) position += trackLength;	

		updateCars(position, speed);
		updateBackgrounds(startPosition, position, &skyOffset, &hillOffset, &treeOffset);

		// -- Check keyboard
    	while (SDL_PollEvent(&event) != 0) {
        	if (event.type == SDL_QUIT) 
            	return SDL_TRUE;
        	else if (event.type == SDL_WINDOWEVENT) {
            	//Window resize/orientation change
            	if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)	{
                	//w = e.window.data1; h = e.window.data2;
                	SDL_RenderPresent(ren);
            	}	
        	}
#ifdef _WIN32        	
        	else if (event.type == SDL_KEYDOWN) {
			    switch (event.key.keysym.sym)  {
	        		case SDLK_LEFT:		touchLeft 	= true;		break;
	        		case SDLK_RIGHT:	touchRight 	= true;		break;
	        		case SDLK_UP:		touchUp 	= true;		break;
	        		case SDLK_DOWN:		touchDown 	= true;		break;
	    		}
	    	}
			else if (event.type == SDL_KEYUP) {
			    switch (event.key.keysym.sym)  {
	        		case SDLK_LEFT:		touchLeft 	= false;	break;
	        		case SDLK_RIGHT:  	touchRight	= false;	break;
	        		case SDLK_UP:		touchUp 	= false;	break;
	        		case SDLK_DOWN:		touchDown 	= false;	break;
	    		}	    		
	    	}
#else
			else if (event.type == SDL_FINGERDOWN) {
	            x = (event.tfinger.x * SCREEN_WIDTH);
            	y = (event.tfinger.y * SCREEN_HEIGHT);	
				if ((x > 0) && (x < SCREEN_WIDTH) && (y > 0) && (y < SCREEN_HEIGHT)) { 
                	if (y > 3 * (SCREEN_HEIGHT / 4)) {						
						touchDown = true;
                	} else {		
                		if (x < (SCREEN_WIDTH / 2)) {	
                			touchLeft = true;
						} else {
                			touchRight = true;
						}
                	}
            	}
			}
			else if (event.type == SDL_FINGERUP) {
	            x = (event.tfinger.x * SCREEN_WIDTH);
            	y = (event.tfinger.y * SCREEN_HEIGHT);	
				if ((x > 0) && (x < SCREEN_WIDTH) && (y > 0) && (y < SCREEN_HEIGHT)) { 
                	if (y > 3 * (SCREEN_HEIGHT / 4)) {						
						touchDown = false;
                	} else {		
                		if (x < (SCREEN_WIDTH / 2)) {	
                			touchLeft = false;
						} else {
                			touchRight = false;
						}
                	}
            	}
        	}
#endif			
    	}	

#ifdef _WIN32 						
		if (touchUp 	== true) speed = speed + (accel * dt);
		if (touchDown 	== true) speed = speed + (breaking * dt);
		if ((touchUp 	== false) && (touchDown == false)) speed = speed + (decel * dt);
#else
		if (touchDown == false) 
			speed = speed + (accel * dt);
		else
			speed = speed + (breaking * dt);
#endif
		if (touchLeft 	== true) playerX = playerX - (dt * 2.0 * (float)speed/(float)maxSpeed);
		if (touchRight 	== true) playerX = playerX + (dt * 2.0 * (float)speed/(float)maxSpeed);
		// -- Speed limited
    	if (speed < 0) speed = 0;
    	if (speed > maxSpeed) speed = maxSpeed;
    	// X axis movement limited
    	if (playerX > 3) playerX = 3;
    	if (playerX < -3) playerX = -3;	
    	
    	playerX = playerX - ((dt * 2.0 * (float)speed/(float)maxSpeed) * ((float)speed/(float)maxSpeed) * findSegment(position+playerZ).curve * centrifugal);

		playerSegment = findSegment(position + playerZ);
		
		// Car in offroad X position
		if ((playerX < -1) || (playerX > 1)) {
			// Decelerate to offroad speed
        	if (speed > offRoadLimit)
          		speed = speed + (offRoadDecel * dt);
          	// Check collision with offroad objects
			for (int j = startPosition; j <= position; j++) {
				playerSegment = findSegment(j + playerZ);
	            for (int i = 0; i < playerSegment.sprites.size(); i++) {
	          		if (collision(playerX,  PLAYER_STRAIGHT_SPRITE.w * scaleSprites, playerSegment.sprites[i].x_offset + (playerSegment.sprites[i].spriteRect.w * scaleSprites)/2 * (playerSegment.sprites[i].x_offset > 0 ? 1 : -1), playerSegment.sprites[i].spriteRect.w * scaleSprites, 1.0)) {
	            		speed = maxSpeed / 5;
						position = playerSegment.p1worldZ - playerZ;
						while (position >= trackLength)	 position -= trackLength;
						while (position <  0)	 		 position += trackLength;
	            		break;
	        		}
	        	}
	    	}
      	}
	  	// Check collision with other cars
		for (int j = startPosition; j <= position; j++) {
			playerSegment = findSegment(j + playerZ);
			for (int i = 0; i < playerSegment.cars.size(); i++) {
	        	if (speed > playerSegment.cars[i].speed) {
					if (collision(playerX, PLAYER_STRAIGHT_SPRITE.w * scaleSprites, playerSegment.cars[i].x_offset, playerSegment.cars[i].spriteRect.w * scaleSprites, 0.8)) {
						speed    = playerSegment.cars[i].speed * (playerSegment.cars[i].speed / speed);
						position = playerSegment.cars[i].z_offset - playerZ;
						while (position >= trackLength)	 position -= trackLength;
						while (position <  0)	 		 position += trackLength;
	            		break;            		
	          		}
	        	}
	      	}			
		}
	
		SDL_SetRenderDrawColor(ren, SKY_COLOR.r, SKY_COLOR.g, SKY_COLOR.b, SKY_COLOR.a);	
		SDL_RenderClear(ren);

		render(ren, position, spriteSheet, speed, touchLeft, touchRight, backgrounds, skyOffset, hillOffset, treeOffset);
    
/////////////////////////////////////////////////////////////////////////
    	sFont.print(ren, 100, 100, 120, 120, SSTR(speed/60));
/////////////////////////////////////////////////////////////////////////    
    
		SDL_RenderPresent(ren);
		
		currentTime = SDL_GetTicks() - lastTime;
        if (currentTime < (1000 * dt)) SDL_Delay( (1000 * dt) - currentTime);
        lastTime = SDL_GetTicks();
	
	}

	SDL_DestroyTexture(backgrounds);
	SDL_DestroyTexture(spriteSheet);
	
	SDL_DestroyRenderer(ren);
  	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
  
	return 0;
}

