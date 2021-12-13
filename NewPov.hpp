#ifndef NEWPOV_HPP
#define NEWPOV_HPP

#include "bmp2matInt.hpp"
#include <string.h>


// Notice: Texture_on: surf_type_ -> TRUE; Texture_off: surf_type_ -> FALSE
bool rot_ = false, trans_ = false, surf_type_ = false, open_ = false;

VectorND rotV, traV, cam;


const char * surface__ = "";

// Povray writer
class Povray : public ofstream{
    protected:
        // ---------- CONFIG VARIABLES ----------------------
        int  blur_S = 500;
        const char* tab = "\t";

        // ---------- POVRAY CONFIG FUNCTIONS ----------------
        void addLib(const char *);
        void assumedGamma(float);
        void blurSamples(void);
        void write(const char *);

    public:
        Povray(const char *);
        void declareNew(const char *);

        // ------------------------- OBJECTS ---------------------------------
        // SETUP OBJECTS
        void camera(const VectorND&, const VectorND&);
        void light(const VectorND&, const VectorND&);

        // GRAPHICAL OBJECTS
        void sphere(const VectorND&, double);
        void cone(const VectorND&, const VectorND&, double, double);
        void cylinder(const VectorND&, const VectorND&, double);
        void triangle(const VectorND&, const VectorND&, const VectorND&);
        void s_triangle(const VectorND&, const VectorND&, const VectorND&);
        void disc(const VectorND&, const VectorND&, double, double=0);
        void bicubic(VectorND *, float=0.01, int=1, int u=4, int v=4);
        void bicubicSurface(VectorND (*func)(double,double), initializer_list<double>, initializer_list<double>, unsigned int);
        void triangleSurface(VectorND (*func)(double,double), initializer_list<double>, initializer_list<double>, initializer_list<unsigned int>);
        void triangleSurface(VectorND (*func)(double,double,double), initializer_list<double>, initializer_list<double>, initializer_list<unsigned int>,double);

        void triangle(const VectorND *);
        void triangle(initializer_list<VectorND>);

        // ------------- MODIFIERS -------------------------------
        // GEOMETRICAL MODIFIERS
        void setGeoMod(void);

        // SURFACE MODIFIERS
        void setSurMod(void);
};

// Add new povray lib. Just write lib
// name quoting.
void Povray::addLib(const char * libName)
{  *this << "#include \"" << libName << "\"" << endl;  }

// Gamma value (1-2.2)
void Povray::assumedGamma(float gam_num)
{  *this << endl << "global_settings{ assumed_gamma " << gam_num << " }\n\n";  }

// Blur samples
void Povray::blurSamples(void)
{  *this << endl << tab << "blur_samples " << this->blur_S << endl;  }

// Write down function
void Povray::write(const char * mssg)
{  *this << mssg;  }



// Contructor: Most general constructor. Assumed gamma
// set in 2.2.
Povray::Povray(const char * name) : ofstream(name){
    this->addLib("colors.inc");
    this->addLib("shapes.inc");
    this->addLib("textures.inc");
    this->addLib("woods.inc");
    this->addLib("stones.inc");
    this->addLib("glass.inc");
    this->addLib("metals.inc");
    this->addLib("skies.inc");

    this->assumedGamma(2.2);
}

// Function used to pick up a config file to declare 
// new objects: textures, pigment, etc.
void Povray::declareNew(const char * file_name){
    ifstream file (file_name); 
    char * line = (char*) malloc (0*sizeof(char));
    while( file.peek()!=EOF )
        *this << char(file.get());
    *this << endl;
}

// ------------------------- OBJECTS ---------------------------------
// SETUP OBJECTS

void Povray::camera(const VectorND& u, const VectorND& v){
    *this << "camera {" << endl
          << tab << "location " << u << endl
          << tab << "look_at " << v;
    this->blurSamples();
    this->setGeoMod();
    *this << "}" << endl;
}

void Povray::light(const VectorND& pos, const VectorND& rgb) {
    *this << "light_source { " << endl
          << tab << pos << ',' << rgb;
    this->setGeoMod();
    *this << "}" << endl;
}

// GRAPHICAL OBJECTS
void Povray::sphere(const VectorND& pos, double r) {
    *this << "sphere {" << endl
          << tab << pos << ',' << r << endl;
    this->setSurMod();
    this->setGeoMod();
    *this << "}" << endl;
}

void Povray::cone(const VectorND& base, const VectorND& end, double r_bas, double r_end) {
    *this << "cone { " << endl
          << tab 
          << base << ',' << r_bas << ',' 
          << end << ',' 
          << end << ',' << r_end;
    (open_) ? *this << tab << "open" << endl : *this << "";
    this->setSurMod();
    this->setGeoMod();
    *this << "}" << endl;
}

void Povray::cylinder(const VectorND& base, const VectorND& end, double r) {
    *this << "cylinder {" << endl
          << tab << base << ',' << endl
          << tab << end << ',' << endl
          << tab << r << endl;
    (open_) ? *this << tab << "open" : *this << "";
    this->setSurMod();
    this->setGeoMod();
    *this << "}" << endl; 
}

void Povray::triangle(const VectorND& corner1, 
                      const VectorND& corner2,
                      const VectorND& corner3) {
    *this << "triangle {" << endl << tab 
          << corner1 << ',' << corner2 << ',' << corner3 << endl;
    this->setSurMod();
    this->setGeoMod();
    *this << "}" << endl;
}

void Povray::triangle(const VectorND * corner) 
{  this->triangle(corner[0],corner[1],corner[2]);  }

void Povray::triangle(initializer_list<VectorND> list) {
    if(list.size() == 3)
        this->triangle(list.begin()[0],list.begin()[1],list.begin()[2]);   
    else
        cerr << "No se insertaron 3 vectores a la lista." << endl;
}

void Povray::disc(const VectorND& centerPos, 
                  const VectorND& normal, 
                  double radius,
                  double inRadius) {
    *this << "disc { " << endl << tab
          << centerPos << ',' << normal << ',' << radius << ',' << inRadius;
    this->setSurMod();
    this->setGeoMod();
    *this << "}" << endl;
}

void Povray::bicubic(VectorND * list, float thick, int type, int u, int v) {
    *this << "bicubic_patch {" << endl
          << tab << "type " << type << endl
          << tab << "flatness " << thick << endl
          << tab << "u_steps " << u << endl
          << tab << "v_steps " << v << endl << tab;
    for(auto vec = &list[0]; vec != &list[16]+1; ++vec) {
        static unsigned int index = 1;
        (index < 17) ? *this << *vec : *this << "";
        (index < 16) ? *this << ',' : *this << "";
        (index%4==0 && index!=0 && index!=16) ? *this << endl << tab : *this << "";
        ++index;
    }
    *this << endl;
    this->setSurMod();
    this->setGeoMod();
    *this << "}" << endl;
}

void Povray::triangleSurface(VectorND (*func)(double,double), 
                                    initializer_list<double> ux,
                                    initializer_list<double> uy,
                                    initializer_list<unsigned int> width) {
    double dx = abs(ux.begin()[0]-ux.begin()[1]) / width.begin()[0];
    double dy = abs(uy.begin()[0]-uy.begin()[1]) / width.begin()[1];
    for(double x = ux.begin()[0]; x < ux.begin()[1]; x += dx) {
        for(double y = uy.begin()[0]; y < uy.begin()[1]; y += dy) {
            this->triangle({func(x,y),func(x+dx,y),func(x+dx,y+dy)});
            this->triangle({func(x,y),func(x,y+dy),func(x+dx,y+dy)});
        }
    }
} 

void Povray::triangleSurface(VectorND (*func)(double,double,double), 
                                    initializer_list<double> ux,
                                    initializer_list<double> uy,
                                    initializer_list<unsigned int> width,
                                    double t) {
    double dx = abs(ux.begin()[0]-ux.begin()[1]) / width.begin()[0];
    double dy = abs(uy.begin()[0]-uy.begin()[1]) / width.begin()[1];
    for(double x = ux.begin()[0]; x < ux.begin()[1]; x += dx) {
        for(double y = uy.begin()[0]; y < uy.begin()[1]; y += dy) {
            this->triangle({func(x,y,t),func(x+dx,y,t),func(x+dx,y+dy,t)});
            this->triangle({func(x,y,t),func(x,y+dy,t),func(x+dx,y+dy,t)});
        }
    }
} 

void Povray::bicubicSurface(VectorND (*func)(double,double), 
                                    initializer_list<double> ux,
                                    initializer_list<double> uy, 
                                    unsigned int pow) {
    VectorND * vec = (VectorND*) malloc (16 * sizeof(VectorND));
    double dx = abs(ux.begin()[0]-ux.begin()[1]) / (4^pow);
    double dy = abs(uy.begin()[0]-uy.begin()[1]) / (4^pow);
    for(double x = ux.begin()[0]; x < ux.begin()[1]; x += dx) {
        static unsigned int index = 0;
        for(double y = uy.begin()[0]; y < uy.begin()[1]; y += dy) {
            vec[index] = func(x,y);
            ++index;
        }
        if(!(index < 16)) {
            index = 0;
            this->bicubic(vec);
            free(vec);
            vec = (VectorND*) malloc (16 * sizeof(VectorND));
        }
    }
}


// ------------- MODIFIERS -------------------------------
// GEOMETRICAL MODIFIERS

void Povray::setGeoMod(void) {
    (rot_ || trans_) ? *this << endl : *this << "";
    (rot_) ? *this << tab << "rotate " << rotV << endl : *this << "";
    (trans_) ? *this << "translate " << traV << endl : *this << "";
    (rot_ || trans_) ? *this << endl : *this << "";
}

// SURFACE MODIFIERS
void Povray::setSurMod(void) {
    string output;
    const char * type = (surf_type_) ? "\ttexture { " : "\tpigment { ";
    output.append(type) += surface__;
    *this << output.append(" }\n");
}

#endif
