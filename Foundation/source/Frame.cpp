//Frame.cpp


#include "Fresco/Foundation/PreCompiled.h"
#include "Fresco/Foundation/Frame.h"


Frame::Frame(const int& rAge, 
			 const bool& rIsTopoComplex,
             const float& rSite, 
             const int& rYearOfLastBurn,
			 const int& rLastBurnSeverity,
             const float& rFireIgnitionFactor, 
             const float& rFireSensitivity, 
             const Species& rSpeciesSubCanopy)
    : Fire(rYearOfLastBurn, rLastBurnSeverity, rFireIgnitionFactor, rFireSensitivity), _yearEstablished(rAge), _yearFrameEstablished(rAge), _site(rSite), _isTopoComplex(rIsTopoComplex), _speciesSubCanopy(rSpeciesSubCanopy)
{
}


//Frame::Frame(const Frame &Frame, const int &nAge) : 
Frame::Frame(const Frame &Frame) 
    : Fire(Frame), _yearEstablished(Frame._yearEstablished), _yearFrameEstablished(Frame._yearFrameEstablished), _site(Frame._site), _isTopoComplex(Frame._isTopoComplex),  _speciesSubCanopy(Frame.type())
{ 
}


void Frame::clear()
{
}


std::ostream& operator<< (std::ostream& s, const Frame& f) 
{
	int bFormat = f._outFlags & outFormat;
	if (f._outFlags & outVeg)			{if (!bFormat) s<<"Veg="<<  f.type()                        <<"\t";         else s<<    f.type()                        <<" "; }
	if (f._outFlags & outAge)			{if (!bFormat) s<<"Age="<<  gYear-f._yearEstablished        <<"\t";			else s<<    gYear-f._yearEstablished        <<" "; }
	if (f._outFlags & outSite)		    {if (!bFormat) s<<"Site="<< f._site                         <<"\t";         else s<<    f._site                         <<" "; }
	if (f._outFlags & outSub)			{if (!bFormat) s<<"Sub="<<  f._speciesSubCanopy             <<"\t";         else s<<    f._speciesSubCanopy             <<" "; }
	if (f._outFlags & outFireAge)		{if (!bFormat) s<<"FAge="<< f.yearOfLastBurn                <<"\t";         else s<<    f.yearOfLastBurn                <<" "; }
	if (f._outFlags & outFireScar)	    {if (!bFormat) s<<"FScar=";	s << (f.lastBurnWasOrigin?"-":"") << f.yearOfLastBurn << "." << f.fireScarID;	if (!bFormat) s << "\t"; else s << " ";	}
	if (f._outFlags & outfireSeverity)  {if (!bFormat) s<<"FSev="<<	(f.yearOfLastBurn==gYear ? (int)f.burnSeverity : 0)	<<"\t";	else s<< (f.yearOfLastBurn==gYear ? (int)f.burnSeverity : 0) <<" "; }
	if (f._outFlags & outfireSeverityHist) {if (!bFormat) s<<"FSevHist="<<	(int)f.burnSeverity		<<"\t";			else s<<	(int)f.burnSeverity				<<" "; }
	f.writeData (s);
	return s;
}


void Frame::writeData (std::ostream& s) const 
//A default function for outputing specific frame data.  This function always outputs 0 after
//a NoData label to indicate that there was no output routine specified
{
	if ((_outFlags & out1) || (_outFlags & out2) || (_outFlags & out3) || (_outFlags & out4)) {
	    int formatting = _outFlags & outFormat;
		if (!formatting)
			s << "NoData=" << 0. << "\t";
        else
            s << 0. << " ";
	}
}
