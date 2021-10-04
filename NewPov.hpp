#ifndef NEWRAY_HPP
#define NEWRAY_HPP

#include "bmp2matInt.hpp"
#include "Matrix3D.hpp"
#include "VectorND.hpp"
#include "matix.hpp"

const char * dtab = "            ";
const char * tab = "      ";
char surface_type = 'p';

bool agate= false, shadow=true, colorMap= false;
double turbulence=0.0, lambda=0.0, omega=0.0, octaves=0.0;
VectorND rgb;

double   rCM[4] = {0.0,0.0,0.0,0.0};
VectorND vCM[4] = {VectorND{1,0,0},VectorND{0,1,0},VectorND{0,0,1},VectorND{1,1,0}};

class Povray : public ofstream{
    public:

        Povray(const char* name) : ofstream(name){
            *this << "#include \"colors.inc\"" << endl
                  << "#include \"shapes.inc\"" << endl
                  << "#include \"textures.inc\"" << endl
                  << "#include \"woods.inc\"" << endl
                  << "#include \"stones.inc\"" << endl
                  << "#include \"glass.inc\"" << endl
                  << "#include \"metals.inc\"" << endl
                  << "#include \"skies.inc\"\n\n"
                  << "global_settings {" << endl
                  << "       assumed_gamma 2.2" << endl
                  << "}" << endl << endl
                  << "#declare thenormal = normal{" << endl
                  << "       crackle" << endl
                  << "       scale 1" << endl
                  << "} \n"
                  << "#declare CloudArea = texture {" << endl
                  << "    pigment {" << endl
                  << "       agate\n"
                  << "       turbulence 1\n"
                  << "       lambda 2\n"
                  << "       frequency 2\n"
                  << "       color_map {\n"
                  << "         [0.0 color rgbf <1,1,1,1>]\n"
                  << "         [0.5 color rgbf <1,1,1,0.35>]\n"
                  << "         [1.0 color rgbf <1,1,1,1>]\n"
                  << "       }\n"
                  << "    }\n"
                  << "}\n\n";
        } 

        void write(char * mssg)
        {   *this << mssg;   }

        void camera(const VectorND& a,const VectorND& b){
            *this << "camera {\n"
                  << "     location " << a << endl
                  << "     look_at " << b << endl
                  << "}" << endl;
        }

        void light(const VectorND& a, const VectorND& b){
            *this << "light_source {\n"
                  << "     " << a << ',' << b << endl
                  << "}\n\n";
        }

        void io_transmitance(int dim, const char * p){
            if(dim!=0){
                dim==3 ? *this << dtab << "color rgb " << rgb : *this << dtab << " color rgbt " << rgb;
                *this << endl;
            }
            else
                *this << dtab << p << endl;
        }

        void object_parameters(const char* p){
            (agate)         ? *this << tab << "agate\n" : *this << "";
            (turbulence==0) ? *this << "" : *this << dtab << "turbulence " << turbulence << endl;
            (lambda==0)     ? *this << "" : *this << dtab << "lambda " << lambda << endl;
            (omega==0)      ? *this << "" : *this << dtab << "omega " << omega << endl;
            (octaves==0)    ? *this << "" : *this << dtab << "octaves " << octaves << endl;
            (p=="")         ? io_transmitance(rgb.dim(),"") : io_transmitance(0,p);        
            if(colorMap){
                *this << "color_map {" << endl;
                for(int index=0; index<4; ++index){
                    *this << dtab << "[" << rCM[index];
                    if(vCM[0].dim()==3)
                        *this << "color rgb " << vCM[index] << "]\n";
                    if(vCM[0].dim()==4)
                        *this << "color rgbt " << vCM[index] << "]\n";
                }
            }
        }

        void modifier (const char * surface){
            if(surface_type=='p'){
                *this << tab << "pigment { " << endl;
                object_parameters(surface);
                *this << tab << "}\n";
            }
            else
                *this << tab << "texture { " << surface << "}\n";
        }

        void edit_rgb (VectorND RGBT)
        {   rgb.copyVector(RGBT);   }


        // ------------ OBJECTS ----------

        void cone (VectorND base, VectorND cap, double r0, double r, const char* surface){
            *this << "cone { " << endl
                  << tab << base << ',' << r0 << ',' 
                         << cap  << ',' << r << endl;
            modifier(surface);
            (shadow) ? *this << "" : *this << tab << "no_shadow" << endl;
            *this << '}' << endl;
        }

        void sphere (VectorND v0, double r0,const char * surface){
            *this << "sphere {" << endl
                  << tab << v0 << ',' << r0 << endl;
            modifier(surface);
            (shadow) ? *this << "" : *this << tab << "no_shadow" << endl;
            *this << '}' << endl;
        }

        void box (VectorND c1, VectorND c2, const char * surface){
            *this << "box {" << endl
                  << tab << c2 << ',' << c2 << endl;
            modifier(surface);
            (shadow) ? *this << "" : *this << tab << "no_shadow" << endl;
            *this << '}' << endl;
        }

        void triangle (VectorND a, VectorND b, VectorND c, const char * surface){
            *this << "triangle {" << endl
                  << tab << a << ',' << b << ',' << c << endl;
            modifier(surface);
            (shadow) ? *this << "" : *this << tab << "no_shadow" << endl;
            *this << '}' << endl;
        }

        //------------TEXCHIDO --------------

        double font_size = 4.0;

        void newBMP (BoolMat& M, VectorND v, double Compresion,const char * surface){
            for(int i=0; i<M.getM()-1; ++i){
                for(int j=0; j<M.getN()-1; ++j){
                    VectorND w1{double(i),double(j),0};
                    VectorND w2{double(i+1),double(j),0};
                    VectorND w3{double(i+1),double(j+1),0};
                    VectorND w4{double(i),double(j+1),0};
                    w1 = w1/Compresion + v;
                    w2 = w2/Compresion + v;
                    w3 = w3/Compresion + v;
                    w4 = w4/Compresion + v;
                    if(M[i][j]<365){ 
                        triangle(w1,w2,w3,surface);
                        triangle(w1,w3,w4,surface);
                    }
                }
            }
        }

        void renderBMP (BoolMat& M, VectorND v, double Compresion, double font_fine, const char * surface){
            double x = font_size/M.getN();
            double y = font_size/M.getM();
            double r = x>y ? font_fine*y : font_fine*x;
            *this << "//---ESTO ES DE TEXCHIDO----" << endl;
            for(int i=0; i<M.getM()-1; ++i){
                for(int j=0; j<M.getN()-1; ++j){
                    VectorND w{double(i),double(j),0};
                    w = w/Compresion;
                    if(M[i][j]<365)                         // Se escoge 365 como la mitad de la escala, pues
                        sphere(w+v,r,surface);              // recuerde que solo se desea detectar los negros.
                }
            }
            *this << "//---ACABA TEXCHIDO----" << endl;
        }

        // ---------- SPECIAL OBJECTS --------------

        void checker(VectorND v, double r, const char * c1, const char * c2){
            *this << "plane { " << v << ", " << r << endl
                  << tab << "pigment {" << endl
                  << dtab <<  "checker " << c1 << ", " << c2 << endl
                  << dtab <<  "scale 2" << endl
                  << tab << "}" << endl
                  << "}" << endl;
        }

        void arrow (VectorND base, VectorND point, double width,const char * surface){
            cone(base, 0.85*point, width, width, surface);     
            cone(0.85*point, point, 2*width, 0, surface);
        }

        void arrow (VectorND base, VectorND point, double width1, double width2, const char * surface){
            cone(base, 0.85*point, width1, width1, surface);     
            cone(0.85*point, point, width2, 0, surface);
        }
};

#endif
