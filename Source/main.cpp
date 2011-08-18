
#include "options.h"
#include <iostream>

// doc : http://www.cmcrossroads.com/bradapp/ftp/src/libs/C++/Options.html
static const char * optv[] = {
    "h|help",
    NULL
  } ;


int main(int argc, char *argv[])
{
	Options  opts(*argv, optv);
	OptArgvIter  iter(--argc, ++argv);
	const char * optarg;
	int  errors = 0;
	char optchar;

	opts.ctrls(Options::PARSE_POS);
	while(optchar  = opts(iter, optarg) ) {
		switch (optchar)
		{
			case 'h' :
			  opts.usage(std::cout, "");
			  return 0;
			  break;
			default :
				++errors;
				break;
		}
	}

	if(errors)
	{
		opts.usage(std::cerr, "");
		return 1;
	}
}


