/*
 * AtoMorph - Simple Library for Morphing 2D Particle Clouds
 * See Copyright Notice at the end of this file
 */

#ifndef _ATOMORPH_H_
#define _ATOMORPH_H_

#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <future>
#include <random>

#include "spline.h"

typedef struct AM_ATOM {
    uint16_t x;
    uint16_t y;
    unsigned char  r;
    unsigned char  g;
    unsigned char  b;
    unsigned char  a;
} AM_ATOM;

class AM_SCENE {
    public:
    AM_SCENE();
    ~AM_SCENE();
    
    void clear();    
    bool init(size_t atoms, size_t frames);
    bool push_atom(size_t frame, AM_ATOM atom);
    bool elect_atoms();    
    void renew_splines();
    void get_xy(size_t atom, float t, float *x, float *y) const;
    void get_rgba(size_t atom, float t, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) const;
    AM_ATOM get_atom(size_t atom, size_t frame) const;
    size_t atom_count() const  {return atoms;}
    size_t frame_count() const {return frames;}
    bool copy_from(const AM_SCENE *scene);
    void shuffle();
    float get_cost();
    bool swap_atoms(size_t frame, size_t atom1, size_t atom2);
    
    private:
    size_t atoms;
    size_t frames;
    
    std::vector<AM_ATOM> *candidates;
    glnemo::CRSpline     *splines;
    
    AM_ATOM **map;
};

class AM_THREAD {
    public:
    AM_THREAD();
    ~AM_THREAD();
    
    bool clear();
    bool init(const AM_SCENE *scene, unsigned seed, int step_size, float exponent);    
    
    bool is_running() const {return running;}
    bool is_paused()  const {return paused;}

    void stop()   { signal_stop  = true; while(running) std::this_thread::sleep_for(std::chrono::milliseconds(0)); step_thread.join();}
    void start()  { running      = true; step_thread = std::thread(&AM_THREAD::run, this);}
    void pause()  { signal_pause = true; while(!paused) std::this_thread::sleep_for(std::chrono::milliseconds(0)); signal_pause = false;}
    void resume() { paused       = false;}    
    
    float get_cost() const {return cost;}
    bool   fetch_scene(AM_SCENE *target) const;
    
    private:
    void run();
    void step();
    float chain_length(AM_ATOM a1, AM_ATOM a2, AM_ATOM a3);
    float chain_gradient(AM_ATOM a1, AM_ATOM a2, AM_ATOM a3);
    AM_SCENE scene;
    float *subcost;
    float  cost;
    std::default_random_engine e1;
    int step_size;
    float magic_exponent;
        
    std::atomic<bool> signal_stop;
    std::atomic<bool> running;
    std::atomic<bool> signal_pause;
    std::atomic<bool> paused;
    std::thread step_thread;    
};

AM_ATOM      am_create_atom(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
float       am_atom_distance(AM_ATOM a1, AM_ATOM a2);
float       am_atom_gradient(AM_ATOM a1, AM_ATOM a2);
const char * am_get_version();
#endif

/*
Original code by Erich Erstu

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/
