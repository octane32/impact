// IMPACT

#ifndef IMP_H
#define IMP_H

#include "util.h"

struct Faction {
  char key;
  str biome; };

struct Zone;
struct Border {
  vec<P2> pv;
  vec<Zone*> zones; };
struct Zone {
  vec<Border*> borders; };

//d
Faction Factions[32];
str Biomes[32] = {"RUINS", "FOREST", "DESERT", "VOLCANIC",       // A-D
                  "FARMLAND", "JUNGLE", "WOODS", "ARCTIC",       // E-H
                  "WASTELAND", "DUNGEON", "SWAMP", "WONDERLAND", // I-L
                  "DYSTOPIA", "HELL", "DUNES", "CANYON",         // M-P
                  "GROVE", "MOUNTAINS", "BEYOND", "CAVERNS",     // Q-T
                  "OCEANIC", "KINGDOM", "PLAINS", "URBAN",       // U-X
                  "VOID", "TERRAFORMED", "COASTAL", "LAKES",     // Y-1
                  "LUNAR", "TUNDRA", "HILLS", "ARCHIPELAGO"};    // 2-5

vec<Border> Borders; // No order <Connecting order>
vec<Zone> Zones; // Zone center distance to map center order
                 // <Connecting order, begin connects to end>

void init_Factions(){ char c; int i;
  for(i = 0, c = 'A'; c <= 'Z'; ++i, ++c) Factions[i].key = c;
  for(c = '0'; c <= '5'; ++i, ++c) Factions[i].key = c;
  for(i = 0; i < 32; ++i) Factions[i].biome = Biomes[i]; }

void drawU(){

}

void drawR(){

}

void init_Zones(){
  drawU();
  drawR(); }

void init_Impact(){
  init_Factions();
  init_Zones(); }

void init_client(){
  init_Impact(); }

void update_client(C3*** img_ptr){}

#endif
