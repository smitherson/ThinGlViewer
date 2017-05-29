// Include standard headers
#include <stdio.h>
#include "ThinglViewer.h"
using namespace glm;


int main( void )
{

  ThinglViewer thinViewer;
  if( thinViewer.initWindow("demo", 1024, 768)) {
      thinViewer.testRender();
  }
	
	return 0;
}

