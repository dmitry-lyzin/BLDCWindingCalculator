#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <stdarg.h>
#define _INTL_NO_DEFINE_MACRO_FPRINTF	1
#define _INTL_NO_DEFINE_MACRO_VFPRINTF	1
#define _INTL_NO_DEFINE_MACRO_PRINTF	1
#define _INTL_NO_DEFINE_MACRO_VPRINTF	1
#define _INTL_NO_DEFINE_MACRO_SPRINTF	1
#define _INTL_NO_DEFINE_MACRO_VSPRINTF	1
#define _INTL_NO_DEFINE_MACRO_SNPRINTF	1
#define _INTL_NO_DEFINE_MACRO_VSNPRINTF	1
#define _INTL_NO_DEFINE_MACRO_ASPRINTF	1
#define _INTL_NO_DEFINE_MACRO_VASPRINTF	1
#include <libintl.h>
#define _USE_MATH_DEFINES
#include <complex>

#define STATIC_ASSERT(exp) static_assert( exp, #exp " FAIL!" );
#define DIV(x)	true
#define ˂	template< class
#define ˂˃	template<>
#define cØnst	const override
#define Ø	override
#define STATIC	constexpr static
#define CE	constexpr
#define OP	operator

#ifdef NDEBUG
#	define IN_DEBUG(...)
#else
#	define IN_DEBUG(...) __VA_ARGS__
#endif

#ifdef __GNUC__
#	define GNUC_EXT(...) __VA_ARGS__
#else
#	define GNUC_EXT(...)
#endif

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
#	include <consoleapi2.h>
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
char	SXEMABUF[1024];

double	CE π = M_PI;
ui	CE m = 3;		// число фаз
angle	CE ε⁰ = size(SXEMABUF);	// очень маленький угол, около 0.00000024⁰, "добавка" для компенсации потерь при делении на 3

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
	int shift;
	if( !a ) return b;
	if( !b ) return a;
	for( shift = 0; ((a | b) & 1) == 0; shift++)
	{
		a >>= 1;
		b >>= 1;
	}
	while( (a & 1) == 0)
		a >>= 1;
	do {
		while( (b & 1) == 0)
			b >>= 1;
		if( a > b)
			swap(a, b);
		b = b - a;
	} while( b != 0);
	return a << shift;
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

//--------------------------------------------------------------------------------------------------------------
template <size_t SIZE = 32 - sizeof(ui)>
struct strf
{
	strf		( cchar *fmt, ... ) GNUC_EXT( __attribute__ ((format( printf, 2, 3))))
					{
						va_list ap;
						va_start( ap, fmt);
						size = vsnprintf( data, SIZE, fmt, ap);
						va_end( ap);
						if(size >= SIZE)
						{
							assert( !"not enough space in the buffer");
							size = SIZE;
						}
					}
	strf		( int		x ): strf( "%d",	x ) {}
	strf		( ui		x ): strf( "%u",	x ) {}
	strf		( long		x ): strf( "%ld",	x ) {}
	strf		( long long	x ): strf( "%lld",	x ) {}
	strf		( ulong		x ): strf( "%lu",	x ) {}
	strf		( ulonglong	x ): strf( "%llu",	x ) {}
	strf		( float		x ): strf( "%g",	x ) {}
	strf		( double	x ): strf( "%g",	x ) {}
CE	strf		( void		  ): size(0) { data[0] = 0; }

CE OP	char*		( void	)	{ return  data;		}
CE OP	string_view	( void	)	{ return{ data, size };	}
CE	char&	OP *	( void	)	{ return *data;		}

	ui	size;
	char	data	[ SIZE	];
};

#if DIV( перегрузки для преобразований число <-> строка )
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

	fprintf( stderr, "%s:\t%s %s\n"
		, str, _("It's not a number. Used by default"), &*strf(*x));
	return false;
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

enum Color { red, green, blue };

void color_print( Color color, cchar *str )
{
	if( ! STDOUT_IS_A_TTY )
	{
		printf( "%s", str);
		return;
	}

#ifdef __unix__
	// ANSI escape color codes
	static cchar *color_codes[3] =
	{ "91" // Bright Red // "93" // Bright Yellow
	, "92" // Bright Green
	, "94" // Bright Blue // "96" // Bright Cyan
	};

	assert( ui(color) < size(color_codes) );
	printf( "\033[%sm%s", color_codes[color], str);
#else
	static WORD attributes[3] =
	{ FOREGROUND_INTENSITY	| FOREGROUND_RED
	, FOREGROUND_INTENSITY	| FOREGROUND_GREEN
	, FOREGROUND_INTENSITY	| FOREGROUND_BLUE
	};

	assert( ui(color) < size(attributes) );
	SetConsoleTextAttribute( hConsole, attributes[color] );
	printf( str);
#endif
}

void color_print( Color color, cchar c )
{
	char str[] = { c, 0 };
	color_print( color, str );
}

void color_print_reset()
{
#ifdef __unix__
	printf( "\033[0m");
#else
	SetConsoleTextAttribute( hConsole, console_screen_buffer_info.wAttributes);
#endif
}

//--------------------------------------------------------------------------------------------------------------
union Val
{
CE	Val		(				): u( 0					){}
CE OP	bool		(				) const	{ return u;			}
CE	bool	OP ==	( const Val &x			) const	{ return u == x.u;		}
CE	bool	OP !=	( const Val &x			) const	{ return u != x.u;		}
CE	bool	OP <	( const Val &x			) const	{ return u <  x.u;		}

CE	Val		( ui x				): u( x					){}
CE	Val		( int x				): u( x					){}
CE OP	ui		(				) const	{ return u;			}
CE	ui*	OP &	(				)	{ return & u;			}

CE	Val		( ui slots, ui poles, ui layers	): conf{ layers - 1, slots, poles	}{}
CE	ui	layers	(				) const	{ return conf.layers + 1;	}
CE	ui	slots	(				) const	{ return conf.slots;		}
CE	ui	poles	(				) const	{ return conf.poles;		}

CE	Val		( double x			): u( ui((x + .5/scale) * scale)	){}
CE OP	double		(				) const	{ return double(u) / scale;	}
	void	doublef	(				) const
	{
		if( u < scale)	printf( ".%0*d", nuls, u	);
		else		printf( "%g", double(u) / scale	);
	}

private:
	ui	u;
	struct
	{
		ui	layers:	1;
		ui	slots:	(sizeof(u)*8 / 2)-1;
		ui	poles:	(sizeof(u)*8 / 2);
	} conf;

STATIC	ui	nuls	= 6;
STATIC	ui	scale	= pow10( nuls);
};

//--------------------------------------------------------------------------------------------------------------
struct Opt
{
CE	Opt		( char _chr, cchar *_shortname, cchar *_longname)
	: column(false), chr(_chr), shortname(_shortname), longname(_longname) {}

	cchar*	shortname;
	cchar*	longname;
	char	chr;
	bool	column;

virtual	strf<>	usage_s	( void				) const	{ return { "%c", chr };				}
virtual	void	usage_l	( void				) const	{ printf("\t%s\t%s\n", &*usage_s(),_(longname));}
virtual	Val	test	( ui slots, ui poles, ui layers	) const	= 0;
virtual	void	print	( Val val			) const	= 0;
virtual	bool	load	( cchar *arg			)	= 0;
};

//--------------------------------------------------------------------------------------------------------------
struct Opt_balans: Opt
{
CE	Opt_balans	( void			): Opt( 'b', "balanc", "stator balance"), var(any) {}
enum	Variant						{ any = 0, yes = 1, no = 2				};
	Variant	var;
virtual	strf<>	usage_s	( void			) cØnst	{ return { "%c[+|-|any]", chr };			}
virtual	Val	test	( ui slots,ui, ui layers) cØnst { ui r = (slots / (layers==1 ? 2 : 1)) % 2 + 1
							; return{ !var || var == r ? r : 0 };
							}
virtual	void	print	( Val val		) cØnst
	{
		static Color cl[] = { green,	red	};
		static cchar *c[] = { "√",	"×"	};
		color_print( cl[ui(val)-1], c[ui(val)-1] );
		color_print_reset();
	}
virtual	bool	load	( cchar *arg		) Ø
	{
		switch( arg[0])
		{
		case  0 :
		case '+':
		case ' ':
		case 'y': var = yes;	return true;

		case '-':
		case 'n': var = no;	return true;

		case 'a':
		case 'x': var = any;	return true;
		default:		return false;
		}
		assert( false);
		return false;
	}
} opt_balans;

//--------------------------------------------------------------------------------------------------------------
struct Opt_range: Opt
{
CE	Opt_range	( char chr, cchar *shortname, cchar *longname, Val _min, Val _max)
	: Opt( chr, shortname, longname), min(_min), max(_max) {}

	Val	min;
	Val	max;
CE	Val	in_range( Val val		) const	{ return (max < val || val < min) ? Val() : val;}

virtual	strf<>	usage_s	( void			) cØnst	{ return { "%c<%s>", chr, _("range") };		}
virtual	void	print	( Val val		) cØnst	{ printf( "%u", ui(val) );			}
virtual	bool	load	( cchar *arg		) Ø
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
struct Opt_range_step: Opt_range
{
CE	Opt_range_step( char chr, cchar *shortname, cchar *longname, Val min, Val max, ui _step)
	: Opt_range( chr, shortname, longname, min, max), step(_step) {}

	ui	step;
virtual	bool	load	( cchar *arg	)
	{
		if( !Opt_range::load( arg))
			return false;
		if( step && ( ! min || ui(min) % step) )
		{
			fprintf( stderr, "%c%s:\t", chr, arg);
			fprintf( stderr, _("The number of %s must be divisible by %u!"), _(longname), step );
			min = ui(min) - ui(min) % step;
			fprintf( stderr, "\n\t%s: %u\n", _("Let's take the nearest suitable one"), ui(min) );
			return false;
		}

		return true;
	}
};

//--------------------------------------------------------------------------------------------------------------
struct Opt_layers		final: Opt_range
{
CE	Opt_layers	( void			): Opt_range( 'l', "layer", "layer winding", 2, 2) {}
virtual	Val	test	( ui, ui, ui layers	) cØnst	{ return layers; }
virtual	void	print	( Val val		) cØnst	{ static cchar* c[] = {"1(LRK)", "2(PLRK)"} //{"single", "double"}
							; printf( "%s", c[ui(val)-1]);
							}
virtual	bool	load	( cchar *arg		) Ø
	{
		if( !Opt_range::load( arg))
			return false;

		if(  1 > ui(min) || ui(min) > 2
		  || 1 > ui(max) || ui(max) > 2
		  )
		{
			fprintf( stderr, "%c%s:\t%s: %s 1-2!\n", chr, arg, _(longname), _("range") );
			exit( EXIT_FAILURE);
		}

		return true;
	}
} opt_layers;

//--------------------------------------------------------------------------------------------------------------
struct Opt_poles		final: Opt_range_step
{
CE	Opt_poles	( void			): Opt_range_step( 'p', "poles", "magnet poles", 2, 100, 2) {}
virtual	Val	test	( ui, ui poles, ui	) cØnst	{ return poles; }
} opt_poles;

//--------------------------------------------------------------------------------------------------------------
struct Opt_slots		final: Opt_range_step
{
CE	Opt_slots	( void			): Opt_range_step( 's', "slots", "slots in the stator", m, 99, m) {}
virtual	Val	test	( ui slots, ui, ui	) cØnst	{ return slots; }
virtual	bool	load	( cchar *arg		) Ø
	{
		if( !Opt_range_step::load( arg))
			return false;

		if( ui(max) >= size(SXEMABUF)/2 )
		{
			fprintf( stderr, "%c%s:\t%s (> %u)!\n", chr, arg, _("Too many slots"), ui( size(SXEMABUF)/2) );
			exit( EXIT_FAILURE);
		}

		return true;
	}
} opt_slots;

//--------------------------------------------------------------------------------------------------------------
struct Opt_cogging		final: Opt_range
{
CE	Opt_cogging	( void			): Opt_range( 'c', "cogging", "cogging steps", 0u, UINT_MAX) {}
virtual	Val	test	( ui slots, ui poles, ui) cØnst	{ return in_range( НОК( slots, poles) );	}
} opt_cogging;

//--------------------------------------------------------------------------------------------------------------
struct Opt_symmetries	final: Opt_range
{
CE	Opt_symmetries	( void				)
	: Opt_range( 'g', "groups", "number of symmetrical groups of windings", 0u, UINT_MAX) {}
virtual	Val	test	( ui slots, ui poles, ui layers	) cØnst
	{ return in_range( НОД	( slots / (layers==1 ? 2 : 1)
				, poles / 2
				)
	); }
} opt_symmetries;

//--------------------------------------------------------------------------------------------------------------
struct Opt_reduction		final: Opt_range
{
CE	Opt_reduction	( void			): Opt_range( 'r', "ƒ/ν", "reduction (ƒ/ν)", 0u, UINT_MAX) {}
virtual	Val	test	( ui slots, ui poles, ui) cØnst	{ return in_range( НОК( slots, poles)/6 );	}
} opt_reduct;

//--------------------------------------------------------------------------------------------------------------
struct Print_only: Opt
{
CE	Print_only	( cchar *shortname, cchar *longname	): Opt( 0, shortname, longname) {}
virtual	bool	load	( cchar *arg				) Ø	{ return false;		}
};

//--------------------------------------------------------------------------------------------------------------
struct Opt_range_01: Opt_range
{
CE	Opt_range_01	( char chr, cchar *shortname, cchar *longname)
	: Opt_range( chr, shortname, longname, 0., 1.) {}

virtual	void	print	( Val val		) cØnst	{ val.doublef();	}
virtual	bool	load	( cchar *arg		) Ø
	{
		double d_min = min;
		double d_max = max;
		ui x = num_sign_num( arg, '-', &d_min, &d_max);
		min = d_min;
		max = d_max;

		switch( x )
		{
		case 0:	return false;
		case 1:	max = min;
		case 2:
			if( d_min > 1. || d_max > 1. )
			{
				fprintf( stderr, "%c%s:\t%s > 1 ?\n", chr, arg, _(longname) );
				exit( EXIT_FAILURE);
			}
			return true;
		}
		assert( false);
		return false;
	}
};

//--------------------------------------------------------------------------------------------------------------
struct Opt_q			final: Opt_range_01
{
	CE	Opt_q	( void			): Opt_range_01( 'q', "q", "slots/poles/phases (q)") {}
virtual	Val	test	( ui slots, ui poles, ui) cØnst	{ return in_range( double(slots/m)/poles );	}
} opt_q;

//--------------------------------------------------------------------------------------------------------------
struct Print_q_fraction		final: Print_only
{
CE	Print_q_fraction( void			): Print_only( "q", "fraction") {}
virtual	Val	test	( ui slots, ui poles, ui) cØnst	{ slots /= m
							; ui nod = НОД( slots, poles)
							; return { slots/nod, poles/nod, 0 };
							}
virtual	void	print	( Val val		) cØnst	{ printf( "%u/%u", val.slots(), val.poles() );	}
} print_q_fraction;

//--------------------------------------------------------------------------------------------------------------
struct Opt_winding_factor	final: Opt_range_01
{
CE	Opt_winding_factor( void		): Opt_range_01( 'w', "WF", "winding factor") {}

virtual	Val	test	( ui slots, ui poles, ui layers	) cØnst	{return in_range( calcWF(slots, poles, layers));}
static	double	calcWF	( ui slots, ui poles, ui layers	)
	{
		ui isSPS = 0;
		if(layers == 1)
			isSPS = 1;

		Complex	Δâ = std::polar( 1., π / slots * poles );
		angle	Δα = div_mul( 180⁰, slots, poles);
		Complex	â  = 1;
		angle	α  = 30⁰ + ε⁰;
		Complex	ȓ  = 0;

		STATIC const double T = 0.25; // 0.55
		static const double fasen_A = sin(T * 2 * π);
		static const double fasen_B = sin(T * 2 * π + (2 * π)/3);
		static const double fasen_C = sin(T * 2 * π - (2 * π)/3);

		double EMF0 = 0;
		for( ui i = 0; i < slots; i++ )
		{
			double EMF = 0;
			if( !(i & isSPS) )
			{
					if( α <  60⁰ )	EMF =  fasen_A;
				else	if( α < 120⁰ )	EMF = -fasen_B;
				else	if( α < 180⁰ )	EMF =  fasen_C;
				else	if( α < 240⁰ )	EMF = -fasen_A;
				else	if( α < 300⁰ )	EMF =  fasen_B;
				else			EMF = -fasen_C;
			}
			ȓ += â * (EMF - EMF0);
			EMF0 = EMF;

			â *= Δâ;
			α += Δα;
		}
		ȓ -= EMF0;

		return 2 * abs(ȓ) / (slots * layers);
	}
} opt_winding_factor;

//--------------------------------------------------------------------------------------------------------------
struct Print_sxema		final: Print_only
{
CE	Print_sxema	( void			): Print_only( "winding scheme", "winding scheme") {}
STATIC	bool	test0	( ui slots, ui poles	)	{ return poles % НОД( slots, m*poles);			}
STATIC	bool	test1	( ui slots, ui poles	)
	{
		if( slots == poles )
			return false;

		assert( slots % m == 0 && slots >= m );	// Количество пазов не делится на число фаз!
		assert( poles % 2 == 0 && poles >= 2 );	// Не четное количество полюсов!

		angle α = 30⁰ + ε⁰;
		angle ρ = div_mul( 180⁰, slots, poles);
		ui a = 0, b = 0, c = 0, A = 0, B = 0, C = 0;

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
STATIC	bool	test2	( ui slots, ui poles	)	{ return test0(slots, poles) == test1(slots, poles);	}
virtual	Val	test	( ui slots, ui poles, ui layers	) cØnst
	{
		if( layers == 1 && slots % 2 )	return Val();
		if( ! test0( slots, poles) )	return Val();
						return Val( slots, poles, layers);
	}
virtual	void	print	( Val val		) cØnst
	{
		ui layers = val.layers();
		ui slots = val.slots();
		ui poles = val.poles();

		assert( slots != poles );		// Количество полюсов равно количеству пазов!
		assert( slots % m == 0 && slots );	// Количество пазов не делится на число фаз!
		assert( poles % 2 == 0 && poles );	// Нечетное количество полюсов!

		if( layers == 1)
			slots /= 2;

		angle Δα = div_mul( 180⁰, slots, poles);

		char *sxema = SXEMABUF;
		angle α = 30⁰ + ε⁰;
		IN_DEBUG( ui a = 0, b = 0, c = 0, A = 0, B = 0, C = 0; )

		for( ui i = 0; i < slots; i++ )
		{
			     if( α <  60⁰ ) { *sxema++ = 'A'; IN_DEBUG( A++; ) }
			else if( α < 120⁰ ) { *sxema++ = 'b'; IN_DEBUG( b++; ) }
			else if( α < 180⁰ ) { *sxema++ = 'C'; IN_DEBUG( C++; ) }
			else if( α < 240⁰ ) { *sxema++ = 'a'; IN_DEBUG( a++; ) }
			else if( α < 300⁰ ) { *sxema++ = 'B'; IN_DEBUG( B++; ) }
			else                { *sxema++ = 'c'; IN_DEBUG( c++; ) }

			α += Δα;
		}

		assert( a == b && a == c && A == B && A == C);

		//printf("\t\t%.*s\n", slots, BUF);
		memcpy( sxema, SXEMABUF, slots);
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

		// если схема не кончается фазой C...
		if( fast_toupper( sxema[ slots-1]) != 'C')
		{
			// меняем фазы B и C местами
			char *p = sxema;
			for( ui i = 0; i < slots; i++, p++ )
				if( fast_toupper(*p) != 'A' )
					*p ^= ('B' ^ 'C');
		}

		if( layers == 1)
		{
			if( ! STDOUT_IS_A_TTY )
			{
				for( ui i = 0; i < slots; sxema++, i++ )
				{
					putchar( *sxema	);
					putchar( '-'	);
				}
				return;
			}
			for( ui i = 0; i < slots; sxema++, i++ )
			{
				color_print( Color(fast_toupper(*sxema)-'A'), *sxema);
				color_print_reset();
				putchar('-');
			}
		}
		else
		{
			if( ! STDOUT_IS_A_TTY )
			{
				printf("%.*s", slots, sxema);
				return;
			}
			for( ui i = 0; i < slots; sxema++, i++ )
				color_print( Color(fast_toupper(*sxema)-'A'), *sxema);
			color_print_reset();
		}
	}
} print_sxema;
#if DIV( тесты алгоритма поиска схемы намотки )
STATIC_ASSERT( Print_sxema::test2( 156,	58)  );
STATIC_ASSERT( Print_sxema::test2( 156,	370) );
STATIC_ASSERT( Print_sxema::test2( 168,	62)  );
STATIC_ASSERT( Print_sxema::test2( 300,	110) );
STATIC_ASSERT( Print_sxema::test2( 300,	410) );
STATIC_ASSERT( Print_sxema::test2( 312,	116) );
STATIC_ASSERT( Print_sxema::test2( 312,	466) );
STATIC_ASSERT( Print_sxema::test2( 324,	174) );
STATIC_ASSERT( Print_sxema::test2( 360,	132) );
STATIC_ASSERT( Print_sxema::test2( 360,	494) );
STATIC_ASSERT( Print_sxema::test2( 372,	230) );
STATIC_ASSERT( Print_sxema::test2( 372,	418) );
STATIC_ASSERT( Print_sxema::test2( 396,	146) );
STATIC_ASSERT( Print_sxema::test2( 396,	494) );
STATIC_ASSERT( Print_sxema::test2( 408,	140) );
STATIC_ASSERT( Print_sxema::test2( 408,	500) );
STATIC_ASSERT( Print_sxema::test2( 420,	154) );
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
Opt *OPTIONS[] = // массив всех опций
{ &opt_slots
, &opt_poles
, &opt_layers
, &opt_balans
, &opt_symmetries
, &opt_cogging
, &opt_q
, &print_q_fraction
, &opt_winding_factor
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
	const ui slots_min	= opt_slots.min;
	const ui slots_max	= opt_slots.max;
	const ui poles_min	= opt_poles.min;
	const ui poles_max	= opt_poles.max;
	const ui layers_min	= opt_layers.min;
	const ui layers_max	= opt_layers.max;

	Val val0[size(OPTIONS)];
	Val val	[size(OPTIONS)];

	ui found = 0;
	for( ui slots = slots_min; slots <= slots_max; slots += m)
	{
		for( ui poles = poles_min; poles <= poles_max; poles += 2)
		{
			for( ui layers = layers_min; layers <= layers_max; layers++)
			{
				for( ui i = 0; i < size(OPTIONS); i++)
				{
					if( ! (val[i] = OPTIONS[i]->test( slots, poles, layers)) )
						goto label1;
				}

				if( ! found++)
				{
					for( ui i = 0; i < size(OPTIONS); i++)
						val0[i] = val[i];
				}
				else
				{
					for( ui i = 0; i < size(OPTIONS)-1; i++) // последнюю колонку не проверяем
					{
						if( val0[i] != val[i])
							OPTIONS[i]->column = true;
					}
					if( val0[slots_col] < val[slots_col] )
						val0[slots_col] = val[slots_col];
				}
			label1:;
			}
		}
	}
	// последняя колонка всегда есть, там схема сидит
	OPTIONS[ size(OPTIONS)-1 ]->column = true;

	if( ! found )
	{
		printf("%s\n", _("No schemes were found"));
		return 0;
	}

	printf( "\n");
	for( ui i = 0; i < size(OPTIONS); i++)
	{
		if( ! OPTIONS[i]->column )
		{
			OPTIONS[i]->print( val0[i]);
			printf( "\t%s\n", _(OPTIONS[i]->longname) );
		}
	}
	printf( "\n");
	for( ui i = 0; i < size(OPTIONS); i++)
	{
		if( OPTIONS[i]->column )
			printf( "%s\t", _(OPTIONS[i]->shortname));
	}
	printf( "\n");
	ui hr_len = val0[slots_col];
	for( ui i = 0; i < size(OPTIONS)-1; i++)
	{
		if( OPTIONS[i]->column )
			hr_len += 8;
	}

	print_hr( hr_len );

	found = 0;
	for( ui slots = slots_min; slots <= slots_max; slots += m)
	{
		for( ui poles = poles_min; poles <= poles_max; poles += 2)
		{
			for( ui layers = layers_min; layers <= layers_max; layers++)
			{
				cchar *bl = "";

				for( ui i = 0; i < size(OPTIONS); i++)
				{
					if( ! (val[i] = OPTIONS[i]->test( slots, poles, layers)) )
						goto label2;
				}

				for( ui i = 0; i < size(OPTIONS); i++)
				{
					if( OPTIONS[i]->column)
					{
						printf("%s", bl);
						OPTIONS[i]->print( val[i]);
						bl = "\t";
					}
				}
				printf("\n");
				found++;
			label2:;
			}
		}
	}

	print_hr( hr_len );
	printf( ngettext("%u scheme found\n", "%u schemes found\n", found), found);

	return found;
}

int usage( void)
{
	printf(	APPNAME " v" VERSION "\nДмитрий Лызин <dmitry_lyzin@mail.ru>\n%s\n\n%s:\n"
	      , _("The program to calculate the winding schemes of multi-pole electric motors (BLDC, etc.)")
	      , _("USAGE")										);
	int n = 79 - printf( "        " APPNAME " [-h]");
	for( ui i = 0; i < size(OPTIONS)-1; i++)
	{
		if( ! OPTIONS[i]->chr )
			continue;
		if( (n -= printf(" [%s]", &*(OPTIONS[i]->usage_s())) ) < 0 )
			n = 79 - printf("\n       ");
	}
	printf(	"\n%s:\n", _("OPTIONS")									);
	printf(	"\t-h\t\t%s\n", _("display this help and exit")						);
	for( ui i = 0; i < size(OPTIONS); i++)
	{
		if( ! OPTIONS[i]->chr )
			continue;
		OPTIONS[i]->usage_l();
	}
	printf( "\n%s:\n\t<%s>  \t", _("ARGS"), _("range")						);
	marginprint( 0, 24, 79-24, _(
		"is the pair of numbers separated by a '-' sign. In this pair, the first or second "
		"or even both numbers can be omitted (one '-' remains) or one number can be specified "
		"(there will be a range of one number)")						);
	printf( "%s:\n", _("EXAMPLE")									);
	marginprint( 8, 8, 79-8, _(
		"For a 46-pole rotor, we will find all the winding options among balanced stators "
		"with a number of slots from 3 to 45 and a winding factor of more than 0.6")		);
	printf( "\n> " APPNAME " p46 b+ s3-45 w0.6-\n"							);

	opt_slots.min = 3;
	opt_slots.max = 45;
	opt_poles.min = 46;
	opt_poles.max = opt_poles.min;
	opt_winding_factor.min = 0.6;
	opt_balans.var = Opt_balans::yes;

	find_n_print_schemes();

	return EXIT_SUCCESS;
}

bool optproc( int opt, cchar *arg )
{
	//printf("!!! %c, \"%s\"\n", opt, arg);
	if( opt == 'h')
		exit( usage() );

	for( ui i = 0; i < size(OPTIONS); i++)
	{
		if( opt == OPTIONS[i]->chr )
			return OPTIONS[i]->load( arg);
	}
	return false;
}

int main( int argc, char *const *argv )
{
#if DIV( тесты )
	//printf( "@@@ %g\n", Opt_winding_factor::calcWF( 54, 54, 2));
	assert( abs( Opt_winding_factor::calcWF( 60, 32, 2) - 0.710950) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 60, 34, 2) - 0.742459) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 60, 38, 2) - 0.801237) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 60, 40, 2) - 0.866025) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 42, 14, 2) - 0.500000) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 42, 16, 2) - 0.538433) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 42, 20, 2) - 0.650123) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 42, 22, 2) - 0.700666) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 42, 26, 2) - 0.789736) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 42, 28, 2) - 0.866025) < 0.000001 );

	assert( abs( Opt_winding_factor::calcWF( 3, 2, 2) - 0.866025) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 3, 4, 2) - 0.866025) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 3, 8, 2) - 0.866025) < 0.000001 );

	assert( abs( Opt_winding_factor::calcWF( 30, 46, 2) - 0.640142) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 36, 46, 2) - 0.866560) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 42, 46, 2) - 0.945145) < 0.000001 );

	assert( abs( Opt_winding_factor::calcWF( 18, 46, 2) - 0.735246) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 18, 46, 1) - 0.735246) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 36, 22, 2) - 0.783226) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 36, 22, 1) - 0.786218) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 12, 10, 2) - 0.933013) < 0.000001 );
	assert( abs( Opt_winding_factor::calcWF( 12, 10, 1) - 0.965926) < 0.000001 );
#endif

	STDOUT_IS_A_TTY = isatty( STDOUT_FILENO);

#ifdef __unix__
	cchar *locale = LOCALE;
#else
	cchar *locale = ".\\locale";
	cchar *fp = strrchr( argv[0], '\\'); // полный путь к exe'шнику
	if( fp )
		locale = strf<1024>( "%.*s\\locale", int(fp - argv[0]), argv[0]);

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
	if( ! setlocale		(LC_MESSAGES, ""))	 // может LC_ALL ? Нет, десятичная запятая - зло!
		perror( strf<1024>("setlocale(LC_MESSAGES, \"%s\")", getenv("LANG")) );
	bindtextdomain		(APPNAME, locale);
	bind_textdomain_codeset	(APPNAME,"UTF-8");
	textdomain		(APPNAME	);

	if( 1 == argc )
	{
		usage();
		#ifndef __unix__
			if( !fp && STDOUT_IS_A_TTY )
			{
				printf( _("\nWHAT TO DO RIGHT NOW:\n") );
				marginprint( 8, 8, 79-8
					, strf<1024>( _("Press CTRL-F until \"%s\" appears, add your "
						"parameters (for example, \" p26 w0.8-\"), press ←┘ and joy. "
						"Then just press ↑, edit the parameters, then ←┘, be happy "
						"and continue in a circle until you get bored ;)")
						, argv[0]
						)
					);
			}
		#endif
		return EXIT_SUCCESS;
	}

	for( int i = 1; i < argc; i++)
	{
		cchar *arg = argv[i];

		if( '-' == arg[0] )
			arg++;

		if( '\0' == arg[0] )
			continue; // что делать с отдельным '-' не знаю... Пока ничего

		if( !optproc( arg[0], &arg[1]) )
			fprintf( stderr, "%s:\t%s\n", arg, _("Incorrect option"));
		/*
		if( arg[1] )
		{
			if( !optproc( arg[0], &arg[1]) )
				fprintf( stderr, "%s:\t%s\n", arg, _("Incorrect option"));
			continue;
		}

		if( i+1 >= argc )
		{
			if( !optproc( arg[0], "") )
				fprintf( stderr, "%s:\t%s\n", arg, _("Incorrect option"));
			continue;
		}
		else
		{
			//fprintf( stderr, "!3 '%c'%c'\n", arg[0], *argv[i+1]);
			if( !optproc( arg[0], argv[i+1]) )
				fprintf( stderr, "%c:\t%s\n", arg[0], _("Incorrect option"));
			else
				i++;
			continue;
		}
		*/
	}

	return ! find_n_print_schemes();
}
