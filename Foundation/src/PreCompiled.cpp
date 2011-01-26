//Global.cpp


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Fresco.h"
#include "Fresco/Foundation/Except.h"
#include "Fresco/Foundation/RasterIO.h"
#include "Poco/Path.h"
//#include <ios>
//#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <fcntl.h>
#ifndef WIN32
#include <unistd.h>
#endif


void					ShowOutput(const EDetailLevel DetailLevel, const std::string output)
//Send the output to FRESCO.  FRESCO then sends the output to cout as well as 
//passing it to any clients using an event.
{
	if (gDetailLevel>=DetailLevel)
		FRESCO->output(output);
}
void                    DoNothing(const bool doBreak)
{
    //This is just a place to put a break point.  Created to verify that a constructor is being called.
    std::cout << std::endl << "DEBUG: DoNothing()" << std::endl;
    FRESCO->output("DEBUG: DoNothing()");
}
void					ShowOutput(const std::string output)
//No detail level specified, so force output by using MINIMAL.
{
	ShowOutput(MINIMAL, output);
}
void					ShowOutput(const EDetailLevel DetailLevel, std::ostringstream& rOutput)
{
	ShowOutput(DetailLevel, rOutput.str());
	rOutput.str("");
}
void					ShowOutput(std::ostringstream& rOutput)
//No detail level specified, so force output by using MINIMAL.
{
	ShowOutput(MINIMAL, rOutput);
}
void                    WriteDebug(const std::string message)
{
    if (FRESCO->isDebugOn())
        std::cout << "DEBUG: " << message << std::flush;
}
std::string				AppendRepYear(const std::string file, const int rep, const int year) 
//Inserts the rep and year (_rep_year) into the filename in front of the last decimal point so as not to corrupt the file extension.
{
	std::string result;
	result = file.substr(0,file.find_last_of(".")) + "_" + ToS(rep) + "_" + ToS(year) + file.substr(file.find_last_of("."),file.length()) + '\0';
	return result;
}

std::string				AppendYearMonth(const std::string file, const int year, const int month) 
//Inserts the year and month (_year_month) into the filename in front of the last decimal point so as not to corrupt the file extension.
{
	std::string result;
	result = file.substr(0,file.find_last_of(".")) + "_" + ToS(year) + "_" + ToS(month) + file.substr(file.find_last_of("."),file.length()) + '\0';
	return result;
}

std::string				AppendYear(const std::string file, const int year) 
//Inserts the year into the filename in front of the last decimal point so as not to corrupt the file extension.
{
	std::string result;
	result = file.substr(0,file.find_last_of(".")) + "_" + ToS(year) + file.substr(file.find_last_of("."),file.length()) + '\0';
	return result;
}

bool                    InputFileExists(std::string absoluteOrRelativeFilePath)
{
	std::string f = GetFullPath(gInputBasePath, absoluteOrRelativeFilePath);
    std::fstream file(f.c_str(), std::ios_base::in);
    bool doesFileExist = file.is_open();
    file.close();    
    return doesFileExist;
}

std::string             FormatDirectory(std::string path)
//Ensure proper format for native OS.
{
    Poco::Path d(path, Poco::Path::PATH_GUESS);
    return d.toString(Poco::Path::PATH_NATIVE);
}

void                    EnsureDirectoryExists(std::string path, bool includesFilename)
//Ensure each part of the path exists.
{
    Poco::Path p(path, Poco::Path::PATH_GUESS);
	Poco::Path d(true);
	if (!p.getDevice().empty())
		d.setDevice(p.getDevice());
	else if(!p.getNode().empty())
		d.setNode(p.getNode());

    int depth = includesFilename ? p.depth() : p.depth() + 1;
    for (int i=0; i<depth; i++)
    {
        d.pushDirectory(p.directory(i));
        Poco::File f(d);
        try { f.createDirectory(); } catch (Poco::Exception& e) 
		{
			if (!f.exists())
				throw Exception(Exception::UNKNOWN, "Unable to create the client output directory: "+f.path()+",  System message:"+e.displayText()+"\n");
		}
    }
}

std::string GetFullPath(const std::string base, const std::string path)
{
	if (path.empty())
		return path;

	Poco::Path p(path, Poco::Path::PATH_GUESS);
	Poco::Path b(base, Poco::Path::PATH_GUESS);
	Poco::Path fp;
	bool isAlreadyFullPath = (!p.getDevice().empty() || !p.getNode().empty());
	if (!isAlreadyFullPath) 
	{
		fp = p.absolute(b);
	}
	else
	{
		fp.assign(p);
	}
	return fp.toString(Poco::Path::PATH_NATIVE);
}


#ifdef _WIN32
    typedef std::ios_base::open_mode IosOpenMode;
#else
	typedef std::_Ios_Openmode IosOpenMode;
#endif


double					ConstDist(const double* const params) 
//A dummy distribution function usually used for debugging and testing. It returns whatever value it is
//told to by the second parameter of the argument Parms.
{
	return params[1];
}


double					NormDist(const double* const params) 
//This function inplements the normal distribution for use by various functions - primarily the neighbors 
//algorithm and related functions. The constant at the end of the function ensures that the integral over
//the range is unity to ensure there is no probability bias added from the distribution. The arguments
//expected are:
//		First : x value (distance)
//		Second : mean
//		Third : stddev
{
	return exp(-0.5 * (params[0]-params[1])*(params[0]-params[1]) / (params[2]*params[2])) / (params[2]*sqrt(2*PI));
}


double					WeibullDist(const double* const params) 
//This function implements the Weibull CDF function for use by various functions, esp. the initial age 
//generation algorithm.  The arguments expected are :
//		First : x value (time)
//		Second : Expected lifetime (denomonator of time fraction)
//		Third : Shape parameter (exponent to time fraction)
{

	return 1 - exp ( -pow (params[0]/params[1],params[2]) );
}


double					WeibullReliability(const double* const params) 
//This function implements the Weibull reliability function for use by various functions, esp. the initial
//age generation algorithm.  The arguments expected are :
//		First : x value (time)
//		Second : Expected lifetime (denomonator of time fraction)
//		Third : Shape parameter (exponent to time fraction)
{
	return exp ( -pow (params[0]/params[1],params[2]) );
}


double					WeibullHazard(const double* const params) 
//This function is actually NOT the standard Weibull hazard function, but is in fact the conditional probability
//of an event occuring given the age and step size used.  For more information see the considerable documentation
//I have written on this topic.  The arguments expected are :
//		First : x value (time)
//		Second : Step size
//		Third : Expected lifetime (donomonator of time fraction)
//		Fourth : Shape parameter (exponent to time fraction)
{
	return exp ( pow(params[0]/params[2],params[3]) - pow((params[0]+params[1])/params[2],params[3]) );
}


double					FatTail(const double* const params) 
//This is the fat tailed seed distribution function from Clark, J.S., The American Naturalist 1998. 152:2,204-224
//with some modifications to suit our particular application.  An example of its use can be seen in the sample
//Excel model I used to prepare the Tundra algorithm Basal Growth.xls.  Parameters are:
//		First: x value (distance)
//		Second: alpha - seed dispersal distance
//		Third: p - probability
{
	return ( params[2]*exp(-params[0]/params[1]) + (1-params[2])*exp(-sqrt(params[0]/params[1])) )/(2*params[1]);
}


double					Site(const double site, const double exp) 
//This function returns a weighted site factor based on the site.  It simply converts the aspect based on a sinusoid
//function so due south returns 1. and due north returns 0.  It is also possible to modify the shape of the sinusoid
//so that it is flatter at the extremes and steeper around 0.  This is done using the argument Pow which is the exponent
//to be used.  Not that in cases where Pow < 0 care needs to be taken to ensure the curve is in fact continuous.  In othe
//words, assume the root of a negative number is the negative of the root of the negation of that number:
//		i.e. sqrt(x) (is assumed) = -sqrt(-x) when x < 0
{
	double temp = cos(site * PI / 180.);
	int root = 1;								//A multiplier to ensure we don't try and take a root of a -ve number
	if (temp < 0 && ((int)exp) != exp)			//If we have a fractional exponent and we are in the negative range...
		root = -1;
	return (root * pow(root*temp,exp) + 1.) / 2.;
}

double					Logistic(const double age, const double* const params) 
//This function is an instance of the logisitic equation k/(1+exp(a-b*x)).  It is used primarily to convert age to
//a fire factor.  The parms represents the parameters to the function k, a, b respectively.  These are: k is the 
//maximum fire factor for that species, b is the rate at which the curves swings up (higher is a sharper transition)
//and a controls the point where maximum slope occurs.  The latter occurs at a = b * Critical Age.
{
	return params[0]/(1+exp(params[1]-params[2]*age));
}


double					LinInterp(double x, double y, const double* const m, const int dimx, const int dimy, const bool Trunc) 
//This function uses linear interpolation to find the value m[x,y] using the floating point values x and y.  This
//function is based on the bilinear interpolation algorithm outlined in Numerical Recipies in C section 3.6 modified
//to handle boundaries more effectively.
//Note the assumption we are working under:
//		We assume that the matrix coefficients represents integer values (indicies) and so the step size is 1
//		We also assume that the x (and y) range is [0,dimx-1] (i.e. the first value in m is at m[0][0]).  If the data
//			does not conform to this, x and y should be transformed before being passed to this function. Only basic
//			range checking is performed.
//Arugments are:
//		x, y are the values to lookup in m
//		dimx, dimy are the x and y dimensions respectively
//		Trunc indicates if the x,y values should be truncated to correspond to the nearest boundary of the matrix - set
//			to true by default
{
	//Perform range checking and apply corrections if we are at the maximum boundary
	if (Trunc) {										//If we are to truncate at the boundary do so
		x = (x < 0) ? 0 : x;
		x = (x > dimx-1) ? dimx-1 : x;
	}
	int nx = (int) floor(x);							//Compute the next lowest integer as the interpolation bin starting point
	nx = (nx < 0) ? 0 : nx;								//Check that it is also in range
	nx = (nx > dimx-2) ? dimx-2 : nx;					
	double t = x - nx;									//Find the offset

	if (Trunc) {										//Repeat for y
		y = (y < 0) ? 0 : y;
		y = (y > dimy-1) ? dimy-1 : y;
	}
	int ny = (int) floor (y);
	ny = (ny < 0) ? 0 : ny;
	ny = (ny > dimy-2) ? dimy-2 : ny;
	double u = y - ny;

	//Perform interpolation
	return (1-t)*(1-u)*m[nx*dimy+ny] + t*(1-u)*m[(nx+1)*dimy+ny] + t*u*m[(nx+1)*dimy+(ny+1)] + (1-t)*u*m[nx*dimy+(ny+1)];
}


void					Integrate(double (*Func)(const double* const), const double* Parms, double* const Result, const double Start, const double End, const double Step) 
//This function computes an approximate discrete integral (numerical) of a function which is otherwise not 
//integrable. It will compute the integral from Start to End and will store the value every Step values.  The
//results are stored in result, and it is assumed that this datastructure is large enough to house all the values.
//Parms is the parameters which will be passed to the function passed in Func.
//The algorthm uses a modified Simpson's rule to compute the integral between x1 and x2 by computing the value
//of the function at x1.5.  THis could also be done with trapazoid rule, but the added benefit in accuracy (2
//orders of magnitude) for 2N-1 additional function calls seemed worth it, especially if we happen to get a 
//bumpy function.
{
	int Num = (int) floor((End-Start)/Step) + 1;										//The number of steps
	double F0, F05, F1;
	double dTmp[3] = {0., Parms[0], Parms[1]};
	dTmp[0] = Start;																	//Compute the f(start)
	F0 =  Func(dTmp);
	Result[0] = 0.;																		//By definition
	for (int i=1; i<Num; i++) {
		dTmp[0] = Start + Step * (i-0.5);												//Compute f(half way between the current points)
		F05 = Func(dTmp);
		dTmp[0] = Start + Step * i;														//Compute f(at the right endpoint of the current step)
		F1 = Func(dTmp);

		Result[i] = Result[i-1] + Step/2. * (F0 + 4*F05 + F1) / 3.;						//Compute the next integral step
		F0 = F1;
	}
}


//Variables needed by random number generator (if declared as static at function scope, values do not reset for each consecutive run)
long		g_idum; //Current value for random number generator
long		iy;
long		iv[NTAB];
int			iset;
float		gset;

long					SeedRandom(long seed) 
//This functions sets the random seed for the user defined random number generator.  It takes as an argument, the seed so
//the user has the option of manually specifying the seed.  This is useful for rerunning former execution paths.
{
	//Init for GetNextRandom (if declared as static at function scope, values do not reset for each consecutive run)
	iy=0;
	//Init for GetNextRandomNorm 
	iset=0;
	gset=0.;

	return g_idum = -seed;
}


const float					GetNextRandom() 
//Random number generator from Numerical Recipes in C++ (Sec 7.1, pg 280)
//Minimal" random number generator of Park and Miller with Bays-Durham shuffle and added
//safeguards. Returns a uniform random deviate between 0.0 and 1.0 (exclusive of the endpoint
//values). Call with g_idum a negative integer to initialize; thereafter, do not alter g_idum between
//successive deviates in a sequence. RNMX should approximate the largest floating value that is
//less than 1.
{
	gTallyOfRandCalls++;
	int j;
	long k;
	float temp;

	if (g_idum<=0 || !iy) {						//Initialize
		if (-(g_idum)<1)
			g_idum=1;							//Be sure to prevent g_idum = 0
		else
			g_idum = -(g_idum);
		for (j=NTAB+7;j>=0;j--) {				//Load the shuffle table (after 8 warm-ups)
			k=(g_idum)/IQ;
			g_idum=IA*(g_idum-k*IQ)-IR*k;
			if (g_idum<0) g_idum += IM;
			if (j<NTAB) iv[j] = g_idum;
		}
		iy=iv[0];
	}
	k		= (g_idum)/IQ;						//Start here when not initializing.
	g_idum	= IA*(g_idum-k*IQ)-IR*k;			//Compute g_idum=(IAg_idum) % IM without over-flows by Schrage's method
	if (g_idum<0) g_idum += IM;
	j		= iy/NDIV;							//Will be in the range 0..NTAB-1
	iy		= iv[j];							//Output previously stored value and reill the shuffle table
	iv[j]	= g_idum;
	if ((temp=AM*iy) > RNMX)
		return RNMX;							//Because users don't expect endpoint values
	else
		return temp;
}


const float					GetNextRandomNorm(const double mean, const double stdDev) 
//Random number with normal distribution from Numerical Recipies in C++ (Sec 7.2, pg 388)
//Returns a normally distributed deviate with zero mean and unit variance, using GetNextRandom
//as the source of uniform deviates.  The function has been modified to return a random
//number with mean dMean and standard deviation dStdDev (see the last line).
{
	float fac,rsq,v1,v2,ret;

	if (g_idum < 0) iset=0;						//Reinitialize.
	if (iset == 0) {							//We don't have an extra deviate handy, so ...
		do 
		{
			v1=2.0f*GetNextRandom()-1.0f;					//Pick two uniform numbers in the square extending from -1 to +1 in each direction,
			v2=2.0f*GetNextRandom()-1.0f;
			rsq=v1*v1+v2*v2;					//See if they are in the unit circle...
		} while (rsq >= 1.0f || rsq == 0.0f);		//... and if they are not, try again.
		fac=sqrt(-2.0f*log(rsq)/rsq);			//Now make the Box-Muller transformation to get two normal deviates. Return one and save the other for next time.
		gset=v1*fac;
		iset=1;									//Set flag.
		ret=v2*fac;
	} else {									//We have an extra deviate handy, so unset the flag, and return it.
		iset=0;
		ret=gset; 
	}
	return ret * stdDev + mean;
}
