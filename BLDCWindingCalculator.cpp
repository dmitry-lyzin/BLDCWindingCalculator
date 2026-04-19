#define _USE_MATH_DEFINES
#define STATIC_TESTS 1
#define STATIC_ASSERT(exp) static_assert( exp, #exp " FAIL!" );
#define ˂ template< class
#define ˂ᵗ template< class
#define ˂˃ template<>
#define 〇 operator
#define ￮ operator
#define ₠ constexpr
#define č const
#define v̆ virtual
#define š static
#define ŏ override
#define ŭ unsigned
#define ٭ * __restrict
#define ٭ʳ * __restrict
#define ᚼ * __restrict
#define ř __restrict
#define čň __restrict const
#define čňŏ __restrict const override
#define ňŏ __restrict override
#define STATIC constexpr static
#define _₠čv̆šňŏŭř˂˃ᚼ

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <complex>

using ui	= unsigned int;
using angle	= unsigned long long;
using cchar	= const char;
using std::swap;
using std::size; //#define size(x) (sizeof (x) / sizeof *(x))
using std::string_view;

#ifdef __unix__
#	include <unistd.h>
#	include <libgen.h>
#	include <libintl.h>
#	define _(str) gettext((str))
#else
#	define _(str) (str)
#	include <windows.h>
#	include <io.h>
#	define STDIN_FILENO 0
#	define STDOUT_FILENO 1
#	define STDERR_FILENO 2
#	pragma warning( disable: 4996)

HANDLE hConsole = nullptr;
CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;

#endif

bool	stdout_is_console = false;
char	BUF[1024];

₠ inline int fast_toupper( int c) { return c & ~('a' ^ 'A'); }
// pow10(n) = 10ⁿ
₠ ui pow10( ui n)
{
	ui res = 1;
	while( n --> 0)
		res *= 10;
	return res;
}
// наибольший общий делить (НОД) a.k.a. greatest common divisor
₠ ui NOD( ui a, ui b)
{
	if( a < b )
		swap(a, b);
	while( a %= b )
		swap(a, b);
	return b;
}
// Наименьшее общее кратное a.k.a. least common multiple
₠ ui NOK( ui a, ui b)
{
	return a / NOD(a, b) * b;
}
˂ num> ₠ num div_mul( num x, ui divider, ui multiplier)
{
	if( 0 == multiplier )
		return 0;
	ui nod = NOD( divider, multiplier);
	divider    /= nod;
	multiplier /= nod;
	return x / divider * multiplier + x % divider * multiplier / divider;
}

#pragma warning( disable: 4455 )
₠ angle operator ""⁰ ( unsigned long long degree )
{
	ui ui_degree = ui(degree);
	₠ const angle _180⁰ = angle(-1)/2+1; // = 0b10....0 = 0x80....0
	angle ret = _180⁰ * (ui_degree / 180u);
	ui_degree %= 180u;
	return ret + div_mul( _180⁰, 180u, ui_degree);
}
#if STATIC_TESTS
STATIC_ASSERT( 180⁰/180	==   1⁰ );
STATIC_ASSERT(  90⁰/3	==  30⁰ );
STATIC_ASSERT( 300⁰/5	==  60⁰ );
STATIC_ASSERT( 300⁰/3*2	== 200⁰ );
//STATIC_ASSERT( 333⁰/3*2	== 222⁰ );
STATIC_ASSERT( div_mul(333⁰, 3, 2) == 222⁰ );
#endif

₠ angle very_small_angle = 32*4;
//₠ double very_small_angle1 = double(very_small_angle) / 180⁰;

// перегрузки для преобразований число <-> строка

#define group1(x) true

#if group1(перегрузки для преобразований число <-> строка)
int fprint( FILE *stream, int		x ) { return fprintf( stream, "%d",	x ); }
int fprint( FILE *stream, ŭ int		x ) { return fprintf( stream, "%u",	x ); }
int fprint( FILE *stream, long		x ) { return fprintf( stream, "%ld",	x ); }
int fprint( FILE *stream, long long	x ) { return fprintf( stream, "%lld",	x ); }
int fprint( FILE *stream, ŭ long	x ) { return fprintf( stream, "%lu",	x ); }
int fprint( FILE *stream, ŭ long long	x ) { return fprintf( stream, "%llu",	x ); }
int fprint( FILE *stream, float		x ) { return fprintf( stream, "%g",	x ); }
int fprint( FILE *stream, double	x ) { return fprintf( stream, "%g",	x ); }

˂ Num> Num	strto			  (cchar *str, cchar **end) { return 0;	STATIC_ASSERT( false		); }
˂˃ int		strto< int		> (cchar *str, cchar **end) { return strtol	(str, (char **)end, 0	); }
˂˃ ŭ int	strto< ŭ int		> (cchar *str, cchar **end) { return strtoul	(str, (char **)end, 0	); }
˂˃ long		strto< long		> (cchar *str, cchar **end) { return strtol	(str, (char **)end, 0	); }
˂˃ long long	strto< long long	> (cchar *str, cchar **end) { return strtoll	(str, (char **)end, 0	); }
˂˃ ŭ long	strto< ŭ long		> (cchar *str, cchar **end) { return strtoul	(str, (char **)end, 0	); }
˂˃ ŭ long long	strto< ŭ long long	> (cchar *str, cchar **end) { return strtoull	(str, (char **)end, 0	); }
˂˃ float	strto< float		> (cchar *str, cchar **end) { return strtof	(str, (char **)end	); }
˂˃ double	strto< double		> (cchar *str, cchar **end) { return strtod	(str, (char **)end	); }
#endif

˂ num> bool str_to_num( cchar ᚼ str, num ᚼ x)
{
	cchar *end = str;
	num res = strto<num>( str, &end);
	if( end > str)
	{
		*x = res;
		return true;
	}

	//fprintf( stderr, "%s: %s %u\n", str, _("It's not a number. Used by default"), *x);
	fprintf( stderr, "%s: %s ", str, _("It's not a number. Used by default"));
	fprint( stderr, *x);
	fprintf( stderr, "\n");
	return false;
}

bool str_to_num1( string_view str, long *x)
{
	cchar *end = &str[0];
	long res = strto<long>( &str[0], &end);
	if( end > &str[0] )
		*x = res;

	if( end - &str[0] == size(str) )
		return true;

	//fprintf( stderr, "%s: %s %u\n", str, _("It's not a number. Used by default"), *x);
	fprintf( stderr, "%.*s: %s ", int(size(str)), &str[0], _("It's not a number. Used by default"));
	fprint( stderr, *x);
	fprintf( stderr, "\n");
	return false;
}

bool str_to_num1( cchar *str, cchar *end, long *x)
{
	int len = end - str;
	long res = strto<long>( str, &end);

	if( end == nullptr || (end - str < len) )
	{
		fprintf( stderr, "%.*s: %s ", len, str, _("It's not a number. Used by default"));
		fprint( stderr, *x);
		fprintf( stderr, "\n");
		return false;
	}

	*x = res;
	return true;
}

˂ num> ui num_sign_num( cchar ᚼ str, cchar sign, num *x1, num *x2 )
{
	cchar *after_sign = str;
	while( *after_sign)
	{
		if( *after_sign++ == sign)
		{
			ui res = 1;
			if( str[0] != sign)
				res = str_to_num( str, x1);
			if( after_sign[0] )
				res = res && str_to_num( after_sign, x2);
			return res * 2;
		}
	}
	return str_to_num( str, x1);
}

ui num_sign_num1( cchar ᚼ str, cchar sign, long *x1, long *x2 )
{
	cchar *after_sign = str;
	while( *after_sign)
	{
		if( *after_sign++ == sign)
		{
			ui res = 1;
			if( str[0] != sign)
				res = str_to_num1( str, x1);
			if( after_sign[0] )
				res = res && str_to_num1( after_sign, x2);
			return res * 2;
		}
	}
	return str_to_num1( str, x1);
}

/*
template< class Num>
ui num_sign_num1( cchar *str, cchar *end, cchar sign, Num *x1, Num *x2 )
{
	for( cchar *after_sign = str; after_sign < end; after_sign++)
	{
		if( *after_sign == sign)
		{
			ui res = 1;
			if( str[0] != sign)
				res = str_to_num1( str, after_sign, x1);
			after_sign++;
			if( after_sign < end )
				res = res && str_to_num1( after_sign, end, x2);
			return res * 2;
		}
	}
	return str_to_num1( str, end, x1);
}

struct Frac
{
	short	num;
	short	den;

₠	Frac		( void		): num(0), den(1) {}
₠	Frac		( int n		): num(n), den(1) {}
₠	Frac		( int n, int d	): num(n), den(d)
	{
		ui nod = NOD(n, d);
		num /= nod;
		den /= nod;
	}
};

STATIC_ASSERT(sizeof ui == sizeof Frac);

void strto( Frac *x, cchar *str, cchar **end)
{
	ui nominator = 0;
	ui denominator= 1;

	//if( !num_sign_num( arg, '/', &multiplier, &denominator))
	//	return false;
	num_sign_num1( str, end, '/', &nominator, &denominator);

	if( !nominator || !denominator )
	{
		*end = nullptr;
		return;
	}

}
int fprintf( FILE *stream, Frac	x ) { return fprintf( stream, "%d/%u",	x.num, x.den ); }

*/

struct Param
{
	bool	column;
	char	opt;
	cchar*	shortname;
	cchar*	longname;

virtual	void	usage_s	( void			) čň	= 0;
virtual	void	usage_l	( void			) čň	= 0;
virtual	ui	calc	( ui slots, ui poles	) čň	= 0;
virtual	void	print	( ui val		) čň	= 0;
virtual	bool	load	( cchar ᚼ arg		)	= 0;

₠	Param		( char _opt, cchar *_shortname, cchar *_longname)
	: column(false), opt(_opt), shortname(_shortname), longname(_longname) {}
};

struct Param_range: Param
{
#undef min
#undef max
	ui	min;
	ui	max;
₠	ui	minmax	( ui val	) čň	{ return (min <= val && val <= max) ? val : 0;		}

virtual	void	usage_s	( void		) čňŏ	{ printf( " [%c<диапазон>]", opt);			}
virtual	void	usage_l	( void		) čňŏ	{ printf( "\t%c<диапазон>\t%s\n", opt, _(longname));	}
virtual	void	print	( ui val	) čňŏ	{ printf( "%u\t", val);					}
virtual	bool	load	( cchar ᚼ arg	)  ňŏ
	{
		switch( num_sign_num( arg, '-', &min, &max))
		{
		case 0:	return false;
		case 1:	max = min;
		case 2:	return true;
		}
		assert( false);
		return false;
	}

₠	Param_range	( char opt, cchar *shortname, cchar *longname, ui _min, ui _max)
	: Param( opt, shortname, longname), min(_min), max(_max) {}
};

struct Param_range_step: Param_range
{
	ui	step;
virtual	bool	load	( cchar ᚼ arg	)
	{
		if( !Param_range::load( arg))
			return false;
		if( step && ( ! min || min % step) )
		{
			fprintf( stderr, "%c%s: ", opt, arg);
			fprintf( stderr, _("Number of %s must be divisible by %u!"), _(longname), step );
			fprintf( stderr, "\n");
			return false;
			//exit( EXIT_FAILURE);
		}

		return true;
	}
₠	Param_range_step( char opt, cchar *shortname, cchar *longname, ui min, ui max, ui _step)
	: Param_range( opt, shortname, longname, min, max), step(_step) {}
};

struct Param_poles		final: Param_range_step
{
virtual	ui	calc	( ui slots, ui poles	) čňŏ	{ return poles; }
₠	Param_poles	( void			): Param_range_step( 'p', "poles", "magnet poles", 2, 100, 2) {}
} par_poles;

struct Param_slots		final: Param_range_step
{
virtual	ui	calc	( ui slots, ui poles	) čňŏ	{ return slots; }
virtual	bool	load	( cchar ᚼ arg		)  ňŏ
	{
		if( !Param_range_step::load( arg))
			return false;

		if( max >= size(BUF) / 2)
		{
			fprintf( stderr, "%c%s: %s (> %u)!\n", opt, arg, _("Too many slots"), ui(size(BUF)) / 2);
			exit( EXIT_FAILURE);
		}

		return true;
	}
₠	Param_slots	( void			)	: Param_range_step( 's', "slots", "slots in the stator", 3, 99, 3) {}
} par_slots;

struct Param_NOK		final: Param_range
{
virtual	ui	calc	( ui slots, ui poles	) čňŏ	{ return minmax( NOK( slots, poles));	}
₠	Param_NOK	( void			)	: Param_range( 'c', "cogging", "cogging steps", 0, -1) {}
} par_NOK;

struct Param_reduction		final: Param_range
{
virtual	ui	calc	( ui slots, ui poles	) čňŏ	{ return minmax( NOK( slots, poles)/6);	}
₠	Param_reduction	( void			)	: Param_range( 'r', "ƒ/ν", "reduction (ƒ/ν)", 0, -1) {}
} par_reduct;

struct Print_config: Param
{
STATIC	auto	half = (sizeof(ui)*8 / 2);
STATIC	ui	pack	( ui slots, ui poles	)	{ return (slots << half) + poles;		}
STATIC	ui	slots	( ui val		)	{ return val >> half;				}
STATIC	ui	poles	( ui val		)	{ return val & ~(ui(-1) << half);		}

virtual	void	usage_s	( void			) čňŏ	{						}
virtual	void	usage_l	( void			) čňŏ	{						}
virtual	ui	calc	( ui slots, ui poles	) čňŏ	{ return pack( slots, poles);			}
virtual	void	print	( ui val		) čňŏ	{ printf( "%u/%u\t", slots(val), poles(val) );	}
virtual	bool	load	( cchar ᚼ arg		)  ňŏ	{ return false;					}

₠	Print_config	( char opt, cchar *shortname, cchar *longname): Param( opt, shortname, longname) {}
₠	Print_config	( void			)	: Param( 0, "config", "configuration") {}
} print_config;

struct Param_q			final: Print_config
{
	ui	sample;
virtual	void	usage_s	( void			) čňŏ	{ printf( " [%c<дробь>]", opt);			}
virtual	void	usage_l	( void			) čňŏ	{ printf( "\t%c<дробь>\t%s\n", opt,_(longname));}
virtual	ui	calc	( ui slots, ui poles	) čňŏ
	{
		slots /= 3;
		ui nod = NOD( slots, poles);
		ui val = pack( slots/nod, poles/nod );
		if( !sample || val == sample )
			return val;
		return 0;
	}
virtual	bool	load	( cchar ᚼ arg		)  ňŏ
	{
		ui multiplier = 0;
		ui denominator= 1;

		//if( !num_sign_num( arg, '/', &multiplier, &denominator))
		//	return false;
		num_sign_num( arg, '/', &multiplier, &denominator);

		if( !multiplier || !denominator )
		{
			fprintf( stderr, "%c%s: %s %s = 0?\n", opt, arg, _(longname), _(shortname));
			exit( EXIT_FAILURE);
		}

		ui nod = NOD( multiplier, denominator);
		sample = pack( multiplier/nod, denominator/nod );
		return true;
	}

₠	Param_q		( void			)	: Print_config( 'q', "q", "slots per pole per phase"), sample(0) {}
} par_q;

struct Param_winding_factor	final: Param_range
{
STATIC	ui	nuls	= 6;
STATIC	ui	scale	= pow10( nuls);
STATIC	ui	toscale	( double x		)	{ return ui((x + .5/scale) * scale); }

virtual	ui	calc	( ui slots, ui poles	) čňŏ
	{
		using namespace std;
		complex<double> res = 0;

		complex<double>	ȓ = polar( 1., M_PI / slots * poles );
		angle		ρ = div_mul( 180⁰, slots, poles);
		complex<double>	â = ȓ;
		angle		α = 30⁰ + very_small_angle + ρ;

		int EMF = 0;
		int previous_EMF = 2; // при i == 0 EMF равнялся бы 2
		for( ui i = 1; i < slots; i++)
		{
			     if( α <  60⁰ ) EMF =  2;
			else if( α < 120⁰ ) EMF =  1;
			else if( α < 180⁰ ) EMF = -1;
			else if( α < 240⁰ ) EMF = -2;
			else if( α < 300⁰ ) EMF = -1;
			else                EMF =  1;

			res += â * double(EMF - previous_EMF);
			previous_EMF = EMF;

			â *= ȓ;
			α += ρ;
		}
		res += double(2 - previous_EMF); // i == 0
		return minmax( toscale( abs(res) / (slots * 2)) );
	}
virtual	void	print	( ui val		) čňŏ
	{
		if( val < scale) printf( ".%0*d\t", nuls, val		);
		else             printf( "%g\t", double(val) / scale	);
	}
virtual	bool	load	( cchar ᚼ arg		)  ňŏ
	{
		double d_min = min; d_min /= scale;
		double d_max = max; d_max /= scale;
		ui x = num_sign_num( arg, '-', &d_min, &d_max);
		min = toscale( d_min);
		max = toscale( d_max);

		switch( x )
		{
		case 0:	return false;
		case 1:	max = min;
		case 2:
			if( d_min > 1. ||  d_max > 1. )
			{
				fprintf( stderr, "%c%s: %s > 1 ?\n", opt, arg, _(longname) );
				exit( EXIT_FAILURE);
			}
			return true;
		}
		assert( false);
		return false;
	}

₠	Param_winding_factor( void		)	: Param_range( 'w', "WF", "winding factor", 0, scale) {}
} par_winding_factor;

struct Print_sxema		final: Print_config
{
STATIC	bool	test1	( ui slots, ui poles	)	{ return (poles / NOD( slots/3, poles)) % 3;		}
STATIC	bool	test2	( ui slots, ui poles	)
	{
		if( slots == poles )
			return false;

		assert( slots % 3 == 0 && slots >= 3 );	// Количество пазов не делится на 3!
		assert( poles % 2 == 0 && poles >= 2 );	// Не четное количество полюсов!\n

		angle α = 30⁰ + very_small_angle;
		angle ρ = div_mul( 180⁰, slots, poles);

		ui a = 0; ui b = 0; ui c = 0;
		ui A = 0; ui B = 0; ui C = 0;

		for( ui i = 0; i < slots; i++ )
		{
			     if( α <  60⁰ ) A++;
			else if( α < 120⁰ ) b++;
			else if( α < 180⁰ ) C++;
			else if( α < 240⁰ ) a++;
			else if( α < 300⁰ ) B++;
			else                c++;

			α += ρ;
		}

		return (a == b && a == c && A == B && A == C) == test1( slots, poles);
	}
virtual	ui	calc	( ui slots, ui poles	) čňŏ	{ return test1(slots, poles) ? pack(slots, poles) : 0;	}
virtual	void	print	( ui val		) čňŏ
	{
		ui slots = Print_config::slots(val);
		ui poles = Print_config::poles(val);
		//printf( "%u/%u\t", slots, poles);

		assert( slots != poles );		// Количество полюсов равно количеству пазов!
		assert( slots % 3 == 0 && slots );	// Количество пазов не делится на 3!
		assert( poles % 2 == 0 && poles );	// Нечетное количество полюсов!

		angle ρ = div_mul( 180⁰, slots, poles);

		char *sxema = BUF;
		angle α = 30⁰ + very_small_angle;
		ui a = 0; ui b = 0; ui c = 0;
		ui A = 0; ui B = 0; ui C = 0;
		for( ui i = 0; i < slots; i++ )
		{
			     if( α <  60⁰ ) { *sxema++ = 'A'; A++; }
			else if( α < 120⁰ ) { *sxema++ = 'b'; b++; }
			else if( α < 180⁰ ) { *sxema++ = 'C'; C++; }
			else if( α < 240⁰ ) { *sxema++ = 'a'; a++; }
			else if( α < 300⁰ ) { *sxema++ = 'B'; B++; }
			else                { *sxema++ = 'c'; c++; }

			α += ρ;
		}

		assert( a == b && a == c && A == B && A == C);

		//printf("\t\t%.*s\n", slots, BUF);
		memcpy( sxema, BUF, slots);
		// "прокручиваем" схему, чтоб она не кончалась фазой А
		while( fast_toupper(*--sxema) == 'A')
			{}
		sxema++;

		if( sxema[0] == 'a') // если первая обмотка оказалась намотаной влево
		{
			// меняем направление всех обмоток на противоположное
			char *p = sxema;
			for( ui i = 0; i < slots; i++, p++ )
				*p ^= ('a' ^ 'A');
		}

		if( fast_toupper( sxema[ slots-1]) != 'C')
		{
			// меняем фазы B и C местами
			char *p = sxema;
			for( ui i = 0; i < slots; i++, p++ )
				if( fast_toupper(*p) != 'A' )
					*p ^= ('B' ^ 'C');
		}

		if( ! stdout_is_console )
		{
			printf("%.*s\t", slots, sxema);
			return;
		}

#ifdef __unix__
		for( ui i = 0; i < slots; sxema++, i++ )
		{
			// ANSI escape color codes
			static cchar *color_codes[3] =
			{ "96" // Bright Cyan //"94" // Bright Blue
			, "92" // Bright Green
			, "93" // Bright Yellow
			};

			assert( ui(fast_toupper(*sxema)-'A') < size(color_codes) );
			printf( "\033[%sm%c", color_codes[ fast_toupper(*sxema)-'A'], *sxema);
		}
		printf( "\033[0m");
#else
		for( ui i = 0; i < slots; sxema++, i++ )
		{
			static WORD attributes[3] =
			{ FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN
			, FOREGROUND_INTENSITY | FOREGROUND_GREEN
			, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED
			};

			assert( ui(fast_toupper(*sxema)-'A') < size(attributes) );
			SetConsoleTextAttribute( hConsole, attributes[ fast_toupper(*sxema)-'A'] );
			putchar( *sxema);
		}
		SetConsoleTextAttribute( hConsole, console_screen_buffer_info.wAttributes);
#endif
	}
₠	Print_sxema	( void			)	: Print_config( 0, "winding scheme", "winding scheme") {}
} print_sxema;
#if STATIC_TESTS
STATIC_ASSERT( Print_sxema::test2( 24, 18) );
STATIC_ASSERT( Print_sxema::test2( 24, 20) );
STATIC_ASSERT( Print_sxema::test2( 24, 22) );
STATIC_ASSERT( Print_sxema::test2( 24, 26) );
STATIC_ASSERT( Print_sxema::test2( 24, 28) );
STATIC_ASSERT( Print_sxema::test2( 24, 30) );
STATIC_ASSERT( Print_sxema::test2( 24, 32) );
STATIC_ASSERT( Print_sxema::test2( 60, 22) );
#endif

Param *PARAMS[] = // все параметры
{ &par_slots
, &par_poles
//, &print_config
, &par_q
, &par_winding_factor
, &par_NOK
, &print_sxema
};
enum { slots_col = 0 };

/*int tab_XpXs( void )
{
	printf(	"%s->", _("slots") );
	for( ui slots = slots_min; slots <= slots_max; slots += 3)
		printf(	"\t%u", slots );
	printf(	"\n");

	printf(	"%s\t", _(POLES) );
	for( ui slots = slots_min; slots <= slots_max; slots += 3)
		printf(	"────────" );
	printf(	"\n");

	ui found = 0;
	for( ui poles = poles_min; poles <= poles_max; poles += 2)
	{
		printf(	"%u", poles );
		for( ui slots = slots_min; slots <= slots_max; slots += 3)
		{
			printf( "\t");
			if( exist_sxema( slots, poles ))
			{
				found++;
				printf(	"%u", NOK( slots, poles)/6 );
			}
		}
		printf(	"\n");
	}

	printf(_(SCHEMES_FOUND), found);
	return !found;
}
*/

// напечатать горизонтальную линию
void print_hr( ui len)
{
	while( len--)
		printf( "─" );
	printf( "\n");
}

int find_n_print_schemes( void )
{
	ui &slots_min = par_slots.min;
	ui &slots_max = par_slots.max;
	ui &poles_min = par_poles.min;
	ui &poles_max = par_poles.max;

	//bool	column	[size(PARAMS)] = {false};
	ui	first_val[size(PARAMS)];
	ui	val	[size(PARAMS)];

	ui found = 0;
	for( ui slots = slots_min; slots <= slots_max; slots += 3)
	{
		for( ui poles = poles_min; poles <= poles_max; poles += 2)
		{
			for( ui i = 0; i < size(PARAMS); i++)
			{
				if( ! (val[i] = PARAMS[i]->calc( slots, poles)) )
					goto label1;
			}

			if( ! found++)
			{
				for( ui i = 0; i < size(PARAMS); i++)
					first_val[i] = val[i];
			}
			else
			{
				for( ui i = 0; i < size(PARAMS)-1; i++) // последнюю колонку не проверяем
				{
					if( first_val[i] != val[i])
						PARAMS[i]->column = true;
				}
				if( first_val[slots_col] < val[slots_col] )
					first_val[slots_col] = val[slots_col];
			}
		label1:;
		}
	}
	// последняя колонка всегда есть, там схема сидит
	PARAMS[ size(PARAMS)-1 ]->column = true;

	if( found == 0 )
	{
		printf(_("%u schemes found\n"), 0);
		return 0;
	}

	printf( "\n");
	for( ui i = 0; i < size(PARAMS); i++)
	{
		if( ! PARAMS[i]->column )
		{
			PARAMS[i]->print( first_val[i]);
			printf( "%s\n", _(PARAMS[i]->longname) );
		}
	}
	printf( "\n");
	for( ui i = 0; i < size(PARAMS); i++)
	{
		if( PARAMS[i]->column )
			printf( "%s\t", _(PARAMS[i]->shortname));
	}
	printf( "\n");
	ui hr_len = first_val[slots_col];
	for( ui i = 0; i < size(PARAMS)-1; i++)
	{
		if( PARAMS[i]->column )
			hr_len += 8;
	}

	print_hr( hr_len );

	found = 0;
	for( ui slots = slots_min; slots <= slots_max; slots += 3)
	{
		for( ui poles = poles_min; poles <= poles_max; poles += 2)
		{
			for( ui i = 0; i < size(PARAMS); i++)
			{
				if( ! (val[i] = PARAMS[i]->calc( slots, poles)) )
					goto label2;
			}

			for( ui i = 0; i < size(PARAMS); i++)
			{
				if( PARAMS[i]->column)
					PARAMS[i]->print( val[i]);
			}
			printf("\n");
			found++;
		label2:;
		}
	}

	print_hr( hr_len );
	if( found == 1)	printf( "%s", _("1 scheme found\n")	);
	else		printf( _("%u schemes found\n"), found	);

	return found;
}

/*
//printf( "%s:\t%u\n%s:\t%u\n%s:\t%0.2g%% = %0.2g°\n", _(MAGNET_POLES), poles, _(SLOTS), slots, _(GAP), double(gap) / 60⁰ * 100., double(gap) / 1⁰ );
bool print_1_scheme( ui slots, ui poles )
{
	//ui val[size(PARAMS)] = { slots, poles, reduct, equil };
	ui val[size(PARAMS)] = {0};
	val[ SLOTS] = slots;
	val[ POLES] = poles;
	//val[ REDUCT]= NOK(slots, poles) / 6;
	val[ COGGING] = calc_equilibrium_points( slots, poles );

	printf( "\n");
	for( ui i = 0; i < size(PARAMS); i++)
	{
		PARAMS[i]->print( val[i]);
		printf( "%s\n", _(PARAMS[i]->longname) );
	}
	printf( "\n");

	printf( "%s\n", _(SXEMA));
	for( ui i = 0; i < slots; i++)
		printf( "─" );
	printf("\n");
	char *sxema = calc_sxema( BUF, slots, poles );
	if( sxema)
	{
		print_sxema( sxema, slots);
		for( ui i = 0; i < slots; i++)
			printf( "─" );
		printf("\n%s", _(SCHEME_FOUND));
		return true;
	}
	print_sxema( BUF, slots);
	for( ui i = 0; i < slots; i++)
		printf( "─" );
	printf( "\n%s\n", _("Unbalanced"));
	return false;
}
*/

int usage( void)
{
	const char *USAGE =
	"Usage:  %s [-h] [file]\n"
	"\n"
	"Parameters:\n"
	"	file		Print text file\n"
	"	-h		Print %s usage\n"
	"\n"
	"example:\n";

	printf(	"\nПрограмма для расчета схем намотки многополюсных электромоторов (BLDC и т.д.)\n\n"
		"Использование: %s [-h]", APPNAME);
	for( ui i = 0; i < size(PARAMS); i++)
		PARAMS[i]->usage_s();
	printf(	"\n\nопции:\n"
		"\t-h\t\tнапечатать этот текст\n");
	for( ui i = 0; i < size(PARAMS); i++)
		PARAMS[i]->usage_l();

	printf(	"\nгде:\n\t<диапазон>\tэто пара чисел через знак '-', в этой паре первое или второе\n"
		"\t\t\tили даже оба числа можно опустить (останется один '-')\n"
		"\t\t\tили указать одно число (будет диапазон из одного числа)\n"
		"\n"
		);

	return EXIT_SUCCESS;
}

bool optproc( int opt, cchar ᚼ arg )
{
	//printf("!!! %c, \"%s\"\n", opt, arg);
	if( opt == 'h')
		exit( usage() );

	for( ui i = 0; i < size(PARAMS); i++)
	{
		if( opt == PARAMS[i]->opt )
			return PARAMS[i]->load( arg);
	}
	return false;
}

₠ ui hash( cchar ᚼ str )
{
	if( !str )
		return 0;
	return ui(str[0]) + (ui(str[1]) << 8);
}

//int main( int argc, cchar* argv[] )
int main( int argc, char *const *argv )
{
	stdout_is_console = isatty( STDOUT_FILENO);

#ifdef __unix__
	//enum
	//{ LANG_ENGLISH  = hash("en")
	//, LANG_GERMAN   = hash("de")
	//, LANG_RUSSIAN  = hash("ru")
	//};

	if( ! setlocale			(LC_MESSAGES, "")) perror(getenv("LANG")); // может LC_ALL ? Нет, десятичная запятая - зло!
	if( ! bindtextdomain		(APPNAME, "."	)) perror("."		);
	if( ! bind_textdomain_codeset	(APPNAME,"UTF-8")) perror("UTF-8"	);
	if( ! textdomain		(APPNAME	)) perror(APPNAME	);

	auto primarylangid = hash( getenv("LANG") );
	//setlocale( LC_CTYPE, ".UTF8");
#else
	auto primarylangid = PRIMARYLANGID( GetUserDefaultLangID());

	SetConsoleOutputCP(65001);
	//freopen("CON", "w", stdout);
	if( isatty( STDERR_FILENO))
		freopen("CON", "w", stderr);
	//freopen("CON", "r", stdin);

	if( stdout_is_console )
	{
		hConsole = GetStdHandle( STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo( hConsole, &console_screen_buffer_info );
		// SetConsoleMode( hConsole, ENABLE_VIRTUAL_TERMINAL_PROCESSING); // под Windows7 не работает
	}
#endif

	//enum { EN, DE, RU, MAX_LANG };
	//ui	LANG	= EN;
	//switch( primarylangid )
	//{
	//case LANG_ENGLISH	: LANG = EN; break;
	//case LANG_GERMAN	: LANG = DE; break;
	//case LANG_RUSSIAN	: LANG = RU; break;
	//default			: LANG = EN; break;
	//}

	if( 1 == argc )
		return usage();

	for( int i = 1; i < argc; i++)
	{
		cchar *arg = argv[i];

		if( '-' == arg[0] )
			arg++;

		if( '\0' == arg[0] )
			continue; // что делать с отдельным '-' не знаю... Пока ничего

		if( arg[1] )
		{
			if( !optproc( arg[0], &arg[1]) )
				fprintf( stderr, "%s: %s\n", arg, _("Wrong parameter"));
			continue;
		}

		if( i+1 >= argc )
		{
			if( !optproc( arg[0], "") )
				fprintf( stderr, "%s: %s\n", arg, _("Wrong parameter"));
			continue;
		}
		else
		{
			if( !optproc( arg[0], argv[i+1]) )
				fprintf( stderr, "%c: %s\n", arg[0], _("Wrong parameter"));
			else
				i++;
			continue;
		}
	}

	//if( slots_min == slots_max && poles_min == poles_max)
	//	return ! print_1_scheme( slots_min, poles_min);

	return ! find_n_print_schemes();
}
