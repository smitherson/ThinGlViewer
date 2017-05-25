// Include standard headers
#include <stdio.h>
#include "ThinGlViewer.h"
using namespace glm;


int main( void )
{

  ThinGlViewer thinViewer;
  thinViewer.initWindow("demo",1024,768);
  thinViewer.render();
	
	return 0;
}

