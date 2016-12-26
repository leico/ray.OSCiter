//
//  rayOSCiter.cpp
//
//  Created by leico on 2016/12/23.
//
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "ext.h"
#include "ext_obex.h"

#define PICOJSON_USE_INT64

typedef struct _rayOSCiter{
  t_object  object;
  void     *out;

} rayOSCiter;

void* newObject (t_symbol *s, long argc, t_atom *argv);
void  freeObject(rayOSCiter *x);
void  assistNavi(rayOSCiter *x, void *b, long m, long a, char *s);
void  getSymbol(rayOSCiter *x, t_symbol *s, long argc, t_atom *argv);

void *class_rayOSCiter;

void ext_main(void *r){

  t_class *c;

  c = class_new("ray.OSCiter", (method)newObject, (method)freeObject, (long)sizeof(rayOSCiter), 0L, A_GIMME, 0);

  class_addmethod(c, (method)assistNavi, "assist", A_CANT, 0);
  class_addmethod(c, (method)getSymbol,  "anything", A_GIMME, 0);

  class_register(CLASS_BOX, c);
  class_rayOSCiter = c;
}


void* newObject (t_symbol *s, long argc, t_atom *argv){

  rayOSCiter *x = NULL;

  if( (x = (rayOSCiter *)object_alloc((t_class *)class_rayOSCiter)) ){

    object_post((t_object *)x, "%s object instance created", s -> s_name);

    x -> out = outlet_new(x, NULL);
  }

  return x;

}
void freeObject(rayOSCiter *x){

}


void  getSymbol(rayOSCiter *x, t_symbol *s, long argc, t_atom *argv){
  
  std :: vector<std :: string> splitdata;
  {
    std :: stringstream ss(s -> s_name);
    std :: string buffer;
    while( std :: getline(ss, buffer, ' ') )
      splitdata.push_back(buffer);
  }

  std :: string strout;
  for(int i = 0 ; i < splitdata.size() ; ++ i){
    
    if(splitdata.at(i).at(0) == '/'){
      
      if( ! strout.empty() ){
        outlet_anything(x -> out, gensym(strout.c_str()), 0, (t_atom *)NIL);
        strout.clear();
      }

      strout = splitdata.at(i) + ' ';
      continue;
    }

    if(strout.empty()) continue;

    strout += splitdata.at(i) + ' ';
    
  }

}

void assistNavi(rayOSCiter *x, void *b, long m, long a, char *s){

  if (m == ASSIST_INLET) { // inlet
    sprintf(s, "I am inlet %ld", a - 1);
  }
  else {	// outlet
    sprintf(s, "I am outlet %ld", a);
  }

}

