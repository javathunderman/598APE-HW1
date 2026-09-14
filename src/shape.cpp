#include "shape.h"
#include <vector>
#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <string>

Shape::Shape(const Vector &c, Texture* t, double ya, double pi, double ro): center(c), texture(t), yaw(ya), pitch(pi), roll(ro){
};

void Shape::setAngles(double a, double b, double c){
   yaw =a; pitch = b; roll = c;
   xcos = cos(yaw);
   xsin = sin(yaw);
   ycos = cos(pitch);
   ysin = sin(pitch);
   zcos = cos(roll);
   zsin = sin(roll);
}

void Shape::setYaw(double a){
   yaw =a;
   xcos = cos(yaw);
   xsin = sin(yaw);
}

void Shape::setPitch(double b){
   pitch = b;
   ycos = cos(pitch);
   ysin = sin(pitch);
}

void Shape::setRoll(double c){
   roll = c;
   zcos = cos(roll);
   zsin = sin(roll);
}

typedef struct {
   double time;
   Shape* shape;
} TimeAndShape;

void calcColor(unsigned char* toFill,Autonoma* c, Ray ray, unsigned int depth){
   ShapeNode* t = c->listStart;
   size_t seen = 0;
   double time;
   unsigned int numShapes = c->numShapes;
   std::vector<TimeAndShape> times;
   times.reserve(numShapes);
   for (seen = 0; seen < numShapes; seen++) {
      time = t->data->getIntersection(ray);
      times.push_back((TimeAndShape){ time, t->data });
      t = t->next;
   }
   std::sort(times.begin(), times.end(), [](const TimeAndShape &t1, const TimeAndShape &t2)
   { 
      return t1.time > t2.time;
   });

   if (seen == 0 || times[0].time == inf) {
      double opacity, reflection, ambient;
      Vector temp = ray.vector.normalize();
      const double x = temp.x;
      const double z = temp.z;
      const double me = (temp.y<0)?-temp.y:temp.y;
      const double angle = atan2(z, x);
      c->skybox->getColor(toFill, &ambient, &opacity, &reflection, fix(angle/M_TWO_PI),fix(me));
      return;
   }

   double curTime = times[0].time;
   Shape* curShape = times[0].shape;

   Vector intersect = curTime*ray.vector+ray.point;
   double opacity, reflection, ambient;
   curShape->getColor(toFill, &ambient, &opacity, &reflection, c, Ray(intersect, ray.vector), depth);
   
   double lightData[3];
   getLight(lightData, c, intersect, curShape->getNormal(intersect), curShape->reversible());
   for (int j = 0; j < 3; j++) {
      toFill[j] = (unsigned char)(toFill[j]*(ambient+lightData[j]*(1-ambient)));
   }
   if(depth<c->depth && (opacity<1-1e-6 || reflection>1e-6)){
      unsigned char col[4];
      if(opacity<1-1e-6){
         Ray nextRay = Ray(intersect+ray.vector*1E-4, ray.vector);
         calcColor(col, c, nextRay, depth+1);
         for (int j = 0; j < 3; j++) {
            toFill[j]= (unsigned char)(toFill[j]*opacity+col[j]*(1-opacity));
         }
      }
      if(reflection>1e-6){
         Vector norm = curShape->getNormal(intersect).normalize();
         Vector vec = ray.vector-2*norm*(norm.dot(ray.vector));
         Ray nextRay = Ray(intersect+vec*1E-4, vec);
         calcColor(col, c, nextRay, depth+1);
         for (int j = 0; j < 3; j++) { // attempt to vectorize
            toFill[j]= (unsigned char)(toFill[j]*(1-reflection)+col[j]*(reflection));   
         }
      }
   }
}
