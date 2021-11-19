#ifndef NEWPOV_HPP
#define NEWPOV_HPP

#include "bmp2matInt.hpp"

// Povray writer
class Povray : public ofstream{
    protected:

        // ----------------
        // CONFIG VARIABLES
        // ---------------- 

        VectorND rotV, traV, cam;
        int  blur_S = 500;
        bool rot_ = false, trans_ = false;
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
        {  *this << endl << "global_settings{ asummed_gamma " << gam_num << " }\n\n";  }

        // Blur samples
        void blurSamples(void)
        {  *this << endl << "blur_samples " << this->blur_S << endl;  }

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

        // ------------------
        // OBJECT CONSTRUCTOR
        // ------------------
        
        void camera(const VectorND& u, const VectorND& v){
            *this << "camera {" << endl
                  << tab << "location " << u << endl
                  << tab << "look_at " << v << endl;
            this->blurSamples();
            (rot_) ? *this << tab << "rotate " << rotV << endl : *this << "";
            (trans_) ? *this << "translate " << traV << endl : *this << "";
            *this << "}";
        }
};

#endif
