#ifndef NEWRAY_HPP
#define NEWRAY_HPP

#include "bmp2matInt.hpp"
#include "Matrix3D.hpp"
#include "VectorND.hpp"
#include "matix.hpp"


class Sphere{
    public:
        double parameters[3];
        
    Sphere(VectorND ty){
        this->parameters[0] = ty.norma();
    
        double miniNorma = sqrt( ty[0]*ty[0] + ty[1]*ty[1] );
        if( ty[2]>0 )
            this->parameters[1] = atan2(miniNorma,ty[2]);
        else if( ty[2]==0 )
            this->parameters[1] = M_PI/2;
        else
            this->parameters[1] = M_PI + atan2(miniNorma,ty[2]);

        double sgnX = ( ty[1]>0 ) ? 1 : -1;
        if( ty[0]>0 && ty[1]>0 )
            this->parameters[2] = atan2(ty[1],ty[0]);
        else if( ty[0]>0 && ty[1]<0 )
            this->parameters[2] = 2*M_PI + atan2(ty[1],ty[0]);
        else if( ty[0]==0 )
            this->parameters[2] = M_PI/2 * sgnX;
        else if( ty[0]<0 )
            this->parameters[2] = M_PI + atan2(ty[1],ty[0]);
    }

    double  operator [] (int index) const
    {  return this->parameters[index%3];  }

    double& operator [] (int index)
    {  return this->parameters[index%3];  } 

    VectorND toVector (void){
        double r = this->parameters[0];
        double th= this->parameters[1];
        double ph= this->parameters[2];

        return r*VectorND{ sin(ph)*cos(th),sin(ph)*sin(th),cos(ph) };
    }
};

const char * dtab = "            ";
const char * tab = "      ";
char surface_type = 'p';

bool agate= false, shadow=true, colorMap= false, bool_rot = false, bool_trans= false, bool_scale= false;
double turbulence=0.0, lambda=0.0, omega=0.0, octaves=0.0, scale_;
 
// -- Finish Varibles ************
bool bool_finish = false;
bool bool_ambient = true, bool_diffuse = true, bool_specular = true, bool_roughness = true;
double ambient=0.1, diffuse=0.1, specular=1, roughness=0.001, reflection=0.2;

VectorND rgb, rot_vec, trans_vec, cam_vec;

double   rCM[4] = {0.0,0.0,0.0,0.0};
VectorND vCM[4] = {VectorND{1,0,0},VectorND{0,1,0},VectorND{0,0,1},VectorND{1,1,0}};

double T0_SE = 0, T_SE;

void setAllFinish(void){
    bool_ambient = true;
    bool_diffuse = true;
    bool_specular= true;
    bool_roughness= true;
}

void offAllFinish(void){
    bool_ambient = false;
    bool_diffuse = false;
    bool_specular= false;
    bool_roughness= false;
}

void resetFinish(void){
    ambient = 0.1;
    diffuse = 0.1;
    specular= 1.0;
    roughness= 0.001;
    reflection= 0.2;
}

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

        void write(const char * mssg)
        {   *this << mssg;   }

        void camera(const VectorND& a,const VectorND& b){
            cam_vec = a;
            *this << "camera {\n"
                  << "     location " << a << endl
                  << "     look_at " << b << endl
                  << "     blur_samples 500 " << endl;
            (bool_rot) ? *this << tab << "rotate " << rot_vec << endl : *this << "";
            (bool_trans) ? *this << "translate " << trans_vec << endl : *this << "";
            *this << "}" << endl;
        }

        void wide_cam(const VectorND& pos, const VectorND& look=O,double chi=0.5*M_PI){
            cam_vec = pos;
            *this << "camera{" << endl
                  << tab << "ultra_wide_angle" << endl
                  << tab << "location " << pos << endl
                  << tab << "look_at " << look << endl
                  << tab << "blur_samples 500" << endl
                  << tab << "right x*image_width/image_height" << endl
                  << tab << "angle " << chi << endl;
            (bool_rot) ? *this << tab << "rotate " << rot_vec << endl : *this << "";
            (bool_trans) ? *this << tab << "translate " << trans_vec << endl : *this << "";
            *this << "}" << endl;
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

        void finish(bool election){
            if( election ){
                *this << tab << "finish { " << endl
                      << dtab << "reflection { " << reflection << "}" <<endl;
                (bool_ambient)   ? *this << dtab << "ambient " << ambient << endl : *this << "";
                (bool_diffuse)   ? *this << dtab << "diffuse " << ambient << endl : *this << "";
                (bool_specular)  ? *this << dtab << "specular " << specular << endl : *this << "";
                (bool_roughness) ? *this << dtab << "roughness " << roughness << endl : *this << "";
                *this << tab << "}";
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
            (bool_finish)? finish(bool_finish) : finish(false);
            (bool_rot)   ? *this << tab << "rotate " << rot_vec << endl : *this << "";
            (bool_trans) ? *this << tab << "translate " << trans_vec << endl : *this << "";
        }

        void edit_rgb (double R, double G,double B){   
            double r = R/256;
            double g = G/256;
            double b = B/256;
            rgb = VectorND{r,g,b};   
        }


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

        void cylinder (VectorND base, VectorND cap, double r, const char * surface){
            *this << "cylinder {" << endl
                  << tab << base << ',' << cap << ',' << r << endl;
            modifier(surface);
            (shadow) ? *this << "" : *this << tab << "no_shadow" << endl;
            *this << '}' << endl;
        }

        //------------TEXCHIDO --------------

        double font_size = 4.0;
        void newBMP (BoolMat& M, VectorND pos, double Compresion,const char * surface, Matrix3D W=Matrix3D(1)){

            double angle = - cam_vec.axisAng('x','z') + VectorND{0,0,1}.axisAng('x','z');
            double theta = cam_vec.axisAng('x','y');

            bool_trans = true;
            bool_rot = true;
            rot_vec = (180/M_PI)*VectorND{theta,angle,0};
            trans_vec = pos;

            Matrix3D N = Matrix3D(180,'z');

            for(int i=0; i<M.getM()-1; ++i){
                for(int j=0; j<M.getN()-1; ++j){
                    VectorND w1 = W*N*( VectorND{double(i),double(j),1}/Compresion );
                    VectorND w2 = W*N*( VectorND{double(i+1),double(j),1}/Compresion );
                    VectorND w3 = W*N*( VectorND{double(i+1),double(j+1),1}/Compresion );
                    VectorND w4 = W*N*( VectorND{double(i),double(j+1),1}/Compresion );
                    if(M[i][j]<365){ 
                        triangle(w1,w2,w3,surface);
                        triangle(w1,w3,w4,surface);
                    }
                }
            }

            bool_rot = false;
            bool_trans = false;
        }

        void renderBMP (BoolMat& M, VectorND pos, double Compresion, double font_fine, const char * surface){
            double x = font_size/M.getN();
            double y = font_size/M.getM();
            double r = x>y ? font_fine*y : font_fine*x;
    
            double angle = - cam_vec.axisAng('x','z') + VectorND{0,0,1}.axisAng('x','z');
            double theta = cam_vec.axisAng('x','y');

            *this << "//---ESTO ES DE TEXCHIDO----" << endl;

            bool_trans = true;
            bool_rot = true;
            rot_vec = (180/M_PI)*VectorND{theta,angle,0};
            trans_vec = pos;
            for(int i=0; i<M.getM()-1; ++i){
                for(int j=0; j<M.getN()-1; ++j){
                    VectorND point{double(i),double(j),1};
                    if(M[i][j]<365){                                          // Se escoge 365 como la mitad de la escala, pues
                        sphere(Matrix3D(180,'z')*point/Compresion,r,surface); // recuerde que solo se desea detectar los negros.
                    }
                }
            }
            bool_rot = false;
            bool_trans = false;
            *this << "//---ACABA TEXCHIDO----" << endl;
        }

        // ---------- SPECIAL OBJECTS --------------

        void checker(VectorND v, double r, const char * c1, const char * c2){
            *this << "plane { " << v << ", " << r << endl
                  << tab << "pigment {" << endl
                  << dtab <<  "checker " << c1 << ", " << c2 << endl;
            (bool_scale) ? *this << dtab <<  "scale " <<  scale_ << endl : *this << "";
            *this << tab << "}" << endl;
            (bool_rot) ? *this << tab << "rotate " << rot_vec << endl : *this << "";
            (bool_trans) ? *this << "translate " << trans_vec << endl : *this << "";
            (bool_finish)? finish(bool_finish) : finish(false);
            *this << "}" << endl;
        }

        void plane (VectorND v, double r, const char * surface){
            *this << "plane { " << v << ", " << r << endl;
            modifier(surface);
            (shadow) ? *this << "" : *this << tab << "no_shadow" << endl;
            *this << '}' << endl;
        }

        void arrow (VectorND base, VectorND point, double width,const char * surface){
            cone(base, 0.85*point, width, width, surface);     
            cone(0.85*point, point, 2*width, 0, surface);
        }

        void arrow (VectorND base, VectorND point, double width1, double width2, const char * surface){
            cone(base, 0.85*point, width1, width1, surface);     
            cone(0.85*point, point, width2, 0, surface);
        }

        void arrow (VectorND base, VectorND point, double width1, double ratio, double Norma=1){
            VectorND Nbase = O;
            VectorND Npoint= point - base;

            bool_trans = true;
            trans_vec = base;
            cone(O, (Norma*ratio)*Npoint, width1, width1, "");     
            cone((Norma*ratio)*Npoint, Norma*Npoint, 2.3*width1, 0, "");
            bool_trans = false;
        }
};

#endif
