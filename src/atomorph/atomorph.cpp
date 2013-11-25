/*
 * See Copyright Notice in atomorph.h
 */
#include "atomorph/atomorph.h"

const char * am_get_version() {
    return "0.1";
}

AM_SCENE::AM_SCENE() {
    atoms      = 0;
    frames     = 0;
    map        = nullptr;
    candidates = nullptr;    
    splines    = nullptr;
}

AM_SCENE::~AM_SCENE() {
    clear();
}

void AM_SCENE::clear(void) {
    if (candidates) {
        delete[] candidates;
        candidates = nullptr;
    }
    if (splines) {
        delete[] splines;
        splines = nullptr;
    }
    if (map) {
        for ( size_t i = 0; i < atoms; ++i) {
            free (map[i]);
        }
        free(map);
        map    = NULL;        
        atoms  = 0;
        frames = 0;        
    }
}

bool AM_SCENE::init(size_t atoms, size_t frames) {
    if (map != nullptr) return false;

    this->atoms  = atoms;
    this->frames = frames;    

    size_t i;
    map = (AM_ATOM **) malloc( atoms * sizeof(AM_ATOM *) );
    if (!map) return false;
    
    for (i = 0 ; i < atoms ; ++i ) {
        map[i] = (AM_ATOM *) malloc( frames * sizeof(AM_ATOM) );
        if (map[i] == nullptr) {
            for(size_t j = 0; j<=i; ++j) {
                free(map[j]);
            }
            free(map);
            return false;
        } 
    }
    
    candidates = new (std::nothrow) std::vector<AM_ATOM>[frames];    
    if (!candidates) {
        clear();
        return false;
    }
    for (i = 0; i < frames; ++i) candidates[i].reserve(atoms);    
    
    splines = new (std::nothrow) glnemo::CRSpline[atoms];    
    if (!splines) {
        clear();
        return false;
    }
    for (i = 0; i < atoms; ++i) splines[i].clearCPoints();
    
    return true;
}

void AM_SCENE::renew_splines() {
    for (size_t i = 0; i < atoms; ++i) {
        splines[i].clearCPoints();
        for (size_t j = 0; j < frames; ++j) {    
            float x = map[i][j].x / 65535.0f;
            float y = map[i][j].y / 65535.0f;
            splines[i].AddSplinePoint(glm::vec3(x, y, 0.0));       
        }    
    }
}

void AM_SCENE::get_xy(size_t atom, float t, float *x, float *y) const {    
    glm::vec3 v = splines[atom].GetInterpolatedSplinePoint(t);
    *x = v.x;
    *y = v.y;    
}

void AM_SCENE::get_rgba(size_t atom, float t, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *a) const {
    size_t cur_frame=static_cast<size_t>(t)*frames;
    cur_frame %= frames;
    size_t next_frame = (cur_frame+1) % frames;
    
    float p = 1.0f/frames;
    float str = 1.0f - (t - p*cur_frame)/p;
    if (str > 1.0f) str = 1.0f;
    if (str < 0.0f) str = 0.0f;

    *r = (str)*map[atom][cur_frame].r + (1.0f-str)*map[atom][next_frame].r;
    *g = (str)*map[atom][cur_frame].g + (1.0f-str)*map[atom][next_frame].g;
    *b = (str)*map[atom][cur_frame].b + (1.0f-str)*map[atom][next_frame].b;
    *a = (str)*map[atom][cur_frame].a + (1.0f-str)*map[atom][next_frame].a;
}

bool AM_SCENE::push_atom(size_t frame, AM_ATOM atom) {
    if (frame >= frames) return false;
    size_t sz_before = candidates[frame].size();
    candidates[frame].push_back(atom);
    if (candidates[frame].size() == sz_before) return false;
    return true;
}

bool AM_SCENE::elect_atoms() {
    for (size_t i=0; i<frames; ++i) {
        if (candidates[i].size() == 0) {
            candidates[i].push_back(am_create_atom(0.5,0.5,0,0,0,0));
        }
        std::random_shuffle( candidates[i].begin(), candidates[i].end() );

        size_t clone=0;
        for (size_t j=candidates[i].size(); (j != 0 && j < atoms); ++j) {
            candidates[i].push_back( candidates[i].at(clone++) );
        }
        if (candidates[i].size() < atoms) return false;
        
        for (size_t a=0; a<atoms; ++a) {
            map[a][i].x = candidates[i].at(a).x;
            map[a][i].y = candidates[i].at(a).y;
            map[a][i].r = candidates[i].at(a).r;
            map[a][i].g = candidates[i].at(a).g;
            map[a][i].b = candidates[i].at(a).b;
            map[a][i].a = candidates[i].at(a).a;                                                            
        }
        candidates[i].clear();
    }
    return true;
}

float AM_SCENE::get_cost() {
    float cost = 0.0;

    for (size_t a=0; a<atoms;  ++a) {        
        for (size_t f=0; f<frames; ++f) {
            size_t nf = (f+1) % frames;             
            cost += am_atom_distance(map[a][f], map[a][nf]);
        }
    }
    return cost;
}

void AM_SCENE::shuffle() {
    size_t f,a;
    std::vector<AM_ATOM> row;
    row.reserve(atoms);
    for (f=0; f<frames; ++f) {
        row.clear();
        for (a=0; a<atoms; ++a) {
            row.push_back(map[a][f]);
        }                
        std::random_shuffle( row.begin(), row.end() );
        for (a=0; a<atoms; ++a) {
            map[a][f] = row[a];
        }
    }
}

AM_ATOM AM_SCENE::get_atom(size_t atom, size_t frame) const {
    return map[atom][frame];
}

bool AM_SCENE::copy_from(const AM_SCENE *scene) {
    size_t atoms  = scene->atom_count();
    size_t frames = scene->frame_count();    
    if (atoms == 0 || frames == 0) return false;
    
    if (atom_count() != atoms || frame_count() != frames) {
        clear();
        if (!init(atoms, frames)) return false;
    }            
    
    for (size_t j=0; j<frames; ++j) {
        for (size_t i=0; i<atoms; ++i) {
            map[i][j] = scene->get_atom(i,j);
        }
    }
    return true;
}

bool AM_SCENE::swap_atoms(size_t frame, size_t atom1, size_t atom2) {
    if (frame >= frames || atom1 >= atoms || atom2 >= atoms) return false;
    if (atom1 == atom2) return true;
    
    AM_ATOM abuf = map[atom1][frame];
    map[atom1][frame] = map[atom2][frame];
    map[atom2][frame] = abuf;
    
    return true;
}

AM_ATOM am_create_atom(float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    uint16_t real_x, real_y;
         if (x >= 1.0) real_x = 65535;
    else if (x <= 0.0) real_x = 0;
    else               real_x = x*65536.0f;

         if (y >= 1.0) real_y = 65535;
    else if (y <= 0.0) real_y = 0;
    else               real_y = y*65536.0f;
    
    AM_ATOM atom;
    atom.x = real_x;
    atom.y = real_y;
    atom.r = r;
    atom.g = g;
    atom.b = b;
    atom.a = a;
    return atom;
}

float am_atom_gradient(AM_ATOM a1, AM_ATOM a2) {
    float gradient = sqrt(pow(abs(a1.r-a2.r)/8.0f, 2.0f) + pow(abs(a1.g-a2.g)/8.0f, 2.0f) + pow(abs(a1.b-a2.b)/8.0f, 2.0f)) / 55.209119491f;
    return (gradient < 0.0f ? 0.0f : (gradient > 1.0f ? 1.0f : gradient));
}

float am_atom_distance(AM_ATOM a1, AM_ATOM a2) {
    float cost  = 0.0f;
    cost  = sqrt(pow(fabs((a1.x - a2.x)/256.0f), 2.0f) + pow(fabs((a1.y - a2.y)/256.0f),2.0f)) / 362.033147696f;
    return (cost < 0.0f ? 0.0f : (cost > 1.0f ? 1.0f : cost));
}

AM_THREAD::AM_THREAD() {
    running      = false;
    signal_stop  = false;
    signal_pause = false;
    paused       = true;
    subcost      = nullptr;
    cost         = 0.0f;    
}

AM_THREAD::~AM_THREAD() {
    pause();
    stop();
    clear();
}

bool AM_THREAD::init(const AM_SCENE *scene, unsigned seed, int step_size, float exponent) {
    if (running && !paused) return false;    
    if (!this->scene.copy_from(scene)) return false;
    
    size_t atoms  = scene->atom_count();
    subcost = new (std::nothrow) float[atoms];    
    if (!subcost) return false;
    
    cost = this->scene.get_cost();    
    for (size_t i = 0; i < atoms; ++i) subcost[i]=0.0;

    std::default_random_engine e(seed);
    e1 = e;
    this->step_size = step_size;
    magic_exponent = exponent;

    return true;
}

bool AM_THREAD::clear() {
    if (running && !paused) return false;
    scene.clear();
    
    if (subcost) {
        delete[] subcost;    
        subcost = nullptr;
    }
    
    return true;
}

float AM_THREAD::chain_length(AM_ATOM a1, AM_ATOM a2, AM_ATOM a3) {
    return am_atom_distance(a1, a2) + am_atom_distance(a2, a3);
}

float AM_THREAD::chain_gradient(AM_ATOM a1, AM_ATOM a2, AM_ATOM a3) {
    return am_atom_gradient(a1, a2) + am_atom_gradient(a2, a3);
}

void AM_THREAD::step() {
    size_t frames = scene.frame_count();
    size_t atoms  = scene.atom_count();

    int max_tries=step_size;
    
    if (frames == 0 || atoms <= 1) return;

    std::uniform_int_distribution<size_t> uniform_dist_atoms (0, atoms  - 1);
    std::uniform_int_distribution<size_t> uniform_dist_frames(0, frames - 1);
    
    float cost_per_atom  = cost / atoms;
    float cost_per_frame = cost_per_atom / frames;

    size_t frame = uniform_dist_frames(e1);
    size_t frame_before = (frame == 0 ? frames-1 : frame-1);
    size_t frame_after  = (frame + 1) % frames;
    size_t atom1 = 0, atom2;
    AM_ATOM a1b,a1c,a1a;
    float chain1b;
    float gradient1b;

    EvalAtom1:
    if (max_tries-- <= 0) return;
    atom1 = uniform_dist_atoms(e1);
    a1b = scene.get_atom(atom1, frame_before);
    a1c = scene.get_atom(atom1, frame);
    a1a = scene.get_atom(atom1, frame_after);        
    chain1b    = chain_length  (a1b,a1c,a1a);
    gradient1b = chain_gradient(a1b,a1c,a1a);

    if (chain1b/2.0 < cost_per_frame) goto EvalAtom1;
    max_tries++;
    Again:
    if (max_tries-- <= 0) return;
    atom2 = uniform_dist_atoms(e1);                              
    if (atom1 == atom2) goto Again;
    
    {    
        AM_ATOM a2b = scene.get_atom(atom2, frame_before);
        AM_ATOM a2c = scene.get_atom(atom2, frame);
        AM_ATOM a2a = scene.get_atom(atom2, frame_after);        
        
        float chain2b    = chain_length(a2b,a2c,a2a);
        float gradient2b = chain_gradient(a2b,a2c,a2a);
        float chain1a    = chain_length(a1b,a2c,a1a);
        float gradient1a = chain_gradient(a1b,a2c,a1a);
        float chain2a    = chain_length(a2b,a1c,a2a);
        float gradient2a = chain_gradient(a2b,a1c,a2a);        
        
        float gain = (chain1b - chain1a) + (chain2b - chain2a);
        
        float c1a3 = pow(chain1a, magic_exponent)*0.99f + 0.01f*gradient1a;
        float c2a3 = pow(chain2a, magic_exponent)*0.99f + 0.01f*gradient2a;
        float c1b3 = pow(chain1b, magic_exponent)*0.99f + 0.01f*gradient1b;
        float c2b3 = pow(chain2b, magic_exponent)*0.99f + 0.01f*gradient2b;
        
        if ((c1a3 + c2a3) < (c1b3 + c2b3)) {
            cost -= gain;
            scene.swap_atoms(frame, atom1, atom2);        
        }
        else goto Again;           
    }
}

void AM_THREAD::run() {
    while (!signal_stop) {
        if (!signal_pause && !paused) {
            step();
        }
        else {
            paused = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(0));
        }
    }
    running      = false;
    signal_stop  = false;
    signal_pause = false;
    paused       = true;    
}

bool AM_THREAD::fetch_scene(AM_SCENE *target) const {
    if (running && !paused) return false;    
    return target->copy_from(&scene);
}


