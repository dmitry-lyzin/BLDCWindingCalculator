#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#define _USE_MATH_DEFINES
#include <complex>
#define _INTL_NO_DEFINE_MACRO_PRINTF 1
#define _INTL_NO_DEFINE_MACRO_FPRINTF 1
#include <libintl.h>

#define STATIC_ASSERT(exp) static_assert( exp, #exp " FAIL!" );
#define DIV(x)	true
#define ˂	template< class
#define ˂˃	template<>
#define cØnst	const override
#define Ø	override
#define STATIC	constexpr static
#define CE	constexpr

using ui	= unsigned int;
using ulong	= unsigned long;
using ulonglong	= unsigned long long;
using angle	= unsigned long long;
using cchar	= const char;
using Complex	= std::complex< double>;
using std::swap;
using std::size;
using std::string_view;

#ifdef __unix__
#	include <unistd.h>
#	include <libgen.h>
#else
#	include <windows.h>
#	include <io.h>
#	undef min
#	undef max
#	define STDIN_FILENO 0
#	define STDOUT_FILENO 1
#	define STDERR_FILENO 2
#	pragma warning( disable: 4996)
	HANDLE hConsole = nullptr;
	CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer_info;
#endif

bool	STDOUT_IS_A_TTY = false;
char	BUF[1024];

inline cchar *_( cchar *msgid)
{
	return gettext( msgid);
}

CE inline int fast_toupper( int c) { return c & ~('a' ^ 'A'); }
// pow10(n) = 10ⁿ
CE ui pow10( ui n)
{
	ui res = 1;
	while( n --> 0)
		res *= 10;
	return res;
}
// наибольший общий делить (НОД) a.k.a. greatest common divisor
CE ui НОД( ui a, ui b)
{
	if( a < b )
		swap(a, b);
	while( a %= b )
		swap(a, b);
	return b;
}
// Наименьшее общее кратное a.k.a. least common multiple
CE ui НОК( ui a, ui b)
{
	return a / НОД(a, b) * b;
}
˂ num> CE num div_mul( num x, ui divider, ui multiplier)
{
	if( 0 == multiplier )
		return 0;
	ui nod = НОД( divider, multiplier);
	divider    /= nod;
	multiplier /= nod;
	return x / divider * multiplier + x % divider * multiplier / divider;
}

#pragma warning( disable: 4455 )
CE angle operator ""⁰ ( unsigned long long degree )
{
	ui ui_degree = ui(degree);
	CE const angle _180⁰ = angle(-1)/2+1; // = 0b10....0 = 0x80....0
	angle ret = _180⁰ * (ui_degree / 180u);
	ui_degree %= 180u;
	return ret + div_mul( _180⁰, 180u, ui_degree);
}
#if DIV( тесты для углов )
STATIC_ASSERT( 180⁰/180	==   1⁰ );
STATIC_ASSERT(  90⁰/3	==  30⁰ );
STATIC_ASSERT( 300⁰/5	==  60⁰ );
STATIC_ASSERT( 300⁰/3*2	== 200⁰ );
//STATIC_ASSERT( 333⁰/3*2	== 222⁰ );
STATIC_ASSERT( div_mul(333⁰, 3, 2) == 222⁰ );
#endif

// очень маленький угол
CE angle ε⁰ = 32*4;

#if DIV( перегрузки для преобразований число <-> строка )
int fprint( FILE *stream, int		x ) { return fprintf( stream, "%d",	x ); }
int fprint( FILE *stream, ui		x ) { return fprintf( stream, "%u",	x ); }
int fprint( FILE *stream, long		x ) { return fprintf( stream, "%ld",	x ); }
int fprint( FILE *stream, long long	x ) { return fprintf( stream, "%lld",	x ); }
int fprint( FILE *stream, ulong		x ) { return fprintf( stream, "%lu",	x ); }
int fprint( FILE *stream, ulonglong	x ) { return fprintf( stream, "%llu",	x ); }
int fprint( FILE *stream, float		x ) { return fprintf( stream, "%g",	x ); }
int fprint( FILE *stream, double	x ) { return fprintf( stream, "%g",	x ); }

˂ Num> Num	strto			  (cchar *str, cchar **end) { return 0;	STATIC_ASSERT( false		); }
˂˃ int		strto< int		> (cchar *str, cchar **end) { return strtol	(str, (char **)end, 0	); }
//˂˃ ui		strto< ui		> (cchar *str, cchar **end) { return strtoul	(str, (char **)end, 0	); }
˂˃ long		strto< long		> (cchar *str, cchar **end) { return strtol	(str, (char **)end, 0	); }
˂˃ long long	strto< long long	> (cchar *str, cchar **end) { return strtoll	(str, (char **)end, 0	); }
˂˃ ulong	strto< ulong		> (cchar *str, cchar **end) { return strtoul	(str, (char **)end, 0	); }
˂˃ ulonglong	strto< ulonglong	> (cchar *str, cchar **end) { return strtoull	(str, (char **)end, 0	); }
˂˃ float	strto< float		> (cchar *str, cchar **end) { return strtof	(str, (char **)end	); }
˂˃ double	strto< double		> (cchar *str, cchar **end) { return strtod	(str, (char **)end	); }
˂˃ CE ui	strto< ui		> (cchar *str, cchar **end)
{
	ui res = 0;
	while( '0' <= *str && *str <= '9' )
		res = res * 10 + (*str++ - '0');

	if( end )
		*end = str;
	return res;
}

// превратить строку в число (constexpr)
CE inline ui N( cchar *str) { return strto<ui>( str, nullptr); }

STATIC_ASSERT( N("123"	) == 123	);
STATIC_ASSERT( N("4321"	) == 4321	);
#endif

˂ num> bool str_to_num( cchar *str, num *x)
{
	cchar *end = str;
	num res = strto<num>( str, &end);
	if( end > str)
	{
		*x = res;
		return true;
	}

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

	fprintf( stderr, "%.*s: %s ", int( size( str)), &str[0], _("It's not a number. Used by default"));
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

˂ num> ui num_sign_num( cchar *str, cchar sign, num *x1, num *x2 )
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

ui num_sign_num1( cchar *str, cchar sign, long *x1, long *x2 )
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

//--------------------------------------------------------------------------------------------------------------
struct Param
{
CE	Param		( char _opt, cchar *_shortname, cchar *_longname)
	: column(false), opt(_opt), shortname(_shortname), longname(_longname) {}

	bool	column;
	char	opt;
	cchar*	shortname;
	cchar*	longname;

virtual	void	usage_s	( void			) const	{ printf( "%c", opt);						}
virtual	void	usage_l	( void			) const	{ printf("\t"); usage_s(); printf("\t%s\n", _(longname));	}
virtual	ui	test	( ui slots, ui poles	) const	= 0;
virtual	void	print	( ui val		) const	= 0;
virtual	bool	load	( cchar *arg		)	= 0;
};

//--------------------------------------------------------------------------------------------------------------
struct Param_balans: Param
{
enum	Sel						{ any = 0, yes = 1, no = 2				};
CE	Param_balans	( void			): Param( 'b', "balanc", "stator balance"), sel(any) {}

	Sel	sel;
virtual	void	usage_s	( void			) cØnst	{ printf( "%c[+|-|any]", opt);			}
virtual	ui	test	( ui slots, ui poles	) cØnst { ui r = slots%2 + 1; return !sel || sel == r ? r : 0;	}
virtual	void	print	( ui val		) cØnst	{ static cchar *c[]={"+","-"}; printf("%s", c[val-1]);	}
virtual	bool	load	( cchar *arg		) Ø
	{
		switch( arg[0])
		{
		case  0 :
		case '+':
		case ' ':
		case 'y': sel = yes;	return true;

		case '-':
		case 'n': sel = no;	return true;

		case 'a':
		case 'x': sel = any;	return true;
		default:		return false;
		}
		assert( false);
		return false;
	}
} par_balans;

//--------------------------------------------------------------------------------------------------------------
struct Param_range: Param
{
CE	Param_range	( char opt, cchar *shortname, cchar *longname, ui _min, ui _max)
	: Param( opt, shortname, longname), min(_min), max(_max) {}

	ui	min;
	ui	max;
CE	ui	minmax	( ui val	) const	{ return (min <= val && val <= max) ? val : 0;			}

virtual	void	usage_s	( void		) cØnst	{ printf( "%c<%s>", opt, _("range"));				}
virtual	void	print	( ui val	) cØnst	{ printf( "%u", val);						}
virtual	bool	load	( cchar *arg	) Ø
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
};

//--------------------------------------------------------------------------------------------------------------
struct Param_range_step: Param_range
{
CE	Param_range_step( char opt, cchar *shortname, cchar *longname, ui min, ui max, ui _step)
	: Param_range( opt, shortname, longname, min, max), step(_step) {}

	ui	step;
virtual	bool	load	( cchar *arg	)
	{
		if( !Param_range::load( arg))
			return false;
		if( step && ( ! min || min % step) )
		{
			fprintf( stderr, "%c%s: ", opt, arg);
			fprintf( stderr, _("Number of %s must be divisible by %u!"), _(longname), step );
			fprintf( stderr, "\n");
			return false;
		}

		return true;
	}
};

//--------------------------------------------------------------------------------------------------------------
struct Param_poles		final: Param_range_step
{
CE	Param_poles	( void			): Param_range_step( 'p', "poles", "magnet poles", 2, 100, 2) {}
virtual	ui	test	( ui slots, ui poles	) cØnst	{ return poles; }
} par_poles;

//--------------------------------------------------------------------------------------------------------------
struct Param_slots		final: Param_range_step
{
CE	Param_slots	( void			)	: Param_range_step( 's', "slots", "slots in the stator", 3, 99, 3) {}
virtual	ui	test	( ui slots, ui poles	) cØnst	{ return slots; }
virtual	bool	load	( cchar *arg		) Ø
	{
		if( !Param_range_step::load( arg))
			return false;

		if( max >= size(BUF) / 2)
		{
			fprintf( stderr, "%c%s: %s (> %u)!\n", opt, arg, _("Too many slots"), ui( size( BUF)) / 2);
			exit( EXIT_FAILURE);
		}

		return true;
	}
} par_slots;

//--------------------------------------------------------------------------------------------------------------
struct Param_cogging		final: Param_range
{
CE	Param_cogging	( void			)	: Param_range( 'c', "cogging", "cogging steps", 0, -1) {}
virtual	ui	test	( ui slots, ui poles	) cØnst	{ return minmax( НОК( slots, poles));	}
} par_cogging;

//--------------------------------------------------------------------------------------------------------------
struct Param_reduction		final: Param_range
{
CE	Param_reduction	( void			)	: Param_range( 'r', "ƒ/ν", "reduction (ƒ/ν)", 0, -1) {}
virtual	ui	test	( ui slots, ui poles	) cØnst	{ return minmax( НОК( slots, poles)/6);	}
} par_reduct;

//--------------------------------------------------------------------------------------------------------------
struct Print_config: Param
{
CE	Print_config	( char opt, cchar *shortname, cchar *longname): Param( opt, shortname, longname) {}
CE	Print_config	( void			)	: Param( 0, "config", "configuration") {}

STATIC	auto	half = (sizeof(ui)*8 / 2);
STATIC	ui	pack	( ui slots, ui poles	)	{ return (slots << half) + poles;		}
STATIC	ui	slots	( ui val		)	{ return val >> half;				}
STATIC	ui	poles	( ui val		)	{ return val & ~(ui(-1) << half);		}

virtual	ui	test	( ui slots, ui poles	) cØnst	{ return pack( slots, poles);			}
virtual	void	print	( ui val		) cØnst	{ printf( "%u/%u", slots(val), poles(val) );	}
virtual	bool	load	( cchar *arg		) Ø	{ return false;					}
} print_config;

//--------------------------------------------------------------------------------------------------------------
struct Param_q			final: Print_config
{
CE	Param_q		( void			)	: Print_config( 'q', "q", "q = slots/poles/phases"), sample(0) {}

	ui	sample;
virtual	void	usage_s	( void			) cØnst	{ printf( "%c<%s>", opt, _("fraction"));			}
virtual	ui	test	( ui slots, ui poles	) cØnst
	{
		slots /= 3;
		ui nod = НОД( slots, poles);
		ui val = pack( slots/nod, poles/nod );
		if( !sample || val == sample )
			return val;
		return 0;
	}
virtual	bool	load	( cchar *arg		) Ø
	{
		ui numerator	= 0;
		ui denominator	= 1;

		num_sign_num( arg, '/', &numerator, &denominator);

		if( !numerator || !denominator )
		{
			fprintf( stderr, "%c%s: %s %s = 0?\n", opt, arg, _(longname), _(shortname));
			exit( EXIT_FAILURE);
		}

		ui nod = НОД( numerator, denominator);
		sample = pack( numerator/nod, denominator/nod );
		return true;
	}
} par_q;

//--------------------------------------------------------------------------------------------------------------
struct Param_winding_factor	final: Param_range
{
CE	Param_winding_factor( void		)	: Param_range( 'w', "WF", "winding factor", 0, scale) {}

STATIC	ui	nuls	= 6;
STATIC	ui	scale	= pow10( nuls);
STATIC	ui	toscale	( double x		)	{ return ui((x + .5/scale) * scale); }

virtual	ui	test	( ui slots, ui poles	) cØnst
	{
		Complex	res = 0;

		Complex	ȓ = std::polar( 1., M_PI / slots * poles );
		angle	ρ = div_mul( 180⁰, slots, poles);
		Complex	â = ȓ;
		angle	α = 30⁰ + ρ + ε⁰;

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
virtual	void	print	( ui val		) cØnst
	{
		if( val < scale) printf( ".%0*d", nuls, val		);
		else             printf( "%g", double(val) / scale	);
	}
virtual	bool	load	( cchar *arg		) Ø
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
			if( d_min > 1. || d_max > 1. )
			{
				fprintf( stderr, "%c%s: %s > 1 ?\n", opt, arg, _(longname) );
				exit( EXIT_FAILURE);
			}
			return true;
		}
		assert( false);
		return false;
	}
} par_winding_factor;

//--------------------------------------------------------------------------------------------------------------
struct Print_sxema		final: Print_config
{
CE	Print_sxema	( void			)	: Print_config( 0, "winding scheme", "winding scheme") {}
STATIC	bool	test0	( ui slots, ui poles	)	{ return (poles / НОД( slots/3, poles)) % 3;		}
STATIC	bool	test1	( ui slots, ui poles	)
	{
		if( slots == poles )
			return false;

		assert( slots % 3 == 0 && slots >= 3 );	// Количество пазов не делится на 3!
		assert( poles % 2 == 0 && poles >= 2 );	// Не четное количество полюсов!\n

		angle α = 30⁰ + ε⁰;
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

		return (a == b && a == c && A == B && A == C);
	}
STATIC	bool	test2	( ui slots, ui poles	)	{ return test0( slots, poles) == test1( slots, poles);	}
virtual	ui	test	( ui slots, ui poles	) cØnst	{ return test0(slots, poles) ? pack(slots, poles) : 0;	}
virtual	void	usage_s	( void			) cØnst	{							}
virtual	void	usage_l	( void			) cØnst	{							}
virtual	void	print	( ui val		) cØnst
	{
		ui slots = Print_config::slots(val);
		ui poles = Print_config::poles(val);

		assert( slots != poles );		// Количество полюсов равно количеству пазов!
		assert( slots % 3 == 0 && slots );	// Количество пазов не делится на 3!
		assert( poles % 2 == 0 && poles );	// Нечетное количество полюсов!

		angle ρ = div_mul( 180⁰, slots, poles);

		char *sxema = BUF;
		angle α = 30⁰ + ε⁰;
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

		if( ! STDOUT_IS_A_TTY )
		{
			printf("%.*s", slots, sxema);
			return;
		}

#ifdef __unix__
		for( ui i = 0; i < slots; sxema++, i++ )
		{
			// ANSI escape color codes
			static cchar *color_codes[3] =
			{ "91" // Bright Red // "93" // Bright Yellow
			, "92" // Bright Green
			, "94" // Bright Blue // "96" // Bright Cyan
			};

			assert( ui(fast_toupper(*sxema)-'A') < size(color_codes) );
			printf( "\033[%sm%c", color_codes[ fast_toupper(*sxema)-'A'], *sxema);
		}
		printf( "\033[0m");
#else
		for( ui i = 0; i < slots; sxema++, i++ )
		{
			static WORD attributes[3] =
			{ FOREGROUND_INTENSITY	| FOREGROUND_RED
			, FOREGROUND_INTENSITY	| FOREGROUND_GREEN
			, FOREGROUND_INTENSITY	| FOREGROUND_BLUE
			};

			assert( ui(fast_toupper(*sxema)-'A') < size(attributes) );
			SetConsoleTextAttribute( hConsole, attributes[ fast_toupper(*sxema)-'A'] );
			putchar( *sxema);
		}
		SetConsoleTextAttribute( hConsole, console_screen_buffer_info.wAttributes);
#endif
	}
} print_sxema;
#if DIV( тесты алгоритма поиска схемы намотки )
STATIC_ASSERT( Print_sxema::test2( 24, 18) );
STATIC_ASSERT( Print_sxema::test2( 24, 20) );
STATIC_ASSERT( Print_sxema::test2( 24, 22) );
STATIC_ASSERT( Print_sxema::test2( 24, 26) );
STATIC_ASSERT( Print_sxema::test2( 24, 28) );
STATIC_ASSERT( Print_sxema::test2( 24, 30) );
STATIC_ASSERT( Print_sxema::test2( 24, 32) );
STATIC_ASSERT( Print_sxema::test2( 60, 22) );
#endif

//--------------------------------------------------------------------------------------------------------------
Param *PARAMS[] = // массив всех параметров
{ &par_slots
, &par_poles
, &par_balans
//, &print_config
, &par_q
, &par_winding_factor
, &par_cogging
, &print_sxema
};
enum { slots_col = 0 };

// продвинуть указатель на utf8 символы p на count позиций
auto utf8forward( cchar *p, ui count)
{
	while( count--)
	{
		auto c = *p++;
		if( (c & 0b11100000) == 0b11000000 ) { p++;	continue; }
		if( (c & 0b11110000) == 0b11100000 ) { p += 2;	continue; }
		if( (c & 0b11111000) == 0b11110000 ) { p += 3;	continue; }
		if( (c & 0b11111100) == 0b11111000 ) { p += 4;	continue; }
		if( (c & 0b11111110) == 0b11111100 ) { p += 5;	continue; }
	}
	return p;
}

// печать msg в колонку шириной width, с отступом margin0 в 1-ой строке,
// и margin1 у всех остальных
void marginprint( ui margin0, ui margin1, ui width, const string_view msg)
{
	cchar *p	= msg.data();
	cchar *end	= msg.length() + p;
	cchar *p1;
	while( (p1 = utf8forward( p, width)) < end)
	{
		while( *--p1 != ' ' )
			;
		printf( "%*s%.*s\n", margin0, "", int(p1 - p), p);
		p1++;
		p = p1;
		margin0 = margin1;
	}
	printf( "%*s%.*s\n", margin0, "", int(end - p), p);
}

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

	ui val0	[size(PARAMS)];
	ui val	[size(PARAMS)];

	ui found = 0;
	for( ui slots = slots_min; slots <= slots_max; slots += 3)
	{
		for( ui poles = poles_min; poles <= poles_max; poles += 2)
		{
			for( ui i = 0; i < size(PARAMS); i++)
			{
				if( ! (val[i] = PARAMS[i]->test( slots, poles)) )
					goto label1;
			}

			if( ! found++)
			{
				for( ui i = 0; i < size(PARAMS); i++)
					val0[i] = val[i];
			}
			else
			{
				for( ui i = 0; i < size(PARAMS)-1; i++) // последнюю колонку не проверяем
				{
					if( val0[i] != val[i])
						PARAMS[i]->column = true;
				}
				if( val0[slots_col] < val[slots_col] )
					val0[slots_col] = val[slots_col];
			}
		label1:;
		}
	}
	// последняя колонка всегда есть, там схема сидит
	PARAMS[ size(PARAMS)-1 ]->column = true;

	if( ! found )
	{
		printf("%s\n", _("No schemes were found"));
		return 0;
	}

	printf( "\n");
	for( ui i = 0; i < size(PARAMS); i++)
	{
		if( ! PARAMS[i]->column )
		{
			PARAMS[i]->print( val0[i]);
			printf( "\t%s\n", _(PARAMS[i]->longname) );
		}
	}
	printf( "\n");
	for( ui i = 0; i < size(PARAMS); i++)
	{
		if( PARAMS[i]->column )
			printf( "%s\t", _(PARAMS[i]->shortname));
	}
	printf( "\n");
	ui hr_len = val0[slots_col];
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
			cchar *bl = "";

			for( ui i = 0; i < size(PARAMS); i++)
			{
				if( ! (val[i] = PARAMS[i]->test( slots, poles)) )
					goto label2;
			}

			for( ui i = 0; i < size(PARAMS); i++)
			{
				if( PARAMS[i]->column)
				{
					printf("%s", bl);
					PARAMS[i]->print( val[i]);
					bl = "\t";
				}
			}
			printf("\n");
			found++;
		label2:;
		}
	}

	print_hr( hr_len );
	printf( ngettext("%u scheme found\n", "%u schemes found\n", found), found);

	return found;
}

int usage( void)
{
	printf(	"\n%s\n\n%s: " APPNAME " [-h]"
	      , _("The program to calculate the winding schemes of multi-pole electric motors (BLDC, etc.)")
	      , _("Usage")										);
	for( ui i = 0; i < size(PARAMS)-1; i++)
	{
		printf(" [");
		PARAMS[i]->usage_s();
		printf("]");
	}
	printf(	"\n\n%s:\n", _("Parameters")								);
	printf(	"\t-h\t\t%s\n", _("display this help and exit")						);
	for( ui i = 0; i < size(PARAMS); i++)
		PARAMS[i]->usage_l();

	printf( "%s:\n\t<%s>  \t", _("Where"), _("range")						);
	marginprint( 0, 24, 79-24, _(
		"is the pair of numbers separated by a '-' sign. In this pair, the first or second "
		"or even both numbers can be omitted (one '-' remains) or one number can be specified "
		"(there will be a range of one number)")						);
	printf( "%s:\n", _("Example")									);
	marginprint( 8, 8, 79-8, _(
		"For a 46-pole rotor, we will find all the winding options among balanced stators "
		"with a number of slots from 3 to 45 and a winding factor of more than 0.6")		);
	printf( "\t>>> " APPNAME " p46 b+ s3-45 w0.6-\n"						);

	par_slots.min = 3;
	par_slots.max = 45;
	par_poles.min = 46;
	par_poles.max = par_poles.min;
	par_winding_factor.min = 600000;
	par_balans.sel = Param_balans::yes;

	find_n_print_schemes();

	return EXIT_SUCCESS;
}

bool optproc( int opt, cchar *arg )
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

int main( int argc, char *const *argv )
{
	STDOUT_IS_A_TTY = isatty( STDOUT_FILENO);

#ifndef __unix__
	SetConsoleOutputCP(65001);
	if( isatty( STDERR_FILENO))
		freopen("CON", "w", stderr);

	if( STDOUT_IS_A_TTY )
	{
		hConsole = GetStdHandle( STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo( hConsole, &console_screen_buffer_info );
		// SetConsoleMode( hConsole, ENABLE_VIRTUAL_TERMINAL_PROCESSING); // под Windows7 не работает
	}
#endif
	if( ! setlocale		(LC_MESSAGES, "")) perror("setlocale(): "); // может LC_ALL ? Нет, десятичная запятая - зло!
	bindtextdomain		(APPNAME, LOCALE);
	bind_textdomain_codeset	(APPNAME,"UTF-8");
	textdomain		(APPNAME	);

	if( 1 == argc )
		return usage();

	for( int i = 1; i < argc; i++)
	{
		cchar *arg = argv[i];

		if( '-' == arg[0] )
			arg++;

		if( '\0' == arg[0] )
			continue; // что делать с отдельным '-' не знаю... Пока ничего

		if( !optproc( arg[0], &arg[1]) )
			fprintf( stderr, "%s: %s\n", arg, _("Wrong parameter"));
		/*
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
			//fprintf( stderr, "!3 '%c'%c'\n", arg[0], *argv[i+1]);
			if( !optproc( arg[0], argv[i+1]) )
				fprintf( stderr, "%c: %s\n", arg[0], _("Wrong parameter"));
			else
				i++;
			continue;
		}
		*/
	}

	return ! find_n_print_schemes();
}
