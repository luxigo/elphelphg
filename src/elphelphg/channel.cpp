/*
 * ElphelPHG - Elphel PHG cameras related developement
 *
 * Copyright (c) 2014 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

// Author(s):
//
//     Luc Deschenaux <l.deschenaux@foxel.ch>
//     Stephane Flotron <s.flotron@foxel.ch>
//
//     Parts of the code are coming from imagej-elphel

#include <opencv2/core/matx.hpp>
#include "cameraArray.hpp"
#include "channel.hpp"
#include "imagej_elphel_preferences.hpp"
#include "sensorData.hpp"
#include "eqrData.hpp"
#include "utils.hpp"

#ifndef cimg_version
#define cimg_use_tiff
#define cimg_display 0
#include <CImg.h>
#endif

using namespace cimg_library;
using namespace cv;

namespace elphelphg {

using namespace utils;

Channel::Channel(CameraArray *array, unsigned int num) {
  ImageJ_Elphel_Preferences *prefs=array->prefs;
  if (num>array->channel_list.size()) {
    throw std::string("Channel index out of range: "+utils::to_string(num,2));
  }

  // get sensor data
  this->calibtiff=prefs->calibtiff_dir+"/"+prefs->calibtiff_prefix+utils::to_string(num,2)+prefs->calibtiff_suffix;
  this->sensor=new SensorData((this->calibtiff+".xml").c_str());

  // get eqr data
  this->eqrtiff=prefs->eqrtiff_dir+"/"+prefs->eqrtiff_prefix+utils::to_string(num,2)+prefs->eqrtiff_suffix;
  this->eqr=new EqrData((this->eqrtiff+".xml").c_str());

  this->getRotation();
  this->getLensCenterVector();

  this->K[0]=sensor->focalLength/(sensor->pixelSize/1000.);
  this->K[1]=0;
  this->K[2]=sensor->px0;

  this->K[3]=0;
  this->K[4]=sensor->focalLength/(sensor->pixelSize/1000.);
  this->K[5]=sensor->py0;

  this->K[6]=0;
  this->K[7]=0;
  this->K[8]=1;
}

void Channel::getEQRPrincipalPoint(double *x0, double *y0) {
  if (!this->calibtiff_cimg) {
    this->calibtiff_cimg=new CImg<float>(this->calibtiff.c_str());
  }
  int height=calibtiff_cimg->height();
  int width=calibtiff_cimg->width();
  int x,y;
  int X;
  int Y;
  double dx,dy,dm;
  double DM;

  for(y=0; y<height; ++y) {
    for(x=0; x<width; ++x) {
      dx=calibtiff_cimg[0](x,y,0,0)-sensor->px0;
      dy=calibtiff_cimg[0](x,y,1,0)-sensor->py0;
      dm=dx*dx+dy*dy;
      if (dm<=DM) {
        DM=dm;
        X=x;
        Y=y;
      }
    }
  }
  *x0=X;
  *y0=Y;

  // modif SF 05/2014
  // refine result
  int n(5); // order of interpolation

  double err = sqrt(DM);
  int k(0);
  // patch around interest point
  double sol[2] = {x0[0], y0[0]};

  while( (err > 1.0e-10) && (k < 1000) ){
      double pow = exp(k*log(2));

      // patch center at sol
	  double patch[18] = {
		 sol[0]-1.0/pow , sol[1]-1.0/pow,
		 sol[0]-1.0/pow , sol[1],
		 sol[0]-1.0/pow , sol[1]+1.0/pow,
	     sol[0]         , sol[1]-1.0/pow,
	     sol[0]         , sol[1],
	     sol[0]         , sol[1]+1.0/pow,
	     sol[0]+1.0/pow , sol[1]-1.0/pow,
	     sol[0]+1.0/pow , sol[1],
	     sol[0]+1.0/pow , sol[1]+1.0/pow, };

	  double map[18];

	  // interpolate mapping and compute difference
    if (!this->calibtiff_cimg) {
      this->calibtiff_cimg=new CImg<float>(this->calibtiff.c_str());
    }
	  for(int i(0); i<9; ++i){
		  double value[2] = {0,0};
		  interpolateSubPix(*this->calibtiff_cimg,value,n,patch[2*i],patch[2*i+1]);
		  map[2*i]   = value[0]-sensor->px0;
		  map[2*i+1] = value[1]-sensor->py0;
	  }

	  double error(1.0e10);

	  // keep the pixel of the patch that minimize error and begin again
	  for(int i(0); i<9; ++i){
          double std = sqrt(map[2*i]*map[2*i]+map[2*i+1]*map[2*i+1]);

          if(std < error){
        	  error = std;
        	  sol[0] = patch[2*i];
        	  sol[1] = patch[2*i+1];
          }
	  }

	  err = error;
	  ++k;
  }

  x0[0] = sol[0];
  y0[0] = sol[1];

}

/**
 * Compute rotation and translation in sensors coordinate
 */
void Channel::getRotation(){
  double elevation=rad(sensor->elevation); //theta
  double heading=rad(sensor->heading); // phi
  double azimuth=rad(sensor->azimuth);
  double psi=rad(sensor->roll);

  // symmetry aroud xz-plane
  Matx<double,3,3> Sy (
    1.0, 0.0,0.0,
    0.0,-1.0,0.0,
    0.0, 0.0,1.0 );
  
  /*
   * Converting from the sub-camera coordinates to the target coordinates
 	0) rotate by -psi around CZ: Vc1= R1*Vc
 	| Xc1 |   | cos(psi)  sin(psi)    0         |   |Xc|
 	| Yc1 | = |-sin(psi)  cos(psi)    0         | * |Yc|
 	| Zc1 |   |    0         0        1         |   |Zc|
   */
  Matx<double,3,3> R0(
      cos(psi),sin(psi),0,
      -sin(psi),cos(psi),0,
      0,0,1
  );

  /*
      1) rotate by - theta around C1X:Vc2= R2*Vc1
      | Xc2 |   |    1         0         0        |   |Xc1|
      | Yc2 | = |    0    cos(theta)   sin(theta) | * |Yc1|
      | Zc2 |   |    0   -sin(theta)   cos(theta) |   |Zc1|
   */
  Matx<double,3,3> R1(
      1.0,0.0,0.0,
      0.0,cos(elevation),sin(elevation),
      0.0,-sin(elevation),cos(elevation)
  );
  /*
      2) rotate by -(azimuth+phi) around C2Y:Vc3= R3*Vc2
      | Xc3 |   | cos(azimuth+phi)    0   sin(azimuth+phi)   |   |Xc2|
      | Yc3 | = |     0               1         0            | * |Yc2|
      | Zc3 |   | -sin(azimuth+phi)   0   cos(azimuth+phi)   |   |Zc2|
   */
  Matx<double,3,3> R2(
      cos(heading+azimuth),0.0,sin(heading+azimuth),
      0.0,1.0,0.0,
      -sin(heading+azimuth),0.0,cos(heading+azimuth)
  );

  // 3) R = R2*R1*R0 transform sensor coordinate to world coordinate
  Matx<double,3,3> Rtemp(Sy*(R2*(R1*(R0*Sy))));

  R[0] = Rtemp(0,0);
  R[1] = Rtemp(1,0);
  R[2] = Rtemp(2,0);
  R[3] = Rtemp(0,1);
  R[4] = Rtemp(1,1);
  R[5] = Rtemp(2,1);
  R[6] = Rtemp(0,2);
  R[7] = Rtemp(1,2);
  R[8] = Rtemp(2,2);
}

/** --------------------------------------------------------
 *  Compute inverse of camera matrix
 * ---------------------------------------------------------
 */

void  Channel::getKinv(){
  /** given camera internal parameter (ax,ay,s,px,py), compute inverse.
   * a close form is given by
   *    1/ax  -s/(ax*ay)    s*py/(ax*ay)-px/ax
   *    0      1/ay        -py/ay
   *    0       0           1
   **/

  double ax = K[0];
  double  s = K[1];
  double px = K[2];
  double ay = K[4];
  double py = K[5];

  Kinv[0] = 1.0/ax;
  Kinv[1] = -s/(ax*ay);
  Kinv[2] = -px/ax+ s*py/(ax*ay);

  Kinv[3] = 0.0;
  Kinv[4] = 1/ay;
  Kinv[5] = -py/ay;

  Kinv[6] = 0.0;
  Kinv[7] = 0.0;
  Kinv[8] = 1.0;
}

/**
 * Calculate sensor entrance pupil coordinates in camera coordinate system
 * @param chn sensor number
 * @return (x,y,z)
 */
void Channel::getLensCenterVector(){
  double elevation=rad(sensor->elevation); //theta
  double heading=rad(sensor->heading); // phi
  double azimuth=rad(sensor->azimuth);

  /* 0) Translate by distance to entrance pupil (lens center)
      | Xc0 |   | 0                     |   |Xc|
      | Yc0 | = | 0                     | + |Yc|
      | Zc0 |   | entrancePupilForward  |   |Zc|
   */
  Matx<double,3,1> T0(0,0,sensor->entrancePupilForward);
  /*
      2) rotate by - theta around C1X:Vc2= R2*Vc1
      | Xc2 |   |    1         0         0        |   |Xc1|
      | Yc2 | = |    0    cos(theta)   sin(theta) | * |Yc1|
      | Zc2 |   |    0   -sin(theta)   cos(theta) |   |Zc1|
   */
  Matx<double,3,3> R2(
      1.0,0.0,0.0,
      0.0,cos(elevation),sin(elevation),
      0.0,-sin(elevation),cos(elevation)
  );
  /*
      3) rotate by -(azimuth+phi) around C2Y:Vc3= R3*Vc2
      | Xc3 |   | cos(azimuth+phi)    0   sin(azimuth+phi)   |   |Xc2|
      | Yc3 | = |     0               1         0            | * |Yc2|
      | Zc3 |   | -sin(azimuth+phi)   0   cos(azimuth+phi)   |   |Zc2|
   */
  Matx<double,3,3> R3(
      cos(heading+azimuth),0.0,sin(heading+azimuth),
      0.0,1.0,0.0,
      -sin(heading+azimuth),0.0,cos(heading+azimuth)
  );
  /*
      4) Now axes are aligned, just translate to get to eyesis coordinates: Vey= T1+Vc3
      | Xey |   |      r * sin (azimuth)       |   |Xc3|
      | Yey | = | height+centerAboveHorizontal | + |Yc3|
      | Zey |   |      r * cos (azimuth)       |   |Zc3|
   */
  Matx<double,3,1> T1(
      sensor->radius*sin(azimuth),
      sensor->height,
      sensor->radius*cos(azimuth)
  );

  // MA=R3*R2;
  // MB=T1+R3*R2*T0;
  //       Matrix MA=R3.times(R2);
  //       Matrix MB=T1.plus(R3.times(R2.times(T0)));
  //       double [] aB=T1.plus(R3.times(R2.times(T0))).getRowPackedCopy();

  // symmetry aroud xz-plane
  Matx<double,3,3> Sy (
    1.0, 0.0,0.0,
    0.0,-1.0,0.0,
    0.0, 0.0,1.0
  );
  
  Matx<double,3,1> result(Sy*T1+Sy*(R3*(R2*T0)));
  lensCenterVector[0]=result.val[0];
  lensCenterVector[1]=result.val[1];
  lensCenterVector[2]=result.val[2];

}

void interpolateSubPix(CImg<float> &calib,double (&value)[2], int order, double u, double v){

	int u0 = floor(u);
	int v0 = floor(v);

	for(int i(0); i< order; ++i)
		for( int j(0); j< order; ++j){
			value[0] += phi(order,u,v,u0+i-order/2,v0+j-order/2)*calib(u0+i-order/2,v0+j-order/2,0,0);
			value[1] += phi(order,u,v,u0+i-order/2,v0+j-order/2)*calib(u0+i-order/2,v0+j-order/2,1,0);
		}
}

double phi(int order, double u, double v, int i, int j){

	double value=1.0;

	int u0 = floor(u);
	int v0 = floor(v);

	for(int k(0); k< order; ++k)
		if( (u0+k-order/2) != i ){
		   value *= (u-u0-k+order/2) / ( (double) (u0+k-i-order/2) );
		}

	for(int l(0); l< order; ++l)
		if( (v0+l-order/2) != j )
			value *= (v-v0-l+order/2)/ ( (double) (v0+l-j-order/2) ) ;

	return value;
}

}
