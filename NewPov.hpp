#ifndef NEWPOV_HPP
#define NEWPOV_HPP

#include "bmp2matInt.hpp"


// Notice: Texture_on: surf_type_ -> TRUE; Texture_off: surf_type_ -> FALSE
bool rot_ = false, trans_ = false, surf_type_ = false, open_ = false;

// Povray writer
class Povray : public ofstream{
    protected:

        // ----------------
        // CONFIG VARIABLES
        // ---------------- 

        VectorND rotV, traV, cam;
        int  blur_S = 500;
        const char* tab = "\t";

        // -----------------------
        // POVRAY CONFIG FUNCTIONS
        // -----------------------
        
        // Add new povray lib. Just write lib
        // name quoting.
        void addLib(const char * libName)
        {  *this << "#include \"" << libName << "\"" << endl;  }

        // Gamma value (1-2.2)
        void assumedGamma(float gam_num)
        {  *this << endl << "global_settings{ assumed_gamma " << gam_num << " }\n\n";  }

        // Blur samples
        void blurSamples(void)
        {  *this << endl << tab << "blur_samples " << this->blur_S << endl;  }

        // Write down function
        void write(const char * mssg)
        {  *this << mssg;  }



    public:
        // Contructor: Most general constructor. Assumed gamma
        // set in 2.2.
        Povray(const char * name) : ofstream(name){
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
        void declareNew(const char * file_name){
            ifstream file (file_name); 
            char * line = (char*) malloc (0*sizeof(char));
            while( file.peek()!=EOF )
                *this << char(file.get());
            *this << endl;
        }

        // -------------------------------------------------------------------
        // ------------------------- OBJECTS ---------------------------------
        // -------------------------------------------------------------------

        // -------------
        // SETUP OBJECTS
        // -------------
        
        void camera(const VectorND& u, const VectorND& v){
            *this << "camera {" << endl
                  << tab << "location " << u << endl
                  << tab << "look_at " << v;
            this->blurSamples();
            this->setGeoMod();
            *this << "}" << endl;
        }

        void light(const VectorND& pos, const VectorND& rgb) {
            *this << "light_source { " << endl
                  << tab << pos << ',' << rgb;
            this->setGeoMod();
            *this << "}" << endl;
        }

        // -----------------
        // GRAPHICAL OBJECTS
        // -----------------

        void sphere(const VectorND& pos, double r) {
            *this << "sphere {" << endl
                  << tab << pos << ',' << r;
            this->setGeoMod();
            *this << "}" << endl;
        }

        void cone(const VectorND& base, const VectorND& end, double r_bas, double r_end) {
            *this << "cone { " << endl
                  << tab 
                  << base << ',' << r_bas << ',' 
                  << end << ',' 
                  << end << ',' << r_end;
            (open_) ? *this << tab << "open" << endl : *this << "";
            this->setGeoMod();
            *this << "}" << endl;
        }

        void cylinder(const VectorND& base, const VectorND& end, double r) {
            *this << "cylinder {" << endl
                  << tab << base << ',' << endl
                  << tab << end << ',' << endl
                  << tab << r << endl;
           (open_) ? *this << tab << "open" : *this << "";
            this->setGeoMod();
           *this << "}" << endl; 
        }

        // -------------------------------------------------------------------
        // ------------------------- MODIFIERS -------------------------------
        // -------------------------------------------------------------------

        // ---------------------
        // GEOMETRICAL MODIFIERS
        // ---------------------

        void setGeoMod(void) {
            (rot_ || trans_) ? *this << endl : *this << "";
            (rot_) ? *this << tab << "rotate " << rotV << endl : *this << "";
            (trans_) ? *this << "translate " << traV << endl : *this << "";
            (rot_ || trans_) ? *this << endl : *this << "";
        }

        // -----------------
        // SURFACE MODIFIERS
        // -----------------

        void setSurMod(void) {
            const char * surface = (surf_type_) ? "texture { " : "pigment { ";
             
        }
};

#endif
