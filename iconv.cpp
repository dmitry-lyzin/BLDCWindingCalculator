#include <stdio.h>
#include <string.h>
#include <iconv.h>

extern "C"
{
	/* Allocates descriptor for code conversion from encoding 'fromcode' to
	encoding 'tocode'. */
	extern iconv_t libiconv_open(const char* tocode, const char* fromcode)
	{
		//fprintf( stderr, "tocode: \"%s\", fromcode: \"%s\"\n", tocode, fromcode);
		static void* dummy[30];
		return dummy;
	}

	/* Converts, using conversion descriptor 'cd', at most '*inbytesleft' bytes
	starting at '*inbuf', writing at most '*outbytesleft' bytes starting at
	'*outbuf'.
	Decrements '*inbytesleft' and increments '*inbuf' by the same amount.
	Decrements '*outbytesleft' and increments '*outbuf' by the same amount. */
	extern size_t libiconv( iconv_t cd, char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft)
	{
		size_t count = *inbytesleft > *outbytesleft ? *outbytesleft : *inbytesleft;
		memcpy( *outbuf, *inbuf, count);
		*inbuf		+= count;
		*inbytesleft	-= count;
		*outbuf		+= count;
		*outbytesleft	-= count;
		return count;
	}
}
