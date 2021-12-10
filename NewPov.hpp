#ifndef NEWPOV_HPP
#define NEWPOV_HPP

#include "bmp2matInt.hpp"
#include <string.h>


// Notice: Texture_on: surf_type_ -> TRUE; Texture_off: surf_type_ -> FALSE
bool rot_ = false, trans_ = false, surf_type_ = false, open_ = false;

const char * surface__ = "";

// Povray writer
class Povray : public ofstream{
    protected:
        // ---------- CONFIG VARIABLES ----------------------
        VectorND rotV, traV, cam;
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
        void bicubic(const initializer_list<VectorND>, float=0.01, int=1, int u=4, int v=4);

        void triangle(const VectorND *);

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

void Povray::bicubic(const initializer_list<VectorND> list, float thick, int type, int u, int v) {
    *this << "bicubic_patch {" << endl
          << tab << "type " << type << endl
          << tab << "flatness " << thick << endl
          << tab << "u_steps " << u << endl
          << tab << "v_steps " << v << endl;
    for(int index = 0, eom = 0; index < 16; ++index) {
        *this << list.begin()[index];
        if(eom < 4) ++eom;
        else continue;
        (index%4 == 0) ? *this << ',' << endl: *this << ',';
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
